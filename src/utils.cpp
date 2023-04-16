#include "utils.h"
#include <string.h>
namespace iu
{
    namespace utils
    {
        std::string getErrorFromErrno()
        {
            char err[124];
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
            strerror_r(errno, err, 124);
            return {err, 124};
#else
            char* erp = nullptr;
            erp = strerror_r(errno, err, 124);
            return {erp};
#endif
        }
    }
}