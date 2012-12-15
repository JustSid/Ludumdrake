//
//  LDResponder.cpp
//  libLDInput
//
//  Created by Sidney
//  Copyright (c) 2012 by Sidney
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "LDResponder.h"

#ifdef super
#undef super
#endif
#define super IOObject

IORegisterClass(LDResponder, super);

static LDResponder *_LDFirstResponder = 0;

void LDResponderDispatchKeyEvent(uint8_t scancode, char character)
{
	if(_LDFirstResponder)
	{
		if(!_LDFirstResponder->handleEvent(scancode, character))
		{
			// TODO: Handle responder chain changes...
		}
	}
}

LDResponder *firstResponder()
{
	return _LDFirstResponder;
}

bool LDResponder::canBecomeFirstResponder()
{
	return false;
}

LDResponder *LDResponder::nextResponder()
{
	return 0;
}

bool LDResponder::handleEvent(uint8_t UNUSED(scancode), char UNUSED(character))
{
	return false;
}

bool LDResponder::isFirstResponder()
{
	return (this == _LDFirstResponder);
}

bool LDResponder::becomeFirstResponder()
{
	if(canBecomeFirstResponder())
	{
		_previousResponder = _LDFirstResponder;
		_LDFirstResponder = this;
	}

	return false;
}

void LDResponder::resignFirstResponder()
{
	if(isFirstResponder())
	{
		_previousResponder->becomeFirstResponder();
	}
}
