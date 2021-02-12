#include "client.h"

client::client() {}
client::~client() {}

int client::startClient(char *address, int port)
{
    bool status = comm.startCommunication(true,address, port);
    if (status == false)
        return 1;
    else
    {
        comm.getPeer(server_IP, server_Port);
        ownedImageCounter = 0;
        return 0;
    }
}

int client::stopClient()
{
    bool status = comm.stopCommunication();
    if (status == false)
        return 1;
    else
        return 0;
}

int client::UIstartClient(string _address,int _port)
{
    char* address = new char[_address.size()];
    strcpy(address, _address.c_str());
    return startClient(address,_port);
}

int client::UIstopClient()
{
    return stopClient();
}

int client::UIsetDefaultImage(const QString & _filepath)
{
    defaultImage.load(_filepath);
    return 0;
}

int client::UIcheckUsername(const string &_username)
{
    int reply;
    int status = checkUsername(_username, reply);
    if (status == -3)
    {
        return reply;
    }
    else
    {
        return -4;
    }
}

int client::UIregisterUsername(const string &_username, const string &_password)
{
    int reply;
    int status = registerUsername(_username,_password,reply);
    if (status == -3)
    {
        if (reply == 0)
        {
            username= _username;
            password = _password;
        }
        return reply;
    }
    else
    {
        return -4;
    }
}

//map <int, tuple<QImage, map<string,int>, bool> > imagesOwned;
int client::UIcreateImage (const QString &_filename)
{
    tuple<QImage, map<string,int>,bool> t;
    ownedImageCounter++;
    int ownedImageID = ownedImageCounter;
    QImage i;
    i.load(_filename);
    get<0>(t)=i;
    get<2>(t)=false;
    imagesOwned[ownedImageID] = t;
    return ownedImageID;
}

int client::UIuploadImage(const int &_ownedImageID)
{
    int reply;
    int status = uploadImage(username,password, get<1>(imagesOwned[_ownedImageID]),
                             get<0>(imagesOwned[_ownedImageID]),reply);
    if (status==-3)
    {
        if (reply > 0 )
        {
            get<2>(imagesOwned[_ownedImageID]) = true;
            ownedServerImageID[_ownedImageID]= reply;
            return reply;
        }
        else
        {
            return reply;
        }
    }
    else
    {
        return -4;
    }
}

//map <int,pair< int,int>> imagesAccessible;
int client::UIgetImagesAccessible(vector <int> &_imageList)
{
    map <int,pair< int,int>> m;
    int status = updateImagesAccessible(username,password,m);
    if(status == -3)
    {
        imagesAccessible.clear();
        for( map <int,pair< int,int>>::const_iterator it = m.begin();
             it != m.end(); ++it)
        {
            imagesAccessible[it->first]=it->second;
            _imageList.push_back(it->first);
        }
    }
    else
    {
        for( map <int,pair< int,int>>::const_iterator it = imagesAccessible.begin();
             it != imagesAccessible.end(); ++it)
        {
            _imageList.push_back(it->first);
        }
    }
    return status;
}

int client::UIgetImageDescription (const int &_imageID, int& _currentViews, int& _maxViews, int& _remainingViews)
{
	if (imagesCached.find(_imageID)!= imagesCached.end())
	{
		_currentViews = imagesCached[_imageID].second;
	}
	else
	{
		_currentViews = 0;
	}
    _maxViews = imagesAccessible[_imageID].first;
    if (_currentViews<_maxViews)
    {
        _remainingViews = _maxViews-_currentViews;
    }
    else
    {
        _remainingViews = 0;
    }
    return 0;
}

