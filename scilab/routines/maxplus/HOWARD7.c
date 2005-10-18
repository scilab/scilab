/* Howard Policy Iteration Algorithm for Max Plus Matrices

 Dec 22, 1997
 Stephane.Gaubert@inria.fr

 The function is documented in the manual page Howard.help

 The max-plus version of Howard's algorithm is described
 in the paper: 
  Numerical computation of spectral elements in max-plus algebra
            Jean Cochet-Terrasson, Guy Cohen, Stephane Gaubert,
	    Michael Mc Gettrick, Jean-Pierre Quadrat
  (to appear in the IFAC Workshop on System Structure and Control,
  Nantes, July 1997).
 */

/* 

   Howard will terminate with an error if MAX_NIterations 
   occur.
   (on all pratical random examples, even for matrices of large size
   - we tried for matrices of size <64Mb -,
   the algorithm requires "at most" 80 iterations).

*/

#define MAX_NIterations 1000

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "Display2.h"

#define EPSILON -HUGE_VAL
/*#define EPSILON  -1.79769313486231570e308*/


/* GLOBAL VARIABLES */

static int *ij;
static double *a;
static double *t;
static int nnodes;
static int narcs;
static double *chi;
static double *v;
static int *pi;
static int *NIterations;
static int *NComponents;
static int verbosemode;
static int ERRORTYPE;

/****************************************************************/

static int *newpi=NULL; /*  new policy */
/* the inverse policy is coded by a linearly chained list */
static 	int *piinv_idx=NULL; /* piinv_idx[i]= pointer to the chain of inverses
				of node i */
static	int *piinv_succ=NULL;/* piinv_idx[j]= pointer to the next inverse */
static	int *piinv_elem=NULL;/* corresponding  node  */
static	int *piinv_last=NULL;/* piinv_last[i]= last inverse of i */
static	double *c=NULL;
static  double *tau=NULL;
static  double *newtau=NULL;
static	double *vaux=NULL;
static	double *newc=NULL;
static	double *newchi=NULL;
static 	int *visited=NULL;
static	int *component=NULL;
static  double lambda=0;
static  double epsilon=0;
static  int color=1;


/* The termination tests are performed up to an epsilon
constant, which is fixed heuristically by the following routine
*/

void Epsilon(double *a,int narcs,double *epsilon)
{ 
  int i;
  double MAX,MIN;
  MAX=a[0];
  MIN=a[0];
  for (i=1;i<narcs; i++)
    {
      if (a[i]> MAX)
	{
	  MAX=a[i];
	}
      if (a[i]<MIN)
	{
	  MIN=a[i];
	}
    }
  /* this value is somehow arbitrary. */
   *epsilon=(MAX-MIN+1)*0.000000001;
   if (verbosemode>1)
     {
       printf("MAX of coefs =%f MIN of coefs=%f MAX-MIN= %f epsilon=%f\n",MAX,MIN,MAX-MIN, *epsilon);
     }
}

/* Build an admissible policy pi and its associated cost vector c */
/* from ij  and A */

/* reasonable greedy rule to determine the first policy. 
  pi(node i) = arc with maximal weigth starting from i 
  for full random matrices, this choice of initial policy seems
  to cut the number of iterations by a factor 1.5, by comparison
  with a random initial policy */ 

static void Initial_Policy()
{ int i;  
  /* we loose a O(nnodes) time here ... */
  /* we use the auxiliary variable vaux to compute the row max of A */ 
  for (i=0; i< nnodes; i++) 
    {
      vaux[i] = EPSILON;
    }
  for (i=0; i<narcs; i++)
    {
      if (vaux[ij[i*2]] <= a[i]) 
	{
       pi[ij[i*2]] =  ij[i*2+1];
       c[ij[i*2]] = a[i];
       vaux[ij[i*2]] = a[i];
     }
    }
}

/* idem : semi Markov case */

