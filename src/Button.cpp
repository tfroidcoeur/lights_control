/*
 * Button.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "Button.h"

std::ostream &operator<<(std::ostream &os, const ButtonMode & m) {
	return os << "mode " << (m.name ? m.name:"NULL") << "(" << m.delay << ","
			<< static_cast<void*>(m.pressed) << ")";
}

