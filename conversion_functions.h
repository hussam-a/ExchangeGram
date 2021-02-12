#ifndef CONVERSION_FUNCTIONS_H
#define CONVERSION_FUNCTIONS_H
#include "includes.h"
#include "packet.h"

string intToStringNetwork(const int &_n);

int stringToIntNetwork(const string &_s);

vector <string> stringToVectorStringNetwork(const string &_s, int& _lastSpaceFoundPostion);   // on the client side

string vectorStringToStringNetwork(const vector<string> & _list, int& _lastSpaceFoundPostion);    // for the server side

//string longToString(const long &_n);

//long stringToLong(const string &_s);

string mapStringIntToString(const map<string,int> &_m);


string mapStringBoolToString(const map<string,bool> &_m);


string mapStringBoolToStringWithCheck(const map<string,bool> &_m);


vector <tuple<int, int,int>> stringToVectorTupleIntIntInt(const string &_s, int& _lastSpaceFoundPostion);

map <int,pair< int,int>> stringToMapIntPairIntInt(const string &_s, int& _lastSpaceFoundPostion);

QImage stringToQImage(const string &_s);

string QImageToString(const QImage &_i);

bool steganography(const int& _imageID, const QImage &_toSteg, const map<string,pair<int,int>> &_accessMap, QImage& _steg); // map of string username to pair of noOfViews and timestamp

bool deSteganography(const int& _imageID, const QImage &_steg, map<string,pair<int,int>>& _accessMap, QImage& _desteg);

string packetToString(const packet& _p);

packet stringToPacket(const string _s);

#endif // CONVERSION_FUNCTIONS_H
