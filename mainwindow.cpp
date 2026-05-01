#include "mainwindow.h"
#include "calculations_of_energy_potentials_window.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::openCalculations_energy_Window()
{
    if (calculations_energy_Window != nullptr) {
        calculations_energy_Window->show();
        calculations_energy_Window->raise();
        calculations_energy_Window->activateWindow();
        return;
    }

    calculations_energy_Window = new Calculations_Energy_window();
    calculations_energy_Window->setAttribute(Qt::WA_DeleteOnClose);

    connect(calculations_energy_Window, &QObject::destroyed, this, [this]() {
        calculations_energy_Window = nullptr;
        this->show();
    });

    calculations_energy_Window->show();
    this->hide();
}

void MainWindow::setupUi()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setWindowTitle("ПВ-БЕЗОПАСНОСТЬ");
    resize(600, 420);

    QLabel *titleLabel = new QLabel("ПВ-БЕЗОПАСНОСТЬ", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QFont titleFont;
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QLabel *subtitleLabel = new QLabel("Главное меню", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);

    QFont subtitleFont;
    subtitleFont.setPointSize(14);
    subtitleLabel->setFont(subtitleFont);

    calculationsButton = new QPushButton("Расчеты энергопотенциалов и категорий взрывоопасности", this);
    directoriesButton = new QPushButton("Справочники", this);
    settingsButton = new QPushButton("Настройки", this);
    helpButton = new QPushButton("Помощь", this);
    exitButton = new QPushButton("Выход", this);

    calculationsButton->setMinimumHeight(45);
    directoriesButton->setMinimumHeight(45);
    settingsButton->setMinimumHeight(45);
    helpButton->setMinimumHeight(45);
    exitButton->setMinimumHeight(45);

    calculationsButton->setMinimumWidth(480);
    directoriesButton->setMinimumWidth(480);
    settingsButton->setMinimumWidth(480);
    helpButton->setMinimumWidth(480);
    exitButton->setMinimumWidth(480);

    QVBoxLayout *menuLayout = new QVBoxLayout;
    menuLayout->addStretch();
    menuLayout->addWidget(titleLabel);
    menuLayout->addSpacing(10);
    menuLayout->addWidget(subtitleLabel);
    menuLayout->addSpacing(30);
    menuLayout->addWidget(calculationsButton, 0, Qt::AlignCenter);
    menuLayout->addWidget(directoriesButton, 0, Qt::AlignCenter);
    menuLayout->addWidget(settingsButton, 0, Qt::AlignCenter);
    menuLayout->addWidget(helpButton, 0, Qt::AlignCenter);
    menuLayout->addWidget(exitButton, 0, Qt::AlignCenter);
    menuLayout->addStretch();

    central->setLayout(menuLayout);
    // connect(calculationsButton, &QPushButton::clicked, this, [this]() {
    //     setWindowTitle("Нажата кнопка расчетов");
    // });

    // connect(directoriesButton, &QPushButton::clicked, this, [this]() {
    //     setWindowTitle("Нажата кнопка справочников");
    // });

    // connect(settingsButton, &QPushButton::clicked, this, [this]() {
    //     setWindowTitle("Нажата кнопка настроек");
    // });

    // connect(helpButton, &QPushButton::clicked, this, [this]() {
    //     setWindowTitle("Нажата кнопка помощи");
    // });

    // connect(exitButton, &QPushButton::clicked, this, [this]() {
    //     setWindowTitle("Нажата кнопка выхода");
    // });
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);

    connect(calculationsButton, &QPushButton::clicked,
            this, &MainWindow::openCalculations_energy_Window);
    connect(directoriesButton, &QPushButton::clicked, this, [this]() {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Справочники");
        msgBox.setText("Раздел справочники пока не реализован.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    });

    connect(settingsButton, &QPushButton::clicked, this, [this]() {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Настройки");
        msgBox.setText("Раздел настройки пока не реализован.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    });

    connect(helpButton, &QPushButton::clicked, this, [this]() {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Помощь");
        msgBox.setText("Раздел помощи пока не реализован.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    });
}