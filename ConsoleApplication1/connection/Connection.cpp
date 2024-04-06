


#include <iostream>
#include <string>
#include <json.h>
#include <fstream>

#include <shlobj.h>

#include "Connection.h"

#include <filesystem>

#include <codecvt>
#include "../Protection/xorstr.hpp"



#define BUFSIZE 1024
#define MD5LEN  16





#include <wininet.h>
#include <windows.h>
#include <psapi.h>


#pragma comment(lib, "Wininet.lib")
#pragma comment(lib,"Urlmon.lib")




bool Connection::internet() {
    if (InternetCheckConnectionA(xorstr_("https://www.google.com/"), FLAG_ICC_FORCE_CONNECTION, 0))
        return true;
    else {
        DelMe();
        ExitProcess(0);
        return false;
    }
}
#include <strsafe.h>
#include "../Protection/antiDebug/antiDebug.h"

#define SELF_REMOVE_STRING  TEXT(xorstr_("cmd.exe /C ping 1.1.1.1 -n 3 -w 3000 > Nul & Del /f /q \"%s\""))

void Connection::DelMe()
{
    TCHAR szModuleName[MAX_PATH];
    TCHAR szCmd[2 * MAX_PATH];
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    GetModuleFileName(NULL, szModuleName, MAX_PATH);

    StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

    CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

void Connection::MD5()
{
    HMODULE hModule = GetModuleHandle(NULL);
    DWORD dwStatus = 0;
    BOOL bResult = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = NULL;
    BYTE rgbFile[BUFSIZE];
    DWORD cbRead = 0;
    BYTE rgbHash[MD5LEN];
    DWORD cbHash = 0;
    CHAR rgbDigits[] = "0123456789ABCDEF";
    char localown[MAX_PATH];
    GetModuleFileNameA(hModule, localown, sizeof(localown));

    hFile = CreateFileA(localown,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        exit(0);
    }

    if (!CryptAcquireContext(&hProv,
        NULL,
        NULL,
        PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT))
    {
        CloseHandle(hFile);
        exit(0);
    }

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        CloseHandle(hFile);
        CryptReleaseContext(hProv, 0);
        exit(0);
    }

    while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
        &cbRead, NULL))
    {
        if (0 == cbRead)
        {
            break;
        }

        if (!CryptHashData(hHash, rgbFile, cbRead, 0))
        {
            CryptReleaseContext(hProv, 0);
            CryptDestroyHash(hHash);
            CloseHandle(hFile);
            exit(0);
        }
    }

    if (!bResult)
    {
        dwStatus = GetLastError();
        CryptReleaseContext(hProv, 0);
        CryptDestroyHash(hHash);
        CloseHandle(hFile);
        exit(0);
    }

    std::string c;

    cbHash = MD5LEN;
    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
        for (DWORD i = 0; i < cbHash; i++)
        {
            c += rgbDigits[rgbHash[i] >> 4];
            c += rgbDigits[rgbHash[i] & 0xf];
        }
    }
    else
    {
        dwStatus = GetLastError();
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);

    Connection::hash = c;
}

std::string Connection::GetRegKeyValue(HKEY hKey, const LPCWSTR lpValue, const LPCWSTR sub)
{
    size_t bufferSize = 0xFFF; // If too small, will be resized down below.
    std::wstring valueBuf; // Contiguous buffer since C++11.
    valueBuf.resize(bufferSize);
    auto cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
    auto regValue = RegGetValueW(hKey, lpValue, sub, RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);

    while (regValue == ERROR_MORE_DATA)
    {
        // Get a buffer that is big enough.
        cbData /= sizeof(wchar_t);
        if (cbData > static_cast<DWORD>(bufferSize))
        {
            bufferSize = static_cast<size_t>(cbData);
        }
        else
        {
            bufferSize *= 2;
            cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
        }
        valueBuf.resize(bufferSize);
        regValue = RegGetValueW(hKey, lpValue, sub, RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);

    }
    if (regValue == ERROR_SUCCESS)
    {
        cbData /= sizeof(wchar_t);
        valueBuf.resize(static_cast<size_t>(cbData - 1)); // remove end null character

        std::string reg{ valueBuf.begin(), valueBuf.end() };
        return reg;
    }
    return "0";
}

