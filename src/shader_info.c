#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <assert.h>
#include <json_object.h>

#include "json-c/json.h"

#include "shader_info.h"

static char* read_all_file(FILE *file) {
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    rewind(file);

    char *buffer = malloc(length+1);
    if (!buffer) {
        return NULL;
    }

    size_t read = fread(buffer, 1, length, file);
    buffer[read] = '\0';
    return buffer;
}

static optional_shaders_array_t parsing_json_file(struct json_object *root) {
    shaders_array_t shaders_array;
    size_t length = json_object_array_length(root);
    resize_shaders_array(&shaders_array, length);
    for (int i = 0; i < length; i++) {
        struct json_object *obj = json_object_array_get_idx(root, i);

        const char *shader_file = json_object_get_string(json_object_object_get(obj, "shader_file"));
        const char *display_name = json_object_get_string(json_object_object_get(obj, "display_name"));

        json_object_put(obj);
    }
}

extern optional_shaders_array_t create_shaders_array_from_json_file(const char *file_name) {
    FILE *file = NULL;
    char *file_content;
    struct json_object *root;

    optional_shaders_array_t optional_shaders_array = create_shaders_array_t_empty();

    if ((file = fopen(file_name, "r"))
        && (file_content = read_all_file(file))
        && (root = json_tokener_parse(file_content))) {

        optional_shaders_array = parsing_json_file(root);
    }

    if (file) {
        fclose(file);
    }

    if (file_content) {
        free(file_content);
    }

    if (root) {
        json_object_put(root);
    }

    return optional_shaders_array;
}

extern void resize_shaders_array(shaders_array_t *this, size_t size) {
    assert(size != 0);
    this->shader_info = (shader_info_t*)realloc(this->shader_info, sizeof(shader_info_t) * size);
    this->size = size;
    this->cap = size*1.5;
}

extern void free_shaders_array(shaders_array_t* this) {
    free(this->shader_info);
}


extern void add_shaders_array(shaders_array_t* this, shader_info_t item) {
    if (this->size >= this->cap) {
        resize_shaders_array(this, this->size+1);
    }

    this->shader_info[this->size+1] = item;
    this->size += 1;
    this->cap += 1;
}
