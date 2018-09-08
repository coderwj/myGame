#include "Renderer.h"

#include "RenderObject.h"
#include "Camera.h"
#include "Engine.h"
#include "Config.h"
#include "HelperFunc.h"

#include "bgfx/bgfx.h"
#include "bx/math.h"

static const uint8_t s_logo[4000] =
{
	0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // ........ . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x08, //  . . . . . . ...
	0xdc, 0x03, 0xdc, 0x07, 0xdc, 0x07, 0xdc, 0x08, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // ........ . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0xde, 0x03, 0xb0, 0x3b, 0xb1, 0x3b, 0xb2, 0x3b, 0xdb, 0x3b, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // ...;.;.;.; . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x03, 0xb1, 0x3b, 0xb2, 0x3b, //  . . . . ....;.;
	0xdb, 0x3b, 0xdf, 0x03, 0xdf, 0x3b, 0xb2, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // .;...;.? . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0xb1, 0x3b, 0xb1, 0x3b, 0xb2, 0x3b, 0xb2, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  ..;.;.;.? . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xb1, 0x3b, 0xb1, 0x3b, 0xb2, 0x3b, //  . . . . ..;.;.;
	0xb2, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0xdf, 0x03, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // .? . ... . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0xb1, 0x3b, 0xb1, 0x3b, 0xb1, 0x3b, 0xb1, 0x3f, 0xdc, 0x0b, 0xdc, 0x03, 0xdc, 0x03, //  ..;.;.;.?......
	0xdc, 0x03, 0xdc, 0x03, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x08, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, // .... . .........
	0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x08, 0x20, 0x0f, 0xb1, 0x3b, 0xb1, 0x3b, 0xb1, 0x3b, // ........ ..;.;.;
	0xb1, 0x3f, 0xb1, 0x3f, 0xb2, 0x0b, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, // .?.?.. . .......
	0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, 0x20, 0x0f, 0x20, 0x01, 0x20, 0x0f, //  . ....... . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, 0x20, 0x0f, 0xde, 0x03, 0xb0, 0x3f, //  ..;.;.;.? ....?
	0xb1, 0x3f, 0xb2, 0x3f, 0xdd, 0x03, 0xde, 0x03, 0xdb, 0x03, 0xdb, 0x03, 0xb2, 0x3f, 0x20, 0x0f, // .?.?.........? .
	0x20, 0x0f, 0xb0, 0x3f, 0xb1, 0x3f, 0xb2, 0x3f, 0xde, 0x38, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, //  ..?.?.?.8.;.;.;
	0xb0, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xb0, 0x3b, 0xb1, 0x3b, 0xb2, 0x3b, 0xb2, 0x3f, // .? . . ..;.;.;.?
	0xdd, 0x03, 0xde, 0x03, 0xb0, 0x3f, 0xb1, 0x3f, 0xb2, 0x3f, 0xdd, 0x03, 0x20, 0x01, 0x20, 0x0f, // .....?.?.?.. . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0xdb, 0x03, //  ..;.;.;.? . ...
	0xb0, 0x3f, 0xb1, 0x3f, 0xdd, 0x03, 0xb1, 0x3b, 0xb0, 0x3b, 0xdb, 0x03, 0xb1, 0x3f, 0x20, 0x0f, // .?.?...;.;...? .
	0x20, 0x0f, 0x20, 0x3f, 0xb0, 0x3f, 0xb1, 0x3f, 0xb0, 0x3b, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, //  . ?.?.?.;.;.;.;
	0xb0, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x08, 0xdc, 0x3b, 0xb1, 0x3b, 0xb1, 0x3f, // .? . . ....;.;.?
	0xb1, 0x3b, 0xb0, 0x3b, 0xb2, 0x3b, 0xb0, 0x3f, 0xdc, 0x03, 0x20, 0x0f, 0x20, 0x01, 0x20, 0x0f, // .;.;.;.?.. . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, 0xdc, 0x0b, 0xdc, 0x07, 0xdb, 0x03, //  ..;.;.;.?......
	0xdb, 0x03, 0xdc, 0x38, 0x20, 0x0f, 0xdf, 0x03, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, 0xdc, 0x03, // ...8 ....;.;.?..
	0xdc, 0x07, 0xb0, 0x3f, 0xb1, 0x3f, 0xb2, 0x3f, 0xdd, 0x3b, 0xb2, 0x3b, 0xb1, 0x3b, 0xdc, 0x78, // ...?.?.?.;.;.;.x
	0xdf, 0x08, 0x20, 0x0f, 0x20, 0x0f, 0xde, 0x08, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, // .. . ....;.;.;.?
	0x20, 0x0f, 0xdf, 0x03, 0xb1, 0x3b, 0xb2, 0x3b, 0xdb, 0x03, 0xdd, 0x03, 0x20, 0x01, 0x20, 0x0f, //  ....;.;.... . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x08, 0xdc, 0x08, 0xdc, 0x08, 0x20, 0x0f, //  . . . ....... .
	0x20, 0x0f, 0xb0, 0x3f, 0xb0, 0x3f, 0xb1, 0x3f, 0xdd, 0x3b, 0xdb, 0x0b, 0xdf, 0x03, 0x20, 0x0f, //  ..?.?.?.;.... .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdf, 0x08, 0xdf, 0x03, 0xdf, 0x03, 0xdf, 0x08, //  . . . .........
	0x20, 0x0f, 0x20, 0x0f, 0xdf, 0x08, 0xdf, 0x03, 0xdf, 0x03, 0x20, 0x0f, 0x20, 0x01, 0x20, 0x0f, //  . ....... . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdb, 0x08, 0xb2, 0x38, 0xb1, 0x38, 0xdc, 0x03, //  . . . ....8.8..
	0xdc, 0x07, 0xb0, 0x3b, 0xb1, 0x3b, 0xdf, 0x3b, 0xdf, 0x08, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // ...;.;.;.. . . .
	0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
	0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
	0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
	0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
	0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
	0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
	0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
	0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x2d, 0x08, 0x3d, 0x08, 0x20, 0x0a, 0x43, 0x0b, 0x72, 0x0b, 0x6f, 0x0b, 0x73, 0x0b, 0x73, 0x0b, // -.=. .C.r.o.s.s.
	0x2d, 0x0b, 0x70, 0x0b, 0x6c, 0x0b, 0x61, 0x0b, 0x74, 0x0b, 0x66, 0x0b, 0x6f, 0x0b, 0x72, 0x0b, // -.p.l.a.t.f.o.r.
	0x6d, 0x0b, 0x20, 0x0b, 0x72, 0x0b, 0x65, 0x0b, 0x6e, 0x0b, 0x64, 0x0b, 0x65, 0x0b, 0x72, 0x0b, // m. .r.e.n.d.e.r.
	0x69, 0x0b, 0x6e, 0x0b, 0x67, 0x0b, 0x20, 0x0b, 0x6c, 0x0b, 0x69, 0x0b, 0x62, 0x0b, 0x72, 0x0b, // i.n.g. .l.i.b.r.
	0x61, 0x0b, 0x72, 0x0b, 0x79, 0x0b, 0x20, 0x0f, 0x3d, 0x08, 0x2d, 0x08, 0x20, 0x01, 0x20, 0x0f, // a.r.y. .=.-. . .
	0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
	0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
	0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
	0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
	0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
};

