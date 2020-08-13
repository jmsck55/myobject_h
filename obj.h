
#pragma once
/*
Built-in Euphoria v4.0 functions to translate into C++

atom --
integer --
object --
sequence --

length --
repeat --

compare --
equal --

and_bits --
not_bits --
or_bits --
xor_bits --

append --
prepend --

find --
match

floor --
remainder --

head
tail
insert
remove
replace
splice

mem_copy
mem_set

subscripting --

print() --

*/

// ADDITIONAL OPERATORS:
/*
ASSIGNMENT:
a = b
a += b
a -= b
a *= b
a /= b
a %= b
a &= b
a |= b
a ^= b
a <<= b
a >>= b
ARITHMETIC:
-a
a + b
a - b
a * b
a / b
a % b
~a
a & b
a | b
a ^ b
a << b
a >> b
COMPARISON:
a == b
a != b
a < b
a > b
a <= b
a >= b
*/

#if BITS == 32
typedef size_t MYSIZE_T;
typedef int integer;
typedef unsigned int uinteger;
typedef float atom;
#endif // BITS == 32
#if BITS == 64
typedef size_t MYSIZE_T;
typedef long long integer;
typedef unsigned long long uinteger;
typedef double atom;
#endif // BITS == 64
#if BITS == 128
typedef unsigned __int128 MYSIZE_T;
typedef __int128 integer;
typedef unsigned __int128 uinteger;
typedef __float128 atom;
#endif // BITS == 128

typedef enum {
	//builtins:
	is_sequence, // uses length
	is_atom, // uses size_t reserved floating-point
	is_integer, // uses size_t reserved integer
	is_uinteger,
    is_duplicate,
	is_repeat
/*
	//arrays: // uses length
	,is_u8,
	is_i8,
	is_u16,
	is_i16,
	is_u32,
	is_i32,
	is_f32,
	is_u64,
	is_i64,
	is_f64,
	is_u80,
	is_i80,
	is_f80,
	is_u128,
	is_i128,
	is_f128
*/
} *is_type_ptr, is_type;

//#define is_duplicate 0x02000000
//#define is_repeat 0x04000000

#define MAKE_DUPLICATE(T) T = is_duplicate
#define IS_DUPLICATE(T) (T == is_duplicate)
#define RESET_DUPLICATE(T) T = is_sequence

#define MAKE_REPEAT(T) T = is_repeat
#define IS_REPEAT(T) (T == is_repeat)
#define RESET_IS_REPEAT(T) T = is_sequence

#define GET_FLAG(T) ((is_type)(T & 0x00FFFFFF))

#define IS_INTEGER(T) (T == is_integer)
#define IS_UINTEGER(T) (T == is_uinteger)
#define IS_FLOAT(T) (T == is_atom)
#define IS_ATOM(T) ((T == is_integer) || (T == is_uinteger) || (T == is_atom))

size_t find(std::forward_list<void *> haystack, void * needle);

void register_obj(class object * myobj);
void cleanup_obj();
void init_obj(); // init()

#define MYSEQUENCE std::list<class object>

class object
{
protected:
	void make_empty_seq();
	void make_object(integer x);
	void make_object(uinteger x);
	void make_object(atom x);
	void make_object(object * x);
	void make_clone(const object & x);
	void make_duplicate(object * value, size_t count); // (length == duplicate count)
	void make_repeat(size_t n);
	void make_repeat(size_t n, object * value);
	void delete_object();
public:
	union {
		MYSIZE_T reserved;
        integer idata;
        uinteger udata;
        atom fdata;
		class dup * duplicate;
		class rep * repeat;
        MYSEQUENCE * seq; // pointer to a container used to store an array of objects
    };
	union {
		MYSIZE_T flags;
		is_type flag;
	};
	void* operator new(std::size_t sz, bool is_toplevel = true);

	/* Not working, use DELETE_OBJECT() instead */
	void operator delete(void* ptr, bool is_toplevel);

	size_t get_length();

	object();
	object(MYSIZE_T res, MYSIZE_T typ); //  typ is type of variable or flags
	object(integer x);
	object(uinteger x);
	object(atom x);
#if BITS != 64
	object(long long x);
	object(unsigned long long x);
	object(double x);
#endif
	object(object * x); // copy
	//object(const object &x); // "make_clone" copy, is recursive
	object(const object& x, bool force_clone = false); // make_clone, force_clone is internal
	object(object * value, size_t count); // create a duplicate (uses pointer)

	// repeat:
	object(size_t n, object * value);
	void eu_repeat(object * value, size_t n);

	void expand_repeat();
    void expand_all(); // expand all levels of "dup"s and "repeat"s.

	// element(s) access
	object * get(integer index);
	object& get_slice(integer first, integer last);
	void set(integer index, object * param);
	void set_slice(integer first, integer last, object * param);

	object& operator= (integer param);
	object& operator= (uinteger param);
	object& operator= (atom param);
#if BITS != 64
	object& operator= (long long param);
	object& operator= (unsigned long long param);
	object& operator= (double param);
#endif

	bool is_object_in_nested_seq(object * ob);

    object& operator= (object * param);

    object& operator[] (size_t offset);

   	object& operator += (object&);
	object& operator -= (object&);
	object& operator *= (object&);
	object& operator /= (object&);

	object& operator %= (object&);
	object& operator ^= (object&);
	object& operator &= (object&);
	object& operator |= (object&);

	object& operator <<= (object&);
	object& operator >>= (object&);

	object * operator + (object&);
	object * operator - (object&);
	object * operator * (object&); // programmers need this
	object * operator / (object&);

	object * operator % (object&);
	object * operator ^ (object&);
	object * operator & (object&);
	object * operator | (object&);

	object * operator << (object&);
	object * operator >> (object&);


	// put more functions here

	void cprint();
	void pretty_print();

	~object();
};
typedef MYSEQUENCE mysequence;

void DELETE_OBJECT(object * ptr);

// end of file

class dup
{
protected:
	void delete_dup();
public:
	union {
		class object * ptr;
	};
	union {
		size_t count;
	};
	dup();
	dup(object * val, size_t c);

	~dup();
};

// end of file

class rep
{
protected:
	void delete_rep();
public:
	union {
		class object * ptr;
	};
	union {
		size_t count;
	};
	rep();
	rep(object * val, size_t c);

	~rep();
};

