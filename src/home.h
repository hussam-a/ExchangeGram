#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QFileDialog>
#include "client.h"

namespace Ui {
class Home;
}

extern client myClient;

class Home : public QDialog
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = 0);
    ~Home();

private slots:
    void on_uploadButton_clicked();

    void on_updateButton_clicked();

    void on_MyImageList_doubleClicked(const QModelIndex &index);

    void on_ServerImageList_doubleClicked(const QModelIndex &index);

    void on_UsersList_doubleClicked(const QModelIndex &index);

    void on_MyImageList_clicked(const QModelIndex &index);

    void on_ServerImageList_clicked(const QModelIndex &index);

    void on_Refresh_clicked();

    void on_pushButton_clicked();

    void on_logout_clicked();

    void on_browseButton_clicked();

    void on_deleteButton_clicked();

    void on_viewButton_clicked();

    void on_ViewOwnImage_clicked();

private:
    void fillLists();
    Ui::Home *ui;
    int myListChosenIndex,serverListChosenIndex,usersChosenIndex;

    QString FileNameChosen;
    vector<int> serverImagesList;
    vector<int> myImagesList;
    vector<string> serverUsersList;
	vector<string>OnlineUsers;



};

#endif // HOME_H
