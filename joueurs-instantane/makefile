all: dep install

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: dep install

dep:
	cd ../libavalam; make
	cd ../libjoueur; make

LIB=-L../tournoi/lib
INC=-I../include

SRC=humain1-instantane.c humain2-instantane.c joueur1-instantane.c joueur2-instantane.c joueur3-instantane.c joueur4-instantane.c 
EXE=$(SRC:.c=.exe)
STATIC=$(SRC:.c=.static)

exe: $(EXE)
static: $(STATIC)

install: dep exe
	cp $(EXE) ../tournoi/bin

%.exe : %.c
	gcc $(DEBUG) $(LIB) $(INC) $< -o $@ -ljoueur-instantane -lpthread -lavalam

%.static : %.c
	gcc $(DEBUG) -static $(LIB) $(INC) $< -o $@ -ljoueur-instantane -lpthread -lavalam

clean: 
	rm -rf $(EXE) $(STATIC)
	cd ../tournoi/bin; rm -rf $(EXE) $(STATIC)

cleanall: clean
	cd ../libavalam; make clean
	cd ../libjoueur; make clean
