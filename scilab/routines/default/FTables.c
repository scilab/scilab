/**************************************
 * Copyright Jean-Philippe Chancelier 
 * ENPC 
 **************************************/

#include <string.h>
#include "FTables0.h"
#include "FTables.h"
#define FTable_H   /* to prevent  a type conflict with GetFuncPtr */ 
#include "../stack-c.h"
#include "../sun/link.h"

extern int C2F(getcodc) __PARAMS((integer *nd1, integer *iflag1));
static int SearchComp  __PARAMS((FTAB *Ftab, char *op, void (**realop) ( )));  
static void Emptyfunc  __PARAMS((void)) {} ;

voidf SetFunction  __PARAMS((char *name, int *rep, FTAB *table));  

/***********************************
 * Search Table for feval 
 ***********************************/

/** the current function fixed by setfeval **/

static ffevalf fevalfonc ;

/** function call **/

void C2F(ffeval)(nn,x1,x2,xres,itype,name) 
     integer *nn,*itype;
     double *x1,*x2,*xres;
     char *name;
{
  (*fevalfonc)(nn,x1,x2,xres,itype,name);
}

/** fixes the function associated to name **/

void C2F(setfeval)(name,rep)
     char *name;
     int *rep;
{
  fevalfonc = (ffevalf) SetFunction(name,rep,FTab_ffeval);
}


/***********************************
 * Search Table for fydot 
 ***********************************/

/** the current function fixed by setfydot **/

static fydotf fydotfonc ;

/** function call **/

void C2F(fydot)(n,t,y,ydot)
     integer *n;
     double *t,*y,*ydot;
{
  (*fydotfonc)(n,t,y,ydot);
}

/** fixes the function associated to name **/

void C2F(setfydot)(name,rep)
     char *name;
     int *rep;
{
  fydotfonc = (fydotf) SetFunction(name,rep,FTab_fydot);
}


/** the current function fixed by setfjac **/

static fjacf fjacfonc ;

/** function call **/

void C2F(fjac)(neq, t, y, ml, mu, pd, nrpd)
     integer *neq,*ml,*mu,*nrpd;
     double *t,*y,*pd;
{
  (*fjacfonc)(neq, t, y, ml, mu, pd, nrpd);
}

/** fixes the function associated to name **/

void C2F(setfjac)(name,rep)
     char *name;
     int *rep;
{
  fjacfonc = (fjacf) SetFunction(name,rep,FTab_fjac);
}

/***********************************
 * Search Table for impl 
 ***********************************/

/** the current function fixed by setfres **/

static fresf fresfonc ;

/** function call **/

void C2F(fres)(ny,t,y,s,r,ires)
     integer *ny,*ires;
     double *t,*y,*s,*r;
{
  (*fresfonc)(ny,t,y,s,r,ires);
}

/** fixes the function associated to name **/

void C2F(setfres)(name,rep)
     char *name;
     int *rep;
{
  fresfonc = (fresf) SetFunction(name,rep,FTab_fres);
}


/** the current function fixed by setfadda **/

static faddaf faddafonc ;

void C2F(fadda)(ny,t,y,ml,mu,p,nrowp)
     integer *ny,*ml,*mu,*nrowp;
     double *t,*y,*p;
{
  (*faddafonc)(ny,t,y,ml,mu,p,nrowp);
}

/** fixes the function associated to name **/

void C2F(setfadda)(name,rep)
     char *name;
     int *rep;
{
  faddafonc = (faddaf) SetFunction(name,rep,FTab_fadda);
}


/** the current function fixed by setfj2 **/

static fj2f fj2fonc ;

/** function call **/

void C2F(fj2)(ny,t,y,s,ml,mu,p,nrowp)
     integer *ny,*ml,*mu,*nrowp;
     double *t,*y,*s,*p;
{
  (*fj2fonc)(ny,t,y,s,ml,mu,p,nrowp);
}

/** fixes the function associated to name **/

void C2F(setfj2)(name,rep)
     char *name;
     int *rep;
{
  fj2fonc = (fj2f) SetFunction(name,rep,FTab_fj2);
}


