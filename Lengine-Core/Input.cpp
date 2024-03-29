#include "Input.hpp"

unsigned char Input::KeyCodes[ 99 ] = 
{
	0x08,
	0x09,
	0x0D,
	0x10,
	0x11,
	0x12,
	0x14,
	0x1B,
	0x20,
	0x21,
	0x22,
	0x23,
	0x24,
	0x25,
	0x26,
	0x27,
	0x28,
	0x2C,
	0x2D,
	0x2E,
	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4C,
	0x4D,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x5A,
	0x70,
	0x71,
	0x72,
	0x73,
	0x74,
	0x75,
	0x76,
	0x77,
	0x78,
	0x79,
	0x7A,
	0x7B,
	0x7C,
	0x7D,
	0x7E,
	0x7F,
	0x80,
	0x81,
	0x82,
	0x83,
	0x84,
	0x85,
	0x86,
	0x87,
	0x90,
	0x91,
	0xA0,
	0xA1,
	0xA2,
	0xA3,
	0xA4,
	0xA5,
	0xBA,
	0xBB,
	0xBC,
	0xBD,
	0xBE,
	0xBF,
	0xC0,
	0xDB,
	0xDC,
	0xDD,
	0xDE
};

unsigned char Input::MouseCodes[ 3 ] =
{
	0x01,
	0x02,
	0x04
};

std::bitset< 99 > Input::KeyStates;
std::bitset< 3  > Input::MouseStates;
Vector2           Input::MousePosition;
std::thread*      Input::InputProcessor;