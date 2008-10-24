/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "stack-c.h"
#include "gsort.h"
#include "sortTemplate.h"
#include "MALLOC.h"
using namespace std;

void mywhole_swapd(double *v,int i, int j, int n);

/*---------------------------------------------------------------------------------*/
template <class T>
T GetMax (T a, T b) {
  return (a>b?1:0);
}
/*---------------------------------------------------------------------------------*/
template <class T>
T my_swapsingle(T *v, int i, int j,int PointerRow,int RowNum)
{
   T temp;
   int Left_Position=i+RowNum*PointerRow;
   int Right_Position=j+RowNum*PointerRow;
   temp = v[Left_Position];
   v[Left_Position] = v[Right_Position];
   v[Right_Position] = temp;
   return 0;
}

/*---------------------------------------------------------------------------------*/
template <class T>
T my_swapsinglecol(T *v, int i, int j,int PointerRow,int RowNum)
{
   T temp;
   int Left_Position=PointerRow+RowNum*i;
   int Right_Position=PointerRow+RowNum*j;
   temp = v[Left_Position];
   v[Left_Position] = v[Right_Position];
   v[Right_Position] = temp;
   return 0;
}
/*---------------------------------------------------------------------------------*/
template <class D>
void swap (D *a, D *b) {
  D *position;
  *position=*a;
  *a=*b;
  *b=*position;
}
/*---------------------------------------------------------------------------------*/
void my_swap(int *index, int i, int j)
{
   int temp;
   temp = index[i];
   index[i] = index[j];
   index[j] = temp;
}
/*---------------------------------------------------------------------------------*/
void my_swapdouble(double *index, int i, int j)
{
   double temp;
   temp = index[i];
   index[i] = index[j];
   index[j] = temp;
}
/*---------------------------------------------------------------------------------*/
void mywhole_swap(char **v,int i, int j)
{

   char *temp= v[i];
   v[i]=v[j];
   v[j]=temp;
}
/*---------------------------------------------------------------------------------*/
void mywhole_swapd(double *v,int i, int j, int ColNum,int RowNum)
{
	int f; /** @TODO rename this variable */
	double temp;
	for (f=0;f<ColNum;f++)
	{
		temp = v[f*RowNum+i];
		v[f*RowNum+i] = v[f*RowNum+j];
	   v[f*RowNum+j] = temp;
	}
}
/*---------------------------------------------------------------------------------*/
void mywhole_swaps(char **v,int i, int j, int ColNum,int RowNum)
{
	char *temp;
	int f; /** @TODO rename this variable */
	for (f=0;f<ColNum;f++)
	{
	  temp=v[f*RowNum+i];
	  v[f*RowNum+i]=v[f*RowNum+j];
      v[f*RowNum+j]=temp;
	}
}
/*---------------------------------------------------------------------------------*/
void my_swapSsingle(char **v, int i, int j,int PointerRow,int RowNum)
{
   char *temp;
   int Left_Position=PointerRow*RowNum+i;
   int Right_Position=PointerRow*RowNum+j;
   temp=v[Left_Position];
   v[Left_Position]=v[Right_Position];
   v[Right_Position] =temp;
}
/*---------------------------------------------------------------------------------*/
//void mywhole_swapdcol(double *v,int i, int j, int n,int RowNum)
	/** @TODO Check why n is unused */
void mywhole_swapdcol(double *v,int i, int j,int RowNum)
{
	int f; /** @TODO rename this variable */
	double temp;
	for (f=0;f<RowNum;f++)
	{
	  temp = v[i*RowNum+f];
	   v[i*RowNum+f] = v[j*RowNum+f];
	   v[j*RowNum+f] = temp;
	}
}
/*---------------------------------------------------------------------------------*/
//void mywhole_swapscol(char **v,int i, int j, int n,int RowNum)
	/** @TODO Check why n is unused */
