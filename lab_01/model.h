#ifndef MODEL_H
#define MODEL_H

#include "points.h"
#include "edges.h"

struct model
{
    point_t center;
    points_t points;
    edges_t edges;
};

using model_t = struct model;

void model_init(model_t &model);

void model_free(model_t &model);

t_error file_read_model(model_t &model, FILE *file_in);

t_error model_download(model_t &model, const char *filename);

t_error model_save(model_t &model, const char *filename);

t_error model_scale(model_t &model, const scale_t &scale);

t_error model_move(model_t &model, const move_t &move);

t_error model_rotate(model_t &model, const rotate_t &rotate);

model_t create_initialized_model();

#endif // MODEL_H
