#include <windows.h>

extern "C" {

	__declspec(dllexport) LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);

}
