LRESULT CALLBACK ZoomBarSubClassProc(HWND, UINT, WPARAM, LPARAM);
WNDPROC lpfnOldWndProc;
HWND hHTCSmartTouchDll;

int iZoom;

LRESULT CALLBACK ZoomBarSubClassProc(HWND hWnd,
                                     UINT message,
                                     WPARAM wParam,
                                     LPARAM lParam)
{
    switch (message)
    {
        case (WM_USER+200):
            // slide
            if (wParam == 0x900)
            { // left
                iZoom = 0;
            }
            else if (wParam == 0x800)
            { // right
                iZoom += 1;
            }
            else
            { // bad data - never happens
                iZoom = 0;
            }
            break;
        case (WM_USER+209):
            // touch or release
            if (wParam == 0x0)
            { // touch
                iZoom = 0;
            }
            else if (wParam == 0x1)
            { // release
                if (iZoom >= 10)
                {
                    // DO YOUR UNLOCKING HERE
                }
                
                iZoom = 0;
            }
            else
            { // bad data - never happens
                iZoom = 0;
            }
            break;
    }
    
    return CallWindowProc(lpfnOldWndProc, hWnd, message, wParam, lParam);
}

void StartZoomBar()
{
    hHTCSmartTouchDll = FindWindow(_T("HTCSmartTouchDll"), NULL);
    lpfnOldWndProc = (WNDPROC)GetWindowLong(hHTCSmartTouchDll, GWL_WNDPROC);
    SetWindowLong(hHTCSmartTouchDll, GWL_WNDPROC, (LONG)ZoomBarSubClassProc);
}

void StopZoomBar()
{
    SetWindowLong(hHTCSmartTouchDll, GWL_WNDPROC, (LONG)lpfnOldWndProc);
    CloseHandle(hHTCSmartTouchDll);
}
