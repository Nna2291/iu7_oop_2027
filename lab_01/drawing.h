#ifndef DRAWING_H
#define DRAWING_H

#include <QGraphicsScene>
#include <QColor>
#include "model.h"
#include "qt_drawing.h"

struct drawing_view
{
    QGraphicsScene *scene;
    double width;
    double height;
    QColor line_color;
};

using drawing_view_t = struct drawing_view;

struct line
{
    point_t first_point;
    point_t second_point;
};

using line_t = struct line;

t_error clear_scene(const drawing_view_t &view);

t_error from_math_to_display(point_t &result, const point_t &point, const drawing_view_t &view);

t_error get_points(line_t &line, const drawing_view_t &view, const edge_t &edge, const point_t *array_points, const int points_count);

t_error draw_line(const drawing_view_t &view, const point_t &p1, const point_t &p2);

t_error draw_lines(const drawing_view_t &view, const points_t &points, const edges_t &edges);

t_error draw_model(const model_t &model, drawing_view_t &view);

#endif // DRAWING_H
