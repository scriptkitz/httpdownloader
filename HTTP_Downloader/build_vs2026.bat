@echo off
rem ============================================================
rem  Build HTTP Downloader with the VS2026 (v18) toolchain, x64.
rem  The original .vcproj (VS2008) cannot be opened by VS2026;
rem  this script builds it from the command line instead.
rem
rem  Compat shims in _vsbuild\compat (first INCLUDE entry):
rem   - schannel.h  : SDK 26100 copy with the typedefs that clash
rem                   with ssl_schannel.h removed (TLS 1.3 types).
rem   - openssl\*.h : forward declarations. The program loads
rem                   libssl/libcrypto at runtime, so headers are
rem                   only needed for type checking.
rem   - afxres.h    : MFC header replacement (includes winres.h).
rem
rem  The 5 files in asm\ are 32-bit CRT helpers (__allmul etc.)
rem  and are not needed for an x64 build.
rem ============================================================
setlocal enabledelayedexpansion

set "MSVC_DIR=D:\Program Files\Microsoft Visual Studio\18\Professional\VC\Tools\MSVC\14.51.36231"
set "SDK_VER=10.0.26100.0"
set "SDK_DIR=D:\Windows Kits\10"

rem --- Toolchain environment (no vcvars needed) ---
set "PATH=%MSVC_DIR%\bin\Hostx64\x64;%SDK_DIR%\bin\%SDK_VER%\x64;%PATH%"
set "INCLUDE=%MSVC_DIR%\include;%SDK_DIR%\Include\%SDK_VER%\ucrt;%SDK_DIR%\Include\%SDK_VER%\shared;%SDK_DIR%\Include\%SDK_VER%\um"
set "LIB=%MSVC_DIR%\lib\x64;%SDK_DIR%\Lib\%SDK_VER%\ucrt\x64;%SDK_DIR%\Lib\%SDK_VER%\um\x64"

cd /d "%~dp0"

if not exist _vsbuild\obj mkdir _vsbuild\obj

set "CFLAGS=/nologo /c /O2 /MT /EHsc /DUNICODE /D_UNICODE /DWIN32 /D_WINDOWS /DNDEBUG /I. /Fo_vsbuild\obj\"

set "SOURCES=categories.cpp connection.cpp dark_mode.cpp dllrbt.cpp doublylinkedlist.cpp drag_and_drop.cpp file_operations.cpp folder_browser.cpp ftp_parsing.cpp http_parsing.cpp list_operations.cpp lite_advapi32.cpp lite_comctl32.cpp lite_comdlg32.cpp lite_crypt32.cpp lite_dlls.cpp lite_gdi32.cpp lite_kernel32.cpp lite_libcrypto.cpp lite_libssl.cpp lite_normaliz.cpp lite_ntdll.cpp lite_ole32.cpp lite_pcre2.cpp lite_powrprof.cpp lite_psftp.cpp lite_rpcrt4.cpp lite_shell32.cpp lite_user32.cpp lite_uxtheme.cpp lite_winmm.cpp lite_ws2_32.cpp lite_zlib1.cpp main.cpp menus.cpp sftp.cpp site_manager_utilities.cpp ssl_openssl.cpp ssl_schannel.cpp string_tables.cpp system_tray.cpp treelistview.cpp utilities.cpp wnd_proc_add.cpp wnd_proc_check_for_updates.cpp wnd_proc_cmessagebox.cpp wnd_proc_download_speed_limit.cpp wnd_proc_fingerprint_prompt.cpp wnd_proc_main.cpp wnd_proc_options.cpp wnd_proc_options_advanced.cpp wnd_proc_options_appearance.cpp wnd_proc_options_connection.cpp wnd_proc_options_ftp.cpp wnd_proc_options_general.cpp wnd_proc_options_proxy.cpp wnd_proc_options_sftp.cpp wnd_proc_options_sftp_fps.cpp wnd_proc_options_sftp_keys.cpp wnd_proc_options_web_server.cpp wnd_proc_search.cpp wnd_proc_site_manager.cpp wnd_proc_update_download.cpp wnd_proc_url_drop.cpp"

echo === Compiling ===
for %%f in (%SOURCES%) do (
    cl %CFLAGS% %%f || ( echo FAILED: %%f & exit /b 1 )
)

echo === Compiling resources ===
rc /nologo /fo _vsbuild\HTTP_Downloader.res HTTP_Downloader.rc || ( echo RC FAILED & exit /b 1 )

echo === Linking ===
set "LIBS=libcmt.lib libvcruntime.lib libucrt.lib kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib comdlg32.lib advapi32.lib ws2_32.lib ole32.lib oleaut32.lib uxtheme.lib normaliz.lib winmm.lib powrprof.lib rpcrt4.lib crypt32.lib version.lib shlwapi.lib"

link /nologo /OUT:_vsbuild\HTTP_Downloader.exe /SUBSYSTEM:WINDOWS /ENTRY:_WinMain /MANIFEST:EMBED /MANIFESTINPUT:compatibility.manifest _vsbuild\obj\*.obj _vsbuild\HTTP_Downloader.res %LIBS% || ( echo LINK FAILED & exit /b 1 )

echo === Done: _vsbuild\HTTP_Downloader.exe ===
endlocal
