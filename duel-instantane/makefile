all: dep install 

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: dep install

dep: 
	cd ../libavalam; make

install: dep duel-instantane.exe
	cp duel-instantane.exe ../tournoi/bin

LIB=-L../tournoi/lib
INC=-I../include

duel-instantane.exe:duel-instantane.c 
	gcc $(DEBUG) $(LIB) $(INC) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast duel-instantane.c -o duel-instantane.exe -lpthread -lavalam

duel-instantane.static:duel-instantane.c 
	gcc -static $(DEBUG) $(INC) $(LIB) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast duel-instantane.c -o duel-instantane.static -lpthread -lavalam

clean: 
	rm -rf duel-instantane.exe duel-instantane.static
	rm -rf ../tournoi/bin/duel-instantane.exe ../tournoi/bin/duel-instantane.static
	
cleanall: clean
	cd ../libavalam; make clean
