#include "assert.h"

#include "shaders_metadata.h"

#include "json-c/json.h"
#include "gtk/gtk.h"

extern shaders_metadata_list_t* shaders_metadata_list_load_from_json(const char *file_name) {
    assert(file_name != NULL);

    shaders_metadata_list_t *list = NULL;
    struct json_object *root = json_object_from_file(file_name);

    if (root) {
        list = shaders_metadata_list_parse_from_json(root);
        json_object_put(root);
    }

    return list;
}

extern shaders_metadata_list_t* shaders_metadata_list_parse_from_json(struct json_object *root) {
    assert(root != NULL);

    size_t length = json_object_array_length(root);
    shaders_metadata_list_t *list = shaders_metadata_list_new(length);

    for (int i = 0; i < length; i++) {
        struct json_object *object = json_object_array_get_idx(root, i);
        if (!object) {
            continue;
        }

        struct json_object *shader_name_obj = json_object_object_get(object, "shader_name");
        struct json_object *display_name_obj = json_object_object_get(object, "display_name");
        struct json_object *shader_files = json_object_object_get(object, "shader_files");

        if (!shader_name_obj || !display_name_obj || !shader_files) {
            continue;
        }

        struct json_object *vertex_obj = json_object_object_get(shader_files, "vertex");
        struct json_object *fragment_obj = json_object_object_get(shader_files, "fragment");

        if (!vertex_obj || !fragment_obj) {
            continue;
        }

        const char *shader_name_raw = json_object_get_string(shader_name_obj);
        const char *display_name_raw = json_object_get_string(display_name_obj);
        const char *vertex_shader_raw = json_object_get_string(vertex_obj);
        const char *fragment_shader_raw = json_object_get_string(fragment_obj);

        if (!g_utf8_validate(shader_name_raw, -1, NULL) ||
            !g_utf8_validate(display_name_raw, -1, NULL) ||
            !g_utf8_validate(vertex_shader_raw, -1, NULL) ||
            !g_utf8_validate(fragment_shader_raw, -1, NULL)) {

            g_warning("⚠️ Пропущен невалидный UTF-8 в элементе %d", i);
            continue;
        }

        shaders_metadata_t shader_metadata = {
            .shader_name = g_strdup(shader_name_raw),
            .display_name = g_strdup(display_name_raw),
            .files = {
                .fragment_shader_file = g_strdup(fragment_shader_raw),
                .vertex_shader_file = g_strdup(vertex_shader_raw)
            }
        };

        list->item[i] = shader_metadata;
    }

    return list;
}

extern void shaders_metadata_list_free(shaders_metadata_list_t *this) {
    assert(this != NULL);

    free(this->item);
}

extern shaders_metadata_list_t* shaders_metadata_list_new(size_t size) {
    assert(size > 0);

    shaders_metadata_list_t* this = (shaders_metadata_list_t*)malloc(sizeof(shaders_metadata_list_t));
    this->size = size;
    this->item = (shaders_metadata_t*)malloc(sizeof(shaders_metadata_t) * size);
    this->selected_idx = 0;
    return this;
}

extern void shaders_metadata_list_set_selected_idx(shaders_metadata_list_t* this, size_t idx) {
    assert(this->size >= idx);

    this->selected_idx = idx;
}
