#include "includes.h"
#include "server.h"
#include "communication.h"

server* serverInstance;
communication *comm;
string operationName[] = {"checkUser","registerUser", "uploadImage", "downloadImage", "getTimeStamp", "updateList", "deleteImage", "getImageList", "GetUsersList", "GetUserListOnline", "LoginOperation", "LogoutOperation"};

bool *servingRequests(message requestMessage, int requestIP, int requestPort){
	long operationID = requestMessage.getOperation_ID();
    string reply;
    cout<<"operation " << operationName[operationID] <<" client " << requestIP << ":"<<requestPort<<endl;
	switch(operationID){

	case 0:
		reply = serverInstance->checkUser(requestMessage);
		break;
	case 1:
		reply = serverInstance->registerUser(requestMessage);
		break;
	case 2:
		reply = serverInstance->uploadImage(requestMessage);
		break;
	case 3:
		reply = serverInstance->downloadImage(requestMessage);
		break;
	case 4:
		reply = serverInstance->getTimeStamp(requestMessage);
		break;
	case 5:
		reply = serverInstance->updateList(requestMessage);
		break;
	case 6:
		reply = serverInstance->deleteImage(requestMessage);
		break;
	case 7:
		reply = serverInstance->getImageList(requestMessage);
		break;
    case 8:
        reply = serverInstance->getUsersList(requestMessage);
        break;
    case 9:
        reply = serverInstance->getOnlineUsersList(requestMessage);
        break;
    case 10:
        reply = serverInstance->loginUser(requestMessage);
        break;
    case 11:
        reply = serverInstance->logoutUser(requestMessage);
        break;
	default:
		reply = "invalid operation ID";
		break;

	}
	comm->sendMessage(message(Reply,Idempotent,operationID,
                             reply.size(),  reply), requestIP, requestPort, requestMessage.getMessageID());

}

void serverApplication(char*address, int port)
{
	comm = new communication();
	serverInstance = new server();
	comm->startCommunication(false,address, port);
	message receivedMessage;
	int clientIP,clientPort;

	while(true)
	{
		//usleep(300000);
		comm->receiveMessage(receivedMessage,clientIP,clientPort); //getRequest

		//craete new thread to serve the message

		thread servingThread(&servingRequests, receivedMessage,clientIP, clientPort);

		servingThread.join();

	}
}
