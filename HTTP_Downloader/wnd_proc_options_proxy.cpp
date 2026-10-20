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

#include "globals.h"
#include "utilities.h"
#include "string_tables.h"
#include "file_operations.h"
#include "doublylinkedlist.h"
#include "options.h"
#include "wnd_proc.h"
#include "cmessagebox.h"
#include "lite_gdi32.h"

// Proxy tab layout (Options window).
#define LIST_PROXY			1001
#define BTN_PROXY_ADD		1002
#define BTN_PROXY_EDIT		1003
#define BTN_PROXY_REMOVE	1004
#define COMBO_PROXY_SELECT	1005
#define STATIC_PROXY_SELECT	1006

// Proxy edit dialog controls.
#define EDIT_DLG_CANCEL		1011
#define EDIT_DLG_SAVE		1012
#define COMBO_PROXY_TYPE	1013
#define BTN_TYPE_HOST		1014
#define BTN_TYPE_IP			1015
#define EDIT_HOSTNAME		1016
#define EDIT_IP_ADDRESS		1017
#define EDIT_PORT			1018
#define BTN_USE_AUTH		1019
#define EDIT_USERNAME		1020
#define EDIT_PASSWORD		1021
#define BTN_RESOLVE_DOMAIN	1022
#define EDIT_IDENT			1023
#define EDIT_ALIAS			1024

HWND g_hWnd_proxy_list = NULL;
HWND g_hWnd_btn_proxy_add = NULL;
HWND g_hWnd_btn_proxy_edit = NULL;
HWND g_hWnd_btn_proxy_remove = NULL;
HWND g_hWnd_combo_proxy_select = NULL;
HWND g_hWnd_static_proxy_select = NULL;

HWND g_hWnd_proxy_edit = NULL;		// Proxy edit dialog.

// The dialog uses its OWN font instance. Sharing the global hFont_options font object
// across a second window made GDI realize/rescale that shared object with degraded text
// rendering (grayscale, no ClearType subpixels) for every window afterwards.
HFONT g_hFont_proxy_edit = NULL;

PROXY_ENTRY *g_edit_proxy = NULL;	// Working copy being edited (or a fresh entry for Add).
bool g_edit_new = false;

void GetProxyTypeText( unsigned char type, wchar_t *buf, unsigned int buf_size )
{
	switch ( type )
	{
		case PROXY_TYPE_HTTPS: { __snwprintf( buf, buf_size, L"HTTPS" ); } break;
		case PROXY_TYPE_SOCKS4: { __snwprintf( buf, buf_size, L"SOCKS v4" ); } break;
		case PROXY_TYPE_SOCKS5: { __snwprintf( buf, buf_size, L"SOCKS v5" ); } break;
		default: { __snwprintf( buf, buf_size, L"HTTP" ); } break;
	}
}

void GetProxyAddressText( PROXY_ENTRY *pe, wchar_t *buf, unsigned int buf_size )
{
	if ( pe == NULL )
	{
		buf[ 0 ] = 0;
		return;
	}

	if ( pe->address_type == 1 )	// IPv4 address.
	{
		// ip_address is stored in IPM_GETADDRESS format (field 1 = high byte); on a
		// little-endian machine the first field lives in the last memory byte.
		unsigned char *ip = ( unsigned char * )&pe->ip_address;
		__snwprintf( buf, buf_size, L"%lu.%lu.%lu.%lu", ip[ 3 ], ip[ 2 ], ip[ 1 ], ip[ 0 ] );
	}
	else
	{
		__snwprintf( buf, buf_size, L"%s", ( pe->hostname != NULL ? pe->hostname : L"" ) );
	}
}

void RefreshProxyList( void )
{
	if ( g_hWnd_proxy_list == NULL )
	{
		return;
	}

	_SendMessageW( g_hWnd_proxy_list, WM_SETREDRAW, FALSE, 0 );
	_SendMessageW( g_hWnd_proxy_list, LVM_DELETEALLITEMS, 0, 0 );

	LVITEMW lvi;
	_memzero( &lvi, sizeof( LVITEMW ) );
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 0;

	wchar_t buf[ 512 ];
	wchar_t addr[ 512 ];

	DoublyLinkedList *node = g_proxy_list;
	int index = 0;

	while ( node != NULL )
	{
		PROXY_ENTRY *pe = ( PROXY_ENTRY * )node->data;

		if ( pe != NULL )
		{
			GetProxyTypeText( pe->type, buf, 512 );
			lvi.pszText = buf;
			lvi.iItem = index;
			lvi.iSubItem = 0;	// Must be 0 for insertion; it's left at 3 by the sub-item updates below.
			_SendMessageW( g_hWnd_proxy_list, LVM_INSERTITEMW, 0, ( LPARAM )&lvi );

			GetProxyAddressText( pe, addr, 512 );
			__snwprintf( buf, 512, L"%s", addr );
			lvi.iSubItem = 1;
			_SendMessageW( g_hWnd_proxy_list, LVM_SETITEMTEXTW, ( WPARAM )index, ( LPARAM )&lvi );

			__snwprintf( buf, 512, L"%hu", pe->port );
			lvi.iSubItem = 2;
			_SendMessageW( g_hWnd_proxy_list, LVM_SETITEMTEXTW, ( WPARAM )index, ( LPARAM )&lvi );

			__snwprintf( buf, 512, L"%s", ( pe->use_authentication ? L"Yes" : L"No" ) );
			lvi.iSubItem = 3;
			_SendMessageW( g_hWnd_proxy_list, LVM_SETITEMTEXTW, ( WPARAM )index, ( LPARAM )&lvi );

			++index;
		}

		node = node->next;
	}

	_SendMessageW( g_hWnd_proxy_list, WM_SETREDRAW, TRUE, 0 );

	// Restore the selection if possible.
	if ( cfg_selected_proxy > 0 && cfg_selected_proxy <= index )
	{
		LVITEMW lvis;
		_memzero( &lvis, sizeof( LVITEMW ) );
		lvis.state = LVIS_SELECTED;
		lvis.stateMask = LVIS_SELECTED;
		_SendMessageW( g_hWnd_proxy_list, LVM_SETITEMSTATE, ( WPARAM )( cfg_selected_proxy - 1 ), ( LPARAM )&lvis );
	}
}

