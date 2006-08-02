/* Allan CORNET INRIA 2006 */
import java.io.*;
import javasci.*; 

class  Exemple7B {

  public static void main(String[] args)
  {
  	  try 
    {
      FileInputStream fichier = new FileInputStream("Exemple7.ser");
      ObjectInputStream ois = new ObjectInputStream(fichier);
      
      int col=4;
  	  int row=2;
  	  int r,c;
  	  double [] b=new double[row*col];
      
      SciDoubleArray a = (SciDoubleArray) ois.readObject();
      a.Send(); /* a MUST be send to Scilab just after ois.readObject*/
      
      b=a.getData();
      
      System.out.println("Exemple 7 B DeSerialize");
      System.out.println("----------------------------------------------------");
  		for(r=0;r<=(a.getRow()*a.getCol())-1;r++)
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
  	
