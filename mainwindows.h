#ifndef MAINWINDOWS_H
#define MAINWINDOWS_H

#include "client.h"
#include "testing_ui.h"
#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QFileDialog>


extern client myClient;

namespace Ui {
class MainWindows;
}

class MainWindows : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindows(QWidget *parent = 0);
    ~MainWindows();

private slots:
    void on_registerButton_clicked();


    void on_Login_clicked();

private:
    Ui::MainWindows *ui;
};

#endif // MAINWINDOWS_H