void RefreshProxyCombo( void )
{
	if ( g_hWnd_combo_proxy_select == NULL )
	{
		return;
	}

	_SendMessageW( g_hWnd_combo_proxy_select, CB_RESETCONTENT, 0, 0 );

	_SendMessageW( g_hWnd_combo_proxy_select, CB_ADDSTRING, 0, ( LPARAM )L"No proxy" );

	wchar_t buf[ 512 ];
	wchar_t addr[ 512 ];
	wchar_t type_buf[ 16 ];

	DoublyLinkedList *node = g_proxy_list;

	while ( node != NULL )
	{
		PROXY_ENTRY *pe = ( PROXY_ENTRY * )node->data;

		if ( pe != NULL )
		{
			GetProxyTypeText( pe->type, type_buf, 16 );
			GetProxyAddressText( pe, addr, 512 );

			// Show the alias when present, else "type | address:port".
			if ( pe->alias != NULL && pe->alias[ 0 ] != 0 )
			{
				__snwprintf( buf, 512, L"%s", pe->alias );
			}
			else
			{
				__snwprintf( buf, 512, L"%s | %s:%hu", type_buf, addr, pe->port );
			}

			_SendMessageW( g_hWnd_combo_proxy_select, CB_ADDSTRING, 0, ( LPARAM )buf );
		}

		node = node->next;
	}

	_SendMessageW( g_hWnd_combo_proxy_select, CB_SETCURSEL, cfg_selected_proxy, 0 );
}

unsigned char GetProxyListSelection( void )
{
	unsigned char index = 0;

	if ( g_hWnd_proxy_list != NULL )
	{
		int sel = ( int )_SendMessageW( g_hWnd_proxy_list, LVM_GETNEXTITEM, ( WPARAM )-1, LVNI_SELECTED );
		if ( sel >= 0 && sel < 0xFF )
		{
			index = ( unsigned char )( sel + 1 );
		}
	}

	return index;	// 1-based. 0 = nothing selected.
}

void ApplyProxySelection( unsigned char selection )
{
	cfg_selected_proxy = selection;
	g_active_proxy = NULL;

	if ( selection > 0 )
	{
		DoublyLinkedList *node = g_proxy_list;
		for ( unsigned char i = 1; i < selection && node != NULL; ++i )
		{
			node = node->next;
		}

		if ( node != NULL )
		{
			g_active_proxy = ( PROXY_ENTRY * )node->data;
		}
		else
		{
			cfg_selected_proxy = 0;
		}
	}

	UpdateActiveProxyRuntimeFields();
	SaveProxyList();
}

void UpdateProxyEditControls( HWND hWnd )
{
	if ( g_edit_proxy == NULL )
	{
		return;
	}

	bool is_socks4 = ( g_edit_proxy->type == PROXY_TYPE_SOCKS4 );
	bool is_socks5 = ( g_edit_proxy->type == PROXY_TYPE_SOCKS5 );

	// The hostname and IP address controls overlap; show the one for the selected address
	// type (EnableWindow alone has no effect on a hidden control).
	_ShowWindow( GetDlgItem( hWnd, EDIT_HOSTNAME ), ( g_edit_proxy->address_type == 0 ? SW_SHOW : SW_HIDE ) );
	_ShowWindow( GetDlgItem( hWnd, EDIT_IP_ADDRESS ), ( g_edit_proxy->address_type == 1 ? SW_SHOW : SW_HIDE ) );
	_EnableWindow( GetDlgItem( hWnd, EDIT_HOSTNAME ), ( g_edit_proxy->address_type == 0 ? TRUE : FALSE ) );
	_EnableWindow( GetDlgItem( hWnd, EDIT_IP_ADDRESS ), ( g_edit_proxy->address_type == 1 ? TRUE : FALSE ) );

	// SOCKS4 has no authentication mechanism; its user ID is the ident username.
	_EnableWindow( GetDlgItem( hWnd, BTN_USE_AUTH ), ( is_socks4 ? FALSE : TRUE ) );
	_EnableWindow( GetDlgItem( hWnd, EDIT_USERNAME ), ( g_edit_proxy->use_authentication && !is_socks4 ? TRUE : FALSE ) );
	_EnableWindow( GetDlgItem( hWnd, EDIT_PASSWORD ), ( g_edit_proxy->use_authentication && !is_socks4 ? TRUE : FALSE ) );

	_EnableWindow( GetDlgItem( hWnd, BTN_RESOLVE_DOMAIN ), ( is_socks4 || is_socks5 ? TRUE : FALSE ) );
	_EnableWindow( GetDlgItem( hWnd, EDIT_IDENT ), ( is_socks4 ? TRUE : FALSE ) );
}

