all: install

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: install

INC=-I../include

install: libavalam.so libavalam.a
	cp libavalam.so libavalam.a ../tournoi/lib

libavalam.so : libavalam.c
	gcc $(DEBUG) $(INC) -shared -fPIC libavalam.c -o libavalam.so

libavalam.a : libavalam.c
	gcc $(DEBUG) $(INC) -c  libavalam.c -o libavalam.o
	ar qvs libavalam.a  libavalam.o

clean: 
	rm -rf  libavalam.so libavalam.a libavalam.o
	rm -rf  ../tournoi/lib/libavalam.so ../tournoi/lib/libavalam.a

