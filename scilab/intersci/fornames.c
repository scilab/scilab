#include "intersci-n.h"

/*****************************************************************
 * The main function here is FixForNames 
 * FixForNames is used to give C or Fortran Names to Scilab 
 *     input arguments and to external variables 
 * For example if a matrix m n is a Scilab input arguments 
 *   a->el[0] points to variable m and a->el[1] points to 
 *   variable n 
 *   According to the stack_position of a (for example suppose here 
 *   that a is the 4th argument) for_names are added to m and n 
 *   m -> m4 and n -> n4 
 * If m is a common dimension for a set of variables 
 *   then m will have more than one for_names at the end of FixForName 
 *   execution 
 ******************************************************************/

extern  int indent ; /* incremental counter for code indentation */
extern  int pass;    /* flag for couting pass on code generation */

static char str[MAXNAM];
static char str1[MAXNAM];

void StrGen(strl,var)
     char *strl;
     VARPTR var;
{
  if ( var->for_type == EXTERNAL )
    {
      /** variables me1 or  me1e1  */
      if ( var->list_el == 0 ) 
	sprintf(str,"%se%d",strl,var->stack_position);
      else 
	sprintf(str,"%se%de%d",strl,var->stack_position,var->list_el);
    }
  else 
    {
      if ( var->list_el == 0 ) 
	sprintf(str,"%s%d",strl,var->stack_position);
      else 
	sprintf(str,"%s%de%d",strl,var->stack_position,var->list_el);
    }
}


void ForMATRIX(var)   VARPTR var ;
{
  StrGen("m",var);
  AddForName1(var->el[0],str,NULL,var->stack_position);
  StrGen("n",var);
  AddForName1(var->el[1],str,NULL,var->stack_position);
}

  
void ForSTRING(var)   VARPTR var ;
{
  StrGen("m",var);
  AddForName1(var->el[0],str,NULL,var->stack_position);
}


void ForIMATRIX(var)   VARPTR var ;
{
  StrGen("m",var);
  AddForName1(var->el[0],str,NULL,var->stack_position);
  StrGen("n",var);
  AddForName1(var->el[1],str,NULL,var->stack_position);
  StrGen("it",var); 
  AddForName1(var->el[2],str,NULL,var->stack_position);
}

void ForSPARSE(var)   VARPTR var ;
{
  StrGen("m",var);
  AddForName1(var->el[0],str,NULL,var->stack_position);
  StrGen("n",var);
  AddForName1(var->el[1],str,NULL,var->stack_position);
}


void ForROW(var)   VARPTR var ;
{
  StrGen("n",var);
  AddForName1(var->el[0],str,NULL,var->stack_position);
}

void ForCOLUMN(var)   VARPTR var ;
{
  StrGen("m",var);
  AddForName1(var->el[0],str,NULL,var->stack_position);
}

void ForVECTOR(var)   VARPTR var ;
{
  if ( var->for_type == EXTERNAL )
    {
      if ( var->list_el == 0 ) 
	sprintf(str,"me%d",var->stack_position);
      else 
	sprintf(str,"l%dme%d",var->stack_position,var->list_el);
      AddForName1(var->el[0],str,NULL,var->stack_position);
    }
  else 
    {
      if ( var->list_el == 0 ) 
	{
	  sprintf(str,"m%d*n%d",var->stack_position,var->stack_position);
	  sprintf(str1,"mn%d",var->stack_position);
	  AddForName1(var->el[0],str,str1,var->stack_position);
	}
      else 
	{
	  sprintf(str,"l%dm%d*l%dn%d",var->stack_position,var->list_el,
		  var->stack_position,var->list_el);
	  sprintf(str1,"l%dmn%d",var->stack_position,var->list_el);
	  AddForName1(var->el[0],str,str1,var->stack_position);
	}
    }
}

void ForPOLYNOM(var)   VARPTR var ;
{
  StrGen("m",var);
  AddForName1(var->el[0],str,NULL,var->stack_position);
}

/** special case for scalars : we add a for_name to var itself 
  since var can be used as a dimension of other variables **/

void ForSCALAR(var)   VARPTR var ;
{
  StrGen("m",var);
  AddForName1(var->vpos,str,NULL,var->stack_position);
}


void ForPOINTER(var)   VARPTR var ;
{
}

void ForANY(var)   VARPTR var ;{}

void ForLIST(var)   VARPTR var ;{} 

void ForTLIST(var)   VARPTR var ;{};


void ForSEQUENCE(var)   VARPTR var ;
{  
  fprintf(stderr,"Wrong type in For function \n");
};

void ForEMPTY(var)    VARPTR var ;
{  
  fprintf(stderr,"Wrong type in For function \n");
};


void ForWORK(var)    VARPTR var ;
{  
  fprintf(stderr,"Wrong type in For function \n");
};

void ForDIMFOREXT(var)    VARPTR var ;
{  
  
};

typedef  struct  {
  int type;
  void  (*fonc) __PARAMS((VARPTR var));} ForRhsTab ;


ForRhsTab FORTAB[] = {
  {DIMFOREXT,ForDIMFOREXT},
  {COLUMN,ForCOLUMN},
  {LIST,ForLIST},
  {TLIST,ForTLIST},
  {MATRIX,ForMATRIX},
  {POLYNOM,ForPOLYNOM},
  {ROW,ForROW},
  {SCALAR,ForSCALAR},
  {SEQUENCE,ForSEQUENCE},
  {STRING,ForSTRING},
  {WORK,ForWORK},
  {EMPTY,ForEMPTY},
  {ANY,ForANY},
  {VECTOR,ForVECTOR},
  {STRINGMAT,ForMATRIX},
  {SCIMPOINTER,ForPOINTER},
  {IMATRIX,ForIMATRIX},
  {SCISMPOINTER,ForPOINTER},
  {SCILPOINTER,ForPOINTER},
  {BMATRIX,ForMATRIX},
  {SCIBPOINTER,ForPOINTER},
  {SCIOPOINTER,ForPOINTER},
  {SPARSE,ForSPARSE}
};


void FixForNames()
{
  int i;
  VARPTR var,var1;
  for (i = 0; i < basfun->nin ; i++) 
    {
      int j;
      var = variables[i];
      (*(FORTAB[var->type].fonc))(var);
      /** If we have a list we must also explore the list elements **/
      if ( var->type == LIST || var->type == TLIST ) 
	{
	  for ( j = 0 ; j < nVariable ; j++) 
	    {
	      var1 = variables[j];
	      if ( var1->stack_position == i+1  && var1->list_el != 0) 
		(*(FORTAB[var1->type].fonc))(var1);
	    }
	}
    }
  for (i = basfun->nin ; i < nVariable ; i++) 
    {
      var = variables[i];
      if ( var->for_type == EXTERNAL )
	{
	  (*(FORTAB[var->type].fonc))(var);
	}
    }
}


