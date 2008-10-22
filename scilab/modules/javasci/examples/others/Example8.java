/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import javasci.* ; 

class Example8 {

  public static void main(String[] args) 
  {
  
    SciString SCIPATH = new SciString("SCILABROOTPATH");
    Scilab.Exec("SCILABROOTPATH=getenv('SCI');");
    
    Scilab.ExecuteScilabScript(SCIPATH.getData()+"/modules/javasci/examples/others/test.sce");
    
    /* "notest.sce" not exist */
    if (! Scilab.ExecuteScilabScript(SCIPATH.getData()+"/modules/javasci/examples/others/notest.sce") )
    {
    	System.out.println("Scilab Error code: "+Scilab.GetLastErrorCode());
    }
     
    Scilab.Finish();
  }
}

  
