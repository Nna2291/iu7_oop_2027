#ifndef EDGES_H
#define EDGES_H

#include <cstdio>
#include "edge.h"
#include "errors.h"

struct edges
{
    edge_t *array;
    size_t count;
};

using edges_t = struct edges;

void edges_init(edges_t &edges);

void edges_free(edges_t &edges);

t_error allocate_edges(edges_t &edges);

t_error file_read_amount_edges(edges_t &edges, FILE *fin);

t_error file_read_data_edges(edges_t &edges, FILE *fin);

t_error file_read_edges(edges_t &edges, FILE *fin);

t_error file_write_edges(const edges_t &edges, FILE *fout);

#endif // EDGES_H
