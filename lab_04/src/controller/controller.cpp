#include "controller.h"
#include <limits>

#define FLOOR_NOT_FOUND -1

Controller::Controller(QObject *parent)
    : QObject(parent)
{
    for (int i = 0; i < CABINS_COUNT; i++)
    {
        _cabins[i] = std::make_unique<Cabin>((cabin_id_t)i, this);
        _cur_floor[i] = START_FLOOR - 1;
        _cur_directions[i] = DIR_STAND;
        _preferred_directions[i] = DIR_UP;

        QObject::connect(_cabins[i].get(), &Cabin::cabin_finish_boarding, this, &Controller::reach_dst_floor_slot);
        QObject::connect(&_cabins[i]->move_timer, &QTimer::timeout, this, [=, this]()
                         { manage_move_slot(static_cast<cabin_id_t>(i)); });
    }

    QObject::connect(this, &Controller::free_cabin_signal, this, [this](cabin_id_t id)
                     { _cabins[id]->cabin_free_slot(); });
    QObject::connect(this, &Controller::move_cabin_signal, this, [this](cabin_id_t id, int floor, direction_t direction)
                     { _cabins[id]->cabin_moving_slot(floor, direction); });
    QObject::connect(this, &Controller::stop_cabin_signal, this, [this](cabin_id_t id, int floor)
                     { _cabins[id]->cabin_start_boarding_slot(floor); });

    for (int i = 0; i < FLOOR_COUNT; i++)
    {
        _floor_buttons[BDIR_UP][i] = std::make_shared<FloorButton>(i + 1, DIR_UP);
        _floor_buttons[BDIR_DOWN][i] = std::make_shared<FloorButton>(i + 1, DIR_DOWN);
        _lift_buttons[CID_FIRST][i] = std::make_shared<LiftButton>(i + 1, CID_FIRST);
        _lift_buttons[CID_SECOND][i] = std::make_shared<LiftButton>(i + 1, CID_SECOND);

        QObject::connect(_floor_buttons[BDIR_UP][i].get(), &FloorButton::activated_signal, this, [=, this]()
                         {
            cabin_id_t id = get_desided_cabin_id(i, DIR_UP);
            manage_cabin_slot(id); });
        QObject::connect(_floor_buttons[BDIR_DOWN][i].get(), &FloorButton::activated_signal, this, [=, this]()
                         {
            cabin_id_t id = get_desided_cabin_id(i, DIR_DOWN);
            manage_cabin_slot(id); });

        QObject::connect(_lift_buttons[CID_FIRST][i].get(), &LiftButton::activated_signal, this, [=, this]()
                         { manage_cabin_slot(CID_FIRST); });
        QObject::connect(_lift_buttons[CID_SECOND][i].get(), &LiftButton::activated_signal, this, [=, this]()
                         { manage_cabin_slot(CID_SECOND); });

        QObject::connect(this, &Controller::button_deactivated_signal, this, &Controller::manage_cabin_slot);

        QObject::connect(_floor_buttons[BDIR_UP][i].get(), &FloorButton::activated_signal, this, [=, this]()
                         { emit floor_buttons_change_color_signal(i + 1, DIR_UP, true); });
        QObject::connect(_floor_buttons[BDIR_DOWN][i].get(), &FloorButton::activated_signal, this, [=, this]()
                         { emit floor_buttons_change_color_signal(i + 1, DIR_DOWN, true); });
        QObject::connect(_lift_buttons[CID_FIRST][i].get(), &LiftButton::activated_signal, this, [=, this]()
                         { emit cabin_buttons_change_color_signal(i + 1, CID_FIRST, true); });
        QObject::connect(_lift_buttons[CID_SECOND][i].get(), &LiftButton::activated_signal, this, [=, this]()
                         { emit cabin_buttons_change_color_signal(i + 1, CID_SECOND, true); });

        QObject::connect(_floor_buttons[BDIR_UP][i].get(), &FloorButton::deactivated_signal, this, [=, this]()
                         { emit floor_buttons_change_color_signal(i + 1, DIR_UP, false); });
        QObject::connect(_floor_buttons[BDIR_DOWN][i].get(), &FloorButton::deactivated_signal, this, [=, this]()
                         { emit floor_buttons_change_color_signal(i + 1, DIR_DOWN, false); });
        QObject::connect(_lift_buttons[CID_FIRST][i].get(), &LiftButton::deactivated_signal, this, [=, this]()
                         { emit cabin_buttons_change_color_signal(i + 1, CID_FIRST, false); });
        QObject::connect(_lift_buttons[CID_SECOND][i].get(), &LiftButton::deactivated_signal, this, [=, this]()
                         { emit cabin_buttons_change_color_signal(i + 1, CID_SECOND, false); });

        QObject::connect(this, &Controller::free_contoller_signal, this, &Controller::free_controller_slot);
    }
    
    for (int i = 0; i < CABINS_COUNT; i++) {
        emit cabin_position_changed_signal(static_cast<cabin_id_t>(i), START_FLOOR);
    }
}

