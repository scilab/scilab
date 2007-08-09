#include <stdio.h>
#include <iostream>
#include <string.h>
#include "stack-c.h"
#include "sortTemplate.h"
#include "MALLOC.h"
using namespace std;
/*------------------------------------------------------------------------------------------*/
template <class T>
T GetMax (T a, T b) {
  return (a>b?1:0);
}
/*------------------------------------------------------------------------------------------*/
template <class T>
T my_swapsingle(T *v, int i, int j,int p,int m1)
{
   T temp;
   temp = v[i+m1*p];
   v[i+m1*p] = v[j+m1*p];
   v[j+m1*p] = temp;
   return 0;
}

/*------------------------------------------------------------------------------------------*/
template <class T>
T my_swapsinglecol(T *v, int i, int j,int p,int m1)
{
   T temp;
   temp = v[p+m1*i];
   v[p+m1*i] = v[p+m1*j];
   v[p+m1*j] = temp;
   return 0;
}
/*------------------------------------------------------------------------------------------*/
template <class D>
void swap (D *a, D *b) {
  D *p;
  *p=*a;
  *a=*b;
  *b=*p;
}
/*------------------------------------------------------------------------------------------*/
void my_swap(int *index, int i, int j)
{
   int temp;
   temp = index[i];
   index[i] = index[j];
   index[j] = temp;
}
/*------------------------------------------------------------------------------------------*/
void my_swapd(double *index, int i, int j)
{
   double temp;
   temp = index[i];
   index[i] = index[j];
   index[j] = temp;
}
/*------------------------------------------------------------------------------------------*/
void mywhole_swap(char **v,int i, int j)
{

   char *temp= v[i];
   v[i]=v[j];
   v[j]=temp;
}
/*------------------------------------------------------------------------------------------*/
void mywhole_swapd(double *v,int i, int j, int n,int m1)
{
	int f;
	double temp;
	for (f=0;f<n;f++)
	{
	  temp = v[f*m1+i];
	   v[f*m1+i] = v[f*m1+j];
	   v[f*m1+j] = temp;
	}
}
/*------------------------------------------------------------------------------------------*/
void mywhole_swaps(char **v,int i, int j, int n,int m1)
{
	char *temp;
	int f;
	for (f=0;f<n;f++)
	{
	  temp=v[f*m1+i];
	  v[f*m1+i]=v[f*m1+j];
      v[f*m1+j]=temp;	  
	}
}
/*------------------------------------------------------------------------------------------*/
void my_swapSsingle(char **v, int i, int j,int p,int m1)
{
   char *temp;
   temp=v[i+m1*p];
   v[i+m1*p]=v[j+m1*p];
   v[j+m1*p] =temp;
}
/*------------------------------------------------------------------------------------------*/
void mywhole_swapdcol(double *v,int i, int j, int n,int m1)
{
	int f;
	double temp;
	for (f=0;f<m1;f++)
	{
	  temp = v[i*m1+f];
	   v[i*m1+f] = v[j*m1+f];
	   v[j*m1+f] = temp;
	}
}
/*------------------------------------------------------------------------------------------*/
void mywhole_swapscol(char **v,int i, int j, int n,int m1)
{
	char *temp;
	int f;
	for (f=0;f<m1;f++)
	{
     temp= v[i*m1+f];
	 v[i*m1+f]=v[j*m1+f];
	 v[j*m1+f]=temp;
	}
}
/*------------------------------------------------------------------------------------------*/
void my_swapSsinglecol(char **v, int i, int j,int p,int m1)
{
   char *temp;
   temp=v[p+m1*i];
   v[p+m1*i]=v[p+m1*j];
   v[p+m1*j]=temp; 
}

