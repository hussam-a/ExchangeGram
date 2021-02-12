#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include "includes.h"

class UDPSocket
{
protected:
    int clientNumber;
    static int clientCounter;
    int sock;
    sockaddr_in myAddr;
    sockaddr_in peerAddr;
    unsigned int sock_len;
    char * myAddress;
    char * peerAddress;
    int myPort;
    int peerPort;
    mutex mutexSockLog;
    bool fileOpen;
    ofstream logFile;
    void sockLog(const string &s);
public:
    UDPSocket ();
    bool shutdownSocket ();
    bool initializeServer (char * _myAddress, int _myPort);
    bool initializeClient (char * _peerAddress, int _peerPort);
    int writeToSocket (char * buffer, int maxBytes );
    int readFromSocket (char * buffer, int maxBytes );
    char* getMyAddress();
    char* getPeerAddress();
    int getMyPort ();
    int getPeerPort ();
    bool setPeer(int _peerAddress,int _peerPort);
    bool getPeer(int &_peerAddress, int&_peerPort);
    ~UDPSocket ( );
};
#endif // UDPSOCKET_H
