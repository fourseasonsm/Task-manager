#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Main Window");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *topPart = new QLabel();
    topPart->setStyleSheet("background-color: white;");
    topPart->setFixedHeight(50);
    topPart->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(topPart);

    QLabel *overlay_usr = new QLabel("Пользователи онлайн", this);
    overlay_usr->setStyleSheet("background-color: rgba(0, 0.5, 0, 0);");
    overlay_usr->setFixedSize(150, 20);
    overlay_usr->move(554, 35);
    overlay_usr->raise();
    QLabel *overlay_prf = new QLabel("Профиль", this);
    overlay_prf->setStyleSheet("background-color: rgba(0, 0.5, 0, 0);");
    overlay_prf->setFixedSize(150, 20);
    overlay_prf->move(45, 35);
    overlay_prf->raise();
    QLabel *overlay_tsk = new QLabel("Текущие задачи", this);
    overlay_tsk->setStyleSheet("background-color: rgba(0, 0.5, 0, 0);");
    overlay_tsk->setFixedSize(150, 20);
    overlay_tsk->move(300, 35);
    overlay_tsk->raise();


    /*QLabel *topleftStripe = new QLabel();
    topleftStripe->setStyleSheet("background-color: lightgreen;");
    mainLayout->addWidget(topleftStripe);
    QLabel *topcenterStripe = new QLabel();
    topcenterStripe->setStyleSheet("background-color: white;");
    mainLayout->addWidget(topcenterStripe);
    QLabel *toprightStripe = new QLabel();
    toprightStripe->setStyleSheet("background-color: lightgreen;");
    mainLayout->addWidget(toprightStripe);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 3);
    mainLayout->setStretch(2, 1);*/

    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setLineWidth(2);
    separator->setStyleSheet("color: black;");
    mainLayout->addWidget(separator);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    QLabel *bottomleftStripe = new QLabel();
    bottomleftStripe->setStyleSheet("background-color: lightgreen;");
    bottomLayout->addWidget(bottomleftStripe);
    QLabel *bottomcenterStripe = new QLabel();
    bottomcenterStripe->setStyleSheet("background-color: white;");
    bottomLayout->addWidget(bottomcenterStripe);
    QLabel *bottomrightStripe = new QLabel(this);
    bottomrightStripe->setStyleSheet("background-color: lightgreen;");
    bottomLayout->addWidget(bottomrightStripe);

    // Устанавливаем размеры полос
    bottomLayout->setStretch(0, 2);
    bottomLayout->setStretch(1, 5);
    bottomLayout->setStretch(2, 2);
    mainLayout->addLayout(bottomLayout);
}


