#ifndef STARTHERE_H
#define STARTHERE_H

#include <QMainWindow>
#include "QStackedWidget"
#include "client.h"


extern client myClient;

namespace Ui {
class startHere;
}

class startHere : public QMainWindow
{
    Q_OBJECT

public:
    explicit startHere(QWidget *parent = 0);
    ~startHere();

private slots:
    void on_submit_clicked();

    void on_choice_clicked();


private:
    Ui::startHere *ui;

//    QWidget *firstPageWidget = new QWidget;
//    QWidget *secondPageWidget = new QWidget;
//    QWidget *thirdPageWidget = new QWidget;

//    QStackedWidget *stackedWidget = new QStackedWidget;
};

#endif // STARTHERE_H
