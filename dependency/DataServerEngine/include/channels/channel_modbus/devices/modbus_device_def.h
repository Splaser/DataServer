#ifndef MODBUS_DEVICE_DEF_H
#define MODBUS_DEVICE_DEF_H

#include <boost/cstdint.hpp>

namespace Channels
{
	namespace Modbus
	{
		namespace Devices
		{
			const boost::int32_t GenericDevice = 0;
			const boost::int32_t Sirius2Old = 1;
			const boost::int32_t Sirius2 = 2;

			// ������ ������������ �� 2007 ����
			const boost::int32_t sirius2old_ml = 0x6B; //  ������-2-��
			// ������ ������������ ����� 2007 ����
			const boost::int32_t sirius2_ml = 0x82; //  ������-2-��
		}
	}
}

#endif // MODBUS_DEVICE_DEF_H
