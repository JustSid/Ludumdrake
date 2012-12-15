//
//  LDIsland.h
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

#ifndef _LDISLAND_H_
#define _LDISLAND_H_

#include <libio/libio.h>
#include <libLDVideo/LDVideo.h>

class LDIsland : public LDView
{
public:
	virtual LDIsland *initWithFrame(const LDFrame &frame);

	virtual void draw();

	void generate(uint32_t maxWidth, uint32_t maxHeight);
	void takeIsland();

private:
	enum
	{
		TileTypeSand = 1,
		TileTypeGrass,
		TileTypeForest,
		TileTypeMountain
	};

	bool _independent;
	uint8_t *_tiles;

	IODeclareClass(LDIsland)
};

#endif
