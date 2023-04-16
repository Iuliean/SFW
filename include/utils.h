#ifndef DEFS_H
#define DEFS_H
#include <iostream>
#include <assert.h>
#define ASSERT(C,M) if(!(C)){std::cerr << M << ':'<<__FILE__<< ':' << __LINE__<< '\n'; assert(C);}

namespace iu
{
    namespace utils
    {
        std::string getErrorFromErrno();
    }
}


#endif //DEFS_H