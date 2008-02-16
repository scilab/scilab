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


class Example6 {

  public static void main(String[] args) {
  double b;
  	
  	SciDouble a = new SciDouble("A");
  	Scilab.Exec("A=4;");
  	a.Get();
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	b=a.getData();
  	
  	System.out.println(b);
    
  	System.out.println("----------------------------------------------------");
 	  SciDouble d = new SciDouble("D",8);
    d.Send();
    Scilab.Exec("P=D*2;");
    Scilab.Exec("Q=P*3;");
    
    SciDouble q = new SciDouble("Q");
    Scilab.Exec("Q=P*3;");
    q.Get();
    q.disp();
    System.out.println("----------------------------------------------------");
    Scilab.Finish();
  }
}