/***********************************
 * Search Table for corr
 *   corr uses two externals : dgetx and dgety 
 ***********************************/

/** the current function fixed by setdgetx **/


static dgetxf dgetxfonc ;

/** function call **/

void C2F(dgetx)(x, incr, istart)
     integer *incr,*istart;
     double *x;
{
  (*dgetxfonc)(x, incr, istart);
}

/** fixes the function associated to name **/

void C2F(setdgetx)(name,rep)
     char *name;
     int *rep;
{
  dgetxfonc = (dgetxf) SetFunction(name,rep,FTab_dgetx);
}


/** the current function fixed by setdgety **/

static dgetyf dgetyfonc ;

void C2F(dgety)(y, incr, istart)
     integer *incr,*istart;
     double *y;
{
  (*dgetyfonc)(y, incr, istart);
}


/** fixes the function associated to name **/

void C2F(setdgety)(name,rep)
     char *name;
     int *rep;
{
  dgetyfonc = (dgetyf) SetFunction(name,rep,FTab_dgety);
}


/***********************************
 * Search Table for colnew 
 *   corr uses : fcoldg , fcolg, fcoldf,fcolf,fcolgu
 ***********************************/

/** the current function fixed by setfcoldg **/

static fcoldgf fcoldgfonc ;

/** function call **/

void C2F(fcoldg)(i,z,dg)
     integer *i;
     double *z,*dg;
{
  (*fcoldgfonc)(i,z,dg);
}

/** fixes the function associated to name **/

void C2F(setfcoldg)(name,rep)
     char *name;
     int *rep;
{
  fcoldgfonc = (fcoldgf) SetFunction(name,rep,FTab_fcoldg);
}


/** the current function fixed by setfcolg **/

static fcolgf fcolgfonc ;

/** function call **/

void C2F(fcolg)(i,z,g)
     integer *i;
     double *z,*g;
{
  (*fcolgfonc)(i,z,g);
}

/** fixes the function associated to name **/

void C2F(setfcolg)(name,rep)
     char *name;
     int *rep;
{
  fcolgfonc = (fcolgf) SetFunction(name,rep,FTab_fcolg);
}



/** the current function fixed by setfcoldf **/

static fcoldff fcoldffonc ;

/** function call **/

void C2F(fcoldf)(x,z,df)
     double *x,*z,*df;
{
  (*fcoldffonc)(x,z,df);
}

/** fixes the function associated to name **/

void C2F(setfcoldf)(name,rep)
     char *name;
     int *rep;
{
  fcoldffonc = (fcoldff) SetFunction(name,rep,FTab_fcoldf);
}


/** the current function fixed by setfcolf **/

static fcolff fcolffonc ;

/** function call **/

void C2F(fcolf)(x,z,df)
     double *x,*z,*df;
{
  (*fcolffonc)(x,z,df);
}

/** fixes the function associated to name **/

void C2F(setfcolf)(name,rep)
     char *name;
     int *rep;
{
  fcolffonc = (fcolff) SetFunction(name,rep,FTab_fcolf);
}

/** the current function fixed by setfcolgu **/

static fcolguf fcolgufonc ;

/** function call **/

void C2F(fcolgu)(x,z,dmval)
     double *x,*z,*dmval;
{
  (*fcolgufonc)(x,z,dmval);
}

/** fixes the function associated to name **/

void C2F(setfcolgu)(name,rep)
     char *name;
     int *rep;
{
  fcolgufonc = (fcolguf) SetFunction(name,rep,FTab_fcolgu);
}


/***********************************
 * Search Table for intg 
 *    uses : fintg 
 ***********************************/

/** the current function fixed by setfintg **/

static fintgf fintgfonc ;

/** function call : WARNING fintg returns a double  **/

double *C2F(fintg)(x)
     double *x;
{
  return((*fintgfonc)(x));
}

/** fixes the function associated to name **/

void C2F(setfintg)(name,rep)
     char *name;
     int *rep;
{
  fintgfonc = (fintgf) SetFunction(name,rep,FTab_fintg);
}

/***********************************
 * Search Table for int2d
 *    uses : fint2d
 ***********************************/

/** the current function fixed by setfint2d **/

