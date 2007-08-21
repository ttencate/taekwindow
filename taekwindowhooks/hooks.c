#include <windows.h>
#include <stdio.h>

#include "hooks.h"

/* The shared data segment.
 * These variables are shared across all instances of the DLL. This is necessary because the hook handler
 * runs in the thread of the window that would have received the event, NOT in the thread of the application
 * that hooked up the handler in the first place.
 * Everything in this segment must be initialized in order to actually become shared.
 * This syntax probably only works with the Microsoft compiler...
 */
#pragma data_seg(".SHARED")

/* The thread ID of the first thread that called init().
 */
DWORD mainThreadId = 0;

/* The current state we're in.
 * dsNone: nothing special going on, all events simply passed on.
 * dsDragging: we're moving or resizing the window identified by draggedWindow.
 * dsIgnoring: we're ignoring and discarding mouse events until the button is released.
 *             This is used when the user attempts to Modifier+drag a window that cannot be moved,
 *             e.g. a maximized window. In that case we don't want to pass those mouse events to that window.
 */
typedef int DragState;
#define	dsNone     0
#define dsDragging 1
#define dsIgnoring 2
DragState currentState = dsNone;

/* Whether we're resizing in the x and/or y direction. Only meaningful while dragging.
 * resizingX == -1 means resizing at left border, 0 means not resizing in x direction, 1 means right border.
 * Similar for y; -1 is top border, 0 is no resizing, 1 is bottom border.
 */
int resizingX = 0, resizingY = 0;

/* The button that we're dragging with.
 * Only meaningful while we're dragging, of course.
 */
typedef DWORD MouseButton;
#define mbLeft   MK_LBUTTON
#define mbMiddle MK_MBUTTON
#define mbRight  MK_RBUTTON
#if _WIN32_WINNT >= 0x0500
	// only declare these if we build for Windows 2000 or above
#define mbX1     MK_XBUTTON1
#define mbX2     MK_XBUTTON2
#endif
MouseButton draggingButton = mbLeft;

/* The last known location of the mouse cursor (screen coordinates).
 * This is used in the mouse event handler to compute the distance travelled since the last mouse event.
 */
POINT lastMousePos = { 0, 0 };

/* The window that we are currently dragging.
 * Only meaningful if currentState is dsDragging.
 */
HWND draggedWindow = NULL;

/* The window that had focus before we started dragging.
 * Only meaningful if currentState is dsDragging.
 */
HWND focusedWindow = NULL;

/* The last known window rectangle of the draggedWindow. Saves us calls to GetWindowRect().
 */
RECT lastRect = { 0, 0, 0, 0 };

/* The modifier keys used for moving and resizing.
 * These are zero-terminated lists of length at most 3 (excluding the terminator).
 */
int moveModifiers[4] = { VK_MENU, 0, 0, 0 };
int resizeModifiers[4] = { VK_MENU, 0, 0, 0 };

/* The mouse buttons used for moving and resizing.
 */
MouseButton moveButton = mbLeft;
MouseButton resizeButton = mbRight;

/* The resize mode used.
 */
typedef int ResizeMode;
#define rsBottomRight 0
#define rsNineSquares 1
ResizeMode resizeMode = rsNineSquares;

/* The desired behaviour of the focus.
 */
typedef int FocusMode;
#define fmRetainFocus 0 // no focus change on moving/resizing
#define fmSwitchFocus 1 // give focus to window being moved/resized
FocusMode focusMode = fmRetainFocus;

/* The desired behaviour of the stacking order (Z-order).
 */
typedef int StackingMode;
#define smRetainStacking 0 // window is not moved to the top when moving/resizing
#define smBringToFront 1   // window is moved to the top when moving/resizing
StackingMode stackingMode = smRetainStacking;

/* End of the shared data segment.
 */
#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")

/* The handle of this instance of the DLL, set in DllMain.
 */
HINSTANCE dllHandle = NULL;

