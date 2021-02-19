
#pragma once

class AttributeSpecifier
{
public:
	static const int NONE = -1;
	const int STRIDE, POSITION_INDEX, NORMAL_INDEX, COLOUR_INDEX;
public:
	AttributeSpecifier(int stride, int positionIndex, int normalIndex, int colourIndex);
	bool hasAttribute(int attr);
};