static void Semi_Initial_Policy()
{ int i;  
  /* we loose a O(nnodes) time here ... */
  /* heuristic rule to determine the initial policy... A */ 
  for (i=0; i< nnodes; i++) 
    {
      vaux[i] = EPSILON;
    }
  for (i=0; i<narcs; i++)
    {
      if (t[i]>0)
	{ 
	  if (vaux[ij[i*2]]<= a[i]/t[i] ) 
	    { 
	      pi[ij[i*2]] =  ij[i*2+1];
	      c[ij[i*2]] = a[i];
	      tau[ij[i*2]] =t[i];
	      vaux[ij[i*2]] = a[i]/t[i];
	    }
	}
      else
	/* degenerate case */
	{ 
	  if (vaux[ij[i*2]]<= a[i]){
	  pi[ij[i*2]] =  ij[i*2+1];
	  c[ij[i*2]] = a[i];
	  tau[ij[i*2]] =t[i];
	  vaux[ij[i*2]] = a[i];
	  }
	}
    }
}

static void New_Display_Inverse()
{ 
 int i,a;
 printf("DISPLAYING INVERSE nnodes=%d\n",nnodes);
 for (i=0; i<nnodes; i++)
   { a=piinv_idx[i];
     while (a != -1)
       {
	 printf("inverse of %d= %d\n",i,piinv_elem[a]);
	 a=piinv_succ[a];
       }
   }
}

static void New_Build_Inverse()
{ int i,j,locus;
  int ptr=0;
 if (verbosemode >1 )
    {
      printf("BUILDING INVERSE\n");
    }
 for (i=0;i<nnodes;i++)
   {
     piinv_idx[i] =-1;
     piinv_last[i]=-1;
   }
 for (i=0;i<nnodes;i++)
   {
     j=pi[i];
     if (piinv_idx[j]==-1) 
       {
	 piinv_succ[ptr]=-1;
	 piinv_elem[ptr]=i;
	 piinv_last[j]=ptr;
	 piinv_idx[j]=ptr;
	 ptr++;
       }
     else
       {
       piinv_succ[ptr]=-1;
       piinv_elem[ptr]=i;
       locus=piinv_last[j];
       piinv_succ[locus]=ptr;
       piinv_last[j]=ptr;
       ptr++;
       };
   }
 if (verbosemode >1)
   {
     New_Display_Inverse();
     printf("INVERSE OK\n");
   }
}

static void Init_Depth_First()
{
  int j;
  for (j=0;j<nnodes;j++)
    {
      visited[j]=0;
      component[j]=0;
    }
}


/* Given the value of v at initial point i, we compute v[j] */
/* for all predecessor j of i, according to the spectral equation */
/* v[j]+ lambda = A(arc from j to i) + v[i] */
/* the array visited is changed by side effect */


static void New_Depth_First_Label(int i)
{ 
  int nexti,a;
  a=piinv_idx[i];
  while (a != -1 && visited[piinv_elem[a]]==0)
    {
      nexti=piinv_elem[a];
      visited[nexti]=1;
      v[nexti]= -lambda + c[nexti]+ v[i];
      component[nexti]=color;
      chi[nexti]= lambda;
      New_Depth_First_Label(nexti);
      a=piinv_succ[a];
    }
}
static void Semi_New_Depth_First_Label(int i)
{ 
  int nexti,a;
  a=piinv_idx[i];
  while (a != -1 && visited[piinv_elem[a]]==0)
    {
      nexti=piinv_elem[a];
      visited[nexti]=1;
      v[nexti]= -lambda*tau[nexti] + c[nexti]+ v[i];
      component[nexti]=color;
      chi[nexti]= lambda;
      Semi_New_Depth_First_Label(nexti);
      a=piinv_succ[a];
    }
}


static void Visit_From(int initialpoint,int color)
{ int index,newindex,i;
  double weight;
  int length;
  if (verbosemode>1)
    {
      printf("visiting from node %d color=%d\n",initialpoint, color);
    }
  index=initialpoint;
  component[index]=color;
  newindex=pi[index];
  while (component[newindex]==0)
    {
      component[newindex]=color;
      index=newindex;
      newindex=pi[index];
    }
  /* a cycle has been detected, since newindex is already visited */ 
  weight=0;
  length=0;
  i=index;
  do
    {
      weight+=c[i];
      length++;
      i=pi[i];
    }
  while (i !=index);
  lambda=weight/length;
  v[i]=vaux[i]; /* keeping the previous value */
  chi[i]=lambda;
  New_Depth_First_Label(index);
}


