#define _CRT_SECURE_NO_DEPRECATE 1

#include <windows.h>
#include <stdio.h>

#include "testapp.hpp"

char *sysCommandName(WPARAM wParam, LPARAM lParam);
char *hitTestName(LRESULT retval);
char *mouseMoveName(WPARAM wParam);

char *messageInfo(MSG *message) {
	return messageInfo(message->message, message->wParam, message->lParam);
}

char *messageInfo(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static char buffer[256];

	char *name = messageName(uMsg);

	switch (uMsg) {
		case WM_SYSCOMMAND:
			sprintf(buffer, "%s with %s", name, sysCommandName(wParam, lParam));
			break;
		case WM_MOUSEMOVE:
			sprintf(buffer, "%s with %s", name, mouseMoveName(wParam));
			break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			sprintf(buffer, "%s with virtual key 0x%04x, repeat count %i; Alt %s; previously %s",
				name, wParam, lParam & 0xFF,
				lParam & 0x20000000 ? "down" : "up",
				lParam & 0x40000000 ? "down" : "up");
			break;
		default:
			strcpy(buffer, name);
	}
	return buffer;
}

char *retValInfo(LRESULT retval, UINT message) {
	static char buffer[256];
	switch (message) {
		case WM_NCHITTEST:
			sprintf(buffer, "%s", hitTestName(retval));
			break;
		default:
			sprintf(buffer, "%d", retval);
	}
	return buffer;
}

char *sysCommandName(WPARAM wParam, LPARAM lParam) {
	static char buffer[256];

	char *scName;
	switch (wParam & 0xFFF0) {
		case SC_CLOSE: scName = "SC_CLOSE"; break;
		case SC_CONTEXTHELP: scName = "SC_CONTEXTHELP"; break;
		case SC_DEFAULT: scName = "SC_DEFAULT"; break;
		case SC_HOTKEY: scName = "SC_HOTKEY"; break;
		case SC_HSCROLL: scName = "SC_HSCROLL"; break;
		case SC_KEYMENU: scName = "SC_KEYMENU"; break;
		case SC_MAXIMIZE: scName = "SC_MAXIMIZE"; break;
		case SC_MINIMIZE: scName = "SC_MINIMIZE"; break;
		case SC_MONITORPOWER: scName = "SC_MONITORPOWER"; break;
		case SC_MOUSEMENU: scName = "SC_MOUSEMENU"; break;
		case SC_MOVE: scName = "SC_MOVE"; break;
		case SC_NEXTWINDOW: scName = "SC_NEXTWINDOW"; break;
		case SC_PREVWINDOW: scName = "SC_PREVWINDOW"; break;
		case SC_RESTORE: scName = "SC_RESTORE"; break;
		case SC_SCREENSAVE: scName = "SC_SCREENSAVE"; break;
		case SC_SIZE: scName = "SC_SIZE"; break;
		case SC_TASKLIST: scName = "SC_TASKLIST"; break;
		case SC_VSCROLL: scName = "SC_VSCROLL"; break;
		default: scName = "unknown"; break;
	}

	char info[32];
	if (lParam == 0) {
		strcpy(info, "chosen with mnemonic");
	} else if (lParam == -1) {
		strcpy(info, "chosen with accelerator");
	} else {
		sprintf(info, "chosen with mouse at %dx%d", LOWORD(lParam), HIWORD(lParam));
	}

	sprintf(buffer, "%s | 0x%01X, %s", scName, wParam & 0x000F, info);
	return buffer;
}

char *hitTestName(LRESULT retval) {
	switch (retval) {
		case HTBORDER: return "HTBORDER";
		case HTBOTTOM: return "HTBOTTOM";
		case HTBOTTOMLEFT: return "HTBOTTOMLEFT";
		case HTBOTTOMRIGHT: return "HTBOTTOMRIGHT";
		case HTCAPTION: return "HTCAPTION";
		case HTCLIENT: return "HTCLIENT";
		case HTCLOSE: return "HTCLOSE";
		case HTERROR: return "HTERROR";
		case HTHELP: return "HTHELP";
		case HTHSCROLL: return "HTHSCROLL";
		case HTLEFT: return "HTLEFT";
		case HTMENU: return "HTMENU";
		case HTMAXBUTTON: return "HTMAXBUTTON";
		case HTMINBUTTON: return "HTMINBUTTON";
		case HTNOWHERE: return "HTNOWHERE";
		case HTRIGHT: return "HTRIGHT";
		case HTSIZE: return "HTSIZE";
		case HTSYSMENU: return "HTSYSMENU";
		case HTTOP: return "HTTOP";
		case HTTOPLEFT: return "HTTOPLEFT";
		case HTTOPRIGHT: return "HTTOPRIGHT";
		case HTTRANSPARENT: return "HTTRANSPARENT";
		case HTVSCROLL: return "HTVSCROLL";
		default: return "unknown";
	}
}

