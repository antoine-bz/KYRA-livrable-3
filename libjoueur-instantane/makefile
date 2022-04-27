all: install 

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: install

INC=-I../include
LIB=-L../libavalam

install: libjoueur-instantane.a libjoueur-instantane.so
	cp  libjoueur-instantane.a libjoueur-instantane.so ../tournoi/lib

libjoueur-instantane.a : libjoueur-instantane.c
	gcc $(INC)  $(DEBUG) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -c libjoueur-instantane.c -o libjoueur-instantane.o
	ar -qvs libjoueur-instantane.a libjoueur-instantane.o

libjoueur-instantane.so : libjoueur-instantane.c
	gcc $(INC)  $(DEBUG) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -shared -fPIC libjoueur-instantane.c -o libjoueur-instantane.so 

clean: 
	rm -rf libjoueur-instantane.a libjoueur-instantane.so libjoueur-instantane.o
	rm -rf ../tournoi/lib/libjoueur-instantane.a ../tournoi/lib/libjoueur-instantane.so
