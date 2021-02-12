#include "starthere.h"
#include <QApplication>
#include "image.h"
#include "client.h"
#include <iostream>
#include "home.h"

using namespace std;

client myClient;


int main(int argc, char *argv[])
{
    system("mkdir tmp");
    // thread t(serverApplication, (char*)("127.0.0.1"),4466);

    QApplication a(argc, argv);
    startHere w;
    w.show();

    return a.exec();
}
//    Image *img = new Image("/home/ola/Desktop/image/selfie.jpeg");
//    map <int, int > m { make_pair(1,3) , make_pair(2,7) };
//    img->setViewRights(m);
//    QImage _img;
//   cout<< img->viewImage(3)<<endl;
//     cout<< img->viewImage(1, _img)<<endl;
//   cout<< img->viewImage(1, _img)<<endl;
//   cout<< img->viewImage(1, _img)<<endl;

//    QLabel imgLabel;
//    imgLabel.setPixmap(QPixmap::fromImage(_img));
//    imgLabel.show();

//#include "mainwindows.h"
//#include <QApplication>
//#include <string.h>
//#include <QImage>
//#include <QFile>
//#include <QDebug>
//#include <QImage>
//#include <QLabel>
//#include <QBuffer>
//#include <string>
//#include <iostream>
//#include <QDataStream>

//using namespace std;


//client myClient;
//string ImageToString(const QImage &_i)
//{
//    QByteArray byteArray;
//    QBuffer buffer(&byteArray);
//    _i.save(&buffer, "png");        // writes the image in jpg format inside the buffer

//    std::string strImage(byteArray.constData(), byteArray.length());
//    string s = to_string(strImage.size());
//    s += " ";
//    s += strImage;
//    return s;
//}

//QImage stringToImage(const string &_s)
//{
//    size_t found = _s.find(" ");
//    string ImageReceived = _s.substr(found+1,_s.size());

//    QImage image;
//    QByteArray baData(ImageReceived.c_str(),ImageReceived.length());
//    image.loadFromData(baData, "png");

//    return image;
//}

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

//    //w.show();


//    QImage myImage;
//    myImage.load("/home/hager/Downloads/q.jpg");
//    QLabel *myLabel = new QLabel(); // sets parent of label to main window

//    string s = ImageToString(myImage);
//    cout<<s.size() << endl;

//    QImage img  = stringToImage(s);
//    myLabel->setPixmap(QPixmap::fromImage(img));
//    myLabel->show();

//    return a.exec();
//}
