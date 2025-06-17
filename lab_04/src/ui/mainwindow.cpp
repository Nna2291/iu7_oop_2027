#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../defines.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QGroupBox>
#include <memory>

MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    layout_(new QVBoxLayout())
{
    ui->setupUi(this);
    
    this->setWindowTitle("Система управления лифтами - 2 лифта");
    this->setStyleSheet("background-color: #f0f0f0; font-family: Arial, sans-serif;");
    
    ui->centralwidget->setLayout(layout_.get());
    
    // Заголовок
    auto titleLabel = new QLabel("СИСТЕМА УПРАВЛЕНИЯ ЛИФТАМИ");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #000000; padding: 20px; text-align: center;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout_->addWidget(titleLabel);
    
    // Основной контейнер
    auto mainWidget = new QWidget();
    auto mainLayout = new QHBoxLayout(mainWidget);
    mainLayout->setSpacing(30);
    
    // Секция вызова лифтов (кнопки на этажах)
    auto callSection = new QGroupBox("");
    callSection->setStyleSheet(
        "QGroupBox {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #000000;"
        "    border: 2px solid #808080;"
        "    border-radius: 10px;"
        "    margin-top: 15px;"
        "    padding: 10px;"
        "    background-color: white;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 10px 0 10px;"
        "    background-color: #808080;"
        "    color: white;"
        "    border-radius: 5px;"
        "}"
    );
    
    auto callLayout = new QGridLayout(callSection);
    
    // Заголовки для секции вызова
    auto floorHeader = new QLabel("Этаж");
    floorHeader->setStyleSheet("font-weight: bold; font-size: 14px; color: #000000; padding: 5px;");
    floorHeader->setAlignment(Qt::AlignCenter);
    
    auto upHeader = new QLabel("↑ Вверх");
    upHeader->setStyleSheet("font-weight: bold; font-size: 14px; color: #000000; padding: 5px;");
    upHeader->setAlignment(Qt::AlignCenter);
    
    auto downHeader = new QLabel("↓ Вниз");
    downHeader->setStyleSheet("font-weight: bold; font-size: 14px; color: #000000; padding: 5px;");
    downHeader->setAlignment(Qt::AlignCenter);
    
    callLayout->addWidget(floorHeader, 0, 0);
    callLayout->addWidget(upHeader, 0, 1);
    callLayout->addWidget(downHeader, 0, 2);
    
    // Кнопки вызова по этажам
    for (int floor = FLOOR_COUNT; floor >= 1; floor--) {
        int row = FLOOR_COUNT - floor + 1;
        
        // Номер этажа
        auto floorLabel = new QLabel(QString("Этаж %1").arg(floor));
        floorLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #000000; "
            "background-color: #f0f0f0; border-radius: 5px; padding: 8px; text-align: center;"
        );
        floorLabel->setAlignment(Qt::AlignCenter);
        callLayout->addWidget(floorLabel, row, 0);
        
        // Кнопка вверх (кроме последнего этажа)
        if (floor != FLOOR_COUNT) {
            auto upButton = new QPushButton("↑");
            upButton->setObjectName(QString("floor%1_up").arg(floor));
            upButton->setStyleSheet(
                "QPushButton {"
                "    background-color: #d0d0d0;"
                "    border: 2px solid #808080;"
                "    border-radius: 15px;"
                "    font-size: 20px;"
                "    color: #000000;"
                "    min-width: 50px;"
                "    min-height: 50px;"
                "    max-width: 50px;"
                "    max-height: 50px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #b0b0b0;"
                "}"
                "QPushButton:pressed {"
                "    background-color: #909090;"
                "}"
            );
            callLayout->addWidget(upButton, row, 1);
            
            QObject::connect(upButton, &QPushButton::clicked, [=, this]() { 
                my_lift_system.manage_floor_call_slot(floor, DIR_UP); 
            });
        }
        
        // Кнопка вниз (кроме первого этажа)
        if (floor > 1) {
            auto downButton = new QPushButton("↓");
            downButton->setObjectName(QString("floor%1_down").arg(floor));
            downButton->setStyleSheet(
                "QPushButton {"
                "    background-color: #d0d0d0;"
                "    border: 2px solid #808080;"
                "    border-radius: 15px;"
                "    font-size: 20px;"
                "    color: #000000;"
                "    min-width: 50px;"
                "    min-height: 50px;"
                "    max-width: 50px;"
                "    max-height: 50px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #b0b0b0;"
                "}"
                "QPushButton:pressed {"
                "    background-color: #909090;"
                "}"
            );
            callLayout->addWidget(downButton, row, 2);
            
            QObject::connect(downButton, &QPushButton::clicked, [=, this]() { 
                my_lift_system.manage_floor_call_slot(floor, DIR_DOWN); 
            });
        }
    }
    
    // Секция управления лифтами
    auto liftsSection = new QWidget();
    auto liftsLayout = new QHBoxLayout(liftsSection);
    liftsLayout->setSpacing(20);
    
    // Лифт 1
    auto lift1Section = new QGroupBox("");
    lift1Section->setStyleSheet(
        "QGroupBox {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #000000;"
        "    border: 2px solid #606060;"
        "    border-radius: 10px;"
        "    margin-top: 15px;"
        "    padding: 10px;"
        "    background-color: white;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 10px 0 10px;"
        "    background-color: #606060;"
        "    color: white;"
        "    border-radius: 5px;"
        "}"
    );
    
    auto lift1Layout = new QGridLayout(lift1Section);
    
    // Лифт 2
    auto lift2Section = new QGroupBox("");
    lift2Section->setStyleSheet(
        "QGroupBox {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #000000;"
        "    border: 2px solid #404040;"
        "    border-radius: 10px;"
        "    margin-top: 15px;"
        "    padding: 10px;"
        "    background-color: white;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 10px 0 10px;"
        "    background-color: #404040;"
        "    color: white;"
        "    border-radius: 5px;"
        "}"
    );
    
    auto lift2Layout = new QGridLayout(lift2Section);
    
    // Кнопки этажей в лифтах
    for (int floor = FLOOR_COUNT; floor >= 1; floor--) {
        int row = FLOOR_COUNT - floor;
        
        // Кнопки лифта 1
        auto lift1Button = new QPushButton(QString::number(floor));
        lift1Button->setObjectName(QString("el1_floor%1").arg(floor));
        lift1Button->setStyleSheet(
            "QPushButton {"
            "    background-color: #e0e0e0;"
            "    border: 2px solid #808080;"
            "    border-radius: 10px;"
            "    font-size: 16px;"
            "    font-weight: bold;"
            "    color: #000000;"
            "    min-width: 60px;"
            "    min-height: 40px;"
            "    margin: 2px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #c0c0c0;"
            "    color: #000000;"
            "    border-color: #606060;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #a0a0a0;"
            "}"
        );
        lift1Layout->addWidget(lift1Button, row, 0);
        
        QObject::connect(lift1Button, &QPushButton::clicked, [=, this]() { 
            my_lift_system.manage_cabin_call_slot(floor, CID_FIRST); 
        });
        
        // Кнопки лифта 2
        auto lift2Button = new QPushButton(QString::number(floor));
        lift2Button->setObjectName(QString("el2_floor%1").arg(floor));
        lift2Button->setStyleSheet(
            "QPushButton {"
            "    background-color: #d0d0d0;"
            "    border: 2px solid #707070;"
            "    border-radius: 10px;"
            "    font-size: 16px;"
            "    font-weight: bold;"
            "    color: #000000;"
            "    min-width: 60px;"
            "    min-height: 40px;"
            "    margin: 2px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #b0b0b0;"
            "    color: #000000;"
            "    border-color: #505050;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #909090;"
            "}"
        );
        lift2Layout->addWidget(lift2Button, row, 0);
        
        QObject::connect(lift2Button, &QPushButton::clicked, [=, this]() { 
            my_lift_system.manage_cabin_call_slot(floor, CID_SECOND); 
        });
    }
    
    liftsLayout->addWidget(lift1Section);
    liftsLayout->addWidget(lift2Section);
    
    // Добавляем секции в основной макет
    mainLayout->addWidget(callSection);
    mainLayout->addWidget(liftsSection);
    
    layout_->addWidget(mainWidget);
    

    
    // Подключаем сигналы для изменения цвета кнопок
    QObject::connect(&my_lift_system, &LiftSystem::floor_buttons_change_color_signal, 
                     this, &MainWindow::change_floor_button_style_slot);
    QObject::connect(&my_lift_system, &LiftSystem::cabin_buttons_change_color_signal, 
                     this, &MainWindow::change_cabin_button_style_slot);
    QObject::connect(&my_lift_system, &LiftSystem::cabin_position_changed_signal,
                     this, &MainWindow::update_cabin_position_slot);
    
    // Принудительно устанавливаем подсветку первых этажей при запуске
    for (int i = 0; i < CABINS_COUNT; i++) {
        update_cabin_position_slot(static_cast<cabin_id_t>(i), START_FLOOR);
    }
}

