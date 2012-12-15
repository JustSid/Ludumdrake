//
//  LDView.cpp
//  libLDVideo
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

#include "LDView.h"

#ifdef super
#undef super
#endif
#define super IOObject

IORegisterClass(LDView, super);

LDView *LDView::initWithFrame(const LDFrame& frame)
{
	if(!super::init())
		return 0;

	_frame = frame;
	_backgroundColor = LDConstants::colorLightGray;
	_subviews = IOArray::alloc()->init();

	allocateBackbuffer();

	return this;
}

void LDView::setFrame(const LDFrame& frame)
{
	_frame = frame;
	allocateBackbuffer();
}

LDFrame LDView::frame()
{
	return _frame;
}

LDFrame LDView::bounds()
{
	return LDFrameMake(0, 0, _frame.width, _frame.height);
}



void LDView::addSubview(LDView *subview)
{
	subview->removeFromSuperview();

	_subviews->addObject(subview);
	subview->_parent = this;

	setNeedsRedraw();
}

void LDView::removeFromSuperview()
{
	if(_parent)
	{
		_parent->_subviews->removeObject(this);
		_parent->setNeedsRedraw();
		_parent = 0;
	}
}


void LDView::setDrawBorder(bool drawBorder)
{
	_drawBorder = drawBorder;
	setNeedsRedraw();
}

void LDView::setNeedsRedraw()
{
	_needsRedraw = true;

	if(_parent)
		_parent->setNeedsRedraw();
}

void LDView::redrawIfNeeded()
{
	if(_needsRedraw)
	{
		_needsRedraw = false;
		draw();
	}
}

void LDView::draw()
{
	for(uint32_t x=0; x<_frame.width; x++)
	{
		for(uint32_t y=0; y<_frame.height; y++)
		{
			uint32_t offset = (y * _frame.width + x) * 2;

			_backbuffer[offset+0] = ' ';
			_backbuffer[offset+1] = _backgroundColor;

			if(_drawBorder)
			{
				if(x == 0 || x == _frame.width - 1)
				{
					_backbuffer[offset+0] = 186;
					_backbuffer[offset+1] = LDConstants::colorLightGray;
				}

				if(y == 0 || y == _frame.height -1)
				{
					_backbuffer[offset+0] = 205;
					_backbuffer[offset+1] = LDConstants::colorLightGray;
				}



				if(x == 0 && y == 0)
				{
					_backbuffer[offset+0] = 201;
					_backbuffer[offset+1] = LDConstants::colorLightGray;
				}

				if(x == 0 && y == _frame.height - 1)
				{
					_backbuffer[offset+0] = 200;
					_backbuffer[offset+1] = LDConstants::colorLightGray;
				}

				if(x == _frame.width - 1 && y == _frame.height - 1)
				{
					_backbuffer[offset+0] = 188;
					_backbuffer[offset+1] = LDConstants::colorLightGray;
				}

				if(x == _frame.width - 1 && y == 0)
				{
					_backbuffer[offset+0] = 187;
					_backbuffer[offset+1] = LDConstants::colorLightGray;
				}
			}
		}
	}

	for(uint32_t i=0; i<_subviews->count(); i++)
	{
		LDView *subview = (LDView *)_subviews->objectAtIndex(i);
		subview->redrawIfNeeded();

		LDFrame frame = subview->frame();
		size_t width = frame.width * 2;

		for(uint32_t i=0; i<frame.height; i++)
		{
			uint32_t offsetVMem = ((frame.y + i) * _frame.width + frame.x) * 2;
			uint32_t offsetView = i * width;

			uint8_t *backbuffer = subview->_backbuffer;
			memcpy(&_backbuffer[offsetVMem], &backbuffer[offsetView], frame.width * 2);
		}
	}
}




void LDView::allocateBackbuffer()
{
	if(_backbuffer)
		kfree(_backbuffer);

	size_t size = _frame.width * _frame.height * 2;

	_backbuffer = (uint8_t *)kalloc(size);
	setNeedsRedraw();

	if(!_backbuffer)
		panic("Couldn't allocate memory for backingbuffer!");
}
