#include "home.h"
#include "ui_home.h"
#include "image.h"
#include "QMessageBox"
#include "includes.h"
#include "mainwindows.h"
#include "qcombobox.h"
using namespace std;

Home::Home(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    fillLists();

    myClient.UIsetDefaultImage("./default.png");
    QString temp = QString::fromStdString(myClient.UIgetUserName());
    ui->welcome->setText(temp);
}

Home::~Home()
{
    delete ui;
}

void Home::on_uploadButton_clicked()
{
    QString image_path;
    cout<<"Upload: " << FileNameChosen.toStdString()<<endl;
    if(FileNameChosen.size() > 0){
        ui->imagePath->setText(FileNameChosen);
    }
    // client send a request to upload an image
    image_path = ui->imagePath->text();
    int ImageID = myClient.UIcreateImage(image_path);
    int reply = myClient.UIuploadImage(ImageID);
    if( reply > 0){
        QMessageBox::about(this,"Success", "Image has been uploaded!");
        ui->imagePath->clear();
        FileNameChosen.clear();
    }
    else if (reply == -4)
    {
        QMessageBox::critical(this, "Fail" , "Connection error occured.","OK");
    }
    else {
        QMessageBox::critical(this,"Error", "Image is not uploaded. Try again!", "OK");
    }

}

void Home::on_updateButton_clicked()
{

    int NumberOfViews;
    string newUser = ui->newUser->text().toStdString();
    string ImageID = ui->ImageID->text().toStdString();

    if(ui->ViewsNo->text().toStdString().size() > 0 && newUser.size() > 0 && ImageID.size() > 0 ){
        NumberOfViews = stoi(ui->ViewsNo->text().toStdString());
        newUser = ui->newUser->text().toStdString();
        cout << newUser << " " << ImageID << " "<<NumberOfViews <<endl;
        map <string,int> Access_List;
        if( myClient.UIgetAccessList(stoi(ImageID),Access_List) == 0){
            cout << Access_List.size() << endl;
            Access_List[(newUser)] = NumberOfViews;
            int reply = myClient.UIupdateAccessList(stoi(ImageID),Access_List);
            cout << Access_List.size() << endl;
            if(reply == 0){
                QMessageBox::critical(this, "Success", "Access List updated", "OK");
                ui->newUser->clear();
                ui->ImageID->clear();
                ui->ViewsNo->clear();
            }
            else if(reply==-4)
            {
                QMessageBox::critical(this, "Fail" , "Connection error occured.","OK");
            }
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "one input is empty.", "OK");
    }
}

void Home::on_MyImageList_doubleClicked(const QModelIndex &index)           // set the global index for updating
{

    myListChosenIndex = index.row();
    int ImageID = myImagesList[myListChosenIndex];
    bool uploaded;

    myClient.UIgetOwnedImageDescription(ImageID,uploaded);

    string title = "Image Descrption for Image ID: " + to_string(ImageID);
    string desc = "Upload status: " + uploaded;
    char * _desc = new char[desc.length() + 1];
    std::strcpy(_desc,desc.c_str());
    char * _title = new char[title.length() + 1];
    std::strcpy(_title,title.c_str());
    QMessageBox::about(this, _title , _desc);
}

void Home::on_ServerImageList_doubleClicked(const QModelIndex &index)       // Download the Image
{
    serverListChosenIndex = index.row();

}

void Home::on_UsersList_doubleClicked(const QModelIndex &index)         // set the index to Global index for updating
{

    usersChosenIndex = index.row();
    //cout << usersChosenIndex << endl;

}

void Home::on_MyImageList_clicked(const QModelIndex &index)         // gets the chosen Image information
{
    myListChosenIndex = index.row();
    //cout << myListChosenIndex << endl;
}

void Home::on_ServerImageList_clicked(const QModelIndex &index)             // gets the chosen Image information
{
    serverListChosenIndex = index.row();
    int ImageID = serverImagesList[serverListChosenIndex];
    int currentViews , maxViews ,remainingViews;

    myClient.UIgetImageDescription(ImageID, currentViews, maxViews, remainingViews);

    string desc = "Current Views: " + to_string(currentViews) + ", Max Views is: " + to_string(maxViews) + ", Remaining Views: " + to_string(remainingViews);
    string title = "Image Descrption for Image ID: " + to_string(ImageID);
    char * _desc = new char[desc.length() + 1];
    std::strcpy(_desc,desc.c_str());
    char * _title = new char[title.length() + 1];
    std::strcpy(_title,title.c_str());
    QMessageBox::about(this, _title , _desc);

}

void Home::on_Refresh_clicked()
{
    //    vector<string> users;
    //    myClient.UIgetUsersListOnline(users);
    //    cout<<"list of online users on the system: "<<endl;
    //    for(int i=0;i<users.size();i++) cout<<users[i]<<endl;
    fillLists();
}

