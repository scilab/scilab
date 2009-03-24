/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file is released into the public domain
 *
 */

import java.io.*;
import javasci.*; 

public class Example7A {

  public static void main(String[] args)
  {
  	int col=4;
 	  int row=2;
 	  int r,c;
 	  double [] b=new double[row*col];
 	  
 	  SciDoubleArray a = new SciDoubleArray("A",row,col);
 	  Scilab.Exec("A=[1,2,3,4;5 6 7 8];");
 		a.Get();
 		b=a.getData();
 		
 		System.out.println("Example 7 A Serialize");
 		System.out.println("----------------------------------------------------");
  	for(r=0;r<=(a.getNumberOfRows()*a.getNumberOfCols())-1;r++)
    {
       System.out.print(""+b[r]+"  ");
    }
    System.out.println("");
  	System.out.println("----------------------------------------------------");
  	
 		try 
    {
     	FileOutputStream fichier = new FileOutputStream("Example7.ser");
     	ObjectOutputStream oos = new ObjectOutputStream(fichier);
     	oos.writeObject(a);
     	oos.flush();
     	oos.close();
   	}
    	
    catch (java.io.IOException e) 
    {
      e.printStackTrace();
    }
    
    Scilab.Finish();
    
  }
}
