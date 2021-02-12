#ifndef PACKET_H
#define PACKET_H
#include "includes.h"

class packet
{

  public:
	packet(){}
	packet(long _packet_ID,long _numOfPackets,long _message_ID,
	bool _message_type,              // 0 for request, 1 for reply
	bool _message_nature,            //0 for idempotent, 1 for not.
	long _Operation_ID,              // the ID of the called procedure
	long _packet_Size,
    long _message_Size,
    string _packet_Content);

    long getPacketID() const;
    long getNumOfPackets() const;
    long getMessageID() const;
    bool getMessageType() const;
    bool getMessageNature() const;
    long getOperation_ID() const;
    long getPacketSize() const;
    long getMessageSize() const;
    string getPacketContent() const;
    packet(const packet& packetToCopy);
    bool operator== (const packet & _ToCompare) const;
	~packet();

  private:
    long packet_ID;
    long numOfPackets;
    long message_ID;
    bool message_type;              // 0 for request, 1 for reply
    bool message_nature;            //0 for idempotent, 1 for not.
    long Operation_ID;              // the ID of the called procedure
    long packet_Size;
    long message_Size;
    string packet_Content;


    friend class boost::serialization::access;
  // When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<.  Likewise, when the class Archive
  // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & packet_ID;                         // either a message header 0, part of the message 1
        ar & numOfPackets;
        ar & message_ID;
        ar & message_type;
        ar & message_nature;
        ar & Operation_ID;
        ar & packet_Size;
        ar & message_Size;
        ar & packet_Content;

    // ADD HERE WHATEVER ATTRIBUTES YOU WANT TO SERIALIZE
    }

};

#endif // PACKET_H
