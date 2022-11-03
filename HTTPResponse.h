#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <unordered_map>

class HTTPResponse
{
public:
   HTTPResponse(const std::string & responseBuffer, const std::string & bodyBuffer);
   ~HTTPResponse();
    bool DisplayHTTPResponse();

private:
    bool ParseHeaderBuffer(const std::string &headerBuffer);

private:
   std::string m_protocol;
   std::string m_responseCode;
   std::string m_responsePhrase;
   std::unordered_map<std::string, std::string> m_fields;
   std::string m_body;
};
#endif
