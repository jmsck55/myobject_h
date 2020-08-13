
#include "stdafx.h"
#include "obj.h"

std::forward_list<void *> all_objects;

size_t nested_delete = 0;
size_t nested_from_library = 0;

bool is_error_message = false;

size_t find(std::forward_list<void *> haystack, void * needle) {
	size_t index = 1; // index starts at one (1), ends at length or size of list
	for (auto it = haystack.begin(); it != haystack.end(); ++it) {
		if (*it == needle) {
			return index;
		}
		index++;
	}
	return (size_t)0;
}

// register all top level new objects:

void register_obj(class object * myobj) {
	all_objects.push_front(myobj);
}

// the program only needs to clean-up top-level objects
void cleanup_obj() {
	object * ptr;
	//all_objects.reverse();
	while (!all_objects.empty()) {
		ptr = (object*)all_objects.front();
		DELETE_OBJECT(ptr);
	}
	if (is_error_message) {
		// wait for Enter key to be pressed
		std::cerr << "Press Enter to continue." << std::endl;
		getc(stdin);
	}
}

void init_obj() {
	// init()
	atexit(cleanup_obj); // callback routine to run at "exit()"
}

void* object::operator new(std::size_t sz, bool is_toplevel)
{
	void * ptr;
	ptr = ::operator new(sz);
	if (is_toplevel) {
		if (nested_from_library == 0) {
			register_obj((object*)ptr);
		}
	}
	return ptr;
}

/* Not working, use DELETE_OBJECT() instead */
void object::operator delete(void* ptr, bool is_toplevel)
{
	if (is_toplevel) {
		all_objects.remove(ptr);
	}
	::operator delete(ptr);
}

size_t object::get_length() {
	object * x;
	x = this;
	while (x->flag == is_duplicate) {
		x = x->duplicate->ptr;
	}
	if (x->flag == is_repeat) {
		return x->repeat->count;
	}
	if ((x->flag != is_sequence) || (x->seq == NULL)) {
		is_error_message = true;
		std::cerr << "Error: object is not a sequence, called from get_length() library function." << std::endl;
		exit(EXIT_FAILURE);
		//return 0;
	}
	return x->seq->size();
}

void object::make_empty_seq() {
	reserved = 0;
	flags = 0;
}
void object::make_object(integer x) {
	//copy
	idata = x;
	flags = 0;
	flag = is_integer;
}
void object::make_object(uinteger x) {
	//copy
	udata = x;
	flags = 0;
	flag = is_uinteger;
}
void object::make_object(atom x) {
	//copy
	fdata = x;
	flags = 0;
	flag = is_atom;
}
void object::make_clone(const object& x) {
// make_clone, copy without using duplicates
	reserved = x.reserved;
	flags = x.flags;
	//if (flag == is_duplicate) {
	while (flag == is_duplicate) {
		flags = duplicate->ptr->flags;
		duplicate = duplicate->ptr->duplicate;
		//here
	}
	if (flag == is_repeat) {
		//object * ob;
		nested_from_library++;
		make_repeat(repeat->count, repeat->ptr);
		//ob = new (false) object(*repeat->ptr, true);
		nested_from_library--;
		//repeat = new rep(ob, repeat->count);
	}
	else if (flag == is_sequence) {
		nested_from_library++;
		seq = new mysequence(*seq); // this statement does everything that it needs to do
		nested_from_library--;
	}
}
void object::make_object(object * x) {
	if (x == NULL) {
		make_empty_seq();
		return;
	}
	if (x->flag == is_duplicate) {
		// it uses dup
		// increase the copy count (dup count)
		x->duplicate->count++;
	}
	else if ((x->flag == is_sequence) || (x->flag == is_repeat)) {
		// make it into a duplicate
		object * ob = new (false) object(x->reserved, x->flags);
		x->duplicate = new dup(ob, 2);
		x->flag = is_duplicate;
	}
	reserved = x->reserved;
	flags = x->flags;
}