bool ValidateProxyEditControls( HWND hWnd )
{
	wchar_t buf[ 512 ];

	// Alias is required.
	_SendMessageW( GetDlgItem( hWnd, EDIT_ALIAS ), WM_GETTEXT, 512, ( LPARAM )buf );
	if ( buf[ 0 ] == 0 )
	{
		return false;
	}

	// Alias must be unique (case-insensitive). Skip the entry being edited.
	unsigned char edit_selection = ( g_edit_new ? 0 : GetProxyListSelection() );
	unsigned char idx = 0;
	DoublyLinkedList *node = g_proxy_list;

	while ( node != NULL )
	{
		++idx;
		if ( !g_edit_new && idx == edit_selection )
		{
			node = node->next;
			continue;
		}

		PROXY_ENTRY *pe = ( PROXY_ENTRY * )node->data;
		if ( pe != NULL && pe->alias != NULL && lstrcmpiW( pe->alias, buf ) == 0 )
		{
			return false;
		}

		node = node->next;
	}

	bool is_hostname = ( _SendMessageW( GetDlgItem( hWnd, BTN_TYPE_HOST ), BM_GETCHECK, 0, 0 ) == BST_CHECKED ? true : false );

	if ( is_hostname )
	{
		_SendMessageW( GetDlgItem( hWnd, EDIT_HOSTNAME ), WM_GETTEXT, 512, ( LPARAM )buf );
		if ( buf[ 0 ] == 0 )
		{
			return false;
		}
	}
	else if ( _SendMessageW( GetDlgItem( hWnd, EDIT_IP_ADDRESS ), IPM_ISBLANK, 0, 0 ) != FALSE )
	{
		return false;
	}

	_SendMessageW( GetDlgItem( hWnd, EDIT_PORT ), WM_GETTEXT, 6, ( LPARAM )buf );
	unsigned long port = _wcstoul( buf, NULL, 10 );
	if ( port == 0 || port > 65535 )
	{
		return false;
	}

	return true;
}

void FillProxyEditControls( HWND hWnd )
{
	if ( g_edit_proxy == NULL )
	{
		return;
	}

	_SendMessageW( GetDlgItem( hWnd, COMBO_PROXY_TYPE ), CB_SETCURSEL, g_edit_proxy->type, 0 );

	_SendMessageW( GetDlgItem( hWnd, BTN_TYPE_HOST ), BM_SETCHECK, ( g_edit_proxy->address_type == 0 ? BST_CHECKED : BST_UNCHECKED ), 0 );
	_SendMessageW( GetDlgItem( hWnd, BTN_TYPE_IP ), BM_SETCHECK, ( g_edit_proxy->address_type == 1 ? BST_CHECKED : BST_UNCHECKED ), 0 );

	_SendMessageW( GetDlgItem( hWnd, EDIT_ALIAS ), WM_SETTEXT, 0, ( LPARAM )( g_edit_proxy->alias != NULL ? g_edit_proxy->alias : L"" ) );
	_SendMessageW( GetDlgItem( hWnd, EDIT_HOSTNAME ), WM_SETTEXT, 0, ( LPARAM )( g_edit_proxy->hostname != NULL ? g_edit_proxy->hostname : L"" ) );

	// ip_address is stored in IPM_GETADDRESS format, which is exactly what IPM_SETADDRESS expects.
	_SendMessageW( GetDlgItem( hWnd, EDIT_IP_ADDRESS ), IPM_SETADDRESS, 0, g_edit_proxy->ip_address );

	wchar_t port[ 6 ];
	__snwprintf( port, 6, L"%hu", g_edit_proxy->port );
	_SendMessageW( GetDlgItem( hWnd, EDIT_PORT ), WM_SETTEXT, 0, ( LPARAM )port );

	_SendMessageW( GetDlgItem( hWnd, BTN_USE_AUTH ), BM_SETCHECK, ( g_edit_proxy->use_authentication ? BST_CHECKED : BST_UNCHECKED ), 0 );
	_SendMessageW( GetDlgItem( hWnd, EDIT_USERNAME ), WM_SETTEXT, 0, ( LPARAM )( g_edit_proxy->username != NULL ? g_edit_proxy->username : L"" ) );
	_SendMessageW( GetDlgItem( hWnd, EDIT_PASSWORD ), WM_SETTEXT, 0, ( LPARAM )( g_edit_proxy->password != NULL ? g_edit_proxy->password : L"" ) );

	_SendMessageW( GetDlgItem( hWnd, BTN_RESOLVE_DOMAIN ), BM_SETCHECK, ( g_edit_proxy->resolve_domain_names ? BST_CHECKED : BST_UNCHECKED ), 0 );
	_SendMessageW( GetDlgItem( hWnd, EDIT_IDENT ), WM_SETTEXT, 0, ( LPARAM )( g_edit_proxy->ident_username != NULL ? g_edit_proxy->ident_username : L"" ) );

	UpdateProxyEditControls( hWnd );

	_EnableWindow( GetDlgItem( hWnd, EDIT_DLG_SAVE ), ( ValidateProxyEditControls( hWnd ) ? TRUE : FALSE ) );
}

void UpdateSaveButtonState( HWND hWnd )
{
	_EnableWindow( GetDlgItem( hWnd, EDIT_DLG_SAVE ), ( ValidateProxyEditControls( hWnd ) ? TRUE : FALSE ) );
}

