#ifndef SOCKET_DESCRIPTOR_H
#define SOCKET_DESCRIPTOR_H

#ifdef _WIN32
    #include "platform/win32/SocketDescriptor.h"
#else
    #include "platform/posix/SocketDescriptor.h"
#endif

#endif //SOCKET_DESCRIPTOR_H