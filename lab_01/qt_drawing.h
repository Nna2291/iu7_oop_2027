#ifndef QT_DRAWING_H
#define QT_DRAWING_H

#include <QGraphicsScene>
#include <QColor>

void qt_clear_scene(QGraphicsScene *scene);
void qt_draw_line(QGraphicsScene *scene, double x1, double y1, double x2, double y2, const QColor &color);

#endif // QT_DRAWING_H