/*
void object::make_object(object * x, object * myup, object * param, object ** node) {
	//copy
	// make an object recursively, an exact copy (verbatim)
	if (IS_DONT_COPY(x->flags)) {
		//copy
		// "dup_object()" is embedded in this function
		// they were so similar I combined them
		RESET_IS_DONT_COPY(x->flags);
		make_object(param, myup, NULL, NULL);
		if (node != NULL) {
			*node = this;
		}
		return;
	}
	reserved = x->reserved;
	length = x->length;
	flags = x->flags;
	up = myup;
	// it already copied:
	// work on these variables for new functions
	if (IS_DUPLICATE(flags)) { // dup doesnot have an "up" variable
		// it uses dup
		// increase the copy count (dup count)
		duplicate->count++;
		return;
	}
	if (IS_REPEAT(flags)) {
		// it has to copy the compressed "repeat"
		// it is not a sequence
		if (x->ob != NULL) {
			ob = new object(x->ob, this, NULL, NULL);
		}
		return;
	}
	if ((GET_FLAG(flags) == is_sequence) && (length > 0)) {
		object * a;
		object * b;
		size_t i;
		a = new object[length];
		ob = a;
		b = x->ob;
		for (i = 0; i < length; i++) {
			a->make_object(b, this, param, node);
			a++;
			b++;
		}
	}
}
*/
void object::make_duplicate(object * value, size_t count) {
	duplicate = new dup(value, count); // dont make a copy
	flag = is_duplicate;
}
void object::make_repeat(size_t n) {
	// repeat
    repeat = new rep(NULL, n);
	flag = is_repeat;
}
void object::make_repeat(size_t n, object * value) {
	// repeat
	object * ob;
    if (value == NULL) {
        make_repeat(n);
        return;
    }
	nested_from_library++;
    ob = new (false) object(*value, true); // make clone of "value" (or optionally a duplicate)
	nested_from_library--;
    repeat = new rep(ob, n);
	flag = is_repeat;
}


object::object()
{
	make_empty_seq();
}
object::object(MYSIZE_T res, MYSIZE_T typ) {
	reserved = res;
	flags = typ; // type of variable or flags
}
object::object(integer x) {
	make_object(x);
}
object::object(uinteger x) {
	make_object(x);
}
object::object(atom x) {
	make_object(x);
}
#if BITS != 64
object::object(long long x) {
	make_object((integer)x);
}
object::object(unsigned long long x) {
	make_object((uinteger)x);
}
object::object(double x) {
	make_object((atom)x);
}
#endif
object::object(object * x) {
    // copy (uses pointers with duplicate count)
	make_object(x);
}
object::object(object * value, size_t count) {
	// create a duplicate (uses pointer)
	make_duplicate(value, count);
}

object::object(const object& x, bool force_clone) {
	// copy (make a clone of the data)
	// is recursive
	// "make_clone()" copy
/*	if (is_toplevel) {
		if (nested_from_library == 0) {
			register_obj(this);
		}
	}*/
	if ((nested_from_library == 0) || (force_clone)) {
		make_clone(x);
	}
	else {
		make_object((object *)&x);
	}
}

// repeat

object::object(size_t n, object * value) {
	make_repeat(n, value);
}

void object::eu_repeat(object * value, size_t n) {
	// when "old_value" goes out of scope, it gets deleted at the end of this function
	object old_value(reserved, flags);
	make_repeat(n, value);
}

void object::expand_repeat() {
	// work on this, almost done
	// make a "sequence" out of a "repeat"
	dup * mydup;
	//if (flag == is_duplicate) {
	while (flag == is_duplicate) {
		mydup = duplicate;
		flag = duplicate->ptr->flag;
        reserved = duplicate->ptr->reserved;
		mydup->ptr = NULL;
		delete mydup;
		//here
	}
	if (flag == is_repeat) {
		if (repeat != NULL) {
            if ((repeat->count == 0) || (repeat->ptr == NULL)) { // Remember: this is a "repeat"
				delete repeat;
                make_empty_seq();
                return;
            }
			if (repeat->ptr->flag != is_sequence) {
				mysequence * tmp;
				nested_from_library++;
				tmp = new mysequence(repeat->count, repeat->ptr);
				// why does it call a delete?
				nested_from_library--;
				delete repeat;
				flags = 0;
				seq = tmp;
			}
			else {
				size_t c;
				mysequence * tmp;
				// treat repeat as a duplicate, repeat "repeat->count" times
				c = repeat->count;
				duplicate->count = 0;
				flag = is_duplicate;
				nested_from_library++;
				tmp = new mysequence(c, *this); // it makes a copy of "*repeat->ptr"
				nested_from_library--;
				//delete repeat; // no need to use delete here
				flags = 0;
				seq = tmp;
			}
		}
	}
}