std::string Connection::getGuid() {
    std::string hardware_guid;

    HW_PROFILE_INFOA hwProfileInfo;
    GetCurrentHwProfileA(&hwProfileInfo);
    return hwProfileInfo.szHwProfileGuid;
}

std::string Connection::delSpaces(std::string& str)
{
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

void Connection::getHwid() {
    std::string Crypytography = GetRegKeyValue(HKEY_LOCAL_MACHINE, xorstr_(L"SOFTWARE\\Microsoft\\Cryptography"), xorstr_(L"MachineGuid"));
    std::string Guid = GetRegKeyValue(HKEY_LOCAL_MACHINE, xorstr_(L"SOFTWARE\\Microsoft\\Cryptography"), xorstr_(L"GUID"));
    std::string lastConfig = GetRegKeyValue(HKEY_LOCAL_MACHINE, xorstr_(L"SYSTEM\\HardwareConfig"), xorstr_(L"LastConfig"));

    std::string guid = Connection::getGuid();

    size_t size{ strlen(Guid.c_str()) };

    size = strlen(lastConfig.c_str());

    lastConfig.erase(0, 1);
    lastConfig.erase(size - 2, 1);

    size = strlen(guid.c_str());
    guid.erase(0, 1);
    guid.erase(size - 2, 1);


    std::stringstream temp;

    temp << delSpaces(guid) << delSpaces(Crypytography) << delSpaces(guid) << delSpaces(lastConfig);

    Connection::loaderJson[xorstr_("Hwid")] = hwid = temp.str();

    Debug::checkALL();
}

void Connection::encrypt(std::string & input) {
    for (char& ch : input) {
        ch = static_cast<char>(ch + 101);
    }
}

void Connection::decrypt(std::string& encryptedText) {
    for (char& ch : encryptedText) {
        ch = static_cast<char>(ch - 101);
    }
}

bool Connection::putAddressInJson(void* ADDRESS_OFFSETS, void* STRUCT_OFFSET) {
    Debug::checkALL();
    wchar_t* documentsFolder;

    if (SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documentsFolder) != S_OK) {
        //std::cout << "problem getting appdata folder" << std::endl;
        //gui::error = "Problem to get folder";
        exit(0);
    }

    Connection::documentFolder = documentsFolder;
    Connection::folder = Connection::documentFolder + xorstr_(L"\\radiation_project");
    Connection::data = Connection::folder + xorstr_(L"\\data.dat");

	std::string fileText;

    std::ifstream file_json(Connection::data);
    if (!file_json) {
        exit(0);
    }
    
    fileText.assign(std::istreambuf_iterator<char>(file_json), std::istreambuf_iterator<char>());

    file_json.close();

    if (fileText == "")
    {
        exit(0);
    }

    Connection::decrypt(fileText);

    Json::Value root = json_decode(fileText);

    strcpy_s( Connection::user, root[xorstr_("user")].asCString());
    strcpy_s( Connection::pass, root[xorstr_("pass")].asCString());

    std::string encryptedText{ root.toStyledString() };

    Connection::encrypt(encryptedText);

    std::ofstream data(Connection::data);
    data << encryptedText;
    data.close();

    if (root[xorstr_("user")].empty() || root[xorstr_("pass")].empty()) {
        ExitProcess(0);
    }

    Connection::getHwid();
    Connection::MD5();

    Connection::loaderJson[xorstr_("User")] = Connection::user;
    Connection::loaderJson[xorstr_("Password")] = Connection::pass;
    Connection::loaderJson[xorstr_("Hwid")] = Connection::hwid;
    Connection::loaderJson[xorstr_("LoaderHash")] = Connection::hash;


    Connection::loaderJson[xorstr_("application")] = xorstr_("Cheat");
#if defined(BOT)
#if defined(FULL)
    Connection::loaderJson[xorstr_("applicationName")] = xorstr_("RavendawnFull");
#else
    Connection::loaderJson[xorstr_("applicationName")] = xorstr_("RavendawnBot");
#endif
#elif defined(MULT)
    Connection::loaderJson[xorstr_("applicationName")] = xorstr_("RavendawnMult");
#endif

#if defined ( DEBUG ) || defined ( TESTE ) || defined ( RELEASE_DEBUG )// DEBUG
    Connection::loaderJson[xorstr_("LoaderStatus")] = xorstr_("Debug");
#elif RELEASE // RELEASE
    Connection::loaderJson[xorstr_("LoaderStatus")] = xorstr_("Release");
#endif  

    if (Connection::Login()) {

        return true;
    }
    else
    {
        ExitProcess(0);

        return false;
    }
}