void Controller::floor_destanation_slot(int floor, direction_t direction)
{
    if (_state != CON_FREE && _state != CON_MANAGING_CABIN && _state != CON_MANAGING_MOVE)
        return;

    for (int id = 0; id < CABINS_COUNT; id++)
    {
        if ((direction == DIR_DOWN && _task_manager.has_floor_call_down(static_cast<cabin_id_t>(id), floor)) ||
            (direction == DIR_UP && _task_manager.has_floor_call_up(static_cast<cabin_id_t>(id), floor)))
        {
            return;
        }
    }

    for (int id = 0; id < CABINS_COUNT; id++)
    {
        if (_cur_floor[id] == floor - 1 && _cur_directions[id] == DIR_STAND)
        {
            emit stop_cabin_signal(static_cast<cabin_id_t>(id), _cur_floor[id]);
            return;
        }
    }

    _state = CON_REQUEST_FLOOR;

    cabin_id_t desided_cabin_id = CID_FIRST;
    double best_weight = std::numeric_limits<double>::max();

    for (int i = 0; i < CABINS_COUNT; i++)
    {
        double current_weight = calculate_cabin_weight(static_cast<cabin_id_t>(i), floor, direction);
        
        qInfo("Лифт %d: вес = %.2f (этаж %d, направление %d)", 
              i, current_weight, _cur_floor[i] + 1, _cur_directions[i]);
        
        if (current_weight < best_weight)
        {
            best_weight = current_weight;
            desided_cabin_id = static_cast<cabin_id_t>(i);
        }
    }
    
    qInfo("Выбран лифт %d с весом %.2f для этажа %d (направление %s)", 
          desided_cabin_id, best_weight, floor, direction == DIR_UP ? "вверх" : "вниз");
          
    qInfo("=== Текущее состояние задач ===");
    _task_manager.print_all_tasks();

    Task new_task(floor, direction, desided_cabin_id, TASK_FLOOR_CALL);
    _task_manager.add_task(new_task);
    
    qInfo("Добавлена задача: %s", new_task.to_string().c_str());
    
    if (direction == DIR_UP)
    {
        emit _floor_buttons[BDIR_UP][floor - 1]->activate_signal();
    }
    else
    {
        emit _floor_buttons[BDIR_DOWN][floor - 1]->activate_signal();
    }
}

void Controller::cabin_destanation_slot(int floor, cabin_id_t id)
{
    if (_state != CON_FREE && _state != CON_MANAGING_CABIN && _state != CON_MANAGING_MOVE)
        return;

    if (_task_manager.has_cabin_call(id, floor))
        return;

    _state = CON_REQUEST_CABIN;
    
    Task cabin_task(floor, DIR_STAND, id, TASK_CABIN_CALL);
    _task_manager.add_task(cabin_task);
    
    qInfo("Добавлена задача кабины: %s", cabin_task.to_string().c_str());

    emit _lift_buttons[id][floor - 1]->activate_signal();
}

void Controller::manage_move_slot(cabin_id_t id)
{
    if (_state != CON_MANAGING_CABIN)
        return;

    _state = CON_MANAGING_MOVE;
    _cur_floor[id] += _cur_directions[id];
    
    emit cabin_position_changed_signal(id, _cur_floor[id] + 1);

    emit manage_cabin_slot(id);
}

void Controller::manage_cabin_slot(cabin_id_t id)
{
    if (_state == CON_FREE || _state == CON_MANAGING_CABIN)
        return;

    _state = CON_MANAGING_CABIN;
    int dst_floor = get_next_visit_floor(id);
    if (dst_floor == FLOOR_NOT_FOUND)
    {
        _cur_directions[id] = DIR_STAND;
        emit free_cabin_signal(id);
    }
    else if (dst_floor > _cur_floor[id])
    {
        _cur_directions[id] = DIR_UP;
        emit move_cabin_signal(id, _cur_floor[id], DIR_UP);
    }
    else if (dst_floor < _cur_floor[id])
    {
        _cur_directions[id] = DIR_DOWN;
        emit move_cabin_signal(id, _cur_floor[id], DIR_DOWN);
    }
    else
    {
        _cur_directions[id] = get_next_direction(id);
        emit stop_cabin_signal(id, _cur_floor[id]);
    }

    if (is_all_cabins_are_free())
        emit free_contoller_signal();
}