/*
void object::expand_all() {
    // Use this function when you need to modify all elements of a sequence in one operation
    // new sequence
    object * ptr;
    if (IS_DUPLICATE(flags)) {
        //object * ptr;
        ptr = duplicate->ptr;
        if (duplicate->count > 0) {
            duplicate->count--;
            make_object(ptr);
        }
        else {
            duplicate->ptr = NULL;
            delete duplicate;

            ob = ptr->ob;
            length = ptr->length;
            flags = ptr->flags;

            ptr->ob = NULL;
            delete ptr;
        }
    }
    if (IS_REPEAT(flags)) {
        expand_repeat();
    }
    if (!IS_ATOM(flag)) {
        if (ob != NULL) {
            size_t i;
            object * iter;
            iter = ob;
            for (i = 0; i < length; i++) {
                iter->expand_all();
                iter++;
            }
        }
    }
    //std::cerr << "Error: bad index passed to library function." << std::endl;
    //exit(EXIT_FAILURE);
}
*/

// Element access:

// get
// get sub-slice
// set
// sub-slice set

object * object::get(integer index) {
	// in "get()" indexes start at 1, and -1 points to the last element
	// assumes that object is a sequence
	integer size;
	integer i;
	object * p;
	object * myob;
	object * x;
	x = this;
	while (x->flag == is_duplicate) {
		x = x->duplicate->ptr;
	}
	if (x->flag == is_repeat) {
		size = x->repeat->count;
		if ((index > size) || (index < -size) || (index == 0)) {
			is_error_message = true;
			std::cerr << "Error: syntax checking, wrong index, called from get() library function." << std::endl;
			exit(EXIT_FAILURE);
		}
		// return a clone copy of repeat->ptr
		myob = new (true) object(x->repeat->ptr); //--here--
		return myob;
	}
	if (x->flag != is_sequence) {
		is_error_message = true;
		std::cerr << "Error: object is not a sequence, called from get() library function." << std::endl;
		exit(EXIT_FAILURE);
	}
	size = (integer)x->seq->size();
	if (index >= 0) {
		MYSEQUENCE::iterator iter;
		if ((index > size) || (index == 0)) {
			is_error_message = true;
			std::cerr << "Error: syntax checking, wrong index, called from get() library function." << std::endl;
			exit(EXIT_FAILURE);
		}
		iter = x->seq->begin();
		for (i = 2; i <= index; i++) {
			++iter;
		}
		p = &(*iter);
	}
	else {
		MYSEQUENCE::reverse_iterator riter;
		if (index < -size) {
			is_error_message = true;
			std::cerr << "Error: syntax checking, wrong index, called from get() library function." << std::endl;
			exit(EXIT_FAILURE);
		}
		riter = x->seq->rbegin();
		for (i = -2; i >= index; i--) {
			++riter;
		}
		p = &(*riter);
	}
	// return a duplicate, or an atom
	myob = new (true) object(p); //--here--
	return myob;
}

