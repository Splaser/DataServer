#ifndef DEVICE_ENCODE_H
#define DEVICE_ENCODE_H

#include <string>

namespace Sirius
{
	static std::string GetName(const WORD deviceId)
	{
		switch ( deviceId )
		{
		case 0x30: return "������-���-1";
		case 0x67: return "������-��";
		case 0x62: return "������-2-�";
		case 0x6B: return "������-��";
		case 0x64: return "������-2-��";
		case 0x5D: return "������-��";
		case 0x65: return "������-�";
		case 0x31: return "������-�";
		case 0x69: return "������-��";
		case 0x42: return "�����-���";
		case 0x61: return "������-��-35";
		case 0x6F: return "";
		case 0x66: return "������-�";
		case 0x32: return "������-��";
		case 0x46: return "������-���";
		case 0x63: return "������-2-�";
		default: return "�����. ����.";
		}
	}
} // Sirius

#endif // DEVICE_ENCODE_H