void Controller::reach_dst_floor_slot(cabin_id_t id)
{
    if (_state != CON_MANAGING_CABIN)
        return;

    _state = CON_REACH_DST_FLOOR;

    int current_floor = _cur_floor[id] + 1;
    
    qInfo("=== Обработка остановки лифта %d на этаже %d ===", id, current_floor);
    qInfo("Задач до удаления: %d", _task_manager.get_task_count_for_cabin(id));
    
    if (_task_manager.has_cabin_call(id, current_floor))
    {
        Task cabin_task(current_floor, DIR_STAND, id, TASK_CABIN_CALL);
        bool removed = _task_manager.remove_task(cabin_task);
        qInfo("Удалена задача кабины: %s (успешно: %s)", cabin_task.to_string().c_str(), removed ? "да" : "нет");
        emit _lift_buttons[id][_cur_floor[id]]->deactivate_signal();
    }

    bool removed_up = false, removed_down = false;
    
    if (_task_manager.has_floor_call_up(id, current_floor))
    {
        Task floor_task_up(current_floor, DIR_UP, id, TASK_FLOOR_CALL);
        removed_up = _task_manager.remove_task(floor_task_up);
        qInfo("Удалена задача этажа ↑: %s (успешно: %s)", floor_task_up.to_string().c_str(), removed_up ? "да" : "нет");
        if (removed_up) {
            emit _floor_buttons[BDIR_UP][_cur_floor[id]]->deactivate_signal();
        }
    }
    
    if (_task_manager.has_floor_call_down(id, current_floor))
    {
        Task floor_task_down(current_floor, DIR_DOWN, id, TASK_FLOOR_CALL);
        removed_down = _task_manager.remove_task(floor_task_down);
        qInfo("Удалена задача этажа ↓: %s (успешно: %s)", floor_task_down.to_string().c_str(), removed_down ? "да" : "нет");
        if (removed_down) {
            emit _floor_buttons[BDIR_DOWN][_cur_floor[id]]->deactivate_signal();
        }
    }

    qInfo("Задач после удаления: %d", _task_manager.get_task_count_for_cabin(id));
    qInfo("=== Состояние всех задач после остановки ===");
    _task_manager.print_all_tasks();
    
    _cur_directions[id] = DIR_STAND;

    emit button_deactivated_signal(id);
}

void Controller::free_controller_slot()
{
    if (_state == CON_FREE)
        return;

    _state = CON_FREE;
    qInfo("[!] Контроллер находится в состоянии покоя.");
}

direction_t Controller::get_direction(int difference)
{
    if (difference == 0)
    {
        return DIR_STAND;
    }

    return difference > 0 ? DIR_UP : DIR_DOWN;
}

cabin_id_t Controller::get_desided_cabin_id(int floor, direction_t direction)
{
    for (int i = 0; i < CABINS_COUNT; i++)
    {
        if ((direction == DIR_UP && _task_manager.has_floor_call_up(static_cast<cabin_id_t>(i), floor + 1)) ||
            (direction == DIR_DOWN && _task_manager.has_floor_call_down(static_cast<cabin_id_t>(i), floor + 1)))
            return static_cast<cabin_id_t>(i);
    }
    return CID_FIRST;
}

