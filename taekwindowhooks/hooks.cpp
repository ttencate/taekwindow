#include <windows.h>

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
enum DragState { dsNone, dsDragging, dsIgnoring };
DragState currentState = dsNone;

/* The button that we're dragging with.
 * Only meaningful while we're dragging, of course.
 */
enum MouseButton { mbLeft, mbMiddle, mbRight };
MouseButton draggingButton = mbLeft;

/* The last known location of the mouse cursor (screen coordinates).
 * This is used in the mouse event handler to compute the distance travelled since the last mouse event.
 */
POINT lastMousePos = { 0, 0 };

/* The window that we are currently dragging.
 * Only meaningful if currentState is dsDragging.
 */
HWND draggedWindow = NULL;

/* The last known window rectangle of the draggedWindow. Saves us calls to GetWindowRect().
 */
RECT lastRect = { 0, 0, 0, 0 };

/* The modifier key used for moving and resizing.
 * This is referred to as Modifier is the comments, but you can read Alt if you like.
 */
int modifier = VK_MENU;

/* Whether or not the modifier key is currently down.
 */
bool modifierDown = false;

/* Whether or not dragging has occurred since the last key-down event of the Modifier.
 * If dragging has occurred, this prevents the key-up event to be passed on.
 */
bool haveDragged = false;

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
	if (!GetLastError())
		return;
	PVOID msg;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBoxW(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
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
		return NULL;
	}
}

/* Reads the configuration from the registry.
 * If no value is present for a certain setting, that setting remains untouched.
 */
void __declspec(dllexport) __stdcall readConfig() {
	// Set up defaults; these are loaded if registry loading fails for some reason.
	modifier = VK_MENU;
	// Open the registry keys.
	HKEY software;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_READ, &software) == ERROR_SUCCESS) {
		HKEY taekwindow;
		if (RegOpenKeyEx(software, "Taekwindow", 0, KEY_READ, &taekwindow) == ERROR_SUCCESS) {
			HKEY ohPointTwo;
			// We'll only change the version number of the key once the registry structure is no longer backwards compatible.
			// That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
			if (RegOpenKeyEx(taekwindow, "0.2", 0, KEY_READ, &ohPointTwo) == ERROR_SUCCESS) {
				// Read stuff.
				DWORD type;
				DWORD data;
				DWORD size = sizeof(data);
				if (RegQueryValueEx(ohPointTwo, "modifier", NULL, &type, (LPBYTE)&data, &size) == ERROR_SUCCESS) {
					if (type == REG_DWORD && size == sizeof(data)) {
						modifier = data;
					}
				}
				// Close the keys again.
				RegCloseKey(ohPointTwo);
			}
			RegCloseKey(taekwindow);
		}
		RegCloseKey(software);
	}
}

/* Returns true if the given window can be moved and resized.
 * Prevents moving/resizing of maximized windows. Other cases may be added later.
 */
bool isDraggableWindow(HWND window) {
	WINDOWINFO info;
	info.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(window, &info);
	if (info.dwStyle & WS_MAXIMIZE) {
		return false;
	} else {
		return true;
	}
}

/* Processes a button-down event.
 * Returns true if the event should not be passed on to the application, false otherwise.
 */
bool processButtonDown(MouseButton button, MOUSEHOOKSTRUCT *eventInfo) {
	if (currentState == dsNone) {
		// Nothing is yet going on. We possibly want to take action if the Modifier key is currently pressed.
		if (modifierDown) {
			if (button == mbLeft || button == mbRight) {
				// Yippee! A Modifier-drag event just started that we want to process.
				currentState = dsDragging;
				haveDragged = true;
				draggingButton = button;
				// Find the actual window being dragged: this is the top-level window that is the ultimate parent
				// of the window receiving the event. Seems to work for MDI's too.
				draggedWindow = GetAncestor(eventInfo->hwnd, GA_ROOT);
				if (isDraggableWindow(draggedWindow)) {
					// Window can be dragged.
					// Capture the mouse so it'll still get events even if the mouse leaves the window
					// (could happen while resizing).
					SetCapture(draggedWindow);
					GetWindowRect(draggedWindow, &lastRect);
				} else {
					// Modifier-dragging an invalid window. The user won't expect her actions to be passed
					// to that window, so we suppress all events until the mouse button is released.
					currentState = dsIgnoring;
				}
				// Either way, we eat the event.
				return true;
			} else {
				// Mouse-down event with a button we don't handle. Stay away from it.
				return false;
			}
		} else {
			// Modifier was up, so we keep our hands off this event.
			return false;
		}
	} else {
		// We're already dragging, and another button was pressed.
		// Naughty user shouldn't do this, but we pass the event anyway, because otherwise the application
		// might receive a mouse-up event without a preceding mouse-down event and get all confused.
		return false;
	}
}

