/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released into the public domain
 *
 */

import javasci.* ; 

class Example2 {

  public static void main(String[] args) {
  
    // Creation of a 4x4 matrix
    SciDoubleArray a = new SciDoubleArray("A",4,4);
    // Randomly fill it (uniform law on (0,1)).
    Scilab.Exec("A=rand(4,4)");
    // Scilab display of the matrix. 
    a.disp();
    SciDoubleArray b = new SciDoubleArray("B",2,2, new double [] {1,2,3,4} );
    b.disp();
    SciDoubleArray v = new SciDoubleArray ("Void",0,0);
    v.disp();
    // Be carreful to the ' (must be doubled)
    // We execute a Scilab instruction
    Scilab.Exec("write(%io(2),'coucou')");
    SciDoubleArray h = new SciDoubleArray("H",2,2, new double [] {1,2,3,4} );
    // We send h to Scilab and execute a Scilab instruction
    Scilab.Exec("disp(H)");
    
    SciDoubleArray hp = new SciDoubleArray("H",2,2);
    hp.disp();
    
    Scilab.Finish();
       
  }
}
