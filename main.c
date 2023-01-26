#include <Windows.h>

//#define IMPL_2

#ifdef IMPL_2
#define MY_VK_CONTROL 162
#define MY_VK_SHIFT 160

LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (wParam != WM_KEYDOWN) return CallNextHookEx(NULL, nCode, wParam, lParam);

    static DWORD firstKeyPressed = 0, firstKeyPressTime = 0;
    LPKBDLLHOOKSTRUCT kbrd = (LPKBDLLHOOKSTRUCT)lParam;

    printf("KEY %lu\n", kbrd->vkCode);
    fflush(stdout);

    if (firstKeyPressed && kbrd->time - firstKeyPressTime < 100)
    {
        if ((firstKeyPressed == MY_VK_CONTROL && kbrd->vkCode == MY_VK_SHIFT) ||
            (firstKeyPressed == MY_VK_SHIFT && kbrd->vkCode == MY_VK_CONTROL))
        {
            PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 2, 0);
            firstKeyPressed = 0;
            firstKeyPressTime = 0;
        }
    }

    if (kbrd->vkCode == MY_VK_CONTROL || kbrd->vkCode == MY_VK_SHIFT)
    {
        firstKeyPressed = kbrd->vkCode;
        firstKeyPressTime = kbrd->time;
    }
    else
    {
        firstKeyPressed = 0;
        firstKeyPressTime = 0;
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
#endif

int WINAPI WinMainCRTStartup()
{
#ifndef IMPL_2
    if (!RegisterHotKey(NULL, WM_HOTKEY, MOD_CONTROL | MOD_SHIFT, 0))
    {
        TCHAR buf[98];
        static LPTSTR errorMsg = TEXT("Failed to register hotkey Ctrl+Shift!\r\n\r\nError code: 0x%08x (Watch its value on MSDN website)");
        wsprintf(buf, errorMsg, GetLastError());
        MessageBox(NULL, buf, TEXT("chkbrd"), MB_ICONERROR);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 2, 0);
        }
    }
    return msg.wParam;
#endif

#ifdef IMPL_2
    if (!SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, NULL, 0))
    {
        TCHAR buf[90];
        static LPTSTR errorMsg = TEXT("Cannot set keyboard hook!\r\n\r\nError code: 0x%08x (Watch its value on MSDN website)");
        wsprintf(buf, errorMsg, GetLastError());
        MessageBox(NULL, buf, TEXT("chkbrd"), MB_ICONERROR);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0);
    return msg.wParam;
#endif
}