void MainWindow::change_floor_button_style_slot(int floor, direction_t direction, bool is_active)
{
    QPushButton *button = nullptr;
    if (direction == DIR_UP)
    {
        button = findChild<QPushButton *>(QString("floor%1_up").arg(floor));
    }
    else if (direction == DIR_DOWN)
    {
        button = findChild<QPushButton *>(QString("floor%1_down").arg(floor));
    }

    if (button)
    {
        if (is_active)
        {
            // Для активных кнопок используем темный цвет
            button->setStyleSheet(
                "QPushButton {"
                "    background-color: #404040;"
                "    border: 3px solid #202020;"
                "    border-radius: 15px;"
                "    font-size: 20px;"
                "    color: white;"
                "    min-width: 50px;"
                "    min-height: 50px;"
                "    max-width: 50px;"
                "    max-height: 50px;"
                "}"
            );
        }
        else
        {
            // Для неактивных кнопок используем светлый цвет
            button->setStyleSheet(
                "QPushButton {"
                "    background-color: #d0d0d0;"
                "    border: 2px solid #808080;"
                "    border-radius: 15px;"
                "    font-size: 20px;"
                "    color: #000000;"
                "    min-width: 50px;"
                "    min-height: 50px;"
                "    max-width: 50px;"
                "    max-height: 50px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #b0b0b0;"
                "}"
                "QPushButton:pressed {"
                "    background-color: #909090;"
                "}"
            );
        }
    }
    this->update();
}