float vertexes[12] = {
	1.f, 1.f, 0.f,
	-1.f, 1.f, 0.f,
	-1.f, -1.f, 0.f,
	1.f, -1.f, 0.f
};

unsigned int indices[6] =
{
	0, 1, 2,
	0, 2, 3
};

bgfx::VertexBufferHandle vbh;
bgfx::IndexBufferHandle ibh;

bgfx::ProgramHandle program;


namespace myEngine
{
	Renderer* Renderer::r = NULL;

	Renderer* Renderer::getInstance()
	{
		if (r == NULL)
			r = new Renderer();
		return r;
	}
	void Renderer::destroyInstance() {
		if (r)
		{
			delete(r);
			r = NULL;
		}
	}
	Renderer::Renderer()
	:m_viewport_width(1280)
	,m_viewport_height(720)
	{

	}
	Renderer::~Renderer()
	{
	}
	bool Renderer::init()
	{
		bgfx::Init init;
		//init.type = bgfx::RendererType::OpenGLES;
		//init.type = bgfx::RendererType::Count;
		init.type = bgfx::RendererType::OpenGL;
		init.vendorId = BGFX_PCI_ID_NONE; //auto select
		init.resolution.width = m_viewport_width;
		init.resolution.height = m_viewport_height;
		init.resolution.reset = BGFX_RESET_VSYNC;
		bgfx::init(init);

		//uint32_t flag = BGFX_DEBUG_WIREFRAME | BGFX_DEBUG_TEXT | BGFX_DEBUG_PROFILER | BGFX_DEBUG_STATS;
		uint32_t flag = BGFX_DEBUG_TEXT | BGFX_DEBUG_PROFILER;
		bgfx::setDebug(flag);

		clear();

		bgfx::VertexDecl _dec;
		_dec.begin();
		_dec.add(bgfx::Attrib::Position, TINYGLTF_TYPE_VEC3, bgfx::AttribType::Float);
		_dec.end();

		vbh = bgfx::createVertexBuffer(bgfx::copy(&(vertexes[0]), 4 * sizeof(float) * 3), _dec);
		ibh = bgfx::createIndexBuffer(bgfx::copy(&(indices[0]), 6 * sizeof(unsigned int)));


		string vs_path = Config::shader_bin_path + "simple_vs.bin";
		int vs_bin_size = HelperFunc::getFileSize(vs_path.c_str());
		char * vs_bin = new char[vs_bin_size];
		HelperFunc::LoadFromFile(vs_path.c_str(), vs_bin, vs_bin_size);
		const bgfx::Memory* _vs_men = bgfx::makeRef(vs_bin, vs_bin_size);
		bgfx::ShaderHandle vertex_shader = bgfx::createShader(_vs_men);
		if (!bgfx::isValid(vertex_shader))
		{
			return false;
		}

		string fs_path = Config::shader_bin_path + "simple_fs.bin";
		int fs_bin_size = HelperFunc::getFileSize(fs_path.c_str());
		char * fs_bin = new char[fs_bin_size];
		HelperFunc::LoadFromFile(fs_path.c_str(), fs_bin, fs_bin_size);
		const bgfx::Memory* _fs_men = bgfx::makeRef(fs_bin, fs_bin_size);
		bgfx::ShaderHandle fragment_shader = bgfx::createShader(_fs_men);
		if (!bgfx::isValid(fragment_shader))
		{
			return false;
		}

		// Create bgfx program.
		program = bgfx::createProgram(vertex_shader, fragment_shader, true);
		if (!bgfx::isValid(program))
		{
			return false;
		}

		bgfx::frame();

		return true;
	}
	
