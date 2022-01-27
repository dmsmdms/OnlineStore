#include <categories.h>
#include <stdbool.h>
#include <store.h>

// must be included after all //
#include <jsonz/object.h>
#include <jsonz/jsonz.h>

static const ngx_str_t getCategories_cmd = ngx_string("getCategories");
static const ngx_str_t editCategory_cmd = ngx_string("editCategory");
static const ngx_str_t addCategory_cmd = ngx_string("addCategory");
static const ngx_str_t delCategory_cmd = ngx_string("delCategory");

ngx_int_t store_json_request_handler(ngx_http_request_t * const restrict request, ngx_chain_t * const restrict chain_out) {
	ngx_http_online_store_request_json_t * restrict request_ctx = NULL;
	request_ctx = ngx_http_get_module_ctx(request, ngx_http_online_store);

	void * const json_request = jsonz_parse((char *)request_ctx->input.data);
	request_ctx->json = json_request;

	if (json_request == NULL || jsonz_object_get_type(json_request) != JSONZ_TYPE_DICT) {
		return NGX_HTTP_BAD_REQUEST;
	}

	void * const cmd_obj = jsonz_dict_get(json_request, "cmd");

	if (cmd_obj == NULL || jsonz_object_get_type(cmd_obj) != JSONZ_TYPE_STRING) {
		return NGX_HTTP_BAD_REQUEST;
	}

	void * const data_obj = jsonz_dict_get(json_request, "data");

	if (data_obj == NULL || jsonz_object_get_type(data_obj) != JSONZ_TYPE_DICT) {
		return NGX_HTTP_BAD_REQUEST;
	}

	ngx_http_online_store_loc_conf_t * restrict conf = NULL;
	conf = ngx_http_get_module_loc_conf(request, ngx_http_online_store);

	ngx_int_t response_code = NGX_HTTP_OK;
	ngx_str_t response_str = ngx_string(NULL);
	const char * const cmd_str = jsonz_string_get_str(cmd_obj);

	if (conf->access_type == ACCESS_TYPE_ADMIN) {
		if (ngx_strcmp(cmd_str, getCategories_cmd.data) == NGX_OK) {
			response_code = getCategoriesJSON(request, &response_str);
		} else if (ngx_strcmp(cmd_str, editCategory_cmd.data) == NGX_OK) {
			response_code = editCategory(request, &response_str, data_obj);
		} else if (ngx_strcmp(cmd_str, addCategory_cmd.data) == NGX_OK) {
			response_code = addCategory(request, &response_str, data_obj);
		} else if (ngx_strcmp(cmd_str, delCategory_cmd.data) == NGX_OK) {
			response_code = delCategory(request, &response_str, data_obj);
		} else {
			return NGX_HTTP_FORBIDDEN;
		}
	} else if (conf->access_type == ACCESS_TYPE_PUBLIC) {
		if (ngx_strcmp(cmd_str, getCategories_cmd.data) == NGX_OK) {
			response_code = getCategoriesJSON(request, &response_str);
		} else {
			return NGX_HTTP_FORBIDDEN;
		}
	} else {
		return NGX_HTTP_FORBIDDEN;
	}

	ngx_buf_t * const restrict buf = ngx_pcalloc(request->pool, sizeof(buf[0]));

	if (buf == NULL) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	buf->pos = response_str.data;
	buf->last = response_str.data + response_str.len;

	chain_out->buf = buf;
	buf->last_buf = true;
	buf->memory = true;

	request->headers_out.status = response_code;
	request->headers_out.content_type = application_json_str;
	request->headers_out.content_length_n = response_str.len;

	return NGX_OK;
}

ngx_int_t getNewId(const bit_item_t * const restrict id_set, const ngx_int_t id_set_size) {
	for (ngx_int_t i = 1; i < id_set_size; ++i) {
		if (IS_BIT_SET(id_set, i) == false) {
			return i;
		}
	}

	return NGX_ERROR;
}
