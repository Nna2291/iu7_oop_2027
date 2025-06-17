#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QPalette>
#include <QApplication>

#include "errors.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "actions.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    
    connect(ui->actionOpen_file, &QAction::triggered, this,
            &MainWindow::on_actionOpen_file_clicked);

    connect(ui->actionSave_file, &QAction::triggered, this,
            &MainWindow::on_actionSave_file_clicked);

    connect(ui->buttonScale, &QPushButton::clicked, this,
            &MainWindow::on_buttonScale_clicked);

    connect(ui->buttonMove, &QPushButton::clicked, this,
            &MainWindow::on_buttonMove_clicked);

    connect(ui->buttonRotate, &QPushButton::clicked, this,
            &MainWindow::on_buttonRotate_clicked);
}

MainWindow::~MainWindow()
{
    request_t request;
    request.action = EXIT;
    choose_option(request);
    delete ui;
}

static double to_radians(const double angle)
{
    return angle * (M_PI / 180);
}

t_error MainWindow::draw()
{
    auto rcontent = ui->graphicsView->contentsRect();
    ui->graphicsView->scene()->setSceneRect(0, 0, rcontent.width(),
                                            rcontent.height());

    request_t request;
    request.action = DRAW;
    request.view = {
        .scene = ui->graphicsView->scene(),
        .width = ui->graphicsView->scene()->width(),
        .height = ui->graphicsView->scene()->height(),
        .line_color = Qt::black
    };

    return choose_option(request);
}

void MainWindow::on_buttonScale_clicked()
{
    request_t request;
    request.action = SCALE;
    request.scale = {.kx = ui->scale_x_box->value(),
                     .ky = ui->scale_y_box->value(),
                     .kz = ui->scale_z_box->value()};

    t_error rc = choose_option(request);
    if (rc)
    {
        error_message(rc);
    }
    else
    {
        rc = draw();
        if (rc)
        {
            error_message(rc);
        }
    }
}

void MainWindow::on_buttonMove_clicked()
{
    request_t request;
    request.action = MOVE;
    request.move = {.dx = ui->move_x_box->value(),
                    .dy = -(ui->move_y_box->value()),
                    .dz = ui->move_z_box->value()};

    t_error rc = choose_option(request);
    if (rc)
    {
        error_message(rc);
    }
    else
    {
        rc = draw();
        if (rc)
        {
            error_message(rc);
        }
    }
}

void MainWindow::on_buttonRotate_clicked()
{
    request_t request;
    request.action = ROTATE;
    request.rotate = {.angle_x_rad = to_radians(ui->angle_x_box->value()),
                      .angle_y_rad = to_radians(ui->angle_y_box->value()),
                      .angle_z_rad = to_radians(ui->angle_z_boz->value())};

    t_error rc = choose_option(request);
    if (rc)
    {
        error_message(rc);
    }
    else
    {
        rc = draw();
        if (rc)
        {
            error_message(rc);
        }
    }
}

void MainWindow::on_actionOpen_file_clicked()
{
    QString path = QFileDialog::getOpenFileName();
    request_t request;
    request.action = LOAD;
    
    QByteArray ba = path.toLocal8Bit();
    request.fname = ba.data();

    t_error rc = choose_option(request);
    if (rc)
    {
        error_message(rc);
    }
    else
    {
        rc = draw();
        if (rc)
        {
            error_message(rc);
        }
    }
}

void MainWindow::on_actionSave_file_clicked()
{
    QString path = QFileDialog::getSaveFileName();

    request_t request;
    request.action = SAVE;
    request.fname = path.toUtf8().data();

    t_error rc = choose_option(request);
    if (rc)
    {
        error_message(rc);
    }
    else
    {
        rc = draw();
        if (rc)
        {
            error_message(rc);
        }
    }
}
