#include "mainwindow.hpp"

#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <cmath>

#include "MatrixLoadCommand.h"
#include "ListLoadCommand.h"
#include "SqliteLoadCommandDecorator.h"
#include "PostgresqlLoadCommandDecorator.h"
#include "TxtLoadCommandDecorator.h"
#include "DrawSceneQtCommand.h"
#include "GetCameraIDsSceneCommand.h"
#include "GetObjectIDsSceneCommand.h"
#include "AddCameraCommand.h"
#include "SetCameraCommand.h"
#include "RemoveCameraCommand.h"
#include "ShiftObjectCommand.h"
#include "RotateObjectCommand.h"
#include "ScaleObjectCommand.h"
#include "CompositeObjectCommand.h"
#include "RemoveObjectCommand.h"
#include "Point.h"
#include "baseexception.h"
#include "BaseHistoryCommand.h"
#include "ToggleFaceCullingCommand.h"

double DegToRad(double angle) { return angle / 180.0 * M_PI; }


MyMainWindow::MyMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setBackgroundBrush(Qt::white);
    ui->graphicsView->scene()->setSceneRect(ui->graphicsView->sceneRect());
}

MyMainWindow::~MyMainWindow() {
    delete ui;
}

void MyMainWindow::on_fileSelectButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Выберите файл"), "",
        tr("Все файлы (*);;Текстовые файлы (*.txt);;База данных SQLite (*.db *.sqlite)"));
    
    if (!fileName.isEmpty()) {
        _selectedFilePath = fileName;
        ui->fileSelectButton->setText(QFileInfo(fileName).fileName());
    }
}

void MyMainWindow::on_loadPushButton_clicked() {
    if (_selectedFilePath.isEmpty()) {
        QMessageBox::critical(nullptr, "Ошибка", "Сначала выберите файл.");
        return;
    }
    
    std::string str = _selectedFilePath.toStdString();
    const char* fname = str.c_str();
    
    std::shared_ptr<BaseLoadCommand> command;

    if (ui->listRadioButton->isChecked())
        command = std::make_shared<ListLoadCommand>();
    else
        command = std::make_shared<MatrixLoadCommand>();
    
    std::shared_ptr<BaseCommand> decorator;
    if (ui->sqliteRadioButton->isChecked())
        decorator = std::make_shared<SqliteLoadCommandDecorator>(*command, fname);
    else if (ui->postgresqlRadioButton->isChecked())
        decorator = std::make_shared<PostgresqlLoadCommandDecorator>(*command, fname);
    else
        decorator = std::make_shared<TxtLoadCommandDecorator>(*command, fname);
    try {
        _facade.Execute(*decorator);
    } catch (BaseException &exc) {
        QMessageBox::critical(nullptr, "Ошибка", exc.what());
        return;
    }
    
    drawScene();
    updateObjectList();
}

void MyMainWindow::on_cameraAddPushbutton_clicked() {
    double x = ui->cameraXSpin->value();
    double y = ui->cameraYSpin->value();
    double z = ui->cameraZSpin->value();
    Point pos(x, y, z);
    AddCameraCommand command(pos);
    _facade.Execute(command);
    updateCameraList();
    updateObjectList();
}

void MyMainWindow::on_cameraDeletePushbutton_clicked() {
    auto cams = getSelectedCameraIds();
    if (cams.size() == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Нужно выбрать хотя бы одну камеру.");
        return;
    }
    for (size_t id : cams) {
        RemoveCameraCommand command(id);
        _facade.Execute(command);
    }
    updateCameraList();
    updateObjectList();
}

void MyMainWindow::on_cameraSetPushbutton_clicked() {
    auto cams = getSelectedCameraIds();
    if (cams.size() != 1) {
        QMessageBox::critical(nullptr, "Ошибка", "Нужно выбрать ровно одну камеру.");
        return;
    }   
    SetCameraCommand command(cams[0]);
    _facade.Execute(command);
    drawScene();
}

void MyMainWindow::updateCameraList() {
    ui->cameraListWidget->clear();
    GetCameraIDsSceneCommand ids;
    _facade.Execute(ids);
    std::vector<size_t> cameraIds = ids.GetIDs();
    for (size_t id : cameraIds) {
        ui->cameraListWidget->addItem(QString::number(id));
    }
}

