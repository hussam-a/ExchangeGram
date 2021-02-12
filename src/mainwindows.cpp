#include "mainwindows.h"
#include "ui_mainwindows.h"
#include "QMessageBox"
#include "home.h"
#include "includes.h"

MainWindows::MainWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindows)
{
    ui->setupUi(this);


}

MainWindows::~MainWindows()
{
    delete ui;
}

void MainWindows::on_registerButton_clicked()
{
    QString username = ui->usernameBox->text();
    QString password = ui->passwordBox->text();

    username.replace(" ", "");
    password.replace(" ", "");

    if(username.size() == 0 || password.size() == 0){
        QMessageBox::critical(this, "Error", "Either username and/or password field is empty.", "OK");
    }
    else if(password.size() < 6){
        QMessageBox::critical(this, "Error", "Password should be at least 6 digits long", "OK");
    }
    else{
        int status = myClient.UIregisterUsername(username.toStdString(),password.toStdString());
        if(status == 0)
        {
            this->hide();
            Home homeWindow;
            homeWindow.setModal(true);
            homeWindow.exec();
        }
        else if (status == -4)
        {
            QMessageBox::critical(this, "Fail" , "Connection error occured.","OK");
        }
        else
        {
            QMessageBox::critical(this,"Error", "Username already registered, choose another.", "OK");
        }
    }
}

void MainWindows::on_Login_clicked()
{
    QString username = ui->usernameBox->text();
    QString password = ui->passwordBox->text();

    username.replace(" ", "");
    password.replace(" ", "");

    if(username.size() == 0 || password.size() == 0){
        QMessageBox::critical(this, "Error", "Either username and/or password field is empty.", "OK");
    }
    else if(password.size() < 6){
        QMessageBox::critical(this, "Error", "Password should be at least 6 digits long", "OK");
    }
    else{

        int status = myClient.UILoginUser(username.toStdString(),password.toStdString());
        if(status == 0)
        {
            this->hide();
            Home homeWindow;
            homeWindow.setModal(true);
            homeWindow.exec();
        }
        else if (status == -4)
        {
            QMessageBox::critical(this, "Fail" , "Connection error occured.","OK");
        }
        else
        {
            QMessageBox::critical(this,"Error", "Username already registered, choose another.", "OK");
        }
    }
}
