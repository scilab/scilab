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


class Example4 {

  public static void main(String[] args) 
  {
    SciString S1 = new SciString("Sentence","Who is the best ?");
    
    S1.disp();
    
    SciString S2 = new SciString("Sentence2",S1);

    Scilab.Exec("Sentence2='Scilab is the best.';");
    S2.disp();
    
    SciString S3 = new SciString("Sentence2");
    S3.disp();

		Scilab.Finish();
    
  }
}

