#pragma once

#include "42.h"

#include "42config/YAML/42sc.h"
#include "42config/YAML/42orbit.h"
#include "42config/txt/42sc.h"
#include "42config/txt/42orbit.h"

long DecodeString(char *s);
long LoadTRVfromFile(const char *Path, const char *TrvFileName,
                     const char *ElemLabel, double Time, struct OrbitType *O);