bool ReadProxyEditControls( HWND hWnd, PROXY_ENTRY **pe_out )
{
	if ( g_edit_proxy == NULL )
	{
		return false;
	}

	int type = ( int )_SendMessageW( GetDlgItem( hWnd, COMBO_PROXY_TYPE ), CB_GETCURSEL, 0, 0 );
	if ( type < 0 )
	{
		return false;
	}

	wchar_t buf[ 512 ];

	_SendMessageW( GetDlgItem( hWnd, EDIT_HOSTNAME ), WM_GETTEXT, 512, ( LPARAM )buf );

	// A hostname or an IP address must be set.
	bool is_hostname = ( _SendMessageW( GetDlgItem( hWnd, BTN_TYPE_HOST ), BM_GETCHECK, 0, 0 ) == BST_CHECKED ? true : false );
	if ( ( is_hostname && buf[ 0 ] == 0 ) ||
		 ( !is_hostname && _SendMessageW( GetDlgItem( hWnd, EDIT_IP_ADDRESS ), IPM_ISBLANK, 0, 0 ) != FALSE ) )
	{
		return false;
	}

	// Duplicate the hostname NOW: buf is reused for the port/credentials below.
	wchar_t *hostname = ( is_hostname && buf[ 0 ] != 0 ? GlobalStrDupW( buf ) : NULL );

	unsigned long ip_address = 0;
	if ( !is_hostname )
	{
		_SendMessageW( GetDlgItem( hWnd, EDIT_IP_ADDRESS ), IPM_GETADDRESS, 0, ( LPARAM )&ip_address );
	}

	_SendMessageW( GetDlgItem( hWnd, EDIT_PORT ), WM_GETTEXT, 6, ( LPARAM )buf );
	unsigned long port = _wcstoul( buf, NULL, 10 );
	if ( port == 0 || port > 65535 )
	{
		return false;
	}

	bool use_authentication = ( _SendMessageW( GetDlgItem( hWnd, BTN_USE_AUTH ), BM_GETCHECK, 0, 0 ) == BST_CHECKED ? true : false );

	// SOCKS4 has no authentication mechanism.
	if ( type == PROXY_TYPE_SOCKS4 )
	{
		use_authentication = false;
	}

	wchar_t alias[ 512 ];
	_SendMessageW( GetDlgItem( hWnd, EDIT_ALIAS ), WM_GETTEXT, 512, ( LPARAM )alias );
	if ( alias[ 0 ] == 0 )
	{
		return false;	// Alias is required.
	}

	// Reuse buf to keep the stack frame small (large frames would pull in __chkstk,
	// which is unavailable in the no-CRT Release build).
	_SendMessageW( GetDlgItem( hWnd, EDIT_USERNAME ), WM_GETTEXT, 512, ( LPARAM )buf );
	wchar_t *username = ( buf[ 0 ] != 0 ? GlobalStrDupW( buf ) : NULL );

	_SendMessageW( GetDlgItem( hWnd, EDIT_PASSWORD ), WM_GETTEXT, 512, ( LPARAM )buf );
	wchar_t *password = ( buf[ 0 ] != 0 ? GlobalStrDupW( buf ) : NULL );

	bool resolve_domain_names = ( _SendMessageW( GetDlgItem( hWnd, BTN_RESOLVE_DOMAIN ), BM_GETCHECK, 0, 0 ) == BST_CHECKED ? true : false );

	// The ident user ID is only meaningful for SOCKS4.
	_SendMessageW( GetDlgItem( hWnd, EDIT_IDENT ), WM_GETTEXT, 512, ( LPARAM )buf );
	wchar_t *ident_username = ( type == PROXY_TYPE_SOCKS4 && buf[ 0 ] != 0 ? GlobalStrDupW( buf ) : NULL );

	// Update the working copy.
	GlobalFree( g_edit_proxy->alias );
	GlobalFree( g_edit_proxy->hostname );
	GlobalFree( g_edit_proxy->username );
	GlobalFree( g_edit_proxy->password );
	GlobalFree( g_edit_proxy->ident_username );
	GlobalFree( g_edit_proxy->punycode_hostname );
	GlobalFree( g_edit_proxy->auth_username_utf8 );
	GlobalFree( g_edit_proxy->auth_password_utf8 );
	GlobalFree( g_edit_proxy->ident_username_utf8 );
	GlobalFree( g_edit_proxy->auth_key );

	_memzero( g_edit_proxy, sizeof( PROXY_ENTRY ) );

	g_edit_proxy->alias = ( alias[ 0 ] != 0 ? GlobalStrDupW( alias ) : NULL );
	g_edit_proxy->type = ( unsigned char )type;
	g_edit_proxy->address_type = ( is_hostname ? 0 : 1 );
	g_edit_proxy->ip_address = ip_address;
	g_edit_proxy->hostname = hostname;
	g_edit_proxy->port = ( unsigned short )port;
	g_edit_proxy->use_authentication = use_authentication;
	g_edit_proxy->username = username;
	g_edit_proxy->password = password;
	g_edit_proxy->resolve_domain_names = resolve_domain_names;
	g_edit_proxy->ident_username = ident_username;

	*pe_out = g_edit_proxy;
	g_edit_proxy = NULL;	// Ownership transferred.

	return true;
}

