#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _SET_SYMB_AVALAM_
	#define _SET_SYMB_AVALAM_ "uUdDtTqQcC123456789"
#endif

int main(int argc, char ** argv) {
	char * graine;
	char * ensemble = _SET_SYMB_AVALAM_; 
	int i, tE, tG;  

	if (argc < 2) {
		fprintf(stderr,"alea <strGraine> [<strEnsemble>]\n");
		return 1; 
	}

	graine = argv[1];
	tG = strlen(graine); 
	
	if (argc == 3) ensemble = argv[2];
	tE = strlen(ensemble); 
	

	fprintf(stderr,"Graine : %s (%d car.)\n", graine, tG);
	fprintf(stderr,"Ensemble : %s (%d car.)\n", ensemble, tE); 

	for(i=0;i<tG;i++) {
		fprintf(stderr,"graine[%d]=%d, graine[%d]%%%d = %d\n", i, graine[i],i,tE, graine[i]%tE);
		printf("%c", ensemble[graine[i]%tE]); 
	}

	return 0;
}
