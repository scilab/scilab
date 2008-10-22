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


class Example11 {

  public static void main(String[] args) {
  	int col=4;
  	int row=2;
  	int r,c;
  	boolean [] b=new boolean[row*col];
  	
  	SciBooleanArray a = new SciBooleanArray("A",row,col);
  	SciBooleanArray bsci = new SciBooleanArray("B",row,col);
  	
  	Scilab.Exec("A=[%t,%t,%t,%f;%f %t %t %t];");
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	for(r=1;r<=a.getNumberOfRows();r++)
    {
     for(c=1;c<=a.getNumberOfCols();c++)
     {
     	System.out.print("A("+(r)+","+(c)+")="+a.GetElement(r,c)+"  ");
     }
     System.out.println(" ");
    }
  	System.out.println("----------------------------------------------------");
  	
  	b=a.getData();
  	
  	for(r=0;r<row;r++)
    {
     for(c=0;c<col;c++)
     {
     	System.out.print("b("+r+","+(c)+")="+ b[c*row+r] +"  ");
     }
     System.out.println(" ");
    }
  	System.out.println("----------------------------------------------------");
  	Scilab.Exec("B=~A;");
    bsci.disp();
    System.out.println("----------------------------------------------------");
		System.out.println("A ScilabType : "+Scilab.TypeVar("A"));
		System.out.println("B ScilabType : "+Scilab.TypeVar("B"));
		System.out.println("----------------------------------------------------");
    Scilab.Finish();
  }
}