/*------------------------------------------------------------------------------------------*/
void lgsortd(double *keys1,int *indices,int m1,int n1,char *typex, char *iord)  /* When it is double matrix in 'lr' or 'lc'*/
{
    int i;
    if (typex[1]=='r') 
	{
		for (i=0;i<m1;i++)  indices[i]=i+1;
	    void my_lgsortdrow(double *v,int *index, int left, int right,char *iord,int m1,int n1);
        my_lgsortdrow( keys1, indices,0, m1 - 1 ,iord,m1,n1);                /* When it is double matrix in 'lr' */
	}
    if (typex[1]=='c') 
	{
        for (i=0;i<n1;i++)  indices[i]=i+1;
	    void my_lgsortdcol(double *v,int *index, int left, int right,char *iord,int m1,int n1);
        my_lgsortdcol( keys1, indices,0, n1 - 1 ,iord,m1,n1);               /* When it is double matrix in 'lc'*/
	}
}
/*------------------------------------------------------------------------------------------*/
void lgsorts(char **keys1,int *indices,int m1,int n1,char *typex, char *iord)  /* When it is string matrix in 'lr' or 'lc'*/
{
    int i;
    if (typex[1]=='r') 
	{
		for (i=0;i<m1;i++)  indices[i]=i+1;
	    void my_qsortslrow(char **v,int *index, int left, int right,char *iord,int m1,int n1);
        my_qsortslrow( keys1, indices,0, m1 - 1 ,iord,m1,n1);                 /* When it is string matrix in 'lr'*/
	}
    if (typex[1]=='c') 
	{
        for (i=0;i<n1;i++)  indices[i]=i+1;
	    void my_qsortscol(char **v,int *index, int left, int right,char *iord,int m1,int n1);
        my_qsortscol( keys1, indices,0, n1 - 1 ,iord,m1,n1);                  /* When it is string matrix in 'lc'*/
	}
}
/*------------------------------------------------------------------------------------------*/
void rowcolsortd(double *keys1, int *indices, int m1, int n1, char *typex, char *iord) /*When it is double matrix in 'r' or 'c'*/
{
    int i,j,p;
	if (typex[0]=='r') 
	{
	    for (i=0;i<m1;i++)
		   for (j=0;j<n1;j++) indices[j*m1+i]=i+1;
		void my_qsortsingle(double *v,int *index, int left, int right,int p,char *iord,int m1);
		for (p=0;p<n1;p++) 
	    { 
              my_qsortsingle( keys1,indices, 0, m1 - 1 ,p,iord,m1);            /*When it is double matrix in 'r' */
		}
	}
	if (typex[0]=='c') 
	{
	    for (i=0;i<m1;i++)
		   for (j=0;j<n1;j++) indices[j*m1+i]=j+1;
		void my_qsortsinglecol(double *v,int *index, int left, int right,int p,char *iord,int m1);
		for (p=0;p<m1;p++) 
	    { 
              my_qsortsinglecol( keys1,indices, 0, n1 - 1 ,p,iord,m1);         /*When it is double matrix in 'c'*/
		}
	}
}
/*------------------------------------------------------------------------------------------*/
void wholesortd(double *keys1, int *indices, int m1, int n1, char *typex, char *iord)  /*When it is double matrix in 'g'*/
{
    int i,j,k;
	k=0;
    for (i=1;i<=m1;i++)
       for (j=1;j<=n1;j++) indices[k++]=(i-1)*n1+j;
	void my_wholesortd(double *v,int *index, int left, int right,char *iord,int n1);
    my_wholesortd( keys1, indices,0, m1*n1-1  ,iord,n1);
}
/*------------------------------------------------------------------------------------------*/
void wholesorts(char **keys, int *index, int m1, int n1, char *typex, char *iord)  /*When it is the string matrix in 'g'*/
{
    int i,j,k;
	k=0;
	for (i=1;i<=m1;i++)
      for (j=1;j<=n1;j++) index[k++]=(i-1)*n1+j;
	void my_wholesorts(char **v,int *index, int left, int right,char *iord,int n1);
    my_wholesorts( keys, index,0, m1*n1 - 1 ,iord,n1);
}
/*------------------------------------------------------------------------------------------*/
void rowcolsorts(char **keys, int *indices, int m1, int n1, char *typex, char *iord)  /*When it is the string matrix in 'r' or 'c'*/
{
    int i,j,p;
    if (typex[0]=='r') 
    {
	    for (i=0;i<m1;i++)
		   for (j=0;j<n1;j++) indices[j*m1+i]=i+1;
        void my_qsortSsinglerow(char **v,int *index, int left, int right,int p,char *iord,int m1);
		for (p=0;p<n1;p++) 
	    { 
		      my_qsortSsinglerow( keys,indices, 0, m1 - 1 ,p,iord,m1);            /*When it is the string matrix in 'r'*/
		}
	}
	if (typex[0]=='c') 
	{
	    for (i=0;i<m1;i++)
		   for (j=0;j<n1;j++) indices[j*m1+i]=j+1;
	    void my_qsortSsinglecol(char **v,int *index, int left, int right,int p,char *iord,int m1);
	    for (p=0;p<m1;p++) 
	    { 
              my_qsortSsinglecol( keys,indices, 0, n1 - 1 ,p,iord,m1);           /*When it is the string matrix in 'c'*/
		}
	}
}
/*------------------------------------------------------------------------------------------*/
void my_wholesorts(char **v,int *index, int left, int right,char *iord,int n1)
{
   int i, last,p;
   void my_swap(int *index, int i, int j);
   void mywhole_swap(char **v,int i, int j);
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   mywhole_swap(v, left, (left + right)/2); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) 
   {/* partition */
       p=0;
	   while (p<n1)
	   { 
		   if (v[i][p]==v[left][p]) p++;
		   else break;
		   if (p>=n1) {
			   p--;
			   break;
		   }
	   }
	   if (iord[0]=='i')
	   {
		   if (v[i][p]<v[left][p])
		   {
			   ++last;
			   mywhole_swap(v, last, i);
			   my_swap(index, last, i);
		   }
	   }
	   else 
	   {
	       if (v[i][p]>v[left][p])
		   {
			   ++last;
			   mywhole_swap(v, last, i);
			   my_swap(index, last, i);
		   }
	   }
   }
   mywhole_swap(v, left, last);
   my_swap(index, left, last);   /* restore partition elem */
   my_wholesorts(v,index, left, last-1,iord,n1);
   my_wholesorts(v,index, last+1, right,iord,n1);
}
/*------------------------------------------------------------------------------------------*/
void my_wholesortd(double *v,int *index, int left, int right,char *iord,int n1)
{
   int i, last;
   void my_swap(int *index, int i, int j);
   void my_swapd(double *v,int i, int j);
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   my_swapd(v, left, (left + right)/2); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) 
   {/* partition */
	   if (iord[0]=='i')
	   {
		   if (v[i]<v[left])
		   {
			   ++last;
			   my_swapd(v, last, i);
			   my_swap(index, last, i);
		   }
	   }
	   else 
	   {
	       if (v[i]>v[left])
		   {
			   ++last;
			   my_swapd(v, last, i);
			   my_swap(index, last, i);
		   }
	   }
   }
   my_swapd(v, left, last);
   my_swap(index, left, last);   /* restore partition elem */
   my_wholesortd(v,index, left, last-1,iord,n1);
   my_wholesortd(v,index, last+1, right,iord,n1);
}
/*------------------------------------------------------------------------------------------*/
void my_lgsortdrow(double *v,int *index, int left, int right,char *iord,int m1,int n1)
{
   int i, last,plist;
   void my_swap(int *index, int i, int j);
   void mywhole_swapd(double *v,int i, int j,int n,int m1);
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   mywhole_swapd(v, left, (left + right)/2,n1,m1); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) 
   {/* partition */
	   plist=0;
	   while (plist<n1)
	   { 
		   if (v[plist*m1+i]==v[plist*m1+left]) plist++;
		   else break;
		   if (plist>=n1) {
			   plist--;
			   break;
		   }
	   }
	   
	   if (iord[0]=='i')
	   {
		   if (!GetMax(v[plist*m1+i],v[plist*m1+left]))
		   {
			   ++last;
			   mywhole_swapd(v, last, i,n1,m1);
			   my_swap(index, last, i);
		   }
	   }
	   else 
	   {
	       if (GetMax(v[plist*m1+i],v[plist*m1+left]))
		   {
			   ++last;
			   mywhole_swapd(v, last, i,n1,m1);
			   my_swap(index, last, i);
		   }
	   }
   }
   mywhole_swapd(v, left, last,n1,m1);
   my_swap(index, left, last);   /* restore partition elem */
   my_lgsortdrow(v,index, left, last-1,iord,m1,n1);
   my_lgsortdrow(v,index, last+1, right,iord,m1,n1);
}
/*------------------------------------------------------------------------------------------*/
void my_qsortslrow(char **v,int *index, int left, int right,char *iord,int m1,int n1)
{
   int i, last,p;
   void my_swap(int *index, int i, int j);
   void mywhole_swaps(char **v,int i, int j,int n,int m1);
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   mywhole_swaps(v, left, (left + right)/2,n1,m1); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) 
   {/* partition */
	   for (p=0;strcmp(v[p*m1+i],v[p*m1+left])==0 ;p++);
	   if (iord[0]=='i')
	   {
		   if (strcmp(v[p*m1+i],v[p*m1+left])<=0)
		   {
			   ++last;
			   mywhole_swaps(v, last, i,n1,m1);
			   my_swap(index, last, i);
		   }
	   }
	   else
	   {
	       if (strcmp(v[p*m1+i],v[p*m1+left])>0)
		   {
			   ++last;
			   mywhole_swaps(v, last, i,n1,m1);
			   my_swap(index, last, i);
		   }
	   }
   }
   mywhole_swaps(v, left, last,n1,m1);
   my_swap(index, left, last);   /* restore partition elem */
   my_qsortslrow(v,index, left, last-1,iord,m1,n1);
   my_qsortslrow(v,index, last+1, right,iord,m1,n1);
}
/*------------------------------------------------------------------------------------------*/
void my_lgsortdcol(double *v,int *index, int left, int right,char *iord,int m1,int n1)
{
   int i, last,p;
   /* prototype */
   void my_swap(int *index, int i, int j);
   void mywhole_swapdcol(double *v,int i, int j,int n,int m1);
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   mywhole_swapdcol(v, left, (left + right)/2,n1,m1); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) 
   {/* partition */
	   for (p=0;v[i*m1+p]==v[left*m1+p] ;p++);
	   if (iord[0]=='i')
	   {
		   if (!GetMax(v[i*m1+p],v[left*m1+p]))
		   {
			   ++last;
			   mywhole_swapdcol(v, last, i,n1,m1);
			   my_swap(index, last, i);
		   }
	   }
	   else
	   {
	       if (GetMax(v[i*m1+p],v[left*m1+p]))
		   {
			   ++last;
			   mywhole_swapdcol(v, last, i,n1,m1);
			   my_swap(index, last, i);
		   }
	   }
   }
   mywhole_swapdcol(v, left, last,n1,m1);
   my_swap(index, left, last);   /* restore partition elem */
   my_lgsortdcol(v,index, left, last-1,iord,m1,n1);
   my_lgsortdcol(v,index, last+1, right,iord,m1,n1);
}
/*------------------------------------------------------------------------------------------*/
void my_qsortscol(char **v,int *index, int left, int right,char *iord,int m1,int n1)
{
   int i, last,p;
   void my_swap(int *index, int i, int j);
   void mywhole_swapscol(char **v,int i, int j,int n,int m1);
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   mywhole_swapscol(v, left, (left + right)/2,n1,m1); /* move partition elem */
   my_swap(index, left, (left + right)/2);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) 
   {/* partition */
	   for (p=0;strcmp(v[i*m1+p],v[left*m1+p])==0 ;p++);
	   if (iord[0]=='i')
	   {
		   if (strcmp(v[i*m1+p],v[left*m1+p])<=0)
		   {
			   ++last;
			   mywhole_swapscol(v, last, i,n1,m1);
			   my_swap(index, last, i);
		   }
	   }
	   else 
	   {
	       if (strcmp(v[i*m1+p],v[left*m1+p])>0)
		   {
			   ++last;
			   mywhole_swapscol(v, last, i,n1,m1);
			   my_swap(index, last, i);
		   }
	   }
   }
   mywhole_swapscol(v, left, last,n1,m1);
   my_swap(index, left, last);   /* restore partition elem */
   my_qsortscol(v,index, left, last-1,iord,m1,n1);
   my_qsortscol(v,index, last+1, right,iord,m1,n1);
}
/*------------------------------------------------------------------------------------------*/
void my_qsortsingle(double *v, int *index,int left, int right,int p,char *iord,int m1)
{
   int i, last;
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   my_swapsingle(v, left, (left + right)/2,p,m1); /* move partition elem */
   my_swapsingle(index, left, (left + right)/2,p,m1);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) /* partition */
   {
	   if (iord[0]=='i')
	   {
		   if (v[p*m1+i]<v[left+p*m1])
		   {
			   ++last;
			    my_swapsingle(v, last, i,p,m1);
			    my_swapsingle(index, last, i,p,m1);
		   }
	   }
	   else 
	   {
	       if (v[i+p*m1]>v[left+p*m1])
		   {
			   ++last;
			   my_swapsingle(v, last, i,p,m1);
			   my_swapsingle(index, last, i,p,m1);
		   }
	   }
   }   
   my_swapsingle(v, left, last,p,m1);   
   my_swapsingle(index, left, last,p,m1); /* restore partition elem */
   my_qsortsingle(v,index, left, last-1,p,iord,m1);
   my_qsortsingle(v,index, last+1, right,p,iord,m1);
}
/*------------------------------------------------------------------------------------------*/
void my_qsortSsinglerow(char **v, int *index,int left, int right,int p,char *iord,int m1)
{
   int i, last;
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   void my_swapSsinglecol(char **v, int i, int j,int p,int m1);
   my_swapSsingle(v, left, (left + right)/2,p,m1); /* move partition elem */
   my_swapsingle(index, left, (left + right)/2,p,m1);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) /* partition */
   {
	   if (iord[0]=='i')
	   {
		   if (strcmp(v[p*m1+i],v[left+p*m1])<0)
		   {
			   ++last;
			    my_swapSsingle(v, last, i,p,m1);
			    my_swapsingle(index, last, i,p,m1);
		   }
	   }
	   else 
	   {
	       if (strcmp(v[i+p*m1],v[left+p*m1])>0)
		   {
			   ++last;
			   my_swapSsingle(v, last, i,p,m1);
			   my_swapsingle(index, last, i,p,m1);
		   }
	   }
   }   
   my_swapSsingle(v, left, last,p,m1);   
   my_swapsingle(index, left, last,p,m1); /* restore partition elem */
   my_qsortSsinglerow(v,index, left, last-1,p,iord,m1);
   my_qsortSsinglerow(v,index, last+1, right,p,iord,m1);
}

