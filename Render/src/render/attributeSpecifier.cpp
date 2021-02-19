
#include "render/attributeSpecifier.h"

AttributeSpecifier::AttributeSpecifier(int stride, int positionIndex, int normalIndex, int colourIndex) : STRIDE(stride), POSITION_INDEX(positionIndex), NORMAL_INDEX(normalIndex), COLOUR_INDEX(colourIndex)
{

}

bool AttributeSpecifier::hasAttribute(int attr)
{
	return attr != NONE;
}
