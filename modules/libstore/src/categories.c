#include <categories.h>
#include <stdbool.h>

// must be included after all //
#include <jsonz/object.h>

// MAX_CATEGORIES_BUF_SIZE = 4Kb
#define MAX_CATEGORIES_BUF_SIZE (4 * 1024)

static const ngx_str_t status_ok = ngx_string("{\"ok\":true}");
static const ngx_str_t err_id_full = ngx_string("{\"ok\":false,\"error\":\"id full\"}");
static const ngx_str_t err_bad_id = ngx_string("{\"ok\":false,\"error\":\"bad id\"}");
static const ngx_str_t err_bad_name = ngx_string("{\"ok\":false,\"error\":\"bad name\"}");
static const ngx_str_t err_bad_link = ngx_string("{\"ok\":false,\"error\":\"bad link\"}");
static const ngx_str_t err_no_enough_memory = ngx_string("{\"ok\":false,\"error\":\"no enough memory\"}");

static void clearJSONCache(ngx_http_online_store_main_conf_t * restrict conf) {
	ngx_pfree(conf->pool, conf->json.data);
	conf->json.data = NULL;
}

ngx_int_t getCategoriesJSON(ngx_http_request_t * const restrict request, ngx_str_t * const restrict response) {
	ngx_http_online_store_main_conf_t * restrict conf = NULL;
	conf = ngx_http_get_module_main_conf(request, ngx_http_online_store);

	if (conf->json.data == NULL) {
		category_t * restrict category = conf->categories;

		ngx_str_t tmp_buf = {
			.data = ngx_palloc(request->pool, MAX_CATEGORIES_BUF_SIZE),
			.len = 0,
		};

		if (tmp_buf.data == NULL) {
			response[0] = err_no_enough_memory;
			return NGX_HTTP_INTERNAL_SERVER_ERROR;
		}

		// Open JSON Array //
		tmp_buf.data[tmp_buf.len++] = '[';

		if (category != NULL) {
			while (category != NULL) {
				tmp_buf.len += snprintf((char *)tmp_buf.data + tmp_buf.len, MAX_CATEGORIES_BUF_SIZE - tmp_buf.len,
					"{\"id\":%lu,\"name\":\"%s\",\"link\":\"%s\"},", category->id, category->name.data, category->link.data);
				category = category->next;
			}

			// Ignore last char ',' //
			tmp_buf.len--;
		}

		// Close JSON Array //
		tmp_buf.data[tmp_buf.len++] = ']';

		const ngx_str_t main_buf = {
			.data = ngx_palloc(conf->pool, tmp_buf.len),
			.len = tmp_buf.len,
		};

		if (main_buf.data == NULL) {
			response[0] = err_no_enough_memory;
			return NGX_HTTP_INTERNAL_SERVER_ERROR;
		}

		ngx_memcpy(main_buf.data, tmp_buf.data, tmp_buf.len);
		ngx_pfree(request->pool, tmp_buf.data);

		response[0] = main_buf;
		conf->json = main_buf;
	} else {
		response[0] = conf->json;
	}

	return NGX_HTTP_OK;
}

