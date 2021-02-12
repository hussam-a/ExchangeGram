#ifndef INCLUDES_H
#define INCLUDES_H
#include <sys/types.h>
#include <sys/socket.h>
#include <endian.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cstddef>
#include <cstdint>
#include <malloc.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <ctype.h>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <queue>
#include <mutex>
#include <map>
#include <tuple>
#include <deque>
#include <thread>
#include <time.h>
#include <sstream>
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <QImage>
#include <QFile>
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QBuffer>
#include <QIODevice>
#include <QImageReader>
#include<iostream>
#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
using namespace std;
using namespace boost;

#define MAX_PACKET_CONTENT_SIZE 4096
#define SERIALIZATION_BUFFER_SIZE 4700
#define BASE64_SERIALIZED_SIZE 6268 //ceil(n / 3) * 4 = BASE64 formula
#define INBETWEEN_REST 1000
#define TIMEOUT_RETRANSMIT_TIMES 12
#define TIMEOUT_SEC 5
#define TIMEOUT_USEC 0
#define HISTORYSIZE 50

#define Request 0
#define Reply 1

#define	Idempotent 0
#define NonIdempotent 1

#define CheckUserOperation 0
//Client sends: username (string)
//Server replies: -2 unexpected problem, 0 username does not exist, 1 username exists

#define RegisterOperation 1
//Client sends: username (string) and password (string),
//Server replies: -2 unexpected problem, -1 auth problem , 0 success, 1 username exists

#define UploadOperation 2
//Client sends: username (string) and password (string), access list , QImage
//Server replies: -2 unexpected problem, -1 auth problem , postive interger imageID

#define DownloadOperation 3
//Client sends: username (string) and password (string), imageid (int) ,
//Server replies: QImage or false image if auth failed or image not found or unexpected error
//both embedded with either true list or empty list

#define GetRevisionCountOperation 4
//Client sends: username (string) and password (string), imageid (int) ,
//Server replies: -2 unexpected problem, -1 auth problem, int revisionCount for that user for that image

#define UpdateListOperation 5
//Client sends: username (string) and password (string), imageid (int),  updated access list
//Server replies: -2 unexpected problem, -1 auth problem, 0 success

#define DeleteOperation 6
//Client sends: username (string) and password (string), imageid (int),
//Server replies: -2 unexpected problem, -1 auth problem, 0 success

#define GetImageList 7
//Client sends: username (string) and password (string),
//Server replies: map of images accessible by this user in form of
//map of int (image id) and pair of int (max views for that user) and int revision count (for that user for that image, for that image)
//empty map if auth error or unexpected error

#define GetUsersList 8
//Client sends: username (string) and password (string),
//Server replies: vector <string> of all users

#define GetUserListOnline 9
//Client sends: username (string) and password (string),
//Server replies: vector <string> of all users online

#define LoginOperation 10
//Client sends: username (string) and password (string),
//Server replies: -2 unexpected problem, -1 auth problem , 0 success

#define LogoutOperation 11
//Client sends: username (string) and password (string),
//Server replies: -2 unexpected problem, 0 success

#endif	//INCLUDES_H
