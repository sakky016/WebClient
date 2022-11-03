#include "WebClient.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------------------------------------------------
// libcurl usage reference:
// https://curl.se/libcurl/c/libcurl-tutorial.html
//-------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------
size_t WriteCallbackFn(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//-------------------------------------------------------------------------------------------------------------------
size_t HeaderCallbackFn(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


//-------------------------------------------------------------------------------------------------------------------
WebClient::WebClient()
{
    curl_global_init(CURL_GLOBAL_ALL);

    m_curlHandle = curl_easy_init();
    if (m_curlHandle == nullptr)
    {
        cout << "Failed to initialize curl!" << endl;
    }

    m_headerList = nullptr;
}


//-------------------------------------------------------------------------------------------------------------------
WebClient::~WebClient()
{
    curl_easy_cleanup(m_curlHandle);
}


//-------------------------------------------------------------------------------------------------------------------
void WebClient::AddHeader(const string & key, const string & value)
{
    m_headers[key] = value;
}


//-------------------------------------------------------------------------------------------------------------------
void WebClient::AddHeadersToCurlHandle()
{
    if (m_headers.empty())
    {
        return;
    }

    for (const auto & entry : m_headers)
    {
        string headerString = entry.first + ": " + entry.second;
        m_headerList = curl_slist_append(m_headerList, headerString.c_str());
        if (m_headerList == nullptr)
        {
            cout << "ERROR: Failed to add " << headerString.c_str() << endl;
        }
    }

    curl_easy_setopt(m_curlHandle, CURLOPT_HTTPHEADER, m_headerList);
}


//-------------------------------------------------------------------------------------------------------------------
HTTPResponse WebClient::Send()
{
    string bodyBuffer;
    string headerBuffer;

    // Set curl options
    curl_easy_setopt(m_curlHandle, CURLOPT_URL, m_url.c_str());
    //curl_easy_setopt(m_curlHandle, CURLOPT_VERBOSE, 1L);
    //curl_easy_setopt(m_curlHandle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(m_curlHandle, CURLOPT_WRITEFUNCTION, WriteCallbackFn);
    curl_easy_setopt(m_curlHandle, CURLOPT_WRITEDATA, &bodyBuffer);
    curl_easy_setopt(m_curlHandle, CURLOPT_HEADERFUNCTION, WriteCallbackFn);
    curl_easy_setopt(m_curlHandle, CURLOPT_HEADERDATA, &headerBuffer);

    AddHeadersToCurlHandle();

    // Send the HTTP request
    curl_easy_perform(m_curlHandle);

    long responseCode = 0;
    curl_easy_getinfo(m_curlHandle, CURLINFO_RESPONSE_CODE, &responseCode);

    // Free header linked list that was used by AddHeaders()
    curl_slist_free_all(m_headerList);

    HTTPResponse response(headerBuffer, bodyBuffer);
    return response;
}
