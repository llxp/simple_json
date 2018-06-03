#include "GeneratedJsonTestClass.h"

DLLEXPORT void JsonPair::serialize()
{
	ADD(first);
	ADD(last);
}

DLLEXPORT void GeneratedJsonTestClass::serialize()
{
	ADD(_id);
	ADD(index);
	ADD(guid);
	ADD(isActive);
	ADD(balance);
	ADD(picture);
	ADD(age);
	ADD(eyeColor);
	ADD(name);
	ADD(bools);
	ADD(strings);
}