void MainWindow::change_cabin_button_style_slot(int floor, cabin_id_t cabin_id, bool is_active)
{
    QPushButton *button = nullptr;
    if (cabin_id == CID_FIRST)
    {
        button = findChild<QPushButton *>(QString("el1_floor%1").arg(floor));
    }
    else if (cabin_id == CID_SECOND)
    {
        button = findChild<QPushButton *>(QString("el2_floor%1").arg(floor));
    }

    if (button)
    {
        bool is_current_floor = (current_cabin_floors_[cabin_id] == floor);
        
        if (is_active)
        {
            if (is_current_floor) {
                // Активная кнопка на текущем этаже - голубая
                button->setStyleSheet(
                    "QPushButton {"
                    "    background-color: #00aaff;"
                    "    border: 3px solid #0088cc;"
                    "    border-radius: 10px;"
                    "    font-size: 16px;"
                    "    font-weight: bold;"
                    "    color: white;"
                    "    min-width: 60px;"
                    "    min-height: 40px;"
                    "    margin: 2px;"
                    "}"
                );
            } else {
                // Активная кнопка не на текущем этаже - темная
                button->setStyleSheet(
                    "QPushButton {"
                    "    background-color: #404040;"
                    "    border: 3px solid #202020;"
                    "    border-radius: 10px;"
                    "    font-size: 16px;"
                    "    font-weight: bold;"
                    "    color: white;"
                    "    min-width: 60px;"
                    "    min-height: 40px;"
                    "    margin: 2px;"
                    "}"
                );
            }
        }
        else
        {
            if (is_current_floor) {
                // Неактивная кнопка на текущем этаже - светло-голубая
                button->setStyleSheet(
                    "QPushButton {"
                    "    background-color: #87ceeb;"
                    "    border: 2px solid #4682b4;"
                    "    border-radius: 10px;"
                    "    font-size: 16px;"
                    "    font-weight: bold;"
                    "    color: #000000;"
                    "    min-width: 60px;"
                    "    min-height: 40px;"
                    "    margin: 2px;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: #00aaff;"
                    "    color: white;"
                    "    border-color: #0088cc;"
                    "}"
                    "QPushButton:pressed {"
                    "    background-color: #0088cc;"
                    "}"
                );
            } else {
                // Неактивная кнопка не на текущем этаже - обычная
                QString hoverColor = (cabin_id == CID_FIRST) ? "#c0c0c0" : "#b0b0b0";
                QString pressedColor = (cabin_id == CID_FIRST) ? "#a0a0a0" : "#909090";
                
                button->setStyleSheet(
                    "QPushButton {"
                    "    background-color: " + (cabin_id == CID_FIRST ? QString("#e0e0e0") : QString("#d0d0d0")) + ";"
                    "    border: 2px solid " + (cabin_id == CID_FIRST ? QString("#808080") : QString("#707070")) + ";"
                    "    border-radius: 10px;"
                    "    font-size: 16px;"
                    "    font-weight: bold;"
                    "    color: #000000;"
                    "    min-width: 60px;"
                    "    min-height: 40px;"
                    "    margin: 2px;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: " + hoverColor + ";"
                    "    color: #000000;"
                    "    border-color: " + (cabin_id == CID_FIRST ? QString("#606060") : QString("#505050")) + ";"
                    "}"
                    "QPushButton:pressed {"
                    "    background-color: " + pressedColor + ";"
                    "}"
                );
            }
        }
    }
    this->update();
}

