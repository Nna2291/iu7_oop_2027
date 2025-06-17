#include "elevator.h"

LiftSystem::LiftSystem(QObject *parent)
    : QObject(parent)
{
    QObject::connect(this, &LiftSystem::floor_destanation_slot, &_controller, &Controller::floor_destanation_slot);
    QObject::connect(this, &LiftSystem::cabin_destanation_slot, &_controller, &Controller::cabin_destanation_slot);

    QObject::connect(&_controller, &Controller::floor_buttons_change_color_signal, this, &LiftSystem::change_floor_button_style_slot);
    QObject::connect(&_controller, &Controller::cabin_buttons_change_color_signal, this, &LiftSystem::change_cabin_button_style_slot);
    QObject::connect(&_controller, &Controller::cabin_position_changed_signal, this, &LiftSystem::cabin_position_changed_slot);
}

void LiftSystem::manage_floor_call_slot(int floor, direction_t direction)
{
    emit floor_destanation_slot(floor, direction);
}

void LiftSystem::manage_cabin_call_slot(int floor, cabin_id_t id)
{
    emit cabin_destanation_slot(floor, id);
}

void LiftSystem::change_floor_button_style_slot(int floor, direction_t direction, bool is_active)
{
    emit floor_buttons_change_color_signal(floor, direction, is_active);
}

void LiftSystem::change_cabin_button_style_slot(int floor, cabin_id_t id, bool is_active)
{
    emit cabin_buttons_change_color_signal(floor, id, is_active);
}

void LiftSystem::cabin_position_changed_slot(cabin_id_t cabin_id, int floor)
{
    emit cabin_position_changed_signal(cabin_id, floor);
}


