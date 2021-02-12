//#include "image.h"

//int Image::imageID = 0;

//Image::Image(QString _path, string _owner_ID){
//    //imageID++;
//    if(_path != ""){
//        imagePath = _path;
//        imageContent.load(_path);
//        image_owner = _owner_ID;
//        //imageContent.show();
//    }
//}
//Client sends: username (string) and password (string), plain image (idk whatever that is), access list (vector of pair of string,int) (or access list inside the image class we will see) ,

//string Image::serializeImage(string _username, string _password)
//{
//    string newImage = intToString(_username.size()) + " " +_username + " " + intToString(_password.size()) + " " + _password + " ";  //_usernamesize+username+passwordsize+password

//    newImage += intToString(viewRights.size());
//    newImage += " ";

//    for(map<string,int>::iterator it = viewRights.begin(); it != viewRights.end(); ++it) {
//        string key = it->first;
//        int value = it->second;
//        newImage += " ";
//        newImage += key;
//        newImage += " ";
//        newImage += to_string(value);
//        newImage += " ";
//    }

//    QByteArray ba;
//    QBuffer buffer(&ba);
//    buffer.open(QIODevice::WriteOnly);
//    imageContent.save(&buffer, "BMP");
//    QByteArray arr = qCompress(buffer.buffer(),5);
//    QString strImage = QString(arr);

//    newImage += intToString(strImage.toStdString().size());
//    newImage += " ";
//    newImage += strImage.toStdString();

//    return newImage;
//}
//void Image::setViewRights(const map<string, int> &_viewRights)
//{
//    viewRights = _viewRights;
//}
//bool Image::addViewRights (const string &userID, const int &view_times ){
//     //viewRights.insert_or_assign( make_pair(userID,view_times ));
//    if(this->checkUser(userID))  return false;
//    viewRights [userID] = view_times;
//    return true;
//}

//bool Image::checkUser(const string &userID){
//    return (viewRights.find(userID) != viewRights.end() );

//}
//Image::~Image(){

//}
//Image::Image(string _image_owner)
//{
//    imagePath = "";
//    imageID++;
//    image_owner = _image_owner;


//}

//int Image::getImageID()
//{
//    return imageID;
//}

//QImage Image::getImageContent() const
//{
//    return imageContent;
//}

//string Image::convertImageToString()
//{
//    /* all seperated by " ": imageID size + imageID + image_owner string size + image owner + imagecontent string size + image conent + accesslist size + key + value
//    static int imageID;             // Id of the image
//    string image_owner;                       // username of the image owner

//    map <string, int > viewRights;          // username and number of views
//    QImage imageContent;
//    */

//    string newImage ="";
//    newImage += image_owner.size();
//    newImage += " ";
//    newImage += image_owner ;
//    newImage += " ";
//    newImage += to_string(imageID);
//    newImage += " ";
//    newImage += (to_string(imageID)).size();
//    newImage += " ";

//    QByteArray ba;
//    QBuffer buffer(&ba);
//    buffer.open(QIODevice::WriteOnly);
//    imageContent.save(&buffer, "BMP");
//    QByteArray arr = qCompress(buffer.buffer(),5);
//    QString str = QString(arr);

//    newImage += (str.toStdString()).size();
//    newImage += " ";
//    newImage += str.toStdString();
//    newImage += " ";

//    newImage += to_string(viewRights.size());
//    newImage += " ";

//    for(map<string,int>::iterator it = viewRights.begin(); it != viewRights.end(); ++it) {
//        string key = it->first;
//        int value = it->second;
//        newImage += " ";
//        newImage += key;
//        newImage += " ";
//        newImage += to_string(value);
//        newImage += " ";
//    }

//    return newImage;
//}

//QString Image::getImagePath() const
//{
//    return imagePath;
//}

//void Image::setImageContent(const QImage &_imageContent)
//{
//    imageContent = imageContent;
//}

//map<string, int> Image::getViewRights() const
//{
//    return viewRights;
//}

//bool Image::checkUser(const string &userID){
//    return (viewRights.find(userID) != viewRights.end() );

//}
//int Image::getUserViewRights(const string &userID){
//    if(this->checkUser(userID)) return viewRights[userID];
//    else return -1;
//}

//bool Image::updateUserViewRights(string userID, int view_times){
//    if (!(this->checkUser(userID))) return false;
//    viewRights [userID] = view_times;
//    return true;
//}


//bool Image::addViewRights (const string &userID, const int &view_times ){
//     //viewRights.insert_or_assign( make_pair(userID,view_times ));
//    if(this->checkUser(userID))  return false;
//    viewRights [userID] = view_times;
//    return true;
//}
//bool Image::removeViewRights (const string &userID ){
//    if (!(this->checkUser(userID))) return false;
//    viewRights.erase(userID);
//    return true;
//}

//void Image::viewImageForCreditedUsers(){

//}
//int Image::viewImage(const string &userID, QImage & _img){

//    if (this->checkUser(userID)){
//        int allowedViewTimes = this->getUserViewRights(userID);
//        cout<<allowedViewTimes<<endl;
//    if( allowedViewTimes <= 0 )
//        return -1; //user already consumed allowed view times
//    this->updateUserViewRights(userID, allowedViewTimes-1);
//     _img = this->imageContent;
//     return 1;
//    }

//    return -2; //user is not listed amongest those who have the right to view the image
//}
//bool Image::fillViewRights(){

//}

