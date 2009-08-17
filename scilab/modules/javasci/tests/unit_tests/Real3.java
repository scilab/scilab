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
class Real3 
{
/*****************************************************************************/
	private static void DisplayResult(String name,double []mat,int r,int c)
	{
		int i=0;
		
		System.out.println("row= "+r);
		System.out.println("col= "+c);
		System.out.println(name+"= ");
		for(i=0;i<r;i++)
		{
			System.out.print(mat[i]+" ");
		}
		System.out.println(" ");
	}
	
/*****************************************************************************/
  public static void main(String[] args) 
    {
			SciDoubleArray A = new SciDoubleArray("A",4,1, new double [] {1.1,2.2,3.3,4.4} );
      SciDoubleArray B = new SciDoubleArray("B",4,1, new double [] {4.4,3.3,2.2,1.1} );
  		SciDoubleArray C = new SciDoubleArray("C",4,1, new double [] {1. ,1. ,1. ,1.} );
  		SciDoubleArray D = new SciDoubleArray("Dim",1,2);
  		SciDoubleArray E = new SciDoubleArray("E",C);
  		
  		Scilab.Exec("C=A+B;");
  		Scilab.Exec("Dim=size(C);");
			
			DisplayResult(A.getName(),A.getData(),A.getNumberOfRows(),A.getNumberOfCols());
			DisplayResult(B.getName(),B.getData(),B.getNumberOfRows(),B.getNumberOfCols());
			DisplayResult(C.getName(),C.getData(),C.getNumberOfRows(),C.getNumberOfCols());
			DisplayResult(E.getName(),E.getData(),E.getNumberOfRows(),E.getNumberOfCols());
			
      Scilab.Finish();
    }
/*****************************************************************************/
}
/*****************************************************************************/  
