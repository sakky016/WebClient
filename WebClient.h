#ifndef HTTPCHANNEL_H
#define HTTPCHANNEL_H

#include <curl/curl.h>
#include <string>
#include <unordered_map>
#include <utility>
#include "HTTPResponse.h"

class WebClient
{
public:
   WebClient();
   ~WebClient();

   // Add a header to be passed to the HTTP request
   void AddHeader(const std::string & key, const std::string & value);

   // Specify the URL for the HTTP request
   void SetURL(const std::string & url) { m_url = url; }

   // Sends an HTTP request
   // @returns  : HTTP Response 
   HTTPResponse Send();

private:
    // Prepares and adds the headers provided by client to the HTTP request
    void AddHeadersToCurlHandle();

private:
    std::unordered_map<std::string, std::string> m_headers;
    std::string m_url;
    struct curl_slist *m_headerList;
    CURL* m_curlHandle;
};
#endif
