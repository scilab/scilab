/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import javasci.* ; 

/**
 * Test the display of a SciDoubleArray
 * @author INRIA
 */
class Disp 
{

  public static void main(String[] args) 
	{
    SciDoubleArray A = new SciDoubleArray("A",4,1, new double [] {100.1,102.2,103.3,104.4} );
		A.Send();
		Scilab.Exec("disp('A=');disp(A)");
		Scilab.Finish();
	}
}

