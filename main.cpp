#include "WebClient.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------------------------------------------------
void Usage(const string & execName)
{
    cout << "Usage: " << execName << " <URL>" << endl; 
}


//-------------------------------------------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        Usage(argv[0]);
        return 0;
    }

    WebClient http;
    http.SetURL(argv[1]);
    http.AddHeader("Content-Type", "text/xml");
    http.AddHeader("Accept-Charset", "utf-8");
    http.AddHeader("Custom-Saket", "Hey Sharad!");
    HTTPResponse response = http.Send();
    response.DisplayHTTPResponse();

    return 0;
}
