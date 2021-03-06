//
//  LDVideo.h
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

#ifndef _LDVideo_H_
#define _LDVideo_H_

#include <libio/libio.h>
#include "LDView.h"
#include "LDLabel.h"
#include "LDPopup.h"

class LDVideoModule : public IOModule
{
public:
	virtual bool publish();
	virtual void unpublish();

	void drawViews();

	LDView *rootView() { return _rootView; }
	LDView *debugLabel() { return _debugLabel; }

private:
	LDView *_rootView;
	LDLabel *_debugLabel;

	void printDebugMessage(const char *message);
	static void handleSyslogdMessage(const char *message);

	IODeclareClass(LDVideoModule)
};

#endif /* _LDVideo_H_ */
