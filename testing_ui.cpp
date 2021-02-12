#include "testing_ui.h"

bool testUIcheckUsername(const string &_username)
{
    if (_username == "hussam")
    {
        true;
    }
    else
    {
        false;
    }
}

bool testUIregisterUsername(const string &_username, const string &_password)
{
    if (_username == "hussam")
    {
        if(_password == "root")
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

bool testUIcreateImage (const QString &_filename)
{
    return true;
}

bool testUIuploadImage(const int &_ownedImageID)
{
    if (_ownedImageID == 1)
        return true;
    else
        return false;
}

bool testUIgetImagesAccessible(vector <int> &_imageList)
{
    _imageList.push_back(1);
    _imageList.push_back(2);
    return true;
}

bool testUIgetImageDescription (const int &_imageID, int& _currentViews, int& _maxViews, int& _remainingViews)
{
    if (_imageID == 1)
    {
        _currentViews = 1;
        _maxViews = 10;
        _remainingViews = 9;
    }
    else
    {
        _currentViews = 1;
        _maxViews = 5;
        _remainingViews = 4;

    }
    return true;
}

bool testUIviewImage(const int &_imageID, QImage& _imageContainer)
{
    if (_imageID==1)
        _imageContainer.load(QString::fromStdString(string("/home/hussam/Desktop/selfie.jpeg")));
    else
        _imageContainer.load(QString::fromStdString(string("/home/hussam/Desktop/06-train-cat-shake-hands.jpg")));
    return true;
}

bool testUIgetOwnedImagesAccessible(vector <int> &_ownedImageList)
{
    _ownedImageList.push_back(1);
    _ownedImageList.push_back(2);
}

bool testUIgetOwnedImageDescription (const int &_ownedImageID, bool &_uploaded)
{
    if (_ownedImageID == 1)
        _uploaded = true;
    else
        _uploaded = false;
    return true;
}

bool testUIgetAccessList(const int &_ownedImageID, map<string,int> &_accessList)
{
    if (_ownedImageID==1)
    {
        _accessList[string("hager")]=4;
        _accessList[string("ola")]=5;
    }
    else
    {
        _accessList[string("superman")]=4;
        _accessList[string("batman")]=5;

    }
}

bool testUIupdateAccessList(const int &_ownedImageID, const map<string,int> &_accessList)
{
    if (_ownedImageID == 1)
        return true;
    else
        return false;
}

bool testUIviewOwnedImage(const int &_ownedImageID, QImage& _imageContainer)
{
    if (_ownedImageID==1)
        _imageContainer.load(QString::fromStdString(string("/home/hussam/Desktop/06-train-cat-shake-hands.jpg")));
    else
        _imageContainer.load(QString::fromStdString(string("/home/hussam/Desktop/selfie.jpeg")));
    return true;
}

bool testUIdeleteOwnedImage(const int &_ownedImageID)
{
    if (_ownedImageID == 1)
        return true;
    else
        return false;
}

void test();

bool startClient(string server, int IP)
{

}
