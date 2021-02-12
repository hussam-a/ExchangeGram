#include "communication.h"
extern string operationName[];

int communication::clientCounter = 0;

bool sortByPacketID(packet a, packet b)
{
    return (a.getPacketID()<b.getPacketID());
}

void communication::commLog(const string &s)
{
    if(fileOpen)
    {
        mutexCommLog.lock();
        logFile << "[" << std::chrono::high_resolution_clock::now().time_since_epoch().count()%10000000000
                << "]\t" << s << endl;
        mutexCommLog.unlock();
    }
}

communication::communication(){

}

bool communication::startCommunication(bool comm_type,char *address, int port)
{
    logFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        if (!comm_type)
        {logFile.open("communicationLogServer.txt");}
        else
        {
//            logFile.open("communicationLogClient.txt");}
            clientNumber = ++clientCounter;
            logFile.open(string("client")+to_string(clientNumber));}
        fileOpen = true;
        commLog("Communication log ready.");
    }
    catch(...)
    {
        fileOpen = false;
        if (!comm_type)
        {cout << "Server communication could not open log. "
                 "Communication will continue to function but will not report on its status." << endl;}
        else
        {cout << "Client communication could not open log. "
                 "Communication will continue to function but will not report on its status." << endl;}
    }

    terminate = false;
    waitingOnReply = false;
    clientReceiveFail = false;
    lastMessageIDReceived = 0;

    if (!comm_type){
        isClient = false;
        if(socket.initializeServer(address,port))
        {
            commLog("Server communication initialized.");
            sendThread = thread(&communication::sendThreadFunction, this);
            receiveThread = thread(&communication::receiveThreadFunction, this);
            return true;
        }
        else
        {
            commLog("Server communication failed to initialize.");
            return false;
        }
    }
    else{
        isClient = true;
        if(socket.initializeClient(address,port))
        {
            commLog("Client communication initialized.");
            sendThread = thread(&communication::sendThreadFunction, this);
            receiveThread = thread(&communication::receiveThreadFunction, this);
            return true;
        }
        else
        {
            commLog("Client communication failed to initialize.");
            return false;
        }
    }
}

