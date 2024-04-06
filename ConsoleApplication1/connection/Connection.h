#pragma once

#include <json.h>

#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Normaliz.lib")

#define CURL_STATICLIB 
#include <curl/curl.h>

//#include "../driver/driver.h"




inline CURL* curl{ NULL };
inline CURLcode res{ CURLE_UNKNOWN_OPTION };
inline std::string readBuffer;


static size_t my_write(void* buffer, size_t size, size_t nmemb, void* param)
{
    std::string& text = *static_cast<std::string*>(param);
    size_t totalsize = size * nmemb;
    text.append(static_cast<char*>(buffer), totalsize);
    return totalsize;
}

inline Json::Value json_decode(const std::string& jsonText) {
    Json::Value root;
    Json::Reader reader;
    bool parsinsSuccessful = reader.parse(jsonText, root, true);
    if (!parsinsSuccessful) {
        //std::cout << "Fail parsing!!" << std::endl;
        //MessageBox(0, "Fail parsing!!", 0, 0);
        //error code here
    }

    return root;
}

namespace Connection {
    //inline _driver* dd = &driver;
	inline std::wstring documentFolder;
	inline std::wstring folder;
	inline std::wstring data;

    inline char user[128];
    inline char pass[128];


    inline bool bLogged{ false };


    inline Json::Value loaderJson;

    inline std::string hwid{ "0" };
    inline std::string hash;
}



namespace Connection
{
    bool internet();
    void DelMe();

    void getHwid();
    std::string GetRegKeyValue(HKEY HKEY, const LPCWSTR lpValue, const LPCWSTR sub);
    std::string getGuid();
    std::string delSpaces(std::string& str);

    void encrypt(std::string& input);
    void decrypt(std::string& encryptedText);


    void MD5();
	bool putAddressInJson(void* ADDRESS_OFFSETS, void* STRUCT_OFFSET);
    bool Login();
    void Ban();

}