
/*    Copyright 2021 Vladislav Tsendrovskii <vtcendrovskii@gmail.com> */

#include "42config/YAML/yaml_functions.h"
#include <unistd.h>

void yaml_read_vector(struct fy_node *node, const char *field, double *v)
{
    int i;
    node = fy_node_by_path(node, field, -1, 0);
    for (i = 0; i < 3; i++)
    {
        size_t value_len;
        struct fy_node_t *item = fy_node_sequence_get_by_index(node, i);
        const char *value = fy_node_get_scalar(item, &value_len);
        sscanf(value, "%lf", &v[i]);
    }
}

void yaml_read_quaternion(struct fy_node *node, const char *field, double *v)
{
    int i;
    node = fy_node_by_path(node, field, -1, 0);
    for (i = 0; i < 4; i++)
    {
        size_t value_len;
        struct fy_node_t *item = fy_node_sequence_get_by_index(node, i);
        const char *value = fy_node_get_scalar(item, &value_len);
        sscanf(value, "%lf", &v[i]);
    }
}

void yaml_read_angles(struct fy_node *node, const char *field, double *a1, double *a2, double *a3)
{
    node = fy_node_by_path(node, field, -1, 0);
    size_t value_len;
    struct fy_node_t *item;
    const char *value;

    item = fy_node_sequence_get_by_index(node, 0);
    value = fy_node_get_scalar(item, &value_len);
    sscanf(value, "%lf", a1);
    item = fy_node_sequence_get_by_index(node, 1);
    value = fy_node_get_scalar(item, &value_len);
    sscanf(value, "%lf", a2);
    item = fy_node_sequence_get_by_index(node, 2);
    value = fy_node_get_scalar(item, &value_len);
    sscanf(value, "%lf", a3);
}
