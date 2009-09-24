/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - INRIA - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: GetCommandArg.h                                                  */
/* desc : tools to retrieve parameters within the command line for        */
/*        graphic routines.                                               */
/*------------------------------------------------------------------------*/

#include "GetCommandArg.h"
#include "GetProperty.h"
#include "DefaultCommandArg.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "Scierror.h"
static char logFlagsCpy[3] ; /* real logflags may use either this or the stack */

/*--------------------------------------------------------------------------*/
/* get_style */
/*--------------------------------------------------------------------------*/
int get_style_arg(char *fname,int pos, int n1,rhs_opts opts[], int ** style )
{
  int m,n,l,first_opt=FirstOpt(),kopt,un=1,ix,i,l1;

  Nbvars = Max(Nbvars,Rhs);


  if ( pos < first_opt ) /* regular argument  */
  { 
    if (VarType(pos))
    {
	    GetRhsVar(pos,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
	    if (m * n < n1)
      {
	      Scierror(999,_("%s: Wrong size for input argument #%d: %d < %d expected.\n"),fname,pos, m*n,n1);
	      return 0;
	    }
	    if ( n1 == 1 && m * n == 1 )
      {
	      ix = 2;
	      CreateVar(Nbvars+1,MATRIX_OF_INTEGER_DATATYPE,&un,&ix,&l1);
	      *istk(l1)=*istk(l);
	      *istk(l1+1)=1;
	      l=l1;
	    }
	    *style = istk(l);
    }
    else /* zero type argument --> default value */
	  {
	    ix = Max(n1,2);
	    CreateVar(Nbvars+1,MATRIX_OF_INTEGER_DATATYPE,&un,&ix,&l);
	    for ( i = 0 ; i < n1 ; ++i )
      {
        *istk(l + i) = i+1 ;
      }
	    if (n1 == 1)
      {
        *istk(l + 1) = 1 ;
      }
	    *style = istk(l);
	  }
  }
  else if ((kopt=FindOpt("style",opts)))
  { /* named argument: style=value */
    GetRhsVar(kopt,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
    if (m * n < n1)
    {
		Scierror(999,_("%s: Wrong size for input argument #%d: %d < %d expected.\n"),fname,kopt,m*n,n1);
      return 0;
    }
    if (n1==1&&m*n==1)
    {
      ix = 2;
      CreateVar(Nbvars+1,MATRIX_OF_INTEGER_DATATYPE,&un,&ix,&l1);
      *istk(l1)=*istk(l);
      *istk(l1+1)=1;
      l=l1;
    }
    *style = istk(l);
  }
  else /* unspecified argument --> default value */
  {
    ix = Max(n1,2);
    CreateVar(Nbvars+1,MATRIX_OF_INTEGER_DATATYPE,&un,&ix,&l);
    for (i = 0 ; i < n1 ; ++i)
    {
      *istk(l + i) = i+1;
    }
    if (n1 == 1)
    {
      *istk(l +1) = 1;
    }
    *style = istk(l);
  }
  return 1;
}
/*--------------------------------------------------------------------------*/
/* get_rect */
/*--------------------------------------------------------------------------*/
int get_rect_arg(char *fname,int pos,rhs_opts opts[], double ** rect )
{
	int m,n,l,first_opt=FirstOpt(),kopt,i;

	if (pos < first_opt) 
		{ 
			if (VarType(pos)) {
				GetRhsVar(pos,MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
				if (m * n != 4) { 
					Scierror(999,"%s: Wrong size for input argument #%d: %d expected\n",fname,pos,4); 
					return 0;
				}
				*rect = stk(l);
	
				for(i=0;i<4;i++)
					if(finite((*rect)[i]) == 0){
						Scierror(999,"%s: Wrong values (Nan or Inf) for input argument: %d finite values expected\n",fname,4); 
						return 0;
					}
			}
			else 
				{
					/** global value can be modified  **/
					double zeros[4] = { 0.0, 0.0, 0.0, 0.0 } ;
					setDefRect( zeros ) ;
					*rect = getDefRect() ;
				}
		}
	else if ((kopt=FindOpt("rect",opts))) {/* named argument: rect=value */
		GetRhsVar(kopt,MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
		if (m * n != 4) { 
			Scierror(999,"%s: Wrong size for input argument #%d: %d expected\n",fname,kopt,4); 
			return 0;
		}
		*rect = stk(l);
    
		for(i=0;i<4;i++)
			if(finite((*rect)[i]) == 0){
				Scierror(999,"%s: Wrong values (Nan or Inf) for input argument: %d finite values expected\n",fname,4); 
				return 0;
			}
	}
	else
		{
			/** global value can be modified  **/
			double zeros[4] = { 0.0, 0.0, 0.0, 0.0 } ;
			setDefRect( zeros ) ;
			*rect = getDefRect() ;
		}
    
	return 1;
}
/*--------------------------------------------------------------------------*/
int get_strf_arg(char *fname,int pos,rhs_opts opts[], char ** strf )
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt)
  { 
    if (VarType(pos))
    {
	    GetRhsVar(pos,STRING_DATATYPE, &m, &n, &l);
	    if ( m * n != 3 )
      { 
		  Scierror(999,_("%s: Wrong size for input argument #%d: String of %d characters expected.\n"),fname,pos, 3);
	      return 0;
	    }
	  *strf = cstk(l); 
    }
    else
	  {
	    /* def value can be changed */
      reinitDefStrf() ;
      *strf = getDefStrf() ;
	  }
  }
  else if ((kopt=FindOpt("strf",opts)))
  {
    GetRhsVar(kopt,STRING_DATATYPE, &m, &n, &l);
    if (m * n != 3)
    { 
		Scierror(999,_("%s: Wrong size for input argument #%d: String of %d characters expected.\n"),fname,kopt,3);
		return 0;
    }
    *strf = cstk(l); 
  }
  else
  {
    /* def value can be changed */
      
    reinitDefStrfN() ;
    *strf = getDefStrf() ;
   
  }
  return 1;
}

/*--------------------------------------------------------------------------*/
int get_legend_arg(char *fname,int pos,rhs_opts opts[], char ** legend )
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
	GetRhsVar(pos,STRING_DATATYPE, &m, &n, &l);
	*legend = cstk(l); 
      }
      else
	{
	  *legend = getDefLegend() ;
	}
    }
  else if ((kopt=FindOpt("leg",opts))) {
    GetRhsVar(kopt,STRING_DATATYPE, &m, &n, &l);
    *legend = cstk(l); 
  }
  else
    {
      *legend = getDefLegend() ;
    }
  return 1;
}
/*--------------------------------------------------------------------------*/
/**
 * retrieve the labels from the command line  and store them into Legend
 */
