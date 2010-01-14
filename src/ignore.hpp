#ifndef IGNORE_HPP
#define IGNORE_HPP

#include "dragworker.hpp"

class IgnoreWorker : public DragWorker {

	public:

		IgnoreWorker();

		virtual void move(POINT const &mousePos);

};

#endif
