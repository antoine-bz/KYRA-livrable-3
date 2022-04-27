all: dep install 

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: dep install

dep: 
	cd ../libavalam; make

install: dep moteur.exe
	cp moteur.exe ../tournoi/bin

LIB=-L../tournoi/lib
INC=-I../include

moteur.exe:moteur.c 
	gcc $(DEBUG) $(LIB) $(INC) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast moteur.c -o moteur.exe -lpthread -lavalam

moteur.static:moteur.c 
	gcc -static $(DEBUG) $(INC) $(LIB) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast moteur.c -o moteur.static -lpthread -lavalam

clean: 
	rm -rf moteur.exe moteur.static
	rm -rf ../tournoi/bin/moteur.exe ../tournoi/bin/moteur.static
	
cleanall: clean
	cd ../libavalam; make clean
