/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released into the public domain
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
  
