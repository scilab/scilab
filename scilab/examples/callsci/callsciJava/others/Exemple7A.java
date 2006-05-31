/* Allan CORNET INRIA 2006 */
import java.io.*;
import javasci.*; 

public class  Exemple7A {

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
 		
 		System.out.println("Exemple 7 A Serialize");
 		System.out.println("----------------------------------------------------");
  	for(r=0;r<=(a.getRow()*a.getCol())-1;r++)
    {
       System.out.print(""+b[r]+"  ");
    }
    System.out.println("");
  	System.out.println("----------------------------------------------------");
  	
 		try 
    {
     	FileOutputStream fichier = new FileOutputStream("Exemple7.ser");
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
