#pragma once

#include "controller.h"
#include "../defines.h"

#include <QObject>

class LiftSystem : public QObject
{
    Q_OBJECT

public:
    explicit LiftSystem(QObject *parent = nullptr);

signals:
    // Сигналы, испускаемые для изменения окраски кнопки
    void floor_buttons_change_color_signal(int, direction_t, bool);
    void cabin_buttons_change_color_signal(int, cabin_id_t, bool);
    void cabin_position_changed_signal(cabin_id_t, int); // Сигнал изменения позиции лифта

    // Сигналы, испускаемые при нажатии кнопки
    void floor_destanation_slot(int floor, direction_t direction);
    void cabin_destanation_slot(int floor, cabin_id_t id);

public slots:
    void manage_floor_call_slot(int floor, direction_t direction);
    void manage_cabin_call_slot(int floor, cabin_id_t id);

    void change_floor_button_style_slot(int floor, direction_t direction, bool is_active);
    void change_cabin_button_style_slot(int floor, cabin_id_t id, bool is_active);
    void cabin_position_changed_slot(cabin_id_t cabin_id, int floor);

private:
    // Состояния подъемной системы
    Controller _controller;
};
