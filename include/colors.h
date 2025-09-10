#pragma once

#include <windows.h>

// going to make this configurable later
 
// Console color macros for neofetch
#define setlght SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3)   // ASCII art
#define setdflt SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7)   // default text  
#define setbarc SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13)  // progress bars

/*
 * 0=Black, 1=Blue, 2=Green, 3=Cyan, 4=Red, 5=Magenta, 6=Yellow, 7=Gray,
 * 8=DarkGray, 9=BrightBlue, 10=BrightGreen, 11=BrightCyan, 12=BrightRed,
 * 13=BrightMagenta, 14=BrightYellow, 15=White
 */
