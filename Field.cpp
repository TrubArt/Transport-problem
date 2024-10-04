#include "Field.h"
#include <Windows.h>

Field::Field(double value) 
	: value(value)
{}

Field::operator double() const
{
	return value;
}

Field::operator bool() const
{
	return (value ? true : false);
}

Field& Field::operator=(const Field& x)
{
	value = x.value;
	return *this;
}

Field& Field::operator=(double x)
{
	return *this = static_cast<Field>(x);
}

Field& Field::operator+=(const Field& x)
{
	value += x.value;
	return *this;
}

Field& Field::operator-=(const Field& x)
{
	value -= x.value;
	return *this;
}

Field operator+(const Field& x, const Field& y)
{
	Field tmp = x;
	tmp += y;
	return tmp;
}

Field operator-(const Field& x, const Field& y)
{
	Field tmp = x;
	tmp -= y;
	return tmp;
}

bool operator<(const Field& a, const Field& b)
{
	return a.get() < b.get();
}

bool operator>(const Field& a, const Field& b)
{
	return b < a;
}

bool operator==(const Field& a, const Field& b)
{
	return a.get() == b.get();
}

bool operator!=(const Field& a, const Field& b)
{
	return !(b == a);
}

bool operator<=(const Field& a, const Field& b)
{
	return !(b > a);
}

bool operator>=(const Field& a, const Field& b)
{
	return !(b < a);
}

double Field::get() const
{
	return value;
}

std::string Field::toString() const
{
	return std::to_string(value);
}

std::ostream& operator<<(std::ostream& out, const Field& x)
{
	return out << x.get();
}



FieldExtra::FieldExtra(double value)
	: Field(value)
	, isDef(true)
{}


bool FieldExtra::getIsDef() const
{
	return isDef;
}

void FieldExtra::setIsDef(bool flag)
{
	isDef = flag;
}