#ifdef DEBUG
/* For debugging purposes: shows a messagebox with the message corresponding to GetLastError(),
 * with the given title.
 */
void showLastError(LPCWSTR title) {
	PVOID msg;

	if (!GetLastError())
		return;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBoxW(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}
#endif

#ifdef DEBUG
/* For debugging purposes: writes a line to the debug log, called debug.txt, in the current working directory.
 * Note that it is opened and closed for every line that's written, because I'm too lazy to figure out
 * whether I can share the file handle between different processes.
 */
void debugLogMessage(char *text) {
	FILE *debugFile;

	debugFile = fopen("debug.txt", "a");

	fprintf(debugFile, text);

	fclose(debugFile);
}
#endif

/* The entry point for the DLL. Stores the instance handle of the DLL for later use.
 */
BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved) {
	if (dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH) {
		dllHandle = (HINSTANCE)hinstDLL;
	} else if (dwReason == DLL_PROCESS_DETACH || dwReason == DLL_THREAD_DETACH) {
		// Nothing to do for the moment.
	}
	return TRUE;
}

/* Initializes the DLL by storing the ID of the thread that will hook up the event hooks.
 * On successful execution, the thread ID is stored and the function returns NULL.
 * If there was already another thread ID (probably another taekwindow.exe process),
 * the function returns the thread ID of that thread and does nothing else.
 */
DWORD __declspec(dllexport) __stdcall init(DWORD threadId) {
	if (mainThreadId) {
		return mainThreadId;
	} else {
		mainThreadId = threadId;
		return 0;
	}
}

DWORD readRegDword(HKEY key, LPSTR value, DWORD def) {
	DWORD type;
	DWORD data;
	DWORD size = sizeof(data);
	if (RegQueryValueEx(key, value, NULL, &type, (LPBYTE)&data, &size) == ERROR_SUCCESS) {
		if (type == REG_DWORD && size == sizeof(data)) {
			return data;
		}
	}
	return def;
}

void __declspec(dllexport) __stdcall readConfig() {
	HKEY software, taekwindow, configKey;
	// Set up defaults; these are loaded if registry loading fails for some reason.
	moveModifiers[0] = VK_MENU;
	moveModifiers[1] = 0;
	resizeModifiers[0] = VK_MENU;
	resizeModifiers[1] = 0;
	// Open the registry keys.
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_READ, &software) == ERROR_SUCCESS) {
		if (RegOpenKeyEx(software, "Taekwindow", 0, KEY_READ, &taekwindow) == ERROR_SUCCESS) {
			// We'll only change the version number of the key once the registry structure is no longer backwards compatible.
			// That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
			if (RegOpenKeyEx(taekwindow, "0.2", 0, KEY_READ, &configKey) == ERROR_SUCCESS) {
				// Read stuff.
				moveModifiers[0] = readRegDword(configKey, "moveModifier0", VK_MENU);
				moveModifiers[1] = readRegDword(configKey, "moveModifier1", 0);
				moveModifiers[2] = readRegDword(configKey, "moveModifier2", 0);
				resizeModifiers[0] = readRegDword(configKey, "resizeModifier0", VK_MENU);
				resizeModifiers[1] = readRegDword(configKey, "resizeModifier1", 0);
				resizeModifiers[2] = readRegDword(configKey, "resizeModifier2", 0);
				moveButton = (MouseButton)readRegDword(configKey, "moveButton", MK_LBUTTON);
				resizeButton = (MouseButton)readRegDword(configKey, "resizeButton", MK_RBUTTON);
				resizeMode = (ResizeMode)readRegDword(configKey, "resizeMode", rsNineSquares);
				focusMode = (FocusMode)readRegDword(configKey, "focusMode", fmRetainFocus);
				stackingMode = (StackingMode)readRegDword(configKey, "stackingMode", smRetainStacking);
				// Close the keys again.
				RegCloseKey(configKey);
			}
			RegCloseKey(taekwindow);
		}
		RegCloseKey(software);
	}
}

