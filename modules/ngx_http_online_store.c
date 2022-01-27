#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <jsonz/jsonz.h>
#include <stdbool.h>
#include <store.h>

#define ERR_CONF_IS_DUPLICATE "is duplicate"
#define ERR_CONF_NOT_IMPLEMENTED "not implemented"

typedef char *
(* def_cmd_cb_t)(ngx_conf_t *, ngx_command_t *, void *);
static char *
ngx_http_online_store_access_cmd(ngx_conf_t * const ngx_conf, ngx_command_t * const cmd, uint8_t * const conf);
static void *
ngx_http_online_store_main_conf(ngx_conf_t * const restrict ngx_conf);
static void *
ngx_http_online_store_loc_conf(ngx_conf_t * const restrict ngx_conf);

const ngx_str_t application_json_str = ngx_string("application/json");
// const ngx_str_t multipart_form_data_str = ngx_string("multipart/form-data");

static ngx_command_t ngx_http_online_store_commands[] = {
	{
		.name	= ngx_string("online_store_access"),
		.type	= NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
		.set	= (def_cmd_cb_t)ngx_http_online_store_access_cmd,
		.conf	= NGX_HTTP_LOC_CONF_OFFSET,
		.offset	= offsetof(ngx_http_online_store_loc_conf_t, access_type),
		.post	= NULL,
	},
	{
		.name	= ngx_string("online_store_data"),
		.type	= NGX_HTTP_MAIN_CONF | NGX_CONF_TAKE1,
		.set	= ngx_conf_set_str_slot,
		.conf	= NGX_HTTP_MAIN_CONF_OFFSET,
		.offset	= offsetof(ngx_http_online_store_main_conf_t, path),
		.post	= NULL,
	},
	ngx_null_command,
};

static ngx_http_module_t ngx_http_online_store_ctx = {
	.preconfiguration	= NULL,
	.postconfiguration	= NULL,
	.create_main_conf	= ngx_http_online_store_main_conf,
	.init_main_conf		= NULL,
	.create_srv_conf	= NULL,
	.merge_srv_conf		= NULL,
	.create_loc_conf	= ngx_http_online_store_loc_conf,
	.merge_loc_conf		= NULL,
};

ngx_module_t ngx_http_online_store = {
	NGX_MODULE_V1,
	.ctx			= &ngx_http_online_store_ctx,
	.commands		= ngx_http_online_store_commands,
	.type			= NGX_HTTP_MODULE,
	.init_master	= NULL,
	.init_module	= NULL,
	.init_process	= NULL,
	.init_thread	= NULL,
	.exit_thread	= NULL,
	.exit_process	= NULL,
	.exit_master	= NULL,
	NGX_MODULE_V1_PADDING
};

static void
ngx_http_online_store_json_handler(ngx_http_request_t * const restrict request)
{
	ngx_http_request_body_t * const restrict request_body = request->request_body;
	request->read_event_handler = ngx_http_online_store_json_handler;

	if (request_body == NULL) {
		ngx_http_finalize_request(request, NGX_HTTP_INTERNAL_SERVER_ERROR);
		return;
	}

	ngx_http_online_store_request_json_t * restrict request_ctx = NULL;
	request_ctx = ngx_http_get_module_ctx(request, ngx_http_online_store);

	ngx_chain_t * restrict chain_in = request_body->bufs;
	const ngx_int_t content_length = request->headers_in.content_length_n;

	while (chain_in != NULL) {
		const ngx_buf_t * const restrict buf = chain_in->buf;
		const ngx_int_t size = buf->last - buf->pos;

		if (request_ctx->input.len + size > (size_t)content_length) {
			ngx_http_finalize_request(request, NGX_HTTP_REQUEST_URI_TOO_LARGE);
			return;
		}

		ngx_memcpy(request_ctx->input.data + request_ctx->input.len, buf->pos, size);
		request_ctx->input.len += size;
		chain_in = chain_in->next;
	}

	if (request_ctx->input.len >= (size_t)content_length) {
		ngx_chain_t chain_out = {
			.buf	= NULL,
			.next	= NULL,
		};

		ngx_int_t response_code = NGX_OK;
		((char *)request_ctx->input.data)[content_length] = '\0';

		response_code = store_json_request_handler(request, &chain_out);
		jsonz_object_free(request_ctx->json);

		if (response_code != NGX_OK) {
			ngx_http_finalize_request(request, response_code);
			return;
		}

		response_code = ngx_http_send_header(request);

		if (response_code != NGX_OK || request->header_only != false) {
			ngx_http_finalize_request(request, response_code);
			return;
		}

		chain_out.buf->memory = true;
		chain_out.buf->last_buf = true;

		response_code = ngx_http_output_filter(request, &chain_out);
		ngx_http_finalize_request(request, response_code);
	}
}

