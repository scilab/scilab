/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

import javasci.SciDoubleArray;
import javasci.Scilab;
import java.io.FileInputStream;
import java.io.ObjectInputStream;

/**
 * @deprecated
 */
class Example7B {

    public static void main(String[] args)
        {
            try 
            {
                FileInputStream fichier = new FileInputStream("Example7.ser");
                ObjectInputStream ois = new ObjectInputStream(fichier);
      
                int col=4;
                int row=2;
                int r,c;
                double [] b=new double[row*col];
      
                SciDoubleArray a = (SciDoubleArray) ois.readObject();
                a.Send(); /* a MUST be send to Scilab just after ois.readObject*/
      
                b=a.getData();
      
                System.out.println("Example 7 B DeSerialize");
                System.out.println("----------------------------------------------------");
                for(r=0;r<=(a.getNumberOfRows()*a.getNumberOfCols())-1;r++)
                {
                    System.out.print(""+b[r]+"  ");
                }
                System.out.println("");
                System.out.println("----------------------------------------------------");
        
            } 
    
            catch (java.io.IOException e) 
            {
                e.printStackTrace();
            }
            catch (ClassNotFoundException e) 
            {
                e.printStackTrace();
            }
    
            Scilab.Finish();
    
        }
}
       
