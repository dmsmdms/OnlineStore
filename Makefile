.PHONY : all clean build_clean
.DEFAULT_GOAL := all

ADMIN_DIR := admin
CONFIG_DIR := config
DATA_DIR := data
MODULES_DIR := modules
PUBLIC_DIR := public

NGX_DIR := $(PWD)/nginx
NGX_BIN := $(NGX_DIR)/objs/nginx
NGX_PREFIX_DIR := $(PWD)/build
NGX_SBIN_PATH := $(NGX_PREFIX_DIR)/nginx
NGX_ADMIN_DIR := $(NGX_PREFIX_DIR)/admin
NGX_CONFIG_DIR := $(NGX_PREFIX_DIR)/config
NGX_DATA_DIR := $(NGX_PREFIX_DIR)/data
NGX_PUBLIC_DIR := $(NGX_PREFIX_DIR)/public
NGX_LOG_DIR := $(NGX_PREFIX_DIR)/log
NGX_NAME := OnlineStore

NGX_HTTP_ACCESS_LOG_FILE := /dev/null
NGX_HTTP_TMP := /tmp/nginx/

NGX_CONFIGURE := $(NGX_CONFIGURE) --prefix=$(NGX_PREFIX_DIR)
NGX_CONFIGURE := $(NGX_CONFIGURE) --sbin-path=$(NGX_SBIN_PATH)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --modules-path=$(NGX_MODULES_DIR)
NGX_CONFIGURE := $(NGX_CONFIGURE) --conf-path=$(NGX_CONFIG_DIR)/nginx.conf
NGX_CONFIGURE := $(NGX_CONFIGURE) --error-log-path=$(NGX_LOG_DIR)/nginx.log
NGX_CONFIGURE := $(NGX_CONFIGURE) --pid-path=/tmp/nginx.pid
NGX_CONFIGURE := $(NGX_CONFIGURE) --lock-path=/tmp/nginx.lock
# NGX_CONFIGURE := $(NGX_CONFIGURE) --user=$(NGX_USER)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --group=$(NGX_GROUP)
NGX_CONFIGURE := $(NGX_CONFIGURE) --build=$(NGX_NAME)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --builddir=$(NGX_BUILD_DIR)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-select_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-select_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-poll_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-poll_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-threads
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-file-aio
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_ssl_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_v2_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_realip_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_addition_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_xslt_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_xslt_module=dynamic
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_image_filter_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_image_filter_module=dynamic
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_geoip_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_geoip_module=dynamic
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_sub_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_dav_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_flv_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_mp4_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_gunzip_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_gzip_static_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_auth_request_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_random_index_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_secure_link_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_degradation_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_slice_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_stub_status_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_charset_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_gzip_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_ssi_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_userid_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_access_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_auth_basic_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_mirror_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_autoindex_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_geo_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_map_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_split_clients_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_referer_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_rewrite_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_proxy_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_fastcgi_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_uwsgi_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_scgi_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_grpc_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_memcached_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_limit_conn_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_limit_req_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_empty_gif_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_browser_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_upstream_hash_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_upstream_ip_hash_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_upstream_least_conn_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_upstream_random_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_upstream_keepalive_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http_upstream_zone_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_perl_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-http_perl_module=dynamic
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-perl_modules_path=PATH
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-perl=PATH
NGX_CONFIGURE := $(NGX_CONFIGURE) --http-log-path=$(NGX_HTTP_ACCESS_LOG_FILE)
NGX_CONFIGURE := $(NGX_CONFIGURE) --http-client-body-temp-path=$(NGX_HTTP_TMP)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --http-proxy-temp-path=$(NGX_HTTP_TMP)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --http-fastcgi-temp-path=$(NGX_HTTP_TMP)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --http-uwsgi-temp-path=$(NGX_HTTP_TMP)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --http-scgi-temp-path=$(NGX_HTTP_TMP)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-http-cache
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-mail
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-mail=dynamic
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-mail_ssl_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-mail_pop3_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-mail_imap_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-mail_smtp_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-stream
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-stream=dynamic
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-stream_ssl_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-stream_realip_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-stream_geoip_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-stream_geoip_module=dynamic
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-stream_ssl_preread_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_limit_conn_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_access_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_geo_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_map_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_split_clients_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_return_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_set_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_upstream_hash_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_upstream_least_conn_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_upstream_random_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-stream_upstream_zone_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-google_perftools_module
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-cpp_test_module
NGX_CONFIGURE := $(NGX_CONFIGURE) --add-module=$(PWD)/$(MODULES_DIR)
# NGX_CONFIGURE := $(NGX_CONFIGURE) --add-dynamic-module=PATH
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-compat
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-cc=PATH
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-cpp=PATH
NGX_CONFIGURE := $(NGX_CONFIGURE) --with-cc-opt="-g0 -O3 -march=native -flto"
NGX_CONFIGURE := $(NGX_CONFIGURE) --with-ld-opt="--lto"
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-cpu-opt=CPU # CPU list: pentium, pentiumpro, pentium3, pentium4, athlon, opteron, sparc32, sparc64, ppc64
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-pcre
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-pcre
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-pcre=DIR
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-pcre-opt=OPTIONS
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-pcre-jit
NGX_CONFIGURE := $(NGX_CONFIGURE) --without-pcre2
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-zlib=DIR
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-zlib-opt=OPTIONS
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-zlib-asm=CPU # CPU list: pentium, pentiumpro
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-libatomic
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-libatomic=DIR
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-openssl=DIR
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-openssl-opt=OPTIONS
# NGX_CONFIGURE := $(NGX_CONFIGURE) --with-debug