//map <int,pair<QImage, int>> imagesCached;
int client::UIviewImage(const int &_imageID, QImage& _imageContainer)
{
    int reply;
	QImage tempImage;
    map<string,pair<int,int>> tempList;
    vector<int> tempV;
//    map <int,pair<QImage, int>>::iterator it = imagesCached.find(_imageID);
	if (imagesCached.find(_imageID)== imagesCached.end() )
    {
        int status = downloadImage(username, password,_imageID,tempImage);
        if (status == -3)
        {
            imagesCached[_imageID]=make_pair(tempImage,0);
            bool pass = deSteganography(_imageID,imagesCached[_imageID].first,tempList,tempImage);
            if (pass)
            {
                int _currentViews = imagesCached[_imageID].second;
                int _maxViews = imagesAccessible[_imageID].first;
                if (_currentViews<_maxViews)
                {
                    _imageContainer = tempImage;
					if (_currentViews == 0)
					imagesCached[_imageID].second = 1;
					else imagesCached[_imageID].second++;
                }
                else
                {
                    _imageContainer = defaultImage;
                }
            }
            else
            {
                _imageContainer = defaultImage;
            }
        }
        else
        {
            _imageContainer = defaultImage;
        }
    }
    else
    {
        int status = getRevisionCount(username,password,_imageID,reply);
        if (status == -3)
        {
            if (reply == imagesAccessible[_imageID].second)
            {
                bool pass = deSteganography(_imageID,imagesCached[_imageID].first,tempList,tempImage);
                if (pass)
                {
                    int   _currentViews = imagesCached[_imageID].second;
                    int   _maxViews = imagesAccessible[_imageID].first;
                    if (_currentViews<_maxViews)
                    {
                        _imageContainer = tempImage;
						if (_currentViews == 0)
						imagesCached[_imageID].second = 1;
						else imagesCached[_imageID].second++;
                    }
                    else
                    {
                        _imageContainer = defaultImage;
                    }
                }
            }
            else
            {
                status = UIgetImagesAccessible(tempV);
                bool pass = deSteganography(_imageID,imagesCached[_imageID].first,tempList,tempImage);
                if (pass)
                {
                    int       _currentViews = imagesCached[_imageID].second;
                    int     _maxViews = imagesAccessible[_imageID].first;
                    if (_currentViews<_maxViews)
                    {
                        _imageContainer = tempImage;
						if (_currentViews == 0)
						imagesCached[_imageID].second = 1;
						else imagesCached[_imageID].second++;
                    }
                    else
                    {
                        _imageContainer = defaultImage;
                    }
                }

            }
        }
        else
        {
            bool pass = deSteganography(_imageID,imagesCached[_imageID].first,tempList,tempImage);
            if (pass)
            {
                int      _currentViews = imagesCached[_imageID].second;
                int     _maxViews = imagesAccessible[_imageID].first;
                if (_currentViews<_maxViews)
                {
                    _imageContainer = tempImage;
					if (_currentViews == 0)
					imagesCached[_imageID].second = 1;
					else imagesCached[_imageID].second++;
                }
                else
                {
                    _imageContainer = defaultImage;
                }
            }
            else
            {
                _imageContainer = defaultImage;
            }
        }
    }
    return 0;
}

int client::UIgetOwnedImagesAccessible(vector <int> &_ownedImageList)
{
    for( map <int, tuple<QImage, map<string,int>, bool> >::const_iterator it = imagesOwned.begin();
         it != imagesOwned.end(); ++it)
    {
        _ownedImageList.push_back(it->first);
    }
    return 0;
}

int client::UIgetOwnedImageDescription (const int &_ownedImageID, bool &_uploaded)
{
    _uploaded=get<2>(imagesOwned[_ownedImageID]);
    return 0;
}

int client::UIgetAccessList(const int &_ownedImageID, map<string,int> &_accessList)
{
    _accessList=get<1>(imagesOwned[_ownedImageID]);
    return 0;
}

int client::UIupdateAccessList(const int &_ownedImageID, const map<string,int> &_accessList)
{
    int reply;
    get<1>(imagesOwned[_ownedImageID])=_accessList;
    if ((get<2>(imagesOwned[_ownedImageID])))
    {
        int serverID = ownedServerImageID[_ownedImageID];
        int status = updateAccessList(username,password,serverID,_accessList,reply);
        if (status == -3)
        {
            return reply;
        }
        else
        {
            return -4;
        }
    }
    else
    {
        return 0;
    }
}

int client::UIviewOwnedImage(const int &_ownedImageID, QImage& _imageContainer)
{
    _imageContainer= get<0>(imagesOwned[_ownedImageID]);
    return 0;
}

//map <int, tuple<QImage, map<string,int>, bool> > imagesOwned;
//map<int,int> ownedServerImageID;
int client::UIdeleteOwnedImage(const int &_ownedImageID)
{
    int reply;
    int serverID = ownedServerImageID[_ownedImageID];
    int status = deleteImage(username,password,serverID,reply);
    if (status == -3)
    {
        if (reply == 0)
        {
            map <int, tuple<QImage, map<string,int>, bool> >::iterator it1 = imagesOwned.find(_ownedImageID);
            imagesOwned.erase(it1);
            map<int,int>::iterator it2 = ownedServerImageID.find(_ownedImageID);
            ownedServerImageID.erase(it2);
            return reply;
        }
        else
        {
            return reply;
        }
    }
    else
    {
        return -4;
    }
}

