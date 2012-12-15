//
//  LDInput.cpp
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

#include "LDInput.h"

#ifdef super
#undef super
#endif
#define super IOModule

IORegisterModule(LDInputModule);
IORegisterClass(LDInputModule, super);

void LDInputModule::handleInput()
{
	uint8_t scancode = inb(0x60);

	bool isKeyDown = !(scancode & 0x80);
	uint8_t unmasked = isKeyDown ? scancode : scancode & ~0x80;
	char character = '\0';

	if(isKeyDown && !_shiftPressed)
	{
		if((unmasked == 0x2A || unmasked == 0x36))
		{
			_shiftPressed = true;
			return;
		}
	}
	else if(!isKeyDown && _shiftPressed)
	{
		if(!(unmasked == 0x2A || unmasked == 0x36))
		{
			_shiftPressed = false;
			return;
		}
	}

	switch(unmasked)
	{
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			character = (unmasked - 2) + '1';
			break;

		case 11:
			character = '0';
			break;

		case 12:
			character = '-';
			break;
		case 13:
			character = '=';
			break;

		case 28:
			character = '\n';
			break;
		case 57:
			character = ' ';
			break;

		case 16:
			character = 'q';
			break;
		case 17:
			character = 'w';
			break;
		case 18:
			character = 'e';
			break;
		case 19:
			character = 'r';
			break;
		case 20:
			character = 't';
			break;
		case 21:
			character = 'y';
			break;
		case 22:
			character = 'u';
			break;
		case 23:
			character = 'i';
			break;
		case 24:
			character = 'o';
			break;
		case 25:
			character = 'p';
			break;

		case 30:
			character = 'a';
			break;
		case 31:
			character = 's';
			break;
		case 32:
			character = 'd';
			break;
		case 33:
			character = 'f';
			break;
		case 34:
			character = 'g';
			break;
		case 35:
			character = 'h';
			break;
		case 36:
			character = 'j';
			break;
		case 37:
			character = 'k';
			break;
		case 38:
			character = 'l';
			break;

		case 44:
			character = 'z';
			break;
		case 45:
			character = 'x';
			break;
		case 46:
			character = 'c';
			break;
		case 47:
			character = 'v';
			break;
		case 48:
			character = 'b';
			break;
		case 49:
			character = 'n';
			break;
		case 50:
			character = 'm';
			break;

		case 52:
			character = '.';
			break;

		default:
			break;
	}

	if(_shiftPressed)
	{
		switch(unmasked)
		{
			case 2:
				character = '!';
				break;
			case 3:
				character = '@';
				break;
			case 4:
				character = '#';
				break;
			case 5:
				character = '$';
				break;
			case 6:
				character = '%';
				break;
			case 7:
				character = '^';
				break;
			case 8:
				character = '&';
				break;
			case 9:
				character = '*';
				break;
			case 10:
				character = '(';
					break;
			case 11:
				character = ')';
				break;
			case 12:
				character = '_';
				break;
			case 13:
				character = '+';
				break;

			default:
				break;
		}

		character = toupper(character);
	}

	if(isKeyDown && _keyDownCommand)
	{
		_keyDownCommand->executeCommand((void *)&unmasked, (void *)&character);
	}

	if(!isKeyDown && _keyUpCommand)
	{
		_keyUpCommand->executeCommand((void *)&unmasked, (void *)&character);
	}
}

void LDInputModule::setKeyDownCommand(IORemoteCommand *command)
{
	_keyDownCommand->release();
	_keyDownCommand = command->retain();
}

void LDInputModule::setKeyUpCommand(IORemoteCommand *command)
{
	_keyUpCommand->release();
	_keyUpCommand = command->retain();
}

bool LDInputModule::publish()
{
	if(!super::publish())
		return false;

	_interruptSource = IOInterruptEventSource::alloc()->initWithInterrupt(0x21);
	_interruptSource->setAction(this, IOMemberFunctionCast(IOInterruptEventSource::Action, this, &LDInputModule::handleInput));

	runLoop()->addEventSource(_interruptSource);

	while(inb(0x64) & 0x1)
		inb(0x60);

	while((inb(0x64) & 0x2)) {}
	outb(0x60, 0xF4);

	return true;
}

void LDInputModule::unpublish()
{
}