void mywhole_swapscol(char **v,int i, int j, int RowNum)
{
	char *temp;
	int f; /** @TODO rename this variable */
	for (f=0;f<RowNum;f++)
	{
     temp= v[i*RowNum+f];
	 v[i*RowNum+f]=v[j*RowNum+f];
	 v[j*RowNum+f]=temp;
	}
}
/*---------------------------------------------------------------------------------*/
void my_swapSsinglecol(char **v, int i, int j,int position,int RowNum)
{
   char *temp;
   temp=v[position+RowNum*i];
   v[position+RowNum*i]=v[position+RowNum*j];
   v[position+RowNum*j]=temp;
}
/*---------------------------------------------------------------------------------*/
void my_lgsortdoublerow(double *Input_Matrix,int *index, int left, int right,char *iord,int RowNum,int RowCol)
{
   int i, last,plist;
   void my_swap(int *index, int i, int j);
   void mywhole_swapd(double *v,int i, int j,int n,int RowNum);
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   mywhole_swapd(Input_Matrix, left, (left + right)/2,RowCol,RowNum); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++)
   {/* partition */
	   plist=0;
	   while (plist<RowCol)
	   {
		   if (Input_Matrix[plist*RowNum+i]==Input_Matrix[plist*RowNum+left]) plist++;
		   else break;
		   if (plist>=RowCol) {
			   plist--;
			   break;
		   }
	   }

	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (!GetMax(Input_Matrix[plist*RowNum+i],Input_Matrix[plist*RowNum+left]))
		   {
			   ++last;
			   mywhole_swapd(Input_Matrix, last, i,RowCol,RowNum);
			   my_swap(index, last, i);
		   }
	   }
	   else
	   {
	       if (GetMax(Input_Matrix[plist*RowNum+i],Input_Matrix[plist*RowNum+left]))
		   {
			   ++last;
			   mywhole_swapd(Input_Matrix, last, i,RowCol,RowNum);
			   my_swap(index, last, i);
		   }
	   }
   }
   mywhole_swapd(Input_Matrix, left, last,RowCol,RowNum);
   my_swap(index, left, last);   /* restore partition elem */
   my_lgsortdoublerow(Input_Matrix,index, left, last-1,iord,RowNum,RowCol);
   my_lgsortdoublerow(Input_Matrix,index, last+1, right,iord,RowNum,RowCol);
}
/*---------------------------------------------------------------------------------*/
void my_lgsortdoublecol(double *Input_Matrix,int *index, int left, int right,char *iord,int RowNum,int RowCol)
{
   int i, last,position;

   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */

   // @FIXME Bruno : Useless parameter at call ? What's the use of RowCol ??
   //   mywhole_swapdcol(Input_Matrix, left, (left + right)/2,RowCol,RowNum); /* move partition elem */

   mywhole_swapdcol(Input_Matrix, left, (left + right)/2,RowNum); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++)
   {/* partition */
	   for (position=0;Input_Matrix[i*RowNum+position]==Input_Matrix[left*RowNum+position] ;position++) {}
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (!GetMax(Input_Matrix[i*RowNum+position],Input_Matrix[left*RowNum+position]))
		   {
			   ++last;
			   // @FIXME Bruno : Useless parameter at call ? What's the use of RowCol ??
			   // mywhole_swapdcol(Input_Matrix, last, i,RowCol,RowNum);
			   mywhole_swapdcol(Input_Matrix, last, i, RowNum);
			   my_swap(index, last, i);
		   }
	   }
	   else
	   {
	       if (GetMax(Input_Matrix[i*RowNum+position],Input_Matrix[left*RowNum+position]))
		   {
			   ++last;
			   // @FIXME Bruno : Useless parameter at call ? What's the use of RowCol ??
			   // mywhole_swapdcol(Input_Matrix, last, i,RowCol,RowNum);
			   mywhole_swapdcol(Input_Matrix, last, i,RowNum);
			   my_swap(index, last, i);
		   }
	   }
   }
   // @FIXME Bruno : Useless parameter at call ? What's the use of RowCol ??
   //   mywhole_swapdcol(Input_Matrix, left, last,RowCol,RowNum);
   mywhole_swapdcol(Input_Matrix, left, last, RowNum);

   my_swap(index, left, last);   /* restore partition elem */
   my_lgsortdoublecol(Input_Matrix,index, left, last-1,iord,RowNum,RowCol);
   my_lgsortdoublecol(Input_Matrix,index, last+1, right,iord,RowNum,RowCol);
}
/*---------------------------------------------------------------------------------*/
void my_qsortslrow(char **Input_Matrix,int *index, int left, int right,char *iord,int RowNum,int RowCol)
{
   int i, last,position;

   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   mywhole_swaps(Input_Matrix, left, (left + right)/2,RowCol,RowNum); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++)
   {/* partition */
	   for (position=0;strcmp(Input_Matrix[position*RowNum+i],Input_Matrix[position*RowNum+left])==0 ;position++) {}
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (strcmp(Input_Matrix[position*RowNum+i],Input_Matrix[position*RowNum+left])<=0)
		   {
			   ++last;
			   mywhole_swaps(Input_Matrix, last, i,RowCol,RowNum);
			   my_swap(index, last, i);
		   }
	   }
	   else
	   {
	       if (strcmp(Input_Matrix[position*RowNum+i],Input_Matrix[position*RowNum+left])>0)
		   {
			   ++last;
			   mywhole_swaps(Input_Matrix, last, i,RowCol,RowNum);
			   my_swap(index, last, i);
		   }
	   }
   }
   mywhole_swaps(Input_Matrix, left, last,RowCol,RowNum);
   my_swap(index, left, last);   /* restore partition elem */
   my_qsortslrow(Input_Matrix,index, left, last-1,iord,RowNum,RowCol);
   my_qsortslrow(Input_Matrix,index, last+1, right,iord,RowNum,RowCol);
}