int Controller::get_next_visit_floor(cabin_id_t id)
{
    std::vector<Task> cabin_tasks = _task_manager.get_tasks_for_cabin(id);
    
    if (cabin_tasks.empty())
    {
        qInfo("Лифт %d: нет задач", id);
        return FLOOR_NOT_FOUND;
    }
    
    int current_floor = _cur_floor[id] + 1;
    
    qInfo("Лифт %d: поиск следующего этажа, задач: %zu, текущий этаж: %d, направление: %d", 
          id, cabin_tasks.size(), current_floor, _cur_directions[id]);

    bool has_current_floor_task = false;
    for (const auto& task : cabin_tasks)
    {
        if (task.get_floor() == current_floor)
        {
            has_current_floor_task = true;
            qInfo("Лифт %d: найдена задача на текущем этаже %d", id, current_floor);
            break;
        }
    }
    
    if (has_current_floor_task)
    {
        return _cur_floor[id];
    }

    if (_cur_directions[id] == DIR_STAND)
    {   
        // Проверяем, есть ли задачи в предпочтительном направлении
        bool has_tasks_in_preferred = false;
        bool has_tasks_in_opposite = false;
        
        for (const auto& task : cabin_tasks)
        {
            if (_preferred_directions[id] == DIR_UP && task.get_floor() > current_floor)
            {
                // При движении вверх: подбираем ВСЕ задачи на этажах выше
                has_tasks_in_preferred = true;
            }
            else if (_preferred_directions[id] == DIR_DOWN && task.get_floor() < current_floor)
            {
                // При движении вниз: подбираем ВСЕ задачи на этажах ниже
                has_tasks_in_preferred = true;
            }
            else if (_preferred_directions[id] == DIR_UP && task.get_floor() < current_floor)
            {
                // Задачи в обратном направлении (для разворота)
                has_tasks_in_opposite = true;
            }
            else if (_preferred_directions[id] == DIR_DOWN && task.get_floor() > current_floor)
            {
                // Задачи в обратном направлении (для разворота)
                has_tasks_in_opposite = true;
            }
        }
        
        qInfo("Лифт %d стоит: предпочтительное направление=%d, задачи в направлении=%s, задачи в обратном=%s", 
              id, _preferred_directions[id], 
              has_tasks_in_preferred ? "да" : "нет", 
              has_tasks_in_opposite ? "да" : "нет");
        
        // Продолжаем в предпочтительном направлении, пока есть задачи
        if (has_tasks_in_preferred)
        {
            int target_floor = find_next_floor_in_direction(id, _preferred_directions[id], current_floor);
            if (target_floor != FLOOR_NOT_FOUND)
            {
                return target_floor;
            }
        }
        else if (has_tasks_in_opposite)
        {
            // Меняем предпочтительное направление
            _preferred_directions[id] = (_preferred_directions[id] == DIR_UP) ? DIR_DOWN : DIR_UP;
            qInfo("Лифт %d: РАЗВОРОТ! Новое направление=%d", id, _preferred_directions[id]);
            
            if (_preferred_directions[id] == DIR_UP)
            {
                for (int floor = current_floor + 1; floor <= FLOOR_COUNT; floor++)
                {
                    if (_task_manager.has_cabin_call(id, floor) || 
                        _task_manager.has_floor_call_up(id, floor) ||
                        _task_manager.has_floor_call_down(id, floor))
                    {
                        qInfo("Лифт %d: после разворота едем вверх к этажу %d", id, floor);
                        return floor - 1;
                    }
                }
            }
            else
            {
                for (int floor = current_floor - 1; floor >= 1; floor--)
                {
                    if (_task_manager.has_cabin_call(id, floor) || 
                        _task_manager.has_floor_call_up(id, floor) ||
                        _task_manager.has_floor_call_down(id, floor))
                    {
                        qInfo("Лифт %d: после разворота едем вниз к этажу %d", id, floor);
                        return floor - 1;
                    }
                }
            }
        }
        
        qInfo("Лифт %d: нет задач в любом направлении", id);
        return FLOOR_NOT_FOUND;
    }
    else
    {
        // Ищем ближайший этаж по ходу движения
        int target_floor = find_next_floor_in_direction(id, _cur_directions[id], current_floor);
        if (target_floor != FLOOR_NOT_FOUND)
        {
            return target_floor;
        }
    }

    qInfo("Лифт %d: этаж не найден", id);
    return FLOOR_NOT_FOUND;
}

direction_t Controller::get_next_direction(cabin_id_t id)
{
    std::vector<Task> cabin_tasks = _task_manager.get_tasks_for_cabin(id);
    
    if (cabin_tasks.empty())
        return DIR_STAND;

    if (_cur_directions[id] == DIR_STAND)
    {
        // Если лифт стоит, то едем к ближайшему этажу
        int dst_visit_floor = -1;
        int min_dst = FLOOR_COUNT + 1;

        for (const auto& task : cabin_tasks)
        {
            int diff = std::abs((_cur_floor[id] + 1) - task.get_floor());
            
            if (diff > 0 && diff < min_dst)
            {
                min_dst = diff;
                dst_visit_floor = task.get_floor();
            }
        }

        if (dst_visit_floor == -1)
            return DIR_STAND;

        if (dst_visit_floor > _cur_floor[id] + 1)
            return DIR_UP;
        else
            return DIR_DOWN;
    }

    int target_floor = get_next_visit_floor(id);
    if (target_floor == FLOOR_NOT_FOUND)
        return DIR_STAND;
    
    int target_real_floor = target_floor + 1;
    int current_real_floor = _cur_floor[id] + 1;
    
    qInfo("Лифт %d: текущий этаж %d, целевой этаж %d", id, current_real_floor, target_real_floor);
    
    if (target_real_floor > current_real_floor)
        return DIR_UP;
    else if (target_real_floor < current_real_floor)
        return DIR_DOWN;
    else
        return DIR_STAND;
}