char *mouseMoveName(WPARAM wParam) {
	static char buffer[256];
	buffer[0] = '\0';

	if (wParam & MK_CONTROL) strcat(buffer, "MK_CONTROL | ");
	if (wParam & MK_SHIFT) strcat(buffer, "MK_SHIFT | ");
	if (wParam & MK_LBUTTON) strcat(buffer, "MK_LBUTTON | ");
	if (wParam & MK_MBUTTON) strcat(buffer, "MK_MBUTTON | ");
	if (wParam & MK_RBUTTON) strcat(buffer, "MK_RBUTTON | ");
	if (wParam & MK_XBUTTON1) strcat(buffer, "MK_XBUTTON1 | ");
	if (wParam & MK_XBUTTON2) strcat(buffer, "MK_XBUTTON2 | ");

	size_t len = strlen(buffer);
	if (len > 0)
		buffer[len-3] = '\0'; // cut off last " | "
	return buffer;
}

// Generated from winuser.h by extracting all WM_ strings
char *messageName(UINT message) {
	static char buffer[256];

	switch (message) {
		case WM_NULL: return "WM_NULL";
		case WM_CREATE: return "WM_CREATE";
		case WM_DESTROY: return "WM_DESTROY";
		case WM_MOVE: return "WM_MOVE";
		case WM_SIZE: return "WM_SIZE";
		case WM_ACTIVATE: return "WM_ACTIVATE";
		case WM_SETFOCUS: return "WM_SETFOCUS";
		case WM_KILLFOCUS: return "WM_KILLFOCUS";
		case WM_ENABLE: return "WM_ENABLE";
		case WM_SETREDRAW: return "WM_SETREDRAW";
		case WM_SETTEXT: return "WM_SETTEXT";
		case WM_GETTEXT: return "WM_GETTEXT";
		case WM_GETTEXTLENGTH: return "WM_GETTEXTLENGTH";
		case WM_PAINT: return "WM_PAINT";
		case WM_CLOSE: return "WM_CLOSE";
		case WM_QUERYENDSESSION: return "WM_QUERYENDSESSION";
		case WM_QUERYOPEN: return "WM_QUERYOPEN";
		case WM_ENDSESSION: return "WM_ENDSESSION";
		case WM_QUIT: return "WM_QUIT";
		case WM_ERASEBKGND: return "WM_ERASEBKGND";
		case WM_SYSCOLORCHANGE: return "WM_SYSCOLORCHANGE";
		case WM_SHOWWINDOW: return "WM_SHOWWINDOW";
		case WM_SETTINGCHANGE: return "WM_SETTINGCHANGE";
		case WM_DEVMODECHANGE: return "WM_DEVMODECHANGE";
		case WM_ACTIVATEAPP: return "WM_ACTIVATEAPP";
		case WM_FONTCHANGE: return "WM_FONTCHANGE";
		case WM_TIMECHANGE: return "WM_TIMECHANGE";
		case WM_CANCELMODE: return "WM_CANCELMODE";
		case WM_SETCURSOR: return "WM_SETCURSOR";
		case WM_MOUSEACTIVATE: return "WM_MOUSEACTIVATE";
		case WM_CHILDACTIVATE: return "WM_CHILDACTIVATE";
		case WM_QUEUESYNC: return "WM_QUEUESYNC";
		case WM_GETMINMAXINFO: return "WM_GETMINMAXINFO";
		case WM_PAINTICON: return "WM_PAINTICON";
		case WM_ICONERASEBKGND: return "WM_ICONERASEBKGND";
		case WM_NEXTDLGCTL: return "WM_NEXTDLGCTL";
		case WM_SPOOLERSTATUS: return "WM_SPOOLERSTATUS";
		case WM_DRAWITEM: return "WM_DRAWITEM";
		case WM_MEASUREITEM: return "WM_MEASUREITEM";
		case WM_DELETEITEM: return "WM_DELETEITEM";
		case WM_VKEYTOITEM: return "WM_VKEYTOITEM";
		case WM_CHARTOITEM: return "WM_CHARTOITEM";
		case WM_SETFONT: return "WM_SETFONT";
		case WM_GETFONT: return "WM_GETFONT";
		case WM_SETHOTKEY: return "WM_SETHOTKEY";
		case WM_GETHOTKEY: return "WM_GETHOTKEY";
		case WM_QUERYDRAGICON: return "WM_QUERYDRAGICON";
		case WM_COMPAREITEM: return "WM_COMPAREITEM";
		case WM_GETOBJECT: return "WM_GETOBJECT";
		case WM_COMPACTING: return "WM_COMPACTING";
		case WM_COMMNOTIFY: return "WM_COMMNOTIFY";
		case WM_WINDOWPOSCHANGING: return "WM_WINDOWPOSCHANGING";
		case WM_WINDOWPOSCHANGED: return "WM_WINDOWPOSCHANGED";
		case WM_POWER: return "WM_POWER";
		case WM_COPYDATA: return "WM_COPYDATA";
		case WM_CANCELJOURNAL: return "WM_CANCELJOURNAL";
		case WM_NOTIFY: return "WM_NOTIFY";
		case WM_INPUTLANGCHANGEREQUEST: return "WM_INPUTLANGCHANGEREQUEST";
		case WM_INPUTLANGCHANGE: return "WM_INPUTLANGCHANGE";
		case WM_TCARD: return "WM_TCARD";
		case WM_HELP: return "WM_HELP";
		case WM_USERCHANGED: return "WM_USERCHANGED";
		case WM_NOTIFYFORMAT: return "WM_NOTIFYFORMAT";
		case WM_CONTEXTMENU: return "WM_CONTEXTMENU";
		case WM_STYLECHANGING: return "WM_STYLECHANGING";
		case WM_STYLECHANGED: return "WM_STYLECHANGED";
		case WM_DISPLAYCHANGE: return "WM_DISPLAYCHANGE";
		case WM_GETICON: return "WM_GETICON";
		case WM_SETICON: return "WM_SETICON";
		case WM_NCCREATE: return "WM_NCCREATE";
		case WM_NCDESTROY: return "WM_NCDESTROY";
		case WM_NCCALCSIZE: return "WM_NCCALCSIZE";
		case WM_NCHITTEST: return "WM_NCHITTEST";
		case WM_NCPAINT: return "WM_NCPAINT";
		case WM_NCACTIVATE: return "WM_NCACTIVATE";
		case WM_GETDLGCODE: return "WM_GETDLGCODE";
		case WM_SYNCPAINT: return "WM_SYNCPAINT";
		case WM_NCMOUSEMOVE: return "WM_NCMOUSEMOVE";
		case WM_NCLBUTTONDOWN: return "WM_NCLBUTTONDOWN";
		case WM_NCLBUTTONUP: return "WM_NCLBUTTONUP";
		case WM_NCLBUTTONDBLCLK: return "WM_NCLBUTTONDBLCLK";
		case WM_NCRBUTTONDOWN: return "WM_NCRBUTTONDOWN";
		case WM_NCRBUTTONUP: return "WM_NCRBUTTONUP";
		case WM_NCRBUTTONDBLCLK: return "WM_NCRBUTTONDBLCLK";
		case WM_NCMBUTTONDOWN: return "WM_NCMBUTTONDOWN";
		case WM_NCMBUTTONUP: return "WM_NCMBUTTONUP";
		case WM_NCMBUTTONDBLCLK: return "WM_NCMBUTTONDBLCLK";
		case WM_NCXBUTTONDOWN: return "WM_NCXBUTTONDOWN";
		case WM_NCXBUTTONUP: return "WM_NCXBUTTONUP";
		case WM_NCXBUTTONDBLCLK: return "WM_NCXBUTTONDBLCLK";
		case WM_INPUT: return "WM_INPUT";
		case WM_KEYDOWN: return "WM_KEYDOWN";
		case WM_KEYUP: return "WM_KEYUP";
		case WM_CHAR: return "WM_CHAR";
		case WM_DEADCHAR: return "WM_DEADCHAR";
		case WM_SYSKEYDOWN: return "WM_SYSKEYDOWN";
		case WM_SYSKEYUP: return "WM_SYSKEYUP";
		case WM_SYSCHAR: return "WM_SYSCHAR";
		case WM_SYSDEADCHAR: return "WM_SYSDEADCHAR";
		case WM_UNICHAR: return "WM_UNICHAR";
		case WM_IME_STARTCOMPOSITION: return "WM_IME_STARTCOMPOSITION";
		case WM_IME_ENDCOMPOSITION: return "WM_IME_ENDCOMPOSITION";
		case WM_IME_COMPOSITION: return "WM_IME_COMPOSITION";
		case WM_INITDIALOG: return "WM_INITDIALOG";
		case WM_COMMAND: return "WM_COMMAND";
		case WM_SYSCOMMAND: return "WM_SYSCOMMAND";
		case WM_TIMER: return "WM_TIMER";
		case WM_HSCROLL: return "WM_HSCROLL";
		case WM_VSCROLL: return "WM_VSCROLL";
		case WM_INITMENU: return "WM_INITMENU";
		case WM_INITMENUPOPUP: return "WM_INITMENUPOPUP";
		case WM_MENUSELECT: return "WM_MENUSELECT";
		case WM_MENUCHAR: return "WM_MENUCHAR";
		case WM_ENTERIDLE: return "WM_ENTERIDLE";
		case WM_MENURBUTTONUP: return "WM_MENURBUTTONUP";
		case WM_MENUDRAG: return "WM_MENUDRAG";
		case WM_MENUGETOBJECT: return "WM_MENUGETOBJECT";
		case WM_UNINITMENUPOPUP: return "WM_UNINITMENUPOPUP";
		case WM_MENUCOMMAND: return "WM_MENUCOMMAND";
		case WM_CHANGEUISTATE: return "WM_CHANGEUISTATE";
		case WM_UPDATEUISTATE: return "WM_UPDATEUISTATE";
		case WM_QUERYUISTATE: return "WM_QUERYUISTATE";
		case WM_CTLCOLORMSGBOX: return "WM_CTLCOLORMSGBOX";
		case WM_CTLCOLOREDIT: return "WM_CTLCOLOREDIT";
		case WM_CTLCOLORLISTBOX: return "WM_CTLCOLORLISTBOX";
		case WM_CTLCOLORBTN: return "WM_CTLCOLORBTN";
		case WM_CTLCOLORDLG: return "WM_CTLCOLORDLG";
		case WM_CTLCOLORSCROLLBAR: return "WM_CTLCOLORSCROLLBAR";
		case WM_CTLCOLORSTATIC: return "WM_CTLCOLORSTATIC";
		case WM_MOUSEMOVE: return "WM_MOUSEMOVE";
		case WM_LBUTTONDOWN: return "WM_LBUTTONDOWN";
		case WM_LBUTTONUP: return "WM_LBUTTONUP";
		case WM_LBUTTONDBLCLK: return "WM_LBUTTONDBLCLK";
		case WM_RBUTTONDOWN: return "WM_RBUTTONDOWN";
		case WM_RBUTTONUP: return "WM_RBUTTONUP";
		case WM_RBUTTONDBLCLK: return "WM_RBUTTONDBLCLK";
		case WM_MBUTTONDOWN: return "WM_MBUTTONDOWN";
		case WM_MBUTTONUP: return "WM_MBUTTONUP";
		case WM_MBUTTONDBLCLK: return "WM_MBUTTONDBLCLK";
		case WM_MOUSEWHEEL: return "WM_MOUSEWHEEL";
		case WM_XBUTTONDOWN: return "WM_XBUTTONDOWN";
		case WM_XBUTTONUP: return "WM_XBUTTONUP";
		case WM_XBUTTONDBLCLK: return "WM_XBUTTONDBLCLK";
		case WM_PARENTNOTIFY: return "WM_PARENTNOTIFY";
		case WM_ENTERMENULOOP: return "WM_ENTERMENULOOP";
		case WM_EXITMENULOOP: return "WM_EXITMENULOOP";
		case WM_NEXTMENU: return "WM_NEXTMENU";
		case WM_SIZING: return "WM_SIZING";
		case WM_CAPTURECHANGED: return "WM_CAPTURECHANGED";
		case WM_MOVING: return "WM_MOVING";
		case WM_POWERBROADCAST: return "WM_POWERBROADCAST";
		case WM_DEVICECHANGE: return "WM_DEVICECHANGE";
		case WM_MDICREATE: return "WM_MDICREATE";
		case WM_MDIDESTROY: return "WM_MDIDESTROY";
		case WM_MDIACTIVATE: return "WM_MDIACTIVATE";
		case WM_MDIRESTORE: return "WM_MDIRESTORE";
		case WM_MDINEXT: return "WM_MDINEXT";
		case WM_MDIMAXIMIZE: return "WM_MDIMAXIMIZE";
		case WM_MDITILE: return "WM_MDITILE";
		case WM_MDICASCADE: return "WM_MDICASCADE";
		case WM_MDIICONARRANGE: return "WM_MDIICONARRANGE";
		case WM_MDIGETACTIVE: return "WM_MDIGETACTIVE";
		case WM_MDISETMENU: return "WM_MDISETMENU";
		case WM_ENTERSIZEMOVE: return "WM_ENTERSIZEMOVE";
		case WM_EXITSIZEMOVE: return "WM_EXITSIZEMOVE";
		case WM_DROPFILES: return "WM_DROPFILES";
		case WM_MDIREFRESHMENU: return "WM_MDIREFRESHMENU";
		case WM_IME_SETCONTEXT: return "WM_IME_SETCONTEXT";
		case WM_IME_NOTIFY: return "WM_IME_NOTIFY";
		case WM_IME_CONTROL: return "WM_IME_CONTROL";
		case WM_IME_COMPOSITIONFULL: return "WM_IME_COMPOSITIONFULL";
		case WM_IME_SELECT: return "WM_IME_SELECT";
		case WM_IME_CHAR: return "WM_IME_CHAR";
		case WM_IME_REQUEST: return "WM_IME_REQUEST";
		case WM_IME_KEYDOWN: return "WM_IME_KEYDOWN";
		case WM_IME_KEYUP: return "WM_IME_KEYUP";
		case WM_MOUSEHOVER: return "WM_MOUSEHOVER";
		case WM_MOUSELEAVE: return "WM_MOUSELEAVE";
		case WM_NCMOUSEHOVER: return "WM_NCMOUSEHOVER";
		case WM_NCMOUSELEAVE: return "WM_NCMOUSELEAVE";
		case WM_WTSSESSION_CHANGE: return "WM_WTSSESSION_CHANGE";
		case WM_TABLET_FIRST: return "WM_TABLET_FIRST";
		case WM_TABLET_LAST: return "WM_TABLET_LAST";
		case WM_CUT: return "WM_CUT";
		case WM_COPY: return "WM_COPY";
		case WM_PASTE: return "WM_PASTE";
		case WM_CLEAR: return "WM_CLEAR";
		case WM_UNDO: return "WM_UNDO";
		case WM_RENDERFORMAT: return "WM_RENDERFORMAT";
		case WM_RENDERALLFORMATS: return "WM_RENDERALLFORMATS";
		case WM_DESTROYCLIPBOARD: return "WM_DESTROYCLIPBOARD";
		case WM_DRAWCLIPBOARD: return "WM_DRAWCLIPBOARD";
		case WM_PAINTCLIPBOARD: return "WM_PAINTCLIPBOARD";
		case WM_VSCROLLCLIPBOARD: return "WM_VSCROLLCLIPBOARD";
		case WM_SIZECLIPBOARD: return "WM_SIZECLIPBOARD";
		case WM_ASKCBFORMATNAME: return "WM_ASKCBFORMATNAME";
		case WM_CHANGECBCHAIN: return "WM_CHANGECBCHAIN";
		case WM_HSCROLLCLIPBOARD: return "WM_HSCROLLCLIPBOARD";
		case WM_QUERYNEWPALETTE: return "WM_QUERYNEWPALETTE";
		case WM_PALETTEISCHANGING: return "WM_PALETTEISCHANGING";
		case WM_PALETTECHANGED: return "WM_PALETTECHANGED";
		case WM_HOTKEY: return "WM_HOTKEY";
		case WM_PRINT: return "WM_PRINT";
		case WM_PRINTCLIENT: return "WM_PRINTCLIENT";
		case WM_APPCOMMAND: return "WM_APPCOMMAND";
		case WM_THEMECHANGED: return "WM_THEMECHANGED";
		case WM_HANDHELDFIRST: return "WM_HANDHELDFIRST";
		case WM_HANDHELDLAST: return "WM_HANDHELDLAST";
		case WM_AFXFIRST: return "WM_AFXFIRST";
		case WM_AFXLAST: return "WM_AFXLAST";
		case WM_PENWINFIRST: return "WM_PENWINFIRST";
		case WM_PENWINLAST: return "WM_PENWINLAST";
		case WM_APP: return "WM_APP";
		case WM_USER: return "WM_USER";
		default:
			sprintf(buffer, "0x%04X", message);
	}

	return buffer;
}