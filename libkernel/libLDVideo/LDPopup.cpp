//
//  LDPopup.h
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

#include "LDPopup.h"

#ifdef super
#undef super
#endif
#define super LDView

IORegisterClass(LDPopup, super);

#define kLDPopupWidth 70
#define kLDPopupLeft  5

LDPopup *LDPopup::initWithText(IOString *text, IOString *title)
{
	uint32_t lines = 3;
	uint32_t row = 1;
	const char *ctext = text->CString();

	while(*ctext != '\0')
	{
		if(*ctext == '\n')
		{
			row = 1;
			lines ++;
		}
		else
		{
			row ++;

			if(row >= (kLDPopupWidth - 2))
			{
				row = 1;
				lines ++;
			}
		}

		ctext ++;
	}

	uint32_t y = (25 - lines) / 2;
	LDFrame frame = LDFrameMake(kLDPopupLeft, y, kLDPopupWidth, lines);

	if(!super::initWithFrame(frame))
		return 0;

	_text = text->retain();
	_title = title->retain();

	setDrawBorder(true);

	return this;
}

void LDPopup::free()
{
	_text->release();
	_title->release();

	super::free();
}

bool LDPopup::handleEvent(uint8_t UNUSED(scancode), char key)
{
	if(key == '\n')
	{
		close();
		return true;
	}

	return false;
}

bool LDPopup::canBecomeFirstResponder()
{
	return true;
}

void LDPopup::show()
{
	if(!_visible)
	{
		LDView *root = LDView::rootView();
		root->addSubview(this);

		_visible = true;

		becomeFirstResponder();
	}
}

void LDPopup::close()
{
	if(_visible)
	{
		_visible = false;
		
		retain();
		autorelease();

		removeFromSuperview();
	}
}

void LDPopup::draw()
{
	super::draw();

	if(_title)
	{
		uint32_t x = 3;
		const char *title = _title->CString();

		while(*title != '\0')
		{
			setCharacter(x ++, 0, *title);
			title ++;
		}
	}

	uint32_t x = 1;
	uint32_t y = 1;

	const char *text = _text->CString();
	while(*text != '\0')
	{
		if(*text == '\n')
		{
			x = 1;
			y ++;
		}
		else
		{
			if(x == 1 && *text == ' ')
			{
				text ++;
				continue;
			}

			setCharacter(x, y, *text);
			setColor(x, y, LDConstants::colorLightGray);

			x ++;
		}

		if(x >= (kLDPopupWidth - 2))
		{
			x = 1;
			y ++;
		}

		text ++;
	}
}
