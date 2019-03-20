#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <bits\stdc++.h>
using namespace std;
#include "resources.hpp"
#include <tchar.h>
#include <windows.h>

const int BuffSize = 64 * 1024; ///Block size
///Assigns increasing values from 0-N (macro like)
enum Buttons{
    ENC,DEC,EXIT
};

///Keys of 2 computers
int key_A = 0,
    key_B = 0;

#include "core.hpp"
#include "UI.hpp"

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

char szClassName[ ] = "Encryption app";

///WinMain since it is win 32 GUI
int WINAPI WinMain ( HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow )
{
    ///Mandatory operations
    ///Can also be implemented as pragma
    ///Or __attribute__((constructor))
    INITPROG( key_A , key_B );
    ///Emulates two computers generating their keys

    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    /** The Window structure **/
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      // This function is called by windows
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);

    ///Icon for application
    wincl.hIconSm = (HICON)LoadImage( GetModuleHandle(NULL) ,
                    MAKEINTRESOURCE(IDI_ICON) , IMAGE_ICON, 16 ,16,0);

    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /// Register the window class, and if it fails quit the program
    if ( RegisterClassEx (&wincl) == false )
        return -1;

    hwnd = CreateWindowEx (
           (DWORD)NULL,
           szClassName,
           "ENCRYPTION DECRYPTION",
           WS_MINIMIZEBOX | WS_SYSMENU,
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           500,                 /* The programs width */
           250,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,
           hThisInstance,
           NULL                /* No Window Creation data */
           );

    ShowWindow (hwnd, nCmdShow);
    while (GetMessage (&messages, NULL, 0, 0)){
            TranslateMessage(&messages);
            DispatchMessage(&messages);
    }

    return (messages.wParam);
}

/**
Messages encountered are translated and passed to this method

    @param hwnd : Window handler
    @param Message : Unsigned int representing message
    @param wp : <Dispatch message>
    @param lp : <Dispatch message>

*/
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wp, LPARAM lp ){
    ///Messages to be handled by this program
    switch (message){
        case WM_CREATE:
                InitLayout ( hwnd );
                break;
        case WM_COMMAND:
                switch(wp){
                    static char file[100];///to avoid re allocation
                    ZeroMemory(file,100);
                    case ENC: {
                              ChooseFile(hwnd,file,ENC);
                              encrypt(key_A,file);
                              }
                              break;
                    case DEC: {
                              ChooseFile(hwnd,file,DEC);
                              decrypt(key_B,file);
                              }
                              break;
                    case EXIT:PostQuitMessage(0);
                              break;
                    }
                break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wp, lp);
    }
    return 0;
}