/*---------------------------------------------------------------------------------*/
void my_qsortsingle(double *Input_Matrix, int *index,int left, int right,int position,char *iord,int RowNum)
{
   int i, last;
   int left_position;
   int right_position;
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   my_swapsingle(Input_Matrix, left, (left + right)/2,position,RowNum); /* move partition elem */
   my_swapsingle(index, left, (left + right)/2,position,RowNum);
   last = left;
   for (i = left + 1; i <= right; i++) /* partition */
   {
	   left_position=position*RowNum+i;
       right_position=left+position*RowNum;
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (Input_Matrix[left_position]<Input_Matrix[right_position])
		   {
			   ++last;
			    my_swapsingle(Input_Matrix, last, i,position,RowNum);
			    my_swapsingle(index, last, i,position,RowNum);
		   }
	   }
	   else
	   {
	       if (Input_Matrix[left_position]>Input_Matrix[right_position])
		   {
			   ++last;
			   my_swapsingle(Input_Matrix, last, i,position,RowNum);
			   my_swapsingle(index, last, i,position,RowNum);
		   }
	   }
   }
   my_swapsingle(Input_Matrix, left, last,position,RowNum);
   my_swapsingle(index, left, last,position,RowNum); /* restore partition elem */
   my_qsortsingle(Input_Matrix,index, left, last-1,position,iord,RowNum);
   my_qsortsingle(Input_Matrix,index, last+1, right,position,iord,RowNum);
}
/*---------------------------------------------------------------------------------*/
void my_qsortsinglecol(double *Input_Matrix, int *index,int left, int right,int position,char *iord,int RowNum)
{
   int i, last;
   int left_position,right_position;
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   my_swapsinglecol(Input_Matrix, left, (left + right)/2,position,RowNum); /* move partition elem */
   my_swapsinglecol(index, left, (left + right)/2,position,RowNum);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) /* partition */
   {
	   left_position=position+RowNum*i;
	   right_position=position+RowNum*left;
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (Input_Matrix[left_position]<Input_Matrix[right_position])
		   {
			   ++last;
			    my_swapsinglecol(Input_Matrix, last, i,position,RowNum);
			    my_swapsinglecol(index, last, i,position,RowNum);
		   }
	   }
	   else
	   {
	       if (Input_Matrix[left_position]>Input_Matrix[right_position])
		   {
			   ++last;
			   my_swapsinglecol(Input_Matrix, last, i,position,RowNum);
			   my_swapsinglecol(index, last, i,position,RowNum);
		   }
	   }
   }
   my_swapsinglecol(Input_Matrix, left, last,position,RowNum);
   my_swapsinglecol(index, left, last,position,RowNum); /* restore partition elem */
   my_qsortsinglecol(Input_Matrix,index, left, last-1,position,iord,RowNum);
   my_qsortsinglecol(Input_Matrix,index, last+1, right,position,iord,RowNum);
}
/*---------------------------------------------------------------------------------*/
void lgsortdouble(double *Input_Matrix,int *indices,int RowNum,int RowCol,char *typex, char *iord)  /* When it is double matrix in 'lr' or 'lc'*/
{
    int i;
    if (typex[1]==ROW_SORT)
	{
		for (i=0;i<RowNum;i++)  indices[i]=i+1;
        my_lgsortdoublerow( Input_Matrix, indices,0, RowNum - 1 ,iord,RowNum,RowCol);                /* When it is double matrix in 'lr' */
	}
    if (typex[1]==COLUMN_SORT)
	{
        for (i=0;i<RowCol;i++)  indices[i]=i+1;
        my_lgsortdoublecol( Input_Matrix, indices,0, RowCol - 1 ,iord,RowNum,RowCol);               /* When it is double matrix in 'lc'*/
	}
}
/*---------------------------------------------------------------------------------*/

