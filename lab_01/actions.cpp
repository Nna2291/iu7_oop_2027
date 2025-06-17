#include "actions.h"
#include "drawing.h"
#include "model.h"
#include "errors.h"

t_error choose_option(request_t &request)
{
    static model_t model = create_initialized_model();
    
    t_error rc = ERR_OK;

    switch (request.action)
    {
        case SAVE:
            rc = model_save(model, request.fname);
            break;
        case LOAD:
            rc = model_download(model, request.fname);
            break;
        case DRAW:
            rc = draw_model(model, request.view);
            break;
        case SCALE:
            rc = model_scale(model, request.scale);
            break;
        case MOVE:
            rc = model_move(model, request.move);
            break;
        case ROTATE:
            rc = model_rotate(model, request.rotate);
            break;
        case EXIT:
            model_free(model);
            break;
        default:
            rc = ERR_UNDEF;
    }

    return rc;
}
