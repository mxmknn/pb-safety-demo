#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Calculations_Energy_window;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *calculationsButton;
    QPushButton *directoriesButton;
    QPushButton *settingsButton;
    QPushButton *helpButton;
    QPushButton *exitButton;

    Calculations_Energy_window *calculations_energy_Window = nullptr;

    void setupUi();
    void openCalculations_energy_Window();
};

#endif // MAINWINDOW_H