#include <stdlib.h>

#include "intersci-n.h"

/*********************************************************************
 * Dealing with the set of variables 
 *********************************************************************/

int ShowVariables() 
{
  int i;
  VARPTR var;
  for (i = 0; i < nVariable; i++) 
    {
      int j;
      var = variables[i];
      fprintf(stderr,"==============variable %d : name %s\n",i+1,var->name);
      fprintf(stderr,"type %s<->%s\n",SGetSciType(var->type),
	      SGetForType(var->for_type));
      fprintf(stderr,"elts : [");
      for (j=0 ; j < var->length ; j++) 
	{
	  fprintf(stderr,"{var %d:%s}",var->el[j],
		  variables[var->el[j]-1]->name);
	}
      fprintf(stderr,"]\n");
      /* fprintf(stderr," name of external function when type is  char *fexternal[MAXNAM]; 
	 external 
      */
      fprintf(stderr," ?  equal %d\n",var->equal);
      fprintf(stderr,"for_names [");
      if ( var->nfor_name != 0) 
	{
	  for (j=0 ; j < var->nfor_name ; j++) 
	    {
	      fprintf(stderr,"{%s:%d}",var->for_name[j],
		      var->for_name_orig[j]);
	      if ( j != var->nfor_name-1) fprintf(stderr,",");
	    }
	}
      fprintf(stderr,"]\n");
      fprintf(stderr," position in a list %d\n",var->list_el);
      if ( var->list_el > 0 )
	fprintf(stderr," List name %s\n",var->list_name);
      fprintf(stderr," type of optional variable %d\n",var->opt_type);
      if ( var->opt_type != 0)
	fprintf(stderr," name or value default for optional variable %s\n",
		var->opt_name);
      fprintf(stderr," 1 if the variable is really present %d\n",var->present);
      fprintf(stderr,"sciarg %d, rhs %d, lhs %d\n",
	      var->is_sciarg,
	      var->stack_position,
	      var->out_position);
    }
  return 0;
}

/*******************************************************
 * this function recomputes the stack_positions 
 * taking into account the fact that external 
 * variables are not allocated on the stack. 
 * We use the fact that after the readfunction 
 * the stack_position increases when i increases  in variables[i]
 * CSTRINGV variables which are not given as arguments 
 * must be treated the same way 
 *******************************************************/

#define ISNONSTACK(var) ( (var)->for_type == EXTERNAL \
   || ( (var)->for_type == CSTRINGV && (var)->is_sciarg == 0  && (var)->list_el == 0 ))

int FixStackPositions() 
{
  int i;
  VARPTR var,var1;
  for (i = nVariable-1 ; i >= 0; i--) 
    {
      int j;
      var = variables[i];
      if ( ISNONSTACK(var) )
	{
	  icre--;
	  for ( j = 0 ; j < nVariable ; j++ ) 
	    {
	      var1 = variables[j];
	      if ( var1->stack_position > var->stack_position 
		   && ! (ISNONSTACK(var1)))
		var1->stack_position--;
	    }
	}
    }
  return 0;
}

/********************************************************
 * return the variable number of variable name. if it does not already exist,
 *  it is created and "nVariable" is incremented
 * p corresponds to the present slot of var structure:
 *  - if the variable does not exist it is created with p value
 *  - if the variable exists it is created with (p or 0) value 
 *********************************************************/


IVAR GetVar(name,p)
     char *name;
     int p;
{
  int i;
  VARPTR var;
  if (strcmp(name,"out") == 0) {
    printf("the name of a variable which is not the output variable\n");
    printf("  of SCILAB function cannot be \"out\"\n");
    exit(1);
  }
  for (i = 0; i < nVariable; i++) {
    var = variables[i];
    if (strcmp(var->name,name) == 0) {
      var->present = var->present || p;
      return(i+1);
    }
  }
  if (nVariable == MAXVAR) {
    printf("too many variables\n");
    printf("  augment constant \"MAXVAR\" and recompile intersci\n");
    exit(1);
  }
  var = VarAlloc();
  if (var == 0) {
    printf("Running out of memory\n");
    exit(1);
  }
  var->name = (char *)malloc((unsigned)(strlen(name) + 1));
  strcpy(var->name,name);
  var->type = 0;
  var->length = 0;
  var->for_type = 0;
  var->equal = 0;
  var->nfor_name = 0;
  var->kp_state = -1;
  var->list_el = 0;
  var->opt_type = 0;
  var->present = p;
  var->out_position = 0;
  var->stack_position = 0;
  var->is_sciarg = 0;
  variables[nVariable++] = var;
  var->vpos = nVariable;
  return(nVariable);
}

