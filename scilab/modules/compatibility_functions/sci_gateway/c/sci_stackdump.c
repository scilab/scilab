/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
int sci_stackdump(char *fname,unsigned long fname_len);
/*-----------------------------------------------------------------------------------*/
int sci_stackdump(char *fname,unsigned long fname_len)
{
  int input_type = 0;
  int m=0, n=0, l=0;

  CheckLhs(1,1);
  CheckRhs(0,1);
  
  if(Rhs==0)
    {
      /* Display all imformations about Scilab stack (all variables) */
      scistackdump("all",0);
    }
  else if(Rhs==1)
    {
      input_type = GetType(Top);
      /* Character string should be: */
      /* "all", "top", "bot", "global", "infos" */
      if(input_type==sci_strings)
        {
          GetRhsVar(Top,"c",&m,&n,&l);
          if(n!=1)
            {
              //error_scilab(999,"Wrong input dimensions.");
              return(0);
            }
          if(!strcmp(cstk(l),"all"))
            scistackdump("all",0);
          else if(!strcmp(cstk(l),"top"))
            scistackdump("top",0);
          else if(!strcmp(cstk(l),"bot"))
            scistackdump("bot",0);
          else if(!strcmp(cstk(l),"global"))
            scistackdump("global",0);
          else if(!strcmp(cstk(l),"infos"))
            scistackdump("infos",0);
          else
            {
              //error_scilab(999,"Wrong input: should be  \"all\", \"top\", \"bot\", \"global\", \"infos\".");
              return(0);
            }            
        }
      else if(input_type==sci_matrix)
        {
          GetRhsVar(Top,"d",&m,&n,&l);
          if(n!=1)
            {
              //error_scilab(999,"Wrong input dimensions.");
              return(0);
            }
          scistackdump("pos",(int)(*stk(l)));
        }
      //else
        //error_scilab(999,"Wrong input type.");
    }

  return 0;
}
/*-----------------------------------------------------------------------------------*/
