#ifndef MOVE_HPP
#define MOVE_HPP

#include "deform.hpp"

class MoveWorker : public DeformWorker {

	/* May be NULL.
	 */
	HWND d_parent;

	/* Screen coordinates.
	 */
	POINT d_windowPos;

	public:

		MoveWorker(POINT mousePos, HWND window);

		virtual void move(POINT const &mousePos);

	private:

		void moveWindow();

};

#endif