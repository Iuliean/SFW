#ifndef SOCKET_DESCRIPTOR_H
#define SOCKET_DESCRIPTOR_H

#ifdef _WIN32
    #include "platform/win32/SocketDescriptor.h" // IWYU pragma: export
#else
    #include "platform/posix/SocketDescriptor.h" // IWYU pragma: export
#endif

#endif //SOCKET_DESCRIPTOR_H