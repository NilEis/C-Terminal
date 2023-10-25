#include "../include/terminal.h"
#include <stdio.h>
#include <stdint.h>
#include "constructor.h"

#ifdef __unix__
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
// #include <stropts.h>

void terminal_init(void)
{
}

/**
 * @brief Clears the console
 * @return (void)
 */
void terminal_clear(void)
{
    printf("\e[1;1H\e[2J");
}
/**
 * @brief Sets the color of the console
 *
 * @param f Foreground
 * @param b Background
 * @return (void)
 */
void terminal_set_color(int f, int b)
{
    int foreground = f;
    int background = b + 10;
    printf("\x1b[%d;%dm", background, foreground);
}

/**
 * @brief Resets the colors to black and white
 * @return (void)
 */
void terminal_reset_color(void)
{
    terminal_set_color(LIGHT_WHITE, BLACK);
}

int terminal_get_width(void)
{
    struct winsize max;
    ioctl(0, TIOCGWINSZ, &max);
    return max.ws_col;
}

int terminal_get_height(void)
{
    struct winsize max;
    ioctl(0, TIOCGWINSZ, &max);
    return max.ws_row;
}

void terminal_set_cursor_pos(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

void terminal_sleep(int ms)
{
    struct timespec req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&req, NULL);
}

int terminal_getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

/**
 Linux (POSIX) implementation of _kbhit().
 Morgan McGuire, morgan@cs.brown.edu
 */
int terminal_kbhit(void)
{
    struct termios term, term_old;
    tcgetattr(0, &term_old);
    tcgetattr(0, &term);
    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &term);
    setbuf(stdin, NULL);
    int bytes;
    ioctl(0, FIONREAD, &bytes);
    tcsetattr(0, TCSANOW, &term_old);
    return bytes > 0;
}

#elif defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <conio.h>
HANDLE console_handle;
CONSOLE_SCREEN_BUFFER_INFO csbi;

LPVOID get_last_windows_error(void)
{
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);
    return lpMsgBuf;
}

#ifdef USE_ANSI
#define ESC "\x1b"
#define CSI "\x1b["
#include <stdint.h>

#ifdef USE_ANSI_CONSTRUCTOR
INITIALIZER(EnableVTMode)
{
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        exit(1);
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        exit(1);
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!SetConsoleMode(hOut, dwMode))
    {
        exit(1);
    }
}

#endif

void terminal_init(void)
{
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        wprintf(L"%s\n", get_last_windows_error());
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        wprintf(L"%s\n", get_last_windows_error());
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        wprintf(L"%s\n", get_last_windows_error());
        return;
    }
}

void terminal_clear(void)
{
    printf(CSI "2J");
}

void terminal_set_color(int f, int b)
{
    int foreground = f;
    int background = b + 10;
    printf(CSI "%d;%dm", background, foreground);
}

void terminal_reset_color(void)
{
    printf(CSI "0m");
}

void terminal_set_cursor_pos(int x, int y)
{
    printf(CSI "%d;%dH", y, x);
}

#else

#include <stdint.h>

/**
 * @brief generates a color from a given foreground and background color
 *
 */
#define COL(f, b) ((uint8_t)((b << 4) | f))

void terminal_init(void)
{
}

/**
 * @brief generates a color from r,g and b
 *
 * @param r activate the red bit
 * @param g activate the green bit
 * @param b activate the blue bit
 * @return uint8_t returns a colour in the form of 0b0rgb
 */
uint8_t color_get(int r, int g, int b)
{
    return (((r != 0) * 0b0100) | ((g != 0) * 0b0010)) | ((b != 0) * 0b0001);
}

/**
 * @brief sets the color of the console
 * @param f the foreground color
 * @param b the background color
 * @return (void)
 */
void terminal_set_color(int f, int b)
{
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, COL(f, b));
}

/**
 * @brief resets the color of the console to black and white
 * @return (void)
 */
void terminal_reset_color(void)
{
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, COL(LIGHT_WHITE, BLACK));
}

/**
 * @brief clears the console
 * @return (void)
 */
void terminal_clear(void)
{

    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (console_handle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer;
    DWORD count;
    DWORD cellCount;
    COORD start_coords = {0, 0};

    if (!GetConsoleScreenBufferInfo(console_handle, &screen_buffer))
    {
        return;
    }

    cellCount = screen_buffer.dwSize.X * screen_buffer.dwSize.Y;

    if (!FillConsoleOutputCharacter(console_handle, (TCHAR)' ', cellCount, start_coords, &count))
    {
        return;
    }

    if (!FillConsoleOutputAttribute(console_handle, screen_buffer.wAttributes, cellCount, start_coords, &count))
    {
        return;
    }

    SetConsoleCursorPosition(console_handle, start_coords);
}

void terminal_set_cursor_pos(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

#endif

int terminal_get_width(void)
{
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int terminal_get_height(void)
{
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void terminal_sleep(int ms)
{
    Sleep(ms);
}

int terminal_getch(void)
{
    return getch();
}

int terminal_kbhit(void)
{
    return kbhit();
}

size_t terminal_safe_gets(char *buffer, size_t size)
{
}

#endif