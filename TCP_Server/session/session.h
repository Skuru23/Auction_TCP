#define BUFF_SIZE 1024

typedef struct session
{
    int login_state;
    char username[30];
    int in_room;
};
