#ifndef MODBUS_EXCEPT_HPP
#define MODBUS_EXCEPT_HPP

#include "../../utils/types.h"

class serial_port_not_open : public std::runtime_error
{
public:
	explicit serial_port_not_open () : runtime_error("���������� ������� ����.") {}
	virtual ~serial_port_not_open() {}
};

class modbus_error : public std::logic_error
{
public:
	explicit modbus_error (const std::string& what_arg) : logic_error(what_arg) {}
	virtual ~modbus_error() {}
};

class modbus_error_notalive : public modbus_error
{
public:
	explicit modbus_error_notalive () : modbus_error("������ CRC.") {};
	virtual ~modbus_error_notalive() {}
};

class modbus_error_crc : public modbus_error
{
public:
	explicit modbus_error_crc () : modbus_error("������ CRC.") {};
	virtual ~modbus_error_crc() {}
};

class modbus_error_request : public modbus_error
{
public:
	explicit modbus_error_request () : modbus_error("������ ������������ �������.") {};
	virtual ~modbus_error_request() {}
};

class modbus_error_unk : public modbus_error
{
public:
	explicit modbus_error_unk () : modbus_error("����������� ������.") {};
	virtual ~modbus_error_unk() {}
};

class modbus_error_timeout : public modbus_error
{
public:
	explicit modbus_error_timeout () : modbus_error("����������� ������.") {};
	virtual ~modbus_error_timeout() {}
};

class modbus_error_01 : public modbus_error
{
public:
	explicit modbus_error_01 () : modbus_error("�������� ��� ������� �� ����� ���� ��������� �� �����������.") {};
	virtual ~modbus_error_01() {}
};

class modbus_error_02 : public modbus_error
{
public:
	explicit modbus_error_02() : modbus_error("����� ������, ��������� � �������, �� �������� ������� ������������.") {};
	virtual ~modbus_error_02() {}
};

class modbus_error_03 : public modbus_error
{
public:
	explicit modbus_error_03() : modbus_error("��������, ������������ � ���� ������ �������, �������� ������������ ��������� ��� ������������.") {};
	virtual ~modbus_error_03() {}
};

class modbus_error_04 : public modbus_error
{
public:
	explicit modbus_error_04() : modbus_error("������������������� ������ ����� �����, ���� ����������� ������� ��������� ������������� ��������.") {};
	virtual ~modbus_error_04() {}
};

class modbus_error_05 : public modbus_error
{
public:
	explicit modbus_error_05() : modbus_error("����������� ������ ������ � ������������ ���, �� ��� ������� ����� �������. ���� ����� ������������ �������� �� ��������� ������ ����-����.") {};
	virtual ~modbus_error_05() {}
};

class modbus_error_06 : public modbus_error
{
public:
	explicit modbus_error_06() : modbus_error("����������� ����� ���������� �������. ������� ������ ��������� ��������� �����, ����� ����������� �����������.") {};
	virtual ~modbus_error_06() {}
};

class modbus_error_07 : public modbus_error
{
public:
	explicit modbus_error_07() : modbus_error("����������� �� ����� ��������� ����������� �������, �������� � �������. ���� ��� ������������ ��� ���������� ������������ �������, ������������� ������� � �������� 13 ��� 14. ������� ������ ��������� ��������������� ���������� ��� ���������� �� ������� � ������������.") {};
	virtual ~modbus_error_07() {}
};

class modbus_error_08 : public modbus_error
{
public:
	explicit modbus_error_08() : modbus_error("����������� �������� ������ ����������� ������, �� ��������� ������ ��������. ������� ����� ��������� ������, �� ������ � ����� ������� ��������� ������.") {};
	virtual ~modbus_error_08() {}
};

#endif // MODBUS_EXCEPT_HPP