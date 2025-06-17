#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "errors.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void on_actionOpen_file_clicked();

    void on_actionSave_file_clicked();

private slots:

    t_error draw();

    void on_buttonScale_clicked();

    void on_buttonMove_clicked();

    void on_buttonRotate_clicked();


private:
    Ui::MainWindow *ui;
    bool isDarkTheme = true;
};

#endif // MAINWINDOW_H
