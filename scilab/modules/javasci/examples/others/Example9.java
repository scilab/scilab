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


class Example9 {

  public static void main(String[] args) {
  	int col=4;
  	int row=2;
  	int r,c;
  	double [] b=new double[row*col];
  	
  	
  	SciComplexArray a = new SciComplexArray("A",row,col);
		Scilab.Exec("A=[ 1. + 10*%i,2. + 20*%i,3. + 30*%i ,4. + 40*%i;  5. + 50*%i,6. + 60*%i,7. + 70*%i ,8. + 80*%i]");
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	for(r=1;r<=a.getNumberOfRows();r++)
    {
     for(c=1;c<=a.getNumberOfCols();c++)
     {
     	System.out.print("A("+(r)+","+(c)+")="+a.GetRealPartElement(r,c)+"  "+a.GetImaginaryPartElement(r,c)+"  ");
     }
     System.out.println(" ");
    }
  	System.out.println("----------------------------------------------------");
  	SciComplexArray s = new SciComplexArray("S",1,4,new double [] {1,2,3,4},new double [] {5,6,7,8});
  	s.disp();
  	System.out.println("----------------------------------------------------");
  	SciComplexArray sp = new SciComplexArray("S",1,4);
  	sp.disp();
  	System.out.println("----------------------------------------------------");
  	Scilab.Finish();
  	
  }
}

