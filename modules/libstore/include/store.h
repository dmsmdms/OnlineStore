#pragma once

#include <ngx_core.h>
#include <ngx_http.h>
#include <bitset.h>

#define UNUSED __attribute__((unused))
#define PACKED __attribute__((packed))

#define MAX_CATEGORIES_ID 99
#define MAX_SUBCATEGORIES_ID 99
#define MAX_PRODUCTS_ID 999

typedef struct category_s {
	struct category_s *	next;
	struct category_s *	last;
	ngx_str_t 			name;
	ngx_str_t 			link;
	ngx_int_t			id;
} category_t;

typedef struct {
	category_t *	categories;
	ngx_pool_t *	pool;
	ngx_str_t		json;
	ngx_str_t		path;
	bit_item_t		id_set[GET_BIT_SET_SIZE(MAX_CATEGORIES_ID)];
} ngx_http_online_store_main_conf_t;

typedef enum {
	ACCESS_TYPE_NONE,
	ACCESS_TYPE_ADMIN,
	ACCESS_TYPE_PUBLIC,
} access_type_t;

typedef struct {
	access_type_t access_type;
} ngx_http_online_store_loc_conf_t;

typedef struct {
	ngx_str_t	input;
	void *		json;
} ngx_http_online_store_request_json_t;

extern const ngx_str_t application_json_str;
extern ngx_module_t ngx_http_online_store;

ngx_int_t store_json_request_handler(ngx_http_request_t * const restrict request, ngx_chain_t * const restrict chain_out);
ngx_int_t getNewId(const bit_item_t * const restrict id_set, const ngx_int_t id_set_size);
