/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

import javasci.Scilab; 

/**
 * @deprecated
 */
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

                System.out.println("Java loop "+i);
                i++;
            }
            System.out.println("Graphics window closed");
            Scilab.Finish();
        }

}