static fint2df fint2dfonc ;

/** function call : WARNING fintg returns a double  **/

double *C2F(fint2d)(x,y)
     double *x,*y;
{
  return((*fint2dfonc)(x,y));
}

/** fixes the function associated to name **/

void C2F(setfint2d)(name,rep)
     char *name;
     int *rep;
{
  fint2dfonc = (fint2df) SetFunction(name,rep,FTab_fint2d);
}

/***********************************
 * Search Table for int3d
 *    uses : fint3d
 ***********************************/

/** the current function fixed by setfint3d **/

static fint3df fint3dfonc ;

/** function call : WARNING fintg returns a double  **/

void C2F(fint3d)(xyz,numfun,v)
     double *xyz,*v;
     integer *numfun;
{
  (*fint3dfonc)(xyz,numfun,v);
}

/** fixes the function associated to name **/

void C2F(setfint3d)(name,rep)
     char *name;
     int *rep;
{
  fint3dfonc = (fint3df) SetFunction(name,rep,FTab_fint3d);
}


/***********************************
 * Search Table for fsolve 
 *    uses : fsolvf and fsolvj 
 ***********************************/

/** the current function fixed by setsolvf **/

static fsolvff fsolvffonc ;

/** function call : fsolvf  **/

void C2F(fsolvf)(n,x,fvec,iflag)
     integer *n,*iflag;
     double *x,*fvec;
{
  (*fsolvffonc)(n,x,fvec,iflag);
}

/** fixes the function associated to name **/

void C2F(setfsolvf)(name,rep)
     char *name;
     int *rep;
{
  fsolvffonc = (fsolvff) SetFunction(name,rep,FTab_fsolvf);
}

/** the current function fixed by setfsolvj **/

static fsolvjf fsolvjfonc ;

/** function call   **/

void C2F(fsolvj)(n,x,fjac,iflag)
     double *x,*fjac;
     integer *n,*iflag;
{
  (*fsolvjfonc)(n,x,fjac,iflag);
}

/** fixes the function associated to name **/

void C2F(setfsolvj)(name,rep)
     char *name;
     int *rep;
{
  fsolvjfonc = (fsolvjf) SetFunction(name,rep,FTab_fsolvj);
}


/***********************************
 * Search Table for optim 
 *    uses : foptim 
 ***********************************/

/** the current function fixed by setsolvf **/

static foptimf foptimfonc ;

/** function call : foptim  **/

void C2F(foptim)(indsim,n,x,f,g,izs,rzs,dzs)
     integer *indsim,*n,*izs;
     float *rzs;
     double *x,*f,*g,*dzs;
{
  (*foptimfonc)(indsim,n,x,f,g,izs,rzs,dzs);
}

/** fixes the function associated to name **/

void C2F(setfoptim)(name,rep)
     char *name;
     int *rep;
{
  foptimfonc = (foptimf) SetFunction(name,rep,FTab_foptim);
}



/***********************************
 * Search Table for schur uses : fschur 
 ***********************************/

/** the current function fixed by setsolvf **/

static fschurf fschurfonc ;

/** function call : fschur  **/

integer *C2F(fschur)(lsize,alpha,beta,s,p)
     integer *lsize;
     double *alpha,*beta,*s,*p;
{
  return((*fschurfonc)(lsize,alpha,beta,s,p));
}

/** fixes the function associated to name **/

void C2F(setfschur)(name,rep) 
     char *name;
     int *rep;
{
  if (strncmp(name,"c",1)== 0 || strncmp(name,"cont",3)== 0 )
      fschurfonc = (fschurf) SetFunction("folhp",rep,FTab_fschur);
  else if (strncmp(name,"d",1)== 0 || strncmp(name,"disc",4)== 0 )
      fschurfonc = (fschurf) SetFunction("find",rep,FTab_fschur);
  else 
    fschurfonc = (fschurf) SetFunction(name,rep,FTab_fschur);
}


/***********************************
 * Search Table for fydot2
 ***********************************/

/** the current function fixed by setfydot2 **/

static fydot2f fydot2fonc ;

/** function call **/