void rowcolsortdouble(double *Input_Matrix, int *indices, int RowNum, int RowCol, char *typex, char *iord) /*When it is double matrix in ROW_SORT or COLUMN_SORT*/
{
    int i,j,position;
	if (typex[0]==ROW_SORT)
	{
	    for (i=0;i<RowNum;i++)
		   for (j=0;j<RowCol;j++) indices[j*RowNum+i]=i+1;

		for (position=0;position<RowCol;position++)
	    {
              my_qsortsingle( Input_Matrix,indices, 0, RowNum - 1 ,position,iord,RowNum);            /*When it is double matrix in ROW_SORT */
		}
	}
	if (typex[0]==COLUMN_SORT)
	{
	    for (i=0;i<RowNum;i++)
		   for (j=0;j<RowCol;j++) indices[j*RowNum+i]=j+1;
		for (position=0;position<RowNum;position++)
	    {
              my_qsortsinglecol( Input_Matrix,indices, 0, RowCol - 1 ,position,iord,RowNum);         /*When it is double matrix in COLUMN_SORT*/
		}
	}
}
/*---------------------------------------------------------------------------------*/

void my_wholesortstring(char **Input_Matrix,int *index, int left, int right,char *iord,int RowCol)
{
   int i, last,position;

   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   mywhole_swap(Input_Matrix, left, (left + right)/2); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++)
   {/* partition */
       position=0;
	   while (position<RowCol)
	   {
		   if (Input_Matrix[i][position]==Input_Matrix[left][position]) position++;
		   else break;
		   if (position>=RowCol) {
			   position--;
			   break;
		   }
	   }
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (Input_Matrix[i][position]<Input_Matrix[left][position])
		   {
			   ++last;
			   mywhole_swap(Input_Matrix, last, i);
			   my_swap(index, last, i);
		   }
	   }
	   else
	   {
	       if (Input_Matrix[i][position]>Input_Matrix[left][position])
		   {
			   ++last;
			   mywhole_swap(Input_Matrix, last, i);
			   my_swap(index, last, i);
		   }
	   }
   }
   mywhole_swap(Input_Matrix, left, last);
   my_swap(index, left, last);   /* restore partition elem */
   my_wholesortstring(Input_Matrix,index, left, last-1,iord,RowCol);
   my_wholesortstring(Input_Matrix,index, last+1, right,iord,RowCol);
}
/*---------------------------------------------------------------------------------*/
void my_wholesortdouble(double *Input_Matrix,int *index, int left, int right,char *iord,int RowCol)
{
   int i, last;

   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   my_swapdouble(Input_Matrix, left, (left + right)/2); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++)
   {/* partition */
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (Input_Matrix[i]<Input_Matrix[left])
		   {
			   ++last;
			   my_swapdouble(Input_Matrix, last, i);
			   my_swap(index, last, i);
		   }
	   }
	   else
	   {
	       if (Input_Matrix[i]>Input_Matrix[left])
		   {
			   ++last;
			   my_swapdouble(Input_Matrix, last, i);
			   my_swap(index, last, i);
		   }
	   }
   }
   my_swapdouble(Input_Matrix, left, last);
   my_swap(index, left, last);   /* restore partition elem */
   my_wholesortdouble(Input_Matrix,index, left, last-1,iord,RowCol);
   my_wholesortdouble(Input_Matrix,index, last+1, right,iord,RowCol);
}


