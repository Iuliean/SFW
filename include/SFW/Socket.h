#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
    #include "platform/win32/Socket.h"
#else
    #include "platform/posix/Socket.h"
#endif

#endif //SOCKET_H