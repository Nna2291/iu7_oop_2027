#include <cmath>
#include "point.h"

void point_init(point_t &point, const double x, const double y, const double z)
{
    point.x = x;
    point.y = y;
    point.z = z;
}

void point_default(point_t &point)
{
    point.x = 0.0;
    point.y = 0.0;
    point.z = 0.0;
}

//Переместить вызов в граф интерфейс

static void translate_to_origin(point_t &point, const point_t &center)
{
    point.x -= center.x;
    point.y -= center.y;
    point.z -= center.z;
}

static void translate_from_origin(point_t &point, const point_t &center)
{
    point.x += center.x;
    point.y += center.y;
    point.z += center.z;
}

static void prepare_rotation(double &cos_val, double &sin_val, const double angle_rad)
{
    cos_val = cos(angle_rad);
    sin_val = sin(angle_rad);
}

static void apply_rotation(double &a, double &b, const double cos_val, const double sin_val)
{
    double temp_a = a;
    double temp_b = b;
    
    a = temp_a * cos_val + temp_b * sin_val;
    b = -temp_a * sin_val + temp_b * cos_val;
}

static void rotate_x(point_t &point, const double angle_rad)
{
    double cos_val, sin_val;
    prepare_rotation(cos_val, sin_val, angle_rad);
    apply_rotation(point.y, point.z, cos_val, sin_val);
}

static void rotate_y(point_t &point, const double angle_rad)
{
    double cos_val, sin_val;
    prepare_rotation(cos_val, sin_val, angle_rad);
    apply_rotation(point.x, point.z, cos_val, sin_val);
}

static void rotate_z(point_t &point, const double angle_rad)
{
    double cos_val, sin_val;
    prepare_rotation(cos_val, sin_val, angle_rad);
    apply_rotation(point.x, point.y, cos_val, sin_val);
}

t_error file_read_data_point(point_t &point, FILE *fin)
{
    t_error rc = ERR_OK;

    if (!fin)
    {
        rc = ERR_FILE_OPEN;
    }
    else
    {
        double x, y, z;

        if (fscanf(fin, "%lf %lf %lf", &x, &y, &z) != 3)
        {
            rc = ERR_POINTS_FMT;
        }
        else
        {
            point.x = x;
            point.y = y;
            point.z = z;
        }
    }
    
    return rc;
}

t_error save_point(FILE* fout, const point_t &point)
{
    t_error rc = ERR_OK;
    if (!fout)
    {
        rc = ERR_FILE_OPEN;
    }
    else
    {
        if (fprintf(fout, "%lf %lf %lf\n", point.x, point.y, point.z) < 0)
        {
            rc = ERR_FILE_OUT;
        }
    }
    return rc;
}

static void scale(point_t &point, const scale_t &scale)
{
    point.x *= scale.kx;
    point.y *= scale.ky;
    point.z *= scale.kz;
}

void scale_point(point_t &point, const point_t &center, const scale_t &scale_data)
{
    translate_to_origin(point, center);

    scale(point, scale_data);

    translate_from_origin(point, center);
}

void move_point(point_t &point, const move_t &move)
{
    point.x += move.dx;
    point.y += move.dy;
    point.z += move.dz;
}

void rotate_point(point_t &point, const point_t &center, const rotate_t &rotate)
{
    translate_to_origin(point, center);
    
    rotate_x(point, rotate.angle_x_rad);
    rotate_y(point, rotate.angle_y_rad);
    rotate_z(point, rotate.angle_z_rad);
    
    translate_from_origin(point, center);
}