void MainWindow::update_cabin_position_slot(cabin_id_t cabin_id, int floor)
{
    // Сбрасываем подсветку с предыдущего этажа
    int old_floor = current_cabin_floors_[cabin_id];
    if (old_floor >= 1 && old_floor <= FLOOR_COUNT && old_floor != floor) {
        QPushButton *old_button = nullptr;
        if (cabin_id == CID_FIRST) {
            old_button = findChild<QPushButton *>(QString("el1_floor%1").arg(old_floor));
        } else {
            old_button = findChild<QPushButton *>(QString("el2_floor%1").arg(old_floor));
        }
        
        if (old_button) {
            // Возвращаем обычный стиль (проверяем, не активна ли кнопка)
            QString objectName = old_button->objectName();
            // Если кнопка темная (активная), оставляем темной, иначе делаем обычной
            
            if (!old_button->styleSheet().contains("#404040"))
            {
                // Кнопка неактивна, делаем обычной
                QString hoverColor = (cabin_id == CID_FIRST) ? "#c0c0c0" : "#b0b0b0";
                QString pressedColor = (cabin_id == CID_FIRST) ? "#a0a0a0" : "#909090";
                
                old_button->setStyleSheet(
                    "QPushButton {"
                    "    background-color: " + (cabin_id == CID_FIRST ? QString("#e0e0e0") : QString("#d0d0d0")) + ";"
                    "    border: 2px solid " + (cabin_id == CID_FIRST ? QString("#808080") : QString("#707070")) + ";"
                    "    border-radius: 10px;"
                    "    font-size: 16px;"
                    "    font-weight: bold;"
                    "    color: #000000;"
                    "    min-width: 60px;"
                    "    min-height: 40px;"
                    "    margin: 2px;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: " + hoverColor + ";"
                    "    color: #000000;"
                    "    border-color: " + (cabin_id == CID_FIRST ? QString("#606060") : QString("#505050")) + ";"
                    "}"
                    "QPushButton:pressed {"
                    "    background-color: " + pressedColor + ";"
                    "}"
                );
            }
        }
    }
    
    // Обновляем текущий этаж
    current_cabin_floors_[cabin_id] = floor;
    
    // Подсвечиваем новый этаж голубым цветом
    if (floor >= 1 && floor <= FLOOR_COUNT) {
        QPushButton *current_button = nullptr;
        if (cabin_id == CID_FIRST) {
            current_button = findChild<QPushButton *>(QString("el1_floor%1").arg(floor));
        } else {
            current_button = findChild<QPushButton *>(QString("el2_floor%1").arg(floor));
        }
        
        if (current_button) {
            // Проверяем, не активна ли кнопка (темная)
            if (current_button->styleSheet().contains("#404040")) {
                // Кнопка активна, делаем голубой с белым текстом
                current_button->setStyleSheet(
                    "QPushButton {"
                    "    background-color: #00aaff;"  // Голубой цвет
                    "    border: 3px solid #0088cc;"
                    "    border-radius: 10px;"
                    "    font-size: 16px;"
                    "    font-weight: bold;"
                    "    color: white;"
                    "    min-width: 60px;"
                    "    min-height: 40px;"
                    "    margin: 2px;"
                    "}"
                );
            } else {
                // Кнопка неактивна, делаем светло-голубой
                current_button->setStyleSheet(
                    "QPushButton {"
                    "    background-color: #87ceeb;"  // Светло-голубой
                    "    border: 2px solid #4682b4;"
                    "    border-radius: 10px;"
                    "    font-size: 16px;"
                    "    font-weight: bold;"
                    "    color: #000000;"
                    "    min-width: 60px;"
                    "    min-height: 40px;"
                    "    margin: 2px;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: #00aaff;"
                    "    color: white;"
                    "    border-color: #0088cc;"
                    "}"
                    "QPushButton:pressed {"
                    "    background-color: #0088cc;"
                    "}"
                );
            }
        }
    }
    
    this->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}