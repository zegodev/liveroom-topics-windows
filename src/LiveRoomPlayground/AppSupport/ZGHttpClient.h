

#ifndef ZGHttpClient_h__
#define ZGHttpClient_h__


#include <windows.h>
#include <wininet.h>

#include <map>
#include <vector>
#include <string>
#include <vector>

namespace ZEGO
{
    namespace APPSUPPORT
    {
        using std::string;
        using std::wstring;
        using std::vector;
        class HttpClient {
         public:
             HttpClient() {

             }
            bool SendHttpRequest(const string &url,
                int *timeout,
                string *response_body,
                int *response_code,
                string methord);

            bool SendHttpRequestW(const wstring &url,
                int *timeout,
                wstring *response_body,
                int *response_code,
                wstring methord);

         private:

              class AutoInternetHandle;

              bool ReadResponse(HINTERNET request, string* response);
              bool ReadResponseW(HINTERNET request, wstring* response);


        
        };

    }
}

#endif // ZGHttpClient_h__
