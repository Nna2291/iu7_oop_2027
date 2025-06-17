#ifndef ACTIONS_H
#define ACTIONS_H

#include "errors.h"
#include "drawing.h"
#include "model_transform.h"

enum action
{
    LOAD = 0,
    SAVE,
    DRAW,
    SCALE,
    MOVE,
    ROTATE,
    EXIT
};

struct request
{
    enum action action;
    drawing_view_t view;
    union
    {
        const char *fname;
        scale_t scale;
        move_t move;
        rotate_t rotate;
    };
};

using request_t = struct request;

t_error choose_option(request_t &request);

#endif // ACTIONS_H