void C2F(fydot2)(n,t,y,ydot)
     integer *n;
     double *t,*y,*ydot;
{
  integer nd1,iflag1;
  C2F(getcodc)(&nd1,&iflag1);
  (*fydot2fonc)(&iflag1,n,&nd1,t,y,ydot);
}

/** fixes the function associated to name **/

void C2F(setfydot2)(name,rep)
     char *name;
     int *rep;
{
  fydot2fonc = (fydot2f) SetFunction(name,rep,FTab_fydot2);
}


/***********************************
 * Search Table for dassl or dassrt 
 ***********************************/

/** the current function fixed by setfresd **/

static fresdf fresdfonc ;

/** function call **/

void C2F(fresd)(t,y,ydot,res,ires,rpar,ipar)
      integer *ires,*ipar;
      double  *t,*y,*ydot,*res,*rpar;
{
  (*fresdfonc)(t,y,ydot,res,ires,rpar,ipar);
}

/** fixes the function associated to name **/

void C2F(setfresd)(name,rep)
     char *name;
     int *rep;
{
  fresdfonc = (fresdf) SetFunction(name,rep,FTab_fresd);
}


/** the current function fixed by setfjacd **/

static fjacdf fjacdfonc ;

/** function call **/

void C2F(fjacd)(t,y,ydot,pd,cj,rpar,ipar)
     integer *ipar;
     double  *t,*y,*ydot,*rpar,*cj,*pd;

{
  (*fjacdfonc)(t,y,ydot,pd,cj,rpar,ipar);
}

/** fixes the function associated to name **/

void C2F(setfjacd)(name,rep)
     char *name;
     int *rep;
{
  fjacdfonc = (fjacdf) SetFunction(name,rep,FTab_fjacd);
}


/** the current function fixed by setfsurfd **/

static fsurfdf fsurfdfonc ;

/** function call **/


void C2F(fsurfd)(neq,t,y,ng,gout,rpar,ipar)
     integer *neq,*ng,*ipar;
     double  *t,*y,*gout,*rpar;
{
  (*fsurfdfonc)(neq,t,y,ng,gout,rpar,ipar);
}

/** fixes the function associated to name **/

void C2F(setfsurfd)(name,rep)
     char *name;
     int *rep;
{
  fsurfdfonc = (fsurfdf) SetFunction(name,rep,FTab_fsurfd);
}

/***********************************
 * Search Table for dasrt ??? 
 **********************************/

/** the current function fixed by setfsurf **/

static fsurff fsurffonc ;

/** function call **/

void C2F(fsurf)(ny, t, y, ng, gout) 
      integer *ny,*ng;
      double  *t,*y,*gout;
{
  (*fsurffonc)(ny, t, y, ng, gout) ;
}

/** fixes the function associated to name **/

void C2F(setfsurf)(name,rep)
     char *name;
     int *rep;
{
  fsurffonc = (fsurff) SetFunction(name,rep,FTab_fsurf);
}


/***********************************
 * Search Table for fbutn
 **********************************/

/** the current function fixed by setfbutn **/

static fbutnf fbutnfonc ;

/** function call **/

void C2F(fbutn)(name,win,entry)
     char *name;
     integer *win,*entry;
{
  (*fbutnfonc)(name,win,entry);
}

/** fixes the function associated to name **/

void C2F(setfbutn)(name,rep)
     char *name;
     int *rep;
{
  fbutnfonc = (fbutnf) SetFunction(name,rep,FTab_fbutn);
}


/***********************************
 * Search Table for interf 
 **********************************/

/** the current function fixed by setinterf **/

static interff interffonc ;

/** function call **/


void C2F(interf)(x1 ,x2 ,x3 ,x4 ,x5 ,x6 ,x7 ,x8 ,x9 ,x10,
		 x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,
		 x21,x22,x23,x24,x25,x26,x27,x28,x29,x30)
     void *x1 ,*x2 ,*x3 ,*x4 ,*x5 ,*x6 ,*x7 ,*x8 ,*x9 ,*x10;
     void *x11,*x12,*x13,*x14,*x15,*x16,*x17,*x18,*x19,*x20;
     void *x21,*x22,*x23,*x24,*x25,*x26,*x27,*x28,*x29,*x30;
{
  (*interffonc)(x1 ,x2 ,x3 ,x4 ,x5 ,x6 ,x7 ,x8 ,x9 ,x10,
	       x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,
	       x21,x22,x23,x24,x25,x26,x27,x28,x29,x30);
}


