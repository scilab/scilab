#include <stdlib.h>

#include "intersci-n.h"
#include "getrhs.h"
#include "crerhs.h"
#include "check.h"

static  char size[MAXNAM];
static  char data[MAXNAM];

/***********************************************
 * Optional variables old style 
 ***********************************************/

void WriteOptArg(f,var)
     VARPTR var;
     FILE *f;
{
  char size[MAXNAM];
  char data[MAXNAM];

  Fprintf(f,indent++,"if( Rhs <= %d) \n", 
	  var->stack_position-1 );
  Fprintf(f,indent,"{\n");

  switch (var->opt_type) 
    {
    case NAME:
      AddDeclare(DEC_LOGICAL,"optvarget");
      Fprintf(f,indent,"if (.not.optvarget(fname,topk,%d,'%s       ')) return\n",
	      var->stack_position,var->opt_name);
      break;
    case VALUE:
      switch (var->type) 
	{
	case SCALAR:
	  (*(CRERHSTAB[var->type].fonc))(f,var);
	  Fprintf(f,indent,"stk(lr%d)= %s\n",var->stack_position,
		  var->opt_name);
	  break;
	case MATRIX:
	  OptvarGetSize(var->opt_name,size,data);

	  AddDeclare1(DEC_DATA,"%s dat%d[]= %s",
		      SGetCDec(var->for_type),
		      var->stack_position,data);
	  Fprintf(f,indent,"m%d = 1;n%d = %s;\n",var->stack_position,
		  var->stack_position,size);
	  (*(CRERHSTAB[var->type].fonc))(f,var);
	  AddDeclare1(DEC_INT,"un=1");
	  Fprintf(f,indent,"C2F(%scopy)(&m%d,dat%d,&un,%s(l%d),&un);\n",
		  SGetForTypeAbrev(var),
		  var->stack_position,
		  var->stack_position,
		  SGetForTypeStack(var),var->stack_position);

	  break;
	case STRING:
	  Fprintf(f,indent,"m%d = %d\n",var->stack_position,
		  strlen(var->opt_name));
	  (*(CRERHSTAB[var->type].fonc))(f,var);
	  Fprintf(f,indent,"call stringcopy(m%d,'%s',cstk(lr%d))\n",
		  var->stack_position,var->opt_name,var->stack_position);
	  break;
	case SCIOPOINTER :
	  (*(CRERHSTAB[var->type].fonc))(f,var);
	  break;
	default:
	  printf("Optional variable with value must be \"SCALAR\" or \"STRING\"\n");
	  exit(1);
	  break;
	}
      break;
    }

  Fprintf(f,indent--,"}\n");
  Fprintf(f,indent++,"else\n");
  Fprintf(f,indent,"{\n");
  (*(RHSTAB[var->type].fonc))(f,var,0);
  Fprintf(f,indent--,"}\n");
}

/***********************************************
 * Optional variables new style 
 * first phase : checking arguments 
 ***********************************************/

void WriteOptArgPhase2(f,i)
     IVAR i;
     FILE *f;
{
  VARPTR var = variables[basfun->in[i]-1];
  int opt_posi = basfun->NewMaxOpt - (basfun->nin - var->stack_position)-1;
  Fprintf(f,indent,"/* default value to optional argument %s */\n",var->name);
  switch (var->opt_type) 
    {
    case NAME:
      Fprintf(f,indent++,"if ( opts[%d].position == -1 ){\n",opt_posi);
      Fprintf(f,indent,"iopos++ ; opts[%d].position = iopos;\n",opt_posi);
      AddDeclare1(DEC_INT,"opt%d",opt_posi);
      Fprintf(f,indent,"GetMatrixptr(\"%s\",&opts[%d].m, &opts[%d].n,&opt%d);\n",
	      var->opt_name,opt_posi,opt_posi,opt_posi);
      Fprintf(f,indent,"CreateVarFrom(opts[%d].position,\"d\",&opts[%d].m, &opts[%d].n,&opts[%d].l,&opt%d)\n",
	      opt_posi,opt_posi,opt_posi,opt_posi,opt_posi);
      ChangeForName2(var,"%s(opts[%d].l)",SGetForTypeStack(var), opt_posi);
      Fprintf(f,--indent,"}\n");
      break;
    case VALUE:
      switch (var->type) 
	{

	case SCALAR:
	  sprintf(data,"{%s}",var->opt_name);
	  OptvarGetSize(var->opt_name,size,data);
	  OptMATRIX(f,var);
	  break;

	case MATRIX:
	  OptvarGetSize(var->opt_name,size,data);
	  OptMATRIX(f,var);
	  break;
	case STRING:
	  sprintf(size,"%d",strlen(var->opt_name));
	  sprintf(data,"\"%s\"",var->opt_name);
	  OptMATRIX(f,var);
	  break;
	case SCIOPOINTER  :
	  sprintf(size,"size-[%d]",strlen(var->opt_name));
	  sprintf(data,"%s",var->opt_name);
	  OptOpointer(f,var);
	  break;
	default:
	  printf("Optional variable with value must be \"SCALAR\" or \"STRING\"\n");
	  exit(1);
	  break;
	}
      break;
    }
}