/************************************************************************
 * return the variable number of variable name which must already  exist 
 ***********************************************************************/

IVAR GetExistVar(name)
     char *name;
{
  int i;
  VARPTR var;
  if (strcmp(name,"out") == 0) {
    printf("the name of a variable which is not the output variable\n");
    printf("  of SCILAB function cannot be \"out\"\n");
    exit(1);
  }
  for (i = 0; i < nVariable; i++) {
    var = variables[i];
    if (strcmp(var->name,name) == 0) {
      /* always present */
      var->present = 1;
      return(i+1);
    }
  }
  i=CreatePredefVar(name);
  if ( i != -1) return(i);
  printf("variable \"%s\" must exist\n",name);
  exit(1);
}

/******************************************************************
 * fname,rhs,lhs,err are predefined variables 
 * if someone want to add them in the Fortran or C Calling sequence 
 * it's done without aby checks 
 ******************************************************************/

int CreatePredefVar(name)
     char *name;
{
  VARPTR var;
  if (strcmp(name,"err")  == 0 
      || strcmp(name,"rhs") == 0 
      || strcmp(name,"lhs") == 0 
      || strcmp(name,"fname") == 0)
    {
      int num ;
      num=GetVar(name,1);
      var = variables[num-1];
      var->for_type = PREDEF;
      return(num);
    }
  return(-1);
}

/********************************************************
 * return the variable number of variable "out"
 * which is created and "nVariable" is incremented 
 ********************************************************/

IVAR GetOutVar(name)
     char *name;
{
  VARPTR var;
  if (strcmp(name,"out") != 0) {
    printf("the name of output variable of SCILAB function\n");
    printf("  must be \"out\"\n");
    exit(1);
  }
  if (nVariable == MAXVAR) {
    printf("too many variables\n");
    printf("  augmente constant \"MAXVAR\" and recompile intersci\n");
    exit(1);
  }
  var = VarAlloc();
  if (var == 0) {
    printf("Running out of memory\n");
    exit(1);
  }
  var->name = (char *)malloc((unsigned)(strlen(name) + 1));
  strcpy(var->name,name);
  var->type = 0;
  var->length = 0;
  var->for_type = 0;
  var->equal = 0;
  var->nfor_name = 0;
  var->kp_state = -1;
  var->list_el = 0;
  var->opt_type = 0;
  var->present = 0;
  var->out_position = 0;
  var->stack_position = 0;
  variables[nVariable++] = var;
  return(nVariable);
}

/* return the variable number of variable "out"
   which must exist */

IVAR GetExistOutVar()
{
  int i;
  char str[4];
  strcpy(str,"out");
  for (i = 0; i < nVariable; i++) {
    if (strcmp(variables[i]->name,str) == 0)
      return(i+1);
  }
  printf("variable \"out\" must exist\n");
  exit(1);
}

/* return the variable number of variable "out" or 0 */

IVAR CheckOutVar()
{
  int i;
  char str[4];
  strcpy(str,"out");
  for (i = 0; i < nVariable; i++) {
    if (strcmp(variables[i]->name,str) == 0)
      return(i+1);
  }
  return 0;
}




/************************************************
 * add name in the for_name array 
 * field of variable ivar 
 * and add the number ivar1 in the for_name_orig array 
 * field (ivar1 is a stack position) 
 * this is used for variables which code dimensions 
 * if for_name contains [m1,n2,m3] 
 * and for_name_orig contains [1,2,5] 
 * this means that stack variables 1,2,5 must have 
 * the same value for m1,n2,m3
 *************************************************/

