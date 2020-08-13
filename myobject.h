
#pragma once

// Not thread safe.

// Make changes:

/*
Update to Euphoria_in_CPP

myobject.h
struct object gets turned into a class
class myobject gets a new data member "toplevel"

how many types of "objects" are there?
atom, integer, sequence, empty_seq, repeat, dont_copy, duplicate
float128, int128
float80, "int80"??? will there be an int80?
float64, int64
float32, int32

(flags == 6)
is_atom, is_integer, is_dont_copy, is_repeat, is_duplicate, is_float128

object data type:
is composed of (size_t, size_t, size_t)
[signed integer or pointer][length][flags]

iter data type:
is composed of (size_t, size_t)
[toplevel pointer][leaf pointer]

floating point types:
float, double, (long double--not on all compilers), (float128--not on all compilers)
[size_t varies according to platform:]
float--builtin in 32-bit (platforms)
double--builtin in 64-bit (platforms)
long double--(planned to be) builtin in 80-bit (platforms)
float128--(planned to be) builtin in 128-bit (platforms)

Try to use integers as much as possible.

array data-types (planned)
(signed)
char (single byte character string, no built-in null terminating byte)
short
int32
int64
"int80"??? will there be an int80?
int128
unsigned??? will there be a need for "unsigned"?
----
float
double
long double
float128

*/

/*
Built-in Euphoria v4.0 functions to translate into C++

atom -- done
integer -- done
object -- done
sequence -- done

length -- done
repeat -- done

compare -- done
equal -- done

and_bits -- done
not_bits -- done
or_bits -- done
xor_bits -- done

append -- done
prepend -- done

find -- done
match

floor -- done
remainder -- done

head
tail
insert
remove
replace
splice

mem_copy
mem_set

subscripting -- done

print() -- done

*/

// BEGIN CODE:



//#define PLATFORM_64

#ifdef PLATFORM_64

// For 64-bit programming:
#define SIZE_T unsigned long long
#define LENGTH(S) ((S).length64)
#define COUNT(S) ((S).count)
//#define LENGTH(S) (((S).length64<<3)>>3)
//#define LENGTH(S) ((S).length64 & 0x1FFFFFFFFFFFFFFFull)

#else

// For 32-bit programming:
#define SIZE_T unsigned int
#define LENGTH(S) ((S).length)
#define COUNT(S) ((S).count)

#endif // PLATFORM_64

#define MAKE_EMPTY_SEQ() data = { 0,0,0,0 }


// way of using an index:
//#define INDEX_OF_SEQ(S,I) (S)[((I)>0)?(I)-1:(I)+(S).length()]
#define SEQ(S,I) (S)[((I)>0)?(I)-1:(I)+(S).length()]

// c = (a>b) ? 25 : 45;

#define IS_ATOM(S) ((S).is_atom==1)
#define IS_INTEGER(S) ((S).is_integer==1)
#define IS_SEQUENCE(S) ((S).is_atom==0)
#define IS_COPY(S) ((S).count!=0)
#define IS_DONT_COPY(S) ((S).is_dont_copy==1)

typedef struct struct1 {
	union {
		long long i64;
		void * ptr;
		double dbl;
		struct struct1 * ob; // only used for debugging, it uses (void *ptr) internally instead
		class myobject * myob; // only used for debugging, it uses (void *ptr) internally instead
	};
	union {
		unsigned long long length64;
		unsigned int length;
	};
	union {
		// "count64" will be used in future updates, (need to make sure the code is working first)
		unsigned long long count64;
		unsigned int count; // is 0 when there are no other copies.
		struct {
			unsigned long long _reserved : 61;
			unsigned long long is_dont_copy : 1; // used internally
			unsigned long long is_integer : 1;
			unsigned long long is_atom : 1;
		};
	};
	union {
		unsigned long long dummy; // dummy until a better name is chosen
		void * up; // notice: pointer to the topmost object of the nested sequence
	};
} *pobject, object;

/*
start with pointer to top-most level
set do_not_copy_flag
duplicate sequence, set count to zero (0), (without copying any down-stream of do_not_copy_flag)
then set flag in function, so that it will not copy any down-stream
delete_myobject() // the whole thing
un-set do_not_copy_flag
*/

static void del_object(object& data);
void delete_object(object& data);

class myobject
{
private:
	object data;
//	SIZE_T duplicates;
public:
	void del_myobject();
	void delete_myobject();
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
	myobject& operator= (myobject&);
	myobject& operator= (long long);
	myobject& operator= (double);
	myobject& operator= (const char *);
	myobject& operator= (const object&); // object passed by reference (pointer)

	myobject& operator[] (SIZE_T); // NOTICE: seq indexes start at 1, offsets start at 0, a[0] is the first element
	myobject& operator += (const myobject&);
	myobject& operator -= (const myobject&);
	myobject& operator *= (const myobject&);
	myobject& operator /= (const myobject&);

	myobject& operator %= (const myobject&);
	myobject& operator ^= (const myobject&);
	myobject& operator &= (const myobject&);
	myobject& operator |= (const myobject&);

	myobject& operator <<= (const myobject&);
	myobject& operator >>= (const myobject&);

	myobject operator + (const myobject&);
	myobject operator - (const myobject&);
	myobject operator * (const myobject&);
	myobject operator / (const myobject&);

	myobject operator % (const myobject&);
	myobject operator ^ (const myobject&);
	myobject operator & (const myobject&);
	myobject operator | (const myobject&);

	myobject operator << (const myobject&);
	myobject operator >> (const myobject&);

	SIZE_T const length();

	long long const i64();
	double const dbl();

	// NOTICE: seq indexes start at 1, offsets start at 0, a[0] is the first element
	// subscripting, starts at 1, example subscript = a.slice(1,-1);
	myobject slice(long long, long long);

	bool const is_atom();
	bool const is_dbl();
	bool const is_integer();
	bool const is_seq();

	// Todo: try to make these faster:
	myobject& append(myobject&);
	myobject& prepend(myobject&);

	myobject& floor_self();

	myobject& to_int(); // long long value
	myobject& to_float(); // double value
	char * to_string(); // converts it into a NULL-terminated character string

	myobject& to_bit(unsigned long long bits);

	void set_duplicate_count(unsigned long long count);
	object dup_myobject(void * up, myobject * node); // returns a copy of an object

	void print();

	unsigned long long find(myobject&);
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