/* Returns true if the given window can be moved and resized.
 * Prevents moving/resizing of maximized windows. Other cases may be added later.
 */
BOOL isDraggableWindow(HWND window) {
	WINDOWINFO info;
	info.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(window, &info);
	if (info.dwStyle & WS_MAXIMIZE) {
		return FALSE;
	} else {
		return TRUE;
	}
}

/* Sets the variables resizingX and resizingY to the proper values,
 * considering the screen-coordinate pointer location.
 */
void setResizingX(POINT const *pt) {
	switch (resizeMode) {
		case rsBottomRight:
			resizingX = 1;
			break;
		case rsNineSquares:
			resizingX = (pt->x - lastRect.left) * 3 / (lastRect.right - lastRect.left) - 1;
			break;
	}
}
void setResizingY(POINT const *pt) {
	switch (resizeMode) {
		case rsBottomRight:
			resizingY = 1;
			break;
		case rsNineSquares:
			resizingY = (pt->y - lastRect.top) * 3 / (lastRect.bottom - lastRect.top) - 1;
			break;
	}
}

/* Returns true if all specified modifiers are down.
 * modifiers points to a 0-terminated array of ints representing the virtual key codes of the modifiers.
 * For safety, this list is assumed to contain at most 3 elements, not counting the terminator.
 */
BOOL allModifiersDown(int *modifiers) {
	int i;
	for (i = 0; i < 3; i++) {
		if (!modifiers[i]) {
			// Reached the terminator.
			return TRUE;
		}
		if (!(GetKeyState(modifiers[i]) & 0x80000000)) {
			// One of 'em is not down.
			return FALSE;
		}
	}
	// No not-down modifier found.
	return TRUE;
}

/* Initiates a window dragging action (i.e. moving/resizing).
 */