bool Connection::Login() {
    Debug::checkALL();

    curl = curl_easy_init();

    std::string body{ Connection::loaderJson.toStyledString() };
    if (curl) {
        struct curl_slist* slist1;
        slist1 = NULL;
        slist1 = curl_slist_append(slist1, xorstr_("Content-Type: application/json"));


        curl_easy_setopt(curl, CURLOPT_URL, xorstr_("https://radiationproject.cc/login.php"));

        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        Json::Value json;
        json = json_decode(readBuffer);

        if (json[xorstr_("Status")] == xorstr_("Password not macth")) {
            ExitProcess(0);
            return false;
        }
        else if (json[xorstr_("Status")] == xorstr_("Hwid updated Sing In again")) {
            ExitProcess(0);
            return false;
        }
        else if (json[xorstr_("Status")] == xorstr_("Hwid not match")) {
            //ExitProcess(0);
            Connection::Ban();
            return false;
        }
        else if (json[xorstr_("Status")] == xorstr_("User don't exist")){
            ExitProcess(0);
            return false;
        }
        else if (json[xorstr_("Status")] == xorstr_("Success"))
        {
            Connection::bLogged = true;
            /*loadOffsets(json);

            if (!std::filesystem::exists(Connection::folder + xorstr_(L"\\CS2"))) {
                std::filesystem::create_directories(Connection::folder + xorstr_(L"\\CS2"));

                Config::save(xorstr_("default"));
                Config::save(xorstr_("legit"));
                Config::save(xorstr_("legit2"));
                Config::save(xorstr_("semi-rage"));
                Config::save(xorstr_("rage"));
            }

            Config::load(xorstr_("default"));*/
            return true;
        }
        else if (json[xorstr_("Status")] == xorstr_("Don't have sub"))
        {
            //ExitProcess(0);
            Connection::Ban();
            return false;
        }
        else if (json[xorstr_("Status")] == xorstr_("Fail"))
        {
            //ExitProcess(0);
            Connection::DelMe();
            return false;
        }
        else if (json[xorstr_("Status")] == xorstr_("don't have permission")){
            ExitProcess(0);
            return false;
        }
        else if (json[xorstr_("Status")] == xorstr_("LoaderStatus empty"))
        {
            //ExitProcess(0);
            Connection::Ban();
            return false;
        }

        readBuffer = "";
    }
    curl_global_cleanup();

    ExitProcess(0);
    return false;
}

#define NAMEOF(name) #name

std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::string convertWstringToString(const std::wstring& wstr) {
    std::string str;
    str.reserve(wstr.size());

    for (wchar_t wch : wstr) {
        if (wch <= 0x7f) {
            str.push_back(static_cast<char>(wch));
        }
    }

    return str;
}

void Connection::Ban() {
    curl = curl_easy_init();

    if (Connection::loaderJson[xorstr_("Hwid")].isNull()) {
        getHwid();
    }

    std::string body{ Connection::loaderJson.toStyledString() };
    if (curl) {
        struct curl_slist* slist1;
        slist1 = NULL;
        slist1 = curl_slist_append(slist1, xorstr_("Content-Type: application/json"));


        curl_easy_setopt(curl, CURLOPT_URL, xorstr_("https://radiationproject.cc/ban.php"));

        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        Connection::DelMe();
        ExitProcess(0);
    }
    curl_global_cleanup();
    //gui::error = xorstr_("FAIL");
    //Debug::bDebug = true;
}
