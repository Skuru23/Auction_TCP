#ifndef RES_H
#define RES_H

#define CONNECTED 1000 //  connected

#define LOGINOK 1010 //  login success
#define UNAMENF 2011 //  usename not found
#define PASSWRONG 2012 //  password wrong
#define ALREADYLOGIN 2013 //  already login
#define ACCLOGIN 2014 //  This account already login in other client

#define JOINNOK 1020 //  Join success
#define NOTLOGIN 2021 //  Not logged in
#define ROOMNE 2021 //  Room not exist
#define ROOMF 2023 //  Room is full
#define ALREADYINROOM 2024 //  Already in other room

#define OUTOK 1030 //  Out success
#define NOTINROOM 2031 //  Not in room

#define LOGOUTOK 1040 //  Logout success

#define GETROOML 1050 //  get room list success server bắt đầu gửi dữ liệu list các phòng

#define ROOMCROK 1060 //  create room success
#define ALREADYEXISTR 2061 //  room already exist
#define ROOMLFULL 2062 //  room list is full

#define ADDITEMOK 1070 //  add success
#define ITEMQFULL 2072 //  Waiting Room Full

#define RETRIEVEOK 1080 // : retrieve sucess
#define ALREADYAUCITEM 2081 // : Item Already Auctioned
#define ITEMNE 2082 //  Nonexistent Item
#define ITEMNO 2083 //  Not own this item

#define BIDOK 1090 //  Successful Bid
#define NOTHIGHER 2091 //  Bid Higher Than Current
#define NOITEM 2092 //  No item to auction

#define BUYOK 1100 //  Successful Buy

#define SYNTAXERR 3000 //  Syntax error
#define SOLDED 4091 //  Item sold, new item arrived
#define NEWBEST 4092 //  Best price is 

#endif