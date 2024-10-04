#pragma once
#ifndef FIELDDD
#define FIELDDD
#include <string>
#include <iostream>

class Field
{
public:
	Field() = default;
	Field(double);

	explicit operator double() const;
	explicit operator bool() const;
	Field& operator=(const Field&);
	Field& operator=(double);
	Field& operator+=(const Field&);
	Field& operator-=(const Field&);

	double get() const;
	std::string toString() const;

protected:
	double value = 0.0;
};
std::ostream& operator<<(std::ostream&, const Field&);
Field operator+(const Field&, const Field&);
Field operator-(const Field&, const Field&);
bool operator<(const Field&, const Field&);
bool operator>(const Field&, const Field&);
bool operator==(const Field&, const Field&);
bool operator!=(const Field&, const Field&);
bool operator<=(const Field&, const Field&);
bool operator>=(const Field&, const Field&);



class FieldExtra : public Field
{
public:
	FieldExtra() = default;
	FieldExtra(double);
	bool getIsDef() const;
	void setIsDef(bool);

private:
	bool isDef = false;
};
#endif // !FIELDDD