void startDragAction(MouseButton button, MOUSEHOOKSTRUCT *eventInfo) {
	// Remember which window had the focus previously, so we can restore it if desired.
	focusedWindow = GetForegroundWindow();
	// Capture the mouse so it'll still get events even if the mouse leaves the window
	// (could happen while resizing).
	SetCapture(draggedWindow);
	GetWindowRect(draggedWindow, &lastRect);
	if (button == resizeButton) {
		// Figure out in which area we're dragging to resize in the proper direction.
		setResizingX(&eventInfo->pt);
		setResizingY(&eventInfo->pt);
	}
	// Pull the window to the top if the user likes that.
	if (stackingMode == smBringToFront) {
		if (focusMode == fmSwitchFocus) {
			// Easiest way to raise and focus.
			BringWindowToTop(draggedWindow);
		} else {
			// When trying to pull the window to the top without activating it,
			// using HWND_TOP and the SWP_NOACTIVATE flag, nothing happens.
			// This HACK seems to work nicely.
			// BUG: windows that are already topmost 
			SetWindowPos(draggedWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			SetWindowPos(draggedWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
	} else { // stackingMode == smRetainStacking
		if (focusMode == fmSwitchFocus) {
			// Change focus, but do not raise.
			// TODO
		} else {
			// Neither change focus, nor raise dragged window.
			// TODO: keep active window active, instead of having it lose focus
		}
	}
}

/* Processes a button-down event.
 * Returns true if the event should not be passed on to the application, false otherwise.
 */
BOOL processButtonDown(MouseButton button, MOUSEHOOKSTRUCT *eventInfo) {
	if (currentState == dsNone) {
		// Nothing is yet going on. We possibly want to take action if the correct mouse button is being used.
		if ((button == moveButton && allModifiersDown(moveModifiers))
		 || (button == resizeButton && allModifiersDown(resizeModifiers))) {
			// Yippee! A Modifier-drag event just started that we want to process.
			currentState = dsDragging;
			draggingButton = button;
			// Find the actual window being dragged: this is the top-level window that is the ultimate parent
			// of the window receiving the event. Seems to work for MDI's too.
			draggedWindow = GetAncestor(eventInfo->hwnd, GA_ROOT);
			if (isDraggableWindow(draggedWindow)) {
				// Window can be dragged.
				startDragAction(button, eventInfo);
			} else {
				// Modifier-dragging an invalid window. The user won't expect her actions to be passed
				// to that window, so we suppress all events until the mouse button is released.
				currentState = dsIgnoring;
			}
			// Either way, we eat the event.
			return TRUE;
		} else {
			// Mouse-down event with button or modifiers that we don't care about. Stay away from it.
			return FALSE;
		}
	} else {
		// We're already dragging, and another button was pressed.
		// Naughty user shouldn't do this, but we pass the event anyway, because otherwise the application
		// might receive a mouse-up event without a preceding mouse-down event and get all confused.
		return FALSE;
	}
}

/* Processes the end of a drag (i.e. move/resize) action.
 */
void endDragAction() {
	ReleaseCapture();
	currentState = dsNone;

	if (focusMode == fmRetainFocus) {
		// Dragging a window makes the focus go away from the currently focused window, with the result that no window has focus at all.
		// So we give focus back to the window that previously had it.
		// There must be a better way, i.e. preventing the focus change altogether, but I don't know how.

		// TODO
	}
	switch (focusMode) {
		case fmRetainFocus:
			// Return focus to the window that previously had it.
			// If NULL, this means no window had focus, so we restore to that situation.
			SetFocus(focusedWindow);
			break;
		case fmSwitchFocus:
			// Switch focus to the window that we were dragging.
			SetWindowPos(draggedWindow, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
			break;
	}
}

/* Processes a mouse button release event.
 * Returns true if the event was processed and should not be passed to the application, false otherwise.
 */
BOOL processButtonUp(MouseButton button) {
	switch (currentState) {
		case dsNone:
			// Nothing going on, pass the event on.
			return FALSE;
		case dsDragging:
			if (button == draggingButton) {
				// End of move or resize action.
				// Process it and eat the event.
				endDragAction();
				return TRUE;
			} else {
				// Other button released during move event. (Naughty user!)
				return FALSE;
			}
		case dsIgnoring:
			// Ignoring all events until the appropriate button was released.
			if (button == draggingButton) {
				// Well, guess what, it was!
				// But we still ignore this up event, of course.
				currentState = dsNone;
			}
			return TRUE;
		default:
			// Should never be reached unless the currentState enum was tortured by inserting a pineapple into its butt.
			return FALSE;
	}
}

/* Processes drag events that we want to handle.
 */
void processDrag(MOUSEHOOKSTRUCT const *eventInfo) {
	int deltaX, deltaY;
	UINT flags;

	// Find out by how much the mouse was moved.
	deltaX = eventInfo->pt.x - lastMousePos.x, deltaY = eventInfo->pt.y - lastMousePos.y;
	if (draggingButton == moveButton) {
		// We're moving the window, so adjust its position.
		lastRect.left += deltaX;
		lastRect.top += deltaY;
		lastRect.right += deltaX;
		lastRect.bottom += deltaY;
	} else if (draggingButton == resizeButton) {
		// Resizing the window, at the correct corner/edge.
		switch (resizingX) {
			case -1:
				lastRect.left += deltaX; break;
			case 1:
				lastRect.right += deltaX; break;
			case 0:
				// We may have come close to a vertical border in the meantime.
				setResizingX(&eventInfo->pt); break;
		}
		switch (resizingY) {
			case -1:
				lastRect.top += deltaY; break;
			case 1:
				lastRect.bottom += deltaY; break;
			case 0:
				// We may have come close to a horizontal border in the meantime.
				setResizingY(&eventInfo->pt); break;
		}
	}

	flags = 0;
	if (focusMode == fmRetainFocus) {
		flags |= SWP_NOACTIVATE;
	}
	if (stackingMode == smRetainStacking) {
		flags |= SWP_NOZORDER | SWP_NOOWNERZORDER;
	}
	SetWindowPos(draggedWindow, HWND_TOP,
	             lastRect.left, lastRect.top, lastRect.right - lastRect.left, lastRect.bottom - lastRect.top,
	             flags);
}

/* The function for handling mouse events. Executed in the context of the process that owns the window.
 * This is the reason why we have to use a separate DLL; see the SetWindowsHookEx documentation for details.
 */
LRESULT __declspec(dllexport) CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	BOOL processed = FALSE; // Set to true if we don't want to pass the event to the application.
	LRESULT res;

	if (nCode == HC_ACTION) {
		MOUSEHOOKSTRUCT *eventInfo = (MOUSEHOOKSTRUCT*)lParam;
		switch (wParam) {
			// We have to process these events separately, because we do not get the full message data.
			// E.g. the WM_LBUTTONDOWN message holds MK_LBUTTON in its wParam, but we do not get this wParam through this hook.
			case WM_LBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
				processed = processButtonDown(mbLeft, eventInfo);
				break;
			case WM_MBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
				processed = processButtonDown(mbMiddle, eventInfo);
				break;
			case WM_RBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
				processed = processButtonDown(mbRight, eventInfo);
				break;
			case WM_LBUTTONUP:
			case WM_NCLBUTTONUP:
				processed = processButtonUp(mbLeft);
				break;
			case WM_MBUTTONUP:
			case WM_NCMBUTTONUP:
				processed = processButtonUp(mbMiddle);
				break;
			case WM_RBUTTONUP:
			case WM_NCRBUTTONUP:
				processed = processButtonUp(mbRight);
				break;
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				switch (currentState) {
					case dsDragging:
						// The mouse was moved while we're dragging a window.
						processDrag(eventInfo);
						processed = TRUE;
						break;
					case dsIgnoring:
						processed = TRUE;
						break;
					case dsNone:
						break;
				}
				break;
			case WM_MOUSEACTIVATE:
				// Sent whenever the mouse was pressed in an inactive window.
				// When moving or resizing it, we want to ignore this.
				if (currentState == dsDragging || currentState == dsIgnoring) {
					processed = TRUE;
				}
				break;
		}
		lastMousePos = eventInfo->pt;
	}

	res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (processed)
		res = 1; // nonzero return value prevents passing the event to the application
	return res;
}

/* The function for handling keyboard events.
 * Or rather, the function to eat keyboard events that the application shouldn't receive.
 */
LRESULT __declspec(dllexport) CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
#ifdef DEBUG
	HWND hwnd;
	POINT mousePos;
	if (wParam == 0x51) {
		// Q button pressed. Panic button for debugging.
		PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
		return 1;
	}
	if (wParam == 0x52) {
		// R button pressed. Raise window under cursor.
		GetCursorPos(&mousePos);
		hwnd = WindowFromPoint(mousePos);
		hwnd = GetAncestor(hwnd, GA_ROOT);
		// When trying to pull the window to the top, using HWND_TOP and the SWP_NOACTIVATE flag,
		// nothing happens. This HACK seems to work nicely.
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		return 1;
	}
	if (wParam == 0x41) {
		// A button pressed. Activate window under cursor.
		GetCursorPos(&mousePos);
		hwnd = WindowFromPoint(mousePos);
		hwnd = GetAncestor(hwnd, GA_ROOT);
		/* activates and raises:
		SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
		*/
		/* activates and raises:
		AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(hwnd, NULL), TRUE);
		SetActiveWindow(hwnd);
		AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(hwnd, NULL), FALSE);
		*/
		/* activates and raises:
		AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(hwnd, NULL), TRUE);
		SetFocus(hwnd);
		AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(hwnd, NULL), FALSE);
		*/
	}
#endif
	if (nCode == HC_ACTION) {
	}
	return CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
}