/*---------------------------------------------------------------------------------*/
void my_qsortscol(char **Input_Matrix,int *index, int left, int right,char *iord,int RowNum,int RowCol)
{
   int i, last,position;

   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */

   // @FIXME Bruno : Useless parameter at call ? What's the use of RowCol ??
   //   mywhole_swapscol(Input_Matrix, left, (left + right)/2,RowCol,RowNum); /* move partition elem */
   mywhole_swapscol(Input_Matrix, left, (left + right)/2, RowNum); /* move partition elem */

   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++)
   {/* partition */
	   for (position=0;strcmp(Input_Matrix[i*RowNum+position],Input_Matrix[left*RowNum+position])==0 ;position++) {}
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (strcmp(Input_Matrix[i*RowNum+position],Input_Matrix[left*RowNum+position])<=0)
		   {
			   ++last;
			   // @FIXME Bruno : Useless parameter at call ? What's the use of RowCol ??
			   // mywhole_swapscol(Input_Matrix, last, i,RowCol,RowNum);
			   mywhole_swapscol(Input_Matrix, last, i, RowNum);

			   my_swap(index, last, i);
		   }
	   }
	   else
	   {
	       if (strcmp(Input_Matrix[i*RowNum+position],Input_Matrix[left*RowNum+position])>0)
		   {
			   ++last;
			   // @FIXME Bruno : Useless parameter at call ? What's the use of RowCol ??
			   // mywhole_swapscol(Input_Matrix, last, i,RowCol,RowNum);
			   mywhole_swapscol(Input_Matrix, last, i, RowNum);

			   my_swap(index, last, i);
		   }
	   }
   }
   // @FIXME Bruno : Useless parameter at call ? What's the use of RowCol ??
   // mywhole_swapscol(Input_Matrix, left, last,RowCol,RowNum);
   mywhole_swapscol(Input_Matrix, left, last,RowNum);

   my_swap(index, left, last);   /* restore partition elem */
   my_qsortscol(Input_Matrix,index, left, last-1,iord,RowNum,RowCol);
   my_qsortscol(Input_Matrix,index, last+1, right,iord,RowNum,RowCol);
}

/*---------------------------------------------------------------------------------*/
void my_qsortSsinglerow(char **Input_Matrix, int *index,int left, int right,int position,char *iord,int RowNum)
{
   int i, last;
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */

   my_swapSsingle(Input_Matrix, left, (left + right)/2,position,RowNum); /* move partition elem */
   my_swapsingle(index, left, (left + right)/2,position,RowNum);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) /* partition */
   {
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (strcmp(Input_Matrix[position*RowNum+i],Input_Matrix[left+position*RowNum])<0)
		   {
			   ++last;
			    my_swapSsingle(Input_Matrix, last, i,position,RowNum);
			    my_swapsingle(index, last, i,position,RowNum);
		   }
	   }
	   else
	   {
	       if (strcmp(Input_Matrix[i+position*RowNum],Input_Matrix[left+position*RowNum])>0)
		   {
			   ++last;
			   my_swapSsingle(Input_Matrix, last, i,position,RowNum);
			   my_swapsingle(index, last, i,position,RowNum);
		   }
	   }
   }
   my_swapSsingle(Input_Matrix, left, last,position,RowNum);
   my_swapsingle(index, left, last,position,RowNum); /* restore partition elem */
   my_qsortSsinglerow(Input_Matrix,index, left, last-1,position,iord,RowNum);
   my_qsortSsinglerow(Input_Matrix,index, last+1, right,position,iord,RowNum);
}


