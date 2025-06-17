#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <memory>

#include "../elevator/elevator.h"
#include "../defines.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void change_floor_button_style_slot(int floor, direction_t direction, bool is_active);
    void change_cabin_button_style_slot(int floor, cabin_id_t cabin_id, bool is_active);
    void update_cabin_position_slot(cabin_id_t cabin_id, int floor);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QVBoxLayout> layout_;
    LiftSystem my_lift_system;
    
    // Отслеживание текущих этажей лифтов для подсветки (0 означает неинициализировано)
    int current_cabin_floors_[CABINS_COUNT] = {0, 0}; 
};

#endif