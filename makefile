all: 
	cd ./libavalam; make
	cd ./libjoueur; make
	cd ./libjoueur-instantane; make
	cd ./moteur; make
	cd ./duel; make
	cd ./duel-instantane; make
	cd ./joueurs; make
	cd ./joueurs-instantane; make
	cd ./genPositions; make

debug: 
	cd ./libavalam; make debug
	cd ./libjoueur; make debug
	cd ./libjoueur-instantane; make debug
	cd ./moteur; make debug
	cd ./duel; make debug
	cd ./duel-instantane; make debug
	cd ./joueurs; make debug
	cd ./joueurs-instantane; make debug
	cd ./genPositions; make debug
	
clean: 
	rm -rf ./tournoi/run/*
	rm -rf ./tournoi/web/data/*
	rm -rf /dev/shm/*.avalam
	cd ./libavalam; make clean
	cd ./libjoueur; make clean
	cd ./libjoueur-instantane; make clean
	cd ./moteur; make clean
	cd ./duel; make clean
	cd ./duel-instantane; make clean
	cd ./joueurs; make clean
	cd ./joueurs-instantane; make clean
	cd ./genPositions; make clean

