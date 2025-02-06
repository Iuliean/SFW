#include "utils.h"
#include <string.h>
namespace iu
{
    namespace utils
    {
        std::string getErrorFromErrno()
        {
            char err[124];
            strerror_r(errno, err, 124);
            return err;
        }
    }
}