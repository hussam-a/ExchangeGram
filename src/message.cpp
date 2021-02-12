# include "message.h"

long message::message_counter = 0;

message::message(
        bool _message_type,              // 0 for request, 1 for reply
        bool _message_nature,            //0 for idempotent, 1 for not.
        long _operation_ID,              // the ID of the called procedure
        long _message_size,
        string _message_content)
{
    message_counter++;
    message_ID = message_counter;
    message_type= _message_type;              // 0 for request, 1 for reply
            message_nature= _message_nature;            //0 for idempotent, 1 for not.
            operation_ID=_operation_ID;              // the ID of the called procedure
            message_size=_message_size;
            //message_content = new char[message_size];
            //strcpy( message_content, _message_content);
            message_content =  _message_content;

}
message::message(std::deque<packet> all_packets)
{

    packet p = all_packets.front();
    message_ID = p.getMessageID();
    message_type = p.getMessageType();
    message_nature = p.getMessageNature();
    operation_ID = p.getOperation_ID();
    message_size = p.getMessageSize();
    message_content = "";
    //message_content = new char[message_size];
    for (int i=0; i< int(all_packets.size()) ; i++)
    {
        string p_content = all_packets[i].getPacketContent();
        message_content += p_content;
        //strcat(message_content,p_content);
    }
}

//bool message::addPacket(packet &newPacket){
//  if(msgs_buffer.size() < message_header.NumOfPackets){
//      msgs_buffer.push(newPacket);
//      return true;
//  }
//  return false;
//}

//bool message::removePacket(packet &newPacket){
//  if(!msgs_buffer.empty()){
//      newPacket = msgs_buffer.front();
//      msgs_buffer.pop();
//      return true;
//  }
//  return false;
//}

long message::getMessageID() const{
    return message_ID;
}

bool message::getMessageType() const
{
    return message_type;
}

bool message::getMessageNature() const
{
    return message_nature;
}

long message::getOperation_ID() const
{
    return operation_ID;
}

long message::getMessageSize() const
{
    return message_size;
}

string message::getMessage_content() const
{
    return message_content;
}

//packet(long packet_ID,long numOfPackets,long message_ID,
//bool message_type,              // 0 for request, 1 for reply
//bool message_nature,            //0 for idempotent, 1 for not.
//long Operation_ID,              // the ID of the called procedure
//long packet_Size,
//char * packet_Content);

bool message::divideIntoPackets(queue<std::tuple<int,int,packet> > &packetbuffer, int IP, int port)
{
    int numOfPackets = message_size / MAX_PACKET_CONTENT_SIZE;

    if(message_size%MAX_PACKET_CONTENT_SIZE !=0){
        numOfPackets++;
    }

    int i=0;
    for (; i <numOfPackets-1 ; i++)
    {
        packet p(i+1,numOfPackets,message_ID,message_type,
                 message_nature,operation_ID, MAX_PACKET_CONTENT_SIZE,message_size,
                 message_content.substr(i*MAX_PACKET_CONTENT_SIZE,MAX_PACKET_CONTENT_SIZE));
        packetbuffer.push(tuple<int,int,packet>(IP, port,p));
    }

    if(message_size%MAX_PACKET_CONTENT_SIZE !=0)
    {
        packet p (i+1,numOfPackets,message_ID,message_type,
                  message_nature,operation_ID,
                  message_size%MAX_PACKET_CONTENT_SIZE,message_size,
                  message_content.substr(i*MAX_PACKET_CONTENT_SIZE, message_size%MAX_PACKET_CONTENT_SIZE));
        packetbuffer.push(tuple<int,int,packet>(IP, port,p));
    }

    return true;
}

message::message ()
{
    message_ID = 0;
    message_type = 0;              // 0 for request, 1 for reply
    message_nature = 0;            //0 for idempotent, 1 for not.
    operation_ID = 0;              // the ID of the called procedure
    message_size = 0;
    message_content = "";
}

message::message(const message& messageToCopy)
{
    message_ID = messageToCopy.getMessageID();
    message_type= messageToCopy.getMessageType();
    message_nature= messageToCopy.getMessageNature();
    operation_ID= messageToCopy.getOperation_ID();
    message_size= messageToCopy.getMessageSize();
    //message_content = new char[message_size];
    //strcpy(message_content,messageToCopy.getMessage_content());
    message_content = messageToCopy.getMessage_content();
}

message::~message(){

}
