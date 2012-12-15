//
//  LDGame.cpp
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

#include "LDGame.h"

#ifdef super
#undef super
#endif
#define super IOModule

#define kMinIslands 8
#define kMaxIslands 12

IORegisterModule(LDGameModule);
IORegisterClass(LDGameModule, super);

void LDGameModule::gameTick()
{
	if(!_gameView)
		return;

	_gameView->tick();
	_renderer->drawViews();
}

void LDGameModule::handleInput(uint8_t *_scancode, char *_character)
{
	char character = *_character;
	uint8_t scancode = *_scancode;

	if(character != '\0')
	{
		printf("%c", character);
	}
	else
	{
		IOLog("Scancode: %x", scancode);
	}
}

void LDGameModule::initGame()
{
	_gameView = LDGameView::alloc()->initWithFrame(LDFrameMake(0, 0, 80, 25));
	_renderer->rootView()->insertSubview(_gameView, 0);

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

		_gameView->addSubview(island);
		_islands->addObject(island);
	}

	LDIsland *startIsland = (LDIsland *)_islands->objectAtIndex((LDRandom() % islands));
	startIsland->takeIsland();
	_playerIslands->addObject(startIsland);
}

bool LDGameModule::publish()
{
	if(!super::publish())
		return false;

	IOThread::currentThread()->setName(IOString::withCString("LD Game"));

	_renderer = (LDVideoModule *)IOModule::withName("libLDVideo.so");
	_inputManager = (LDInputModule *)IOModule::withName("libLDInput.so");

	_renderer->waitForPublish();
	_inputManager->waitForPublish();


	LDRandomSeed((uint32_t)time_getUnixTime());

	_islands = IOArray::alloc()->init();
	_playerIslands = IOArray::alloc()->init();

	_rendererCommand = IORemoteCommand::alloc()->init();
	_rendererCommand->setAction(this, 0);

	_heartbeat = IOTimerEventSource::alloc()->initWithDate(IODate::alloc()->initWithTimestampSinceNow(5), true);
	_heartbeat->setAction(this, IOMemberFunctionCast(IOTimerEventSource::Action, this, &LDGameModule::gameTick));

	_renderer->runLoop()->addEventSource(_heartbeat);
	_renderer->runLoop()->addEventSource(_rendererCommand);

	_rendererCommand->executeAction(IOMemberFunctionCast(IORemoteCommand::Action, this, &LDGameModule::initGame));

	// Input command handlers
	IORemoteCommand *keyDown = IORemoteCommand::alloc()->init();
	keyDown->setAction(this, IOMemberFunctionCast(IORemoteCommand::Action, this, &LDGameModule::handleInput));

	_renderer->runLoop()->addEventSource(keyDown);
	_inputManager->setKeyDownCommand(keyDown);

	return true;
}

void LDGameModule::unpublish()
{
}
