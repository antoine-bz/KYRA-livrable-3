/********* Moteur de tournoi : HAKUNAMAVALAM ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"
#include <time.h>
#define nbsituation 21 //nombre de situations servant à l'analyse 
#define TEMPSALOUE 1 //temps aloué pour jouer en secondes (à ne pas definir dans l'etat de fonctionement actuelle du programme)
#define TPSANALYSE 0 //temps moyen utilisé pour l'analyse des situations (à ne pas definir dans l'etat actuelle du programme)
//prototype de fonction 
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups);
int ComparerCoup(int chalengeur,int meilleurCoup,double *poidscoups);
int rangoptimum(int nbcouptt,int nbcoutanaly,int const tempsaloue);
void recursif(int rang,T_Position P,double *poidscoups, const int rangarret,octet myColor, int indicemodif,int indicepoid);
void analysersituation(T_Position P,int rang, int i, T_ListeCoups listeCoups,double *poidscoups,octet myColor,double listconstpoid[][3]);//initialisation
void situation1(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]);
int voisinsInferieurs(T_Position P,octet myColor,T_Coup Coup);
void situation2(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]);
void situation3(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3],T_ListeCoups listeCoups);
void situation4(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]);
void situation5(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]);
void situation6(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3],T_ListeCoups listeCoups);
void situation7(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3],T_ListeCoups listeCoups);
void situation1contre(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]);
void afecterpoid(int rang,double listconstpoid[][3],int indicesituation,double *ptrpoid);
double listconstpoid[nbsituation][3]=
{// {rang 1,ran 2 , constante k de division}
	{800,100,190},//situation 2.1  //0
	{900,-9000,190},//situation 2.2 //1
	{-900,-9000,190},//situation 2.3//2
	{-800,-8000,190},//situation 2.4//3
	
	{900,200,190},//situation 1.1//4
	{1000,-10000,190},//situation 1.2//5
	{-1000,-10000,190},//situation 1.3//6
	{-900,-9000,190},//situation 1.4//7
	
	{-19000,1000,190},//situation 3//8
	
	{100000,0,0},//situation 4//9//mettre null fait une erreur
	
	{-100,0,190},//situation 5.1//10
	{-300,-30,190},//situation 5.2//11
	{100,-10,190},//situation 5.3//12	
	{100,0,190},//situation 5.4//13
	
	{-10000,0,0},//fausse situation ponderation forcé.//14

	{10000,0,190},//situation 6.1 //15
	{-10000,0,190},//situation 6.2 //16
	
	{-900,-10,190},//situation 7.1//17
	{10000,0,190},//situation 7.2//18
	{10000,0,190},//situation 7.3//19
	{9000,0,190}//situation 7.4//20
	//ajouter ici d'autre constante de situation (ne pas oublier d'augmenter nbsituation en consequence)
};


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int i=0; //numero du coups analysés 
	octet myColor = currentPosition.trait; 
	//printf1("mycolor:%d",myColor);
	double poidscoups[listeCoups.nb];
	int meilleurCoup=0;
	int rang =1;//rang d'analyse initialisé à 1 et non 0 comme dans le livrable 2 pour se faciliter les calcules sur les modulos
	int rangarret=3;//a determiner selon une fonction dependante du temps imparti //ici initilisé à 2 dans l'etat actuelle de l'ia
	int nbcoupanalyser=0;//nombre de coups passant une analyse poussé sur le rang 1
	int PALIEPOID=0;
	T_Position PFuture;
	//variable de mesure de temps pour les relevé statistique
	long clk_tck = CLOCKS_PER_SEC;
	clock_t t1, t2;
	//printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	// afficherListeCoups(listeCoups);
	/* Recuperation du temps initial en "clock ticks" */
	t1 = clock();
	for(i=0;i<listeCoups.nb; i++) {poidscoups[i]=0;} //on met les poids à 0
	for(i=0;i<listeCoups.nb; i++) {//on analyse les coups direct evidement
		printf("\nrang 1 %d->%d\n",listeCoups.coups[i].origine,listeCoups.coups[i].destination);
		analysersituation(currentPosition,rang,i,listeCoups,&(poidscoups[i]),myColor,listconstpoid);
		if(ComparerCoup(i,meilleurCoup,poidscoups)==1){//on selectionne le meilleur coup
			meilleurCoup=i;
		}
		
	}
	//ecrireIndexCoup(meilleurCoup);
	for(i=0;i<listeCoups.nb; i++) {if(poidscoups[i]>PALIEPOID)nbcoupanalyser++;}
	//printf("nbcoup:%d nbcoup analyser:%d\n",listeCoups.nb,nbcoupanalyser);
	//rangarret=rangoptimum(listeCoups.nb,nbcoupanalyser,TEMPSALOUE);//calcule du rang d'arret
	for(i=0;i<listeCoups.nb; i++) {	
		if(rang<rangarret){
		if(poidscoups[i]>PALIEPOID){ //si le rang d'arret  le permet et que le coup a un interé strategique minimal calculer les poids pour les coups futures
			PFuture=jouerCoup(currentPosition,listeCoups.coups[i].origine,listeCoups.coups[i].destination);
			//afficherPosition(currentPosition);
			//afficherPosition(PFuture);
			printf("\nrang 2 %d->%d\n",listeCoups.coups[i].origine,listeCoups.coups[i].destination);
			recursif(rang+1,PFuture,poidscoups,rangarret,myColor,listeCoups.coups[i].destination,i);
		}
		else{
			afecterpoid(1,listconstpoid,14,&poidscoups[i]);//on force l'exclusion des coups respectant le critére de poid au rang 1
		}
		}
	}
	for(i=0;i<listeCoups.nb; i++) {
		if(ComparerCoup(i,meilleurCoup,poidscoups)==1){//on selectionne le meilleur coup aprés l'analyse approfondit
			meilleurCoup=i;
			printf("\nmeilleurs coups %lf pour %d->%d\n",poidscoups[i],listeCoups.coups[i].origine,listeCoups.coups[i].destination);
		}
		
	}
	ecrireIndexCoup(meilleurCoup);
	/* Recuperation du temps final en "clock ticks" */
	t2 = clock();
	/* Affichage des différents temps et temps consommes */
   (void)printf("Nb ticks/seconde = %ld,  Nb ticks depart : %ld, "
   "Nb ticks final : %ld\n",
    clk_tck, (long)t1, (long)t2);
   (void)printf("Temps consomme (s) : %lf \n",(double)(t2-t1)/(double)clk_tck);
}
//retourne 1 si le coups chalengeur et plus interessant que le precedent meilleur coups
int ComparerCoup(int chalengeur,int meilleurCoup,double *poidscoups){
	if(poidscoups[chalengeur]>=poidscoups[meilleurCoup]){return 1;}
	return 0;
}
//parcours les coups succesifs possibles , et affecte le poids du coup d'origine en fonction du rang
void recursif(int rang,T_Position P,double *poidscoups,const int rangarret,octet myColor,const int indicemodif,int indicepoid){
	int i;//indice de coup
	T_Position PFuture;//position future engendré par un coup 
	T_ListeCoups listeCoups;
	listeCoups = getCoupsLegaux(P);// on initialise la nouvelle liste de coups
	for(i=0;i<listeCoups.nb; i++) {//on parcours les coups
		if(indicemodif==listeCoups.coups[i].origine||indicemodif==listeCoups.coups[i].destination){//on analyse que les coups impacté directement par le coup du rang d'avant
			analysersituation(P,rang,i,listeCoups,&(poidscoups[indicepoid]),myColor,listconstpoid);//on affecte les poids pour se rang 
		}
	}
	if(rang<rangarret){//si le rang d'arret le permet parcourir le rang suivant
		for(i=0;i<listeCoups.nb; i++) {//on parcour les coups
			if(indicemodif==listeCoups.coups[i].origine||indicemodif==listeCoups.coups[i].destination){//on analyse que les coups impacté directement par le coup du rang d'avant
				//on reinitilise PFuture celon le coup
				PFuture=jouerCoup(P,listeCoups.coups[i].origine,listeCoups.coups[i].destination);
				//on passe a l'analyse recursive suivente
				recursif(rang+1,PFuture,poidscoups,rangarret,myColor,listeCoups.coups[i].destination,indicepoid);
			}
		}
	}
}
//appele les differente situations pour analyser un coup donné et affecter des ponderation au coup de rang 1 qui en est l'origine 
void analysersituation(T_Position P,int rang, int i, T_ListeCoups listeCoups,double *poidscoups,octet myColor,double listconstpoid[][3]){
	int taillepilefuture;
	taillepilefuture=P.cols[listeCoups.coups[i].origine].nb + P.cols[listeCoups.coups[i].destination].nb;
	situation5(P,myColor,listeCoups.coups[i],rang,(poidscoups),listconstpoid); 
	situation4(P,myColor,listeCoups.coups[i],rang,(poidscoups),listconstpoid); 
	situation3(P,myColor,listeCoups.coups[i],rang,(poidscoups),listconstpoid,listeCoups); 
	situation6(P,myColor,listeCoups.coups[i],rang,(poidscoups),listconstpoid,listeCoups); 
	situation7(P,myColor,listeCoups.coups[i],rang,(poidscoups),listconstpoid,listeCoups); 
	if(taillepilefuture<5){//si pile definitive non complète
		situation1(P,myColor,listeCoups.coups[i],rang,(poidscoups),listconstpoid); 
	}
	else {//si pile complète
		situation2(P,myColor,listeCoups.coups[i],rang,(poidscoups),listconstpoid);
		}
}
//situation 1 :regarde si on peut créer des piles définitives de non 5(ex:une pile de 4 n'ayant que des voisins 2)
void situation1(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]){
	if(voisinsInferieurs(P,myColor,Coup)==1){
		if(P.cols[Coup.origine].couleur==myColor){
		
			if(P.cols[Coup.destination].couleur==myColor){ //  M/M
				afecterpoid(rang,listconstpoid,4,ptrpoid);
				printf("Pd M/M%d->%d ",Coup.origine,Coup.destination);
				printf("poid:%lf ",*ptrpoid);
			}
			else{ //	M/L

				afecterpoid(rang,listconstpoid,5,ptrpoid);
				printf("Pd M/L%d->%d ",Coup.origine,Coup.destination);
				printf("poid:%lf ",*ptrpoid);
			}	
		}
		else{
			if(P.cols[Coup.destination].couleur==myColor){ //	L/M
				afecterpoid(rang,listconstpoid,6,ptrpoid);
				printf("Pd L/M%d->%d ",Coup.origine,Coup.destination);
				printf("poid:%lf ",*ptrpoid);
			}
			else{ //	L/L
				afecterpoid(rang,listconstpoid,7,ptrpoid);
				printf("Pd L/L%d->%d ",Coup.origine,Coup.destination);
				printf("poid:%lf ",*ptrpoid);
			}
		}
	}
}

