#include <winhttp.h>
#include <string>
#pragma comment(lib, "winhttp.lib")

struct ReleaseAboutResult {
    HANDLE finished;
    volatile LONG references;
    std::string status;
};

static void ReleaseAboutDone(ReleaseAboutResult *result)
{
    if (!InterlockedDecrement(&result->references)) {
        CloseHandle(result->finished);
        delete result;
    }
}

static DWORD WINAPI ReadLatestRelease(void *context)
{
    ReleaseAboutResult *result=static_cast<ReleaseAboutResult *>(context);
    HINTERNET session=WinHttpOpen(L"APRSIS32",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,0,0,0);
    HINTERNET server=NULL, request=NULL;
    std::string body;
    if (session) {
        WinHttpSetTimeouts(session,3000,3000,3000,5000);
        server=WinHttpConnect(session,L"api.github.com",INTERNET_DEFAULT_HTTPS_PORT,0);
    }
    if (server) request=WinHttpOpenRequest(server,L"GET",L"/repos/kj4dxk/APRSIS32/releases/latest",
        NULL,WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,WINHTTP_FLAG_SECURE);
    if (request && WinHttpSendRequest(request,WINHTTP_NO_ADDITIONAL_HEADERS,0,WINHTTP_NO_REQUEST_DATA,0,0,0)
        && WinHttpReceiveResponse(request,NULL)) {
        DWORD status=0, size=sizeof(status);
        if (WinHttpQueryHeaders(request,WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX,&status,&size,WINHTTP_NO_HEADER_INDEX) && status==200) {
            char chunk[4096];
            DWORD count=0;
            do {
                if (!WinHttpReadData(request,chunk,sizeof(chunk),&count)) { body.clear(); break; }
                body.append(chunk,count);
            } while (count && body.size()<1024*1024);
        }
    }
    if (request) WinHttpCloseHandle(request);
    if (server) WinHttpCloseHandle(server);
    if (session) WinHttpCloseHandle(session);
    result->status="Could not check GitHub for updates.";
    size_t field=body.find("\"tag_name\"");
    if (field!=std::string::npos) {
        size_t colon=body.find(':',field+10);
        size_t start=colon==std::string::npos ? colon : body.find_first_not_of(" \r\n\t",colon+1);
        if (start!=std::string::npos && start+16<body.size() && body[start]=='"' && body[start+16]=='"') {
            std::string tag=body.substr(start+1,15);
            bool valid=true;
            for (size_t i=0;i<tag.size();i++)
                valid=valid && ((i==4 || i==7 || i==10) ? tag[i]=='.' : tag[i]>='0' && tag[i]<='9');
            if (valid) result->status=(tag>VER_FILE_VERSION_STR ? "Update available on GitHub: " : "Latest stable on GitHub: ")+tag;
        }
    }
    SetEvent(result->finished);
    ReleaseAboutDone(result);
    return 0;
}

struct ReleaseAboutDialog {
    bool online;
    ReleaseAboutResult *result;
    HANDLE updater;
    WCHAR stage[MAX_PATH];
};

