
#include "stdafx.h"
#include "ZGHttpClient.h"
#include "ZGUtilTools.h"
#include "ZGHelper.h"

namespace ZEGO
{
    namespace APPSUPPORT
    {

        class HttpClient::AutoInternetHandle {
         public:
          explicit AutoInternetHandle(HINTERNET handle) : handle_(handle) {}
          ~AutoInternetHandle() {
            if (handle_) {
              InternetCloseHandle(handle_);
            }
          }

          HINTERNET get() { return handle_; }

         private:
          HINTERNET handle_;
        };


        bool HttpClient::SendHttpRequest(const string &url, int *timeout, string *response_body, int *response_code, string methord)
        {
            if (response_code) {
                *response_code = 0;
            }

            // Break up the URL and make sure we can handle it
            char scheme[16] = {0}, host[256] = {0}, path[256] = {0};
            URL_COMPONENTSA components;
            memset(&components, 0, sizeof(components));
            components.dwStructSize = sizeof(components);
            components.lpszScheme = scheme;
            components.dwSchemeLength = sizeof(scheme) / sizeof(scheme[0]);
            components.lpszHostName = host;
            components.dwHostNameLength = sizeof(host) / sizeof(host[0]);
            components.lpszUrlPath = path;
            components.dwUrlPathLength = sizeof(path) / sizeof(path[0]);
            if (!InternetCrackUrlA(url.c_str(), static_cast<DWORD>(url.size()),
                0, &components)) {
                return false;
            }
            bool secure = false;
            if (strcmp(scheme, "https") == 0) {
                secure = true;
            }
            else if (strcmp(scheme, "http") != 0) {
                return false;
            }

            char user_agent[50] = {0};

            sprintf_s(user_agent, "ZGHttpClient / 1.0 (Windows) %lld", ZGHelperInstance()->GetCurTimeStampMs());

            AutoInternetHandle internet(InternetOpenA(user_agent,
                INTERNET_OPEN_TYPE_PRECONFIG,
                NULL,  // proxy name
                NULL,  // proxy bypass
                0));   // flags
            if (!internet.get()) {
                return false;
            }

            AutoInternetHandle connection(InternetConnectA(internet.get(),
                host,
                components.nPort,
                NULL,    // user name
                NULL,    // password
                INTERNET_SERVICE_HTTP,
                0,       // flags
                NULL));  // context
            if (!connection.get()) {
                return false;
            }

            DWORD http_open_flags = secure ? INTERNET_FLAG_SECURE : 0;
            http_open_flags |= INTERNET_FLAG_NO_COOKIES;
            http_open_flags |= INTERNET_FLAG_RELOAD;
            http_open_flags |= INTERNET_FLAG_DONT_CACHE;

            AutoInternetHandle request(HttpOpenRequestA(connection.get(),
                methord.c_str(),
                path,
                NULL,    // version
                NULL,    // referer
                NULL,    // agent type
                http_open_flags,
                NULL));  // context
            if (!request.get()) {
                return false;
            }

//             string content_type_header = GenerateRequestHeader();
//             HttpAddRequestHeadersA(request.get(),
//                 content_type_header.c_str(),
//                 static_cast<DWORD>(-1),
//                 HTTP_ADDREQ_FLAG_ADD);


            if (timeout) {
                if (!InternetSetOptionA(request.get(),
                    INTERNET_OPTION_SEND_TIMEOUT,
                    timeout,
                    sizeof(*timeout))) {
                    fwprintf(stderr, L"Could not unset send timeout, continuing...\n");
                }

                if (!InternetSetOptionA(request.get(),
                    INTERNET_OPTION_RECEIVE_TIMEOUT,
                    timeout,
                    sizeof(*timeout))) {
                    fwprintf(stderr, L"Could not unset receive timeout, continuing...\n");
                }
            }

            if (!HttpSendRequestA(request.get(), NULL, 0, nullptr, 0)) {
                return false;
            }

            // The server indicates a successful upload with HTTP status 200.
            char http_status[4];
            DWORD http_status_size = sizeof(http_status);
            if (!HttpQueryInfoA(request.get(), HTTP_QUERY_STATUS_CODE,
                static_cast<LPVOID>(&http_status), &http_status_size,
                0)) {
                return false;
            }

            int http_response = strtol(http_status, NULL, 10);
            if (response_code) {
                *response_code = http_response;
            }

            bool result_200 = (http_response == 200);

            bool read_response_result = ReadResponse(request.get(), response_body);
            
            return result_200 && read_response_result;
        }


