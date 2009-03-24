/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA 
 * 
 * This file is released into the public domain
 *
 */

import javasci.* ; 

class Example5 {

  public static void main(String[] args) 
  {
  	int col=4;
  	int row=2;
  	int r,c,k;
  	String [] b=new String [row*col];
  	String [] d=new String[row*col];
  	
  	SciStringArray a = new SciStringArray("A",row,col);
  	Scilab.Exec("A=['Aaa','Bbb','Ccc','Ddd';'Eee','Fff','Ggg','Hhh'];");
  	a.Get();
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

		
		for (k=0;k<row*col;k++)
		{
			d[k]=new String("Sentence ")+k;
		}
		
		SciStringArray D = new SciStringArray("D",2,4,d);
		D.disp();
		
		SciStringArray Dp = new SciStringArray("D",2,4);
		Dp.disp();
		
		Scilab.Finish();

  	
    
  }
}