void OpenProxyEditDialog( bool new_entry, unsigned char edit_index )
{
	if ( g_hWnd_proxy_edit != NULL )
	{
		_SetForegroundWindow( g_hWnd_proxy_edit );
		return;
	}

	// Build the working copy.
	if ( new_entry )
	{
		g_edit_proxy = ( PROXY_ENTRY * )GlobalAlloc( GPTR, sizeof( PROXY_ENTRY ) );
		if ( g_edit_proxy != NULL )
		{
			g_edit_proxy->type = PROXY_TYPE_HTTP;
			g_edit_proxy->address_type = 0;
			g_edit_proxy->ip_address = 0x7F000001;	// 127.0.0.1 (IPM_GETADDRESS format).
			g_edit_proxy->hostname = GlobalStrDupW( L"localhost" );
			g_edit_proxy->port = 8080;
		}
		g_edit_new = true;
	}
	else if ( edit_index > 0 )
	{
		DoublyLinkedList *node = g_proxy_list;
		for ( unsigned char i = 1; i < edit_index && node != NULL; ++i )
		{
			node = node->next;
		}

		if ( node != NULL )
		{
			PROXY_ENTRY *src = ( PROXY_ENTRY * )node->data;
			g_edit_proxy = ( PROXY_ENTRY * )GlobalAlloc( GPTR, sizeof( PROXY_ENTRY ) );

			if ( g_edit_proxy != NULL )
			{
				g_edit_proxy->alias = ( src->alias != NULL ? GlobalStrDupW( src->alias ) : NULL );
				g_edit_proxy->type = src->type;
				g_edit_proxy->address_type = src->address_type;
				g_edit_proxy->ip_address = src->ip_address;
				g_edit_proxy->hostname = ( src->hostname != NULL ? GlobalStrDupW( src->hostname ) : NULL );
				g_edit_proxy->port = src->port;
				g_edit_proxy->use_authentication = src->use_authentication;
				g_edit_proxy->username = ( src->username != NULL ? GlobalStrDupW( src->username ) : NULL );
				g_edit_proxy->password = ( src->password != NULL ? GlobalStrDupW( src->password ) : NULL );
				g_edit_proxy->resolve_domain_names = src->resolve_domain_names;
				g_edit_proxy->ident_username = ( src->ident_username != NULL ? GlobalStrDupW( src->ident_username ) : NULL );
			}
		}
		g_edit_new = false;
	}

	if ( g_edit_proxy == NULL )
	{
		return;
	}

	// Center the window over the options window. Compute the position BEFORE creating the
	// window so it's born on the same monitor as the options window (creating at 0,0 and
	// then moving can bounce the window across monitors and trigger DPI change churn).
	int width = _SCALE_O_( 380 );
	int height = _SCALE_O_( 430 );

	RECT rc_options; _GetWindowRect( g_hWnd_options, &rc_options );
	int options_width = rc_options.right - rc_options.left;
	int options_height = rc_options.bottom - rc_options.top;

	int pos_x = rc_options.left + ( ( options_width - width ) / 2 );
	int pos_y = rc_options.top + ( ( options_height - height ) / 2 );

	// Independent font instance for this dialog (see g_hFont_proxy_edit comment).
	// Create it BEFORE the window so WM_CREATE can assign it to the controls.
	if ( g_hFont_proxy_edit == NULL )
	{
		g_hFont_proxy_edit = UpdateFont( current_dpi_options );
	}

	g_hWnd_proxy_edit = _CreateWindowExW( 0, L"class_proxy_edit", ( new_entry ? L"Add Proxy" : L"Edit Proxy" ), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, pos_x, pos_y, width, height, g_hWnd_options, NULL, NULL, NULL );
	if ( g_hWnd_proxy_edit == NULL )
	{
		FreeProxyEntry( g_edit_proxy );
		g_edit_proxy = NULL;
	}
	else
	{
		// An owned window always stays above its owner, which gives the modal behavior
		// we want. We deliberately do NOT disable the options window here: the
		// disable/enable cycle made the options window redraw its text without ClearType
		// (bold/black text) after the dialog was closed.
	}
}