void C2F(interf1)(name) char *name;
{
  ((interff1) *interffonc)(name,strlen(name));
}

/** fixes the function associated to name **/

void C2F(setinterf)(name,rep)
     char *name;
     int *rep;
{
  interffonc = (interff) SetFunction(name,rep,FTab_interf);
}

/** function call **/


/*******************************************
 * General functions 
 *******************************************/

voidf SetFunction(name,rep,table)
     char *name;
     int *rep;
     FTAB table[];
{
  void (*loc)();
  char *s;
  strncpy(buf,name,MAXNAME);
  s=buf ; while ( *s != ' ' && *s != '\0') { s++;};
  *s= '\0';
  if ( SearchComp(table,buf,&loc) == OK) 
    {
      *rep = 0;
      return(loc);
    }
  if ( SearchInDynLinks(buf,&loc) >= 0 )
    {
      *rep = 0;
      return(loc);
    }
  loc = Emptyfunc;
  *rep = 1;
  /* sciprint(" Function %s not found\r\n",name); */
  return(loc);
}



/*******************************************
 * Attention trier la table 
 * cherche un operateur dans une table : 
 * a ameliorer en utilisant bsearch 
 *******************************************/

static int SearchComp(Ftab,op,realop) 
     FTAB Ftab[];
     void (**realop)();
     char *op;
{
  int i=0;
  while ( Ftab[i].name != (char *) 0) 
     {
       int j;
       j = strcmp(op,Ftab[i].name);
       if ( j == 0 )
         {
           *realop = Ftab[i].f;
	   return(OK);
	 }
       else
         { 
	   if ( j <= 0)
             {
               /* sciprint("\nUnknow function <%s>\r\n",op); */
               return(FAIL);
             }
	   else i++;
         }
     }
  /* sciprint("\n Unknow function <%s>\r\n",op); */
  return(FAIL);
}




/*********************************************************
 * General function to get an external 
 * when writting an interface 
 * In 
 *   name : name of calling function for error message 
 *   n    : position of external in argument list 
 *   Table: table of stored function 
 *   scifun: a function 
 *   lhs,rhs : expected lhs,rhs values of the external 
 * 
 * if external argument is a 'string' : an associated function 
 * is searched in Table and in dynamically linked functions 
 * if found the function is returned 
 *
 * if external argument is a macro (lhs,rhs) are checked 
 * and scifun is returned 
 * 
 *********************************************************/

#define a_chain 10
#define a_function 13

extern int C2F(vartype) __PARAMS((integer*));

voidf GetFuncPtr(name,n,Table,scifun,ifunc,lhs,rhs)
        char *name;
        int n,*lhs,*rhs,*ifunc;
        FTAB *Table; 
        voidf scifun;
{
  int type,rep,mm,nn,nlhs,nrhs;
  voidf f;
  type=C2F(vartype)(&n);
  switch ( type)
    {
    case a_chain :
      GetRhsVar(n, "c", &mm, &nn, ifunc);
      f = SetFunction(cstk(*ifunc),&rep,Table);
      if ( rep == 1 )
        {
	  Scierror(999,"%s: external %s not found \r\n",name,cstk(*ifunc));
          return (voidf) 0;
        }  
      return f ;
    case  a_function :
      GetRhsVar(n, "f", &nlhs,&nrhs, ifunc);
      /*
      if ( nlhs != *lhs || nrhs != *rhs ) 
	{
	  Scierror(999,"%s: given external has wrong (lhs,rhs) values (%d,%d), (%d,%d) required\r\n",name,
		   nlhs,nrhs,*lhs,*rhs);
	  return (voidf) 0 ;
	}
      */
      return (voidf) scifun ;
    default:
      sciprint("Wrong parameter in %s ! (number %d)\r\n",name,n);
      Error(999);
      return (voidf) 0 ;
    }
}
