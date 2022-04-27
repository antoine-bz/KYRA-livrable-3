/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

int Pilevoisins(int d)
{
	int dest=0;
	dest=d;
	int pile1,pile2,pile3,pile4,pile5=0,pile6=0,pile7=0,pile8=0;
	int somme;
	T_Position currentPosition;

	if( dest==2 )
	{
		pile1=currentPosition.cols[d-2].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d+5].nb;
		pile4=currentPosition.cols[d+4].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if( dest==0)
	{
		pile1=currentPosition.cols[d+1].nb;
		pile2=currentPosition.cols[d+2].nb;
		pile3=currentPosition.cols[d+3].nb;
		pile4=currentPosition.cols[d+4].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if( dest==47)
	{
		pile1=currentPosition.cols[d-1].nb;
		pile2=currentPosition.cols[d-2].nb;
		pile3=currentPosition.cols[d-3].nb;
		pile4=currentPosition.cols[d-4].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

		if( dest==45 )
	{
		pile1=currentPosition.cols[d+2].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d-5].nb;
		pile4=currentPosition.cols[d-4].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

		if( dest==1 )
	{
		pile1=currentPosition.cols[d-1].nb;
		pile2=currentPosition.cols[d+2].nb;
		pile3=currentPosition.cols[d+3].nb;
		pile4=currentPosition.cols[d+4].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

		if( dest==46 )
	{
		pile1=currentPosition.cols[d+1].nb;
		pile2=currentPosition.cols[d-2].nb;
		pile3=currentPosition.cols[d-3].nb;
		pile4=currentPosition.cols[d-4].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if( dest==3)
	{
		pile1=currentPosition.cols[d-3].nb;
		pile2=currentPosition.cols[d-2].nb;
		pile3=currentPosition.cols[d-1].nb;
		pile4=currentPosition.cols[d+1].nb;
		pile5=currentPosition.cols[d+3].nb;
		pile6=currentPosition.cols[d+4].nb;
		pile7=currentPosition.cols[d+5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5 && (currentPosition.cols[d].nb+pile2+1)>=5 && (currentPosition.cols[d].nb+pile3+1)>=5 && (currentPosition.cols[d].nb+pile4+1)>=5 && (currentPosition.cols[d].nb+pile5+1)>=5 && (currentPosition.cols[d].nb+pile6+1)>=5 && (currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==44)
	{
		pile1=currentPosition.cols[d+3].nb;
		pile2=currentPosition.cols[d+2].nb;
		pile3=currentPosition.cols[d+1].nb;
		pile4=currentPosition.cols[d-1].nb;
		pile5=currentPosition.cols[d-3].nb;
		pile6=currentPosition.cols[d-4].nb;
		pile7=currentPosition.cols[d-5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5 && (currentPosition.cols[d].nb+pile2+1)>=5 && (currentPosition.cols[d].nb+pile3+1)>=5 && (currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==4)
	{
		pile1=currentPosition.cols[d+3].nb;
		pile2=currentPosition.cols[d+4].nb;
		pile3=currentPosition.cols[d+1].nb;
		pile4=currentPosition.cols[d-1].nb;
		pile5=currentPosition.cols[d-3].nb;
		pile6=currentPosition.cols[d-4].nb;
		pile7=currentPosition.cols[d+5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==43)
	{
		pile1=currentPosition.cols[d-3].nb;
		pile2=currentPosition.cols[d-4].nb;
		pile3=currentPosition.cols[d-1].nb;
		pile4=currentPosition.cols[d+1].nb;
		pile5=currentPosition.cols[d+3].nb;
		pile6=currentPosition.cols[d+4].nb;
		pile7=currentPosition.cols[d-5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==9)
	{
		pile1=currentPosition.cols[d+6].nb;
		pile2=currentPosition.cols[d+7].nb;
		pile3=currentPosition.cols[d+1].nb;
		pile4=currentPosition.cols[d-1].nb;
		pile5=currentPosition.cols[d-4].nb;
		pile6=currentPosition.cols[d-5].nb;
		pile7=currentPosition.cols[d+5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==38)
	{
		pile1=currentPosition.cols[d-6].nb;
		pile2=currentPosition.cols[d-7].nb;
		pile3=currentPosition.cols[d-1].nb;
		pile4=currentPosition.cols[d+1].nb;
		pile5=currentPosition.cols[d+4].nb;
		pile6=currentPosition.cols[d+5].nb;
		pile7=currentPosition.cols[d-5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==14 || dest==15 || dest==16)
	{
		pile1=currentPosition.cols[d-6].nb;
		pile2=currentPosition.cols[d-7].nb;
		pile3=currentPosition.cols[d-1].nb;
		pile4=currentPosition.cols[d+1].nb;
		pile5=currentPosition.cols[d+8].nb;
		pile6=currentPosition.cols[d+9].nb;
		pile7=currentPosition.cols[d-5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==31 || dest==32 || dest==33)
	{
		pile1=currentPosition.cols[d+6].nb;
		pile2=currentPosition.cols[d+7].nb;
		pile3=currentPosition.cols[d+1].nb;
		pile4=currentPosition.cols[d-1].nb;
		pile5=currentPosition.cols[d-8].nb;
		pile6=currentPosition.cols[d-9].nb;
		pile7=currentPosition.cols[d+5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==17)
	{
		pile1=currentPosition.cols[d-6].nb;
		pile2=currentPosition.cols[d-7].nb;
		pile3=currentPosition.cols[d-1].nb;
		pile4=currentPosition.cols[d+1].nb;
		pile5=currentPosition.cols[d+8].nb;
		pile6=currentPosition.cols[d+9].nb;
		pile7=currentPosition.cols[d+7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==30)
	{
		pile1=currentPosition.cols[d+6].nb;
		pile2=currentPosition.cols[d+7].nb;
		pile3=currentPosition.cols[d+1].nb;
		pile4=currentPosition.cols[d-1].nb;
		pile5=currentPosition.cols[d-8].nb;
		pile6=currentPosition.cols[d-9].nb;
		pile7=currentPosition.cols[d-7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==21)
	{
		pile1=currentPosition.cols[d-9].nb;
		pile2=currentPosition.cols[d-8].nb;
		pile3=currentPosition.cols[d-1].nb;
		pile4=currentPosition.cols[d+1].nb;
		pile5=currentPosition.cols[d+8].nb;
		pile6=currentPosition.cols[d+9].nb;
		pile7=currentPosition.cols[d+7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==26)
	{
		pile1=currentPosition.cols[d+9].nb;
		pile2=currentPosition.cols[d+8].nb;
		pile3=currentPosition.cols[d+1].nb;
		pile4=currentPosition.cols[d-1].nb;
		pile5=currentPosition.cols[d-8].nb;
		pile6=currentPosition.cols[d-9].nb;
		pile7=currentPosition.cols[d-7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==23)
	{
		pile1=currentPosition.cols[d-9].nb;
		pile2=currentPosition.cols[d-10].nb;
		pile3=currentPosition.cols[d-1].nb;
		pile4=currentPosition.cols[d+1].nb;
		pile5=currentPosition.cols[d+8].nb;
		pile6=currentPosition.cols[d+9].nb;
		pile7=currentPosition.cols[d+7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==24)
	{
		pile1=currentPosition.cols[d+9].nb;
		pile2=currentPosition.cols[d+10].nb;
		pile3=currentPosition.cols[d+1].nb;
		pile4=currentPosition.cols[d-1].nb;
		pile5=currentPosition.cols[d-8].nb;
		pile6=currentPosition.cols[d-9].nb;
		pile7=currentPosition.cols[d-7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==5)
	{
		pile1=currentPosition.cols[d-4].nb;
		pile2=currentPosition.cols[d+3].nb;
		pile3=currentPosition.cols[d-1].nb;
		pile4=currentPosition.cols[d+4].nb;
		pile5=currentPosition.cols[d+5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==42)
	{
		pile1=currentPosition.cols[d+4].nb;
		pile2=currentPosition.cols[d-3].nb;
		pile3=currentPosition.cols[d+1].nb;
		pile4=currentPosition.cols[d-4].nb;
		pile5=currentPosition.cols[d-5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

		if(dest==6)
	{
		pile1=currentPosition.cols[d-4].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d-3].nb;
		pile4=currentPosition.cols[d+6].nb;
		pile5=currentPosition.cols[d+7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==41)
	{
		pile1=currentPosition.cols[d+4].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d+3].nb;
		pile4=currentPosition.cols[d-6].nb;
		pile5=currentPosition.cols[d-7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==7 || dest==8)
	{
		pile1=currentPosition.cols[d+1].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d-3].nb;
		pile4=currentPosition.cols[d-4].nb;
		pile5=currentPosition.cols[d-5].nb;
		pile6=currentPosition.cols[d+7].nb;
		pile7=currentPosition.cols[d+6].nb;
		pile8=currentPosition.cols[d+5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5&&(currentPosition.cols[d].nb+pile8+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==39 || dest==40)
	{
		pile1=currentPosition.cols[d-1].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d+3].nb;
		pile4=currentPosition.cols[d+4].nb;
		pile5=currentPosition.cols[d+5].nb;
		pile6=currentPosition.cols[d-7].nb;
		pile7=currentPosition.cols[d-6].nb;
		pile8=currentPosition.cols[d-5].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5&&(currentPosition.cols[d].nb+pile8+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==13)
	{
		pile1=currentPosition.cols[d+1].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d-6].nb;
		pile4=currentPosition.cols[d-7].nb;
		pile5=currentPosition.cols[d-5].nb;
		pile6=currentPosition.cols[d+7].nb;
		pile7=currentPosition.cols[d+8].nb;
		pile8=currentPosition.cols[d+9].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5&&(currentPosition.cols[d].nb+pile8+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==34)
	{
		pile1=currentPosition.cols[d-1].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d+6].nb;
		pile4=currentPosition.cols[d+7].nb;
		pile5=currentPosition.cols[d+5].nb;
		pile6=currentPosition.cols[d-7].nb;
		pile7=currentPosition.cols[d-8].nb;
		pile8=currentPosition.cols[d-9].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5&&(currentPosition.cols[d].nb+pile8+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==22)
	{
		pile1=currentPosition.cols[d+1].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d-8].nb;
		pile4=currentPosition.cols[d-9].nb;
		pile5=currentPosition.cols[d-10].nb;
		pile6=currentPosition.cols[d+7].nb;
		pile7=currentPosition.cols[d+8].nb;
		pile8=currentPosition.cols[d+9].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5&&(currentPosition.cols[d].nb+pile8+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==25)
	{
		pile1=currentPosition.cols[d-1].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d+8].nb;
		pile4=currentPosition.cols[d+9].nb;
		pile5=currentPosition.cols[d+10].nb;
		pile6=currentPosition.cols[d-7].nb;
		pile7=currentPosition.cols[d-8].nb;
		pile8=currentPosition.cols[d-9].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5&&(currentPosition.cols[d].nb+pile7+1)>=5&&(currentPosition.cols[d].nb+pile8+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==10)
	{
		pile1=currentPosition.cols[d+1].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d-5].nb;
		pile4=currentPosition.cols[d+5].nb;
		pile5=currentPosition.cols[d+6].nb;
		pile6=currentPosition.cols[d+7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==37)
	{
		pile1=currentPosition.cols[d-1].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d+5].nb;
		pile4=currentPosition.cols[d-5].nb;
		pile5=currentPosition.cols[d-6].nb;
		pile6=currentPosition.cols[d-7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==12)
	{
		pile1=currentPosition.cols[d+1].nb;
		pile2=currentPosition.cols[d-6].nb;
		pile3=currentPosition.cols[d-5].nb;
		pile4=currentPosition.cols[d+8].nb;
		pile5=currentPosition.cols[d+9].nb;
		pile6=currentPosition.cols[d+10].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==35)
	{
		pile1=currentPosition.cols[d-1].nb;
		pile2=currentPosition.cols[d+6].nb;
		pile3=currentPosition.cols[d+5].nb;
		pile4=currentPosition.cols[d-8].nb;
		pile5=currentPosition.cols[d-9].nb;
		pile6=currentPosition.cols[d-10].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==18)
	{
		pile1=currentPosition.cols[d+1].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d-7].nb;
		pile4=currentPosition.cols[d+8].nb;
		pile5=currentPosition.cols[d+9].nb;
		pile6=currentPosition.cols[d+7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==29)
	{
		pile1=currentPosition.cols[d-1].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d+7].nb;
		pile4=currentPosition.cols[d-8].nb;
		pile5=currentPosition.cols[d-9].nb;
		pile6=currentPosition.cols[d-7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5&&(currentPosition.cols[d].nb+pile5+1)>=5&&(currentPosition.cols[d].nb+pile6+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==11)
	{
		pile1=currentPosition.cols[d+5].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d+6].nb;
		pile4=currentPosition.cols[d+7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==36)
	{
		pile1=currentPosition.cols[d-5].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d-6].nb;
		pile4=currentPosition.cols[d-7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	
	if(dest==20)
	{
		pile1=currentPosition.cols[d-8].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d+8].nb;
		pile4=currentPosition.cols[d+9].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==27)
	{
		pile1=currentPosition.cols[d+8].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d-8].nb;
		pile4=currentPosition.cols[d-9].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5&&(currentPosition.cols[d].nb+pile4+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==28)
	{
		pile1=currentPosition.cols[d-8].nb;
		pile2=currentPosition.cols[d+1].nb;
		pile3=currentPosition.cols[d-7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	if(dest==19)
	{
		pile1=currentPosition.cols[d+8].nb;
		pile2=currentPosition.cols[d-1].nb;
		pile3=currentPosition.cols[d+7].nb;
		if((currentPosition.cols[d].nb+pile1+1)>=5&&(currentPosition.cols[d].nb+pile2+1)>=5&&(currentPosition.cols[d].nb+pile3+1)>=5)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

}


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int i;
	int IndiceCoups=0;
	int BestCoups=0;
	int ValeurCoups=0;
	octet o, d; 
	octet myColor = currentPosition.trait; 
	T_Voisins pionVoisinO = getVoisins(o); //Origine des pions voisins
	T_Voisins pionVoisinD = getVoisins(d); //Destination des pions voisins
        T_Voisins pile1, pile2, pile3, pile4, pile5, pile6, pile7,pile8,pile9;  
	// afficherListeCoups(listeCoups);

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	for(i=0;i<listeCoups.nb; i++) 
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
/**************************************************************************************/
	// Si je peux gagner une colonne, je la prends 
	/*if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].nb == 4) ) 

		{
			printf("On choisit ce coup ! \n"); 
			ecrireIndexCoup(i);
			return; // on quitte la fonction 
		}
	
	if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].nb == 3) && (currentPosition.cols[o].nb == 2) && (currentPosition.cols[d]!= myColor))
		{
			printf("On choisit ce coup ! \n");
			ecrireIndexCoup(i);
		}

	if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].nb == 2) && (currentPosition.cols[o].nb == 3) && (currentPosition.cols[d]!= myColor))
		{
			printf("On choisit ce coup ! \n");
			ecrireIndexCoup(i);
		}

	if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].nb == 1) && (currentPosition.cols[o].nb == 4) && (currentPosition.cols[d]!= myColor))
		{
			printf("On choisit ce coup ! \n");
			ecrireIndexCoup(i);
		}

	if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].nb == 3) && (currentPosition.cols[o].nb == 2) && (currentPosition.cols[d]!= myColor))
		{
			printf("On choisit ce coup ! \n");
			ecrireIndexCoup(i);
		}*/
/**************************************************************************************/

//si on peut faire une pile de 5 let's go
	
	if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].nb+currentPosition.cols[o].nb == 5) && (currentPosition.cols[d].couleur != myColor))
		{
			printf("On choisit ce coup ! \n");
			ValeurCoups=5;
			if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		}
		
/**************************************************************************************/

//isole une pile en déplaçant un pion de notre couleur sur un pion adverse

	if (pionVoisinD.nb==0 && currentPosition.cols[d].couleur !=myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)<=5)
		{
			printf("On choisit ce coup ! \n");
			ValeurCoups=10;
			if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		}

/**************************************************************************************/

// un de nos pions sur un de l'adversaire pour faire une pile de 2 imprenable

	if(Pilevoisins(d)==0 && currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==2)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=8;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

// un de nos pions sur un de l'adversaire pour faire une pile de 3 imprenable

	if(Pilevoisins(d)==0 && currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==3)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=8;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

// un de nos pions sur un de l'adversaire pour faire une pile de 4 imprenable

	if(Pilevoisins(d)==0 && currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==4)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=8;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

// un de nos pions sur un de l'adversaire pour faire une pile de 4

	if(currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==4)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=3;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

// un de nos pions sur un de l'adversaire pour faire une pile de 3

	if(currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==3)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=3;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

// un de nos pions sur un de l'adversaire pour faire une pile de 2

	if(currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==2)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=3;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}


/**************************************************************************************/

// Déplacement de adversaire sur adversaire

	if(currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==3)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=2;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

///1->1 =2	

	if(currentPosition.cols[d].couleur !=myColor && currentPosition.cols[o].couleur !=myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==2)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=4;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}
	
/**************************************************************************************/

///1->2 = 3

	if(currentPosition.cols[d].couleur !=myColor && currentPosition.cols[o].couleur !=myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==3)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=3;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

// on déplace un pion adversaire sur un autre de l'adversaire pour faire une pile de 3 et on regarde s'il y a 2 voisins au pion de destination ainsi avec 2 voisins on a 1/2 d'avoir un de nos pions à proximité

	if(currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur != myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==3 && pionVoisinD.nb<=3 /*ou ionVoisinD.nb==2*/)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=2;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

// on déplace un pion adversaire sur un autre de l'adversaire pour faire une pile de 4 et on regarde s'il y a 2 voisins au pion de destination ainsi avec 2 voisins on a 1/2 d'avoir un de nos pions à proximité

	if(currentPosition.cols[d].couleur != myColor && currentPosition.cols[o].couleur != myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==4 && pionVoisinD.nb==2 && pionVoisinO.nb==1)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=2;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

//isole une pile en déplaçant un pion de notre sur un pion de notre couleur

	if (pionVoisinD.nb==0 && currentPosition.cols[d].couleur == myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)<=5)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=9;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

//faire une pile de 2 en bougeant un de nos pions sur un autres de nos pions

	if(currentPosition.cols[d].couleur == myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==2)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=1;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

//faire une pile de 3 en bougeant un de nos pions sur un autres de nos pions

	if(currentPosition.cols[d].couleur == myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==3)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=1;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}


/**************************************************************************************/

//faire une pile de 4 en bougeant un de nos pions sur un autres de nos pions

	if(currentPosition.cols[d].couleur == myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==4)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=1;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}

/**************************************************************************************/

//faire une pile de 5 en bougeant un de nos pions sur un autres de nos pions

	if(currentPosition.cols[d].couleur == myColor && currentPosition.cols[o].couleur == myColor && (currentPosition.cols[d].nb+currentPosition.cols[o].nb)==5)
	{
		printf("On choisit ce coup ! \n");
		ValeurCoups=1;
		if(ValeurCoups>BestCoups)
			{
				IndiceCoups=i;
				BestCoups=ValeurCoups;
			}
		
	}
/**************************************************************************************/



	}//FIN DU FOR

	ecrireIndexCoup(IndiceCoups);
}	//FIN DU PROGRAMME


