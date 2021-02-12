#include "conversion_functions.h"

string  intToStringNetwork(const int &_n)
{
    return to_string(htonl(_n));
}

int stringToIntNetwork(const string &_s){
    return ntohl(int(stoul(_s)));
}

//string longToString(const long &_n){
//    return to_string(htobe64(_n));
//}

//long stringToLong(const string &_s)
//{
//    return be64toh(stol(_s));
//}

string mapStringIntToString(const map<string,int> &_m)
{
    string s = intToStringNetwork(_m.size());
    s += " ";

    for( map<string,int>::const_iterator it = _m.begin(); it != _m.end(); ++it) {
        string key = it->first;
        int value = it->second;
        s += key;
        s += " ";
        s += intToStringNetwork(value);
        s += " ";
    }

    return s;
}


string mapStringBoolToString(const map<string,bool> &_m){
    string s = intToStringNetwork(_m.size());
    s += " ";

    for( auto p : _m ){
        string key = p.first;
        s += key;
        s += " ";
    }

    return s;


}



string mapStringBoolToStringWithCheck(const map<string,bool> &_m){
    int checkSize = 0;
    for( auto p : _m) if(p.second) checkSize++;
    string s = intToStringNetwork(checkSize);
    s += " ";

    for( auto p : _m ){
        if(p.second == false) continue;
        string key = p.first;
        s += key;
        s += " ";
    }

    return s;


}


vector <tuple<int, int,int>> stringToVectorTupleIntIntInt(const string &_s, int& _lastSpaceFoundPostion)
{
    vector <tuple<int, int,int>> v;
    int stop1 =_s.find(' ');
    int stop2;
    int numOfItems = stringToIntNetwork(_s.substr(0,stop1));
    for (int i=0; i < numOfItems; ++i)
    {
        stop2= _s.find(' ',stop1+1);
        int first = stringToIntNetwork(_s.substr(stop1+1,(stop2-stop1-1)));
        stop1 =_s.find(' ', stop2+1);
        int second = stringToIntNetwork(_s.substr(stop2+1,(stop1-stop2-1)));
        stop2 =_s.find(' ', stop1+1);
        int third = stringToIntNetwork(_s.substr(stop1+1,(stop2-stop1-1)));
        v.push_back(tuple<int,int,int>(first,second,third));
    }
    _lastSpaceFoundPostion = stop2;
    return v;
}

map <int,pair< int,int>> stringToMapIntPairIntInt(const string &_s, int& _lastSpaceFoundPostion)
{
    map <int,pair< int,int>> m;
    int stop1 =_s.find(' ');
    int stop2,stop3;
    int first;
    int numOfItems = stringToIntNetwork(_s.substr(0,stop1));
    for (int i=0; i < numOfItems; ++i)
    {
        if(i==0)
        {
        stop2= _s.find(' ',stop1+1);
        first = stringToIntNetwork(_s.substr(stop1+1,(stop2-stop1-1)));
        }
        else{
            stop2 = _s.find(' ',stop3+1);
            first = stringToIntNetwork(_s.substr(stop1+1,(stop2-stop3-1)));
        }
        stop1 =_s.find(' ', stop2+1);
        int second = stringToIntNetwork(_s.substr(stop2+1,(stop1-stop2-1)));
        stop3 =_s.find(' ', stop1+1);
        int third = stringToIntNetwork(_s.substr(stop1+1,(stop3-stop1-1)));
        m[first]=make_pair(second,third);
    }
    _lastSpaceFoundPostion = stop2;
    return m;
}

string QImageToString(const QImage &_i)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    _i.save(&buffer,"png");        // writes the image in png format inside the buffer

    std::string strImage(byteArray.constData(), byteArray.length());
    string s = to_string(strImage.size());
    s += " ";
    s += strImage;
    return s;
}

QImage stringToQImage(const string &_s)
{
    size_t found = _s.find(" ");
    string ImageReceived = _s.substr(found+1,_s.size());

    QImage image;
    QByteArray baData(ImageReceived.c_str(),ImageReceived.length());
    image.loadFromData(baData,"png");

    return image;
}

bool steganography(const int& _imageID, const QImage &_toSteg, const map<string,pair<int,int>> &_accessMap, QImage& _steg)
{
//    time_t t = time(NULL);
//    string tmpName = string(to_string (_imageID) + "_" + to_string((int)t));
//    string tmpImageName = tmpName + ".png";
//    if (_toSteg.isNull() ) cout<<"stegano image is NULL"<<endl;
//    else cout <<"stega image NOT NULL"<<endl;
//    string tmpFileName = tmpName + ".txt";
//    bool saved = _toSteg.save("./tmp/" + QString::fromStdString(tmpImageName), "PNG");
//    if (saved == true) cout<<"stegano image saved "<<tmpImageName<<endl;
//    else perror("stegano not saved");
//    std::ofstream outFile ("./tmp/" + tmpFileName );
//    boost::archive::text_oarchive accessMap_oarch(outFile);
//    accessMap_oarch << _accessMap;
//    outFile.close();
//    string stegCommand = "lsb hide -i ./tmp/"+ tmpImageName +" -f ./tmp/" + tmpFileName + " -o ./tmp/"+ tmpImageName;
//    system(stegCommand.c_str());
//    usleep(1500000);
//    _steg.load("./tmp/" + QString::fromStdString(tmpImageName));
    //system( (string("rm ./tmp/"+tmpImageName + " ./tmp/"+tmpFileName)).c_str());
  _steg=_toSteg;
    return true;
}

