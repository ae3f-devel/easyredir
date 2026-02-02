#include "./easyredir.h"

#if (defined(_WIN32) && _WIN32)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <assert.h>

#define jmpreturn(a) { RET = (a); goto LBL_RET; }

#ifndef unless
#define unless(x) if(!(x))
#endif

EASYREDIR_IMPL int easyredir_entry2(
    const char* ae2f_restrict const rd_path_istream,
    const char* ae2f_restrict const rd_path_ostream,
    const char* ae2f_restrict const rd_path_estream,
    const char* ae2f_restrict const rd_workdir,
    const char* ae2f_restrict const rd_process,
    const int c_is_append,
    const int c_argc, const char** ae2f_restrict rd_argv
)
{
    int RET = 0;
    DWORD exitCode = 0;
    
    HANDLE hIn = INVALID_HANDLE_VALUE;
    HANDLE hOut = INVALID_HANDLE_VALUE;
    HANDLE hErr = INVALID_HANDLE_VALUE;

    char* cmdLine = NULL;
    size_t cmdLen = 0;
    int i;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    assert(rd_path_istream);
    assert(rd_path_ostream);
    assert(rd_path_estream);
    assert(rd_process);
    assert(*rd_process);
    assert(rd_workdir);

    cmdLen = strlen(rd_process) + 3;
    for (i = 0; i < c_argc; ++i) {
        cmdLen += (strlen(rd_argv[i]) + 3);
    }

    cmdLine = (char*)malloc(cmdLen);
    ae2f_unexpected_but_if(cmdLine == NULL) {
        assert(0 && "malloc for cmdLine failed");
        jmpreturn(-1);
    }

    sprintf(cmdLine, "\"%s\"", rd_process);
    for (i = 0; i < c_argc; ++i) {
        strcat(cmdLine, " \"");
        strcat(cmdLine, rd_argv[i]);
        strcat(cmdLine, "\"");
    }

    if (rd_path_istream[0]) {
        hIn = CreateFileA(rd_path_istream, GENERIC_READ, FILE_SHARE_READ, 
                          &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
        ae2f_unexpected_but_if(hIn == INVALID_HANDLE_VALUE) {
            assert(0 && "ISTREAM open has failed.");
            jmpreturn(-1);
        }
        si.hStdInput = hIn;
    } else {
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    }

    if (rd_path_ostream[0]) {
        DWORD creationDisp = c_is_append ? OPEN_ALWAYS : CREATE_ALWAYS;
        hOut = CreateFileA(rd_path_ostream, GENERIC_WRITE, FILE_SHARE_READ, 
                           &sa, creationDisp, FILE_ATTRIBUTE_NORMAL, NULL);

        ae2f_unexpected_but_if(hOut == INVALID_HANDLE_VALUE) {
            assert(0 && "OSTREAM open has failed.");
            jmpreturn(-1);
        }

        if (c_is_append) {
            SetFilePointer(hOut, 0, NULL, FILE_END);
        }
        si.hStdOutput = hOut;
    } else {
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    if (rd_path_estream[0]) {
        DWORD creationDisp = c_is_append ? OPEN_ALWAYS : CREATE_ALWAYS;
        hErr = CreateFileA(rd_path_estream, GENERIC_WRITE, FILE_SHARE_READ, 
                           &sa, creationDisp, FILE_ATTRIBUTE_NORMAL, NULL);

        ae2f_unexpected_but_if(hErr == INVALID_HANDLE_VALUE) {
            assert(0 && "ESTREAM open has failed.");
            jmpreturn(-1);
        }

        if (c_is_append) {
            SetFilePointer(hErr, 0, NULL, FILE_END);
        }
        si.hStdError = hErr;
    } else {
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    }

    si.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcessA(
            NULL, 
            cmdLine, 
            NULL, 
            NULL, 
            TRUE, 
            0, 
            NULL, 
            (rd_workdir && rd_workdir[0]) ? rd_workdir : NULL, 
            &si, 
            &pi)) 
    {
        assert(0 && "CreateProcess has failed");
        jmpreturn(-1);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
        RET = -1;
    } else {
        RET = (int)exitCode;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

LBL_RET:
    if (cmdLine) free(cmdLine);

    unless(hIn == INVALID_HANDLE_VALUE) CloseHandle(hIn);
    unless(hOut == INVALID_HANDLE_VALUE) CloseHandle(hOut);
    unless(hErr == INVALID_HANDLE_VALUE) CloseHandle(hErr);

    return RET;
}

#undef jmpreturn
#undef unless

#endif