void object::set(integer index, object * param) {
	integer size;
	integer i;
	//here, cannot use duplicate of setting the data
	object * x;
	//if (flag == is_duplicate) {
	while (flag == is_duplicate) {
		x = new (false) object(this);
		delete_object();
		reserved = x->reserved;
		flags = x->flags;
	}
	if (flag == is_repeat) {
		expand_repeat();
	}
	if ((flag != is_sequence) || (seq == NULL)) {
		is_error_message = true;
		std::cerr << "Error: object is not a sequence, called from get() library function." << std::endl;
		exit(EXIT_FAILURE);
	}
	size = (integer)seq->size();
	if ((index > size) || (index < -size) || (index == 0)) {
		is_error_message = true;
		std::cerr << "Error: syntax checking, wrong index, called from get() library function." << std::endl;
		exit(EXIT_FAILURE);
	}
	if (index >= 1) {
		MYSEQUENCE::iterator iter;
		iter = seq->begin();
		for (i = 2; i <= index; i++) {
			++iter;
		}
		//(*iter).delete_object(); // next line deletes object
		*iter = param; // overloaded operator, makes a duplicate
	}
	else {
		MYSEQUENCE::reverse_iterator riter;
		riter = seq->rbegin();
		for (i = -2; i >= index; i--) {
			++riter;
		}
		//(*riter).delete_object(); // next line deletes object
		*riter = param; // overloaded operator, makes a duplicate
	}
}
/*
//here:
object& object::operator[] (size_t offset) {
    object * ptr;
    if (IS_DUPLICATE(flags)) {
        //object * ptr;
        ptr = duplicate->ptr;
        if (duplicate->count > 0) {
            duplicate->count--;
            make_object(ptr);
        }
        else {
            duplicate->ptr = NULL;
            delete duplicate;

            ob = ptr->ob;
            length = ptr->length;
            flags = ptr->flags;

            ptr->ob = NULL;
            delete ptr;
        }
    }
    if (IS_REPEAT(flags)) {
        expand_repeat();
    }
    if (!IS_ATOM(flag)) {
        if (ob != NULL) {
            if (offset < length) {
                ptr = ob;
                ptr += offset; // pointer arithmetic
                return *ptr;
            }
        }
    }
    std::cerr << "Error: bad index passed to library function." << std::endl;
    exit(EXIT_FAILURE);
}
*/
object& object::operator= (integer param) {
    delete_object();
    make_object(param);
    return *this;
}
object& object::operator= (uinteger param) {
    delete_object();
    make_object(param);
    return *this;
}
object& object::operator= (atom param) {
    delete_object();
    make_object(param);
    return *this;
}
#if BITS != 64
object& object::operator= (long long param) {
    delete_object();
    make_object((integer)param);
    return *this;
}
object& object::operator= (unsigned long long param) {
    delete_object();
    make_object((uinteger)param);
    return *this;
}
object& object::operator= (double param) {
    delete_object();
    make_object((atom)param);
    return *this;
}
#endif
//*
// The assignment operator:
bool object::is_object_in_nested_seq(object * ob) {
	object * x;
	x = this;
	while (x->flag == is_duplicate) {
		x = x->duplicate->ptr;
	}
	if (x == ob) {
		return true;
	}
	if (x->flag == is_repeat) {
		if (x->repeat->ptr == NULL) {
			// it represents an array of "count" number of empty sequences
			return false;
		}
		return x->repeat->ptr->is_object_in_nested_seq(ob);
	}	
	if (x->flag == is_sequence) {
		if (x->seq != NULL) {
			for (MYSEQUENCE::iterator it = x->seq->begin(); it != x->seq->end(); ++it) {
				if ((*it).is_object_in_nested_seq(ob)) {
					return true;
				}
			}
		}
	}
	return false;
}

object& object::operator= (object * param) {
	// 
	// only problem is when "this" contains "param", or "param" contains "this" (problem solved)
	if (this == param) {
		return *this;
	}
	//old_data = new (false) object(reserved, flags);
	switch (param->flag)
	{
	case is_integer:
	    delete_object();
	    make_object(param->idata);
		return *this;
	case is_uinteger:
	    delete_object();
	    make_object(param->udata);
		return *this;
	case is_atom:
	    delete_object();
	    make_object(param->fdata);
		return *this;
	}
// if param contains "this", then when "this" gets deleted, part of param also gets deleted
//  skip over deleting "this"
	if (!param->is_object_in_nested_seq(this)) {
// if "this" contains param, then when "this" gets deleted, param gets deleted (with dup, param did not get deleted)
		size_t b;
		param->make_object(param); // make param into a duplicate
		b = param->duplicate->count;
		delete_object(); // if param is in "this", then count will be decreased by one (1)	
		if (b == param->duplicate->count) {
			// param was not stored in "this"
			param->duplicate->count--;
		}
	}
	// param was found in "this"
	make_object(param);
	return *this;
}

