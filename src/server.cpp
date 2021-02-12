#include "server.h"


//int server::startServer(char *address, int port)
//{
//	comm.startCommunication(true,address, port);
//	return 0;
//}


void loadUsersDB(map<string, string>& users){
  string db_file = "users.txt";
  ifstream db (db_file);

  if(db){
    string username, passwd;
    while(db >> username >> passwd){

      users[username] = passwd;
    }
    // std::copy(istream_iterator<pair<string,string> >(db),
    //           istream_iterator<pair<string,string>>(),
    //             inserter(users));
  }

  cout<<"loading users database"<<endl;


}

void saveUsersDB (const map<string,string>& users){
  string db_file = "users.txt";
  ofstream db (db_file);
  //ostream_iterator<string> db_stream(db, "\n");
  if(db){
    for(auto user: users){
      db << user.first << " "<< user.second<<endl;
    }

  //   std::copy(users.begin(),
  //             users.end(),
  //             db_stream);
  // }
    }
  cout<<"saving users database"<<endl;
}

void server::displayStatus(){
    cout<<"users status:\n";
    for(auto user: status){

          cout<<user.first;
          cout<<((user.second)?": online":": offline");
          cout<<endl;
}
}
server::server() {

    loadUsersDB(authMap);

//    for_each(authMap.begin(),authMap.end(),
//                     [&](const map<string,string>::value_type& p)
//                     { status[p.first] = false; });

    for(auto user: authMap) status[user.first] = false;
     displayStatus();

}
server::~server(){
    saveUsersDB(authMap);
}

string server::checkUser(message m)
{
	string username = m.getMessage_content();
	int reply = 0;

   // iterator i = authMap.find(username);
    if (authMap.find(username) != authMap.end())
	{
		reply = 1 ; // username exists
	}

    string replyS = intToStringNetwork(reply);

	return replyS;
}

string server::registerUser(message m)
{
	string message_content = m.getMessage_content();

	size_t found = message_content.find(" ");
	size_t found1 = message_content.find(" ",found+1);
	string username_received = message_content.substr(found+1,found1-found-1);


	size_t found2 = message_content.find(" ",found1+1);
	string password_receieved = message_content.substr(found2+1);

	int reply ;
    //iterator i = authMap.find(username);
	if (authMap.find(username_received) != authMap.end())
	{
		reply = 1 ; // username exists
	}
	else
	{
		reply = 0 ;
		authMap[username_received]=password_receieved;
        status[username_received] = true;
        saveUsersDB(authMap);
        cout<<"user: "<<username_received<<" has gone online"<<endl;
	}
    string replyS = intToStringNetwork(reply);

	return replyS;
}

string server::loginUser(message m)
{
    string message_content = m.getMessage_content();

    size_t found = message_content.find(" ");
    size_t found1 = message_content.find(" ",found+1);
    string username_received = message_content.substr(found+1,found1-found-1);


    size_t found2 = message_content.find(" ",found1+1);
    string password_receieved = message_content.substr(found2+1);

    int reply ;
    //iterator i = authMap.find(username);
    if (authMap.find(username_received) != authMap.end())
    {
        if ( authMap[username_received]== password_receieved){
            status[username_received] = true;
            reply = 0 ;
            cout<<"user: "<<username_received<<" has gone online"<<endl;

        }
        else reply =-1;
    }
    else
    {
        reply = -2 ;

    }
    string replyS = intToStringNetwork(reply);

    return replyS;
}

