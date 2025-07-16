/*
	HTTP Downloader can download files through HTTP(S), FTP(S), and SFTP connections.
	Copyright (C) 2015-2025 Eric Kutcher

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _LITE_USER32_H
#define _LITE_USER32_H

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define USER32_USE_STATIC_LIB

#ifdef USER32_USE_STATIC_LIB

	//__pragma( comment( lib, "user32.lib" ) )

	//#define _AdjustWindowRectEx		AdjustWindowRectEx
	#define _BeginDeferWindowPos	BeginDeferWindowPos
	#define _BeginPaint				BeginPaint
	#define _CallWindowProcW		CallWindowProcW
	//#define _CharLowerA				CharLowerA
	//#define _CharLowerBuffA			CharLowerBuffA
	//#define _CharUpperA			CharUpperA
	#define _CheckMenuItem			CheckMenuItem
	#define _ClientToScreen			ClientToScreen
	#define _CloseClipboard			CloseClipboard
	#define _CreateMenu				CreateMenu
	#define _CreatePopupMenu		CreatePopupMenu
	#define _CreateWindowExW		CreateWindowExW
	#define _DefWindowProcW			DefWindowProcW
	#define _DeferWindowPos			DeferWindowPos
	//#define _DeleteMenu				DeleteMenu
	#define _DestroyIcon			DestroyIcon
	#define _DestroyMenu			DestroyMenu
	#define _DestroyWindow			DestroyWindow
	#define _DispatchMessageW		DispatchMessageW
	//#define _DrawEdge				DrawEdge
	#define _DrawFocusRect			DrawFocusRect
	#define _DrawIconEx				DrawIconEx
	//#define _DrawMenuBar			DrawMenuBar
	#define _DrawTextW				DrawTextW
	#define _EmptyClipboard			EmptyClipboard
	#define _EnableMenuItem			EnableMenuItem
	#define _EnableWindow			EnableWindow
	#define _EndDeferWindowPos		EndDeferWindowPos
	//#define _EndMenu				EndMenu
	#define _EndPaint				EndPaint
	//#define _EnumChildWindows		EnumChildWindows
	#define _ExitWindowsEx			ExitWindowsEx
	#define _FillRect				FillRect
	#define _FindWindowW			FindWindowW
	#define _FlashWindow			FlashWindow
	#define _FrameRect				FrameRect
	//#define _GetClassLongPtrW		GetClassLongPtrW
	#define _GetClientRect			GetClientRect
	#define _GetClipboardData		GetClipboardData
	#define _GetCursorPos			GetCursorPos
	#define _GetDC					GetDC
	#define _GetDlgCtrlID			GetDlgCtrlID
	//#define _GetDlgItem				GetDlgItem
	#define _GetFocus				GetFocus
	#define _GetIconInfo			GetIconInfo
	#define _GetKeyState			GetKeyState
	//#define _GetMenuItemInfoW		GetMenuItemInfoW
	//#define _GetMessagePos			GetMessagePos
	#define _GetMessageW			GetMessageW
	#define _GetMonitorInfoW		GetMonitorInfoW
	#define _GetParent				GetParent
	#define _GetScrollInfo			GetScrollInfo
	//#define _GetSubMenu				GetSubMenu
	#define _GetSysColor			GetSysColor
	#define _GetSysColorBrush		GetSysColorBrush
	#define _GetSystemMetrics		GetSystemMetrics
	#define _GetWindow				GetWindow
	#define _GetWindowDC			GetWindowDC
	//#define _GetWindowLongW			GetWindowLongW
	#define _GetWindowLongPtrW		GetWindowLongPtrW
	#define _GetWindowRect			GetWindowRect
	#define _InsertMenuItemW		InsertMenuItemW
	#define _InvalidateRect			InvalidateRect
	//#define _IsCharAlphaNumericA	IsCharAlphaNumericA
	#define _IsClipboardFormatAvailable	IsClipboardFormatAvailable
	#define _IsDialogMessageW		IsDialogMessageW
	#define _IsIconic				IsIconic
	#define _IsWindowEnabled		IsWindowEnabled
	#define _IsWindowVisible		IsWindowVisible
	#define _IsZoomed				IsZoomed
	#define _KillTimer				KillTimer
	#define _LoadCursorW			LoadCursorW
	#define _LoadIconW				LoadIconW
	#define _LoadImageW				LoadImageW
	#define _LockWorkStation		LockWorkStation
	#define _MapWindowPoints		MapWindowPoints
	//#define _MessageBoxA			MessageBoxA
	#define _MessageBoxW			MessageBoxW
	#define _MonitorFromRect		MonitorFromRect
	#define _MonitorFromWindow		MonitorFromWindow
	#define _OffsetRect				OffsetRect
	#define _OpenClipboard			OpenClipboard
	#define _PostMessageW			PostMessageW
	#define _PostQuitMessage		PostQuitMessage
	#define _RegisterClassExW		RegisterClassExW
	#define _RegisterClipboardFormatW	RegisterClipboardFormatW
	#define _RegisterWindowMessageW	RegisterWindowMessageW
	#define _ReleaseCapture			ReleaseCapture
	#define _ReleaseDC				ReleaseDC
	//#define _RemoveMenu				RemoveMenu
	#define _ScreenToClient			ScreenToClient
	#define _ScrollWindow			ScrollWindow
	#define _SendMessageA			SendMessageA
	#define _SendMessageW			SendMessageW
	#define _SendNotifyMessageW		SendNotifyMessageW
	#define _SetCapture				SetCapture
	//#define _SetClassLongPtrW		SetClassLongPtrW
	#define _SetClipboardData		SetClipboardData
	#define _SetCursor				SetCursor
	#define _SetFocus				SetFocus
	#define _SetForegroundWindow	SetForegroundWindow
	#define _SetLayeredWindowAttributes		SetLayeredWindowAttributes
	#define _SetMenu				SetMenu
	#define _SetMenuItemInfoW		SetMenuItemInfoW
	//#define _SetRect				SetRect
	#define _SetScrollInfo			SetScrollInfo
	#define _SetScrollPos			SetScrollPos
	#define _SetTimer				SetTimer
	//#define _SetWindowLongW			SetWindowLongW
	#define _SetWindowLongPtrW		SetWindowLongPtrW
	#define _SetWindowPos			SetWindowPos
	//#define _SetWindowTextW			SetWindowTextW
	#define _ShowWindow				ShowWindow
	#define _SystemParametersInfoW	SystemParametersInfoW
	#define _TrackMouseEvent		TrackMouseEvent
	#define _TrackPopupMenu			TrackPopupMenu
	#define _TranslateMessage		TranslateMessage

	#define _CreateWindowW			CreateWindowW

#else

	#define USER32_STATE_SHUTDOWN	0
	#define USER32_STATE_RUNNING	1

	//typedef BOOL ( WINAPI *pAdjustWindowRectEx )( LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle );
	typedef HDWP ( WINAPI *pBeginDeferWindowPos )( int nNumWindows );
	typedef HDC ( WINAPI *pBeginPaint )( HWND hwnd, LPPAINTSTRUCT lpPaint );
	typedef LRESULT ( WINAPI *pCallWindowProcW )( WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	//typedef LPSTR ( WINAPI *pCharLowerA )( LPSTR lpsz );
	//typedef DWORD ( WINAPI *pCharLowerBuffA )( LPTSTR lpsz, DWORD cchLength );
	//typedef LPSTR ( WINAPI *pCharUpperA )( LPSTR lpsz );
	typedef DWORD ( WINAPI *pCheckMenuItem )( HMENU hmenu, UINT uIDCheckItem, UINT uCheck );
	typedef BOOL ( WINAPI *pClientToScreen )( HWND hWnd, LPPOINT lpPoint );
	typedef BOOL ( WINAPI *pCloseClipboard )( void );
	typedef HMENU ( WINAPI *pCreateMenu )( void );
	typedef HMENU ( WINAPI *pCreatePopupMenu )( void );
	typedef HWND ( WINAPI *pCreateWindowExW )( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam );
	typedef LRESULT ( WINAPI *pDefWindowProcW )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	typedef HMENU ( WINAPI *pDeferWindowPos )( HDWP hWinPosInfo, HWND hWnd, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags );
	//typedef BOOL ( WINAPI *pDeleteMenu )( HMENU hMenu, UINT uPosition, UINT uFlags );
	typedef BOOL ( WINAPI *pDestroyIcon )( HICON hIcon );
	typedef BOOL ( WINAPI *pDestroyMenu )( HMENU hMenu );
	typedef BOOL ( WINAPI *pDestroyWindow )( HWND hWnd );
	typedef LRESULT ( WINAPI *pDispatchMessageW )( const MSG *lpmsg );
	//typedef BOOL ( WINAPI *pDrawEdge )( HDC hdc, LPRECT qrc, UINT edge, UINT grfFlags );
	typedef BOOL ( WINAPI *pDrawFocusRect )( HDC hDC, const RECT *lprc );
	typedef BOOL ( WINAPI *pDrawIconEx )( HDC hdc, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags );
	//typedef BOOL ( WINAPI *pDrawMenuBar )( HWND hWnd );
	typedef int ( WINAPI *pDrawTextW )( HDC hDC, LPCTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat );
	typedef BOOL ( WINAPI *pEmptyClipboard )( void );
	typedef BOOL ( WINAPI *pEnableMenuItem )( HMENU hMenu, UINT uIDEnableItem, UINT uEnable );
	typedef BOOL ( WINAPI *pEnableWindow )( HWND hWnd, BOOL bEnable );
	typedef BOOL ( WINAPI *pEndDeferWindowPos )( HDWP hWinPosInfo );
	//typedef BOOL ( WINAPI *pEndMenu )();
	typedef BOOL ( WINAPI *pEndPaint )( HWND hWnd, const PAINTSTRUCT *lpPaint );
	//typedef BOOL ( WINAPI *pEnumChildWindows )( HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam );
	typedef BOOL ( WINAPI *pExitWindowsEx )( UINT uFlags, DWORD dwReason );
	typedef int ( WINAPI *pFillRect )( HDC hDC, const RECT *lprc, HBRUSH hbr );
	typedef HWND ( WINAPI *pFindWindowW )( LPCWSTR lpClassName, LPCWSTR lpWindowName );
	typedef BOOL ( WINAPI *pFlashWindow )( HWND hWnd, BOOL bInvert );
	typedef int ( WINAPI *pFrameRect )( HDC hDC, const RECT *lprc, HBRUSH hbr );
	//typedef ULONG_PTR WINAPI ( WINAPI *pGetClassLongPtrW )( HWND hWnd, int nIndex );
	typedef BOOL ( WINAPI *pGetClientRect )( HWND hWnd, LPRECT lpRect );
	typedef HANDLE ( WINAPI *pGetClipboardData )( UINT uFormat );
	typedef BOOL ( WINAPI *pGetCursorPos )( LPPOINT lpPoint );
	typedef HDC ( WINAPI *pGetDC )( HWND hWnd );
	typedef int ( WINAPI *pGetDlgCtrlID )( HWND hWnd );
	//typedef HWND ( WINAPI *pGetDlgItem )( HWND hDlg, int nIDDlgItem );
	typedef HWND ( WINAPI *pGetFocus )();
	typedef BOOL ( WINAPI *pGetIconInfo )( HICON hIcon, PICONINFO piconinfo );
	typedef SHORT ( WINAPI *pGetKeyState )( int nVirtKey );
	//typedef BOOL ( WINAPI *pGetMenuItemInfoW )( HMENU hMenu, UINT uItem, BOOL fByPosition, LPMENUITEMINFO lpmii );
	//typedef DWORD ( WINAPI *pGetMessagePos )( void );
	typedef BOOL ( WINAPI *pGetMessageW )( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax );
	typedef BOOL ( WINAPI *pGetMonitorInfoW )( HMONITOR hMonitor, LPMONITORINFO lpmi );
	typedef HWND ( WINAPI *pGetParent )( HWND hWnd );
	typedef BOOL ( WINAPI *pGetScrollInfo )( HWND hwnd, int nBar, LPSCROLLINFO lpsi );
	//typedef HMENU ( WINAPI *pGetSubMenu )( HMENU hMenu, int nPos );
	typedef DWORD ( WINAPI *pGetSysColor )( int nIndex );
	typedef HBRUSH ( WINAPI *pGetSysColorBrush )( int nIndex );
	typedef int ( WINAPI *pGetSystemMetrics )( int nIndex );
	typedef HWND ( WINAPI *pGetWindow )( HWND hWnd, UINT uCmd );
	typedef HDC ( WINAPI *pGetWindowDC )( HWND hWnd );
	//typedef LONG ( WINAPI *pGetWindowLongW )( HWND hWnd, int nIndex );
	typedef LONG_PTR ( WINAPI *pGetWindowLongPtrW )( HWND hWnd, int nIndex );
	typedef BOOL ( WINAPI *pGetWindowRect )( HWND hWnd, LPRECT lpRect );
	typedef BOOL ( WINAPI *pInsertMenuItemW )( HMENU hMenu, UINT uItem, BOOL fByPosition, LPCMENUITEMINFO lpmii );
	typedef BOOL ( WINAPI *pInvalidateRect )( HWND hWnd, const RECT *lpRect, BOOL bErase );
	//typedef BOOL ( WINAPI *pIsCharAlphaNumericA )( CHAR ch );
	typedef BOOL ( WINAPI *pIsClipboardFormatAvailable )( UINT format );
	typedef BOOL ( WINAPI *pIsDialogMessageW )( HWND hDlg, LPMSG lpMsg );
	typedef BOOL ( WINAPI *pIsIconic )( HWND hWnd );
	typedef BOOL ( WINAPI *pIsWindowEnabled )( HWND hWnd );
	typedef BOOL ( WINAPI *pIsWindowVisible )( HWND hWnd );
	typedef BOOL ( WINAPI *pIsZoomed )( HWND hWnd );
	typedef BOOL ( WINAPI *pKillTimer )( HWND hWnd, UINT_PTR uIDEvent );
	typedef HCURSOR ( WINAPI *pLoadCursorW )( HINSTANCE hInstance, LPCTSTR lpCursorName );
	typedef HICON ( WINAPI *pLoadIconW )( HINSTANCE hInstance, LPCTSTR lpIconName );
	typedef HANDLE ( WINAPI *pLoadImageW )( HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad );
	typedef BOOL ( WINAPI *pLockWorkStation )();
	typedef int ( WINAPI *pMapWindowPoints )( HWND hWndFrom, HWND hWndTo, LPPOINT lpPoints, UINT cPoints );
	//typedef int ( WINAPI *pMessageBoxA )( HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType );
	typedef int ( WINAPI *pMessageBoxW )( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType );
	typedef HMONITOR ( WINAPI *pMonitorFromRect )( LPCRECT lprc, DWORD dwFlags );
	typedef HMONITOR ( WINAPI *pMonitorFromWindow )( HWND hwnd, DWORD dwFlags );
	typedef BOOL ( WINAPI *pOffsetRect )( LPRECT lprc, int dx, int dy );
	typedef BOOL ( WINAPI *pOpenClipboard )( HWND hWndNewOwner );
	typedef BOOL ( WINAPI *pPostMessageW )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	typedef VOID ( WINAPI *pPostQuitMessage )( int nExitCode );
	typedef ATOM ( WINAPI *pRegisterClassExW )( const WNDCLASSEX *lpwcx );
	typedef UINT ( WINAPI *pRegisterClipboardFormatW )( LPCTSTR lpszFormat );
	typedef UINT ( WINAPI *pRegisterWindowMessageW )( LPCWSTR lpString );
	typedef BOOL ( WINAPI *pReleaseCapture )( void );
	typedef int ( WINAPI *pReleaseDC )( HWND hWnd, HDC hDC );
	//typedef BOOL ( WINAPI *pRemoveMenu )( HMENU hMenu, UINT uPosition, UINT uFlags );
	typedef BOOL ( WINAPI *pScreenToClient )( HWND hWnd, LPPOINT lpPoint );
	typedef BOOL ( WINAPI *pScrollWindow )( HWND hWnd, int XAmount, int YAmount, const RECT *lpRect, const RECT *lpClipRect );
	typedef LRESULT ( WINAPI *pSendMessageA )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	typedef LRESULT ( WINAPI *pSendMessageW )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	typedef BOOL ( WINAPI *pSendNotifyMessageW )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	typedef HWND ( WINAPI *pSetCapture )( HWND hWnd );
	//typedef ULONG_PTR WINAPI ( WINAPI *pSetClassLongPtrW )( HWND hWnd, int nIndex, LONG_PTR dwNewLong );
	typedef HANDLE ( WINAPI *pSetClipboardData )( UINT uFormat, HANDLE hMem );
	typedef HCURSOR ( WINAPI *pSetCursor )( HCURSOR hCursor );
	typedef HWND ( WINAPI *pSetFocus )( HWND hWnd );
	typedef BOOL ( WINAPI *pSetForegroundWindow )( HWND hWnd );
	typedef BOOL ( WINAPI *pSetLayeredWindowAttributes )( HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags );
	typedef BOOL ( WINAPI *pSetMenu )( HWND hWnd, HMENU hMenu );
	typedef BOOL ( WINAPI *pSetMenuItemInfoW )( HMENU hMenu, UINT uItem, BOOL fByPosition, LPMENUITEMINFO lpmii );
	//typedef BOOL ( WINAPI *pSetRect )( LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom );
	typedef int ( WINAPI *pSetScrollInfo )( HWND hwnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw );
	typedef int ( WINAPI *pSetScrollPos )( HWND hWnd, int nBar, int nPos, BOOL bRedraw );
	typedef UINT_PTR ( WINAPI *pSetTimer )( HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc );
	//typedef LONG ( WINAPI *pSetWindowLongW )( HWND hWnd, int nIndex, LONG dwNewLong );
	typedef LONG_PTR ( WINAPI *pSetWindowLongPtrW )( HWND hWnd, int nIndex, LONG_PTR dwNewLong );
	typedef BOOL ( WINAPI *pSetWindowPos )( HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags );
	//typedef BOOL ( WINAPI *pSetWindowTextW )( HWND hWnd, LPCTSTR lpString );
	typedef BOOL ( WINAPI *pShowWindow )( HWND hWnd, int nCmdShow );
	typedef BOOL ( WINAPI *pSystemParametersInfoW )( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni );
	typedef BOOL ( WINAPI *pTrackMouseEvent )( LPTRACKMOUSEEVENT lpEventTrack );
	typedef BOOL ( WINAPI *pTrackPopupMenu )( HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, const RECT *prcRect );
	typedef BOOL ( WINAPI *pTranslateMessage )( const MSG *lpMsg );

	//extern pAdjustWindowRectEx		_AdjustWindowRectEx;
	extern pBeginDeferWindowPos		_BeginDeferWindowPos;
	extern pBeginPaint				_BeginPaint;
	extern pCallWindowProcW			_CallWindowProcW;
	//extern pCharLowerA				_CharLowerA;
	//extern pCharLowerBuffA			_CharLowerBuffA;
	//extern pCharUpperA			_CharUpperA;
	extern pCheckMenuItem			_CheckMenuItem;
	extern pClientToScreen			_ClientToScreen;
	extern pCloseClipboard			_CloseClipboard;
	extern pCreateMenu				_CreateMenu;
	extern pCreatePopupMenu			_CreatePopupMenu;
	extern pCreateWindowExW			_CreateWindowExW;
	extern pDefWindowProcW			_DefWindowProcW;
	extern pDeferWindowPos			_DeferWindowPos;
	//extern pDeleteMenu				_DeleteMenu;
	extern pDestroyIcon				_DestroyIcon;
	extern pDestroyMenu				_DestroyMenu;
	extern pDestroyWindow			_DestroyWindow;
	extern pDispatchMessageW		_DispatchMessageW;
	//extern pDrawEdge				_DrawEdge;
	extern pDrawFocusRect			_DrawFocusRect;
	extern pDrawIconEx				_DrawIconEx;
	//extern pDrawMenuBar				_DrawMenuBar;
	extern pDrawTextW				_DrawTextW;
	extern pEmptyClipboard			_EmptyClipboard;
	extern pEnableMenuItem			_EnableMenuItem;
	extern pEnableWindow			_EnableWindow;
	extern pEndDeferWindowPos		_EndDeferWindowPos;
	//extern pEndMenu					_EndMenu;
	extern pEndPaint				_EndPaint;
	//extern pEnumChildWindows		_EnumChildWindows;
	extern pExitWindowsEx			_ExitWindowsEx;
	extern pFillRect				_FillRect;
	extern pFindWindowW				_FindWindowW;
	extern pFlashWindow				_FlashWindow;
	extern pFrameRect				_FrameRect;
	//extern pGetClassLongPtrW		_GetClassLongPtrW;
	extern pGetClientRect			_GetClientRect;
	extern pGetClipboardData		_GetClipboardData;
	extern pGetCursorPos			_GetCursorPos;
	extern pGetDC					_GetDC;
	extern pGetDlgCtrlID			_GetDlgCtrlID;
	//extern pGetDlgItem				_GetDlgItem;
	extern pGetFocus				_GetFocus;
	extern pGetIconInfo				_GetIconInfo;
	extern pGetKeyState				_GetKeyState;
	//extern pGetMenuItemInfoW		_GetMenuItemInfoW;
	//extern pGetMessagePos			_GetMessagePos;
	extern pGetMessageW				_GetMessageW;
	extern pGetMonitorInfoW			_GetMonitorInfoW;
	extern pGetParent				_GetParent;
	extern pGetScrollInfo			_GetScrollInfo;
	//extern pGetSubMenu				_GetSubMenu;
	extern pGetSysColor				_GetSysColor;
	extern pGetSysColorBrush		_GetSysColorBrush;
	extern pGetSystemMetrics		_GetSystemMetrics;
	extern pGetWindow				_GetWindow;
	extern pGetWindowDC				_GetWindowDC;
	//extern pGetWindowLongW			_GetWindowLongW;
	extern pGetWindowLongPtrW		_GetWindowLongPtrW;
	extern pGetWindowRect			_GetWindowRect;
	extern pInsertMenuItemW			_InsertMenuItemW;
	extern pInvalidateRect			_InvalidateRect;
	//extern pIsCharAlphaNumericA		_IsCharAlphaNumericA;
	extern pIsClipboardFormatAvailable _IsClipboardFormatAvailable;
	extern pIsDialogMessageW		_IsDialogMessageW;
	extern pIsIconic				_IsIconic;
	extern pIsWindowEnabled			_IsWindowEnabled;
	extern pIsWindowVisible			_IsWindowVisible;
	extern pIsZoomed				_IsZoomed;
	extern pKillTimer				_KillTimer;
	extern pLoadCursorW				_LoadCursorW;
	extern pLoadIconW				_LoadIconW;
	extern pLoadImageW				_LoadImageW;
	extern pLockWorkStation			_LockWorkStation;
	extern pMapWindowPoints			_MapWindowPoints;
	//extern pMessageBoxA				_MessageBoxA;
	extern pMessageBoxW				_MessageBoxW;
	extern pMonitorFromRect			_MonitorFromRect;
	extern pMonitorFromWindow		_MonitorFromWindow;
	extern pOffsetRect				_OffsetRect;
	extern pOpenClipboard			_OpenClipboard;
	extern pPostMessageW			_PostMessageW;
	extern pPostQuitMessage			_PostQuitMessage;
	extern pRegisterClassExW		_RegisterClassExW;
	extern pRegisterClipboardFormatW	_RegisterClipboardFormatW;
	extern pRegisterWindowMessageW	_RegisterWindowMessageW;
	extern pReleaseCapture			_ReleaseCapture;
	extern pReleaseDC				_ReleaseDC;
	//extern pRemoveMenu				_RemoveMenu;
	extern pScreenToClient			_ScreenToClient;
	extern pScrollWindow			_ScrollWindow;
	extern pSendMessageA			_SendMessageA;
	extern pSendMessageW			_SendMessageW;
	extern pSendNotifyMessageW		_SendNotifyMessageW;
	extern pSetCapture				_SetCapture;
	//extern pSetClassLongPtrW		_SetClassLongPtrW;
	extern pSetClipboardData		_SetClipboardData;
	extern pSetCursor				_SetCursor;
	extern pSetFocus				_SetFocus;
	extern pSetForegroundWindow		_SetForegroundWindow;
	extern pSetLayeredWindowAttributes		_SetLayeredWindowAttributes;
	extern pSetMenu					_SetMenu;
	extern pSetMenuItemInfoW		_SetMenuItemInfoW;
	//extern pSetRect					_SetRect;
	extern pSetScrollInfo			_SetScrollInfo;
	extern pSetScrollPos			_SetScrollPos;
	extern pSetTimer				_SetTimer;
	//extern pSetWindowLongW			_SetWindowLongW;
	extern pSetWindowLongPtrW		_SetWindowLongPtrW;
	extern pSetWindowPos			_SetWindowPos;
	//extern pSetWindowTextW			_SetWindowTextW;
	extern pShowWindow				_ShowWindow;
	extern pSystemParametersInfoW	_SystemParametersInfoW;
	extern pTrackMouseEvent			_TrackMouseEvent;
	extern pTrackPopupMenu			_TrackPopupMenu;
	extern pTranslateMessage		_TranslateMessage;

	extern unsigned char user32_state;

	#define _CreateWindowW( lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam ) _CreateWindowExW( 0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam )

	bool InitializeUser32();
	bool UnInitializeUser32();

#endif

typedef enum
{
	MDT_EFFECTIVE_DPI = 0,
	MDT_ANGULAR_DPI = 1,
	MDT_RAW_DPI = 2,
	MDT_DEFAULT
} MONITOR_DPI_TYPE;

typedef UINT ( WINAPI *pGetDpiForWindow )( HWND hWnd );
typedef BOOL ( WINAPI *pSystemParametersInfoForDpi )( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni, UINT dpi );
typedef int ( WINAPI *pGetSystemMetricsForDpi )( int nIndex, UINT dpi );
//typedef BOOL ( WINAPI *pAdjustWindowRectExForDpi )( LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi );

typedef HRESULT ( WINAPI *pGetDpiForMonitor )( HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT *dpiX, UINT *dpiY );

//

extern pGetDpiForWindow				_GetDpiForWindow;
extern pSystemParametersInfoForDpi	_SystemParametersInfoForDpi;
extern pGetSystemMetricsForDpi		_GetSystemMetricsForDpi;
//extern pAdjustWindowRectExForDpi	_AdjustWindowRectExForDpi;

extern pGetDpiForMonitor			_GetDpiForMonitor;

UINT __GetDpiForWindow( HWND hWnd );
BOOL __SystemParametersInfoForDpi( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni, UINT dpi );
int __GetSystemMetricsForDpi( int nIndex, UINT dpi );
//BOOL __AdjustWindowRectExForDpi( LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi );

bool InitializeDPIFunctions();
bool UnInitializeDPIFunctions();

#define _SCALE_( x, var ) MulDiv( ( x ), current_##var, USER_DEFAULT_SCREEN_DPI )
#define _SCALE2_( x, var ) MulDiv( ( x ), current_##var, last_##var )
#define _UNSCALE_( x, var ) MulDiv( ( x ), USER_DEFAULT_SCREEN_DPI, current_##var )

#endif
