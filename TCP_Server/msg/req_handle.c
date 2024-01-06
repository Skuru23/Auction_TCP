#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>

#include "req_handle.h"
#include "send_msg.h"
#include "../room/room.h"
#include "../item/item.h"
#include "../val/response.h"

#include "../bid/bid.h"
#include "../auth/auth_handle.h"

#define BUFF_SIZE 1024

int request_handle(int sesit, char *req)
{
    char cmd[10];
    sscanf(req, "%s", cmd);
    if (strcmp(cmd, "LOGIN") == 0)
    {
        if (sess_store[sesit].is_loggedin == 1)
        {
            send_code(sess_store[sesit].conn_sock, ALREADYLOGIN);
            return 1;
        }
        char username[1024];
        char password[1024];
        memset(username, '\0', sizeof(username));
        sscanf(req, "LOGIN %s %s", username, password);

        switch (login_handle(username, password))
        {
        case LOGIN_SUCCESS:
            sess_store[sesit].is_loggedin = 1;
            return send_code(sess_store[sesit].conn_sock, LOGINOK);
        case LG_USER_BLOCK:
            return send_code(sess_store[sesit].conn_sock, ACCBLOCK);
        case INCORRECT_PASSWORD:
            return send_code(sess_store[sesit].conn_sock, WRONG_PASSWORD);
        case LG_USER_NOT_EXIST:
            return send_code(sess_store[sesit].conn_sock, UNAMENF);
        default:
            break;
        }
    }
    else if (strcmp(cmd, "SIGNUP") == 0)
    {
        if (sess_store[sesit].is_loggedin == 1)
        {
            send_code(sess_store[sesit].conn_sock, ALREADYLOGIN);
            return 1;
        }
        char username[1024];
        char password[1024];
        memset(username, '\0', sizeof(username));
        sscanf(req, "SIGNUP %s %s", username, password);
        if (signup_handle(username, password))
        {
            return send_code(sess_store[sesit].conn_sock, SIGNUPSUCESS);
        }
        return send_code(sess_store[sesit].conn_sock, SIGNUPFAIL);
    }
    else if (strcmp(cmd, "JOIN") == 0)
    {
        char room_name[1024];
        memset(room_name, '\0', sizeof(room_name));
        sscanf(req, "JOIN %s", room_name);
        switch (join_room(room_name, sesit))
        {
        case USER_NOT_LOGINED_IN:
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
            break;
        case ALREADY_IN_ROOM:
            return send_code(sess_store[sesit].conn_sock, ALREADYINROOM);
            break;
        case ROOM_NOT_FOUND:
            return send_code(sess_store[sesit].conn_sock, ROOMNE);
            break;
        case FULL_ROOM:
            return send_code(sess_store[sesit].conn_sock, ROOMF);
            break;
        case ROOM_OK:
            return send_code(sess_store[sesit].conn_sock, JOINNOK);
            break;
        default:
            return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
            break;
        }
    }

    // Create Room Handle
    else if (strcmp(cmd, "ROOMCR") == 0)
    {
        char room_name[30];
        sscanf(req, "ROOMCR %s", room_name);
        if (strlen(room_name) <= 0)
        {
            printf("Not found Name\n");
            return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
        }

        switch (create_room(room_name, sess_store[sesit]))
        {
        case 0:
            return send_code(sess_store[sesit].conn_sock, ROOMCROK);
        case 1:
            return send_code(sess_store[sesit].conn_sock, ROOMLFULL);
        case 2:
            return send_code(sess_store[sesit].conn_sock, ALREADYEXISTR);
        case 3:
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        }
    }
    else if (strcmp(cmd, "ROOML") == 0)
    {
        send_roomlist(sess_store[sesit].conn_sock, room_store, ROOM_NUM);
    }
    else if (strcmp(cmd, "ITEMADD") == 0)
    {
        char item_name[30];
        int stating_bid, direct_sell_price;
        if (sscanf(req, "ITEMADD %s %d %d", item_name, &stating_bid, &direct_sell_price) != 3)
        {
            printf("Agrument not enough\n");
            return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
        }
        printf("Item adding\n");
        switch (addItem(item_name, stating_bid, direct_sell_price, sesit))
        {

        case 0:
            printf("Item added\n");
            return send_code(sess_store[sesit].conn_sock, ADDITEMOK);
        case 1:
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        case 2:
            return send_code(sess_store[sesit].conn_sock, NOTINROOM);
        case 3:
            return send_code(sess_store[sesit].conn_sock, ALREADYEXISTITEM);
        default:
            printf("It's a bug\n");
            return send_code(sess_store[sesit].conn_sock, 300);
        }
    }

    else if (strcmp(cmd, "BID") == 0)
    {
        int bid;
        sscanf(req, "BID %d", &bid);
        switch (bidding(sesit, bid))
        {
        case 0:
            // return send_code(sess_store[sesit].conn_sock, BIDOK);
            return 1;
        case 1:
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        case 2:
            return send_code(sess_store[sesit].conn_sock, NOTINROOM);
        case 3:
            return send_code(sess_store[sesit].conn_sock, NOITEM);
        case 4:
            return send_code(sess_store[sesit].conn_sock, NOTHIGHER);
        default:
            printf("It's a bug\n");
            return send_code(sess_store[sesit].conn_sock, 300);
        }
    }

    else if (strcmp(cmd, "BUYNOW") == 0)
    {
        switch (buynow(sesit))
        {
        case 0:
            return 1;
        case 1:
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        case 2:
            return send_code(sess_store[sesit].conn_sock, NOTINROOM);
        case 3:
            return send_code(sess_store[sesit].conn_sock, NOITEM);
        default:
            printf("It's a bug\n");
            return send_code(sess_store[sesit].conn_sock, 300);
        }
    }
    else if (strcmp(cmd, "BYE") == 0)
    {
        return send_code(sess_store[sesit].conn_sock, 300);
    }
    else
        return send_code(sess_store[sesit].conn_sock, 300);

    return 0;
}