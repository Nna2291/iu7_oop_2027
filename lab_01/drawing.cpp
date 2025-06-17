#include <QPen>
#include <QApplication>
#include <QPalette>

#include "drawing.h"
#include "errors.h"

t_error clear_scene(const drawing_view_t &view)
{
    if (!view.scene)
    {
        return ERR_SCENE_VIEW;
    }

    qt_clear_scene(view.scene);
    return ERR_OK;
}

t_error from_math_to_display(point_t &result, const point_t &point, const drawing_view_t &view)
{
    t_error rc = ERR_OK;
    if (!view.scene)
    {
        rc = ERR_SCENE_VIEW;
    }
    
    if (view.width <= 0 || view.height <= 0)
    {
        rc = ERR_SIZE_VIEW;
    }
    
    if (rc == ERR_OK)
    {
        result = point;
        result.x += view.width / 2;
        result.y += view.height / 2;
    }

    return rc;
}

t_error get_points(line_t &line, const drawing_view_t &view, const edge_t &edge, const point_t *array_points, int points_count)
{
    t_error rc = ERR_OK;
    
    if (!array_points)
    {
        rc = ERR_MEM;
    }
    
    if (!view.scene)
    {
        rc = ERR_SCENE_VIEW;
    }
    
    if (edge.start_ind >= points_count || edge.end_ind >= points_count)
    {
        rc = ERR_EDGES_FMT;
    }

    if (rc != ERR_OK)
    {
        return rc;
    }
    
    rc = from_math_to_display(line.first_point, array_points[edge.start_ind], view);
    if (rc == ERR_OK)
    {
        rc = from_math_to_display(line.second_point, array_points[edge.end_ind], view);
    }

    return rc;
}

t_error draw_line(const drawing_view_t &view, const point_t &p1, const point_t &p2)
{
    if (!view.scene)
    {
        return ERR_SCENE_VIEW;
    }

    qt_draw_line(view.scene, p1.x, p1.y, p2.x, p2.y, view.line_color);
    return ERR_OK;
}

// Дописать условие rc
t_error draw_lines(const drawing_view_t &view, const points_t &points, const edges_t &edges)
{
    if (!view.scene)
    {
        return ERR_SCENE_VIEW;
    }

    if (!points.array || points.count <= 0)
    {
        return ERR_MEM;
    }

    if (!edges.array || edges.count <= 0)
    {
        return ERR_MEM;
    }

    t_error rc = ERR_OK;
    line_t line;

    for (size_t i = 0; i < edges.count; ++i)
    {
        const edge_t &edge = edges.array[i];
        rc = get_points(line, view, edge, points.array, points.count);
        if (rc == ERR_OK)
        {
            rc = draw_line(view, line.first_point, line.second_point);
        }
    }

    return rc;
}


t_error draw_model(const model_t &model, drawing_view_t &view)
{
    t_error rc = clear_scene(view);
    if (rc != ERR_OK)
    {
        return rc;
    }
        
    rc = draw_lines(view, model.points, model.edges);
    return rc;
}
