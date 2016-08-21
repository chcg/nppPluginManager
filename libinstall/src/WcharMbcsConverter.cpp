/*
This file is part of Plugin Manager Plugin for Notepad++

Copyright (C)2009-2010 Dave Brotherstone <davegb@pobox.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Modified for inclusion in VS2010 project "Python Script"
*/
#include "precompiled_headers.h"
#include "libinstall/WcharMbcsConverter.h"

using namespace std;


boostOrStd::shared_ptr<wchar_t> WcharMbcsConverter::char2wchar(const char* mbStr)
{
	
	boostOrStd::shared_ptr<wchar_t> wideCharStr;

	int len = ::MultiByteToWideChar(CP_UTF8, 0, mbStr, -1, NULL, 0);
	
	
	if (len > 0)
	{
		wideCharStr.reset(new wchar_t[len]);
		MultiByteToWideChar(CP_UTF8, 0, mbStr, -1, wideCharStr.get(), len);
	}
	else
	{
		wideCharStr.reset(new wchar_t[1]);
		wideCharStr.get()[0] = 0;
	}

	return wideCharStr;
}


boostOrStd::shared_ptr<char> WcharMbcsConverter::wchar2char(const wchar_t* wcStr)
{

	boostOrStd::shared_ptr<char> multiByteStr;

	int len = WideCharToMultiByte(CP_UTF8, 0, wcStr, -1, NULL, 0, NULL, NULL);

	if (len > 0)
	{
		multiByteStr.reset(new char[len]);
		WideCharToMultiByte(CP_UTF8, 0, wcStr, -1, multiByteStr.get(), len, NULL, NULL);
	}
	else
	{
		multiByteStr.reset(new char[1]);
		multiByteStr.get()[0] = 0;
	}

	return multiByteStr;
}

	//static boostOrStd::shared_ptr<const TCHAR>   char2tchar(const char* mbStr);
	//static boostOrStd::shared_ptr<const char>    tchar2char(const TCHAR* tStr);
boostOrStd::shared_ptr<TCHAR> WcharMbcsConverter::char2tchar(const char* mbStr)
{
#ifdef _UNICODE
	return char2wchar(mbStr);
#else
	int len = strlen(mbStr) + 1;
	boostOrStd::shared_ptr<TCHAR> result(new TCHAR[len]);
	strcpy_s(result.get(), len, mbStr);
	return result;
#endif
}

boostOrStd::shared_ptr<char> WcharMbcsConverter::tchar2char(const TCHAR* tStr)
{
#ifdef _UNICODE
	return wchar2char(tStr);
#else
	int len = _tcslen(tStr) + 1;
	boostOrStd::shared_ptr<TCHAR> result(new TCHAR[len]);
	strcpy_s(result.get(), len, tStr);
	return result;
#endif
}


