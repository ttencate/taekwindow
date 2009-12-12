#ifndef NORMAL_HPP
#define NORMAL_HPP

#include "dragstate.hpp"

class NormalState : public DragState {

	public:

		virtual bool onMouseDown(MouseDownEvent const &event);

	private:

		/* Returns true if the modifier key (e.g. Alt) is currently pressed.
		 */
		bool isModifierDown() const;
};

#endif