int get_labels_arg(char *fname,int pos,rhs_opts opts[], char ** labels )
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
  { 
    if (VarType(pos)) {
      GetRhsVar(pos,STRING_DATATYPE, &m, &n, &l);
      *labels = cstk(l); 
    }
    else
    {

      /* jb silvy 03/2006 */
      /* do not change the legend if one already exists */
      sciPointObj * pSubWin = sciGetCurrentSubWin() ;
      if ( sciGetLegendDefined( pSubWin ) )
      {
        *labels = NULL ;
      }
      else
      {
        *labels = getDefLegend() ;
      }
    }
  }
  else if ((kopt=FindOpt("leg",opts)))
  {
    GetRhsVar(kopt,STRING_DATATYPE, &m, &n, &l);
    *labels = cstk(l); 
  }
  else
  {

    /* jb silvy 03/2006 */
    /* do not change the legend if one already exists */
    sciPointObj * pSubWin = sciGetCurrentSubWin() ;
    if ( sciGetLegendDefined( pSubWin ) )
    {
      *labels = NULL ;
    }
    else
    {
      *labels = getDefLegend() ;
    }
  }
  return 1;
}

/*--------------------------------------------------------------------------*/
int get_nax_arg(int pos,rhs_opts opts[], int ** nax, BOOL * flagNax )
{
  int i,m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt)
  {
    if (VarType(pos))
    {
      GetRhsVar(pos,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
      CheckLength(pos,m*n,4);
      for (i = 0 ; i < 4; ++i)
      {
        // When i = 1 or 3 we talk about the number of ticks, this value can be -1 to say 'AutoTicks'
        *istk(l+i) = Max((int)  *istk(l+i),-(i%2));
      }
      *nax=istk(l);
      *flagNax = TRUE;
    }
    else  
    {
	    *nax = getDefNax() ;
	    *flagNax = FALSE;
    }
  }
  else if ((kopt=FindOpt("nax",opts)))
  {
    GetRhsVar(kopt,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
    CheckLength(kopt,m*n,4);
    for (i = 0 ; i < 4; ++i)
    {
      // When i = 1 or 3 we talk about the number of ticks, this value can be -1 to say 'AutoTicks'
      *istk(l+i) = Max((int)  *istk(l+i),-(i%2));
    }
    *nax=istk(l);
    *flagNax = TRUE;
  }
  else 
  {
    *nax = getDefNax() ;
    *flagNax = FALSE;
  } 
  return 1;
}


/*--------------------------------------------------------------------------*/
int get_zminmax_arg(char *fname,int pos,rhs_opts opts[], double ** zminmax )
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
        GetRhsVar(pos,MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
	if (m * n != 2) { 
      Scierror(999,"%s: Wrong size for input argument #%d: %d expected\n",fname,pos,2);
	  return 0;
	}
	*zminmax = stk(l); 
      }
      else 
	{
	  /** global value can be modified  **/
    double zeros[2] = { 0.0, 0.0 } ;
    setDefZminMax( zeros ) ;
	  *zminmax = getDefZminMax() ;
	}
    }
  else if ((kopt=FindOpt("zminmax",opts))) {/* named argument: rect=value */
    GetRhsVar(kopt,MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
    if (m * n != 2) { 
      Scierror(999,"%s: Wrong size for input argument #%d: %d expected\n",fname,kopt,2);
      return 0;
    }
    *zminmax = stk(l); 
  }
  else
    {
      /** global value can be modified  **/
      double zeros[2] = { 0.0, 0.0 } ;
      setDefZminMax( zeros ) ;
      *zminmax = getDefZminMax() ;
    }
    
  return 1;
}

