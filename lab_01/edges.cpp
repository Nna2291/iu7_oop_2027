#include <cstdlib>
#include "edges.h"

void edges_init(edges_t &edges)
{
    edges.array = NULL;
    edges.count = 0;
}

void edges_free(edges_t &edges)
{
    if (edges.array)
    {
        free(edges.array);
    }
    edges_init(edges);
}

t_error allocate_edges(edges_t &edges)
{
    t_error rc = ERR_OK;
    if (edges.count <= 0)
    {
        rc = ERR_EDGES_LENGTH;
    }
    else
    {
        edge_t *temp_array = (edge_t *) malloc(edges.count * sizeof(edge_t));
        if (!temp_array)
        {
            rc = ERR_MEM;
        }
        else
        {
            edges.array = temp_array;
        }
    }
    return rc;
}

t_error file_read_amount_edges(edges_t &edges, FILE *fin)
{
    t_error rc = ERR_OK;
    if (!fin)
    {
        rc = ERR_FILE_OPEN;
    }
    else if (fscanf(fin, "%zu", &edges.count) != 1)
    {
        rc = ERR_EDGES_FMT;
    }
    else if (edges.count <= 0)
    {
        rc = ERR_EDGES_LENGTH;
    }
    return rc;
}

t_error file_read_data_edges(edges_t &edges, FILE *fin)
{
    if (!fin)
    {
        return ERR_FILE_OPEN;
    }

    t_error rc = allocate_edges(edges);
    if (rc != ERR_OK)
    {
        return rc;
    }

    for (size_t i = 0; rc == ERR_OK && i < edges.count; i++)
    {
        rc = read_edge(edges.array[i], fin);
    }
    if (rc != ERR_OK)
    {
        edges_free(edges);
        return rc;
    }

    return rc;
}

t_error file_read_edges(edges_t &edges, FILE *fin)
{
    if (!fin)
    {
        return ERR_FILE_OPEN;
    }

    edges_t tmp;
    edges_init(tmp);

    t_error rc = file_read_amount_edges(tmp, fin);
    if (rc != ERR_OK)
    {
        edges_free(edges);
        return rc;
    }
    
    rc = file_read_data_edges(tmp, fin);
    if (rc == ERR_OK)
    {
        edges_free(edges);
        edges = tmp;
    }
    
    return rc;
}

t_error file_write_edges(const edges_t &edges, FILE *fout)
{
    if (!fout)
    {
        return ERR_FILE_OPEN;
    }

    if (!edges.array)
    {
        return ERR_NO_DATA;
    }

    if (edges.count <= 0) 
    {
        return ERR_EDGES_LENGTH;
    }

    t_error rc = ERR_OK;
    if (fprintf(fout, "%zu\n", edges.count) < 0)
    {
        rc = ERR_FILE_OUT;
    }
    else
    {
        for (size_t i = 0; i < edges.count; i++)
        {
            rc = save_edge(fout, edges.array[i]);
        }
    }

    return rc;
}