bool Controller::is_all_cabins_are_free()
{
    return !_task_manager.has_any_tasks();
}

double Controller::calculate_cabin_weight(cabin_id_t id, int target_floor, direction_t direction)
{
    double weight = 0.0;
    
    int distance = std::abs(target_floor - (_cur_floor[id] + 1));
    weight += distance * 10.0;
    
    int task_count = _task_manager.get_task_count_for_cabin(id);
    weight += task_count * 20.0;
    
    // Бонус/штраф за направление движения
    if (_cur_directions[id] == DIR_STAND)
    {
        // Свободный лифт - бонус
        weight -= 15.0;
    }
    else
    {
        // Проверяем, совпадает ли направление движения с запросом
        int cur_distance = target_floor - (_cur_floor[id] + 1);
        direction_t move_direction = get_direction(cur_distance);
        
        if (_cur_directions[id] == move_direction && _cur_directions[id] == direction)
        {
            // Лифт движется в нужном направлении и может забрать пассажира по пути
            weight -= 25.0;
        }
        else if (_cur_directions[id] != move_direction)
        {
            weight += 30.0;
        }
    }
    
    // Штраф за смену направления движения
    if (_cur_directions[id] != DIR_STAND)
    {
        int cur_distance = target_floor - (_cur_floor[id] + 1);
        direction_t required_direction = get_direction(cur_distance);
        
        if (_cur_directions[id] != required_direction && required_direction != DIR_STAND)
        {
            weight += 15.0;
        }
    }
    
    return weight;
}

int Controller::find_next_floor_in_direction(cabin_id_t id, direction_t direction, int current_floor)
{
    if (direction == DIR_UP)
    {
        int furthest_down_call = FLOOR_NOT_FOUND;
        for (int floor = FLOOR_COUNT; floor >= current_floor + 1; floor--)
        {
            if (_task_manager.has_floor_call_down(id, floor))
            {
                furthest_down_call = floor;
            }
        }
        
        for (int floor = current_floor + 1; floor <= FLOOR_COUNT; floor++)
        {
            if (_task_manager.has_cabin_call(id, floor) || 
                _task_manager.has_floor_call_up(id, floor))
            {
                if (furthest_down_call != FLOOR_NOT_FOUND && floor < furthest_down_call)
                {
                    qInfo("Лифт %d: найден этаж %d по пути к конечной цели %d", id, floor, furthest_down_call);
                    return floor - 1;
                }
                else if (furthest_down_call == FLOOR_NOT_FOUND)
                {
                    qInfo("Лифт %d: найден этаж %d по ходу вверх", id, floor);
                    return floor - 1;
                }
            }
        }
        
        if (furthest_down_call != FLOOR_NOT_FOUND)
        {
            qInfo("Лифт %d: едем к конечной цели %d для разворота", id, furthest_down_call);
            return furthest_down_call - 1;
        }
    }
    else if (direction == DIR_DOWN)
    {
        int furthest_up_call = FLOOR_NOT_FOUND;
        for (int floor = 1; floor <= current_floor - 1; floor++)
        {
            if (_task_manager.has_floor_call_up(id, floor))
            {
                furthest_up_call = floor;
                break;
            }
        }
        
        for (int floor = current_floor - 1; floor >= 1; floor--)
        {
            if (_task_manager.has_cabin_call(id, floor) || 
                _task_manager.has_floor_call_down(id, floor))
            {
                if (furthest_up_call != FLOOR_NOT_FOUND && floor > furthest_up_call)
                {
                    qInfo("Лифт %d: найден этаж %d по пути к конечной цели %d", id, floor, furthest_up_call);
                    return floor - 1;
                }
                else if (furthest_up_call == FLOOR_NOT_FOUND)
                {
                    qInfo("Лифт %d: найден этаж %d по ходу вниз", id, floor);
                    return floor - 1;
                }
            }
        }
        
        if (furthest_up_call != FLOOR_NOT_FOUND)
        {
            qInfo("Лифт %d: едем к конечной цели %d для разворота", id, furthest_up_call);
            return furthest_up_call - 1;
        }
    }
    
    return FLOOR_NOT_FOUND;
}

Controller::~Controller()
{
}
