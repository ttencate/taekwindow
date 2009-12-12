#ifndef IGNORE_HPP
#define IGNORE_HPP

#include "mousedown.hpp"

class IgnoreState : public MouseDownState {

	public:

		IgnoreState(MouseButton button);

		virtual void enter();

};

#endif
