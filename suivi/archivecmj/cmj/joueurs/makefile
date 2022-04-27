all: dep install

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: dep install

dep:
	cd ../libavalam; make
	cd ../libjoueur; make

LIB=-L../tournoi/lib
INC=-I../include
OPTS=-D_POSIX_C_SOURCE=199309L -std=c99

SRCTOM=humain1.c humain2.c humain3.c joueur1.c joueur2.c joueur3.c joueur4.c joueur5.c

SRC2018=floyd.c floyd2.c naysson.c

SRC2019=humain1.c humain2.c humain3.c 1-avaltoname.c 2-legende.c 3-avalam2i.c 4-yazur.c 5-vroum.c 6-alexa.c 7-aya.c 8-botabdul.c 9-mark4.c 10-chomage.c 11-neotheone.c 12-shao.c 13-ig22qi.c 14-kantaou2i.c 15-binks.c 16-jarvis.c 17-tsavalam.c 14-kantaou2i-2.c 14-kantaou2i-blitz.c 9-mark5.c 5-vroum-2.c 3-avalam2i-2.c kantalam2i.c kantalam2i-2.c alexandre.c

SRC=humain1.c humain2.c humain3.c 10-avalamHub.c 13-coronAvalam.c 4-bahahavalam.c 7-patrickBalkalam.c 11-hakunamavalam.c 1-savalami.c 5-airalam.c 8-AvalamKedavra.c 12-sidAvalam.c 3-avalamaFache.c 6-swag.c 9-AvalamCorp.c 2-faceavalamer.c hakunamavalam2.c
#14-kantaou2i-blitz.c 9-mark5.c 5-vroum-2.c 3-avalam2i-2.c kantalam2i-blitz-2.c

EXE=$(SRC:.c=.exe)
STATIC=$(SRC:.c=.static)

exe: $(EXE)
static: $(STATIC)

install: dep exe
	cp $(EXE) ../tournoi/bin

# Ajouter $(OPTS) pour exécution sur VM
%.exe : %.c
	gcc $(OPTS) $(DEBUG) $(LIB) $(INC) $< -o $@ -ljoueur -lpthread -lavalam

%.static : %.c
	gcc $(DEBUG) -static $(LIB) $(INC) $< -o $@ -ljoueur -lpthread -lavalam

clean: 
	rm -rf $(EXE) $(STATIC)
	cd ../tournoi/bin; rm -rf $(EXE) $(STATIC)

cleanall: clean
	cd ../libavalam; make clean
	cd ../libjoueur; make clean
