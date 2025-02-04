#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
    #include "platform/win32/Socket.h" // IWYU pragma: export
#else
    #include "platform/posix/Socket.h" // IWYU pragma: export
#endif

#endif //SOCKET_H