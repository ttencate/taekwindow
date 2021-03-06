#ifndef DRAGWORKER_HPP
#define DRAGWORKER_HPP

#include <windows.h>

#include "cursorwindow.hpp"

class DragWorker {

	protected:
		
		CursorWindow d_cursorWindow;

	public:

		virtual void move(POINT const &mousePos) = 0;

};

#endif