static int Semi_Visit_From(int initialpoint,int color)
{ int index,newindex,i;
  double weight;
  double glength;
  if (verbosemode>1)
    {
      printf("visiting from node %d color=%d\n",initialpoint, color);
    }
  index=initialpoint;
  component[index]=color;
  newindex=pi[index];
  while (component[newindex]==0)
    {
      component[newindex]=color;
      index=newindex;
      newindex=pi[index];
    }
  /* a cycle has been detected, since newindex is already visited */ 
  weight=0;
  glength=0;
  i=index;
  do
    { 
      weight+=c[i];
      glength+=tau[i];
      i=pi[i];
    }
  while (i !=index);
  if (glength<= 0)
    {
      if (verbosemode >0)
	{
	  printf("ERROR: cycle with non-positive weight %g found at node %d\n",glength, i);
	  printf("CURRENT POLICY: \n");
	  Display_Semi_Policy(pi,c,tau,nnodes);
	}
      return(2);
    }
  lambda=weight/glength;
  v[i]=vaux[i]; /* keeping the previous value */
  chi[i]=lambda;
  Semi_New_Depth_First_Label(index);
  return(0);
}


/*       Value() */
/* Computes the value (v,chi) associated with a policy pi */

static void Value()
{ int initialpoint;
  color=1;
  if (verbosemode>0)
    {
      printf("Computing the value\n");
    }
  Init_Depth_First();
  initialpoint=0;
  do
    { 
      Visit_From(initialpoint,color);
      while ((initialpoint<nnodes) && (component[initialpoint] !=0))
	{ 
	  initialpoint++;
	}
      color++;
    }
  while (initialpoint<nnodes);
  *NComponents=--color;
  if (verbosemode>0)
    {
      printf("Value OK\n");
    }
}


/*       Value()  (Semi-Markov case) */
/* Computes the value (v,chi) associated with a policy pi */

static int Semi_Value()
{ int initialpoint;
  color=1;
  if (verbosemode>0)
    {
      printf("Computing the value\n");
    }
  Init_Depth_First();
  initialpoint=0;
  do
    { 
      if (Semi_Visit_From(initialpoint,color)!=0)
	{
	  return(2);
	}
      while ((initialpoint<nnodes) && (component[initialpoint] !=0))
	{ 
	  initialpoint++;
	}
      color++;
    }
  while (initialpoint<nnodes);
  *NComponents=--color;
  if (verbosemode>0)
    {
      printf("Value OK\n");
    }
  return(0);
}


static void Show_Info_Improve_Chi(int i)
{
int I,J;
/*printf("type 1 improvement\n");*/
if (verbosemode>0)
  {
  I=ij[i*2];
  J=ij[i*2+1];
  printf("Improvement of the cycle time at node %d\n",I);
  printf("arc %d: %d--->%d chi[%d]-chi[%d]=%f-%f=%g>0\n",i,I,J,I,J,chi[J],chi[I],chi[J]-chi[I]);
  }
}

static void Show_Info_Improve_Bias(int i)
{
int I,J;
/*printf("type 2 improvement\n");*/
if (verbosemode>0)
  {
  I=ij[i*2];
  J=ij[i*2+1];
  printf("Improvement of the BIAS at node %d\n",I);
  printf("A[%d]+v[%d] - chi[%d]-v[%d]= %f + %f -%f -%f =%f >0\n",i,J,I,I,a[i],v[J],chi[I],v[I],a[i]+v[J]-chi[I]-v[I]);
  }
}

