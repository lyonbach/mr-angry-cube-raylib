#include "Utilities.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#ifndef PLATFORM_WEB
    #include <openssl/md5.h>
#endif

void Utilities::Log(std::string message, std::string prefix, TraceLogLevel logLevel)
{
    TraceLog(logLevel, ("[" + prefix + "]: " + message).c_str());
}

void Utilities::Log(Vector3 vector, std::string prefix, TraceLogLevel logLevel)
{
    std::string message = "\nX: " + std::to_string(vector.x) + "\nY: " + std::to_string(vector.y) + "\nZ: " + std::to_string(vector.z);
    Log(message, prefix, logLevel);
}

#ifndef PLATFORM_WEB
std::string Utilities::GenerateHash()
{
    // Get the current time
    std::time_t now = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");

    // Convert the time string to MD5 hash
    std::string timeString = ss.str();
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(timeString.c_str()), timeString.size(), digest);

    // Convert the MD5 hash to a hexadecimal string
    std::ostringstream md5String;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        md5String << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }

    return md5String.str();
}
#else
std::string Utilities::GenerateHash()
{
    return std::to_string(GetTime());
}
#endif