string server::logoutUser(message m)
{
    string message_content = m.getMessage_content();

    size_t found = message_content.find(" ");
    size_t found1 = message_content.find(" ",found+1);
    string username_received = message_content.substr(found+1,found1-found-1);


    size_t found2 = message_content.find(" ",found1+1);
    string password_receieved = message_content.substr(found2+1);

    int reply ;
    //iterator i = authMap.find(username);
    if (authMap.find(username_received) != authMap.end())
    {
        if ( authMap[username_received] == password_receieved){
            status[username_received] = false;
            cout<<"user "<<username_received<<" has gone offline"<<endl;
            reply = 0 ;
        }
        else reply =-1;
    }
    else
    {
        reply = -2 ;
    }
    string replyS = intToStringNetwork(reply);

    return replyS;
}
string server:: updateList(message m)        //DONE!
// what is supposed to happen here is that given username, password, image ID, and the list
// Server finds the corresponding image, destegnographies it, updates the list and then applies stegnography and save it
{
	string message_content = m.getMessage_content();

	size_t found = message_content.find(" ");
	size_t found1 = message_content.find(" ",found+1);
	string username_received = message_content.substr(found+1,found1-found-1);

	size_t found2 = message_content.find(" ",found1+1);
	size_t found3 = message_content.find(" ",found2+1);
	string password_receieved = message_content.substr(found2+1,found3-found2-1);

	size_t found4 = message_content.find(" ", found3+1);
	int imageID = stringToIntNetwork(message_content.substr(found3+1,found4-found3-1));

	size_t found5 = message_content.find(" ", found4+1);
	int viewRightSize = stringToIntNetwork(message_content.substr(found4+1,found5-found4-1));


	int reply;
	if (authMap[username_received] == password_receieved)
	{
		reply = 0;

	}
	else
	{
		reply = -1;
		string replyS = intToStringNetwork(reply);
		return replyS;
	}

	//check the owner of the image
	if(ownerMap[imageID]!=username_received)
	{
		reply = -1;
		string replyS = intToStringNetwork(reply);
		return replyS;
	}

	mutexImageMap.lock();
	QImage image = imageMap[imageID];

	QImage desimage ;

	 map<string,pair<int,int>> accessMap2;

	// destegnography

    bool flag = deSteganography(imageID, image, accessMap2, desimage);



	// obtain the list and save it in map <string,pair<int,int>>>


	// compare the available list and update accordingly

	size_t found6 = found5;
	size_t found7;
	mutexaccessMap.lock();
	for(int i = 0 ; i<viewRightSize; i++) // in a list, if the user has zero number of views,
		//we do not erase it from the list, simply that user does not have access rights to the particular image
	{
		string user1;
		if (i==0) {
			found6 = message_content.find(" ", found5+1);
			 user1 = message_content.substr(found5+1,found6-found5-1);
		}
			else {
			found6 = message_content.find(" ", found7+1);

		 user1 = message_content.substr(found7+1,found6-found7-1);
		}
		found7 = message_content.find(" ", found6+1);
		int noOfViews = stringToIntNetwork(message_content.substr(found6+1,found7-found6-1));

		if (accessMap2.find(user1) == accessMap2.end())//check whether a new user is being added to the list
		{
			accessMap2.insert(make_pair(user1,make_pair(noOfViews,1)));
			tuple<int,int,int> t(imageID,noOfViews,1);
			accessMap[user1].push_back(t);  // list of all images accessbile by a client
		}
		else if (accessMap2[user1].first!= noOfViews)
		{
			accessMap2[user1].first = noOfViews;
			accessMap2[user1].second++;
			vector <tuple<int,int,int>> v = accessMap[user1] ; //modifying inside the accessMap for an existing user in the list

			for(tuple<int,int,int> &tup: v)
			{
				if(get<0>(tup)== imageID)
				{
					get<1>(tup) = noOfViews;
					get<2>(tup) = accessMap2[user1].second;
				}
			}
		}

	}
	mutexaccessMap.unlock();

	// after updating the list, stegnography
	QImage stegImage;

    bool flag2 = steganography(imageID, desimage,accessMap2, stegImage);

	imageMap[imageID]= stegImage;

	mutexImageMap.unlock();




    string replyS = intToStringNetwork(reply);

	return replyS;
}

bool server::authMsg(string message_content){

    size_t found = message_content.find(" ");
    size_t found1 = message_content.find(" ",found+1);
    string username_received = message_content.substr(found+1,found1-found-1);

    size_t found2 = message_content.find(" ",found1+1);
    size_t found3 = message_content.find(" ",found2+1);
    string password_receieved = message_content.substr(found2+1,found3-found2-1);



    if (authMap[username_received] != password_receieved) return false;// autho problem
    return true;
    // autho passed
}

string server::getUsersList(message m){
    if ( authMsg(m.getMessage_content()) )
        return mapStringBoolToString(status);
    else
        return intToStringNetwork(-1);

}

