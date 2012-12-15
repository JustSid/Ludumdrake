//
//  LDView.h
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

#ifndef _LDVIEW_H_
#define _LDVIEW_H_

#include <libio/libio.h>
#include "LDConstants.h"

struct LDFrame
{
	uint32_t x, y;
	uint32_t width, height;
};

static inline LDFrame LDFrameMake(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	LDFrame frame;
	frame.x = x;
	frame.y = y;
	frame.width  = width;
	frame.height = height;
	return frame;
}

class LDVideoModule;
class LDView : public IOObject
{
friend class LDVideoModule;
public:
	virtual LDView *initWithFrame(const LDFrame& frame);

	void setNeedsRedraw();
	void redrawIfNeeded();

	virtual void draw();

	void addSubview(LDView *subview);
	void insertSubview(LDView *subview, uint32_t index);
	void removeFromSuperview();

	void setDrawBorder(bool drawBorder);
	void setHidden(bool hidden);

	bool drawBorder() { return _drawBorder; }
	bool hidden() { return _hidden; }

	virtual void setFrame(const LDFrame& frame);
	LDFrame frame();
	LDFrame bounds();

protected:
	void setCharacter(uint32_t x, uint32_t y, char c);
	void setColor(uint32_t x, uint32_t y, LDColor color);

	LDFrame _frame;
	uint8_t *_backbuffer;
	LDColor _backgroundColor;

private:
	void allocateBackbuffer();

	LDView *_parent;
	IOArray *_subviews;

	bool _drawBorder;
	bool _needsRedraw;
	bool _hidden;

	IODeclareClass(LDView)
};

inline void LDView::setCharacter(uint32_t x, uint32_t y, char c)
{
	uint32_t offset = (y * _frame.width + x) * 2;
	_backbuffer[offset] = c;
}

inline void LDView::setColor(uint32_t x, uint32_t y, LDColor color)
{
	uint32_t offset = (y * _frame.width + x) * 2;
	_backbuffer[offset + 1] = color;
}

#endif
