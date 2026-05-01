#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

class Calculations_Energy_window;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *calculationsButton = nullptr;
    QPushButton *directoriesButton = nullptr;
    QPushButton *settingsButton = nullptr;
    QPushButton *helpButton = nullptr;
    QPushButton *exitButton = nullptr;

    Calculations_Energy_window *calculations_energy_Window = nullptr;

    void setupUi();
    void openCalculations_energy_Window();
};

#endif