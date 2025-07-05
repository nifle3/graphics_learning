#include "stdio.h"

#include "optional.h"

typedef struct {
    char *shader_file;
    char *display_name;
} shader_info_t;

typedef struct {
    size_t size;
    size_t cap;
    shader_info_t *shader_info;
} shaders_array_t;

DEFINE_OPTIONAL(shaders_array_t);

extern optional_shaders_array_t create_shaders_array_from_json_file(const char*);

extern void free_shaders_array(shaders_array_t*);

extern void resize_shaders_array(shaders_array_t*, size_t);

extern void add_shaders_array(shaders_array_t*, shader_info_t);
