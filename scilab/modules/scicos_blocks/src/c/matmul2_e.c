#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>
extern int sciprint();
void matmul2_e(scicos_block *block,int flag)
{
 if (flag==1){
  int nu,mu,i,ut;
  double v,*rpar;
  ut=GetInType(block,1);
  mu=GetOutPortRows(block,1);
  nu=GetOutPortCols(block,1);
  rpar=GetRparPtrs(block);
  switch (ut)
  {
      case SCSINT32_N :{
           long *u1,*u2,*y1;
           u1=Getint32InPortPtrs(block,1);
           u2=Getint32InPortPtrs(block,2);
           y1=Getint32OutPortPtrs(block,1);
           for (i=0;i<mu*nu;i++)
	     {v=(double)u1[i]*(double)u2[i];
	       if ((v<rpar[0])|(v>rpar[1])) 
		 {sciprint("overflow error");
		   set_block_error(-4);
		   return;}
	       else y1[i]=(long)v;}
           break;}

      case SCSINT16_N :{
           short *u1,*u2,*y1;
           u1=Getint16InPortPtrs(block,1);
           u2=Getint16InPortPtrs(block,2);
           y1=Getint16OutPortPtrs(block,1);
           for (i=0;i<mu*nu;i++)
	     {v=(double)u1[i]*(double)u2[i];
	       if ((v<rpar[0])|(v>rpar[1])) 
		 {sciprint("overflow error");
		   set_block_error(-4);
		   return;}
	       else  y1[i]=(short)v;}
           break;}

      case SCSINT8_N :{
           char *u1,*u2,*y1;
           u1=Getint8InPortPtrs(block,1);
           u2=Getint8InPortPtrs(block,2);
           y1=Getint8OutPortPtrs(block,1);
           for (i=0;i<mu*nu;i++)
	     {v=(double)u1[i]*(double)u2[i];
	       if ((v<rpar[0])|(v>rpar[1])) 
		 {sciprint("overflow error");
		   set_block_error(-4);
		   return;}
	       else y1[i]=(char)v;}
           break;}

      case SCSUINT32_N :{
           unsigned long *u1,*u2,*y1;
           u1=Getuint32InPortPtrs(block,1);
           u2=Getuint32InPortPtrs(block,2);
           y1=Getuint32OutPortPtrs(block,1);
           for (i=0;i<mu*nu;i++)
	     {v=(double)u1[i]*(double)u2[i];
	       if ((v<rpar[0])|(v>rpar[1])) 
		 {sciprint("overflow error");
		   set_block_error(-4);
		   return;}
	       else y1[i]=(unsigned long)v;}
           break;}

      case SCSUINT16_N :{
           unsigned short *u1,*u2,*y1;
           u1=Getuint16InPortPtrs(block,1);
           u2=Getuint16InPortPtrs(block,2);
           y1=Getuint16OutPortPtrs(block,1);
           for (i=0;i<mu*nu;i++)
	     {v=(double)u1[i]*(double)u2[i];
	       if ((v<rpar[0])|(v>rpar[1])) 
		 {sciprint("overflow error");
		   set_block_error(-4);
		   return;}
	       else y1[i]=(unsigned short)v;}
           break;}

      case SCSUINT8_N :{
           unsigned char *u1,*u2,*y1;
           u1=Getuint8InPortPtrs(block,1);
           u2=Getuint8InPortPtrs(block,2);
           y1=Getuint8OutPortPtrs(block,1);
           for (i=0;i<mu*nu;i++)
	     {v=(double)u1[i]*(double)u2[i];
	       if ((v<rpar[0])|(v>rpar[1])) 
		 {sciprint("overflow error");
		   set_block_error(-4);
		   return;}
	       else y1[i]=(unsigned char)v;}
           break;}

      default :{
          set_block_error(-4); 
	  return;} 
   } 
  }
}
