#include <cstdlib>

#include "points.h"
#include "errors.h"

void points_default(points_t &points)
{
    points.array = NULL;
    points.count = 0;
}

void free_points(points_t &points)
{
    free(points.array);
    points_default(points);
}

t_error allocate_points(point_t *&arr_points, size_t count)
{
    t_error rc = ERR_OK;

    arr_points = (point_t *) malloc(count * sizeof(point_t));
    if (!arr_points)
    {
        rc = ERR_MEM;
    }

    return rc;
}

t_error file_read_amount_points(size_t &count, FILE *fin)
{
    t_error rc = ERR_OK;
    size_t temp_count = 0;
    if (fin == NULL)
    {
        rc = ERR_FILE_OPEN;
    }

    else if (fscanf(fin, "%zu", &temp_count) != 1)
    {
        rc = ERR_POINTS_FMT;
    }
    else if (temp_count <= 0)
    {
        rc = ERR_POINTS_LENGTH;
    }
    count = temp_count;
    return rc;
}

t_error file_read_data_points(point_t *&points, size_t count, FILE *fin)
{
    t_error rc = ERR_OK;
    if (fin == NULL)
    {
        rc = ERR_FILE_OPEN;
    }
    else if (count <= 0)
    {
        rc = ERR_POINTS_LENGTH;
    }
    else if (!points)
    {
        rc = ERR_MEM;
    }
    else
    {
        for (size_t i = 0; i < count; i++)
        {
            rc = file_read_data_point(points[i], fin);
        }
    }

    return rc;
}

t_error file_read_points(points_t &points, FILE *fin)
{
    if (!fin)
    {
        return ERR_FILE_OPEN;
    }

    t_error rc = file_read_amount_points(points.count, fin);
    if (rc != ERR_OK)
    {
        return rc;
    }

    rc = allocate_points(points.array, points.count);
    if (rc != ERR_OK)
    {
        return rc;
    }
    
    rc = file_read_data_points(points.array, points.count, fin);
    if (rc != ERR_OK)
    {
        free_points(points);
    }

    return rc;
}

t_error file_write_points(const points_t &points, FILE *fout)
{
    if (!fout)
    {
        return ERR_FILE_OUT;
    }
    
    if (!points.array)
    {
        return ERR_NO_DATA;
    }
    
    if (points.count <= 0)
    {
        return ERR_POINTS_LENGTH;
    }
    
    if (fprintf(fout, "%zu\n", points.count) < 0)
    {
        return ERR_FILE_OUT;
    }
    
    t_error rc = ERR_OK;
    for (size_t i = 0; i < points.count; i++)
    {
        rc = save_point(fout, points.array[i]);
        if (rc != ERR_OK)
        {
            return rc;
        }
    }

    return rc;
}

static void calculate_average(point_t &center, const point_t *points, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        center.x += points[i].x;
        center.y += points[i].y;
        center.z += points[i].z;
    }
    
    center.x /= count;
    center.y /= count;
    center.z /= count;
}

static void calculate_center_coordinates(point_t &center, const points_t &points)
{
    point_default(center);
    
    calculate_average(center, points.array, points.count);
}

t_error calculate_center(point_t &center, const points_t &points)
{
    if (!points.array)
    {
        return ERR_MODEL_LOAD;
    }
    
    if (points.count <= 0)
    {
        return ERR_POINTS_LENGTH;
    }
    
    calculate_center_coordinates(center, points);
    
    return ERR_OK;
}

t_error scale_points(points_t &points, const point_t &center, const scale_t &coef)
{
    t_error rc = ERR_OK;

    if (!points.array || points.count <= 0)
    {
        rc = ERR_MODEL_LOAD;
    }
    else
    {
        for (size_t i = 0; i < points.count; i++)
        {
            scale_point(points.array[i], center, coef);
        }
    }

    return rc;
}

t_error move_points(points_t &points, point_t &center, const move_t &coef)
{
    t_error rc = ERR_OK;

    if (!points.array || points.count <= 0)
    {
        rc = ERR_MODEL_LOAD;
    }
    else
    {
        move_point(center, coef);

        for (size_t i = 0; i < points.count; i++)
        {
            move_point(points.array[i], coef);
        }
    }

    return rc;
}

t_error rotate_points(points_t &points, const point_t &center, const rotate_t &coef)
{
    t_error rc = ERR_OK;

    if (!points.array || points.count <= 0)
    {
        rc = ERR_MODEL_LOAD;
    }
    else
    {
        for (size_t i = 0; i < points.count; i++)
        {
            rotate_point(points.array[i], center, coef);
        }
    }

    return rc;
}
