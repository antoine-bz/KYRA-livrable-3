all: 
	cd ./libavalam; make
	cd ./libjoueur; make
	cd ./moteur; make
	cd ./duel; make
	cd ./joueurs; make
	cd ./genPositions; make

debug: 
	cd ./libavalam; make debug
	cd ./libjoueur; make debug
	cd ./moteur; make debug
	cd ./duel; make debug
	cd ./joueurs; make debug
	cd ./genPositions; make debug
	
clean: 
	rm -rf ./tournoi/run/*
	rm -rf ./tournoi/web/data/*
	rm -rf /dev/shm/*.avalam
	cd ./libavalam; make clean
	cd ./libjoueur; make clean
	cd ./moteur; make clean
	cd ./duel; make clean
	cd ./joueurs; make clean
	cd ./genPositions; make clean

