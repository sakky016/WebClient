#include "HTTPResponse.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

//-------------------------------------------------------------------------------------------------------------------
inline std::string& rtrim(string& input, const string& trimChars)
{
    input.erase(input.find_last_not_of(trimChars) + 1);
    return input;
}


//-------------------------------------------------------------------------------------------------------------------
inline std::string& ltrim(string& input, const string& trimChars)
{
    input.erase(0, input.find_first_not_of(trimChars));
    return input;
}


//-------------------------------------------------------------------------------------------------------------------
inline std::string& trim(string& input, const string& trimChars)
{
    return ltrim(rtrim(input, trimChars), trimChars);
}


//-------------------------------------------------------------------------------------------------------------------
HTTPResponse::HTTPResponse(const string & headerBuffer, const string & bodyBuffer)
{
    m_body = bodyBuffer;

    ParseHeaderBuffer(headerBuffer);
}


//-------------------------------------------------------------------------------------------------------------------
HTTPResponse::~HTTPResponse()
{

}


//-------------------------------------------------------------------------------------------------------------------
bool HTTPResponse::ParseHeaderBuffer(const string &headerBuffer)
{
    size_t lineIndex = 0;
    string line;
    stringstream headerStream(headerBuffer);
    while (getline(headerStream, line))
    {
        trim(line, "\n\r ");

        if (!line.empty())
        {

            if (lineIndex == 0)
            {
                // 1st line in the header has Protocol and Response code
                size_t spacePos1 = line.find(" ");
                if (spacePos1 != string::npos)
                {
                    m_protocol = line.substr(0, spacePos1);
                    string reason = line.substr(spacePos1 + 1, line.size() - spacePos1 + 1);
                    size_t spacePos2 = reason.find(" ");
                    if (spacePos2 != string::npos)
                    {
                        m_responseCode = reason.substr(0, spacePos2);
                        m_responsePhrase = reason.substr(spacePos2 + 1, reason.size() - spacePos2 + 1);
                    }
                }
            }
            else
            {
                // Headers
                size_t colonPos = line.find(":");
                if (colonPos != string::npos)
                {
                    string headerKey = line.substr(0, colonPos);
                    string headerValue = line.substr(colonPos + 1, line.size() - colonPos + 1);
                    trim(headerKey, " ");
                    trim(headerValue, " ");
                    m_fields[headerKey] = headerValue;
                }
            }
        }

        lineIndex++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
bool HTTPResponse::DisplayHTTPResponse()
{
    cout << "-----------------------------------------------------------------\n";
    cout << "HTTP Response Header:\n";
    cout << "-----------------------------------------------------------------\n";

    cout << setw(30) << "Protocol" << ": " << m_protocol << endl;
    cout << setw(30) << "Response code" << ": " << m_responseCode << endl;
    cout << setw(30) << "Response phrase" << ": " << m_responsePhrase << endl;
    for (const auto & entry : m_fields)
    {
        cout << setw(30) << entry.first << ": " << entry.second << endl;
    }
    
    cout << "-----------------------------------------------------------------\n";
    cout << "HTTP Response Body:\n";
    cout << "-----------------------------------------------------------------\n";
    cout << m_body << endl;
    cout << "-----------------------------------------------------------------\n";

}
