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
class Integer3 
{
/*****************************************************************************/
	private static void DisplayResult(String name,int []mat,int r,int c)
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
			SciIntegerArray A = new SciIntegerArray("A",4,1, new int [] {1,2,3,4} );
      SciIntegerArray B = new SciIntegerArray("B",4,1, new int [] {4,3,2,1} );
  		SciIntegerArray C = new SciIntegerArray("C",4,1, new int [] {1 ,1 ,1 ,1} );
  		SciIntegerArray D = new SciIntegerArray("Dim",1,2);
  		SciIntegerArray E = new SciIntegerArray("E",C);
  		
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
