#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <type_traits>

#ifdef WIN32
    #include <Windows.h>
#endif

std::string utf16_to_utf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(str);
}

std::wstring utf8_to_utf16(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(str);
}

int main( int argc, char** argv )
{
    try
    {
#ifdef WIN32
        { // Create a file using a utf16 encoded wstring and write utf8 data to it

            // This only works on windows because by filesystem and iostream work in utf8, not utf16 
            std::filesystem::path path(utf8_to_utf16("울란바토르.txt"));
            std::ofstream f(path);
            if(!f.is_open())
                throw std::runtime_error("unable to open file");

            f << "𠜎𠜱𠝹𠱓𠱸𠲖𠳏";
        }
#endif

        { // Create a file using a utf8 encoded string and write utf8 data to it
            std::filesystem::path path = std::filesystem::u8path("猫.txt");
            std::ofstream f(path);
            if(!f.is_open())
                throw std::runtime_error("unable to open file");

            f << "찦차를 타고 온";
        }

#ifndef WIN32
        { // Create a file (same as above) only this time the u8path type is not required since we are on Linux where utf8 is the norm
            std::filesystem::path path("猫猫.txt");
            std::ofstream f(path);
            if(!f.is_open())
                throw std::runtime_error("unable to open file");

            f << "찦차를 타고 온온온온온";

            // While this works on Linux, it is better to specify the path using u8path since that is cross platform
        }
#endif

#ifdef WIN32
        { // Get a utf16 encoded wstring from the Windows API, convert it to utf8, append a utf8 filename and then write to the file

            // Fetch the local users TEMP path using WIN API calls which returns the path as a wide char array of utf16 encoded data
            WCHAR temp_path[MAX_PATH] = {0};
            DWORD result = GetTempPathW(MAX_PATH, temp_path);
            std::wstring temp_path_utf16 = &temp_path[0];

            // Convert the utf16 path to utf8
            std::string temp_path_utf8 = utf16_to_utf8(temp_path_utf16);

            // Construct the path using the utf8 string
            std::filesystem::path path = std::filesystem::u8path(temp_path_utf8);

            // Append a utf8 filename component
            path += std::filesystem::u8path("🎂🎂🎂.txt");

            // Write some utf8 data to the file
            std::ofstream f(path);
            if(!f.is_open())
                throw std::runtime_error("unable to open file");

            f << u8"田中さんにあげて下さい";
        }
#endif

        // TODO: what's a good cross platform strategy given linux operates in utf8 and windows operates in utf16
        
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what();
    }    

    std::cout << "\n\n";
    return 0;
}