/*
#define assignment_op(T) \
object& object::operator T (object& param) { \
    switch (flag) \
    { \
    case is_integer: \
        switch (param.flag) \
        { \
        case is_integer: \
            idata T param.idata; \
            return *this; \
        case is_atom: \
            fdata = (atom)idata; \
            fdata T param.fdata; \
            return *this; \
        case is_sequence: \
        default: \
            break; \
        } \
    case is_atom: \
        switch (param.flag) \
        { \
        case is_integer: \
            fdata T (atom)param.idata; \
            return *this; \
        case is_atom: \
            fdata T param.fdata; \
            return *this; \
        case is_sequence: \
        default: \
            break; \
        } \
    case is_sequence: \
    default: \
        break; \
    } \
    size_t i; \
    object * a; \
    switch (param.flag) \
    { \
    case is_integer: \
    case is_atom: \
        a = ob; \
        for (i = 0; i < length; i++) { \
            *a++ T param; \
        } \
        return *this; \
    case is_sequence: \
    default: \
        size_t i; \
        object * a; \
        object * b; \
        if (length != param.length) { \
            std::cerr << "Error: lengths of data are not equal in library function." << std::endl; \
            exit(EXIT_FAILURE); \
        } \
        a = ob; \
        b = param.ob; \
        for (i = 0; i < length; i++) { \
            *a++ T *b++; \
        } \
        return *this; \
    } \
}

#define int_assignment_op(T) \
object& object::operator T (object& param) { \
    if (flag == is_integer) { \
        if (param.flag == is_integer) { \
            idata T param.idata; \
            return *this; \
        } \
    } \
    if (flag != is_atom) { \
        size_t i; \
        object * a; \
        if (param.flag == is_integer) { \
            a = ob; \
            for (i = 0; i < length; i++) { \
                *a++ T param; \
            } \
            return *this; \
        } \
        if (param.flag != is_atom) { \
            object * b; \
            if (length != param.length) { \
                std::cerr << "Error: lengths of data are not equal in library function." << std::endl; \
                exit(EXIT_FAILURE); \
            } \
            a = ob; \
            b = param.ob; \
            for (i = 0; i < length; i++) { \
                *a++ T *b++; \
            } \
            return *this; \
        } \
    } \
    std::cerr << "Error: data is not an integer in library function." << std::endl; \
    exit(EXIT_FAILURE); \
}

assignment_op(+= )
assignment_op(-= )
assignment_op(*= )
assignment_op(/= )

int_assignment_op(%= )
int_assignment_op(^= )
int_assignment_op(&= )
int_assignment_op(|= )

int_assignment_op(<<= )
int_assignment_op(>>= )

#define myoperator(S,T) \
object * object::operator S (object& param) { \
    object * tmp; \
    tmp = new object(this); \
    *tmp T param; \
    return tmp; \
}

myoperator(+, +=)
myoperator(-, -=)
myoperator(*, *=)
myoperator(/ , /=)

myoperator(%, %=)
myoperator(^, ^=)
myoperator(&, &=)
myoperator(| , |=)

myoperator(<< , <<=)
myoperator(>> , >>=)
*/


// put more functions here

