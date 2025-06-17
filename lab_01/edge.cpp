#include "edge.h"
#include "errors.h"

t_error read_edge(edge_t &edge, FILE *file_in)
{
    t_error rc = ERR_OK;
    if (!file_in)
    {
        return ERR_FILE_OPEN;
    }
    
    if (fscanf(file_in, "%d %d", &edge.start_ind, &edge.end_ind) != 2)
    {
        rc = ERR_EDGES_FMT;
    }

    return rc;
}

t_error save_edge(FILE *fout, const edge_t &edge)
{
    t_error rc = ERR_OK;
    if (!fout)
    {
        return ERR_FILE_OUT;
    }

    if (fprintf(fout, "%d %d\n", edge.start_ind, edge.end_ind) < 0)
    {
        rc = ERR_FILE_OUT;
    }
    return rc;
}