bool deSteganography(const int& _imageID, const QImage &_steg, map<string,pair<int,int>>& _accessMap, QImage& _desteg)
{
//    time_t t = time(NULL);
//    string tmpName = string(to_string (_imageID) + "_" + to_string((int)t));
//    string tmpImageName = tmpName + ".png";
//    string tmpFileName = tmpName + ".txt";
//    bool saved = _steg.save("./tmp/" + QString::fromStdString(tmpImageName), "PNG");
//    if (saved == true) cout<<"destegano image saved "<<tmpImageName<<endl;
//    else perror("de stegano not saved");
//    string deStegCommand = "lsb  reveal -i ./tmp/"+ tmpImageName +" -o ./tmp/" + tmpFileName;
//    system(deStegCommand.c_str());
//    usleep(1500000);
//    std::ifstream inFile ("./tmp/" + tmpFileName);
//    boost::archive::text_iarchive accessMap_iarch(inFile);
//    accessMap_iarch >> _accessMap;
//    inFile.close();
//    _desteg.load("./tmp/" + QString::fromStdString(tmpImageName));
    //system( (string("rm ./tmp/"+tmpImageName + " ./tmp/"+tmpFileName)).c_str());
_desteg=_steg;
    return true;
}

//long packet_ID;
//long numOfPackets;
//long message_ID;
//bool message_type;              // 0 for request, 1 for reply
//bool message_nature;            //0 for idempotent, 1 for not.
//long Operation_ID;              // the ID of the called procedure
//long packet_Size;
//long message_Size;
//string packet_Content;

string packetToString(const packet& _p)
{
    return string(intToStringNetwork(_p.getPacketID()) + ' '
                  +intToStringNetwork(_p.getNumOfPackets()) + ' '
                  +intToStringNetwork(_p.getMessageID()) + ' '
                  +intToStringNetwork((int)_p.getMessageType()) + ' '
                  +intToStringNetwork((int)_p.getMessageNature()) + ' '
                  +intToStringNetwork(_p.getOperation_ID()) + ' '
                  +intToStringNetwork(_p.getPacketSize()) + ' '
                  +intToStringNetwork(_p.getMessageSize()) + ' '
                  +intToStringNetwork(_p.getPacketContent().size()) + ' '
                  +_p.getPacketContent());
}

packet stringToPacket(const string _s)
{
    size_t found = _s.find(' ',0);
    long packet_ID = stringToIntNetwork(_s.substr(0,found));
    size_t found1 = _s.find(' ',found+1);
    long numOfPackets = stringToIntNetwork(_s.substr(found+1,found1-found-1));
    found = _s.find(' ',found1+1);
    long message_ID =stringToIntNetwork(_s.substr(found1+1,found-found1-1));
    found1 = _s.find(' ',found+1);
    bool message_type =stringToIntNetwork(_s.substr(found+1,found1-found-1));
    found =  _s.find(' ',found1+1);
    bool message_nature= stringToIntNetwork(_s.substr(found1+1,found-found1-1));
    found1 = _s.find(' ',found+1);
    long Operation_ID =stringToIntNetwork(_s.substr(found+1,found1-found-1));
    found =  _s.find(' ',found1+1);
    long packet_Size =stringToIntNetwork(_s.substr(found1+1,found-found1-1));
    found1 = _s.find(' ',found+1);
    long message_Size =stringToIntNetwork(_s.substr(found+1,found1-found-1));
    found =  _s.find(' ',found1+1);
    int packet_ContentSize =stringToIntNetwork(_s.substr(found1+1,found-found1-1));
    string packet_Content = _s.substr(found+1,packet_ContentSize);

    return packet(packet_ID,numOfPackets,message_ID,message_type,message_nature,
                  Operation_ID,packet_Size,message_Size,packet_Content);
}


vector<string> stringToVectorStringNetwork(const string &_s, int &_lastSpaceFoundPostion)
{

    vector <string> v;
    int stop1 =_s.find(' ');
    //int stop2;
    int numOfItems = stringToIntNetwork(_s.substr(0,stop1));
    cout<<"number of users: "<<numOfItems<<endl;
    istringstream iss(_s.substr(stop1+1) );
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(v));

    return v;

}

string vectorStringToStringNetwork(const vector<string> &_list, int &_lastSpaceFoundPostion)
{

}
