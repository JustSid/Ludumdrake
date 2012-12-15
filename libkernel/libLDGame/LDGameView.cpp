//
//  LDGameView.cpp
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

#include "LDGameView.h"
#include "LDRandom.h"

#ifdef super
#undef super
#endif
#define super LDView

IORegisterClass(LDGameView, super);


#define kMinIslands 8
#define kMaxIslands 12

LDGameView *LDGameView::initWithFrame(const LDFrame &frame)
{
	if(!super::initWithFrame(frame))
		return 0;

	_islands = IOArray::alloc()->init();
	_playerIslands = IOArray::alloc()->init();

	uint32_t islands = LDRandomRange(kMinIslands, kMaxIslands);
	for(uint32_t i=0; i<islands; i++)
	{
		LDIsland *island = LDIsland::alloc()->initWithFrame(LDFrameMake(0, 0, 1, 1));
		
		while(1)
		{
			island->generate(LDRandomRange(10, 20), LDRandomRange(10, 20));
			LDFrame frame = island->frame();

			uint32_t x = LDRandomRange(0, 80 - frame.width);
			uint32_t y = LDRandomRange(0, 25 - frame.height);

			frame = LDFrameMake(x, y, frame.width, frame.height);

			bool fitsInPlace = true;

			for(uint32_t j=0; j<_islands->count(); j++)
			{
				LDIsland *other = (LDIsland *)_islands->objectAtIndex(j);

				if(LDFrameIntersects(frame, other->frame()))
				{
					fitsInPlace = false;
					break;
				}
			}

			if(fitsInPlace)
			{
				island->setFrame(frame);
				break;
			}
		}

		addSubview(island);
		_islands->addObject(island);
	}

	_selected = (LDRandom() % islands);
	_selectedIsland = (LDIsland *)_islands->objectAtIndex(_selected);

	_selectedIsland->takeIsland();
	_playerIslands->addObject(_selectedIsland);

	selectIsland(_selected);

	return this;
}

void LDGameView::selectIsland(int32_t index)
{
	_selectedIsland->setSelected(false);
	_selected = index;

	if(_selected < 0)
		_selected = _islands->count() - 1;

	if(_selected >= (int32_t)_islands->count())
		_selected = 0;

	_selectedIsland = (LDIsland *)_islands->objectAtIndex(_selected);
	_selectedIsland->setSelected(true);

	IOLog("Selected island %i", _selected);

	setNeedsRedraw();
}

bool LDGameView::handleEvent(uint8_t scancode, char UNUSED(key))
{
	switch(scancode)
	{
		case 75:
			selectIsland(_selected + 1);
			break;

		case 77:
			selectIsland(_selected - 1);
			break;

		default:
			IOLog("Scancode: %i", scancode);
			break;
	}

	return true;
}

void LDGameView::tick()
{
	unix_time_t time = time_getUnixTime();
	if(time != _lastTick)
	{
		_lastTick = time;
		_tick ++;
		_tick %= 3;

		setNeedsRedraw();
	}
}

void LDGameView::draw()
{
	super::draw();

	for(uint32_t x=0; x<_frame.width; x++)
	{
		for(uint32_t y=0; y<_frame.height; y++)
		{
			setCharacter(x, y, 176 + _tick);
			setColor(x, y, LDConstants::colorBlue);
		}
	}
}
