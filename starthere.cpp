#include "starthere.h"
#include "ui_starthere.h"
#include "mainwindows.h"
#include <string>
#include <QMessageBox>
#include "includes.h"
#include "home.h"
#include "serverapplication.h"

startHere::startHere(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startHere)
{
    ui->setupUi(this);
}

startHere::~startHere()
{
    delete ui;
}

void startHere::on_submit_clicked()
{
    QString serverIP = ui->IP_box->text();
    QString serverPort = ui->port_box->text();

    int port = stoi(serverPort.toStdString());

    if(serverIP.size() == 0 || serverPort.size() == 0){
        QMessageBox::critical(this, "Error", "Either Ip or port field is empty.", "OK");
        //stackedWidget->setCurrentIndex( 1 );
    }
    else{
        if(ui->choice->isChecked()){
            char * IPtoSend = new char[serverIP.toStdString().size()];
            strcpy(IPtoSend,serverIP.toStdString().c_str());
            serverApplication(IPtoSend,port);
        }
        else{
            int status = myClient.UIstartClient(serverIP.toStdString(), port);
            if (status == 1)
            {
                QMessageBox::critical(this, "Fail" , "Connection error occured.","OK");
            }
            else
            {
            this->hide();
            MainWindows mainWindow;
            mainWindow.setModal(true);
            mainWindow.exec();
            }
        }

    }
}

void startHere::on_choice_clicked()
{

}

