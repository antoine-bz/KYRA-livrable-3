all: install 

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: install

INC=-I../include
LIB=-L../libavalam

install: libjoueur.a libjoueur.so
	cp  libjoueur.a libjoueur.so ../tournoi/lib

libjoueur.a : libjoueur.c
	gcc $(INC)  $(DEBUG) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -c libjoueur.c -o libjoueur.o
	ar -qvs libjoueur.a libjoueur.o

libjoueur.so : libjoueur.c
	gcc $(INC)  $(DEBUG) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -shared -fPIC libjoueur.c -o libjoueur.so 

clean: 
	rm -rf libjoueur.a libjoueur.so libjoueur.o
	rm -rf ../tournoi/lib/libjoueur.a ../tournoi/lib/libjoueur.so
