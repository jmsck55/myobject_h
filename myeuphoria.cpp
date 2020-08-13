
// myeuphoria.cpp : Defines the entry point for the console application.
//

// About myobject_h:
// I call it "Euphoria_in_CPP" or "MYOBJECT_H" or "obj.h"

// Future: Once I get it working, I'll need to work on recursive "math", "trig" functions.

// Not thread safe.

#include "stdafx.h"

#include "obj.h"

//HERE: Work on compare functions next.

// Modify for linked lists. Done.

#define A
#define B
#define C
#define D
#define E

void set_a(void) {
	// NOTE: doesn't work without new operator.
	// work around:
	object tmp((integer)99);
#ifdef A
	object * a = new object((integer)1);
#endif
#ifdef B
	object * b = new object((size_t)3, a);
#endif
#ifdef C
	object * c;
#endif
#ifdef D
	object * d;
#endif

#ifdef E
	object * e;
#endif

#ifdef B
	b->expand_repeat();
#endif

#ifdef C
	c = new object((size_t)4, b);
	c->expand_repeat(); // there is a memory leak somewhere in this line of code.
#endif
#ifdef D
	d = new object((size_t)2, c);
	d->expand_repeat();
#endif

//here:
#ifdef E
	e = c->get(-1);
	e->cprint();
	std::cout << std::endl;

	//exit(2);

	DELETE_OBJECT(e);
#endif

#ifdef A
	a->cprint();
	std::cout << std::endl;
#endif
#ifdef B
	b->cprint();
	std::cout << std::endl;
#endif
#ifdef C
	c->cprint();
	std::cout << std::endl;
#endif
#ifdef D
	d->cprint();
	std::cout << std::endl;
#endif

	// catches properly now:
	//std::cout << a->get_length() << std::endl;

	//exit(EXIT_SUCCESS);

#ifdef A
	DELETE_OBJECT(a);
#endif
#ifdef B
	DELETE_OBJECT(b);
#endif
#ifdef C
	DELETE_OBJECT(c);
#endif
#ifdef D
	DELETE_OBJECT(d);
#endif

}

/*
struct X {
    // custom placement new
    static void* operator new(std::size_t sz, bool b);
    // custom placement delete
    static void operator delete(void* ptr, bool b);
};

// custom placement new
static void* X::operator new(std::size_t sz, bool b) {
	std::cout << "custom placement new called, b = " << b << '\n';
	return ::operator new(sz);
}
    // custom placement delete
static void X::operator delete(void* ptr, bool b)
{
	std::cout << "custom placement delete called, b = " << b << '\n';
	::operator delete(ptr);
}

int main() {
   try {
     X* p1 = new (true) X;
     ::delete p1;
   } catch(const std::exception&) { }
}
*/
void set_b(void) {
	object * a;
	object * b;
//	object * x;
	atom f;
	MYSEQUENCE::iterator iter;

	a = new object((integer)1);
	b = new object((size_t)3, a);

	DELETE_OBJECT(a);

	a = new object((integer)2);

	f = 2.3;
	*a = f;

	b->set(1, a);

	b->cprint();
	std::cout << std::endl;

	*a = b;

	a->cprint();
	std::cout << std::endl;


	DELETE_OBJECT(a);
	DELETE_OBJECT(b);
}

void set_c(void) {
	integer i = 3;
	atom a = 2.5;
	object ob;

	ob = a;
	ob.eu_repeat(&ob,(integer)2);
	ob = &ob;
	ob.cprint();
	std::cout << std::endl;
	ob.pretty_print();
	std::cout << std::endl;

//STATUS: still debugging

}

int main()
{
	init_obj();

	std::cout << "Object bit Size is: " << sizeof(object) * 8 << std::endl;
	std::cout << "Bits is: " << sizeof(atom) * 8 << std::endl;

	//set_a();
	//set_b();
	set_c();

	puts("Hi world.\n");
	getc(stdin);
	return 0;
}

