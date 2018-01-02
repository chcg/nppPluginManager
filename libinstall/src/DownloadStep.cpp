#include "precompiled_headers.h"
#include "libinstall/DownloadStep.h"
#include "libinstall/DownloadManager.h"
#include "libinstall/Decompress.h"
#include "libinstall/tstring.h"
#include "libinstall/InstallStep.h"
#include "libinstall/WcharMbcsConverter.h"
#include "libinstall/DirectLinkSearch.h"
#include "libinstall/ProxyInfo.h"
#include "libinstall/ModuleInfo.h"

using namespace std;

DownloadStep::DownloadStep(const TCHAR *url, const TCHAR *filename)
{
    _url = url;

    if (filename)
        _filename = filename;
}

StepStatus DownloadStep::perform(tstring &basePath, TiXmlElement* forGpup,
                                 std::function<void(const TCHAR*)> setStatus,
                                 std::function<void(const int)> stepProgress,
                                 const ModuleInfo* moduleInfo,
                                 CancelToken &cancelToken)
{
    DownloadManager downloadManager(cancelToken);



    tstring downloadFilename;

    if (_filename.empty())
    {
        TCHAR tDownloadFilename[MAX_PATH];
        ::GetTempFileName(basePath.c_str(), _T("download"), 0, tDownloadFilename);
        downloadFilename = tDownloadFilename;
    }
    else
    {
        TCHAR tDownloadPath[MAX_PATH];
        PathCombine(tDownloadPath, basePath.c_str(), _filename.c_str());
        downloadFilename = tDownloadPath;
    }

    // Set the status
    tstring status = _T("Downloading ");
    status.append(_url);
    setStatus(status.c_str());

    // Link up the progress callback
    downloadManager.setProgressFunction(stepProgress);

	StepStatus stepStatusReturnValue = downloadHelper(_url, downloadManager, downloadFilename, basePath, forGpup, setStatus, stepProgress, moduleInfo, cancelToken);

	if (stepStatusReturnValue != STEPSTATUS_FAIL)
	{
		return stepStatusReturnValue;
	}
	else if(_url.find(_T("https://")) != std::string::npos)
	{
		//fallback to http requests for WinXP, due to missing TLS 1.2 support 
		tstring url_as_http = _url;
		url_as_http.replace(url_as_http.begin(), (url_as_http.begin()+5), _T("http"));
		stepStatusReturnValue = downloadHelper(url_as_http, downloadManager, downloadFilename, basePath, forGpup, setStatus, stepProgress, moduleInfo, cancelToken);

		if (stepStatusReturnValue != STEPSTATUS_FAIL)
		{
			return stepStatusReturnValue;
		}
	}

    return STEPSTATUS_FAIL;
}

StepStatus DownloadStep::downloadHelper(tstring & downloadURL, DownloadManager & downloadManager, tstring & downloadFilename, tstring & basePath, TiXmlElement * forGpup, std::function<void(const TCHAR*)>& setStatus, std::function<void(int)>& stepProgress, const ModuleInfo * moduleInfo, CancelToken & cancelToken)
{
	tstring contentType;

	if (downloadManager.getUrl(downloadURL.c_str(), downloadFilename, contentType, moduleInfo))
	{
		if (contentType == _T("text/html"))
		{
			DirectLinkSearch linkSearch(downloadFilename.c_str());
			std::shared_ptr<TCHAR> realLink;
			if (!_filename.empty())
			{
				realLink = linkSearch.search(_filename.c_str());
			}
			else
			{
				tstring::size_type lastQuestionMark = downloadURL.find_last_of(_T('?'));
				tstring::size_type lastSlash = downloadURL.find_last_of(_T('/'));
				if (lastSlash != tstring::npos)
				{
					lastSlash++;
				}

				tstring::size_type length = lastQuestionMark;
				if (length != tstring::npos)
				{
					length = length - lastSlash;
				}


				tstring filenameInUrl = downloadURL.substr(lastSlash, length);
				realLink = linkSearch.search(filenameInUrl.c_str());
			}

			if (realLink.get())
			{
				downloadURL = realLink.get();
				return perform(basePath, forGpup, setStatus, stepProgress, moduleInfo, cancelToken);
			}
			else
				return STEPSTATUS_FAIL;
		}
		else {
			// Attempt to unzip file into basePath
			// Assume it is a zip file - if unzipping fails, then check if the filename is filled in
			// - if it is, then just leave the file as it is (ie. direct download)
			//   the file will be available for copying or installing.
			if (Decompress::unzip(downloadFilename, basePath) || !_filename.empty())
			{
				return STEPSTATUS_SUCCESS;
			}
		}
	}

	return STEPSTATUS_FAIL;
}
