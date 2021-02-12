//#ifndef IMAGE_H
//#define IMAGE_H

//#include <QImage>
//#include <string>
//#include <map>
//#include <QFile>
//#include <QDebug>
//#include <QImage>
//#include <QLabel>
//#include <QBuffer>
//#include <iostream>
//#include "includes.h"
//#include "conversion_functions.h"

//class Image
//{
//public:
//    Image(QString _path, string _image_owner);
//    string serializeImage(string _username, string _password);
//    void setViewRights(const map<string, int> &_viewRights);
//    bool updateUserViewRights(string userID, int view_times);
//    ~Image();

//    static int getImageID();
//    QString getImagePath() const;
//    void setImageContent(const QImage &_imageContent);
//    void setImageName(const string &_name);
//    bool fillViewRights();
//    int viewImage(const string &userID, QImage &_img);
//    map<string, int> getViewRights() const;
//  //  Image convertStringtoImage(string _image);
//  // Image(QImage _image_content);
//  // Image(string _image_owner);

//private:
//    int imageID;             // Id of the image
//    map <string, int > viewRights;          // username and number of views
//    string image_owner;                       // username of the image owner
//    QImage imageContent;


//    QString imagePath;
//    bool checkUser(const string &userID);
//    int getUserViewRights(const string &userID);
//    bool addViewRights (const string & userID, const int &view_times );
//    bool removeViewRights (const string &userID );
//    void viewImageForCreditedUsers();
//    QImage getImageContent() const;
//};

//#endif // IMAGE_H

//#ifndef IMAGE_H
//#define IMAGE_H
//#include "includes.h"
//class theactualimage;

//class image
//{
//public:
//    image();
//    ~image();
//    bool view (theactualimage& container);
//    bool setDescription(string owner, string uploadDate,string viewsLeft);
//    bool getDescription(string &owner, string &uploadDate,string &viewsLeft);

//private:
//    string owner;
//    theactualimage data;
//};

//#endif // IMAGE_H
