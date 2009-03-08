/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released into the public domain
 *
 */

import javasci.* ; 
import java.lang.Thread.*;
import java.io.*;

  class LoopEvent extends Thread
  {
  	public void run()
  	{
  		System.out.println("Press 'q' and 'return' to quit.");
  		Scilab.Exec("plot2d();quit");
  		do 
  		{
  			Scilab.Events();
  		}
  		while ( Scilab.HaveAGraph() != false );
  		Scilab.Events();
  		System.out.println("Graphics window closed");
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
  		System.out.println("'q' pressed.");
  		Scilab.Events();
  		if (Scilab.HaveAGraph()) {
  		Scilab.Exec("xdel(winsid());quit");
  	}
  	  thread.stop();
  		Scilab.Finish();
  		
  }

}