void object::cprint() {
	object * x;
	x = this;
	while (x->flag == is_duplicate) {
		x = x->duplicate->ptr;
	}
	if (x->flag == is_repeat) {
		size_t i, len;
		object * a;
		std::cout << "{";
		len = x->repeat->count;
		if (len != 0) {
            a = x->repeat->ptr;
			if (a == NULL) {
				std::cout << "{}";
			}
			else {
	            a->cprint(); // print first value
			}
            for (i = 1; i < len; i++) { // print the rest of the values
                std::cout << ",";
				if (a == NULL) {
					std::cout << "{}";
				}
				else {
					a->cprint(); // print first value
				}
            }
		}
		std::cout << "}";
		return;
	}
#if BITS == 128
{
	bool atom_flag = true;
	__float128 r;
	char buf[128];
	switch (x->flag) {
	case is_integer:
		r = (__float128) x->idata;
		break;
	case is_uinteger:
		r = (__float128) x->udata;
		break;
	case is_atom:
		r = (__float128) x->fdata;
		break;
	default:
		atom_flag = false;
		break;
	}
	if (atom_flag) {
		int n = quadmath_snprintf (buf, sizeof(buf), "%Qg", r);
		//int n = quadmath_snprintf (buf, sizeof(buf), "%+-#46.20Qe", r);
		if ((size_t)n < sizeof(buf)) {
			printf ("%s", buf);
		}
		return;
	}
}
#else
	switch (x->flag) {
	case is_integer:
		std::cout << x->idata;
		return;
	case is_uinteger:
		std::cout << x->udata;
		return;
	case is_atom:
		std::cout << x->fdata;
		return;
	default:
		break;
	}
#endif
	if (x->seq == NULL) {
		std::cout << "{}";
		return;
	}
	if (x->seq->size() == 0) {
		std::cout << "{}";
		return;
	}
	if (x->flag == is_sequence) {
	    MYSEQUENCE::iterator iter = x->seq->begin();
		std::cout << "{";
		iter->cprint(); // print first value
		iter++;
		while (iter != x->seq->end()) {
            std::cout << ",";
            iter->cprint();
            ++iter;
		}
		std::cout << "}";
		return;
	}
}

void object::pretty_print() {
	// pretty_print not done yet
	cprint();
}

void object::delete_object() {
// delete top-level objects
	switch (flag)
	{
	case is_integer:
	case is_uinteger:
	case is_atom:
		return;
    case is_duplicate:
		if (duplicate->count > 1) {
			duplicate->count--;
			return;
		}
		else {
			delete duplicate;
			return;
		}
    case is_repeat:
		//RESET_IS_REPEAT(flags);
		//if ((flag != is_integer) && (flag != is_atom)) {
		delete repeat;
		//}
		return;
    case is_sequence:
		// always a sequence:
/*		if (seq != NULL) {
			nested_delete++;
			seq->clear();
			nested_delete--;
		}*/
		nested_delete++;
		delete seq;
		nested_delete--;
		return;
	}
	std::cerr << "Error: memory leak, did not catch object in delete_object()" << std::endl;
}

object::~object()
{
	delete_object();
}
/*	if (!find(all_objects, this)) {
		if (nested_delete > 0) {
			delete_object();
		}
		return;
	}
	all_objects.remove(this);
	delete_object();
*/

void DELETE_OBJECT(object * ptr) {
// Delete top-level objects
//	if (!find(all_objects, ptr)) {
//		return;
//	}
	all_objects.remove(ptr);
	//ptr->delete_object();
	::delete ptr;
}

//end of file

dup::dup()
{
	ptr = NULL;
	count = 0;
}
dup::dup(object * val, size_t c) {
	ptr = val;
	count = c;
}


void dup::delete_dup() {
	// A copy of the deconstructor:
	//delete this;
	nested_delete++;
	::delete ptr;
	nested_delete--;
	//ptr->delete_object();
	//delete ptr;
}

dup::~dup()
{
	delete_dup();
}

//end of file

rep::rep()
{
	ptr = NULL;
	count = 0;
}
rep::rep(object * val, size_t c) {
	ptr = val;
	count = c;
}


void rep::delete_rep() {
	// A copy of the deconstructor:
	//delete this;
	nested_delete++;
	::delete ptr;
	nested_delete--;
	//ptr->delete_object();
	//delete ptr;
}

rep::~rep()
{
	delete_rep();
}