        bool HttpClient::SendHttpRequestW(const wstring &url, int *timeout, wstring *response_body, int *response_code, wstring methord)
        {
            if (response_code) {
                *response_code = 0;
            }

            // Break up the URL and make sure we can handle it
            wchar_t scheme[16] = { 0 }, host[256] = { 0 }, path[256] = { 0 };
            URL_COMPONENTS components;
            memset(&components, 0, sizeof(components));
            components.dwStructSize = sizeof(components);
            components.lpszScheme = scheme;
            components.dwSchemeLength = sizeof(scheme) / sizeof(scheme[0]);
            components.lpszHostName = host;
            components.dwHostNameLength = sizeof(host) / sizeof(host[0]);
            components.lpszUrlPath = path;
            components.dwUrlPathLength = sizeof(path) / sizeof(path[0]);
            if (!InternetCrackUrl(url.c_str(), static_cast<DWORD>(url.size()),
                0, &components)) {
                return false;
            }
            bool secure = false;
            if (wcscmp(scheme, L"https") == 0) {
                secure = true;
            }
            else if (wcscmp(scheme, L"http") != 0) {
                return false;
            }

            const wchar_t kUserAgent[] = L"ZegoUploader/1.0 (Windows)";

            AutoInternetHandle internet(InternetOpen(kUserAgent,
                INTERNET_OPEN_TYPE_PRECONFIG,
                NULL,  // proxy name
                NULL,  // proxy bypass
                0));   // flags
            if (!internet.get()) {
                return false;
            }

            AutoInternetHandle connection(InternetConnect(internet.get(),
                host,
                components.nPort,
                NULL,    // user name
                NULL,    // password
                INTERNET_SERVICE_HTTP,
                0,       // flags
                NULL));  // context
            if (!connection.get()) {
                return false;
            }

            DWORD http_open_flags = secure ? INTERNET_FLAG_SECURE : 0;
            http_open_flags |= INTERNET_FLAG_NO_COOKIES;
            http_open_flags |= INTERNET_FLAG_RELOAD;
            http_open_flags |= INTERNET_FLAG_DONT_CACHE;
            AutoInternetHandle request(HttpOpenRequest(connection.get(),
                methord.c_str(),
                path,
                NULL,    // version
                NULL,    // referer
                NULL,    // agent type
                http_open_flags,
                NULL));  // context
            if (!request.get()) {
                return false;
            }
            if (timeout) {
                if (!InternetSetOption(request.get(),
                    INTERNET_OPTION_SEND_TIMEOUT,
                    timeout,
                    sizeof(*timeout))) {
                    fwprintf(stderr, L"Could not unset send timeout, continuing...\n");
                }

                if (!InternetSetOption(request.get(),
                    INTERNET_OPTION_RECEIVE_TIMEOUT,
                    timeout,
                    sizeof(*timeout))) {
                    fwprintf(stderr, L"Could not unset receive timeout, continuing...\n");
                }
            }

            if (!HttpSendRequest(request.get(), NULL, 0, nullptr, 0)) {
                return false;
            }

            // The server indicates a successful upload with HTTP status 200.
            char http_status[4];
            DWORD http_status_size = sizeof(http_status);
            if (!HttpQueryInfo(request.get(), HTTP_QUERY_STATUS_CODE,
                static_cast<LPVOID>(&http_status), &http_status_size,
                0)) {
                return false;
            }

            int http_response = strtol(http_status, NULL, 10);
            if (response_code) {
                *response_code = http_response;
            }

            bool result_200 = (http_response == 200);

            wstring wstr_response;
            bool read_response_result = ReadResponseW(request.get(), &wstr_response);

            return result_200 && read_response_result;
        }

        // static
        bool HttpClient::ReadResponse(HINTERNET request, string *response) {
          bool has_content_length_header = false;
          char content_length[32];
          DWORD content_length_size = sizeof(content_length);
          DWORD claimed_size = 0;
          string response_body;

          if (HttpQueryInfoA(request, HTTP_QUERY_CONTENT_LENGTH,
                            static_cast<LPVOID>(&content_length),
                            &content_length_size, 0)) {
            has_content_length_header = true;
            claimed_size = strtol(content_length, NULL, 10);
            response_body.reserve(claimed_size);
          }


          DWORD bytes_available;
          DWORD total_read = 0;
          BOOL return_code;

          while (((return_code = InternetQueryDataAvailable(request, &bytes_available,
              0, 0)) != 0) && bytes_available > 0) {
            vector<char> response_buffer(bytes_available);
            DWORD size_read;

            return_code = InternetReadFile(request,
                                           &response_buffer[0],
                                           bytes_available, &size_read);

            if (return_code && size_read > 0) {
              total_read += size_read;
              response_body.append(&response_buffer[0], size_read);
            } else {
              break;
            }
          }

          bool succeeded = return_code && (!has_content_length_header ||
                                           (total_read == claimed_size));
          if (succeeded && response) {
              *response = UTF8ToGBK(response_body.c_str());
          }

          return succeeded;
        }

        
        wstring UTF8ToWide(const string &utf8) {
            if (utf8.length() == 0) {
                return wstring();
            }

            // compute the length of the buffer we'll need
            int charcount = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);

            if (charcount == 0) {
                return wstring();
            }

            // convert
            wchar_t* buf = new wchar_t[charcount];
            MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, buf, charcount);
            wstring result(buf);
            delete[] buf;
            return result;
        }


        bool HttpClient::ReadResponseW(HINTERNET request, wstring* response)
        {
            bool has_content_length_header = false;
            wchar_t content_length[32];
            DWORD content_length_size = sizeof(content_length);
            DWORD claimed_size = 0;
            string response_body;

            if (HttpQueryInfo(request, HTTP_QUERY_CONTENT_LENGTH,
                static_cast<LPVOID>(&content_length),
                &content_length_size, 0)) {
                has_content_length_header = true;
                claimed_size = wcstol(content_length, NULL, 10);
                response_body.reserve(claimed_size);
            }


            DWORD bytes_available;
            DWORD total_read = 0;
            BOOL return_code;

            while (((return_code = InternetQueryDataAvailable(request, &bytes_available,
                0, 0)) != 0) && bytes_available > 0) {
                vector<char> response_buffer(bytes_available);
                DWORD size_read;

                return_code = InternetReadFile(request,
                    &response_buffer[0],
                    bytes_available, &size_read);

                if (return_code && size_read > 0) {
                    total_read += size_read;
                    response_body.append(&response_buffer[0], size_read);
                }
                else {
                    break;
                }
            }

            bool succeeded = return_code && (!has_content_length_header ||
                (total_read == claimed_size));
            if (succeeded && response) {
                *response = UTF8ToWide(response_body.c_str());
            }

            return succeeded;
        }


}
}
