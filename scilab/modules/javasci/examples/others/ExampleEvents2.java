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

class  ExampleEvents2
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

