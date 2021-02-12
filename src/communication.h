#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include "message.h"
#include "udpsocket.h"
#include "base64.h"
#include "conversion_functions.h"

class communication
{
public:
    bool sendMessage(message messageToSend, int IP,int port, int clientMSGID=0);
    bool receiveMessage(message &messageContainer, int &IP, int &port);
    bool startCommunication(bool comm_type,char *address, int port);
    void sendThreadFunction();
    void receiveThreadFunction();
    bool stopCommunication();
    communication();
    ~communication();
    bool setPeer(int _peerAddress,int _peerPort);
    bool getPeer(int &_peerAddress, int&_peerPort);

private:
    static int clientCounter;
    int clientNumber;

    mutex mutexReceiveMessage;

    void commLog(const string &s);
    bool receiveFromSocket (char* packetContent, int &serialized_packet_size, int &IP, int &port);
    bool sendFromSocket(char* packetContent,int serialized_packet_size, int IP, int port);

    bool isClient;
    bool terminate;

    mutex mutexClientReceiveFail;
    bool clientReceiveFail;

    mutex mutexWaitingOnReply;
    bool waitingOnReply;

    bool fileOpen;
    mutex mutexCommLog;
    ofstream logFile;

    tuple<int,int,message> lastMessageSent;

    mutex mutexIncomingPackets;
    map<std::pair<int,int>,std::deque<packet> > incomingPackets;

    mutex mutexOutgoingPackets;
    queue<std::tuple<int,int,packet> >  outgoingPackets;

    mutex mutexIncomingMessages;
    queue<std::tuple<int,int,message> > incomingMessages;

    int lastMessageIDReceived;
    queue<tuple<int,int,int>> repliedQueue;

    mutex mutexSentReplies;
    map<tuple<int,int,int>,message> sentReplies;

    UDPSocket socket;
    thread sendThread, receiveThread;
};
#endif // COMMUNICATION_H
