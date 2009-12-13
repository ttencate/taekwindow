#include "testapp.hpp"

HHOOK msgHook;
bool alwaysCaption = false;

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	log("> WindowProc received %s", messageName(uMsg));
	indent();
	LRESULT retval;
	switch (uMsg) {
		case WM_CLOSE:
			PostQuitMessage(0);
			retval = 0;
			break;
		case WM_KEYDOWN:
			if (wParam == 'M') {
				log("+ Posting WM_SYSCOMMAND with SC_MOVE");
				PostMessage(hwnd, WM_SYSCOMMAND, SC_MOVE, 0);
			} else if (wParam == 'S') {
				log("+ Posting WM_SYSCOMMAND with SC_SIZE");
				PostMessage(hwnd, WM_SYSCOMMAND, SC_SIZE, 0);
			} else if (wParam == 'H') {
				alwaysCaption = !alwaysCaption;
				if (alwaysCaption) {
					log("+ Overriding response to WM_NCHITTEST");
				} else {
					log("+ WM_NCHITTEST behaviour returned to normal");
				}
			}
			retval = 0;
			break;
		default:
			log("> WindowProc calling DefaultWindowProc for %s", messageInfo(uMsg, wParam, lParam));
			indent();
			retval = DefWindowProc(hwnd, uMsg, wParam, lParam);
			outdent();
			log("< DefaultWindowProc returned %s", retValInfo(retval, uMsg));
			if (alwaysCaption && uMsg == WM_NCHITTEST) {
				retval = HTRIGHT;
				log("+ Modifying return value to %s", retValInfo(retval, uMsg));
			}
			break;
	}
	outdent();
	log("< WindowProc returning %s from %s", retValInfo(retval, uMsg), messageName(uMsg));
	return retval;
}

LRESULT CALLBACK getMsgProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code >= 0) {
		log("! Got %smessage %s", wParam == PM_REMOVE ? "and removed " : "", messageInfo((MSG*)lParam));
	}
	return CallNextHookEx(msgHook, code, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	startLogging();

	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS;
	windowClass.lpfnWndProc = &windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = NULL;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)COLOR_BTNFACE;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"VictimWindow";
	windowClass.hIconSm = NULL;
	ATOM classAtom = RegisterClassEx(&windowClass);

	HWND hwnd = CreateWindowEx(0, (LPCWSTR)classAtom, L"Taekwindow Victim Application", WS_OVERLAPPEDWINDOW | WS_SIZEBOX | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

	msgHook = SetWindowsHookEx(WH_GETMESSAGE, &getMsgProc, hInstance, GetCurrentThreadId());

	log("Entering message loop");
	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		log("> Dispatching message %s", messageInfo(&message));
		indent();
		TranslateMessage(&message);
		DispatchMessage(&message);
		outdent();
		log("< Dispatched message %s", messageName(message.message));
	}
	log("Message loop terminated");

	UnhookWindowsHookEx(msgHook);

	endLogging();
}
