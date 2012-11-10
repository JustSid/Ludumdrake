//
//  IOModule.h
//  libio
//
//  Created by Sidney Just
//  Copyright (c) 2012 by Sidney Just
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

#ifndef _IOMODULE_H_
#define _IOMODULE_H_

#include "IOObject.h"
#include "IOArray.h"
#include "IOService.h"
#include "IOThread.h"

class IOModule : public IOObject
{
public:
	virtual IOModule *init();
	virtual void free();

	virtual bool publish();
	virtual void unpublish();

	IOThread *getThread() { return _thread; }

private:
	static void finalizePublish(IOThread *thread);
	void preparePublishing();

	IOThread *_thread;
	IOArray *_providers;
	bool _published;

	IODeclareClass(IOModule)
};

#define IOModuleRegister(class) \
	extern "C" { \
		void *IOModulePublish() \
		{ \
			IOModule *module = class::alloc()->init(); \
			return module; \
		} \
		void IOModuleUnpublish(IOModule *module) \
		{ \
			module->unpublish(); \
		} \
	} \
	
#endif /* _IOMODULE_H_ */
