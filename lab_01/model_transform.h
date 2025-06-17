#ifndef MODEL_TRANSFORM_H
#define MODEL_TRANSFORM_H

struct scale
{
    double kx;
    double ky;
    double kz;
};

using scale_t = struct scale;

struct move
{
    double dx;
    double dy;
    double dz;
};

using move_t = struct move;

struct rotate
{
    double angle_x_rad;
    double angle_y_rad;
    double angle_z_rad;
};

using rotate_t = struct rotate;

#endif // MODEL_TRANSFORM_H
