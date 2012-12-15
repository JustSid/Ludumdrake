//
//  LDLabel.cpp
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

#include "LDLabel.h"

#ifdef super
#undef super
#endif
#define super LDView

IORegisterClass(LDLabel, super);

LDLabel *LDLabel::initWithFrame(const LDFrame &frame)
{
	if(!super::initWithFrame(frame))
		return 0;

	_string = IOString::alloc()->initWithCString("");
	return this;
}

void LDLabel::draw()
{
	super::draw();

	uint32_t x = 1;
	uint32_t y = 1;

	const char *string = _string->CString();
	const char *temp = string + strlen(string);

	size_t offset = 0;
	size_t lines = 0;

	int inset = drawBorder() ? 1 : 0;

	while(temp >= string)
	{
		if(*temp == '\n')
		{
			if((++ lines) >= _frame.height - inset)
			{
				offset = (temp + 1) - string;
				break;
			}
		}

		temp --;
	}

	string += offset;
	while(*string != '\0')
	{
		if(*string == '\n')
		{
			x = 1;
			y ++;
		}
		else
		{
			setCharacter(x, y, *string);
			x ++;
		}

		if(x >= _frame.width - inset)
		{
			y ++;
			x = 1;
		}

		if(y >= _frame.height - inset)
			break;

		string ++;
	}
}

void LDLabel::setString(IOString *string)
{
	_string->release();
	_string = string->retain();

	setNeedsRedraw();
}

void LDLabel::appendString(IOString *string)
{
	size_t maxLength = (_frame.width * _frame.height);
	IOString *temp = IOString::withFormat("%s%s", _string->CString(), string->CString());

	if(temp->length() > maxLength)
	{
		const char *cstring = temp->CString();
		cstring = &cstring[temp->length() - maxLength];

		temp = IOString::withCString(cstring);
	}

	_string->release();
	_string = temp->retain();

	setNeedsRedraw();
}

IOString *LDLabel::string()
{
	return _string;
}