/*--------------------------------------------------------------------------*/
int get_colminmax_arg(char *fname,int pos,rhs_opts opts[], int ** colminmax )
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    {
      if (VarType(pos)) 
	{
	  GetRhsVar(pos,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
	  CheckLength(pos,m*n,2);
	  *colminmax=istk(l);
	}
      else
	{
	  /** global value can be modified  **/
    int zeros[2] = { 0, 0 } ;
    setDefColMinMax( zeros ) ;
	  *colminmax = getDefColMinMax() ;
	}
    }
  else if ((kopt=FindOpt("colminmax",opts))) 
    {
      GetRhsVar(kopt,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
      CheckLength(kopt,m*n,2);
      *colminmax=istk(l);
    }
  else 
    {
      /** global value can be modified  **/
      int zeros[2] = { 0, 0 } ;
      setDefColMinMax( zeros ) ;
      *colminmax = getDefColMinMax() ;
    } 
  return 1;
}

/*--------------------------------------------------------------------------*/
int get_colout_arg(char *fname,int pos,rhs_opts opts[], int ** colout )
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    {
      if (VarType(pos)) 
	{
	  GetRhsVar(pos,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
	  CheckLength(pos,m*n,2);
	  *colout = istk(l);
	}
      else
	{
	  /** global value can be modified  **/
    int newDefCO[2] = { -1, -1 } ;
    setDefColOut( newDefCO ) ;
	  *colout = getDefColOut() ;
	}
    }
  else if ((kopt=FindOpt("colout",opts))) 
    {
      GetRhsVar(kopt,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
      CheckLength(kopt,m*n,2);
      *colout=istk(l);
    }
  else 
    {
      /** global value can be modified  **/
      int newDefCO[2] = { -1, -1 } ;
      setDefColOut( newDefCO ) ;
      *colout = getDefColOut() ;
    } 
  return 1;
}
/*--------------------------------------------------------------------------*/
int get_with_mesh_arg(char *fname,int pos,rhs_opts opts[], BOOL * withMesh)
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    {
      if (VarType(pos)) 
	{
	  GetRhsVar(pos,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &l);
	  CheckLength(pos,m*n,1);
	  *withMesh = *(istk(l));
	}
      else
	{
	  /** global value can be modified  **/
	  setDefWithMesh( FALSE );
	  *withMesh = getDefWithMesh() ;
	}
    }
  else if ((kopt=FindOpt("mesh",opts))) 
    {
      GetRhsVar(kopt,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &l);
      CheckLength(kopt,m*n,1);
      *withMesh = *(istk(l));
    }
  else 
    {
      /** global value can be modified  **/
      setDefWithMesh( FALSE );
      *withMesh = getDefWithMesh() ;
    } 
  return 1;
}

/*--------------------------------------------------------------------------*/
int get_logflags_arg(char *fname,int pos,rhs_opts opts[], char ** logFlags )
{
  int m,n,l,first_opt=FirstOpt(),kopt;
  
  if (pos < first_opt) /* regular argument  */
  { 
    if (VarType(pos))
    {
	    GetRhsVar(pos,STRING_DATATYPE, &m, &n, &l);
	    if ((m * n != 2)&&(m * n != 3))
      {
		  Scierror(999,"%s: Wrong size for input argument #%d: %d or %d expected\n",fname, pos, 2, 3);
	      return 0;
	    }
	    if (m * n == 2)
      {
	      if ((*cstk(l)!='l'&&*cstk(l)!='n')||(*cstk(l+1)!='l'&&*cstk(l+1)!='n'))
        {
	        Err=pos;
	        Error(116);
	        return 0;
	      }
	      logFlagsCpy[0]='g';
        logFlagsCpy[1]=*cstk(l);
        logFlagsCpy[2]=*cstk(l+1) ;
        *logFlags = logFlagsCpy ;
      }
	    else 
      {
	      if (((*cstk(l)!='g')&&(*cstk(l)!='e')&&(*cstk(l)!='o')) || 
	          (*cstk(l+1)!='l'&&*cstk(l+1)!='n') || 
	          (*cstk(l+2)!='l'&&*cstk(l+2)!='n'))
        {
	        Err=pos;
	        Error(116);
	        return 0;
	      }
        *logFlags = cstk(l) ;  
      }
      
    }
    else /* zero type argument --> default value */
	  {
      *logFlags = getDefLogFlags() ;
	  }
  }
  else if ((kopt=FindOpt("logflag",opts)))
  { /* named argument: style=value */
    GetRhsVar(kopt,STRING_DATATYPE, &m, &n, &l);
    if ((m * n != 2)&&(m * n != 3))
    {
	  Scierror(999,"%s: Wrong size for input argument #%d: %d or %d expected\n",fname, kopt, 2, 3);
      return 0;
    }
    if (m * n == 2)
    {
      if ((*cstk(l)!='l'&&*cstk(l)!='n')||(*cstk(l+1)!='l'&&*cstk(l+1)!='n'))
      {
	      Err=kopt;
	      Error(116);
	      return 0;
      }
      logFlagsCpy[0]='g';
      logFlagsCpy[1]=*cstk(l);
      logFlagsCpy[2]=*cstk(l+1) ;
      *logFlags = logFlagsCpy ;
    }
    else
    {
      if (((*cstk(l)!='g')&&(*cstk(l)!='e')&&(*cstk(l)!='o')) || 
	         (*cstk(l+1)!='l'&&*cstk(l+1)!='n') || 
	         (*cstk(l+2)!='l'&&*cstk(l+2)!='n'))
      {
	      Err=kopt;
	      Error(116);
	      return 0;
      }
      
      *logFlags = cstk(l) ;
    }
  }
  else /* unspecified argument --> default value */
  {
    *logFlags = getDefLogFlags() ;
  }
  return 1;
}
/*--------------------------------------------------------------------------*/
int get_optional_double_arg(     char  * fname,
                                  int    pos  ,
                                 char  * name ,
                               double ** value,
                                  int    sz   ,
                             rhs_opts    opts[] ) 
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
	GetRhsVar(pos,MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
	CheckLength(pos,m*n,sz)
	  *value = stk(l);
      }
    }
  else if ((kopt=FindOpt(name,opts))) {
    GetRhsVar(kopt,MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
    CheckLength(kopt,m*n,sz)
      *value = stk(l);
  }
  return 1;
}
/*--------------------------------------------------------------------------*/
int get_optional_int_arg(     char  * fname,
                               int    pos  ,
                              char  * name ,
                               int ** value,
                               int    sz   ,
                          rhs_opts    opts[] ) 
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
	GetRhsVar(pos,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
	CheckLength(pos,m*n,sz)
	  *value = istk(l);
      }
    }
  else if ((kopt=FindOpt(name,opts))) {
    GetRhsVar(kopt,MATRIX_OF_INTEGER_DATATYPE, &m, &n, &l);
    CheckLength(kopt,m*n,sz)
      *value = istk(l);
  }
  return 1;
}
/*--------------------------------------------------------------------------*/