int client::UILoginUser(const string &_username, const string &_password)
{
    int reply;
    int status = loginUser(_username,_password,reply);
    if (status == -3)
    {
        if (reply == 0)
        {
            username = _username;
            password = _password;
        }
        return reply;
    }
    else
    {
        return -4;
    }
}

int client::UILogoutUser()
{

    int reply;
    int status = logoutUser(username,password,reply);
    if (status == -3)
    {
        return reply;
    }
    else
    {
        return -4;
    }
}

int client::UIgetAllUsersList(vector<string> &_usersList)
{
    vector<string> m;
    int status = getAllUsersList(m);
    if(status == -3)
    {
        allUsersList.clear();
        for(int i =0; i<m.size() ; i++)
        {
            allUsersList.push_back(m[i]);
            _usersList.push_back(m[i]);
        }
    }
    else
    {
        for(int i =0; i< allUsersList.size() ; i++)
        {
            _usersList.push_back(allUsersList[i]);
        }
    }
    return status;
}
int client::UIgetUsersListOnline(vector<string> &_usersList)
{
    vector<string> m;
    int status = getUsersListOnline(m);
    if(status == -3)
    {
        allOnlineUsersList.clear();
        for(int i =0; i<m.size() ; i++)
        {
            allOnlineUsersList.push_back(m[i]);
            _usersList.push_back(m[i]);
        }
    }
    else
    {
        for(int i =0; i< allOnlineUsersList.size() ; i++)
        {
            _usersList.push_back(allOnlineUsersList[i]);
        }
    }
    return status;
}

string client::UIgetUserName()
{
    return username;
}
// Operation 0
int client::checkUsername(const string &_username, int& _reply)
{
    // username
    bool sendStatus = comm.sendMessage(message(Request,Idempotent,CheckUserOperation,
                                               _username.size(),_username),server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _reply = stringToIntNetwork(m.getMessage_content());
        return -3;
    }
    else{
        return -4;
    }
}

// Operation 1
int client::registerUsername(const string &_username, const string &_password, int& _reply)
{
    // username, password
    string messageContent = intToStringNetwork(_username.size()) + " " +_username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    bool sendStatus = comm.sendMessage(message(Request,Idempotent,RegisterOperation,
                                               messageContent.size(), messageContent),server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _reply = stringToIntNetwork(m.getMessage_content());
        return -3;
    }
    else{
        return -4;
    }
}

// Operation 2
int client::uploadImage(const string &_username, const string &_password,const map<string,int> &_accessList
                        , const QImage &_image , int& _reply)
{
    // convert image to string
    string messageContent = intToStringNetwork(_username.size()) + " " + _username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    messageContent += " ";
    messageContent += mapStringIntToString(_accessList);
    messageContent += QImageToString(_image);
    bool sendStatus = comm.sendMessage(message(Request,Idempotent,UploadOperation,
                                               messageContent.size(), messageContent),server_IP,server_Port);
    //	cout << sendStatus << endl;
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _reply = stringToIntNetwork(m.getMessage_content());
        return -3;
    }
    else{
        return -4;
    }

}

// Operation 3
int client::downloadImage(const string& _username, const string& _password, const int &_imageID, QImage &_imageReply)
{
    // username, password, imageID
    string messageContent = intToStringNetwork(_username.size()) + " " + _username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    messageContent += " ";
    messageContent += intToStringNetwork(_imageID);

    bool sendStatus = comm.sendMessage(message(Request,Idempotent,DownloadOperation, messageContent.size(), messageContent),
                                       server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _imageReply = stringToQImage(m.getMessage_content());
        return -3;
    }
    else
    {
        return -4;
    }

}

// Operation 4
int client::getRevisionCount(const string &_username, const string &_password,
                             const int& _imageID, int& _revisionCountReply)
{
    // username, password, imageID
    string messageContent = intToStringNetwork(_username.size()) + " " + _username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    messageContent += " ";
    messageContent += intToStringNetwork(_imageID);

    bool sendStatus = comm.sendMessage(message(Request,Idempotent,GetRevisionCountOperation, messageContent.size(), messageContent),
                                       server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _revisionCountReply = stringToIntNetwork(m.getMessage_content());
        return -3;
    }
    else
    {
        return -4;
    }

}