//permet de savoir les les voisins de la destination pourront créer une pile au tour suivant
int voisinsInferieurs(T_Position P,octet myColor,T_Coup Coup)
{
T_Voisins v;
int test=1;
int i=0;
v=getVoisins(Coup.destination);
	while(i<v.nb && test!=0)
	{
		if(v.cases[i]!=Coup.origine)
		{
			if((P.cols[v.cases[i]].nb+P.cols[Coup.destination].nb+P.cols[Coup.origine].nb)<=5)
			{
			test=0;
			}
		}
	i++;
	}
return test;
}
//afectation des poids dans le cas d'une pile complète : situation 2
void situation2(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]){
	
	if(P.cols[Coup.origine].couleur==myColor){
		if(P.cols[Coup.destination].couleur==myColor){ //  M/M
			afecterpoid(rang,listconstpoid,0,ptrpoid);
			printf("P5 M/M%d->%d ",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
		}
		else{ //	M/L
			afecterpoid(rang,listconstpoid,1,ptrpoid);
			printf("P5 M/L%d->%d ",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
		}	
	}
	else{
		if(P.cols[Coup.destination].couleur==myColor){ //	L/M
			afecterpoid(rang,listconstpoid,2,ptrpoid);
			printf("P5 L/M%d->%d ",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
		}
		else{ //	L/L
			afecterpoid(rang,listconstpoid,3,ptrpoid);
			printf("P5 L/L%d->%d ",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
		}
	}
}

//situation 3 pondere les coups inevitablement favorable
void situation3(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3],T_ListeCoups listeCoups){
int var=1;
int i;
	if(P.cols[Coup.origine].couleur==P.cols[Coup.destination].couleur)//si origine et destination on la même couleur
	{
		for(i=0;i<listeCoups.nb&&var==1;i++){//on parcours la liste des coups 
			if((listeCoups.coups[i].origine!=Coup.origine||listeCoups.coups[i].origine!=Coup.destination)&&//on exclus notre origine et notre destination
			   (listeCoups.coups[i].destination!=Coup.origine||listeCoups.coups[i].destination!=Coup.destination))//si un autre coups peux s'interposé
			{
				var=0;
			}
		}
	}
	else{var=0;}
	if(var==1){
	afecterpoid(rang,listconstpoid,8,ptrpoid);
	printf("ine favo %d->%d ",Coup.origine,Coup.destination);
	printf("poid:%lf ",*ptrpoid);
	}
}


//situation 4:cette situation est vide pour le moment n'ayant pas de coup predefini 
void situation4(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]){

}

//situation 5: permet de departager dans le cas d'un simple empilement selons les couleur 
void situation5(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]){
	if(P.cols[Coup.origine].couleur==myColor){
		if(P.cols[Coup.destination].couleur==myColor){ //  M/M
			afecterpoid(rang,listconstpoid,10,ptrpoid);
			printf("S M/M%d->%d ",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
		}
		else{ //	M/L
			afecterpoid(rang,listconstpoid,13,ptrpoid);
			printf("S M/L%d->%d ",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
		}	
	}
	else{
		if(P.cols[Coup.destination].couleur==myColor){ //	L/M
			afecterpoid(rang,listconstpoid,11,ptrpoid);
			printf("S L/M%d->%d ",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
		}
		else{ //	L/L
			afecterpoid(rang,listconstpoid,12,ptrpoid);
			printf("S L/L%d->%d ",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
		}
	}
}

//permet simplement d'affecter un poid donné à un coup donné selon le rang de l'analyse
void afecterpoid(int rang,double listconstpoid[][3],int indicesituation,double *ptrpoid){
	if(rang==1){*ptrpoid=*ptrpoid+listconstpoid[indicesituation][rang-1];}
	if(rang==2){*ptrpoid=*ptrpoid+listconstpoid[indicesituation][rang-1];}
	if(rang==3&&listconstpoid[indicesituation][2]!=0){*ptrpoid=*ptrpoid+listconstpoid[indicesituation][0]/listconstpoid[indicesituation][2];}
	if(rang==4&&listconstpoid[indicesituation][2]!=0){*ptrpoid=*ptrpoid+listconstpoid[indicesituation][1]/(listconstpoid[indicesituation][2]);}
	//if(rang<=2)(*ptrpoid) += listconstpoid[indicesituation][2-(rang%2)-1];//dans les deux premier rang on affecte directement
	//else (*ptrpoid) += (listconstpoid[indicesituation][2-(rang%2)-1])/((((rang-rang%2)/2)-1)*listconstpoid[indicesituation][3]);//dans les rang secondaire on reprend les rangs d'origine en %2 que l'on divise par un constante suivant le rang , les rang eloignés on moins d'importance tactique.
}
//retourne le rang d'arret optimum selons le temps aloué , le nombre de coups et le nombre de coups analysé au rang 1
int rangoptimum(int nbcoup,int nbcoutanaly,int const tempsaloue){
	int rang = 0;
	double temps =0;//initialisé ici le temps de rang 1 selon le modele mathématique
	do {
		rang++;
		temps=0;//exprimé ici la fonction de suite du modéle mathematique 
	} while (temps<=tempsaloue);
	return rang-1;
}
void situation6(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3],T_ListeCoups listeCoups){
T_Voisins v;
v=getVoisins(Coup.origine);
int i ,j;
int testun;
int testdeux;
for(i=0;i<v.nb;i++){
	testun=0;
	testdeux=0;
	if(v.cases[i]!=Coup.destination){
		
		for(j=0;j<listeCoups.nb; j++) {
			if(listeCoups.coups[j].destination==v.cases[i]){
				if(listeCoups.coups[j].origine==Coup.origine){testun=1;}
				else{
					if(P.cols[listeCoups.coups[j].origine].couleur==P.cols[listeCoups.coups[j].destination].couleur&&(P.cols[listeCoups.coups[j].origine].nb +P.cols[listeCoups.coups[j].destination].nb==5)){}
					else{testdeux=1;}
				}
			}
		}
		if(testun==1&&testdeux==0){
			if(P.cols[v.cases[i]].couleur==myColor){afecterpoid(rang,listconstpoid,15,ptrpoid);
			printf("P iso m %d->%d",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
			}
			else{afecterpoid(rang,listconstpoid,16,ptrpoid);
			printf("P iso l %d->%d",Coup.origine,Coup.destination);
			printf("poid:%lf ",*ptrpoid);
			}
		}	
	}
}
}
void situation7(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3],T_ListeCoups listeCoups){
	int j;
	T_Coup coupbis;
	for(j=0;j<listeCoups.nb; j++){
		coupbis.origine=listeCoups.coups[j].origine;
		coupbis.destination=listeCoups.coups[j].destination;
		if(listeCoups.coups[j].destination==Coup.destination||listeCoups.coups[j].origine==Coup.destination){
			situation1contre(P, myColor,coupbis,rang,ptrpoid,listconstpoid);
		}
	}
}

void situation1contre(T_Position P,octet myColor,T_Coup Coup,int rang,double *ptrpoid,double listconstpoid[][3]){
	if(voisinsInferieurs(P,myColor,Coup)==1){
		if(P.cols[Coup.origine].couleur==myColor){
		
			if(P.cols[Coup.destination].couleur==myColor){ //  M/M
				afecterpoid(rang,listconstpoid,17,ptrpoid);
				printf("EPd M/M%d->%d ",Coup.origine,Coup.destination);
				printf("poid:%lf ",*ptrpoid);
					
			}
			else{ //	M/L

				afecterpoid(rang,listconstpoid,18,ptrpoid);
				printf("EPd M/L%d->%d ",Coup.origine,Coup.destination);
				printf("poid:%lf ",*ptrpoid);
			}	
		}
		else{
			if(P.cols[Coup.destination].couleur==myColor){ //	L/M
				afecterpoid(rang,listconstpoid,19,ptrpoid);
				printf("EPd L/M%d->%d ",Coup.origine,Coup.destination);
				printf("poid:%lf ",*ptrpoid);
			}
			else{ //	L/L
				afecterpoid(rang,listconstpoid,20,ptrpoid);
				printf("EPd L/L%d->%d ",Coup.origine,Coup.destination);
				printf("poid:%lf ",*ptrpoid);
			}
		}
	}
}
