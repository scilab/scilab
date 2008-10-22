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


class Example10 {

  public static void main(String[] args) {
  	
  	SciComplex a = new SciComplex("A");
		Scilab.Exec("A=1. + 80*%i");
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	
   	System.out.println("A= "+a.getRealPartData()+"  "+a.getImaginaryPartData()+"  ");
  	System.out.println("----------------------------------------------------");
  	SciComplex s = new SciComplex("S",4,8);
  	s.disp();
  	System.out.println("----------------------------------------------------");
  	System.out.println("S= "+s.getRealPartData()+"  "+s.getImaginaryPartData()+"  ");
  	System.out.println("----------------------------------------------------");
  	SciComplex Ap = new SciComplex("A");
  	Ap.disp();
  }
}

