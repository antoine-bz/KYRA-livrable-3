#! /bin/bash

if [ $# -eq 0 ]
then 
	echo "genPositions <graine> [<graine>]*"
	exit 0
fi

POS=""

INDICE=1
echo "Parcours des arguments:" 
for ARG in $@
do
	echo "Argument $INDICE : $ARG"
	
	chaineFEN=$(./alea_avalam.exe $ARG 2>/dev/null)
	echo "Chaine FEN obtenue : $chaineFEN"

	echo $ARG.json > fichier_commentaire
	echo "Mot utilise pour générer cette position : $ARG" >> fichier_commentaire

	./web/mkDiag.exe $INDICE "$chaineFEN j" < fichier_commentaire

	INDICE=$(expr $INDICE + 1)

	# Hypothèse : le programme mkDiag écrit sous data/ (son comportement par défaut...)
	POS="$POS ./web/avalam-diag.html?p=./data/$ARG.json" 
done

rm fichier_commentaire
firefox $POS &
