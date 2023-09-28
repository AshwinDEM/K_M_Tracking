#include <windows.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>

double calculateDistance(POINT p1, POINT p2) {
  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
  return std::sqrt(dx * dx + dy * dy);
}

int main() {
  std::ofstream csvFile("mouse_data.csv", std::ios::app);
  if (!csvFile.is_open()) {
    std::cerr << "Error opening the CSV file." << std::endl;
    return 1;
  }

  if (csvFile.tellp() == 0) {
    csvFile << "Date,Time,X coordinate,Y coordinate,Button state,Current Window,Time Interval (ms),Velocity (pixels/ms),Acceleration (pixels/ms^2),Distance Traveled (pixels)" << std::endl;
  }

  POINT prevCursorPosition = {0, 0};
  bool prevLeftMousePressed = false;
  bool prevRightMousePressed = false;
  char prevWindowTitle[256] = "";
  auto prevTimestamp = std::chrono::high_resolution_clock::now();
  double prevVelocity = 0.0;

  char windowTitle[256] = "";

  while (true) {
    POINT cursorPosition;
    GetCursorPos(&cursorPosition);

    bool isLeftMousePressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    bool isRightMousePressed = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

    auto currentTimestamp = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(currentTimestamp);
    std::tm* localTime = std::localtime(&timestamp);

    char dateStr[11];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localTime);

    char timeStr[9];
    std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localTime);

    auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimestamp - prevTimestamp);
    double distance = calculateDistance(prevCursorPosition, cursorPosition);
    double velocity = distance / interval.count();
    double acceleration = (velocity - prevVelocity) / interval.count();

    HWND foregroundWindow = GetForegroundWindow();
    GetWindowTextA(foregroundWindow, windowTitle, sizeof(windowTitle));


    if ((cursorPosition.x != prevCursorPosition.x || cursorPosition.y != prevCursorPosition.y) ||
        (isLeftMousePressed != prevLeftMousePressed) ||
        (isRightMousePressed != prevRightMousePressed) ||
        (strcmp(windowTitle, prevWindowTitle) != 0) ) {

      csvFile << dateStr << ","
              << timeStr << ","
              << cursorPosition.x << ","
              << cursorPosition.y << ","
              << (isLeftMousePressed ? "Left Pressed" : (isRightMousePressed ? "Right Pressed" : "None")) << ","
              << windowTitle << ","
              << interval.count() << ","
              << std::fixed << std::setprecision(2) << velocity << ","
              << std::fixed << std::setprecision(2) << acceleration << ","
              << distance << std::endl;

      prevCursorPosition = cursorPosition;
      prevLeftMousePressed = isLeftMousePressed;
      prevRightMousePressed = isRightMousePressed;
      strncpy(prevWindowTitle, windowTitle, sizeof(prevWindowTitle));
      prevTimestamp = currentTimestamp;
      prevVelocity = velocity;
    }

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
      break;
    }

    Sleep(10);
  }

  csvFile.close();

  return 0;
}
