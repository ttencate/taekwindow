#ifndef MOVEMAX_HPP
#define MOVEMAX_HPP

#include "deform.hpp"

class MoveMaxWorker : public DeformWorker {

	/* The monitor that the window is currently on.
	 */
	HMONITOR d_currentMonitor;

	public:

		MoveMaxWorker(POINT mousePos, HWND window);

		virtual void move(POINT const &mousePos);

	private:

		void moveToMonitor(HMONITOR monitor);

};

#endif
