#include "Utilities.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>

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


void Utilities::Log(std::string message, std::string prefix, TraceLogLevel logLevel)
{
    TraceLog(logLevel, ("[" + prefix + "]: " + message).c_str());
}