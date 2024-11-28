#include "utils.h"
#include <string.h>
namespace iu
{
    namespace utils
    {
        std::string getErrorFromErrno()
        {
            char err[124];
            strerror_s(err, 124, errno);
            return err;
        }
    }
}