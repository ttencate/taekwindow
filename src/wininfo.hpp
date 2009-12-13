#ifndef WININFO_HPP
#define WININFO_HPP

/* Returns the first parent in the parent chain of the given window
 * that meets the given criterium, or NULL if there is no such parent.
 */
HWND findFirstParent(HWND window, bool (*criterium)(HWND));

/* Returns the last parent in the parent chain of the given window
 * that meets the given criterium, or NULL if there is no such parent.
 */
HWND findLastParent(HWND window, bool (*criterium)(HWND));

/* Returns true if the given window has the given window class name.
 */
bool windowHasClass(HWND window, wchar_t const *className);

/* Returns true if the window is currently maximized.
 */
bool isMaximizedWindow(HWND window);

/* Returns true if the window is fullscreen.
 */
bool isFullscreenWindow(HWND window);

/* Returns true if the window has the WS_CAPTION style.
 */
bool isCaptionWindow(HWND window);

/* Returns true if the window has a thick border (i.e. is resizable).
 */
bool isThickBorderWindow(HWND window);

/* Returns true if we should allow moving of this window (be it maximized or not).
 */
bool isMovableWindow(HWND window);

/* Returns true if we should allow resizing of this window (be it maximized or not).
 */
bool isResizableWindow(HWND window);

/* Various combinations of movability, resizability and being maximized or not.
 */
bool isRestoredMovableWindow(HWND window);
bool isRestoredResizableWindow(HWND window);
bool isMaximizedMovableWindow(HWND window);
bool isMaximizedResizableWindow(HWND window);

/* Activates a window, then restores its original position in the Z order.
 * This has the effect of activating it without bringing it to the front,
 * something that the Windows API normally doesn't allow.
 */
void activateWithoutRaise(HWND window);

#endif