/*---------------------------------------------------------------------------------*/
void my_qsortSsinglecol(char **Input_Matrix, int *index,int left, int right,int position,char *iord,int RowNum)
{
   int i, last;
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */

   my_swapSsinglecol(Input_Matrix, left, (left + right)/2,position,RowNum); /* move partition elem */
   my_swapsinglecol(index, left, (left + right)/2,position,RowNum);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) /* partition */
   {
	   if (iord[0]==INCREASE_COMMAND)
	   {
		   if (strcmp(Input_Matrix[position+RowNum*i],Input_Matrix[position+RowNum*left])<0)
		   {
			   ++last;
			    my_swapSsinglecol(Input_Matrix, last, i,position,RowNum);
			    my_swapsinglecol(index, last, i,position,RowNum);
		   }
	   }
	   else
	   {
	       if (strcmp(Input_Matrix[position+RowNum*i],Input_Matrix[position+RowNum*left])>0)
		   {
			   ++last;
			   my_swapSsinglecol(Input_Matrix, last, i,position,RowNum);
			   my_swapsinglecol(index, last, i,position,RowNum);
		   }
	   }
   }
   my_swapSsinglecol(Input_Matrix, left, last,position,RowNum);
   my_swapsinglecol(index, left, last,position,RowNum); /* restore partition elem */
   my_qsortSsinglecol(Input_Matrix,index, left, last-1,position,iord,RowNum);
   my_qsortSsinglecol(Input_Matrix,index, last+1, right,position,iord,RowNum);
}
/*---------------------------------------------------------------------------------*/
// void wholesortdouble(double *Input_Matrix, int *indices, int RowNum, int RowCol, char *typex, char *iord)
/*When it is double matrix in 'g'*/
	/** @TODO check why typex is unused */
void wholesortdouble(double *Input_Matrix, int *indices, int RowNum, int RowCol, char *iord)
{
    int i,j,k;
	k=0;
    for (i=1;i<=RowNum;i++)
       for (j=1;j<=RowCol;j++) indices[k++]=(i-1)*RowCol+j;
    my_wholesortdouble( Input_Matrix, indices,0, RowNum*RowCol-1  ,iord,RowCol);
}
/*---------------------------------------------------------------------------------*/
// void wholesortstring(char **Input_Matrix, int *index, int RowNum, int RowCol, char *typex, char *iord)
/*When it is the string matrix in 'g'*/
	/** @TODO check why typex is unused */
void wholesortstring(char **Input_Matrix, int *index, int RowNum, int RowCol, char *iord)
{
    int i,j,k;
	k=0;
	for (i=1;i<=RowNum;i++)
      for (j=1;j<=RowCol;j++) index[k++]=(i-1)*RowCol+j;
    my_wholesortstring( Input_Matrix, index,0, RowNum*RowCol - 1 ,iord,RowCol);
}
/*---------------------------------------------------------------------------------*/
void rowcolsortstring(char **Input_Matrix, int *indices, int RowNum, int RowCol, char *typex, char *iord)  /*When it is the string matrix in ROW_SORT or COLUMN_SORT*/
{
    int i,j,position;
    if (typex[0]==ROW_SORT)
    {
	    for (i=0;i<RowNum;i++)
		   for (j=0;j<RowCol;j++) indices[j*RowNum+i]=i+1;
		for (position=0;position<RowCol;position++)
	    {
		      my_qsortSsinglerow( Input_Matrix,indices, 0, RowNum - 1 ,position,iord,RowNum);            /*When it is the string matrix in ROW_SORT*/
		}
	}
	if (typex[0]==COLUMN_SORT)
	{
	    for (i=0;i<RowNum;i++)
		   for (j=0;j<RowCol;j++) indices[j*RowNum+i]=j+1;
	    for (position=0;position<RowNum;position++)
	    {
              my_qsortSsinglecol( Input_Matrix,indices, 0, RowCol - 1 ,position,iord,RowNum);           /*When it is the string matrix in COLUMN_SORT*/
		}
	}
}
/*---------------------------------------------------------------------------------*/
void lgsortstring(char **Input_Matrix,int *indices,int RowNum,int RowCol,char *typex, char *iord)  /* When it is string matrix in 'lr' or 'lc'*/
{
    int i;
    if (typex[1]==ROW_SORT)
	{
		for (i=0;i<RowNum;i++)  indices[i]=i+1;
        my_qsortslrow( Input_Matrix, indices,0, RowNum - 1 ,iord,RowNum,RowCol);                 /* When it is string matrix in 'lr'*/
	}
    if (typex[1]==COLUMN_SORT)
	{
        for (i=0;i<RowCol;i++)  indices[i]=i+1;
        my_qsortscol( Input_Matrix, indices,0, RowCol - 1 ,iord,RowNum,RowCol);                  /* When it is string matrix in 'lc'*/
	}
}
/*---------------------------------------------------------------------------------*/