// operation 5
int client::updateAccessList(const string& _username, const string& _password,
                             const int &_imageID, map<string, int> _accessList, int& _reply)
{
    string messageContent = intToStringNetwork(_username.size()) + " " + _username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    messageContent += " ";
    messageContent += intToStringNetwork(_imageID);
    messageContent += " ";
    messageContent += mapStringIntToString(_accessList);

    bool sendStatus = comm.sendMessage(message(Request,Idempotent,UpdateListOperation
                                               , messageContent.size(), messageContent),
                                       server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _reply = stringToIntNetwork(m.getMessage_content());
        return -3;
    }
    else
    {
        return -4;
    }
}

// operation 6
int client::deleteImage(const string& _username, const string& _password, const int &_imageID, int& _reply)
{

    string messageContent = intToStringNetwork(_username.size()) + " " + _username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    messageContent += " ";
    messageContent += intToStringNetwork(_imageID);

    bool sendStatus = comm.sendMessage(message(Request,Idempotent,DeleteOperation
                                               , messageContent.size(), messageContent),
                                       server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _reply = stringToIntNetwork(m.getMessage_content());
        return -3;
    }
    else
    {
        return -4;
    }

}

// operation 7
int client::updateImagesAccessible(const string& _username, const string& _password,
                                   map <int,pair< int,int>> &_imageList)
{
    // username, password

    string messageContent = intToStringNetwork(_username.size()) + " " + _username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    bool sendStatus = comm.sendMessage(message(Request,Idempotent,GetImageList
                                               , messageContent.size(), messageContent),
                                       server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2, temp3;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _imageList = stringToMapIntPairIntInt(m.getMessage_content(), temp3);
        return -3;
    }
    else
    {
        return -4;
    }

}

int client::loginUser(const string &_username, const string &_password, int &_reply)
{
    // username, password
    string messageContent = intToStringNetwork(_username.size()) + " " +_username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    bool sendStatus = comm.sendMessage(message(Request,Idempotent,LoginOperation,
                                               messageContent.size(), messageContent),server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _reply = stringToIntNetwork(m.getMessage_content());
        return -3;
    }
    else{
        return -4;
    }

}

int client::logoutUser(const string &_username, const string &_password, int &_reply)
{

    // username, password
    string messageContent = intToStringNetwork(_username.size()) + " " +_username + " " +
            intToStringNetwork(_password.size()) + " " + _password;  //_usernamesize+username+passwordsize+password
    bool sendStatus = comm.sendMessage(message(Request,Idempotent,LogoutOperation,
                                               messageContent.size(), messageContent),server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        _reply = stringToIntNetwork(m.getMessage_content());
        return -3;
    }
    else{
        return -4;
    }
}

int client::getAllUsersList(vector<string> & _usersList)
{
    // username, password
    string messageContent = intToStringNetwork(username.size()) + " " + username + " " + intToStringNetwork(password.size()) + " " + password;  //_usernamesize+username+passwordsize+password
    bool sendStatus = comm.sendMessage(message(Request,Idempotent, GetUsersList, messageContent.size(), messageContent),server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2, temp3;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        cout<<"inside get all users"<<endl;
        cout<<m.getMessage_content()<<endl;
        _usersList = stringToVectorStringNetwork(m.getMessage_content(), temp3);
        return -3;
    }
    else
    {
        return -4;
    }
}

int client::getUsersListOnline(vector<string> &_usersList)
{
    // username, password
    string messageContent = intToStringNetwork(username.size()) + " " + username + " " + intToStringNetwork(password.size()) + " " + password;  //_usernamesize+username+passwordsize+password
    bool sendStatus = comm.sendMessage(message(Request,Idempotent, GetUserListOnline, messageContent.size(), messageContent),server_IP,server_Port);
    if(sendStatus){
        message m;
        int temp1,temp2, temp3;
        if(!comm.receiveMessage(m,temp1, temp2))
            return -4;
        cout<<"inside get all users"<<endl;
        cout<<m.getMessage_content()<<endl;
        _usersList = stringToVectorStringNetwork(m.getMessage_content(), temp3);
        return -3;
    }
    else
    {
        return -4;
    }
}

