# include "scicos_block4.h"
# include "../machine.h"
# include <math.h>
# include <memory.h>
extern int sciprint();

void convert(scicos_block *block,int flag)
{
 int m,n,i;
 int *ipar;
 double v,w,k;
 
 m=GetInPortRows(block,1);
 n=GetInPortCols(block,1);
 ipar=GetIparPtrs(block);
 
 if ((flag==1)|(flag==6))
    {
     switch (*ipar){
	    case 1:{ 
		   void *u,*y;
		   int so;
		   so=GetSizeOfOut(block,1);
     	  	   u=GetInPortPtrs(block,1);
     	  	   y=GetOutPortPtrs(block,1);
     	  	   memcpy(y,u,m*n*so);
     	  	   break;}
	    case 2:{
		   double *u;
		   long *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if (fabs(w)>k/2-1)
			     {if (w>=0) w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     else w=-(-(k/2)+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(long)w;}
		   break;}
	    case 3:{
		   double *u;
		   short *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if (fabs(w)>k/2-1)
			     {if (w>=0) w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     else w=-(-(k/2)+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(short)w;}
		   break;}
	    case 4:{
		   double *u;
		   char *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if (fabs(w)>k/2-1)
			     {if (w>=0) w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     else w=-(-(k/2)+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(char)w;}
		   break;}
	    case 5:{
		   double *u;
		   unsigned long *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned long)w;}
		   break;}
	    case 6:{
		   double *u;
		   unsigned short *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned short)w;}
		   break;}
	    case 7:{
		   double *u;
		   unsigned char *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 8:{
		   long *u;
		   double *y;
		   u=Getint32InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 9:{
		   long *u;
		   short *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if (fabs(w)>k/2-1)
			     {if (w>=0) w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     else w=-(-(k/2)+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(short)w;}
		   break;}
	    case 10:{
		   long *u;
		   char *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if (fabs(w)>k/2-1)
			     {if (w>=0) w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     else w=-(-(k/2)+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(char)w;}
		   break;}
	    case 11:{
		   long *u;
		   unsigned short *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned short)w;}
		   break;}
	    case 12:{
		   long *u;
		   unsigned char *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 13:{
		   short *u;
		   double *y;
		   u=Getint16InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 14:{
		   short *u;
		   long *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(long)u[i];}
		   break;}
	    case 15:{
		   short *u;
		   char *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if (fabs(w)>k/2-1)
			     {if (w>=0) w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     else w=-(-(k/2)+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(char)w;}
		   break;}
	    case 16:{
		   short *u;
		   unsigned long *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned long)u[i];}
		   break;}
	    case 17:{
		   short *u;
		   unsigned char *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 18:{
		   char *u;
		   double *y;
		   u=Getint8InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 19:{
		   char *u;
		   long *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(long)u[i];}
		   break;}
	    case 20:{
		   char *u;
		   short *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(short)u[i];}
		   break;}
	    case 21:{
		   char *u;
		   unsigned long *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned long)u[i];}
		   break;}
	    case 22:{
		   char *u;
		   unsigned short *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned short)u[i];}
		   break;}
	    case 23:{
		   unsigned long *u;
		   double *y;
		   u=Getuint32InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 24:{
		   unsigned long *u;
		   short *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if ((w)>k/2-1)
			     { w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(short)w;}
		   break;}
	    case 25:{
		   unsigned long *u;
		   char *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if ((w)>k/2-1)
			     {w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(char)w;}
		   break;}
	    case 26:{
		   unsigned long *u;
		   unsigned short *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned short)w;}
		   break;}
	    case 27:{
		   unsigned long *u;
		   unsigned char *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 28:{
		   unsigned short *u;
		   double *y;
		   u=Getuint16InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 29:{
		   unsigned short *u;
		   long *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(long)u[i];}
		   break;}
	    case 30:{
		   unsigned short *u;
		   char *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         if (w>k/2-1)
			     {w=(-k/2+fabs(w-(double)((int)(w/(k/2)))*(k/2)));
			     }
		         y[i]=(char)w;}
		   break;}
	    case 31:{
		   unsigned short *u;
		   unsigned long *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned long)u[i];}
		   break;}
	    case 32:{
		   unsigned short *u;
		   unsigned char *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 33:{
		   unsigned char *u;
		   double *y;
		   u=Getuint8InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 34:{
		   unsigned char *u;
		   long *y;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(long)u[i];}
		   break;}
	    case 35:{
		   unsigned char *u;
		   short *y;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(short)u[i];}
		   break;}
	    case 36:{
		   unsigned char *u;
		   unsigned long *y;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned long)u[i];}
		   break;}
	    case 37:{
		   unsigned char *u;
		   unsigned short *y;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned short)u[i];}
		   break;}
	    case 38:{
		   double *u;
		   long *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++)
		       {if (u[i]>k/2-1)
		           {y[i]=(long)(k/2-1);}
		        else if (u[i]<-(k/2))
			   {y[i]=-(long)(k/2);}
		        else {y[i]=(long)(u[i]);}
		       }
		   break;}
	    case 39:{
		   double *u;
		   short *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>k/2-1)
		           {y[i]=(short)(k/2-1);}
		        else if (u[i]<-(k/2))
			   {y[i]=-(short)(k/2);}
		        else {y[i]=(short)(u[i]);}
		       }
		   break;}
	    case 40:{
		   double *u;
		   char *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>k/2-1)
		           {y[i]=(char)(k/2-1);}
		        else if (u[i]<-(k/2))
			   {y[i]=-(char)(k/2);}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 41:{
		   double *u;
		   unsigned long *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>=k)
		       	     {y[i]=(unsigned long)(k-1);}
		   	else if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned long)(u[i]);}
		   	}
		   break;}
	    case 42:{
		   double *u;
		   unsigned short *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>=k)
		       	     {y[i]=(unsigned short)(k-1);}
		   	else if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 43:{
		   double *u;
		   unsigned char *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>=k)
		       	     {y[i]=(unsigned char)(k-1);}
		   	else if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 44:{
		   long *u;
		   short *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(long)(k/2-1))
		           {y[i]=(short)(k/2-1);}
		        else if (u[i]<-(long)(k/2))
			   {y[i]=-(short)(k/2);}
		        else {y[i]=(short)(u[i]);}
		       }
		   break;}
	    case 45:{
		   long *u;
		   char *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(long)(k/2-1))
		           {y[i]=(char)(k/2-1);}
		        else if (u[i]<-(long)(k/2))
			   {y[i]=-(char)(k/2);}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 46:{
		   long *u;
		   unsigned long *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned long)(u[i]);}
		   	}
		   break;}
	    case 47:{
		   long *u;
		   unsigned short *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>=(long)(k))
		       	     {y[i]=(unsigned short)(k-1);}
		   	else if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 48:{
		   long *u;
		   unsigned char *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>=(long)k)
		       	     {y[i]=(unsigned char)(k-1);}
		   	else if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 49:{
		   short *u;
		   char *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(short)(k/2-1))
		           {y[i]=(char)(k/2-1);}
		        else if (u[i]<-(short)(k/2))
			   {y[i]=-(char)(k/2);}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 50:{
		   short *u;
		   unsigned long *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]<0)  y[i]=0;
			 else y[i]=(unsigned long)u[i];}
		   break;}
	    case 51:{
		   short *u;
		   unsigned short *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 52:{
		   short *u;
		   unsigned char *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>=(short)k)
		       	     {y[i]=(unsigned char)(k-1);}
		   	else if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 53:{
		   char *u;
		   unsigned long *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
			{if (u[i]<0)  y[i]=0;
			 else y[i]=(unsigned long)u[i];}
		   break;}
	    case 54:{
		   char *u;
		   unsigned short *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 55:{
		   char *u;
		   unsigned char *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 56:{
		   long *y;
		   unsigned long *u;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]>(unsigned long)(k/2-1))
			     {y[i]=(long)(k/2-1);}
		   	else {y[i]=(long)(u[i]);}
		   	}
		   break;}
	    case 57:{
		   unsigned long *u;
		   short *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(unsigned long)(k/2-1))
			     {y[i]=(short)(k/2-1);}
		   	else {y[i]=(short)(u[i]);}
		   	}
		   break;}
	    case 58:{
		   unsigned long *u;
		   char *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(unsigned long)(k/2-1))
			     {y[i]=(char)(k/2-1);}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	    case 59:{
		   unsigned long *u;
		   unsigned short *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(unsigned long)(k/2-1))
			     {y[i]=(unsigned short)(k/2-1);}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 60:{
		   unsigned long *u;
		   unsigned char *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(unsigned long)(k/2-1))
			     {y[i]=(unsigned char)(k/2-1);}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 61:{
		   unsigned short *u;
		   short *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(unsigned short)(k/2-1))
			     {y[i]=(short)(k/2-1);}
		   	else {y[i]=(short)(u[i]);}
		   	}
		   break;}
	    case 62:{
		   unsigned short *u;
		   char *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(unsigned short)(k/2-1))
			     {y[i]=(char)(k/2-1);}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	    case 63:{
		   unsigned short *u;
		   unsigned char *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(unsigned short)(k/2-1))
			     {y[i]=(unsigned char)k/2-1;}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 64:{
		   unsigned char *u;
		   char *y;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(unsigned char)(k/2-1))
			     {y[i]=(char)(k/2-1);}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	    case 65:{
		   double *u;
		   long *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++)
		   {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		           {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(long)(u[i]);}
		       }
		   break;}
	    case 66:{
		   double *u;
		   short *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		           {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(short)(u[i]);}
		       }
		   break;}
	    case 67:{
		   double *u;
		   char *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		           {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 68:{
		   double *u;
		   unsigned long *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		           {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned long)(u[i]);}
		   	}
		   break;}
	    case 69:{
		   double *u;
		   unsigned short *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 70:{
		   double *u;
		   unsigned char *y;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 71:{
		   long *u;
		   short *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		            {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(short)(u[i]);}
		       }
		   break;}
	    case 72:{
		   long *u;
		   char *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		            {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 73:{
		   long *u;
		   unsigned long *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned long)(u[i]);}
		   	}
		   break;}
	    case 74:{
		   long *u;
		   unsigned short *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 75:{
		   long *u;
		   unsigned char *y;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 76:{
		   short *u;
		   char *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 77:{
		   short *u;
		   unsigned long *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]<0)  
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
			 else y[i]=(unsigned long)u[i];}
		   break;}
	    case 78:{
		   short *u;
		   unsigned short *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			    {if (flag==1) 
                                {sciprint("overflow error");
			         set_block_error(-4);
			         return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 79:{
		   short *u;
		   unsigned char *y;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (((u[i]>=k) | (u[i]<0))& (flag==1))
		       	    {if (flag==1) 
                                 {sciprint("overflow error");
			          set_block_error(-4);
			          return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 80:{
		   char *u;
		   unsigned long *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
			{if (u[i]<0)
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
			 else y[i]=(unsigned long)u[i];}
		   break;}
	    case 81:{
		   char *u;
		   unsigned short *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]<0)
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 82:{
		   char *u;
		   unsigned char *y;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 83:{
		   long *y;
		   unsigned long *u;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(long)(u[i]);}
		   	}
		   break;}
	    case 84:{
		   unsigned long *u;
		   short *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(short)(u[i]);}
		   	}
		   break;}
	    case 85:{
		   unsigned long *u;
		   char *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	    case 86:{
		   unsigned long *u;
		   unsigned short *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 87:{
		   unsigned long *u;
		   unsigned char *y;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 88:{
		   unsigned short *u;
		   short *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(short)(u[i]);}
		   	}
		   break;}
	    case 89:{
		   unsigned short *u;
		   char *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	    case 90:{
		   unsigned short *u;
		   unsigned char *y;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 91:{
		   unsigned char *u;
		   char *y;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint("overflow error");
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	   }
	}
 }
