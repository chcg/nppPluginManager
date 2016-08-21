


#include <tchar.h>
#include <string>
#include <map>
#include <list>
#include <memory>
#include <set>
#include <sstream>



#include <windows.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <process.h>
#include <strsafe.h>

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
using namespace std::placeholders;
#endif

typedef std::basic_string<TCHAR>			tstring;