	void Renderer::render()
	{
		clear();

		bgfx::setViewRect(0, 0, 0, uint16_t(m_viewport_width), uint16_t(m_viewport_width));

		bgfx::setVertexBuffer(0, vbh);
		bgfx::setIndexBuffer(ibh);

		bgfx::setState(BGFX_STATE_DEFAULT);

		bgfx::submit(0, program);

		//for (size_t i = 0; i < m_RenderObjects.size(); i++)
		//{
		//	m_RenderObjects[i]->draw();
		//}
		//m_RenderObjects.clear();

		//bgfx::dbgTextImage(10, 15, 40, 12, s_logo, 160);

		bgfx::frame();
	}
	void Renderer::setViewPort(int width, int height)
	{
		m_viewport_width = width;
		m_viewport_height = height;
		//bgfx::setViewRect(0, 0, 0, width, height);
	}
	void Renderer::clearDepth() const
{
		bgfx::setViewClear(0, BGFX_CLEAR_DEPTH, 0xaaaaaaff, 1.0f, 0);
	}
	void Renderer::clearColor() const
{
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0xaaaaaaff, 1.0f, 0);
	}
	void Renderer::clear() const
{
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xaaaaaaff, 1.0f, 0);
	}
	void Renderer::pushRenderObject(RenderObject * v)
	{
		m_RenderObjects.push_back(v);
	}
}