static ngx_int_t
ngx_http_online_store_handler(ngx_http_request_t * const restrict request)
{
	if ((request->method & NGX_HTTP_POST) == false) {
		return NGX_HTTP_NOT_ALLOWED;
	}

	ngx_int_t content_length = request->headers_in.content_length_n;

	if (content_length <= 0) {
		return NGX_HTTP_LENGTH_REQUIRED;
	}

	ngx_table_elt_t * restrict content_type = request->headers_in.content_type;

	if (content_type == NULL) {
		return NGX_HTTP_BAD_REQUEST;
	}

	ngx_http_online_store_request_json_t * restrict request_ctx = NULL;
	request_ctx = ngx_http_get_module_ctx(request, ngx_http_online_store);

	if (request_ctx == NULL) {
		request_ctx = ngx_pcalloc(request->pool, sizeof(request_ctx[0]));

		if (request_ctx == NULL) {
			return NGX_HTTP_INTERNAL_SERVER_ERROR;
		}

		request_ctx->input.data = ngx_palloc(request->pool, content_length + sizeof('\0'));
		request_ctx->input.len = 0;

		if (request_ctx->input.data == NULL) {
			return NGX_HTTP_INTERNAL_SERVER_ERROR;
		}

		ngx_http_set_ctx(request, request_ctx, ngx_http_online_store);
	}

	if (ngx_strcmp(content_type->value.data, application_json_str.data) == NGX_OK) {
		const ngx_int_t response_code = ngx_http_read_client_request_body(request, ngx_http_online_store_json_handler);

		if (response_code >= NGX_HTTP_SPECIAL_RESPONSE) {
			return response_code;
		}

		return NGX_DONE;
	} else {
		return NGX_HTTP_UNSUPPORTED_MEDIA_TYPE;
	}
}

static char *
ngx_http_online_store_access_cmd(ngx_conf_t * const restrict ngx_conf, ngx_command_t * const restrict cmd, uint8_t * const conf)
{
	access_type_t * const restrict access_type_ptr = (access_type_t *)(conf + cmd->offset);

	if (access_type_ptr[0] != ACCESS_TYPE_NONE) {
		return ERR_CONF_IS_DUPLICATE;
	}

	const ngx_str_t * const restrict args = ngx_conf->args->elts;
	const ngx_str_t arg = args[1];

	if (ngx_strcmp(arg.data, "admin") == NGX_OK) {
		access_type_ptr[0] = ACCESS_TYPE_ADMIN;
	} else if (ngx_strcmp(arg.data, "public") == NGX_OK) {
		access_type_ptr[0] = ACCESS_TYPE_PUBLIC;
	} else {
		return ERR_CONF_NOT_IMPLEMENTED;
	}

	ngx_http_core_loc_conf_t * restrict core_loc_conf = NULL;

	core_loc_conf = ngx_http_conf_get_module_loc_conf(ngx_conf, ngx_http_core_module);
	core_loc_conf->handler = ngx_http_online_store_handler;

	return NGX_CONF_OK;
}

static void *
ngx_http_online_store_main_conf(ngx_conf_t * const restrict ngx_conf)
{
	ngx_http_online_store_main_conf_t * restrict conf = NULL;
	conf = ngx_pcalloc(ngx_conf->pool, sizeof(conf[0]));

	if (conf == NULL) {
		return NGX_CONF_ERROR;
	}

	conf->pool = ngx_conf->pool;
	return conf;
}

static void *
ngx_http_online_store_loc_conf(ngx_conf_t * const restrict ngx_conf)
{
	ngx_http_online_store_loc_conf_t * restrict conf = NULL;
	conf = ngx_pcalloc(ngx_conf->pool, sizeof(conf[0]));

	if (conf == NULL) {
		return NGX_CONF_ERROR;
	}

	conf->access_type = ACCESS_TYPE_NONE;
	return conf;
}
