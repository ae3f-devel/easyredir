#include "./easyredir.h"

#if (defined(_WIN32) && _WIN32)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <assert.h>
#include <ae2f/cc.h>

#define jmpreturn(a) { RET = (a); goto LBL_RET; }

#ifndef unless
#define unless(x) if(!(x))
#endif

/** TODO: complete implementation */
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
    DWORD PROC_RET = 0;
    
    HANDLE ISTREAM = INVALID_HANDLE_VALUE;
    HANDLE OSTREAM = INVALID_HANDLE_VALUE;
    HANDLE ESTREAM = INVALID_HANDLE_VALUE;

    char* ae2f_restrict cmdLine = NULL;
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

    strcpy(cmdLine, "\"");
    strcat(cmdLine, rd_process);
    strcat(cmdLine, "\"");

    for (i = 0; i < c_argc; ++i) {
        strcat(cmdLine, " \"");
        strcat(cmdLine, rd_argv[i]);
        strcat(cmdLine, "\"");
    }

    if (rd_path_istream[0]) {
        ISTREAM= CreateFileA(rd_path_istream, GENERIC_READ
			, FILE_SHARE_READ
                        , &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
        ae2f_unexpected_but_if(ISTREAM == INVALID_HANDLE_VALUE) {
            assert(0 && "ISTREAM open has failed.");
            jmpreturn(-1);
        }
        si.hStdInput = ISTREAM;
    } else {
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    }

    if (rd_path_ostream[0]) {
        DWORD creationDisp = 0;
        
        if (c_is_append) {
            /* * OPEN_ALWAYS: Opens the file if it exists, or creates a new one if it doesn't.
             * This prevents crashes when the file is missing and preserves existing 
             * content for append mode (unlike CREATE_ALWAYS which truncates).
             */
            creationDisp = OPEN_ALWAYS;
        } else {
            creationDisp = CREATE_ALWAYS;
        }

        OSTREAM = CreateFileA(rd_path_ostream, GENERIC_WRITE
			, FILE_SHARE_WRITE
                        , &sa, creationDisp, FILE_ATTRIBUTE_NORMAL, NULL);

        ae2f_unexpected_but_if(OSTREAM == INVALID_HANDLE_VALUE) {
            assert(0 && "OSTREAM open has failed.");
            jmpreturn(-1);
        }

        if (c_is_append) {
            SetFilePointer(OSTREAM, 0, NULL, FILE_END);
        }
        si.hStdOutput = OSTREAM;
    } else {
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    if (rd_path_estream[0]) {
        DWORD creationDisp = 0;
        if (c_is_append) {
            creationDisp = OPEN_ALWAYS;
        } else {
            creationDisp = CREATE_ALWAYS;
        }

        ESTREAM = CreateFileA(rd_path_estream
			, GENERIC_WRITE
			, FILE_SHARE_WRITE
			, &sa, creationDisp, FILE_ATTRIBUTE_NORMAL, NULL);

        ae2f_unexpected_but_if(ESTREAM == INVALID_HANDLE_VALUE) {
            assert(0 && "ESTREAM open has failed.");
            jmpreturn(-1);
        } 

        if (c_is_append) {
            SetFilePointer(ESTREAM, 0, NULL, FILE_END);
        }
        si.hStdError = ESTREAM;
    } else {
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    }

    si.dwFlags |= STARTF_USESTDHANDLES;

    ae2f_expected_but_else (CreateProcessA(
            NULL, 
            cmdLine, 
            NULL, 
            NULL, 
            TRUE, 
            0, 
            NULL, 
            (rd_workdir[0]) ? rd_workdir : NULL, 
            &si, 
            &pi)) 
    {
        assert(0 && "CreateProcess has failed");
        jmpreturn(-1);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    ae2f_expected_but_else (GetExitCodeProcess(pi.hProcess, &PROC_RET)) {
        RET = -1;
    } else {
        RET = (int)PROC_RET;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

LBL_RET:
    if (cmdLine) free(cmdLine);

    unless(ISTREAM == INVALID_HANDLE_VALUE) CloseHandle(ISTREAM);
    unless(OSTREAM == INVALID_HANDLE_VALUE) CloseHandle(OSTREAM);
    unless(ESTREAM == INVALID_HANDLE_VALUE) CloseHandle(ESTREAM);

    return RET;
}

#undef jmpreturn
#undef unless

#endif