void AddForName(ivar,name,cname,ivar1)
     IVAR ivar;
     IVAR ivar1;
     char* name;
     char* cname;
{
  VARPTR var;
  int l;
  var = variables[ivar-1];
  l = var->nfor_name;
  if (l == MAXARG) {
    printf("too many \"for_name\" for variable \"%s\"\n",var->name);
    printf("  augment constant \"MAXARG\" and recompile intersci\n");
    exit(1);
  }
  var->for_name[l] = (char *)malloc((unsigned)(strlen(name) + 1));
  if ( cname != NULL) 
    {
      var->C_name[l] = (char *)malloc((unsigned)(strlen(cname) + 1));
      strcpy(var->C_name[l],cname);
    }
  else 
    var->C_name[l] = NULL;
  var->for_name_orig[l] = ivar1;
  strcpy(var->for_name[l],name);
  var->nfor_name = l + 1;
}

/***************************
 * add name in the for_name array 
 * field of variable ivar 
 * this names are not cleared between 
 * the two passes 
 ***************************/

void AddForName1(ivar,name,cname,ivar1)
     IVAR ivar;
     IVAR ivar1;
     char* name;
     char* cname;
{
  VARPTR var;
  int l;
  var = variables[ivar-1];
  l = var->nfor_name;
  if ( pass == 0 && var->kp_state == -1 ) 
    {
      var->kp_state = var->nfor_name ;
    }
  if (l == MAXARG) {
    printf("too many \"for_name\" for variable \"%s\"\n",var->name);
    printf("  augment constant \"MAXARG\" and recompile intersci\n");
    exit(1);
  }
  var->for_name[l] = (char *)malloc((unsigned)(strlen(name) + 1));
  if ( cname != NULL) 
    {
      var->C_name[l] = (char *)malloc((unsigned)(strlen(cname) + 1));
      strcpy(var->C_name[l],cname);
    }

  else 
    var->C_name[l] = NULL;
  var->for_name_orig[l] = ivar1;
  strcpy(var->for_name[l],name);
  var->nfor_name = l + 1;
}


/********************************************************
 ** if kp_state is not -1 then kp_state is 0 and the for_names are cleaned
 *(in fact just forgotten the free call should be added 
 ********************************************************/

void ForNameClean() 
{
  VARPTR var;
  int i;
  for (i = 0; i < nVariable; i++) {
    var = variables[i];
    if ( var->kp_state != -1 ) 
      {
	var->nfor_name = var->kp_state ;
      }
  }
}

/***********************************************************
 * Changes the first for_name of variable number ivar to name 
 * or add the forname if necessary. 
 * if the forname is added, since we do not change the kp_state,
 * this value is kept between the two pass. 
 ************************************************************/

#define FORNAME 128


#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif 

#ifdef __STDC__ 
void ChangeForName2(VARPTR varptr,char * format ,...)
#else 
     /*VARARGS0*/
     void ChangeForName2(va_alist) va_dcl
#endif
{
  char forbuf[FORNAME];
  va_list ap;
#ifdef __STDC__ 
  va_start(ap,format);
#else 
  char *format;
  VARPTR varptr;
  va_start(ap);
  varptr = va_arg(ap, VARPTR );
  format = va_arg(ap, char *);
#endif
  vsprintf(forbuf,format,ap);
  ChangeForName1(varptr,forbuf);
  va_end(ap);
}

void ChangeForName1(var,name)
     VARPTR var;
     char* name;
{
  int l,pos=0;
  l = var->nfor_name;
  if ( l  != 0)
    { 
      int i;
      for ( i=0 ; i < l ; i++) 
	{
	  if ( var->for_name_orig[i] == var->stack_position ) 
	    { 
	      pos = i ; break;
	    }
	}
    }
  var->for_name[pos] = (char *)malloc((unsigned)(strlen(name) + 1));
  strcpy(var->for_name[pos],name);
  if ( pos != 0) 
    {
      int xx;
      char *loc = var->for_name[pos];
      var->for_name[pos] =       var->for_name[0];
      var->for_name[0] = loc;
      xx= var->for_name_orig[pos];
      var->for_name_orig[pos] =       var->for_name_orig[0];
      var->for_name_orig[0] = xx;
    }
  /* we keep the C name  var->C_name[0] = NULL; */
  /** if l==0 a for_name **/
  if (l == 0) var->nfor_name = 1;
}

