#ifndef POINTS_H
#define POINTS_H

#include "point.h"

struct points
{
    point_t *array;
    size_t count;
};

using points_t = struct points;

void points_default(points_t &points);

void free_points(points_t &points);

t_error allocate_points(point_t *&arr_points, size_t count);

t_error file_read_amount_points(size_t &count, FILE *fin);

t_error file_read_data_points(point_t *&points, size_t count, FILE *fin);

t_error file_read_points(points_t &points, FILE *fin);

t_error file_write_points(const points_t &points, FILE *fout);

t_error calculate_center(point_t &center, const points_t &points);

t_error scale_points(points_t &points, const point_t &center, const scale_t &coef);

t_error move_points(points_t &points, point_t &center, const move_t &coef);

t_error rotate_points(points_t &points, const point_t &center, const rotate_t &coef);

#endif // POINTS_H
