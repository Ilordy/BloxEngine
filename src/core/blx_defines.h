#pragma once
#include "cglm/cglm.h"
#include <stdio.h>
#define BLX_TRUE 1
#define BLX_FALSE 0
#define Vec3_Up (vec3){0.0f, 1.0f, 0.0f}
#define Vec3_Forward (vec3){0.0f, 0.0f, -1.0f} //Negated due to openGL being a right handed coord system. 
#define Vec3_Right (vec3){-1.0f, 0.0f, 0.0f} //Negated due to openGL being a right handed coord system. 
#define PRINTVEC3(vec) printf("(%f, %f, %f)\n", vec[0], vec[1], vec[2])
#define PRINTVEC3S(vec) printf("(%f, %f, %f)\n", vec.x, vec.y, vec.z)
#define PRINTVEC2(vec) printf("(%f, %f)\n", vec[0], vec[1])
#define PRINTVEC2S(vec) printf("(%f, %f)\n", vec.x, vec.y)
typedef unsigned char blxBool;
typedef unsigned long long uint64;

#ifdef BLXEXPORT
#ifdef _MSC_VER
#define BLXAPI __declspec(dllexport)
#else
#define BLXAPI __attribute__((visibility("default")))
#endif
#else

#ifdef _MSC_VER
#define BLXAPI __declspec(dllimport)
#else
#define BLXAPI
#endif
#endif

#ifdef _MSC_VER
#define WIN32
#define BLXWIN32
#endif
#ifdef _GNUC
#define LINUX
#endif