/***********************************************************
 * Function used to deal with types 
 * conversion from code <-> to string 
 ************************************************************/

/* Attention tableau en ordre alphabetique */

static struct btype { char *sname ;
  int  code ;}
SType[] = {
  {"any",	ANY},
  {"bmatrix",    BMATRIX},
  {"bpointer",   SCIBPOINTER},
  {"column",	COLUMN},
  {"empty",	EMPTY},
  {"imatrix",    IMATRIX},
  {"list", 	LIST},
  {"lpointer",	SCILPOINTER},
  {"matrix",	MATRIX},
  {"mpointer",	SCIMPOINTER},
  {"opointer",	SCIOPOINTER},
  {"polynom",	POLYNOM},
  {"row",	ROW},
  {"scalar",	SCALAR},
  {"sequence",	SEQUENCE},
  {"smpointer",  SCISMPOINTER},
  {"sparse",    SPARSE},
  {"string",	STRING},
  {"stringmat",	STRINGMAT},
  {"tlist", 	TLIST},
  {"mlist", 	MLIST},
  {"vector",	VECTOR},
  {"work",	WORK},
  {(char *) 0 ,  -1}
};

/* Type Scilab:  renvoit un codage du type en nombre entier etant donne une chaine */

int GetBasType(sname)
     char *sname;
{ 
  int i=0;
  while ( SType[i].sname != (char *) NULL)
    {
      int j ;
      j = strcmp(sname,SType[i].sname);
      if ( j == 0 ) 
	{ 
	  return(SType[i].code);
	}
      else 
	{ 
	  if ( j <= 0) 
	    break;
	  else i++;
	}
    }
  printf("the type of variable \"%s\" is unknown\n",sname);
  exit(1);
}

/**********************************************
 * This function returns the string description of 
 * a Scilab type given its code number 
 **********************************************/

char *SGetSciType(type)
     int type;
{
  int i=0;
  while ( SType[i].code  != -1 ) 
    {
      if ( SType[i].code == type ) 
	return(SType[i].sname);
      else 
	i++;
    }
  return("unknown");
}

/* Warning : Ftype is lexicographically sorted */

static struct ftype { char *fname ; /* full fortran type name */
  int  code ; /* associated code */
  char *abrev; /* abbrev code : just for c d i r */
  char *st_name; /* stack to use :just for c d i r*/ 
  char *b_convert; /* converter for building lhs (for lists)*/
  int dec; /* declaration for the Fortran name */
  char *ctype; /* type in C */
}
FType[] = {
  {"Cstringv",CSTRINGV,"XX","XX","XX",-1,"XX"},
  {"boolean",BOOLEAN,"b","istk","icopy",DEC_INT,"int"},
  {"bpointer",BPOINTER,"XX","XX","XX",-1,"XX"},
  {"char",CHAR,"c","cstk","cvstr1",DEC_CHAR,"char"},
  {"double", DOUBLE,"d","stk","dcopy",DEC_DOUBLE,"double"},
  {"int",INT,"i","istk","int2db",DEC_INT,"int"},
  {"integer",INT,"i","istk","int2db",DEC_INT,"int"},
  {"lpointer",LPOINTER,"XX","XX","XX",-1,"XX"},
  {"mpointer",MPOINTER,"XX","XX","XX",-1,"XX"},
  {"opointer",OPOINTER,"XX","XX","XX",-1,"XX"},
  {"predef",PREDEF,"XX","XX","XX",-1,"XX"},
  {"real",REAL,"r","sstk","rea2db",DEC_REAL,"float"},
  {"smpointer",SMPOINTER,"XX","XX","XX",-1,"XX"},
  {(char *) 0 ,  -1}
};

/**********************************************
 * This function returns the code of a Fortran (or C) type 
 * given its full name 
 **********************************************/

