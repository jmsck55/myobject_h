========================================================================
    CONSOLE APPLICATION : "Euphoria syntax in CPP" Project Overview
========================================================================

AppWizard has created this "Euphoria syntax in CPP" application for you.

This file contains a summary of what you will find in each of the files that
make up your "Euphoria syntax in CPP" application.


    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

    This is the main application source file.

	NOTICE: Still needs unit testing.
	NOTICE: Needs error handling.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named [!output PROJECT_NAME].pch and a precompiled types file named StdAfx.object.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

TODO:

/*
Built-in Euphoria v4.0 functions to translate into C++

atom
integer
object
sequence

length
repeat

compare
equal

and_bits
not_bits
or_bits
xor_bits

append
prepend

find
match

floor
remainder

head
tail
insert
remove
replace
splice

mem_copy
mem_set

subscripting

print()

*/

TODO:

IMPLIMENT:

class myobject
{
private:
	object data;
//	SIZE_T duplicates;
public:
	void myobject::del_myobject();
	void myobject::delete_myobject();
	myobject();
	myobject(double);
	myobject(long long);
	myobject(const object&); // object passed by reference (pointer)

	myobject(object * ob, SIZE_T length); // objects passed by reference (pointer)
	myobject(myobject * myob, SIZE_T length);

	myobject(const char * str, SIZE_T length);
	myobject(const char * str);
	myobject(const long long *, SIZE_T);
	myobject(const double *, SIZE_T);
	myobject(myobject&, SIZE_T); // repeat(), for sequence creation
	myobject(myobject&, myobject&); // concat two sequences into a new sequence
	~myobject();
	myobject& myobject::operator= (myobject&);
	myobject& myobject::operator= (long long);
	myobject& myobject::operator= (double);
	myobject& myobject::operator= (const char *);
	myobject& myobject::operator= (const object&); // object passed by reference (pointer)

	myobject& myobject::operator[] (SIZE_T); // NOTICE: seq indexes start at 1, offsets start at 0, a[0] is the first element
	myobject& myobject::operator += (const myobject&);
	myobject& myobject::operator -= (const myobject&);
	myobject& myobject::operator *= (const myobject&);
	myobject& myobject::operator /= (const myobject&);

	myobject& myobject::operator %= (const myobject&);
	myobject& myobject::operator ^= (const myobject&);
	myobject& myobject::operator &= (const myobject&);
	myobject& myobject::operator |= (const myobject&);

	myobject& myobject::operator <<= (const myobject&);
	myobject& myobject::operator >>= (const myobject&);

	myobject myobject::operator + (const myobject&);
	myobject myobject::operator - (const myobject&);
	myobject myobject::operator * (const myobject&);
	myobject myobject::operator / (const myobject&);

	myobject myobject::operator % (const myobject&);
	myobject myobject::operator ^ (const myobject&);
	myobject myobject::operator & (const myobject&);
	myobject myobject::operator | (const myobject&);

	myobject myobject::operator << (const myobject&);
	myobject myobject::operator >> (const myobject&);

	SIZE_T const myobject::length();

	long long const myobject::i64();
	double const myobject::dbl();

	// NOTICE: seq indexes start at 1, offsets start at 0, a[0] is the first element
	// subscripting, starts at 1, example subscript = a.slice(1,-1);
	myobject myobject::slice(long long, long long);

	bool const myobject::is_atom();
	bool const myobject::is_dbl();
	bool const myobject::is_integer();
	bool const myobject::is_seq();

	// Todo: try to make these faster:
	myobject& myobject::append(myobject&);
	myobject& myobject::prepend(myobject&);

	myobject& myobject::floor_self();

	myobject& myobject::to_int(); // long long value
	myobject& myobject::to_float(); // double value
	char * myobject::to_string(); // converts it into a NULL-terminated character string

	myobject& myobject::to_bit(unsigned long long bits);

	void myobject::set_duplicate_count(unsigned long long count);
	object myobject::dup_myobject(void * up, myobject * node); // returns a copy of an object

	void myobject::print();

	unsigned long long myobject::find(myobject&);
};

// related functions:

myobject repeat(myobject& ob, SIZE_T n);

long long compare(myobject&, myobject&);
bool equal(myobject&, myobject&);

bool operator< (myobject&, myobject&);
bool operator> (myobject&, myobject&);
bool operator== (myobject&, myobject&);
bool operator!= (myobject&, myobject&);
bool operator<= (myobject&, myobject&);
bool operator>= (myobject&, myobject&);

myobject operator-(const myobject& ob); // negate() function
myobject operator~(const myobject& ob); //myobject not_bits(myobject a);

myobject and_bits(myobject& a, myobject& b);
myobject or_bits(myobject& a, myobject& b);
myobject xor_bits(myobject& a, myobject& b);
myobject not_bits(myobject& a);

myobject remainder(myobject& num, myobject& den);
myobject floor(myobject& a);

