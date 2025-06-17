#pragma once

#include "../cabin/cabin.h"
#include "../defines.h"
#include "../button/floor_button.h"
#include "../button/lift_button.h"
#include "../task/task_manager.h"

#include <QObject>
#include <iostream>

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

signals:
    void floor_buttons_change_color_signal(int, direction_t, bool);
    void cabin_buttons_change_color_signal(int, cabin_id_t, bool);
    void cabin_position_changed_signal(cabin_id_t, int); // Новый сигнал для позиции лифта

    void button_deactivated_signal(cabin_id_t);
    void free_cabin_signal(cabin_id_t);
    void move_cabin_signal(cabin_id_t, int, direction_t);
    void stop_cabin_signal(cabin_id_t, int);
    void free_contoller_signal();

public slots:
    void floor_destanation_slot(int floor, direction_t direction);
    void cabin_destanation_slot(int floor, cabin_id_t id);

    void manage_move_slot(cabin_id_t id);
    void manage_cabin_slot(cabin_id_t cabin_id);
    void reach_dst_floor_slot(cabin_id_t id);
    void free_controller_slot();

private:
    direction_t get_direction(int difference);
    direction_t get_next_direction(cabin_id_t id);
    cabin_id_t get_desided_cabin_id(int floor, direction_t direction);

    // Новый метод для расчета веса лифта
    double calculate_cabin_weight(cabin_id_t id, int target_floor, direction_t direction);

    int get_next_visit_floor(cabin_id_t id);
    int find_next_floor_in_direction(cabin_id_t id, direction_t direction, int current_floor);
    bool is_all_cabins_are_free();

    enum controller_state_t
    {
        CON_FREE,
        CON_REQUEST_CABIN,
        CON_REQUEST_FLOOR,
        CON_MANAGING_CABIN,
        CON_MANAGING_MOVE,
        CON_REACH_DST_FLOOR,
    };

    controller_state_t _state = CON_FREE;

    std::unique_ptr<Cabin> _cabins[CABINS_COUNT] = {};
    int _cur_floor[CABINS_COUNT] = {};
    direction_t _cur_directions[CABINS_COUNT] = {};
    direction_t _preferred_directions[CABINS_COUNT] = {}; // Предпочтительное направление SCAN

    std::shared_ptr<FloorButton> _floor_buttons[DIRECTIONS_COUNT][FLOOR_COUNT];
    std::shared_ptr<LiftButton> _lift_buttons[CABINS_COUNT][FLOOR_COUNT];

    // Заменяем трёхмерный массив на систему задач
    TaskManager _task_manager;
}; 
