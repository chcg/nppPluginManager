
#include <memory>
#include <string>
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <limits.h>
#include <map>
#include <set>
#include <list>

#include <shlwapi.h>
#include <commctrl.h>
#include <process.h>




#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WindowsX.h>
#include <WinInet.h>

#include <strsafe.h>
typedef std::basic_string<TCHAR>			tstring;

#if USE_BOOST


#pragma warning (push)
#pragma warning (disable : 4512) // assignment operator could not be generated
#include <boost/bind.hpp>
#pragma warning (pop)

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
namespace boostOrStd = boost;
#else

#include <functional>   // std::bind and std::function
//#include <memory> //std::shared_ptr already included above
namespace boostOrStd = std;
#endif

#include <sstream>

#ifdef UNICODE
#define tostringstream std::wostringstream
#else
#define tostringstream std::ostringstream
#endif

#define FMTDBGSTR(stream)  ((tostringstream&)(tostringstream() << tstring() << stream)).str().c_str() 