int GetForType(type)
     char *type;
{
  int i=0;
  while ( FType[i].fname != (char *) NULL)
    {
      int j;
      j = strcmp(type,FType[i].fname);
      if ( j == 0 ) 
	{ 
	  return(FType[i].code);
	}
      else 
	{ 
	  if ( j <= 0) 
	    break;
	  else i++;
	}
    }
  return(EXTERNAL);
}

/**********************************************
 * This function returns the string description of 
 * a C or Fortran type given its code number 
 **********************************************/

char *SGetForType(type)
     int type;
{
  int i=0;
  while ( FType[i].code  != -1 ) 
    {
      if ( FType[i].code == type ) 
	return(FType[i].fname);
      else 
	i++;
    }
  return("External");
}


/**********************************************
 * This function returns an abbrev string 'c' 'd' 'i' 'r' 'b'
 * of a C or Fortran type given its code number 
 **********************************************/

char *SGetForTypeAbrev(var)
     VARPTR var;
{
  int i=0;
  while ( FType[i].code  != -1 ) 
    {
      if ( FType[i].code == var->for_type ) 
	return(FType[i].abrev);
      else 
	i++;
    }
  return("XX");
}

/**********************************************
 * This function returns the code for declaration 
 * of a C or Fortran type given its code number 
 **********************************************/

int SGetForDec(type)
     int type;
{
  int i=0;
  while ( FType[i].code  != -1 ) 
    {
      if ( FType[i].code == type ) 
	return(FType[i].dec);
      else 
	i++;
    }
  return(-1);
}

/**********************************************
 * This function returns the C string for declaration 
 * of a C or Fortran type given its code number 
 **********************************************/

char* SGetCDec(type)
     int type;
{
  int i=0;
  while ( FType[i].code  != -1 ) 
    {
      if ( FType[i].code == type ) 
	return(FType[i].ctype);
      else 
	i++;
    }
  return("XXX");
}


/**********************************************
 * given a variable and according to its C or Fortran type,
 * this function returns the stack name to use 
 **********************************************/

char *SGetForTypeStack(var)
     VARPTR var;
{
  int i=0;
  while ( FType[i].code  != -1 ) 
    {
      if ( FType[i].code == var->for_type ) 
	{
	  if ( FType[i].st_name[0] == 'X' ) 
	    {
	      printf("incompatibility between Scilab and Fortran type for variable \"%s\"\n",
		     var->name);   
	      exit(1);
	    }
	  return(FType[i].st_name);
	}
      else 
	i++;
    }
  printf("Unknown Fortran type for variable \"%s\"\n", var->name);   
  exit(1);
}




/**********************************************
 * given a variable and according to its C or Fortran type,
 * this function returns the name of the converter to use 
 * for back conversion 
 **********************************************/

char *SGetForTypeBConvert(var)
     VARPTR var;
{
  int i=0;
  while ( FType[i].code  != -1 ) 
    {
      if ( FType[i].code == var->for_type ) 
	{
	  if ( FType[i].b_convert[0] == 'X' ) 
	    {
	      printf("incompatibility between Scilab and Fortran type for variable \"%s\"\n",
		     var->name);   
	      exit(1);
	    }
	  return(FType[i].b_convert);
	}
      else 
	i++;
    }
  printf("Unknown Fortran type for variable \"%s\"\n", var->name);   
  exit(1);
}



/**********************************************
 * This function returns an abbrev string 'c' 'd' 'i' 'r' 'b'
 * of a C or Fortran type to use for back converting 
 * an external variable 
 * XXXXX : only for MATRIX et BMATRIX
 **********************************************/

char *SGetExtForTypeAbrev(var)
     VARPTR var;
{
  if ( var->type == BMATRIX ) 
    return("b");
  else 
    {
      if ( strcmp(var->fexternal,"cintf")==0) 
	return "i";
      else 
	return("d");
    }
}


/**********************************************
 * given a variable and according to its C or Fortran type,
 * this function returns the stack name to use 
 * for back conversion of external
 * XXXXX : idem 
 **********************************************/

char *SGetExtForTypeStack(var)
     VARPTR var;
{
  if ( var->type == BMATRIX ) 
    return(FType[1].st_name);
  else 
    return(FType[4].st_name);
}

