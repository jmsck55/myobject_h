objects = main.o stdafx.o obj.o

a.out : $(objects)
		g++ -g -o a.out $(objects) -std=gnu++11 -lquadmath

main.o : main.cpp stdafx.h obj.h
		g++ -g -c main.cpp -std=gnu++11 -lquadmath
stdafx.o : stdafx.cpp stdafx.h
		g++ -g -c stdafx.cpp -std=gnu++11 -lquadmath
obj.o : obj.cpp stdafx.h obj.h
		g++ -g -c obj.cpp -std=gnu++11 -lquadmath

.PHONY : clean
clean :
		rm a.out $(objects)
