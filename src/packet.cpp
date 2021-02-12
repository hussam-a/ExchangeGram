#include "packet.h"

packet::packet(long _packet_ID,long _numOfPackets,long _message_ID,
               bool _message_type,              // 0 for request, 1 for reply
               bool _message_nature,            //0 for idempotent, 1 for not.
               long _Operation_ID,              // the ID of the called procedure
               long _packet_Size,
               long _message_Size,
               string _packet_Content)
{
    packet_ID = _packet_ID;
    numOfPackets = _numOfPackets;
    message_ID = _message_ID;
    message_type= _message_type;              // 0 for request, 1 for reply
    message_nature= _message_nature;            //0 for idempotent, 1 for not.
    Operation_ID = _Operation_ID;              // the ID of the called procedure
    packet_Size = _packet_Size;
    message_Size = _message_Size;
    packet_Content = _packet_Content;
    //	if(packet_Size > 0)
    //	{
    //		packet_Content = new char[packet_Size];
    //		strcpy( packet_Content, _packet_Content);
    //	}
    //	else
    //	{
    //		packet_Content = nullptr;
    //	}
}

long packet::getPacketID() const{
    return packet_ID;
}

string packet::getPacketContent() const{
    return packet_Content;
}
long packet::getNumOfPackets() const{
    return numOfPackets;
}

long packet::getMessageID() const{
    return message_ID;
}

bool packet::getMessageType() const{
    return message_type;
}

bool packet::getMessageNature() const{
    return message_nature;
}

long packet::getOperation_ID() const{
    return Operation_ID;
}

long packet::getPacketSize() const{
    return packet_Size;
}
long packet::getMessageSize() const{
    return message_Size;
}

packet::packet(const packet& packetToCopy)
{
    packet_ID = packetToCopy.getPacketID();
    numOfPackets = packetToCopy.getNumOfPackets();
    message_ID = packetToCopy.getMessageID();
    message_type = packetToCopy.getMessageType();         // 0 for request, 1 for reply
    message_nature = packetToCopy.getMessageNature();           //0 for idempotent, 1 for not.
    Operation_ID = packetToCopy.getOperation_ID();            // the ID of the called procedure
    packet_Size = packetToCopy.getPacketSize();
    message_Size = packetToCopy.getMessageSize();
    //	packet_Content = new char[packet_Size];
    //    strcpy(packet_Content,packetToCopy.getPacketContent());
    packet_Content = packetToCopy.getPacketContent();
}

bool packet::operator== (const packet & _ToCompare) const
{
    if( packet_ID != _ToCompare.getPacketID()) return false;
    if( numOfPackets != _ToCompare.getNumOfPackets()) return false;
    if( message_ID != _ToCompare.getMessageID()) return false;
    if(message_type != _ToCompare.getMessageType()) return false;
    if( message_nature != _ToCompare.getMessageNature()) return false;
    if(Operation_ID != _ToCompare.getOperation_ID()) return false;
    if(packet_Size != _ToCompare.getPacketSize()) return false;
    if(message_Size != _ToCompare.getMessageSize()) return false;
    if(packet_Content != _ToCompare.getPacketContent()) return false;
    return true;
}

packet::~packet(){
    //delete [] packet_Content;
}
