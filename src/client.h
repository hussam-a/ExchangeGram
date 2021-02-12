#ifndef CLIENT_H
#define CLIENT_H
#include "communication.h"
//#include "image.h"
#include "conversion_functions.h"

class client
{

public:
    client();
    ~client();

    int UIstartClient(string _address,int _port);
	int UIstopClient();
	int UIsetDefaultImage(const QString& _filepath);
    int UIcheckUsername(const string &_username);
    int UIregisterUsername(const string &_username, const string &_password);
    int UIcreateImage (const QString &_filename);
    int UIuploadImage(const int &_ownedImageID);
    int UIgetImagesAccessible(vector <int> &_imageList);
    int UIgetImageDescription (const int &_imageID, int& _currentViews, int& _maxViews, int& _remainingViews);
    int UIviewImage(const int &_imageID, QImage& _imageContainer);
    int UIgetOwnedImagesAccessible(vector <int> &_ownedImageList);
    int UIgetOwnedImageDescription (const int &_ownedImageID, bool &_uploaded);
    int UIgetAccessList(const int &_ownedImageID, map<string,int> &_accessList);
    int UIupdateAccessList(const int &_ownedImageID, const map<string,int> &_accessList);
    int UIviewOwnedImage(const int &_ownedImageID, QImage& _imageContainer);
    int UIdeleteOwnedImage(const int &_ownedImageID);

    int UILoginUser(const string &_username, const string & _password);
    int UILogoutUser();
    int UIgetAllUsersList(vector<string> & _usersList);
    int UIgetUsersListOnline(vector<string> & _usersList);

    string UIgetUserName();

	int startClient(char *address, int port);
	int stopClient();

    int checkUsername(const string &_username, int& _reply);
    int registerUsername(const string &_username, const string &_password, int& _reply);
    int uploadImage(const string &_username, const string &_password, const map<string,int> &_accessList ,
                     const QImage &_image, int& _reply);
    int downloadImage(const string& _username,
                       const string& _password, const int &_imageID, QImage &_imageReply);
    int getRevisionCount(const string &_username, const string &_password,
                          const int& _imageID, int& _revisionCountReply);
    int updateAccessList(const string& _username, const string& _password,
                          const int &_imageID, map<string, int> _accessList, int& _reply);
    int deleteImage(const string& _username, const string& _password, const int &_imageID, int& _reply);
    int updateImagesAccessible(const string& _username, const string& _password,
                                map <int,pair< int,int>> &_imageList);

    int loginUser(const string &_username, const string & _password, int& _reply);
    int logoutUser(const string &_username, const string & _password, int& _reply);
    int getAllUsersList(vector<string> & _usersList);
    int getUsersListOnline(vector<string> & _usersList);

private:
    QImage defaultImage;
    int server_IP, server_Port;
    string username;
    string password;
    communication comm;
	map <int, tuple<QImage, map<string,int>, bool> > imagesOwned; // <int local_image_ID, tuple<QImage image, map<string username, int maxviews>, bool uploaded or not>
	map <int,pair<QImage, int>> imagesCached; // <remote ID, pair<QImage Image, int currentviews>>
	map <int,pair< int,int>> imagesAccessible; // <int remoteID, pair< maxviews, revisioncount>>
    int ownedImageCounter;
	map<int,int> ownedServerImageID;

    vector<string> allUsersList;
    vector<string> allOnlineUsersList;
};

#endif // CLIENT_H