/* Processes a mouse button release event.
 * Returns true if the event was processed and should not be passed to the application, false otherwise.
 */
bool processButtonUp(MouseButton button) {
	switch (currentState) {
		case dsNone:
			// Nothing going on, pass the event on.
			return false;
		case dsDragging:
			if (button == draggingButton) {
				// End of move or resize action.
				// Release the capture and eat the event.
				ReleaseCapture();
				currentState = dsNone;
				return true;
			} else {
				// Other button released during move event. (Naughty user!)
				return false;
			}
		case dsIgnoring:
			// Ignoring all events until the appropriate button was released.
			if (button == draggingButton) {
				// Well, guess what, it was!
				// But we still ignore this up event, of course.
				currentState = dsNone;
			}
			return true;
		default:
			// Should never be reached unless the currentState enum was tortured by inserting a pineapple into its butt.
			return false;
	}
}

/* The function for handling mouse events. This is the reason why we have to use a separate DLL;
 * see the SetWindowsHookEx documentation for details.
 */
LRESULT __declspec(dllexport) __stdcall mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	bool processed = false; // Set to true if we don't want to pass the event to the application.
	if (nCode >= 0 && nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
		MOUSEHOOKSTRUCT *eventInfo = (MOUSEHOOKSTRUCT*)lParam;
		switch (wParam) {
			case WM_LBUTTONDOWN:
				processed = processButtonDown(mbLeft, eventInfo);
				break;
			case WM_RBUTTONDOWN:
				processed = processButtonDown(mbRight, eventInfo);
				break;
			case WM_LBUTTONUP:
				processed = processButtonUp(mbLeft);
				break;
			case WM_RBUTTONUP:
				processed = processButtonUp(mbRight);
				break;
			case WM_MOUSEMOVE:
				int deltaX, deltaY;
				switch (currentState) {
					case dsDragging:
						// We are handling the moving or resizing of a window.
						// Find out the movement since the last known mouse position.
						deltaX = eventInfo->pt.x - lastMousePos.x, deltaY = eventInfo->pt.y - lastMousePos.y;
						if (draggingButton == mbLeft) {
							lastRect.left += deltaX;
							lastRect.top += deltaY;
							lastRect.right += deltaX;
							lastRect.bottom += deltaY;
						} else if (draggingButton == mbRight) {
							lastRect.right += deltaX;
							lastRect.bottom += deltaY;
						}
						// Now possibly the most important statement in the program: apply the movement/resizing to the window!
						SetWindowPos(draggedWindow, NULL,
							lastRect.left, lastRect.top, lastRect.right - lastRect.left, lastRect.bottom - lastRect.top,
							SWP_NOACTIVATE | SWP_NOOWNERZORDER);
						// No break here: fall through to the next case.
					case dsIgnoring:
						processed = true;
						break;
					case dsNone:
						break;
				}
				break;
		}
		lastMousePos = eventInfo->pt;
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (processed)
		res = 1; // nonzero return value prevents passing the event to the application
	return res;
}

/* The function for handling keyboard events.
 * Or rather, the function to eat keyboard events that the application shouldn't receive.
 */
LRESULT __declspec(dllexport) __stdcall keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
#ifdef DEBUG
	if (wParam == 0x51) {
		// Q button pressed. Panic button for debugging.
		PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
		return 1;
	}
#endif
	if (nCode >= 0 && nCode == HC_ACTION) {
		if (wParam == modifier) {
			// Something happened to the modifier key.
			bool wasDown = modifierDown;
			modifierDown = !(lParam & 0x80000000);
			//MessageBoxA(NULL, modifierDown?"keydown":"keyup", lParam&0x4000, MB_OK);
			if (wasDown && !modifierDown) {
				// Modifier was released. Only pass the event on if there was no drag event.
				if (haveDragged) {
					return 1;
				}
				haveDragged = false;
			} else if (!wasDown && modifierDown) {
				// Modifier was pressed. There has been no drag event since.
				haveDragged = false;
			}
		}
	}
	return CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
}