string server::getOnlineUsersList(message m){

    if ( authMsg(m.getMessage_content()) )
        return mapStringBoolToStringWithCheck(status);
    else
        return intToStringNetwork(-1);

}

string server:: uploadImage(message m )   //done!
// the server applies stegnography on the (access rights + image) and saves the new image
{
	string message_content = m.getMessage_content();

	size_t found = message_content.find(" ");
	size_t found1 = message_content.find(" ",found+1);
	string username_received = message_content.substr(found+1,found1-found-1);

	size_t found2 = message_content.find(" ",found1+1);
	size_t found3 = message_content.find(" ",found2+1);
	string password_receieved = message_content.substr(found2+1,found3-found2-1);


	int reply;
	if (authMap[username_received] != password_receieved) // autho problem
	{

		reply = -1;
		string replyS = intToStringNetwork(reply);
		return replyS;
	}
	// autho passed

	size_t found4 = message_content.find(" ",found3+1);
	int viewRightsSize = stringToIntNetwork(message_content.substr(found3+1,found4-found3-1));

	int imageID = ownerMap.size();
	imageID++;
	ownerMap.insert(make_pair(imageID,username_received));

	map <string, pair<int,int>> arMap2;

	size_t found5 = found4;
	size_t found6;

	for(int i = 0 ; i<viewRightsSize; i++)
	{
		string user1;
		if (i==0) {
			found5 = message_content.find(" ", found4+1);
			 user1 = message_content.substr(found4+1,found5-found4-1);
		}
			else {
			found5 = message_content.find(" ", found6+1);

		 user1 = message_content.substr(found6+1,found5-found6-1);
		}
		found6 = message_content.find(" ", found5+1);
		int noOfViews = stringToIntNetwork(message_content.substr(found5+1,found6-found5-1));

		arMap2.insert(make_pair(user1,make_pair(noOfViews,1))); //new entry
		tuple<int,int,int> t(imageID,noOfViews,1);
		mutexaccessMap.lock();
		accessMap[user1].push_back(t);  // list of all images accessbile by a client
		mutexaccessMap.unlock();

		int reply = imageID;
		string replyS = intToStringNetwork(reply);
		return replyS;
	}
QImage image_content;
	if(viewRightsSize==0)
	{
		int imageSize = stringToIntNetwork(message_content.substr(found4+1,found5-found4-1));
		image_content = stringToQImage(message_content.substr(found5+1));
	}
	else{
	size_t found7 = message_content.find(" ",found6+1);
	int imageSize = stringToIntNetwork(message_content.substr(found6+1,found7-found6-1));
	 image_content = stringToQImage(message_content.substr(found7+1));
	}




		QImage stegimage ;
	// stegnography of arMap2 + image

    bool flag = steganography(imageID, image_content,arMap2,stegimage);
    if(flag == true) cout<<"stegano done"<<endl;

	mutexImageMap.lock();
	imageMap[imageID] = stegimage;
	mutexImageMap.unlock();

	reply = imageID;
	string replyS = intToStringNetwork(reply);
	return replyS;
}

string server::getTimeStamp(message m) // this function will reply with timestamp, -1 if autho problem
// using the imageID, the server destegnographies the image, get the access list and the timestamp using the username of the user
{
	string message_content = m.getMessage_content();

	size_t found = message_content.find(" ");
	size_t found1 = message_content.find(" ",found+1);
	string username_received = message_content.substr(found+1,found1-found-1);

	size_t found2 = message_content.find(" ",found1+1);
	size_t found3 = message_content.find(" ",found2+1);
	string password_receieved = message_content.substr(found2+1,found3-found2-1);

	int reply=0;
	if (authMap[username_received] != password_receieved) // autho problem
	{

		reply = -1;
		string replyS = intToStringNetwork(reply);
		return replyS;
	}
	//autho passed

	int imageID = stringToIntNetwork(message_content.substr(found3+1));

	mutexaccessMap.lock();
	vector<tuple<int,int,int>> v = accessMap[username_received];

	for(tuple<int,int,int> &tup: v)
	{
		if(get<0>(tup)== imageID)
		{
			reply = get<2>(tup);
			break;
		}
	}
	mutexaccessMap.unlock();



//	mutexImageMap.lock();
//	QImage image = imageMap[imageID];
//	mutexImageMap.unlock();
//	QImage desimage ;

//	// destegnography using QImage image. now we have map<string,pair<int,int>> accessMap for a certain image

//	map<string,pair<int,int>> accessMap2;

//    bool flag = deSteganography(imageID, image,accessMap2,desimage);


//	reply = accessMap2[username_received].second ;

	string replyS = intToStringNetwork(reply);
	return replyS;
}