/*------------------------------------------------------------------------------------------*/
void my_qsortsinglecol(double *v, int *index,int left, int right,int p,char *iord,int m1)
{
   int i, last;
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   my_swapsinglecol(v, left, (left + right)/2,p,m1); /* move partition elem */
   my_swapsinglecol(index, left, (left + right)/2,p,m1);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) /* partition */
   {
	   if (iord[0]=='i')
	   {
		   if (v[p+m1*i]<v[p+m1*left])
		   {
			   ++last;
			    my_swapsinglecol(v, last, i,p,m1);
			    my_swapsinglecol(index, last, i,p,m1);
		   }
	   }
	   else
	   {
	       if (v[p+m1*i]>v[p+m1*left])
		   {
			   ++last;
			   my_swapsinglecol(v, last, i,p,m1);
			   my_swapsinglecol(index, last, i,p,m1);
		   }
	   }
   }   
   my_swapsinglecol(v, left, last,p,m1);   
   my_swapsinglecol(index, left, last,p,m1); /* restore partition elem */
   my_qsortsinglecol(v,index, left, last-1,p,iord,m1);
   my_qsortsinglecol(v,index, last+1, right,p,iord,m1);
}
/*------------------------------------------------------------------------------------------*/
void my_qsortSsinglecol(char **v, int *index,int left, int right,int p,char *iord,int m1)
{
   int i, last;
   if (left >= right) /* do nothing if array contains */
       return;        /* fewer than two elements */
   void my_swapSsinglecol(char **v, int i, int j,int p,int m1);
   my_swapSsinglecol(v, left, (left + right)/2,p,m1); /* move partition elem */
   my_swapsinglecol(index, left, (left + right)/2,p,m1);
   last = left;                        /* to v[0] */
   for (i = left + 1; i <= right; i++) /* partition */
   {
	   if (iord[0]=='i')
	   {
		   if (strcmp(v[p+m1*i],v[p+m1*left])<0)
		   {
			   ++last;
			    my_swapSsinglecol(v, last, i,p,m1);
			    my_swapsinglecol(index, last, i,p,m1);
		   }
	   }
	   else 
	   {
	       if (strcmp(v[p+m1*i],v[p+m1*left])>0)
		   {
			   ++last;
			   my_swapSsinglecol(v, last, i,p,m1);
			   my_swapsinglecol(index, last, i,p,m1);
		   }
	   }
   }   
   my_swapSsinglecol(v, left, last,p,m1);   
   my_swapsinglecol(index, left, last,p,m1); /* restore partition elem */
   my_qsortSsinglecol(v,index, left, last-1,p,iord,m1);
   my_qsortSsinglecol(v,index, last+1, right,p,iord,m1);
}
/*------------------------------------------------------------------------------------------*/