static void Semi_Show_Info_Improve_Bias(int i)
{
int I,J;
/*printf("type 2 improvement\n");*/
if (verbosemode>0)
  {
  I=ij[i*2];
  J=ij[i*2+1];
  printf("Improvement of the BIAS at node %d\n",I);
  printf("A[%d]+v[%d] - chi[%d]*t[%d]-v[%d]= %f + %f -%f*%f -%f =%f >0\n",i,J,J,J,I,a[i],v[J],chi[I],t[i],v[I],a[i]+v[J]-chi[I]*t[i]-v[I]);
  }
}

static void Init_Improve()
{
  int i;
  for (i=0;i<nnodes; i++)
    {
      newchi[i]=chi[i];
      vaux[i]=v[i];
      newpi[i]=pi[i];
      newc[i]=c[i];
    }
}

static void Semi_Init_Improve()
{
  int i;
  for (i=0;i<nnodes; i++)
    {
      newchi[i]=chi[i];
      vaux[i]=v[i];
      newpi[i]=pi[i];
      newc[i]=c[i];
      newtau[i]=tau[i];
    }
}

static void First_Order_Improvement(int *improved)
{ int i;
  for (i=0;i<narcs; i++)
    {
      if (chi[ij[i*2+1]]>newchi[ij[i*2]])
	{ 
	  Show_Info_Improve_Chi(i);
	  *improved=1;
	  newpi[ij[i*2]]=ij[i*2+1];
	  newchi[ij[i*2]]=chi[ij[i*2+1]];
	  newc[ij[i*2]]=a[i];
	}
    }
}
static void Semi_First_Order_Improvement(int *improved)
{ int i;
  for (i=0;i<narcs; i++)
    {
      if (chi[ij[i*2+1]]>newchi[ij[i*2]])
	{ 
	  Show_Info_Improve_Chi(i);
	  *improved=1;
	  newpi[ij[i*2]]=ij[i*2+1];
	  newchi[ij[i*2]]=chi[ij[i*2+1]];
	  newc[ij[i*2]]=a[i];
	  newtau[ij[i*2]]=t[i];
	}
    }
}

static void Second_Order_Improvement(int *improved)
{ int i;     
  double w;
 if (*NComponents >1) /* a bit more complicated */
   {
     for (i=0;i<narcs; i++)
	{
	  if (chi[ij[i*2+1]]==newchi[ij[i*2]])
	  /* arc i is critical */
	{
	  w=a[i]+ v[ij[i*2+1]] - chi[ij[i*2]];
	  if (w>vaux[ij[i*2]] + epsilon)
	    {
	      Show_Info_Improve_Bias(i);
	      *improved=1;
	      vaux[ij[i*2]]=w;
	      newpi[ij[i*2]]=ij[i*2+1];
	      newc[ij[i*2]]=a[i];
	    }
	}
	}
   }
 else /* we know that all the arcs realize the max in the 
	 first order improvement */
   {
     for (i=0;i<narcs; i++)
	{
	  w=a[i]+ v[ij[i*2+1]] - chi[ij[i*2]];
	  if (w>vaux[ij[i*2]] + epsilon)
	    {
	      Show_Info_Improve_Bias(i);
	      *improved=1;
	      vaux[ij[i*2]]=w;
	      newpi[ij[i*2]]=ij[i*2+1];
	      newc[ij[i*2]]=a[i];
	    }
	}
   }
}

static void Semi_Second_Order_Improvement(int *improved)
{ int i;     
  double w;
 if (*NComponents >1) /* a bit more complicated */
   {
     for (i=0;i<narcs; i++)
	{
	  if (chi[ij[i*2+1]]==newchi[ij[i*2]])
	  /* arc i is critical */
	{
	  w=a[i]+ v[ij[i*2+1]] - chi[ij[i*2+1]]*t[i];
	  if (w>vaux[ij[i*2]] + epsilon)
	    {
	      Semi_Show_Info_Improve_Bias(i);
	      *improved=1;
	      vaux[ij[i*2]]=w;
	      newpi[ij[i*2]]=ij[i*2+1];
	      newc[ij[i*2]]=a[i];
	      newtau[ij[i*2]]=t[i];
	    }
	}
	}
   }
 else /* we know that all the arcs realize the max in the 
	 first order improvement */
   {
     for (i=0;i<narcs; i++)
	{
	  w=a[i]+ v[ij[i*2+1]] - chi[ij[i*2+1]]*t[i];
	  if (w>vaux[ij[i*2]] + epsilon)
	    {
	      Semi_Show_Info_Improve_Bias(i);
	      *improved=1;
	      vaux[ij[i*2]]=w;
	      newpi[ij[i*2]]=ij[i*2+1];
	      newc[ij[i*2]]=a[i];
	      newtau[ij[i*2]]=t[i];
	    }
	}
   }
}