string server::deleteImage(message m) // replies with 1 if autho problem or image ID is not found
// using the image ID, the server erases the image
{
	string message_content = m.getMessage_content();

	size_t found = message_content.find(" ");
	size_t found1 = message_content.find(" ",found+1);
	string username_received = message_content.substr(found+1,found1-found-1);

	size_t found2 = message_content.find(" ",found1+1);
	size_t found3 = message_content.find(" ",found2+1);
	string password_receieved = message_content.substr(found2+1,found3-found2-1);

	int reply;
	if (authMap[username_received] != password_receieved) // autho problem
	{

		reply = -1;
		string replyS = intToStringNetwork(reply);
		return replyS;
	}
	//autho passed

	int imageID = stringToIntNetwork(message_content.substr(found3+1));

	mutexImageMap.lock();

	if(ownerMap.find(imageID) == ownerMap.end() || imageMap.find(imageID) == imageMap.end())
	{
		reply = -1; // imageID is not found
		string replyS = intToStringNetwork(reply);
		return replyS;
	}

	ownerMap.erase(imageID);
	imageMap.erase(imageID);
	//erasing the image from accessMap
	mutexImageMap.unlock();
	mutexaccessMap.lock();
	vector <tuple<int,int,int>> v = accessMap[username_received] ; //modifying inside the accessMap and delete all images for users in the list

	for (int i=0; i < v.size(); i++)
		{
			if(get<0>(v[i]) == imageID)
			{
				v.erase(v.begin() + i);
				i--;
			}
		}
	mutexaccessMap.unlock();

	reply = 0 ;
	string replyS = intToStringNetwork(reply);
	return replyS;

}

string server::downloadImage(message m)
// using the image ID,  the image is sent to the client where he performs destegnography
{
	string message_content = m.getMessage_content();

	size_t found = message_content.find(" ");
	size_t found1 = message_content.find(" ",found+1);
	string username_received = message_content.substr(found+1,found1-found-1);

	size_t found2 = message_content.find(" ",found1+1);
	size_t found3 = message_content.find(" ",found2+1);
	string password_receieved = message_content.substr(found2+1,found3-found2-1);

	int reply;
	if (authMap[username_received] != password_receieved) // autho problem
	{

		string replyS = NULL;
		return replyS;
	}
	//autho passed


	int imageID = stringToIntNetwork(message_content.substr(found3+1));
	mutexImageMap.lock();
	QImage image = imageMap[imageID];
	mutexImageMap.unlock();

	string replyS = QImageToString(image);
	return replyS;
}

string server::getImageList(message m)
{
	string message_content = m.getMessage_content();

	size_t found = message_content.find(" ");
	size_t found1 = message_content.find(" ",found+1);
	string username_received = message_content.substr(found+1,found1-found-1);

	size_t found2 = message_content.find(" ",found1+1);
	size_t found3 = message_content.find(" ",found2+1);
	string password_receieved = message_content.substr(found2+1,found3-found2-1);


	if (authMap[username_received] != password_receieved) // autho problem
	{

		string messageContent = NULL;
		return messageContent;
	}
	//autho passed

	string messageContent;
	mutexaccessMap.lock();
	vector<tuple<int,int,int>> v = accessMap[username_received] ; //accessing the access vector for the given username

	int v_size = v.size();

	messageContent = intToStringNetwork(v_size) + " ";


	for(tuple<int,int,int> &tup: v)
	{
		messageContent += intToStringNetwork(get<0>(tup)) + " ";
		messageContent += intToStringNetwork(get<1>(tup)) + " ";
		messageContent += intToStringNetwork(get<2>(tup)) + " ";

	}
	mutexaccessMap.unlock();

	return messageContent;
}
