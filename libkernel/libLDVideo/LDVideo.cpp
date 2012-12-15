//
//  LDVideo.cpp
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

#include "LDVideo.h"

#ifdef super
#undef super
#endif
#define super IOModule

IORegisterModule(LDVideoModule);
IORegisterClass(LDVideoModule, super);

typedef void (*syslogd_message_handler_t)(const char *);
extern "C" void syslogd_setMessageHandler(syslogd_message_handler_t handler);
extern "C" void vd_setCursor(uint32_t x, uint32_t y);

static uint8_t *LDVideoMemory = (uint8_t *)0xB8000;
static LDVideoModule *sharedModule = 0;

#define kLDVideoWidth  80
#define kLDVideoHeight 25

bool LDVideoModule::publish()
{
	if(!super::publish())
		return false;

	sharedModule = this;

	syslogd_setMessageHandler(&LDVideoModule::handleSyslogdMessage);
	vd_setCursor(0, 0);
	memset(LDVideoMemory, 0, kLDVideoHeight * kLDVideoWidth * 2);

	_rootView = LDView::alloc()->initWithFrame(LDFrameMake(0, 0, kLDVideoWidth, kLDVideoHeight));
	_rootView->_backgroundColor = LDConstants::colorBlue;

	LDView *test = LDView::alloc()->initWithFrame(LDFrameMake(10, 5, 20, 5));
	test->_backgroundColor = LDConstants::colorRed;
	test->setDrawBorder(true);

	_rootView->addSubview(test);
	
	_eventSource = IOTimerEventSource::alloc()->initWithDate(IODate::alloc()->initWithTimestampSinceNow(5), true);
	_eventSource->setAction(this, IOMemberFunctionCast(IOTimerEventSource::Action, this, &LDVideoModule::drawViews));

	runLoop()->addEventSource(_eventSource);

	return true;
}


void LDVideoModule::drawViews()
{
	_rootView->redrawIfNeeded();
	memcpy(LDVideoMemory, _rootView->_backbuffer, kLDVideoWidth * kLDVideoHeight * 2);
}


void LDVideoModule::printDebugMessage(const char *message)
{
	while(*message != '\0')
	{
		if(*message == '\n')
		{
			cursorY ++;
			cursorX = 0;

			message ++;
		}

		uint32_t index = (cursorY * kLDVideoWidth + cursorX) * 2;
		LDVideoMemory[index] = *message;
		LDVideoMemory[index + 1] = 0x7;

		cursorX ++;
		message ++;
	}
}

void LDVideoModule::handleSyslogdMessage(const char *message)
{
	sharedModule->printDebugMessage(message);
}

void LDVideoModule::unpublish()
{
}
