#ifndef MESSAGE_H
#define MESSAGE_H
#include "packet.h"

class message
{

public:
    message(bool _message_type,bool _messageNature, long _Operation_ID, long _message_size, string _message_content);
	message(std::deque <packet> all_packets);
    long getMessageID() const;
    bool getMessageType() const;
    bool getMessageNature() const;
    long getOperation_ID() const;
    long getMessageSize() const;
    string getMessage_content() const;
	bool divideIntoPackets(queue<std::tuple<int,int,packet> > &packetbuffer, int IP, int port);
    message ();
    message(const message& messageToCopy);
    ~message();

private:
    static long message_counter;
    long message_ID;
	bool message_type;              // 0 for request, 1 for reply
	bool message_nature;            //0 for idempotent, 1 for not.
    long operation_ID;              // the ID of the called procedure
	long message_size;
    string message_content;

};
#endif // MESSAGE_H
