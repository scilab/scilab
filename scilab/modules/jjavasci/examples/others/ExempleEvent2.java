import javasci.* ; 
import java.lang.Thread.*;
import java.io.*;

  class LoopEvent extends Thread
  {
  	public void run()
  	{
  		int i=0;
  		Scilab.Exec("plot2d();");
  		System.out.println("Appuyer sur 'q' et 'return' pour quitter");
  		do 
  		{
  			Scilab.Events();
  		}
  		while ( (i==0) && ( Scilab.HaveAGraph() != false) );
  		Scilab.Events();
  		System.out.println("Fenetre Graphique fermee");
  		Scilab.Finish();
  	}
  }

class  ExempleEvent2
{

  public static void main(String[] args)  throws Exception

  {
    	char c;
	    Thread thread = new LoopEvent();
    	thread.start();

  	  	
  	
  		while (System.in.read()!='q')
  		{
  			try
   			{
  		 		Thread.sleep( 1 );
  			}
  			catch ( InterruptedException e )
   			{
   			}

  		}
  		thread.stop();
  }

}