static void Improve(int *improved)
{
  *improved=0;
  Init_Improve();
  if (*NComponents>1) /* a first order policy improvement may occur */
    First_Order_Improvement(improved);
  if (*improved ==0)
    {
      Second_Order_Improvement(improved);
    }
}

/* semi Markov variant */
static void Semi_Improve(int *improved)
{
  *improved=0;
  Semi_Init_Improve();
  if (*NComponents>1) /* a first order policy improvement may occur */
    Semi_First_Order_Improvement(improved);
  if (*improved ==0)
    {
      Semi_Second_Order_Improvement(improved);
    }
}


static void Allocate_Memory()
{
  newpi=(int *)calloc(nnodes, sizeof(int));
  piinv_idx=(int *)calloc(nnodes, sizeof(int));
  piinv_succ=(int *)calloc(nnodes, sizeof(int));
  piinv_elem=(int *)calloc(nnodes, sizeof(int));
  piinv_last=(int *)calloc(nnodes, sizeof(int));
  visited=(int *)calloc(nnodes, sizeof(int));
  component=(int *)calloc(nnodes, sizeof(int));
  c=(double *)calloc(nnodes, sizeof(double));
  newc=(double *)calloc(nnodes, sizeof(double));
  vaux=(double *)calloc(nnodes, sizeof(double));
  newchi=(double *)calloc(nnodes, sizeof(double));
  if ((newchi==NULL)|| (vaux==NULL) || (newc==NULL)|| (c==NULL)|| (component==NULL) || (visited==NULL)|| (piinv_idx==NULL) ||(piinv_succ== NULL) ||( piinv_elem==NULL)|| (  piinv_last==NULL) || (newpi==NULL))
    {
      printf("error in Howard... memory allocation failed...\n");
      exit(1);
    }
}

static void Semi_Allocate_Memory()
{
  Allocate_Memory();
  tau=(double *)calloc(nnodes, sizeof(double));
  newtau=(double *)calloc(nnodes, sizeof(double));
  if ((tau==NULL)|| (newtau==NULL))
    {
      printf("error in Howard... memory allocation failed...\n");
      exit(1);
    }
}

static void Free_Memory()
{
  free(newpi);
  free(piinv_idx);
  free(piinv_succ);
  free(piinv_elem);
  free(piinv_last);
  free(visited);
  free(component);
  free(c);
  free(newc);
  free(vaux);
  free(newchi);
}

static void Semi_Free_Memory()
{
  free(tau);
  free(newtau);
  Free_Memory();
 }



void Show_Info()
{
  if (verbosemode >0)
    {
      printf("verbosemode=%d",verbosemode);
      printf("*** ITERATION %d of Max Plus Howard Algorithm *** \n",*NIterations);
      Display_Policy(pi,c,nnodes);
      printf("vector chi=\n");
      Display_Vector(nnodes,chi);
      printf("vector v=\n");
      Display_Vector(nnodes,v);
    }
}

void Semi_Show_Info()
{
  if (verbosemode >0)
    {
      printf("verbosemode=%d",verbosemode);
      printf("*** ITERATION %d of Max Plus Howard Algorithm *** \n",*NIterations);
      Display_Semi_Policy(pi,c,tau,nnodes);
      printf("vector chi=\n");
      Display_Vector(nnodes,chi);
      printf("vector v=\n");
      Display_Vector(nnodes,v);
    }
}


