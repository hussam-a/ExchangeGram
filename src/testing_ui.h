#ifndef TESTING_UI_H
#define TESTING_UI_H
#include "includes.h"
#include "client.h"


    bool startClient(string server, int IP);
    bool testUIcheckUsername(const string &_username);
    bool testUIregisterUsername(const string &_username, const string &_password);
    bool testUIcreateImage (const QString &_filename);
    bool testUIuploadImage(const int &_ownedImageID);
    bool testUIgetImagesAccessible(vector <int> &_imageList);
    bool testUIgetImageDescription (const int &_imageID, int& _currentViews, int& _maxViews, int& _remainingViews);
    bool testUIviewImage(const int &_imageID, QImage& _imageContainer);
    bool testUIgetOwnedImagesAccessible(vector <int> &_ownedImageList);
    bool testUIgetOwnedImageDescription (const int &_ownedImageID, bool &_uploaded);
    bool testUIgetAccessList(const int &_ownedImageID, map<string,int> &_accessList);
    bool testUIupdateAccessList(const int &_ownedImageID, const map<string,int> &_accessList);
    bool testUIviewOwnedImage(const int &_ownedImageID, QImage& _imageContainer);
    bool testUIdeleteOwnedImage(const int &_ownedImageID);
    void test();

#endif // TESTING_UI_H

