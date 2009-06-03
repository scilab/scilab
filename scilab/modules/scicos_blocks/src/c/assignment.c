#include "scicos_block4.h"
#include <stdio.h>
#include <memory.h>

void assignment(scicos_block *block,int flag)
{
  int my,ny,so;
  double *out0;
  my=GetOutPortRows(block,1);
  ny=GetOutPortCols(block,1);
  out0=GetRealOparPtrs(block,6);
  so=GetSizeOfOut(block,1);  
  if (flag==4){
    /*array that contains 
      1 - the size of the indx1
      2 - indx1
      3 - the size of index2
      4 - index2
    */
    if((*(block->work)=scicos_malloc((my+ny+2)*sizeof(int)))==NULL)
      {set_block_error(-16);
	return;}

  }

  else if (flag==1){
    double *otsz, *indxopt, *indxb, *indx1, *indx2;
    int typ,i,j,nev;
    int *tempind1 = NULL,*tempind2 = NULL;
	void *y = NULL, *u = NULL;
    tempind1=(int *)*(block->work);
    tempind2=tempind1+(my+1);
    otsz=GetRealOparPtrs(block,5);
    indx2=GetRealOparPtrs(block,4);
    indx1=GetRealOparPtrs(block,3);
    indxopt=GetRealOparPtrs(block,2);
    indxb=GetRealOparPtrs(block,1);
    typ=GetOutType(block,1);
    nev=GetNevIn(block);
    if (nev%2==1){
      if (*out0==1){ /* initilize to the first input*/
	void *y0,*y;
	y=GetOutPortPtrs(block,1);
	y0=GetInPortPtrs(block,1);
	memcpy(y,y0,my*ny*so);
      }
    }
    if ((*indxopt==3)|(*indxopt==5)){
      if ((typ==SCSREAL_N)|(typ==SCSCOMPLEX_N)) {
	double *ind1;
	ind1=GetRealInPortPtrs(block,2+(int)*out0);
	*tempind1=1;
	*(tempind1+1)=max(min((int)(*ind1),my-(1-(int)(*indxb)))-(int)(*indxb),0);
      }
      else if ((typ==SCSINT8_N)|(typ==SCSUINT8_N)){
	char  *ind1;
	ind1=Getint8InPortPtrs(block,2+(int)*out0);
	*tempind1=1;
	*(tempind1+1)=max(min((int)(*ind1),my-(1-(int)(*indxb)))-(int)(*indxb),0);
      }
      else if ((typ==SCSINT16_N)|(typ==SCSUINT16_N)){
	short *ind1;
	ind1=Getint16InPortPtrs(block,2+(int)*out0);
	*tempind1=1;
	*(tempind1+1)=max(min((int)(*ind1),my-(1-(int)(*indxb)))-(int)(*indxb),0);
      }
      else if ((typ==SCSINT32_N)|(typ==SCSUINT32_N)){
	long *ind1;
	ind1=Getint32InPortPtrs(block,2+(int)*out0);
	*tempind1=1;
	*(tempind1+1)=max(min((int)(*ind1),my-(1-(int)(*indxb)))-(int)(*indxb),0);
      }
    }
    else if ((*indxopt==2)|(*indxopt==4)){
      *tempind1=GetOparSize(block,3,1)*GetOparSize(block,3,2);
      for (i=0;i<*tempind1;i++)
	*(tempind1+1+i)=max(min((int)(*(indx1+i)),my-(1-(int)(*indxb)))-(int)(*indxb),0);
    }
    else {
      *tempind1=my;
      for (i=0;i<my;i++)
	*(tempind1+1+i)=i;
    }
    if (ny==1){
      *tempind2=1;
      *(tempind2+1)=0;
    }
    else { /*matrix case */
      if (((*(indxopt+1)==3)|(*(indxopt+1)==5))&((*indxopt==3)|(*indxopt==5))){
	if ((typ==SCSREAL_N)|(typ==SCSCOMPLEX_N)) {
	  double *ind2;	  
	  ind2=GetRealInPortPtrs(block,3+(int)*out0);
	  *tempind2=1;
	  *(tempind2+1)=max(min((int)(*ind2),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
	}
	else if ((typ==SCSINT8_N)|(typ==SCSUINT8_N)){
	  char  *ind2;
	  ind2=Getint8InPortPtrs(block,3+(int)*out0);
	  *tempind2=1;
	  *(tempind2+1)=max(min((int)(*ind2),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
	}
	else if ((typ==SCSINT16_N)|(typ==SCSUINT16_N)){
	  short *ind2;
	  ind2=Getint16InPortPtrs(block,3+(int)*out0);
	  *tempind2=1;
	  *(tempind2+1)=max(min((int)(*ind2),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
	}
	else if ((typ==SCSINT32_N)|(typ==SCSUINT32_N)){
	  long *ind2;
	  ind2=Getint32InPortPtrs(block,3+(int)*out0);
	  *tempind2=1;
	  *(tempind2+1)=max(min((int)(*ind2),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
	}
      }
      else if (((*(indxopt+1)==3)|(*(indxopt+1)==5))&(!((*indxopt==3)|(*indxopt==5)))){
	if ((typ==SCSREAL_N)|(typ==SCSCOMPLEX_N)) {
	  double *ind2;	  
	  ind2=GetRealInPortPtrs(block,2+(int)*out0);
	  *tempind2=1;
	  *(tempind2+1)=max(min((int)(*ind2),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
	}
	else if ((typ==SCSINT8_N)|(typ==SCSUINT8_N)){
	  char  *ind2;
	  ind2=Getint8InPortPtrs(block,2+(int)*out0);
	  *tempind2=1;
	  *(tempind2+1)=max(min((int)(*ind2),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
	}
	else if ((typ==SCSINT16_N)|(typ==SCSUINT16_N)){
	  short *ind2;
	  ind2=Getint16InPortPtrs(block,2+(int)*out0);
	  *tempind2=1;
	  *(tempind2+1)=max(min((int)(*ind2),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
	}
	else if ((typ==SCSINT32_N)|(typ==SCSUINT32_N)){
	  long *ind2;
	  ind2=Getint32InPortPtrs(block,2+(int)*out0);
	  *tempind2=1;
	  *(tempind2+1)=max(min((int)(*ind2),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
	}
      }
      else if ((*(indxopt+1)==2)|(*(indxopt+1)==4)){
	*tempind2=GetOparSize(block,4,1)*GetOparSize(block,4,2);
	for (i=0;i<*tempind2;i++)
	  *(tempind2+1+i)=max(min((int)(*(indx2+i)),ny-(1-(int)(*indxb)))-(int)(*indxb),0);
      }
      else {
	*tempind1=ny;
	for (i=0;i<ny;i++)
	  *(tempind2+1+i)=i;
      }
    }

    y=GetOutPortPtrs(block,1);
    u=GetInPortPtrs(block,1+(int)*out0);
    for (j=0;j<tempind2[0];j++) 
	{
      for (i=0;i<tempind1[0];i++) 
	  {
		memcpy(((int*)y+((*(tempind2+1+j)*(my))+(*(tempind1+1+i)))*so),u,so);
      }
    }      
  }
  else if (flag==5) {
    if (*(block->work)!=0) scicos_free(*(block->work));
  } 
}
