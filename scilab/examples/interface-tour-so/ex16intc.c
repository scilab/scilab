#include  "stack-c.h"

#define CreateRefFromName(n,nx) if(! C2F(createreffromname)(n,nx)){return 0;}

int intex16c(char* fname)
{ 
  int *header;
  int l3,m3,n3;
  CheckRhs(1,1);    CheckLhs(1,4);
  GetRhsVar(1,"d", &m, &n, &l3);
  CreateRefFromName(2,"param");
  CreateVar(3, "d",(m3=2, &m3), (n3=3, &n3), &l3);
  stk(l3)[0]=11;stk(l3)[1]=21;stk(l3)[2]=12;
  stk(l3)[3]=22;stk(l3)[4]=13;stk(l3)[5]=23;
  CreateRef(4,3);
  LhsVar(1)=2;  LhsVar(2)=3;  LhsVar(3)=1;  LhsVar(4)=4;
  return 0;
}

void *GetRawData(int lw)
     /* same as GetData BUT does not go to the pointed variable if lw is a reference */
{
  int lw1 = lw + Top - Rhs ;
  int l1 = *lstk(lw1);
  int *loci = (int *) stk(l1);
  C2F(intersci).ntypes[lw - 1] = '$';
  C2F(intersci).iwhere[lw - 1] = l1;
  return loci;
}

int CreateRef(int number,int  pointed)
/* variable number is created as a reference to variable pointed */
{
  int offset; int point_ed; int *header;
  if (pointed > number) return 0;   /* The variable created (number) should point to an existing variable */
  CreateData( number, 4*sizeof(int) );
  header =  GetRawData(number);
  offset = Top -Rhs;
  point_ed = offset + pointed;
  header[0]= - *istk( iadr(*lstk( point_ed )) );  /* reference : 1st entry (type) is opposite */
  header[1]= *lstk(point_ed);  /* pointed adress */
  header[2]= point_ed; /* pointed variable */
  header[3]= *lstk(point_ed + 1)- *lstk(point_ed);  /* size of pointed variable */
  C2F(intersci).ntypes[number-1]= '-';
  return 1;
}

int C2F(createreffromname)(number, name)
     int number; char *name;
     /* variable number is created as a reference pointing to variable "name" */
     /* name must be an existing Scilab variable */
{
  int *header; int lw; int fin;
  CreateData(number, 4*sizeof(int));
  header = (int *) GetData(number);
  if (C2F(objptr)(name,&lw,&fin,strlen(name))) {
    header[0]= - *istk( iadr(*lstk(fin))); /* type of reference = - type of pointed variable */
    header[1]= lw; /* pointed adress */
    header[2]= fin; /* pointed variable */
    header[3]= *lstk(fin+1)- *lstk(fin);  /*size of pointed variable */
    return 1;
  }
  else
    {  
      Scierror(999,"CreateRefFromName: variable %s not found\r\n",name);
      return 0;
    }
}

void *GetDataFromName( char *name)
     /* usage:  header = (int *) GetDataFromName("pipo"); header[0] = type of variable pipo etc... */
{
  void *header; int lw; int fin;
 if (C2F(objptr)(name,&lw,&fin,strlen(name))) {
    header = istk( iadr(*lstk(fin)));  
    return (void *) header;
  }
 else
    {  
      Scierror(999,"GetDataFromName: variable %s not found\r\n",name);
      return (void *) 0;
    }
}
