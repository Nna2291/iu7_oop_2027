#ifndef EDGE_H
#define EDGE_H

#include <cstdio>
#include "errors.h"

struct edge
{
    int start_ind;
    int end_ind;
};

using edge_t = struct edge;

t_error read_edge(edge_t &edge, FILE *file_in);

t_error save_edge(FILE *fout, const edge_t &edge);

#endif // EDGE_H