bool communication::sendFromSocket(char* serialized_packet, int serialized_packet_size, int IP, int port){
    socket.setPeer(IP, port);
    if (socket.writeToSocket(serialized_packet,serialized_packet_size) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool communication::receiveFromSocket (char* packetContent, int &serialized_packet_size, int &IP, int &port)
{
    serialized_packet_size = socket.readFromSocket(packetContent,BASE64_SERIALIZED_SIZE );
    if (serialized_packet_size == -1)
    {

        return false;
    }

    socket.getPeer(IP,port);
    return true;
}

bool communication::stopCommunication()
{
    try
    {
        terminate = true;
        waitingOnReply= true;
        clientReceiveFail = false;
        socket.shutdownSocket();
        sendThread.join();
        receiveThread.join();
        commLog(string("Communication module shutdown."));
        logFile.close();
        return true;
    }
    catch(...)
    {
        commLog(string("Communication module shutdown failed."));
        return false;
    }
}

communication::~communication(){

}

bool communication::setPeer(int _peerAddress,int _peerPort)
{
    if (socket.setPeer(_peerAddress, _peerPort))
    {return true;}
    else
    {return false;}
}

bool communication::getPeer(int &_peerAddress, int&_peerPort)
{
    if (socket.getPeer(_peerAddress, _peerPort))
    {return true;}
    else
    {return false;}
}

bool communication::sendMessage(message messageToSend, int IP, int port, int clientMSGID)
{
    lastMessageSent = tuple<int,int,message>(IP,port,messageToSend);
    mutexOutgoingPackets.lock();
    messageToSend.divideIntoPackets(outgoingPackets, IP, port);
    mutexOutgoingPackets.unlock();

    if (isClient)
    {
        commLog(string("REQUEST\tMessage put on send queue: ID:") + to_string(messageToSend.getMessageID()) + string(" OpID:") + operationName[messageToSend.getOperation_ID()]+
                string(", ")+ to_string(messageToSend.getMessageSize())+ string(" bytes, to: ")
                + string(socket.getPeerAddress()) +string(":")+ to_string(port));
    }
    else
    {
        commLog(string("REPLY\tMessage put on send queue: ID:") + to_string(messageToSend.getMessageID()) + string(" OpID:") + operationName[messageToSend.getOperation_ID()]+
                string(", ")+ to_string(messageToSend.getMessageSize())+ string(" bytes, to: ")
                + string(socket.getPeerAddress()) +string(":")+ to_string(port));
    }

    if (!isClient)
    {
        if(repliedQueue.size()>=HISTORYSIZE)
        {
            tuple<int,int,int> remove = repliedQueue.front();
            repliedQueue.pop();
            sentReplies.erase(sentReplies.find(remove));
        }
        repliedQueue.push(tuple<int,int,int>(IP,port,clientMSGID));
        mutexSentReplies.lock();
        sentReplies[tuple<int,int,int>(IP,port,clientMSGID)]=messageToSend;
        mutexSentReplies.unlock();
    }

    return true;
}

void communication::sendThreadFunction()
{
    while(1)
    {
        if (terminate)
        {
            return;
        }
        mutexOutgoingPackets.lock();
        if(outgoingPackets.size() > 0)
        {
            tuple<int,int,packet> tupleToSend = outgoingPackets.front();
            outgoingPackets.pop();
            mutexOutgoingPackets.unlock();
            packet  _packet = std::get<2>(tupleToSend);
            struct in_addr ip_addr;
            ip_addr.s_addr = ntohl(std::get<0>(tupleToSend));
            string serialized_packet = packetToString( _packet);
            string sent_string = base64_encode
                    (reinterpret_cast<const unsigned char *>(serialized_packet.c_str()),serialized_packet.size());
            sent_string.resize (serialized_packet.size()+(BASE64_SERIALIZED_SIZE-serialized_packet.size()),'+');
            if(sendFromSocket((char *)(sent_string.c_str()), BASE64_SERIALIZED_SIZE,
                              std::get<0>(tupleToSend), std::get<1>(tupleToSend)))
            {
                commLog(string("Packet written to socket: Packet ID:") + to_string(_packet.getPacketID())
                        + ", "  + to_string(_packet.getPacketSize())+ string(" bytes, Message ID:")+ to_string(_packet.getMessageID())+
                        string(", to: ")
                        + string(inet_ntoa(ip_addr))
                        +string(":")+ to_string(std::get<1>(tupleToSend)));
            }
            else
            {
                commLog(string("Packet failed to write to socket: Packet ID:") + to_string(_packet.getPacketID())
                        + ", "  + to_string(_packet.getPacketSize())+ string(" bytes, to: ")
                        +string(" Message ID:")+ to_string(_packet.getMessageID())+
                        string(", to: ")
                        + string(inet_ntoa(ip_addr))
                        +string(":")+ to_string(std::get<1>(tupleToSend)));
            }

        }
        else
        {
            mutexOutgoingPackets.unlock();
        }
    }

}

bool communication::receiveMessage(message &messageContainer, int &IP, int &port)
{
    mutexReceiveMessage.lock();
    tuple<int,int,message> tupleReceived;
    while (true)
    {
        mutexWaitingOnReply.lock();
        waitingOnReply = true;
        mutexWaitingOnReply.unlock();
        if(isClient)
        {
            mutexClientReceiveFail.lock();
            if(clientReceiveFail)
            {
                clientReceiveFail = false;
                mutexClientReceiveFail.unlock();
                commLog(string("Permenantly failed to receive REPLY for last REQUEST sent"));
                return false;
            }
            else
            {
                mutexClientReceiveFail.unlock();
            }
        }
        mutexIncomingMessages.lock();
        if (incomingMessages.size()>0)
        {
            tupleReceived = incomingMessages.front();
            incomingMessages.pop();
            mutexIncomingMessages.unlock();
            IP = get<0>(tupleReceived);
            port = get<1>(tupleReceived);
            messageContainer = get<2>(tupleReceived);
            struct in_addr ip_addr;
            ip_addr.s_addr = ntohl(IP);
            if (isClient)
            {
                if (lastMessageIDReceived < messageContainer.getMessageID())
                {
                    lastMessageIDReceived = messageContainer.getMessageID();
                    commLog(string("REPLY\tMessage received: ID:") + to_string(messageContainer.getMessageID())
                             + string(" OpID:") + operationName[messageContainer.getOperation_ID()]+string(", ")+ to_string(messageContainer.getMessageSize())+ string(" bytes, from: ")
                            + string(inet_ntoa(ip_addr)) +string(":")+ to_string(port));
                    mutexReceiveMessage.unlock();
                    return true;
                }
                else
                {
                    commLog(string("REPLY\tDuplicate message received and discarded: ID:") + to_string(messageContainer.getMessageID())+
                             string(" OpID:") + operationName[messageContainer.getOperation_ID()]+ string(", ")+ to_string(messageContainer.getMessageSize())+ string(" bytes, from: ")
                            + string(inet_ntoa(ip_addr)) +string(":")+ to_string(port));
                    continue;
                }
            }
            else
            {
                mutexSentReplies.lock();
                if (sentReplies.find(tuple<int,int,int>(IP,port,messageContainer.getMessageID()))==sentReplies.end())
                {
                    mutexSentReplies.unlock();
                    commLog(string("REQUEST\tMessage received: ID:") + to_string(messageContainer.getMessageID())+
                              string(" OpID:") + operationName[messageContainer.getOperation_ID()]+ string(", ")+ to_string(messageContainer.getMessageSize())+ string(" bytes, from: ")
                            + string(inet_ntoa(ip_addr)) +string(":")+ to_string(port));
                    mutexReceiveMessage.unlock();
                    return true;
                }
                else
                {
                    commLog(string("REQUEST\tMessage received has already been replied to, REPLY resent: ID:") + to_string(messageContainer.getMessageID())+
                             string(" OpID:") + operationName[messageContainer.getOperation_ID()]+ string(", ")+ to_string(messageContainer.getMessageSize())+ string(" bytes, to: ")
                            + string(inet_ntoa(ip_addr)) +string(":")+ to_string(port));
                    mutexSentReplies.unlock();
                    sendMessage(sentReplies[tuple<int,int,int>(IP,port,messageContainer.getMessageID())]
                            ,IP,port);
                    continue;
                }
            }
        }
        else
        {
            mutexIncomingMessages.unlock();
        }
    }
}

void communication::receiveThreadFunction()
{
    while(1)
    {
        if(isClient && !waitingOnReply)
        {
            while(1)
            {
                mutexWaitingOnReply.lock();
                if (waitingOnReply)
                {
                    mutexWaitingOnReply.unlock();
                    if (terminate)
                    {
                        return;
                    }
                    break;
                }
                else
                {
                    mutexWaitingOnReply.unlock();
                    if (terminate)
                    {
                        return;
                    }
                }
            }
        }
        char * received_string= new char[BASE64_SERIALIZED_SIZE];
        int received_size;
        int IP, port;
        bool status = receiveFromSocket(received_string,received_size, IP, port);
        if (isClient && !status)
        {
            int i;
            commLog("Packet receive fail.");
            for (i =0 ; i < TIMEOUT_RETRANSMIT_TIMES; ++i)
            {
                if (terminate)
                {
                    return;
                }
                mutexWaitingOnReply.lock();
                if(waitingOnReply == false)
                {
                    break;
                }
                else{
                    mutexWaitingOnReply.unlock();}
                commLog(string("Packet receive timed-out. Retransmitting last REQEUST sent."));
                sendMessage(get<2>(lastMessageSent),get<0>(lastMessageSent),get<1>(lastMessageSent));
                status = receiveFromSocket(received_string,received_size, IP, port);
                if(status)
                {
                    break;
                }
                else
                {
                    commLog("Packet receive fail.");
                }
            }
            if(i == TIMEOUT_RETRANSMIT_TIMES)
            {
                mutexWaitingOnReply.lock();
                waitingOnReply = false;
                mutexWaitingOnReply.unlock();
                mutexClientReceiveFail.lock();
                clientReceiveFail = true;
                mutexClientReceiveFail.unlock();
                continue;
            }
        }

        if (terminate)
        {
            return;
        }
        string serialized_received_packet = base64_decode(string(received_string));

        packet received_packet;
        try
        {
            received_packet =  stringToPacket(serialized_received_packet);
        }
        catch(...)
        {
            commLog("Corrupt packet received.");
            continue;
        }

        struct in_addr ip_addr;
        ip_addr.s_addr = ntohl(IP);
        commLog(string("Packet read from socket: Packet ID:") + to_string(received_packet.getPacketID())
                + ", "  + to_string(received_packet.getPacketSize())+ string(" bytes, Message ID:")
                + to_string(received_packet.getMessageID())+
                string(", from: ")
                + string(inet_ntoa(ip_addr))
                +string(":")+ to_string(port));

        mutexIncomingPackets.lock();
        if(incomingPackets.find(make_pair(IP,port)) != incomingPackets.end() )
        {
            if(received_packet.getMessageID() == incomingPackets[make_pair(IP,port)].front().getMessageID())
            {
                if (find(incomingPackets[make_pair(IP,port)].begin()
                         ,incomingPackets[make_pair(IP,port)].end(), received_packet)
                        == incomingPackets[make_pair(IP,port)].end())
                {
                    incomingPackets[make_pair(IP,port)].push_back(received_packet);
                    if (received_packet.getNumOfPackets()==int(incomingPackets[make_pair(IP,port)].size()))
                    {
                        commLog(string("Incoming message now on queue: ID:")
                                + to_string(incomingPackets[make_pair(IP,port)].front().getMessageID())+
                                string(", ")+
                                to_string(incomingPackets[make_pair(IP,port)].front().getMessageSize())
                                + string(" bytes, from: ")
                                + string(inet_ntoa(ip_addr)) +string(":")+ to_string(port));
                        sort(incomingPackets[make_pair(IP,port)].begin(),incomingPackets[make_pair(IP,port)].end(),sortByPacketID);
                        mutexIncomingMessages.lock();
                        incomingMessages.push(tuple<int,int,message>(IP,port,(message(incomingPackets[make_pair(IP,port)]))));
                        mutexIncomingMessages.unlock();
                        incomingPackets.erase(make_pair(IP,port));
                        mutexWaitingOnReply.lock();
                        waitingOnReply = false;
                        mutexWaitingOnReply.unlock();
                    }
                }
                else
                {
                    commLog(string("Duplicate packet discarded: Packet ID:") + to_string(received_packet.getPacketID())
                            + ", "  + to_string(received_packet.getPacketSize())+ string(" bytes, Message ID:")
                            + to_string(received_packet.getMessageID())+
                            string(", from: ")
                            + string(inet_ntoa(ip_addr))
                            +string(":")+ to_string(port));
                }
            }
            else
            {
                if(received_packet.getMessageID() > incomingPackets[make_pair(IP,port)].front().getMessageID())
                {
                    commLog(string("Cleared old receive queue, abandoned old packets from: ")
                            + string(inet_ntoa(ip_addr))
                            +string(":")+ to_string(port));
                    incomingPackets[make_pair(IP,port)].clear();
                    incomingPackets[make_pair(IP,port)].push_back(received_packet);
                    if (received_packet.getNumOfPackets()==int(incomingPackets[make_pair(IP,port)].size()))
                    {
                        commLog(string("Incoming message now on queue: ID:")
                                + to_string(incomingPackets[make_pair(IP,port)].front().getMessageID())+
                                string(", ")+
                                to_string(incomingPackets[make_pair(IP,port)].front().getMessageSize())
                                + string(" bytes, from: ")
                                + string(inet_ntoa(ip_addr)) +string(":")+ to_string(port));
                        sort(incomingPackets[make_pair(IP,port)].begin(),incomingPackets[make_pair(IP,port)].end(),sortByPacketID);
                        mutexIncomingMessages.lock();
                        incomingMessages.push(tuple<int,int,message>(IP,port,(message(incomingPackets[make_pair(IP,port)]))));
                        mutexIncomingMessages.unlock();
                        incomingPackets.erase(make_pair(IP,port));
                        mutexWaitingOnReply.lock();
                        waitingOnReply = false;
                        mutexWaitingOnReply.unlock();
                    }
                }
                else
                {
                    commLog(string("Old packet discarded from: ") + string(inet_ntoa(ip_addr))
                            +string(":")+ to_string(port));
                }
            }

        }
        else
        {
            incomingPackets[make_pair(IP,port)].push_back(received_packet);
            if (received_packet.getNumOfPackets()==int(incomingPackets[make_pair(IP,port)].size()))
            {
                commLog(string("Incoming message now on queue: ID:")
                        + to_string(incomingPackets[make_pair(IP,port)].front().getMessageID())+
                        string(", ")+
                        to_string(incomingPackets[make_pair(IP,port)].front().getMessageSize())
                        + string(" bytes, from: ")
                        + string(inet_ntoa(ip_addr)) +string(":")+ to_string(port));
                sort(incomingPackets[make_pair(IP,port)].begin(),incomingPackets[make_pair(IP,port)].end(),sortByPacketID);
                mutexIncomingMessages.lock();
                incomingMessages.push(tuple<int,int,message>(IP,port,(message(incomingPackets[make_pair(IP,port)]))));
                mutexIncomingMessages.unlock();
                incomingPackets.erase(make_pair(IP,port));
                mutexWaitingOnReply.lock();
                waitingOnReply = false;
                mutexWaitingOnReply.unlock();
            }
        }
        mutexIncomingPackets.unlock();
    }
}
