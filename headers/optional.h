#include "stddef.h"

#define DEFINE_OPTIONAL(T) \
typedef struct { \
    int has_value; \
    T *t; \
} optional_##T; \
\
extern int is_##T##_ok(optional_##T *optional) { \
    return optional->has_value; \
} \
\
extern optional_##T create_##T##_empty() { \
    optional_##T opt = { .has_value = 0, .t = NULL }; \
    return opt; \
} \
\
extern optional_##T create_##T(T* t) { \
    optional_##T opt = { .t = t, .has_value = 1 }; \
    return opt; \
}
