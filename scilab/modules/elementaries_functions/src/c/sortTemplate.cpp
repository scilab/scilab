#include <stdio.h>
#include <iostream>
#include <string.h>
#include "stack-c.h"
#include "sortTemplate.h"

using namespace std;
/*------------------------------------------------------------------------------------------*/
template <class T>
T GetMax (T a, T b) {
  return (a>b?1:0);
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
template <class T>
void mutisort (T *keys1,int indices[20],int m1,int n1,char *typex, char *iord)
{
    int i,j,p,f;
    T keys[20][20];
	if (typex[1]=='r') 
	{
	    for (i=0;i<m1;i++)  indices[i]=i+1;
		for (i=1;i<=n1;i++)
		for (j=1;j<=m1;j++)  keys[j-1][i-1]=keys1[(i-1)*m1+j-1];
		for(i=m1-1;i>0;i--)
		  for(j=0;j<i;j++)
		  {
			  for (p=0;keys[j][p]==keys[j+1][p] ;p++);
			  if (iord[0]=='i')
			  {
				  if (GetMax(keys[j][p],keys[j+1][p])) 
				  {
 						for (f=0;f<n1;f++)
						{                
						  swap(indices[j],indices[j+1]);
						  swap(keys[j][f],keys[j+1][f]);			
						}
				  }
			  }
			  else if (iord[0]=='d')
				  if (!GetMax(keys[j][p],keys[j+1][p])) 
				  {
 						for (f=0;f<n1;f++)
						{                
						  swap(indices[j],indices[j+1]);
						  swap(keys[j][f],keys[j+1][f]);			
						}
				  }
		   }
		  for (i=1;i<=n1;i++)
			  for (j=1;j<=m1;j++)  keys1[(i-1)*m1+j-1]=keys[j-1][i-1];
	}
	if (typex[1]=='c') 
	{
		for (i=0;i<n1;i++)  indices[i]=i+1;
		for (i=1;i<=n1;i++)
			for (j=1;j<=m1;j++)  keys[i-1][j-1]=keys1[(i-1)*m1+j-1];
		for(i=n1-1;i>0;i--)
		  for(j=0;j<i;j++)
		  {
			  for (p=0;keys[j][p]==keys[j+1][p] ;p++);
			  if (iord[0]=='i')
			  {
				  if (GetMax(keys[j][p],keys[j+1][p])) 
				  {
 					  swap(indices[j],indices[j+1]);	
					  for (f=0;f<m1;f++)
						{                
						  swap(keys[j][f],keys[j+1][f]);			
						}
				  }
			  }
			  else if (iord[0]=='d')
				  if (!GetMax(keys[j][p],keys[j+1][p])) 
				  {
					  swap(indices[j],indices[j+1]);
					  for (f=0;f<m1;f++)
						{                
						  swap(keys[j][f],keys[j+1][f]);			
						}
				  }
		  }
		  for (i=1;i<=n1;i++)
			  for (j=1;j<=m1;j++)  keys1[(i-1)*m1+j-1]=keys[i-1][j-1];
	}
	
}
/*------------------------------------------------------------------------------------------*/
template <class T>
void mutirowsort (T *keys1,int indices[20],int m1,int n1,char *typex, char *iord)

{
    int i,j,p;
    T keys[20][20];
	int Xindices[20][20];
	if (typex[0]=='r') 
	{
	    for (i=0;i<m1;i++)
		   for (j=0;j<n1;j++) Xindices[i][j]=i+1;
		for (i=1;i<=n1;i++)
		   for (j=1;j<=m1;j++)  keys[j-1][i-1]=keys1[(i-1)*m1+j-1];
		for (p=0;p<n1;p++) 
	    { 
			for(i=m1-1;i>0;i--)
			  for(j=0;j<i;j++)
			  {
				  if (iord[0]=='i')
				  {
					  if (GetMax(keys[j][p],keys[j+1][p])) 
					  {
							  swap(Xindices[j][p],Xindices[j+1][p]);
							  swap(keys[j][p],keys[j+1][p]);			
					  }
				  }
				  else if (iord[0]=='d')
					  if (!GetMax(keys[j][p],keys[j+1][p])) 
					  {
							  swap(Xindices[j][p],Xindices[j+1][p]);
							  swap(keys[j][p],keys[j+1][p]);			
					  }
			   }
		}
		  for (i=1;i<=n1;i++)
			  for (j=1;j<=m1;j++)
			  {
				  keys1[(i-1)*m1+j-1]=keys[j-1][i-1];
				  indices[(i-1)*m1+j-1]=Xindices[j-1][i-1];
			  }   
	}
	if (typex[0]=='c') 
	{
		 for (i=0;i<m1;i++)
		   for (j=0;j<n1;j++) Xindices[j][i]=j+1;
		for (i=1;i<=n1;i++)
			for (j=1;j<=m1;j++)  keys[i-1][j-1]=keys1[(i-1)*m1+j-1];
		for (p=0;p<m1;p++) 
	    { 
			for(i=n1-1;i>0;i--)
			  for(j=0;j<i;j++)
			  {
				  if (iord[0]=='i')
				  {
					  if (GetMax(keys[j][p],keys[j+1][p])) 
					  {
							  swap(Xindices[j][p],Xindices[j+1][p]);
							  swap(keys[j][p],keys[j+1][p]);			
					  }
				  }
				  else if (iord[0]=='d')
					  if (!GetMax(keys[j][p],keys[j+1][p])) 
					  {
							  swap(Xindices[j][p],Xindices[j+1][p]);
							  swap(keys[j][p],keys[j+1][p]);			
					  }
			   }
		}
		  for (i=1;i<=n1;i++)
			  for (j=1;j<=m1;j++) 
			  {
				  keys1[(i-1)*m1+j-1]=keys[i-1][j-1];
				  indices[(i-1)*m1+j-1]=Xindices[i-1][j-1];
			  }
	}
	
}
/*------------------------------------------------------------------------------------------*/
template <class T>
void wholesort (T *keys,int indices[20],int m1,int n1,char *typex, char *iord)

{
    int i,j,k;
	k=0;
    for (i=1;i<=m1;i++)
    for (j=1;j<=n1;j++) indices[k++]=(i-1)*n1+j;
	for(i=m1*n1-1;i>0;i--)
	  for(j=0;j<i;j++)
	  {
		  if (iord[0]=='i')
		  {
			  if (GetMax(keys[j],keys[j+1])) 
			  {
					  swap(indices[j],indices[j+1]);
					  swap(keys[j],keys[j+1]);			
			  }
		  }
		  else if (iord[0]=='d')
			  if (!GetMax(keys[j],keys[j+1])) 
			  {
					  swap(indices[j],indices[j+1]);
					  swap(keys[j],keys[j+1]);			
			  }
	   }
	
}
/*------------------------------------------------------------------------------------------*/
void lgsortd(double *keys1,int indices[20],int m1,int n1,char *typex, char *iord)
{
  mutisort (keys1,indices,m1,n1,typex,iord);
}
/*------------------------------------------------------------------------------------------*/

void lgsorts1(char *keys1,int indices[20],int m1,int n1,char *typex, char *iord)
{
  mutisort (keys1,indices,m1,n1,typex,iord);
}
/*------------------------------------------------------------------------------------------*/
void rowcolsortd(double *keys1, int indices[20], int m1, int n1, char *typex, char *iord)
{
     mutirowsort(keys1,indices,m1,n1,typex,iord);
}
/*------------------------------------------------------------------------------------------*/
void wholesortd(double *keys1, int indices[20], int m1, int n1, char *typex, char *iord)
{
     wholesort(keys1,indices,m1,n1,typex,iord);
}
/*------------------------------------------------------------------------------------------*/
void wholesorts(char **keys, int indices[20], int m1, int n1, char *typex, char *iord)
{
    int i,j,k,p,f;
	k=0;
    for (i=1;i<=m1;i++)
      for (j=1;j<=n1;j++) indices[k++]=(i-1)*n1+j;
	for(i=m1*n1-1;i>0;i--)
	  for(j=0;j<i;j++)
	  {
		  for (p=0;keys[j][p]==keys[j+1][p] ;p++);
		  if (iord[0]=='i')
		  {
			  if (GetMax(keys[j][p],keys[j+1][p])) 
			  {
					  swap(indices[j],indices[j+1]);
					  for (f=0;f<max(strlen(keys[j]),strlen(keys[j+1]));f++)
						{                
						  swap(keys[j][f],keys[j+1][f]);			
						}
			
			  }
		  }
		  else if (iord[0]=='d')
			  if (!GetMax(keys[j][p],keys[j+1][p])) 
			  {
					  swap(indices[j],indices[j+1]);
					  for (f=0;f<max(strlen(keys[j]),strlen(keys[j+1]));f++)
						{                
						  swap(keys[j][f],keys[j+1][f]);			
						}
			
			  }
	   }








}
/*------------------------------------------------------------------------------------------*/
void rowcolsorts(char *keys1, int indices[20], int m1, int n1, char *typex, char *iord)
{
     mutirowsort(keys1,indices,m1,n1,typex,iord);
}
/*------------------------------------------------------------------------------------------*/