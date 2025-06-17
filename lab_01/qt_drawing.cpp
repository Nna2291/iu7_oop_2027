#include <QPen>
#include "qt_drawing.h"

void qt_clear_scene(QGraphicsScene *scene)
{
    if (scene)
    {
        scene->clear();
    }
}

void qt_draw_line(QGraphicsScene *scene, double x1, double y1, double x2, double y2, const QColor &color)
{
    if (scene)
    {
        QPen pen;
        pen.setColor(color);
        scene->addLine(x1, y1, x2, y2, pen);
    }
}
