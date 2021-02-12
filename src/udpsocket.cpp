#include "udpsocket.h"

int UDPSocket::clientCounter = 0;

void UDPSocket::sockLog(const string &s)
{
    if(fileOpen)
    {
        mutexSockLog.lock();
        logFile << "[" << std::chrono::high_resolution_clock::now().time_since_epoch().count()%10000000000
                << "]\t" << s << endl;
        mutexSockLog.unlock();
    }
}

UDPSocket::UDPSocket ()
{
    myAddress = NULL;
    peerAddress = NULL;
    myPort = -1;
    peerPort = -1;
    sock_len = 0;
    /*
  int socket(int domain, int type, int protocol);
  domain: IPv4 Internet protocols
  On success, a file descriptor for the new socket is returned.  On
  error, -1 is returned, and errno is set appropriately.
  */
    sock = socket(AF_INET, SOCK_DGRAM, 0 );
    if(sock == -1)
    {
        cout << "Failed to create a UDP socket." << endl;
    }
}

bool UDPSocket::shutdownSocket ()
{
    int Return =  shutdown(sock, SHUT_RDWR);
    if (Return == -1 && errno != ENOTCONN)
    {
        sockLog(string("Socket shutdown failed."));
        return false;
    }
    else
    {
        if (Return == 0 || errno == ENOTCONN)
        {
            sockLog(string("Socket shutdown."));
            return true;
        }
    }
    return false;
}

bool UDPSocket::initializeServer (char * _myAddress, int _myPort)
{
    logFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        logFile.open("socketLogServer.txt");
        fileOpen = true;
        sockLog("Server socket log ready");
    }
    catch(...)
    {
        cout << "Server socket couldnot open log. Socket will continue to function but will not report on its status."
             << endl;
    }

    myPort = _myPort;

    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(myPort);
    if ( _myAddress != NULL ){
        myAddress = _myAddress;
        if (strcmp (myAddress,"") !=0 ){
            myAddr.sin_addr.s_addr = inet_addr(myAddress);    }
        else myAddr.sin_addr.s_addr = INADDR_ANY;
    }
    else myAddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(myAddr.sin_zero), 8);

    //binding socket
    if( bind (sock, (struct sockaddr *) &myAddr, sizeof(struct sockaddr)) != 0)
    {
        close(sock);
        sockLog("Failed to bind server socket.");
        return false;
    }
    else
    {
        sockLog(string("Server socket bound to ") + string(_myAddress)+string(":") +to_string(_myPort));
    }

    sockLog("Server socket initialized.");
    return true;
}

bool UDPSocket::initializeClient (char * _peerAddress, int _peerPort)
{
    logFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
//        logFile.open("socketLogClient.txt");
        clientNumber = ++clientCounter;
        logFile.open(string("socket")+to_string(clientNumber));
        fileOpen = true;
        sockLog("Client socket log ready.");
    }
    catch(...)
    {
        cout << "Client socket couldnot open log. Socket will continue to function but will not report on its status." << endl;
    }

    //filling in peer/server fields
    peerPort = _peerPort;

    peerAddr.sin_family = AF_INET;
    peerAddr.sin_port = htons(peerPort);
    if ( _peerAddress != NULL ){
        peerAddress = _peerAddress;
        if (strcmp (peerAddress,"") !=0 ){
            peerAddr.sin_addr.s_addr = inet_addr(peerAddress);    }
        else peerAddr.sin_addr.s_addr = INADDR_ANY;
    }
    else peerAddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(peerAddr.sin_zero), 8);

    struct timeval tv;
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = TIMEOUT_USEC;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

    sockLog("Client socket initialized.");
    return true;
}

int UDPSocket::writeToSocket (char * buffer, int maxBytes ){
    if ( buffer != NULL )
    {
        sock_len = sizeof(peerAddr);
        int bytes_sent_count = sendto(sock,buffer,maxBytes,0,(struct sockaddr *)&peerAddr, sock_len);
        if( bytes_sent_count < 0)
        {
            sockLog(string("Data write error, to: ") + string(peerAddress) + string(":") + to_string(peerPort) );
            return -1;
        }
        if ( bytes_sent_count != maxBytes)
        {
            sockLog(string("Incomplete data write error, wrote: ") +
                    to_string(bytes_sent_count)+ string(" bytes, to: ")
                    + string(peerAddress) +string(":")+ to_string(peerPort));
            return -1;

        }

        else
        {
            sockLog(string("Wrote: ") +
                    to_string(bytes_sent_count)+ string(" bytes, to: ")
                    + string(peerAddress) +string(":")+ to_string(peerPort));
            return bytes_sent_count;
        }
    }
    else
    {
        sockLog(string("Write buffer error."));
        return -1;
    }
}

int UDPSocket::readFromSocket (char * buffer, int maxBytes )
{
    if ( buffer != NULL )
    {
        memset (buffer,0,maxBytes);
        sock_len = sizeof(peerAddr);
        int i = recvfrom(sock,buffer,maxBytes,0, (struct sockaddr *)&peerAddr,&sock_len);
        if( i < 0 )
        {
            sockLog(string("Data read error."));
            return -1;
        }
        else if (i == 0)
        {
            sockLog(string("Socket is shutting down."));
            return -1;
        }
        else if(i<maxBytes)
        {
            peerAddress = inet_ntoa(peerAddr.sin_addr);
            peerPort = ntohs(peerAddr.sin_port);
            sockLog(string("Incomplete Read: ") +
                    to_string(i)+ string(" bytes, from: ")
                    + string(peerAddress) +string(":")+ to_string(peerPort));
            return i;
        }
        else
        {
            peerAddress = inet_ntoa(peerAddr.sin_addr);
            peerPort = ntohs(peerAddr.sin_port);
            sockLog(string("Read: ") +
                    to_string(i)+ string(" bytes, from: ")
                    + string(peerAddress) +string(":")+ to_string(peerPort));
            return i;
        }
    }else
    {
        sockLog(string("Read buffer error."));
        return -1;
    }
}

int UDPSocket::getMyPort (){
    return myPort;

}
int UDPSocket::getPeerPort (){
    return peerPort;

}

char* UDPSocket::getMyAddress()
{
    return myAddress;
}

char* UDPSocket::getPeerAddress()
{
    return peerAddress;
}

bool UDPSocket::setPeer(int _peerAddress,int _peerPort)
{
    peerAddr.sin_addr.s_addr=htonl(_peerAddress);
    myAddr.sin_port = htons(_peerPort);
    return true;
}

bool UDPSocket::getPeer(int &_peerAddress, int&_peerPort)
{
    _peerAddress = ntohl(peerAddr.sin_addr.s_addr);
    _peerPort = peerPort;
    return true;
}

UDPSocket::~UDPSocket ( )
{
    close (sock);
    logFile.close();
}
