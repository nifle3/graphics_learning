#ifndef __SHADERS_METADATA_H__
#define __SHADERS_METADATA_H__

#include "stdlib.h"

#include "json-c/json.h"

typedef struct {
    char *shader_name;
    char *display_name;
    struct {
        char *fragment_shader_file;
        char *vertex_shader_file;
    } files;
} shaders_metadata_t;

typedef struct {
    size_t size;
    shaders_metadata_t *item;
} shaders_metadata_list_t;

extern shaders_metadata_list_t* shaders_metadata_list_load_from_json(const char*);
extern shaders_metadata_list_t* shaders_metadata_list_parse_from_json(struct json_object*);

extern void shaders_metadata_list_free(shaders_metadata_list_t*);
extern shaders_metadata_list_t* shaders_metadata_list_new(size_t size);

#endif
