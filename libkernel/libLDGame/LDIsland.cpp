//
//  LDIsland.cpp
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

#include "LDIsland.h"
#include "LDRandom.h"

#ifdef super
#undef super
#endif
#define super LDView

IORegisterClass(LDIsland, super);

#define kLDIslandMinWidth  5
#define kLDIslandMinHeight 2

LDIsland *LDIsland::initWithFrame(const LDFrame &frame)
{
	if(!super::initWithFrame(frame))
		return 0;

	_independent = true;

	return this;
}

void LDIsland::takeIsland()
{
	_independent = false;
}

void LDIsland::draw()
{
	super::draw();

	LDFrame frame = this->frame();

	uint32_t centerX = frame.width / 2;
	uint32_t centerY = frame.height / 2;

	for(uint32_t x=0; x<frame.width; x++)
	{
		for(uint32_t y=0; y<frame.height; y++)
		{
			if(x == centerX && y == centerY)
			{
				LDColor color = (_independent) ? LDConstants::colorBlue : LDConstants::colorRed;

				setCharacter(x, y, 219);
				setColor(x, y, color);

				continue;
			}

			uint32_t i = (y * frame.width) + x;
			uint8_t character = 219;

			switch(_tiles[i])
			{
				case TileTypeSand:
					setCharacter(x, y, character);
					setColor(x, y, LDConstants::colorYellow);
					break;

				case TileTypeGrass:
					setCharacter(x, y, character);
					setColor(x, y, LDConstants::colorLightGreen);
					break;

				case TileTypeForest:
					setCharacter(x, y, character);
					setColor(x, y, LDConstants::colorGreen);
					break;

				case TileTypeMountain:
					setCharacter(x, y, character);
					setColor(x, y, LDConstants::colorDarkGray);
					break;

				case 0:
				default:
					setColor(x, y, LDConstants::colorTransparent);
					break;
			}
		}
	}
}

void LDIsland::generate(uint32_t maxWidth, uint32_t maxHeight)
{
	uint32_t width  = kLDIslandMinWidth + LDRandom() % (maxWidth - kLDIslandMinWidth);
	uint32_t height = kLDIslandMinHeight + LDRandom() % (maxHeight - kLDIslandMinHeight);

	LDFrame newFrame = frame();
	newFrame.width  = width;
	newFrame.height = height;

	setFrame(newFrame);

	_tiles = (uint8_t *)kalloc(width * height * sizeof(uint8_t));

	int32_t centerX = (int32_t)(width / 2);
	int32_t centerY = (int32_t)(height / 2);

	uint32_t temp = (uint32_t)(centerX + centerY);

	for(uint32_t x=0; x<width; x++)
	{
		for(uint32_t y=0; y<height; y++)
		{
			uint32_t i = (y * width) + x;

			int32_t distX = centerX - ((int32_t)x);
			int32_t distY = centerY - ((int32_t)y);

			distX = (distX < 0) ? -distX : distX;
			distY = (distY < 0) ? -distY : distY;

			uint32_t distance = (uint32_t)(distX + distY);

			bool place = (((LDRandom() % temp) + 2) >= distance);
			if(place)
			{
				_tiles[i] = TileTypeSand;

				uint32_t temp2 = (temp / 2) - 2;

				if(distance <= temp2)
				{
					_tiles[i] = TileTypeGrass;

					if(distance <= temp2 - 2)
					{
						_tiles[i] = TileTypeForest;

						if(distance <= temp2 - 4)
						{
							_tiles[i] = TileTypeMountain;
						}
					}
				}

			}
			else
			{
				_tiles[i] = 0;
			}
		}
	}
}