LRESULT CALLBACK ProxyEditWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_CREATE:
		{
			// The window is positioned at creation time (OpenProxyEditDialog), so no
			// repositioning is needed here.

			HWND hWnd_combo_type = _CreateWindowW( WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 40 ), _SCALE_O_( 345 ), _SCALE_O_( 100 ), hWnd, ( HMENU )COMBO_PROXY_TYPE, NULL, NULL );
			_SendMessageW( hWnd_combo_type, CB_ADDSTRING, 0, ( LPARAM )L"HTTP" );
			_SendMessageW( hWnd_combo_type, CB_ADDSTRING, 0, ( LPARAM )L"HTTPS" );
			_SendMessageW( hWnd_combo_type, CB_ADDSTRING, 0, ( LPARAM )L"SOCKS v4" );
			_SendMessageW( hWnd_combo_type, CB_ADDSTRING, 0, ( LPARAM )L"SOCKS v5" );

			_CreateWindowW( WC_STATIC, L"Alias:", WS_CHILD | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 13 ), _SCALE_O_( 40 ), _SCALE_O_( 17 ), hWnd, NULL, NULL, NULL );
			_CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_AUTOHSCROLL | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 55 ), _SCALE_O_( 10 ), _SCALE_O_( 300 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_ALIAS, NULL, NULL );

			_CreateWindowW( WC_BUTTON, L"Hostname", BS_AUTORADIOBUTTON | WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 70 ), _SCALE_O_( 110 ), _SCALE_O_( 20 ), hWnd, ( HMENU )BTN_TYPE_HOST, NULL, NULL );
			_CreateWindowW( WC_BUTTON, L"IPv4 address", BS_AUTORADIOBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 130 ), _SCALE_O_( 70 ), _SCALE_O_( 110 ), _SCALE_O_( 20 ), hWnd, ( HMENU )BTN_TYPE_IP, NULL, NULL );

			_CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_AUTOHSCROLL | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 95 ), _SCALE_O_( 345 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_HOSTNAME, NULL, NULL );
			_SendMessageW( GetDlgItem( hWnd, EDIT_HOSTNAME ), EM_LIMITTEXT, MAX_DOMAIN_LENGTH, 0 );

			_CreateWindowExW( WS_EX_CLIENTEDGE, WC_IPADDRESS, NULL, WS_CHILD | WS_TABSTOP, _SCALE_O_( 10 ), _SCALE_O_( 95 ), _SCALE_O_( 345 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_IP_ADDRESS, NULL, NULL );

			_CreateWindowW( WC_STATIC, L"Port:", WS_CHILD | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 128 ), _SCALE_O_( 40 ), _SCALE_O_( 17 ), hWnd, NULL, NULL, NULL );
			_CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_AUTOHSCROLL | ES_CENTER | ES_NUMBER | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 55 ), _SCALE_O_( 125 ), _SCALE_O_( 80 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_PORT, NULL, NULL );
			_SendMessageW( GetDlgItem( hWnd, EDIT_PORT ), EM_LIMITTEXT, 5, 0 );

			_CreateWindowW( WC_BUTTON, L"Use authentication", BS_AUTOCHECKBOX | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 160 ), _SCALE_O_( 170 ), _SCALE_O_( 20 ), hWnd, ( HMENU )BTN_USE_AUTH, NULL, NULL );

			_CreateWindowW( WC_STATIC, L"Username:", WS_CHILD | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 188 ), _SCALE_O_( 70 ), _SCALE_O_( 17 ), hWnd, NULL, NULL, NULL );
			_CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_AUTOHSCROLL | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 85 ), _SCALE_O_( 185 ), _SCALE_O_( 270 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_USERNAME, NULL, NULL );

			_CreateWindowW( WC_STATIC, L"Password:", WS_CHILD | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 218 ), _SCALE_O_( 70 ), _SCALE_O_( 17 ), hWnd, NULL, NULL, NULL );
			_CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_PASSWORD | ES_AUTOHSCROLL | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 85 ), _SCALE_O_( 215 ), _SCALE_O_( 270 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_PASSWORD, NULL, NULL );

			_CreateWindowW( WC_BUTTON, L"Resolve domain names", BS_AUTOCHECKBOX | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 248 ), _SCALE_O_( 180 ), _SCALE_O_( 20 ), hWnd, ( HMENU )BTN_RESOLVE_DOMAIN, NULL, NULL );

			_CreateWindowW( WC_STATIC, L"Ident username:", WS_CHILD | WS_VISIBLE, _SCALE_O_( 10 ), _SCALE_O_( 276 ), _SCALE_O_( 110 ), _SCALE_O_( 17 ), hWnd, NULL, NULL, NULL );
			_CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_AUTOHSCROLL | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 125 ), _SCALE_O_( 273 ), _SCALE_O_( 230 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_IDENT, NULL, NULL );

			_CreateWindowW( WC_BUTTON, ST_V_Save, BS_PUSHBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 190 ), _SCALE_O_( 330 ), _SCALE_O_( 80 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_DLG_SAVE, NULL, NULL );
			_CreateWindowW( WC_BUTTON, ST_V_Cancel, BS_PUSHBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE, _SCALE_O_( 275 ), _SCALE_O_( 330 ), _SCALE_O_( 80 ), _SCALE_O_( 23 ), hWnd, ( HMENU )EDIT_DLG_CANCEL, NULL, NULL );

			if ( g_hFont_proxy_edit != NULL )
			{
				HWND hWnd_child = _GetWindow( hWnd, GW_CHILD );
				while ( hWnd_child != NULL )
				{
					_SendMessageW( hWnd_child, WM_SETFONT, ( WPARAM )g_hFont_proxy_edit, TRUE );

					// Some composite controls (IP address, combo box) ignore WM_SETFONT on
					// themselves; their inner child controls need the font individually.
					EnumChildWindows( hWnd_child, EnumChildFontProc, ( LPARAM )g_hFont_proxy_edit );

					hWnd_child = _GetWindow( hWnd_child, GW_HWNDNEXT );
				}
			}

			FillProxyEditControls( hWnd );

			_ShowWindow( hWnd, SW_SHOWNORMAL );

			return 0;
		}
		break;

		case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
				case COMBO_PROXY_TYPE:
				{
					if ( HIWORD( wParam ) == CBN_SELCHANGE && g_edit_proxy != NULL )
					{
						int type = ( int )_SendMessageW( GetDlgItem( hWnd, COMBO_PROXY_TYPE ), CB_GETCURSEL, 0, 0 );
						if ( type >= 0 )
						{
							g_edit_proxy->type = ( unsigned char )type;
							UpdateProxyEditControls( hWnd );
							UpdateSaveButtonState( hWnd );
						}
					}
				}
				break;

				case BTN_TYPE_HOST:
				case BTN_TYPE_IP:
				{
					if ( g_edit_proxy != NULL )
					{
						g_edit_proxy->address_type = ( LOWORD( wParam ) == BTN_TYPE_HOST ? 0 : 1 );
						UpdateProxyEditControls( hWnd );
						UpdateSaveButtonState( hWnd );
					}
				}
				break;

				case BTN_USE_AUTH:
				{
					if ( g_edit_proxy != NULL )
					{
						g_edit_proxy->use_authentication = ( _SendMessageW( GetDlgItem( hWnd, BTN_USE_AUTH ), BM_GETCHECK, 0, 0 ) == BST_CHECKED ? true : false );
						UpdateProxyEditControls( hWnd );
					}
				}
				break;

				case EDIT_ALIAS:
				case EDIT_HOSTNAME:
				case EDIT_PORT:
				{
					if ( HIWORD( wParam ) == EN_CHANGE )
					{
						UpdateSaveButtonState( hWnd );
					}
				}
				break;

				case EDIT_DLG_SAVE:
				{
					PROXY_ENTRY *pe = NULL;
					if ( ReadProxyEditControls( hWnd, &pe ) && pe != NULL )
					{
						// New entry: append to the list. Existing entry: replace in place.
						if ( g_edit_new )
						{
							DoublyLinkedList *node = DLL_CreateNode( pe );
							if ( node != NULL )
							{
								DLL_AddNode( &g_proxy_list, node, -1 );
							}
							else
							{
								FreeProxyEntry( pe );
							}
						}
						else
						{
							// The edit dialog was opened with a copy; find the original by
							// position. The list hasn't changed while the dialog was open.
							DoublyLinkedList *node = g_proxy_list;
							for ( unsigned char i = 1; i < GetProxyListSelection() && node != NULL; ++i )
							{
								node = node->next;
							}

							if ( node != NULL )
							{
								FreeProxyEntry( ( PROXY_ENTRY * )node->data );
								node->data = pe;
							}
							else
							{
								FreeProxyEntry( pe );
							}
						}

						// If the edited entry was the active one, refresh its runtime fields.
						if ( cfg_selected_proxy > 0 && cfg_selected_proxy == GetProxyListSelection() )
						{
							ApplyProxySelection( cfg_selected_proxy );
						}
						else
						{
							SaveProxyList();
						}

						RefreshProxyList();
						RefreshProxyCombo();

						// Adding or editing an entry changes the saved options.
						_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );

						_SendMessageW( hWnd, WM_CLOSE, 0, 0 );
					}
				}
				break;

				case EDIT_DLG_CANCEL:
				{
					_SendMessageW( hWnd, WM_CLOSE, 0, 0 );
				}
				break;
			}

			return 0;
		}
		break;

		case WM_NOTIFY:
		{
			// IP address field changed.
			if ( ( ( LPNMHDR )lParam )->code == IPN_FIELDCHANGED &&
				 ( ( LPNMHDR )lParam )->idFrom == EDIT_IP_ADDRESS )
			{
				UpdateSaveButtonState( hWnd );
			}

			return 0;
		}
		break;

		case WM_CLOSE:
		{
			// NOTE: Do NOT force a full invalidate of the options window here. Doing so made
			// every control in the options window repaint without ClearType (bold/black text).
			_DestroyWindow( hWnd );

			return 0;
		}
		break;

		case WM_DESTROY:
		{
			if ( g_edit_proxy != NULL )
			{
				FreeProxyEntry( g_edit_proxy );
				g_edit_proxy = NULL;
			}

			// Release this dialog's own font (the controls are gone by now).
			if ( g_hFont_proxy_edit != NULL )
			{
				_DeleteObject( g_hFont_proxy_edit );
				g_hFont_proxy_edit = NULL;
			}

			g_hWnd_proxy_edit = NULL;

			return _DefWindowProcW( hWnd, msg, wParam, lParam );
		}
		break;

		default:
		{
			return _DefWindowProcW( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return TRUE;
}

LRESULT CALLBACK ProxyTabWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_CREATE:
		{
			RECT rc;
			_GetClientRect( hWnd, &rc );

			g_hWnd_proxy_list = _CreateWindowExW( WS_EX_CLIENTEDGE, WC_LISTVIEWW, NULL, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 10, 10, rc.right - 20, rc.bottom - 85, hWnd, ( HMENU )LIST_PROXY, NULL, NULL );
			_SendMessageW( g_hWnd_proxy_list, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER );

			LVCOLUMNW lvc;
			_memzero( &lvc, sizeof( LVCOLUMNW ) );
			lvc.mask = LVCF_TEXT | LVCF_WIDTH;

			lvc.pszText = L"Protocol";
			lvc.cx = 80;
			_SendMessageW( g_hWnd_proxy_list, LVM_INSERTCOLUMNW, 0, ( LPARAM )&lvc );

			lvc.pszText = L"Address/Hostname";
			lvc.cx = 260;
			_SendMessageW( g_hWnd_proxy_list, LVM_INSERTCOLUMNW, 1, ( LPARAM )&lvc );

			lvc.pszText = L"Port";
			lvc.cx = 60;
			_SendMessageW( g_hWnd_proxy_list, LVM_INSERTCOLUMNW, 2, ( LPARAM )&lvc );

			lvc.pszText = L"Authentication";
			lvc.cx = 100;
			_SendMessageW( g_hWnd_proxy_list, LVM_INSERTCOLUMNW, 3, ( LPARAM )&lvc );

			g_hWnd_btn_proxy_add = _CreateWindowW( WC_BUTTON, L"Add", BS_PUSHBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 10, rc.bottom - 65, 80, 23, hWnd, ( HMENU )BTN_PROXY_ADD, NULL, NULL );
			g_hWnd_btn_proxy_edit = _CreateWindowW( WC_BUTTON, L"Edit", BS_PUSHBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 95, rc.bottom - 65, 80, 23, hWnd, ( HMENU )BTN_PROXY_EDIT, NULL, NULL );
			g_hWnd_btn_proxy_remove = _CreateWindowW( WC_BUTTON, L"Remove", BS_PUSHBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 180, rc.bottom - 65, 80, 23, hWnd, ( HMENU )BTN_PROXY_REMOVE, NULL, NULL );

			g_hWnd_static_proxy_select = _CreateWindowW( WC_STATIC, L"Use proxy:", WS_CHILD | WS_VISIBLE, 10, rc.bottom - 32, 60, 17, hWnd, ( HMENU )STATIC_PROXY_SELECT, NULL, NULL );
			g_hWnd_combo_proxy_select = _CreateWindowW( WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 80, rc.bottom - 35, rc.right - 90, 200, hWnd, ( HMENU )COMBO_PROXY_SELECT, NULL, NULL );

			if ( hFont_options != NULL )
			{
				HWND hWnd_child = _GetWindow( hWnd, GW_CHILD );
				while ( hWnd_child != NULL )
				{
					_SendMessageW( hWnd_child, WM_SETFONT, ( WPARAM )hFont_options, 0 );

					// Some composite controls (IP address, combo box) ignore WM_SETFONT on
					// themselves; their inner child controls need the font individually.
					EnumChildWindows( hWnd_child, EnumChildFontProc, ( LPARAM )hFont_options );

					hWnd_child = _GetWindow( hWnd_child, GW_HWNDNEXT );
				}
			}

			RefreshProxyList();
			RefreshProxyCombo();

			return 0;
		}
		break;

		case WM_SIZE:
		{
			RECT rc;
			_GetClientRect( hWnd, &rc );

			_SetWindowPos( g_hWnd_proxy_list, HWND_TOP, 10, 10, rc.right - 20, rc.bottom - 85, SWP_NOZORDER );
			_SetWindowPos( g_hWnd_btn_proxy_add, HWND_TOP, 10, rc.bottom - 65, 80, 23, SWP_NOZORDER );
			_SetWindowPos( g_hWnd_btn_proxy_edit, HWND_TOP, 95, rc.bottom - 65, 80, 23, SWP_NOZORDER );
			_SetWindowPos( g_hWnd_btn_proxy_remove, HWND_TOP, 180, rc.bottom - 65, 80, 23, SWP_NOZORDER );
			_SetWindowPos( g_hWnd_static_proxy_select, HWND_TOP, 10, rc.bottom - 32, 60, 17, SWP_NOZORDER );
			_SetWindowPos( g_hWnd_combo_proxy_select, HWND_TOP, 80, rc.bottom - 35, rc.right - 90, 200, SWP_NOZORDER );

			return 0;
		}
		break;

		case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
				case BTN_PROXY_ADD:
				{
					OpenProxyEditDialog( true, 0 );
				}
				break;

				case BTN_PROXY_EDIT:
				{
					OpenProxyEditDialog( false, GetProxyListSelection() );
				}
				break;

				case BTN_PROXY_REMOVE:
				{
					unsigned char selection = GetProxyListSelection();
					if ( selection > 0 )
					{
						if ( CMessageBoxW( hWnd, L"Remove the selected proxy entry?", PROGRAM_CAPTION, CMB_ICONWARNING | CMB_YESNO ) == CMBIDYES )
						{
							DoublyLinkedList *node = g_proxy_list;
							for ( unsigned char i = 1; i < selection && node != NULL; ++i )
							{
								node = node->next;
							}

							if ( node != NULL )
							{
								DLL_RemoveNode( &g_proxy_list, node );
								FreeProxyEntry( ( PROXY_ENTRY * )node->data );
								GlobalFree( node );

								// Adjust the active selection: if the removed entry was at or
								// before the active index, clamp it.
								if ( cfg_selected_proxy >= selection )
								{
									ApplyProxySelection( ( cfg_selected_proxy > selection ? cfg_selected_proxy - 1 : 0 ) );
								}
								else
								{
									SaveProxyList();
								}

								RefreshProxyList();
								RefreshProxyCombo();

								_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
							}
						}
					}
				}
				break;

				case COMBO_PROXY_SELECT:
				{
					if ( HIWORD( wParam ) == CBN_SELCHANGE )
					{
						unsigned char selection = ( unsigned char )_SendMessageW( g_hWnd_combo_proxy_select, CB_GETCURSEL, 0, 0 );
						ApplyProxySelection( selection );

						RefreshProxyList();

						// Choosing the active proxy is an option change: enable the Apply button.
						_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
					}
				}
				break;
			}

			return 0;
		}
		break;

		case WM_SAVE_OPTIONS:
		{
			// The list file and the selection are persisted as changes happen; make sure
			// everything is consistent when the options dialog is saved.
			SaveProxyList();

			return 0;
		}
		break;

		case WM_DESTROY:
		{
			g_hWnd_proxy_list = NULL;
			g_hWnd_btn_proxy_add = NULL;
			g_hWnd_btn_proxy_edit = NULL;
			g_hWnd_btn_proxy_remove = NULL;
			g_hWnd_combo_proxy_select = NULL;
			g_hWnd_static_proxy_select = NULL;

			return 0;
		}
		break;

		default:
		{
			return _DefWindowProcW( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return TRUE;
}
