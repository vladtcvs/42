
/*    Copyright 2021 Vladislav Tsendrovskii <vtcendrovskii@gmail.com> */

#pragma once

#include "42.h"
#include <libfyaml.h>

void read_wheel(struct fy_node *wheel, struct WhlType *whl);
void read_mtb(struct fy_node *mtb, struct MTBType *m);
void read_thruster(struct fy_node *thr, struct ThrType *t);
void read_gyro(struct fy_node *gyro, struct GyroType *Gyro);
void read_magnetometer(struct fy_node *mag, struct MagnetometerType *MAG);
void read_sun_sensor(struct fy_node *css, struct CssType *CSS);
void read_fine_sun_sensor(struct fy_node *fss, struct FssType *FSS);
void read_star_tracker(struct fy_node *st, struct StarTrackerType *ST);
void read_gps(struct fy_node *gps, struct GpsType *GPS);
void read_accelerometer(struct fy_node *acc, struct AccelType *Accel);

void InitSpacecraftYAML(struct SCType *S, double *PosVec, double *VelVec, long *UseCM);
