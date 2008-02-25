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
/*****************************************************************************/
class Real1 
{
/*****************************************************************************/
  public static void main(String[] args) 
    {
			SciDoubleArray A = new SciDoubleArray("A",4,1, new double [] {1.1,2.2,3.3,4.4} );
      SciDoubleArray B = new SciDoubleArray("B",4,1, new double [] {4.4,3.3,2.2,1.1} );
  		SciDoubleArray C = new SciDoubleArray("C",4,1, new double [] {1. ,1. ,1. ,1.} );
  		SciDoubleArray D = new SciDoubleArray("Dim",1,2);
  		SciDoubleArray E = new SciDoubleArray("E",C);
  		
  		
  		Scilab.Exec("disp('A=');disp(A);");
  		Scilab.Exec("disp('B=');disp(B);");
			Scilab.Exec("disp('C=');disp(C);");
			Scilab.Exec("disp('E=');disp(E);");
			
  		Scilab.Exec("C=A+B;");
  		Scilab.Exec("Dim=size(C);");
			
			Scilab.Exec("disp('A=');disp(A);");
  		Scilab.Exec("disp('B=');disp(B);");
			Scilab.Exec("disp('C=');disp(C);");
			Scilab.Exec("disp('Dim=');disp(Dim);");
			Scilab.Exec("disp('E=');disp(E);");
			
			Scilab.Finish();

    }
/*****************************************************************************/
}
/*****************************************************************************/
