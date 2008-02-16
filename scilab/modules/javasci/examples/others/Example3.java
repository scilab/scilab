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

class Example3 {

  public static void main(String[] args) {
        
  SciDoubleArray a = new SciDoubleArray("A",4,1, new double [] {1,2,3,4} );
  SciDoubleArray b = new SciDoubleArray("B",4,1, new double [] {3,1,2,4} );
  SciDoubleArray c = new SciDoubleArray("C",4,1, new double [] {0,0,0,0} );
  
  Scilab.Exec("disp(A);");
  Scilab.Exec("disp(B);");
  Scilab.Exec("disp(C);");
  
  a.disp();
  b.disp();
  c.disp();
    
  Scilab.Exec("C=A+B;");
  a.Get();
  b.Get();
  c.Get();
  
  a.disp();
  b.disp();
  c.disp();
  
  SciDoubleArray d = new SciDoubleArray("C",4,1);
  d.disp();
  
  Scilab.Finish();
  
	}
}
  