void Home::fillLists()
{
    //1. client sends a request to get the list of the images he can access on the server

    QStringListModel *model = new QStringListModel(this);

    serverImagesList.clear();
    int status = myClient.UIgetImagesAccessible(serverImagesList);
    if (status == -4)
    {
        QMessageBox::critical(this, "Fail" , "Connection error occured.","OK");
    }

    QStringList List1,List2, List3;

    for(int i = 0 ; i < serverImagesList.size() ; i++){
        QString temp = QString::fromStdString(to_string(serverImagesList[i]));
        List1 << temp;
    }
    model->setStringList(List1);
    ui->ServerImageList->setModel(model);
    ui->ServerImageList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //2. fetch the user's own images
    myImagesList.clear();
    model = new QStringListModel(this);

    myClient.UIgetOwnedImagesAccessible(myImagesList);


    for(int i = 0 ; i < myImagesList.size() ; i++){
        QString temp = QString::fromStdString(to_string(myImagesList[i]));
        List2 << temp;
    }

    model->setStringList(List2);
    ui->MyImageList->setModel(model);
    ui->MyImageList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 3. Fetch the list of server users
    OnlineUsers.clear();

    serverUsersList.clear();

    status = myClient.UIgetAllUsersList(serverUsersList);
    if (status == -4)
    {
        QMessageBox::critical(this, "Fail" , "Connection error occured.","OK");
    }


    status = myClient.UIgetUsersListOnline(OnlineUsers);
    if (status == -4)
    {
        QMessageBox::critical(this, "Fail" , "Connection error occured.","OK");
    }


    model = new QStringListModel(this);

    ui->UsersList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for(int i = 0 ; i < serverUsersList.size() ; i++){
        QString temp = QString::fromStdString(serverUsersList[i]);
        if ( std::find(OnlineUsers.begin(), OnlineUsers.end(), serverUsersList[i]) != OnlineUsers.end() ){
            temp.append(": Online");
        }
        else{
            temp.append(": Offline");
        }
        List3 << temp;
    }
    model->setStringList(List3);
    ui->UsersList->setModel(model);

}

void Home::on_pushButton_clicked()          // check User button
{

    string User = ui->checkuser->text().toStdString();
    if(User.size() == 0){
        QMessageBox::critical(this, "Error", "box is empty.", "OK");

    }
    else{
        int reply =  myClient.UIcheckUsername(User);
        if(reply == 0){
            QMessageBox::about(this, "Error" , "Username doesn't exist.");

        }else if(reply == 1){
            QMessageBox::about(this, "Success" , "Username exists on the server");

        }else if(reply == -4){
            QMessageBox::about(this, "Fail" , "Connection error occured.");
        }
    }

}

void Home::on_logout_clicked()
{
    int reply = myClient.UILogoutUser();
    if(reply == 0){
        this->hide();
        MainWindows mainWindow;
        mainWindow.setModal(true);
        mainWindow.exec();

    }
    else if (reply == -4)
    {
        QMessageBox::about(this, "Fail" , "Connection error occured.");
    }
    else{
        QMessageBox::about(this, "Fail" , "Operation failed");

    }

}

void Home::on_browseButton_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    //  dialog.setOption(QFileDialog::DontUseNativeDialog,true);
    // dialog.setOption(QFileDialog::DontResolveSymlinks);
    dialog.setNameFilterDetailsVisible(true);
    dialog.setViewMode(QFileDialog::Detail);
    QStringList filters;
    filters <<"Image files (*.png)";
    dialog.setOption(QFileDialog::HideNameFilterDetails,false);
    dialog.setNameFilters(filters);
    int res = dialog.exec();
    QDir directory;
    QString file =  directory.currentPath();
    //    cout << res << endl;
    //    cout << file.toStdString() << endl;

    QStringList filesList;

    if (res)
    {
        directory = dialog.selectedFiles()[0];
        filesList = dialog.selectedFiles();
        QString fileName;
        foreach(fileName, filesList)
        {
            // cout << "FileName: " << fileName.toStdString();
            FileNameChosen = fileName;
            ui->imagePath->setText(FileNameChosen);
            cout << directory.absoluteFilePath(fileName).toStdString();

        }
    }
}

void Home::on_deleteButton_clicked()            // delete an Image
{
    int reply = myClient.UIdeleteOwnedImage(myListChosenIndex+1);
    if(reply == 0){ // reply is returned from the above function not status! status will be returned as -4 if there is a failure in communication.
        //Reply will return -1 if there is an authorization problem
        QMessageBox::about(this, "Success" , "Image deleted!");

    }
    else if (reply == -4)
    {
        QMessageBox::about(this, "Fail" , "Connection error occured.");
    }
    else{
        QMessageBox::about(this, "Fail" , "Operation failed");

    }
}

void Home::on_viewButton_clicked()
{
    // Download the image if user double Clicked on it
    cout << serverListChosenIndex << endl;
    int ImageID = serverImagesList[serverListChosenIndex];
    cout << ImageID << endl;
    QImage ImageToView;
    int reply = myClient.UIviewImage(ImageID, ImageToView);

    if(reply == 0)
    {
        QLabel *myLabel = new QLabel();
        myLabel->setPixmap(QPixmap::fromImage(ImageToView));
        myLabel->show();
    }
    else if (reply == -4)
    {
        QMessageBox::about(this, "Fail" , "Connection error occured.");
    }


}

void Home::on_ViewOwnImage_clicked()
{
    QImage ImageToView;
    myClient.UIviewOwnedImage(myImagesList[myListChosenIndex], ImageToView);

    QLabel *myLabel = new QLabel();
    myLabel->setPixmap(QPixmap::fromImage(ImageToView));
    myLabel->show();
}
