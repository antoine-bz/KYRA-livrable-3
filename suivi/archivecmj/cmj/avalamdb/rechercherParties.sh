#! /bin/bash

#recherche toutes les parties jouées par le joueur dont le nom est passé en argument, et crée dans le répertoire avalamdb/search des liens symboliques menant aux parties trouvées, ainsi qu’un fichier parties.html permettant de recenser ces parties. Le format du fichier parties.html est donné en annexes. Une fois ce fichier créé, le script lance un navigateur pour le visualiser.

PATHDB=$(pwd)

NOM=$1

rm -r  $PATHDB/search
mkdir -p $PATHDB/search
echo "<h1>Parties du joueur $NOM </h1>" > $PATHDB/search/parties.html

> resultats


# on recupere les noms des fichiers contenant le NOM cherche 
find $PATHDB/db -type f -exec grep -H "\"j\":\".*$NOM.*\"" {} \; | cut -d: -f1 > resultats
find $PATHDB/db -type f -exec grep -H "\"r\":\".*$NOM.*\"" {} \; | cut -d: -f1 >> resultats

NUMFIC=1
# Il faut parcourir les lignes de chaque fichier 
for ligne in $(cat resultats)
do 
	# echo "traitement de $ligne"
	ln -s $ligne $PATHDB/search/p$NUMFIC

	j=$(cat $ligne | grep '"j":' | cut -d: -f2 | tr -d ',"')
	r=$(cat $ligne | grep '"r":' | cut -d: -f2 | tr -d ',"')

	echo "<a href="../web/avalam-partie.html?p=p$NUMFIC">$j - $r</a> <br />" >> $PATHDB/search/parties.html

	NUMFIC=$(expr $NUMFIC + 1)
done 

rm resultats
firefox $PATHDB/search/parties.html & 

