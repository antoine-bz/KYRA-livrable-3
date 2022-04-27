#! /bin/sh 

MAX=$1
if [ $# -ne 1 ]; then 
	MAX=3
fi

# Pour lancer un joueur : ./bin/<programmeJoueur> <NomJoueur>

numJ=1
while [ $numJ -le $MAX ]
do
	echo "Creation du joueur $numJ"
	./bin/joueur$numJ-instantane.exe "Joueur $numJ" &
	numJ=$(expr $numJ + 1)
done
