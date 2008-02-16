/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import javasci.* ; 


class  Boolean {

  public static void main(String[] args) {

    boolean b;
  	
  	SciBoolean a = new SciBoolean("A");
  	Scilab.Exec("A=%t;");
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	b=a.getData();
		
  	System.out.println(b);
    
  	System.out.println("----------------------------------------------------");
	  SciBoolean d = new SciBoolean("D",true);
	  SciBoolean p = new SciBoolean("P");
    Scilab.Exec("P=~D;");
    p.disp();
    System.out.println("----------------------------------------------------");
    Scilab.Finish();
  }
}

