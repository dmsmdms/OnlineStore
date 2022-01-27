#pragma once

#include <store.h>

ngx_int_t getCategoriesJSON(ngx_http_request_t * const restrict request, ngx_str_t * const restrict response);
ngx_int_t addCategory(ngx_http_request_t * const restrict request, ngx_str_t * const restrict response, void * const json);
ngx_int_t delCategory(ngx_http_request_t * const restrict request, ngx_str_t * const restrict response, void * const json);
ngx_int_t editCategory(ngx_http_request_t * const restrict request, ngx_str_t * const restrict response, void * const json);
