/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
#include <math.h>
#include <memory.h>
#include "scicos.h"
#include "scicos_block4.h"
#include "sciprint.h"
#include "localization.h"
#include "MALLOC.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP void convert(scicos_block *block,int flag)
{
 int m = 0,n = 0,i = 0;
 int *ipar = NULL;
 double v = 0.,w = 0.,k = 0.;
 
 m=GetInPortRows(block,1);
 n=GetInPortCols(block,1);
 ipar=GetIparPtrs(block);
 
 if ((flag==1)|(flag==6))
    {
     switch (*ipar){
	    case 1:{ 
		   void *u = NULL,*y = NULL;
		   int so;
		   so=GetSizeOfOut(block,1);
     	   u=GetInPortPtrs(block,1);
     	   y=GetOutPortPtrs(block,1);
     	   memcpy(y,u,m*n*so);
     	  break;
			   }
	    case 2:{
		   double *u = NULL;
		   long *y = NULL;
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
		   double *u = NULL;
		   short *y = NULL;
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
		   double *u = NULL;
		   char *y = NULL;
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
		   double *u = NULL;
		   unsigned long *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned long)w;}
		   break;}
	    case 6:{
		   double *u = NULL;
		   unsigned short *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned short)w;}
		   break;}
	    case 7:{
		   double *u = NULL;
		   unsigned char *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 8:{
		   long *u = NULL;
		   double *y = NULL;
		   u=Getint32InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 9:{
		   long *u = NULL;
		   short *y = NULL;
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
		   long *u = NULL;
		   char *y = NULL;
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
		   long *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned short)w;}
		   break;}
	    case 12:{
		   long *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 13:{
		   short *u = NULL;
		   double *y = NULL;
		   u=Getint16InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 14:{
		   short *u = NULL;
		   long *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(long)u[i];}
		   break;}
	    case 15:{
		   short *u = NULL;
		   char *y = NULL;
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
		   short *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned long)u[i];}
		   break;}
	    case 17:{
		   short *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 18:{
		   char *u = NULL;
		   double *y = NULL;
		   u=Getint8InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 19:{
		   char *u = NULL;
		   long *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(long)u[i];}
		   break;}
	    case 20:{
		   char *u = NULL;
		   short *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(short)u[i];}
		   break;}
	    case 21:{
		   char *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned long)u[i];}
		   break;}
	    case 22:{
		   char *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned short)u[i];}
		   break;}
	    case 23:{
		   unsigned long *u = NULL;
		   double *y = NULL;
		   u=Getuint32InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 24:{
		   unsigned long *u = NULL;
		   short *y = NULL;
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
		   unsigned long *u = NULL;
		   char *y = NULL;
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
		   unsigned long *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned short)w;}
		   break;}
	    case 27:{
		   unsigned long *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 28:{
		   unsigned short *u = NULL;
		   double *y = NULL;
		   u=Getuint16InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 29:{
		   unsigned short *u = NULL;
		   long *y = NULL;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(long)u[i];}
		   break;}
	    case 30:{
		   unsigned short *u = NULL;
		   char *y = NULL;
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
		   unsigned short *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned long)u[i];}
		   break;}
	    case 32:{
		   unsigned short *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {v=(double)u[i];
		         w=v-(double)((int)(v/k))*k;
		         y[i]=(unsigned char)w;}
		   break;}
	    case 33:{
		   unsigned char *u = NULL;
		   double *y = NULL;
		   u=Getuint8InPortPtrs(block,1);
		   y=GetRealOutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) y[i]=(double) u[i];
		   break;}
	    case 34:{
		   unsigned char *u = NULL;
		   long *y = NULL;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(long)u[i];}
		   break;}
	    case 35:{
		   unsigned char *u = NULL;
		   short *y = NULL;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(short)u[i];}
		   break;}
	    case 36:{
		   unsigned char *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned long)u[i];}
		   break;}
	    case 37:{
		   unsigned char *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {y[i]=(unsigned short)u[i];}
		   break;}
	    case 38:{
		   double *u = NULL;
		   long *y = NULL;
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
		   double *u = NULL;
		   short *y = NULL;
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
		   double *u = NULL;
		   char *y = NULL;
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
		   double *u = NULL;
		   unsigned long *y = NULL;
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
		   double *u = NULL;
		   unsigned short *y = NULL;
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
		   double *u = NULL;
		   unsigned char *y = NULL;
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
		   long *u = NULL;
		   short *y = NULL;
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
		   long *u = NULL;
		   char *y = NULL;
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
		   long *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned long)(u[i]);}
		   	}
		   break;}
	    case 47:{
		   long *u = NULL;
		   unsigned short *y = NULL;
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
		   long *u = NULL;
		   unsigned char *y = NULL;
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
		   short *u = NULL;
		   char *y = NULL;
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
		   short *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]<0)  y[i]=0;
			 else y[i]=(unsigned long)u[i];}
		   break;}
	    case 51:{
		   short *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 52:{
		   short *u = NULL;
		   unsigned char *y = NULL;
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
		   char *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
			{if (u[i]<0)  y[i]=0;
			 else y[i]=(unsigned long)u[i];}
		   break;}
	    case 54:{
		   char *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 55:{
		   char *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			     {y[i]=0;}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 56:{
		   long *y = NULL;
		   unsigned long *u = NULL;
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
		   unsigned long *u = NULL;
		   short *y = NULL;
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
		   unsigned long *u = NULL;
		   char *y = NULL;
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
		   unsigned long *u = NULL;
		   unsigned short *y = NULL;
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
		   unsigned long *u = NULL;
		   unsigned char *y = NULL;
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
		   unsigned short *u = NULL;
		   short *y = NULL;
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
		   unsigned short *u = NULL;
		   char *y = NULL;
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
		   unsigned short *u = NULL;
		   unsigned char *y = NULL;
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
		   unsigned char *u = NULL;
		   char *y = NULL;
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
		   double *u = NULL;
		   long *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++)
		   {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		           {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(long)(u[i]);}
		       }
		   break;}
	    case 66:{
		   double *u = NULL;
		   short *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		           {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(short)(u[i]);}
		       }
		   break;}
	    case 67:{
		   double *u = NULL;
		   char *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		           {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 68:{
		   double *u = NULL;
		   unsigned long *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		           {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned long)(u[i]);}
		   	}
		   break;}
	    case 69:{
		   double *u = NULL;
		   unsigned short *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 70:{
		   double *u = NULL;
		   unsigned char *y = NULL;
	  	   u=GetRealInPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 71:{
		   long *u = NULL;
		   short *y = NULL;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		            {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(short)(u[i]);}
		       }
		   break;}
	    case 72:{
		   long *u = NULL;
		   char *y = NULL;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		            {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 73:{
		   long *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned long)(u[i]);}
		   	}
		   break;}
	    case 74:{
		   long *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 75:{
		   long *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>=k) | (u[i]<0))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 76:{
		   short *u = NULL;
		   char *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if ((u[i]>k/2-1) | (u[i]<-(k/2)))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		        else {y[i]=(char)(u[i]);}
		       }
		   break;}
	    case 77:{
		   short *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]<0)  
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
			 else y[i]=(unsigned long)u[i];}
		   break;}
	    case 78:{
		   short *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
			    {if (flag==1) 
                                {sciprint(_("overflow error"));
			         set_block_error(-4);
			         return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 79:{
		   short *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (((u[i]>=k) | (u[i]<0))& (flag==1))
		       	    {if (flag==1) 
                                 {sciprint(_("overflow error"));
			          set_block_error(-4);
			          return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 80:{
		   char *u = NULL;
		   unsigned long *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint32OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
			{if (u[i]<0)
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
			 else y[i]=(unsigned long)u[i];}
		   break;}
	    case 81:{
		   char *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint16OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]<0)
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 82:{
		   char *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getint8InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]<0)
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 83:{
		   long *y = NULL;
		   unsigned long *u = NULL;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint32OutPortPtrs(block,1);
		   k=pow(2,32);
		   for (i=0;i<m*n;i++) 
		   	{if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(long)(u[i]);}
		   	}
		   break;}
	    case 84:{
		   unsigned long *u = NULL;
		   short *y = NULL;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(short)(u[i]);}
		   	}
		   break;}
	    case 85:{
		   unsigned long *u = NULL;
		   char *y = NULL;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	    case 86:{
		   unsigned long *u = NULL;
		   unsigned short *y = NULL;
	  	   u=Getuint32InPortPtrs(block,1);
	       y=Getuint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned short)(u[i]);}
		   	}
		   break;}
	    case 87:{
		   unsigned long *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getuint32InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 88:{
		   unsigned short *u = NULL;
		   short *y = NULL;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint16OutPortPtrs(block,1);
		   k=pow(2,16);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(short)(u[i]);}
		   	}
		   break;}
	    case 89:{
		   unsigned short *u = NULL;
		   char *y = NULL;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	    case 90:{
		   unsigned short *u = NULL;
		   unsigned char *y = NULL;
	  	   u=Getuint16InPortPtrs(block,1);
	           y=Getuint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(unsigned char)(u[i]);}
		   	}
		   break;}
	    case 91:{
		   unsigned char *u = NULL;
		   char *y = NULL;
	  	   u=Getuint8InPortPtrs(block,1);
	           y=Getint8OutPortPtrs(block,1);
		   k=pow(2,8);
		   for (i=0;i<m*n;i++) 
		        {if (u[i]>(k/2-1))
		       	    {if (flag==1)
                               {sciprint(_("overflow error"));
			        set_block_error(-4);
			        return;}}
		   	else {y[i]=(char)(u[i]);}
		   	}
		   break;}
	   }
	}
 }
/*--------------------------------------------------------------------------*/ 
