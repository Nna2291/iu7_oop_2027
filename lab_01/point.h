#ifndef POINT_H
#define POINT_H

#include <cstdio>

#include "errors.h"
#include "model_transform.h"

struct point
{
    double x;
    double y;
    double z;
};

using point_t = struct point;

void point_init(point_t &point, const double x, const double y, const double z);

void point_default(point_t &point);

t_error file_read_data_point(point_t &point, FILE *fin);

t_error save_point(FILE* fout, const point_t &point);

void scale_point(point_t &point, const point_t &center, const scale_t &scale_data);

void move_point(point_t &point, const move_t &move);

void rotate_point(point_t &point, const point_t &center, const rotate_t &rotate);

#endif // POINT_H
