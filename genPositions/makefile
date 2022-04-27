all : alea_avalam.exe

# Pour activer le mode debugage, il faut définir __DEBUG__
debug: DEBUG=-D'__DEBUG__'
debug: alea_avalam.exe

alea_avalam.exe: alea_avalam.c
	gcc $(DEBUG) $(LIB) $(INC) alea_avalam.c -o alea_avalam.exe 

clean: 
	rm -rf alea.exe alea_avalam.exe
	rm -rf data/*
