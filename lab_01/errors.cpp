#include <QMessageBox>

#include "errors.h"

void error_message(t_error &return_code)
{
    switch (return_code)
    {
        case ERR_FILE_OPEN:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка файла");
            break;
        case ERR_FILE_INVALID:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка файла");
            break;


        case ERR_MEM:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка памяти");
            break;


        case ERR_POINTS_LENGTH:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка количества точек");
            break;
        case ERR_POINTS_FMT:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка формата точек");
            break;


        case ERR_EDGES_LENGTH:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка количества рёбер");
            break;
        case ERR_EDGES_FMT:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка формата рёбер");
            break;


        case ERR_MODEL_LOAD:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка загрузки");
            break;

        case ERR_SCENE_VIEW:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка модели");
            break;
        case ERR_SIZE_VIEW:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка области отображения");
            break;


        case ERR_NO_DATA:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка файла");
            break;
        case ERR_FILE_OUT:
            QMessageBox::critical(NULL, "Ошибка", "Ошибка файла");
            break;
        default:
            QMessageBox::critical(NULL, "Ошибка", "ПИЗДА!");
    }
}