static int Check_Rows()
{ 
  int i;
  int *u=NULL;
  u=(int *)calloc(nnodes,sizeof(int));
  for (i=0; i<narcs;i++ )
    {
       u[ij[2*i]]=1;
    }
  for (i=0; i<nnodes;i++ )
    {
     if (u[i]==0)
       {
	 if (verbosemode>0)
	   {
	     printf("ERROR : node numbered %d has no sucessor (recall that nodes are internally numbered from 0)\n",i);
	   }
	 free(u);
	 return(1);
       }
    }
 free(u);
 return(0);
}

static int Universal_Security_Check()
{
   if (nnodes<1)
    {
      if (verbosemode>0)
	{
	printf("ERROR: nnodes must be a positive integer\n");
	}
      return(3);
    }
  if (narcs<1)
    { if (verbosemode>0)
      {
	printf("ERROR: narcs must be a positive integer\n");
      }
      return(4);
    }
  if (verbosemode >-1)
    {
      return(Check_Rows());
    }
  else
     {
       return(0);
     }
 }

static int Security_Check()
{ int errorflag;
  errorflag=Universal_Security_Check();
  if (verbosemode >0)
     {
       Display_Sparse_Matrix(narcs,ij,a);

     }
  return(errorflag);
}

static int Semi_Security_Check()
{
  int errorflag;
  errorflag=Universal_Security_Check();
  if (verbosemode >0)
     {
       Display_Semi_Sparse_Matrix(narcs,ij,a,t);
     }
  return(errorflag);
}

void Import_Arguments(int *IJ, double *A,int NNODES, int NARCS, double *CHI, double *V, int *POLICY, int *NITERATIONS, int *NCOMPONENTS, int VERBOSEMODE)
{
ij=IJ;
a=A;
nnodes=NNODES;
narcs=NARCS;
chi=CHI;
v=V;
pi=POLICY;
NIterations=NITERATIONS;
NComponents=NCOMPONENTS;
verbosemode=VERBOSEMODE;
}

void Semi_Import_Arguments(int *IJ, double *A,double *T,int NNODES, int NARCS, double *CHI, double *V, int *POLICY, int *NITERATIONS, int *NCOMPONENTS, int VERBOSEMODE)
{
ij=IJ;
a=A;
t=T;
nnodes=NNODES;
narcs=NARCS;
chi=CHI;
v=V;
pi=POLICY;
NIterations=NITERATIONS;
NComponents=NCOMPONENTS;
verbosemode=VERBOSEMODE;
}

static void Update_Policy()
{
register int i;
      for (i=0;i<nnodes;i++)
	   {
	     pi[i]=newpi[i];
    	     c[i]=newc[i];
	     vaux[i]=v[i]; /* We need a to keep a copy of the current value function */
	   }
}

static void Semi_Update_Policy()
{
register int i;
      for (i=0;i<nnodes;i++)
	   {
	     pi[i]=newpi[i];
    	     c[i]=newc[i];
	     tau[i]=newtau[i];
	     vaux[i]=v[i]; /* We need a to keep a copy of the current value function */
	   }
}

static int End_Message()
{
  if (*NIterations ==MAX_NIterations)
    {
      printf("ERROR : maximal number of iterations (=%d) reached\n",MAX_NIterations);
      printf("This should not happen (usual number of iterations is <80,\n");
      printf("the worst case number of iterations is expected to be of order nnodes\n");
      printf("If you do expect a large number of iterations, you may change\n");
      printf("manually the constant MAX_NIterations in HOWARD?.c\n");
      printf("If you suspect an error, you may wish to rerun the program with the flag verbosemode >0 to trace what happens\n");
      printf("Bug reports should be sent to Stephane.Gaubert@inria.fr\n");
      return(5);
    }
  if (verbosemode >1)
     {
       printf("END OF HOWARD: OK\n");
     }
  return(0);
}



