all: dep install 

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: dep install

dep: 
	cd ../libavalam; make

install: dep duel.exe
	cp duel.exe ../tournoi/bin

LIB=-L../tournoi/lib
INC=-I../include

duel.exe:duel.c 
	gcc $(DEBUG) $(LIB) $(INC) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast duel.c -o duel.exe -lpthread -lavalam

duel.static:duel.c 
	gcc -static $(DEBUG) $(INC) $(LIB) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast duel.c -o duel.static -lpthread -lavalam

clean: 
	rm -rf duel.exe duel.static
	rm -rf ../tournoi/bin/duel.exe ../tournoi/bin/duel.static
	
cleanall: clean
	cd ../libavalam; make clean