static HANDLE StartReleaseUpdate(WCHAR *stage)
{
    WCHAR temp[MAX_PATH], script[MAX_PATH], target[MAX_PATH], system[MAX_PATH];
    if (!GetTempPathW(MAX_PATH,temp) || !GetTempFileNameW(temp,L"a32",0,stage) ||
        !DeleteFileW(stage) || !CreateDirectoryW(stage,NULL) ||
        !GetModuleFileNameW(NULL,target,MAX_PATH) || !GetSystemDirectoryW(system,MAX_PATH) ||
        FAILED(StringCchPrintfW(script,MAX_PATH,L"%s\\ReleaseUpdate.ps1",stage))) return NULL;
    HRSRC source=FindResourceW(NULL,L"APRSIS32_RELEASE_UPDATE",MAKEINTRESOURCEW(10));
    if (!source) return NULL;
    HGLOBAL resource=LoadResource(NULL,source);
    if (!resource) return NULL;
    HANDLE file=CreateFileW(script,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
    if (file==INVALID_HANDLE_VALUE) return NULL;
    DWORD count=0, size=SizeofResource(NULL,source);
    BOOL saved=WriteFile(file,LockResource(resource),size,&count,NULL);
    CloseHandle(file);
    if (!saved || count!=size) return NULL;
    WCHAR powershell[MAX_PATH], command[MAX_PATH*4];
    if (FAILED(StringCchPrintfW(powershell,MAX_PATH,L"%s\\WindowsPowerShell\\v1.0\\powershell.exe",system)) ||
        FAILED(StringCchPrintfW(command,ARRAYSIZE(command),
        L"\"%s\" -NoProfile -NonInteractive -ExecutionPolicy Bypass -File \"%s\" -Target \"%s\" -Installed %S -ParentId %lu",
        powershell,script,target,VER_FILE_VERSION_STR,GetCurrentProcessId()))) return NULL;
    STARTUPINFOW startup={sizeof(startup)};
    PROCESS_INFORMATION process={0};
    if (!CreateProcessW(powershell,command,NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,NULL,&startup,&process)) return NULL;
    CloseHandle(process.hThread);
    return process.hProcess;
}

static void ConfirmReleaseStartup()
{
    WCHAR name[128];
    DWORD size=GetEnvironmentVariableW(L"APRSIS32_UPDATE_EVENT",name,ARRAYSIZE(name));
    if (!size || size>=ARRAYSIZE(name) || wcsncmp(name,L"Local\\APRSIS32-Update-",21)) return;
    HANDLE event=OpenEventW(EVENT_MODIFY_STATE,FALSE,name);
    if (event) { SetEvent(event); CloseHandle(event); }
}

static INT_PTR CALLBACK ReleaseAboutProc(HWND dialog,UINT message,WPARAM command,LPARAM parameter)
{
    const int StatusId=501, ReleasesId=502, UpdateId=503;
    ReleaseAboutDialog *state=(ReleaseAboutDialog *)GetWindowLongPtr(dialog,DWLP_USER);
    switch(message) {
    case WM_INITDIALOG: {
        state=(ReleaseAboutDialog *)parameter;
        SetWindowLongPtr(dialog,DWLP_USER,(LONG_PTR)state);
        SetWindowText(dialog,TEXT("About APRSIS32"));
        struct Control { LPCWSTR type; LPCWSTR text; int id,x,y,w,h; DWORD style; };
        const Control controls[]={
            {L"STATIC",L"APRSIS32 by Lynn W. Deffenbaugh, KJ4ERJ",0,8,8,232,10,0},
            {L"STATIC",TEXT("Installed: ") TEXT(VER_FILE_VERSION_STR),0,8,22,232,10,0},
            {L"STATIC",state->online?L"Checking GitHub...":L"Internet access is disabled.",StatusId,8,36,232,24,0},
            {L"BUTTON",L"GitHub Releases",ReleasesId,8,63,89,15,WS_TABSTOP},
            {L"BUTTON",L"Update",UpdateId,130,63,52,15,WS_TABSTOP},
            {L"BUTTON",L"Close",IDCANCEL,190,63,49,15,WS_TABSTOP|BS_DEFPUSHBUTTON}
        };
        for (unsigned i=0;i<ARRAYSIZE(controls);i++) {
            const Control &c=controls[i];
            RECT bounds={c.x,c.y,c.x+c.w,c.y+c.h};
            MapDialogRect(dialog,&bounds);
            HWND control=CreateWindowW(c.type,c.text,WS_CHILD|WS_VISIBLE|c.style,bounds.left,bounds.top,
                bounds.right-bounds.left,bounds.bottom-bounds.top,dialog,(HMENU)(INT_PTR)c.id,GetModuleHandle(NULL),NULL);
            SendMessage(control,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),TRUE);
        }
        EnableWindow(GetDlgItem(dialog,UpdateId),FALSE);
        if (state->online) {
            ReleaseAboutResult *result=new ReleaseAboutResult;
            result->finished=CreateEvent(NULL,TRUE,FALSE,NULL);
            result->references=2;
            HANDLE thread=result->finished ? CreateThread(NULL,0,ReadLatestRelease,result,0,NULL) : NULL;
            if (thread) {
                CloseHandle(thread);
                state->result=result;
                SetTimer(dialog,1,100,NULL);
            } else {
                ReleaseAboutDone(result);
                ReleaseAboutDone(result);
                SetDlgItemText(dialog,StatusId,TEXT("Could not start the version check."));
            }
        }
        return TRUE;
    }
    case WM_TIMER:
        if (state && state->updater) {
            WCHAR path[MAX_PATH];
            StringCchPrintfW(path,MAX_PATH,L"%s\\restart",state->stage);
            if (GetFileAttributesW(path)!=INVALID_FILE_ATTRIBUTES &&
                WaitForSingleObject(state->updater,0)==WAIT_TIMEOUT) {
                HWND owner=GetWindow(dialog,GW_OWNER);
                EndDialog(dialog,IDCANCEL);
                PostMessage(owner,WM_CLOSE,0,0);
            } else if (WaitForSingleObject(state->updater,0)==WAIT_OBJECT_0) {
                KillTimer(dialog,1);
                CloseHandle(state->updater);
                state->updater=NULL;
                char report[512]={0};
                DWORD count=0;
                StringCchPrintfW(path,MAX_PATH,L"%s\\status",state->stage);
                HANDLE file=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
                if (file!=INVALID_HANDLE_VALUE) {
                    ReadFile(file,report,sizeof(report)-1,&count,NULL);
                    CloseHandle(file);
                }
                SetDlgItemTextA(dialog,StatusId,count?report:"The update could not be installed.");
                EnableWindow(GetDlgItem(dialog,UpdateId),TRUE);
            }
            return TRUE;
        }
        if (state && state->result && WaitForSingleObject(state->result->finished,0)==WAIT_OBJECT_0) {
            KillTimer(dialog,1);
            SetDlgItemTextA(dialog,StatusId,state->result->status.c_str());
            EnableWindow(GetDlgItem(dialog,UpdateId),state->result->status.find("Update available")==0);
        }
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(command)==UpdateId && state && !state->updater &&
            state->result && state->result->status.find("Update available")==0) {
            state->updater=StartReleaseUpdate(state->stage);
            if (state->updater) {
                EnableWindow(GetDlgItem(dialog,UpdateId),FALSE);
                SetDlgItemText(dialog,StatusId,TEXT("Downloading and verifying the update..."));
                SetTimer(dialog,1,100,NULL);
            } else SetDlgItemText(dialog,StatusId,TEXT("Could not start the update."));
            return TRUE;
        }
        if (LOWORD(command)==ReleasesId) {
            ShellExecute(dialog,TEXT("open"),TEXT("https://github.com/kj4dxk/APRSIS32/releases/latest"),NULL,NULL,SW_SHOWNORMAL);
            return TRUE;
        }
        if (LOWORD(command)==IDCANCEL) {
            if (!state || !state->updater) EndDialog(dialog,IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void ShowReleaseAbout(HWND owner,bool online)
{
    ReleaseAboutDialog state={online,NULL,NULL,{0}};
    struct alignas(DWORD) DialogTemplate { DLGTEMPLATE dialog; WORD menu, windowClass, title; };
    DialogTemplate layout={{WS_POPUP|WS_CAPTION|WS_SYSMENU|DS_MODALFRAME|DS_CENTER,0,0,0,0,248,86},0,0,0};
    DialogBoxIndirectParam(GetModuleHandle(NULL),&layout.dialog,owner,ReleaseAboutProc,(LPARAM)&state);
    if (state.result) ReleaseAboutDone(state.result);
    if (state.updater) CloseHandle(state.updater);
}
