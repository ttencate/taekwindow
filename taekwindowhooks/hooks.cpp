#include <windows.h>

extern "C" {
	
	__declspec(dllexport) LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
		if (nCode > 0) { // if nCode < 0, do nothing as per Microsoft's recommendations
			bool altPressed = true;
			if (altPressed) {
				// if Alt is pressed, we want to know which mouse button is down, if any
				MOUSEHOOKSTRUCT *eventInfo = (MOUSEHOOKSTRUCT*)lParam;
				MessageBoxW(NULL, L"Alt is down!", L"Hello", MB_OK);
			}
		}
		// call next hook in chain only if we haven't returned by now
		return CallNextHookEx((HHOOK)37 /* ignored */, nCode, wParam, lParam);
	}

}
