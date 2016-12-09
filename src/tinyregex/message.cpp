#include "message.h"
namespace tyre
{
    Message::Message()
    {

    }

    Message::Message(const string_t &content, int row_, int col_, MessageType type_)
        :type(type_),msgContent(content),row(row_),col(col_)
    {

    }
}

