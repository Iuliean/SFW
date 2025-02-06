#include "utils.h"
#include <string.h>

namespace iu
{
    namespace utils
    {
        std::string getErrorFromErrno()
        {
            char err[124];
//for now it can stay like this but this file may be a system dependant file
#ifdef _WIN32
            strerror_s(err, 124);
#else
            strerror_r(errno, err, 124);
#endif
            return err;
        }
    }
}