HTML_FILE_TYPES := html css js png
CONF_FILE_TYPES := conf
DATA_FILE_TYPES :=
MODS_FILE_TYPES := c h

ADMIN_DIRS := $(shell find $(ADMIN_DIR) -type d)
ADMIN_SRC := $(foreach type, $(HTML_FILE_TYPES), $(foreach dir, $(ADMIN_DIRS), $(wildcard $(dir)/*.$(type))))
ADMIN_OUT := $(patsubst $(ADMIN_DIR)/%, $(NGX_ADMIN_DIR)/%, $(ADMIN_SRC))

CONFIG_DIRS := $(shell find $(CONFIG_DIR) -type d)
CONFIG_SRC := $(foreach type, $(CONF_FILE_TYPES), $(foreach dir, $(CONFIG_DIRS), $(wildcard $(dir)/*.$(type))))
CONFIG_OUT := $(patsubst $(CONFIG_DIR)/%, $(NGX_CONFIG_DIR)/%, $(CONFIG_SRC))

DATA_DIRS := $(shell find $(DATA_DIR) -type d)
DATA_SRC := $(foreach type, $(DATA_FILE_TYPES), $(foreach dir, $(DATA_DIRS), $(wildcard $(dir)/*.$(type))))
DATA_OUT := $(patsubst $(DATA_DIR)/%, $(NGX_DATA_DIR)/%, $(DATA_SRC))

MODULES_DIRS := $(shell find $(MODULES_DIR) -type d)
MODULES_SRC := $(foreach type, $(MODS_FILE_TYPES), $(foreach dir, $(MODULES_DIRS), $(wildcard $(dir)/*.$(type))))

PUBLIC_DIRS := $(shell find $(PUBLIC_DIR) -type d)
PUBLIC_SRC := $(foreach type, $(HTML_FILE_TYPES), $(foreach dir, $(PUBLIC_DIRS), $(wildcard $(dir)/*.$(type))))
PUBLIC_OUT := $(patsubst $(PUBLIC_DIR)/%, $(NGX_PUBLIC_DIR)/%, $(PUBLIC_SRC))

$(NGX_SBIN_PATH): $(NGX_BIN)
	@ mkdir -p $(NGX_LOG_DIR)
	@ cp $(NGX_BIN) $(NGX_SBIN_PATH)

$(NGX_BIN): $(NGX_DIR)/Makefile $(MODULES_SRC)
	@ $(MAKE) -C $(NGX_DIR)

$(NGX_DIR)/Makefile: Makefile $(MODULES_DIR)/config
	@ cd $(NGX_DIR) && ./auto/configure $(NGX_CONFIGURE)

$(NGX_ADMIN_DIR)/%: $(ADMIN_DIR)/%
	@ mkdir -p $(@D)
	@ cp $< $@

$(NGX_CONFIG_DIR)/%: $(CONFIG_DIR)/%
	@ mkdir -p $(@D)
	@ cp $< $@

$(NGX_DATA_DIR)/%: $(DATA_DIR)/%
	@ mkdir -p $(@D)
	@ cp $< $@

$(NGX_PUBLIC_DIR)/%: $(PUBLIC_DIR)/%
	@ mkdir -p $(@D)
	@ cp $< $@

build_clean:
	@ rm -rf $(NGX_PREFIX_DIR)

clean: build_clean
	@ $(MAKE) -C $(NGX_DIR) clean

all: $(NGX_SBIN_PATH) $(ADMIN_OUT) $(CONFIG_OUT) $(DATA_OUT) $(PUBLIC_OUT)