ngx_int_t addCategory(ngx_http_request_t * const restrict request, ngx_str_t * const restrict response, void * const json) {
	ngx_http_online_store_main_conf_t * restrict conf = NULL;
	conf = ngx_http_get_module_main_conf(request, ngx_http_online_store);
	const ngx_int_t new_id = getNewId(conf->id_set, MAX_CATEGORIES_ID);

	if (new_id < NGX_OK) {
		response[0] = err_id_full;
		return NGX_HTTP_FORBIDDEN;
	}

	void * const name_obj = jsonz_dict_get(json, "name");

	if (name_obj == NULL || jsonz_object_get_type(name_obj) != JSONZ_TYPE_STRING) {
		response[0] = err_bad_name;
		return NGX_HTTP_BAD_REQUEST;
	}

	void * const link_obj = jsonz_dict_get(json, "link");

	if (link_obj == NULL || jsonz_object_get_type(link_obj) != JSONZ_TYPE_STRING) {
		response[0] = err_bad_link;
		return NGX_HTTP_BAD_REQUEST;
	}

	category_t * const restrict new_category = ngx_pcalloc(conf->pool, sizeof(new_category[0]));
	category_t * const restrict next_category = conf->categories;

	if (new_category == NULL) {
		response[0] = err_no_enough_memory;
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	conf->categories = new_category;
	new_category->next = next_category;
	new_category->id = new_id;

	if (next_category != NULL) {
		next_category->last = new_category;
	}

	ngx_str_t name = {
		.data = (uint8_t *)jsonz_string_get_str(name_obj),
	};

	name.len = ngx_strlen(name.data);
	new_category->name.data = ngx_palloc(conf->pool, name.len + sizeof('\0'));
	new_category->name.len = name.len;

	if (new_category->name.data == NULL) {
		response[0] = err_no_enough_memory;
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	ngx_str_t link = {
		.data = (uint8_t *)jsonz_string_get_str(link_obj),
	};

	link.len = ngx_strlen(link.data);
	new_category->link.data = ngx_palloc(conf->pool, link.len + sizeof('\0'));
	new_category->link.len = link.len;

	if (new_category->name.data == NULL) {
		response[0] = err_no_enough_memory;
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	ngx_memcpy(new_category->name.data, name.data, name.len + sizeof('\0'));
	ngx_memcpy(new_category->link.data, link.data, link.len + sizeof('\0'));

	SET_BIT(conf->id_set, new_id);
	clearJSONCache(conf);

	response[0] = status_ok;
	return NGX_HTTP_OK;
}

ngx_int_t delCategory(ngx_http_request_t * const restrict request, ngx_str_t * const restrict response, void * const json) {
	ngx_http_online_store_main_conf_t * restrict conf = NULL;
	conf = ngx_http_get_module_main_conf(request, ngx_http_online_store);

	void * const id_obj = jsonz_dict_get(json, "id");

	if (id_obj == NULL || jsonz_object_get_type(id_obj) != JSONZ_TYPE_INT) {
		response[0] = err_bad_id;
		return NGX_HTTP_BAD_REQUEST;
	}

	const char * const id_str = jsonz_string_get_str(id_obj);
	const ngx_int_t id = atoi(id_str);

	for (category_t * restrict category = conf->categories; category != NULL; category = category->next) {
		if (category->id == id) {
			category_t * restrict last_category = category->last;
			category_t * restrict next_category = category->next;

			if (last_category != NULL) {
				last_category->next = next_category;
			} else {
				conf->categories = next_category;
			}

			if (next_category != NULL) {
				next_category->last = last_category;
			}

			ngx_pfree(conf->pool, category->name.data);
			ngx_pfree(conf->pool, category->link.data);
			ngx_pfree(conf->pool, category);

			UNSET_BIT(conf->id_set, id);
			clearJSONCache(conf);

			response[0] = status_ok;
			return NGX_HTTP_OK;
		}
	}

	response[0] = status_ok;
	return NGX_HTTP_OK;
}

ngx_int_t editCategory(ngx_http_request_t * const restrict request, ngx_str_t * const restrict response, void * const json) {
	ngx_http_online_store_main_conf_t * restrict conf = NULL;
	conf = ngx_http_get_module_main_conf(request, ngx_http_online_store);

	void * const id_obj = jsonz_dict_get(json, "id");

	if (id_obj == NULL || jsonz_object_get_type(id_obj) != JSONZ_TYPE_INT) {
		response[0] = err_bad_id;
		return NGX_HTTP_BAD_REQUEST;
	}

	void * const name_obj = jsonz_dict_get(json, "name");

	if (name_obj == NULL || jsonz_object_get_type(name_obj) != JSONZ_TYPE_STRING) {
		response[0] = err_bad_name;
		return NGX_HTTP_BAD_REQUEST;
	}

	void * const link_obj = jsonz_dict_get(json, "link");

	if (link_obj == NULL || jsonz_object_get_type(link_obj) != JSONZ_TYPE_STRING) {
		response[0] = err_bad_link;
		return NGX_HTTP_BAD_REQUEST;
	}

	const ngx_int_t id = jsonz_number_get_int_value(id_obj);
	for (category_t * restrict category = conf->categories; category != NULL; category = category->next) {
		if (category->id == id) {
			ngx_str_t name = {
				.data = (uint8_t *)jsonz_string_get_str(name_obj),
			};

			if (ngx_strcmp(name.data, category->name.data) != NGX_OK) {
				ngx_pfree(conf->pool, category->name.data);
				name.len = ngx_strlen(name.data);

				category->link.data = ngx_palloc(conf->pool, name.len + sizeof('\0'));
				category->link.len = name.len;

				ngx_memcpy(category->name.data, name.data, name.len + sizeof('\0'));
			}

			ngx_str_t link = {
				.data = (uint8_t *)jsonz_string_get_str(link_obj),
			};

			if (ngx_strcmp(link.data, category->link.data) != NGX_OK) {
				ngx_pfree(conf->pool, category->link.data);
				link.len = ngx_strlen(link.data);

				category->link.data = ngx_palloc(conf->pool, link.len + sizeof('\0'));
				category->link.len = link.len;

				ngx_memcpy(category->link.data, link.data, link.len + sizeof('\0'));
			}

			// Clear JSON Cache //
			clearJSONCache(conf);

			response[0] = status_ok;
			return NGX_HTTP_OK;
		}
	}

	response[0] = err_bad_id;
	return NGX_HTTP_BAD_REQUEST;
}
