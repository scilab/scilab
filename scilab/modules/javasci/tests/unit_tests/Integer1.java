/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
class Integer1 
{
/*****************************************************************************/
  public static void main(String[] args) 
    {
			SciIntegerArray A = new SciIntegerArray("A",4,1, new int [] {1,2,3,4} );
      SciIntegerArray B = new SciIntegerArray("B",4,1, new int [] {4,3,2,1} );
  		SciIntegerArray C = new SciIntegerArray("C",4,1, new int [] {1 ,1 ,1 ,1} );
  		SciIntegerArray D = new SciIntegerArray("Dim",1,2);
  		SciIntegerArray E = new SciIntegerArray("E",C);
  		
  		
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
