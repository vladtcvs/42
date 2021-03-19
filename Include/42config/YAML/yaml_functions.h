/*    Copyright 2021 Vladislav Tsendrovskii <vtcendrovskii@gmail.com> */

#pragma once

#include <libfyaml.h>

void yaml_read_vector(struct fy_node *node, const char *field, double *v);
void yaml_read_quaternion(struct fy_node *node, const char *field, double *v);
void yaml_read_angles(struct fy_node *node, const char *field, double *a1, double *a2, double *a3);

#define yaml_read_3_vals(node, field, v1, v2, v3) yaml_read_angles(node, field, v1, v2, v3)
