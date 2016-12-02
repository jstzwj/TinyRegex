#ifndef MESSAGE_H
#define MESSAGE_H
#include"base.h"
namespace tyre
{
    //message type
    enum MessageType
    {
        WARNING,ERROR,SUGGEST
    };

    //message
    class Message
    {
    public:
        Message();
        Message(const string_t &content, int row_=0, int col_=0, MessageType type_=SUGGEST);
        MessageType type;
        string_t msgContent;
        int row;
        int col;
    };
}


#endif // MESSAGE_H
