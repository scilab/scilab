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
import java.io.*;



class ExampleEvents
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

