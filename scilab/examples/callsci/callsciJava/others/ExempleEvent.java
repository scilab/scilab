import javasci.* ; 
import java.io.*;



class  ExempleEvent
{
  
  public static void main(String[] args)  throws Exception

  {
    int i=0;
        
	  Scilab.Exec("plot3d();");
  	
  	while (Scilab.HaveAGraph()!=false)
  	{
  		Scilab.Events();
  		try
   		{
  		 	Thread.sleep( 1 );
  		}
  		catch ( InterruptedException e )
   		{
   		}
   		
   		System.out.println("Boucle Java sans fin "+i);
		  i++;
  	}
  	Scilab.Finish();
  	
  }

}