void OptMATRIX(f,var)
     FILE *f;
     VARPTR var;
{
  int opt_posi = basfun->NewMaxOpt - (basfun->nin - var->stack_position)-1;
  Fprintf(f,indent++,"if ( opts[%d].position == -1 ){\n",opt_posi);
  Fprintf(f,indent,"iopos++ ; opts[%d].position = iopos;\n",opt_posi);
  AddDeclare1(DEC_DATA,"%s xdat%d[]= %s, *dat%d = xdat%d",
	      SGetCDec(var->for_type),
	      opt_posi,data,opt_posi,opt_posi);
  AddDeclare1(DEC_INT,"m%d",var->stack_position);
  AddDeclare1(DEC_INT,"n%d",var->stack_position);
  AddDeclare1(DEC_INT,"l%d",var->stack_position);

  switch ( var->type ) 
    {
    case MATRIX : 
      Fprintf(f,indent,"opts[%d].m = 1;opts[%d].n = %s;  opts[%d].type = \"%s\";\n",
	      opt_posi,opt_posi,size,opt_posi,SGetForTypeAbrev(var));
      break;
    case STRING : 
      Fprintf(f,indent,"opts[%d].m = %s;opts[%d].n = 1;  opts[%d].type = \"%s\";\n",
	      opt_posi,size,opt_posi,opt_posi,SGetForTypeAbrev(var));
      break;
    case SCALAR:
      Fprintf(f,indent,"opts[%d].m = 1;opts[%d].n = 1;\n",opt_posi,opt_posi);
      break;

    }
  Fprintf(f,indent,"CreateVarFromPtr(opts[%d].position,opts[%d].type,&opts[%d].m,&opts[%d].n,&dat%d);\n",
	  opt_posi,opt_posi,opt_posi,opt_posi,
	  opt_posi);
  Fprintf(f,indent,"opts[%d].l = VarPtr(opts[%d].position);\n",opt_posi, opt_posi);
  switch ( var->type ) 
    {
    case MATRIX : 
      ChangeForName2(variables[var->el[0]-1],"opts[%d].m",opt_posi);
      ChangeForName2(variables[var->el[1]-1],"opts[%d].n",opt_posi);
      break;
    case STRING :
      ChangeForName2(variables[var->el[0]-1],"opts[%d].m",opt_posi);
      break;
    }
  ChangeForName2(var,"%s(opts[%d].l)",SGetForTypeStack(var), opt_posi);
  Fprintf(f,--indent,"}\n");
  Fprintf(f,indent," else { \n"); 
  /* should be optimized to exploit dimension infos stored in opts */
  Fprintf(f,indent++,"GetRhsVar(%d,\"%s\",&m%d,&n%d,&l%d);\n",var->stack_position,
  	  SGetForTypeAbrev(var),var->stack_position,var->stack_position,var->stack_position);
  (*(CHECKTAB[var->type].fonc))(f,var,0);
  Fprintf(f,--indent,"} \n");
}

void OptOpointer(f,var)
     FILE *f;
     VARPTR var;
{

  Fprintf(f,indent++,"if ( %sok == 0) {\n",var->name);
  Fprintf(f,indent,"%spos=iopos=iopos+1;\n",var->name);
  Fprintf(f,indent,"CreateSCO(iopos,&lr%d,%s);\n",
	  var->stack_position,data);
  AddDeclare1(DEC_INT,"lr%d",var->stack_position);
  Fprintf(f,--indent,"}\n");

}


/*****************************************
 * get the size of an optional argument 
 * which is coded as "(size)/..../" in the 
 * optvar string 
 * and the next characters are stored in data 
 *****************************************/

void OptvarGetSize(optvar,size,data)
     char *optvar,*size,*data;
{
  int i,j=0,ok=0;
  for ( i = 0 ; i < (int) strlen(optvar) ; i++ ) 
    {
      if ( optvar[i] == ')' ) 
	{ 
	  size[j++] = '\0'; break;
	}
      if ( ok ==1 ) size[j++]= optvar[i];
      if ( optvar[i] == '(' ) ok =1 ;
    }
  if ( i < (int) strlen(optvar)) strcpy(data,optvar+i+1);
  ok=0;
  for ( i = 0 ; i < (int) strlen(data) ; i++ ) 
    if ( data[i] == '/' ) 
      {
	if (ok ==0 ) {
	  data[i]='{';ok=1;}
	else
	  data[i]='}';
      }
}







