//
//  LDGameView.h
//  libLDGame
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

#ifndef _LDGAMEVIEW_H_
#define _LDGAMEVIEW_H_

#include <libio/libio.h>
#include <libLDVideo/LDVideo.h>

#include "LDIsland.h"

class LDGameView : public LDView
{
public:
	virtual LDGameView *initWithFrame(const LDFrame &frame);

	virtual bool canBecomeFirstResponder() { return true; }
	virtual bool handleEvent(uint8_t scancode, char key);

	virtual void draw();
	void tick();

private:
	void selectIsland(int32_t index);

	unix_time_t _lastTick;
	int _tick;

	IOArray *_playerIslands;
	IOArray *_islands;

	LDIsland *_selectedIsland;
	int32_t _selected;

	IODeclareClass(LDGameView)
};

#endif