void MyMainWindow::updateObjectList() {
    ui->objectListWidget->clear();
    GetObjectIDsSceneCommand ids;
    _facade.Execute(ids);
    std::vector<size_t> objectIds = ids.GetIDs();
    for (size_t id : objectIds) {
        ui->objectListWidget->addItem(QString::number(id));
    }
}

std::vector<size_t> MyMainWindow::getSelectedObjectIds() {
    std::vector<size_t> ids;
    for (int i = 0; i < ui->objectListWidget->count(); i++) {
        if (ui->objectListWidget->item(i)->isSelected()) {
            ids.push_back(ui->objectListWidget->item(i)->text().toInt());
        }
    }
    return ids;
}

std::vector<size_t> MyMainWindow::getSelectedCameraIds() {
    std::vector<size_t> ids;
    for (int i = 0; i < ui->cameraListWidget->count(); i++) {
        if (ui->cameraListWidget->item(i)->isSelected()) {
            ids.push_back(ui->cameraListWidget->item(i)->text().toInt());
        }
    }
    return ids;
}

void MyMainWindow::drawScene() {
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->setSceneRect(ui->graphicsView->sceneRect());
    DrawSceneQtCommand drawcommand(ui->graphicsView->scene());
    _facade.Execute(drawcommand);
}

void MyMainWindow::on_objectMovePushbutton_clicked() {
    auto objs = getSelectedObjectIds();
    if (objs.size() == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Нужно выбрать хотя бы один объект.");
        return;
    }
    
    saveStateBeforeTransform(objs);
    
    double x = ui->obkectMoveXSpin->value();
    double y = ui->objectMoveYSpin->value();
    double z = ui->objectMoveZSpin->value();
    for (size_t id : objs) {
        ShiftObjectCommand command(id, x, y, z);
        _facade.Execute(command);
    }
    drawScene();
}

void MyMainWindow::on_objectRotatePushbutton_clicked() {
    auto objs = getSelectedObjectIds();
    if (objs.size() == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Нужно выбрать хотя бы один объект.");
        return;
    }
    
    saveStateBeforeTransform(objs);
    
    double x = DegToRad(ui->objectRotateXSpin->value());
    double y = DegToRad(ui->objectRotateYSpin->value());
    double z = DegToRad(ui->objectRotateZSpin->value());
    for (size_t id : objs) {
        RotateObjectCommand command(id, x, y, z);
        _facade.Execute(command);
    }
    drawScene();
}

void MyMainWindow::on_objectScalePushbutton_clicked() {
    auto objs = getSelectedObjectIds();
    if (objs.size() == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Нужно выбрать хотя бы один объект.");
        return;
    }
    
    saveStateBeforeTransform(objs);
    
    double x = ui->objectScaleXSpin->value();
    double y = ui->objectScaleYSpin->value();
    double z = ui->objectScaleZSpin->value();
    for (size_t id : objs) {
        ScaleObjectCommand command(id, x, y, z);
        _facade.Execute(command);
    }
    drawScene();
}

void MyMainWindow::on_objectDeletePushbutton_clicked() {
    auto objs = getSelectedObjectIds();
    if (objs.size() == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Нужно выбрать хотя бы один объект.");
        return;
    }
    for (auto &id : objs) {
        RemoveObjectCommand command(id);
        _facade.Execute(command);
    }

    drawScene();
    updateCameraList();
    updateObjectList();
}

void MyMainWindow::on_objectCompositePushbutton_clicked() { 
    auto objs = getSelectedObjectIds();
    if (objs.size() == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Нужно выбрать хотя бы один объект.");
        return;
    }
    CompositeObjectCommand command(objs);
    _facade.Execute(command);
    drawScene();
}

void MyMainWindow::saveStateBeforeTransform(const std::vector<size_t>& objectIds) {
    for (size_t id : objectIds) {
        SaveStateCommand saveCommand(id);
        _facade.Execute(saveCommand);
    }
}

void MyMainWindow::on_undoLastTransformPushbutton_clicked() {
    auto objs = getSelectedObjectIds();
    if (objs.size() == 0) {
        QMessageBox::critical(nullptr, "Ошибка", "Нужно выбрать хотя бы один объект.");
        return;
    }
    
    for (size_t id : objs) {
        RestoreStateCommand restoreCommand(id);
        _facade.Execute(restoreCommand);
    }
    
    drawScene();
}

void MyMainWindow::on_toggleFaceCullingPushbutton_clicked() {
    ToggleFaceCullingCommand toggleCommand;
    _facade.Execute(toggleCommand);
    
    drawScene();
}
