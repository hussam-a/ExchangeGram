#ifndef SERVER_H
#define SERVER_H
#include "includes.h"
#include "message.h"
#include "communication.h"
//#include "image.h"
#include "conversion_functions.h"

class server
{
    public:
        server();
        ~server();
        int startServer(char *address, int port);
		string checkUser(message m);
		string registerUser(message m);
		string updateList(message m);
		string uploadImage(message m);
		string getTimeStamp(message m);
		string deleteImage(message m);
		string downloadImage(message m);
		string getImageList(message m);
        string getUsersList(message m);
        string getOnlineUsersList(message m);


        string loginUser(message m);
        string logoutUser(message m);

    private:
        map <string, string > authMap; // string username, string password
		map <int, string> ownerMap; // int imageid, string username
		map <int, QImage> imageMap; // int imageid, image image
		map<string,vector<tuple<int,int,int>>> accessMap ; // string username, vector <tuple <imageID, views, revision>>> // list of images owner can view, not the ones he owns
		// it gets updated in UploadImageOperation and UpdateListOperation
		map<string, bool > status;  //string username, bool online or offline
		std::mutex mutexaccessMap;
		std::mutex mutexImageMap;
        void displayStatus();
        bool authMsg(string message_content);


};

#endif // SERVER_H
