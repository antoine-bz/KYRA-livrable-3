#! /bin/bash

# déplace les fichiers de  tournoi/web/data  dont le nom commence par un “p” dans le répertoire avalamdb/db, dans un répertoire portant un nom correspondant à la date et l’heure actuelle, sans espaces.

PATHDATA=$(pwd)/../tournoi/web/data
PATHDB=$(pwd)
 
DATE=$(date '+%d%m%Y%H%M')

mkdir -p $PATHDB/db/$DATE
mv $PATHDATA/p* $PATHDB/db/$DATE
