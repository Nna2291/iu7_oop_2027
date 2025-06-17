#ifndef ERRORS_H
#define ERRORS_H

enum error
{

    ERR_OK = 0,

    ERR_FILE_OPEN,
    ERR_FILE_INVALID,
    ERR_FILE_POINTER_INVALID,

    ERR_MEM,

    ERR_POINTS_LENGTH,
    ERR_POINTS_FMT,

    ERR_EDGES_LENGTH,
    ERR_EDGES_FMT,

    ERR_MODEL_LOAD,

    ERR_SCENE_VIEW,
    ERR_SIZE_VIEW,

    ERR_NO_DATA,
    ERR_FILE_OUT,

    ERR_UNDEF
};

using t_error = enum error;

void error_message(t_error &return_code);

#endif // ERRORS_H
