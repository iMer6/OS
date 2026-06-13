#pragma once
#include <windows.h>

// the preprocessor directive #define takes up only one line
// Backslash '\' for line feed

// Configures structures and runs the program
// with normal priority (NORMAL_PRIORITY_CLASS)
#define LAUNCH_PROCESS_NOW( \
    appPath, /* file path */\
    cmdLine, /* command line arguments */\
    outProcInfo /* where Windows will write info about the created process */\
) \
    do { \
        STARTUPINFO si; /* how the new progarm window will appear */\
        ZeroMemory( /* fills the STARTUPINFO memory area with zeros */\
            &si, /* address of the structure */\
            sizeof(si) /* structure size (bytes) */\
        ); \
        si.cb = sizeof(si); /* cd = Count of Bytes */\
        /* FIll the outProcInfo passed by the user with zeros */\
        ZeroMemory( \
            &outProcInfo, \
            sizeof(outProcInfo) \
        ); \
        CreateProcess( \
            appPath, /* path to executable file */\
            cmdLine, /* command line arguments to be executed */\
            NULL, /* process gets a default security descriptor, handle cannot be inherited */\
            NULL, /* thread gets a default security descriptor, handle cannot be inherited */\
            FALSE, /* handles are not inherited */\
            NORMAL_PRIORITY_CLASS, /* standard Windows Task Scheduler priority */\
            NULL, /* new process copies the environment variables of the calling process */\
            NULL, /* working folder = folder in which the program is running */\
            &si, /* full path to the current directory for the process */\
            &outProcInfo /* pointer to a PROCESS_INFORMATION structure */\
                /* that receives result (hProcess, hThread, dwProcessId, dwThreadId) */\
        ); \
    } while(0);

// Macro for delayed launch
#define LAUNCH_PROCESS_CUSTOM( \
    appPath, /* file path */\
    cmdLine, /* command line arguments */\
    flags, /* process creation flags (CREATE_SUSPENDED, CREATE_NEW_CONSOLE or 0) */\
    outProcInfo /* where Windows will write info about the created process */\
) \
    do { \
        STARTUPINFO si; /* how the new progarm window will appear */\
        ZeroMemory( /* fills the STARTUPINFO memory area with zeros */\
            &si, /* address of the structure */\
            sizeof(si) /* structure size (bytes) */\
        ); \
        si.cb = sizeof(si); /* cd = Count of Bytes */\
        /* FIll the outProcInfo passed by the user with zeros */\
        ZeroMemory( \
            &outProcInfo, \
            sizeof(outProcInfo) \
        ); \
        CreateProcess( \
            appPath, /* path to executable file */\
            cmdLine, /* command line arguments to be executed */\
            NULL, /* process gets a default security descriptor, handle cannot be inherited */\
            NULL, /* thread gets a default security descriptor, handle cannot be inherited */\
            FALSE, /* handles are not inherited */\
            flags, /* process creation flags */\
            NULL, /* new process copies the environment variables of the calling process */\
            NULL, /* working folder = folder in which the program is running */\
            &si, /* full path to the current directory for the process */\
            &outProcInfo /* pointer to a PROCESS_INFORMATION structure */\
                /* that receives result (hProcess, hThread, dwProcessId, dwThreadId) */\
        ); \
    } while(0);
