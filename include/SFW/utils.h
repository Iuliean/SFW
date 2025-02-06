#ifndef DEFS_H
#define DEFS_H
#include <assert.h>
#include <iostream>
#define ASSERT(C,M) if(!(C)){std::cerr << M << ':'<<__FILE__<< ':' << __LINE__<< '\n'; assert(C);}

#ifdef _WIN32
    #ifdef SFW_EXPORT
        #define SFW_API __declspec(dllexport)
    #endif
    #ifdef SFW_IMPORT
        #define SFW_API __declspec(dllimport)
    #endif
#else
    #define SFW_API 
#endif

namespace iu
{
    namespace utils
    {
        std::string getErrorFromErrno();
    }
}


#endif //DEFS_H