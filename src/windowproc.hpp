#ifndef WINDOWPROC_HPP
#define WINDOWPROC_HPP

#include <windows.h>

class WindowProcFunc {
	public:
		virtual LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
		virtual WindowProcFunc *clone() const = 0;
};

template<typename Receiver>
class WindowProcFwd : public WindowProcFunc {

	public:

		typedef LRESULT (Receiver::*Proc)(HWND, UINT, WPARAM, LPARAM);

		WindowProcFwd(Receiver &receiver, Proc proc)
		:
			d_receiver(&receiver),
			d_proc(proc)
		{	
		}

		virtual LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			return (d_receiver->*d_proc)(hwnd, msg, wParam, lParam);
		}

		virtual WindowProcFunc *clone() const {
			return new WindowProcFwd(*this);
		}

	private:

		Receiver *d_receiver;
		Proc d_proc;

};

#endif