/* INPUT of Howard Algorithm =
   ij,A,nnodes,narcs : sparse description of a matrix 
   verbosemode: normal value is zero
   higher values (1,2) yield more info for debugging purposes.
   Value -1 of verbosemode suppresses the checking of consistency 
   of data.
   
   OUTPUT =
   chi cycle time vector
   v bias
   pi optimal policy
   NIterations: Number of iterations of the algorithm
   NComponents: Number of connected components of the optimal policy

   REQUIRES: O(nnodes) SPACE
   One iteration requires: O(narcs+nnodes) TIME
   
   Experimentally, the number of iterations N_H(nnodes,...) seems to grow
   slowy with the dimension, something like 
   N_H(nnodes)=O(log(nnodes)) for full matrices.
   The matrix A must have at least one finite entry
   per row (this is checked when verbosemode is >=1)

   */


/*  
The following variables should be defined in the environment
from which the Howard routine is called.

INPUT VARIABLES
int NNODES;   
              number of nodes of the graph 
int NARCS;    
              number of arcs of the graph 
int *IJ; 
              array of integers of size 2*narcs 
       
	      for (0 <=k <narcs), the arc numbered k  goes from 
	      IJ[k][0] =(IJ[2k]) to IJ[k][1] (=IJ[2k+1])

double *A; 
              array of double of size narcs
	      A[k]=weight of the arc numbered k 

OUTPUT VARIABLES

double *V;
               array of double of size nnodes (the bias vector)
double *CHI;
               array of double of size nnodes (the cycle time vector)
int *POLICY;       
               array of integer of size nnodes (an optimal policy)
int NITERATIONS; 
               integer: the number of iterations of the algorithm

int NCOMPONENTS; 
               integer: the number of connected components of the optimal
	       policy which is returned.
*/

int Howard(int *IJ, double *A,int NNODES,int NARCS,double *CHI,double *V,int *POLICY,int *NITERATIONS,int *NCOMPONENTS,int VERBOSEMODE)
{
  int improved=0;
  *NITERATIONS=0;
  Import_Arguments(IJ,A,NNODES,NARCS,CHI,V,POLICY,NITERATIONS,NCOMPONENTS,VERBOSEMODE);
  ERRORTYPE=Security_Check();
  if (ERRORTYPE !=0)
    {
      return(ERRORTYPE);
    }
  Allocate_Memory();
  Epsilon(a,narcs,&epsilon);
  Initial_Policy();
  New_Build_Inverse();
  do
    { 
       Value();
       Show_Info();
       Improve(&improved);
       Update_Policy();
       New_Build_Inverse();
       (*NIterations)++;
     }
  while ((improved != 0) && *NIterations <MAX_NIterations);
  Free_Memory();
  ERRORTYPE=End_Message();
  return(ERRORTYPE);
}

/*  Input of Semi_Howard is identical, plus 
    double *T: array of double of size narcs (delays).
*/
int Semi_Howard(int *IJ, double *A,double *T,int NNODES,int NARCS,double *CHI,double *V,int *POLICY,int *NITERATIONS,int *NCOMPONENTS,int VERBOSEMODE)
{
  int improved=0;
  *NITERATIONS=0;
  Semi_Import_Arguments(IJ,A,T,NNODES,NARCS,CHI,V,POLICY,NITERATIONS,NCOMPONENTS,VERBOSEMODE);
  ERRORTYPE=Semi_Security_Check();
  if(ERRORTYPE !=0)
    {
      return(ERRORTYPE);
    }
  Semi_Allocate_Memory();
  Epsilon(a,narcs,&epsilon);
  Semi_Initial_Policy();
  New_Build_Inverse();
  do
    {  
       if(Semi_Value()!=0)
	 {
	   return(2);
	 }
       Semi_Show_Info();
       Semi_Improve(&improved);
       Semi_Update_Policy();
       New_Build_Inverse();
       (*NIterations)++;
     }
  while ((improved != 0) && *NIterations <MAX_NIterations);
  Semi_Free_Memory();
  ERRORTYPE=End_Message();
  return(ERRORTYPE);
}

