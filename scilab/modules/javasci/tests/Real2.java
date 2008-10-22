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
class Real2 
{
/*****************************************************************************/
public static void main(String[] args) 
{
  SciDoubleArray F = new SciDoubleArray("F",2,2, new double [] {1,2,3,4} );
  SciDoubleArray G = new SciDoubleArray("G",2,2, new double [] {5,6,7,8} );
  SciDoubleArray DimH = new SciDoubleArray("DimH",1,2);

	Scilab.Exec("disp('F=');disp(F);");
	Scilab.Exec("disp('G=');disp(G);");
  
  Scilab.Exec("H=F*G;");
  Scilab.Exec("DimH=size(H);");
      
  double DxDy[]=DimH.getData();
  
  SciDoubleArray H = new SciDoubleArray("H",(int)DxDy[0],(int)DxDy[1]);
  Scilab.Exec("H=F*G;");
  
  H.Get();
  
  Scilab.Exec("disp('H=');disp(H);");

}
/*****************************************************************************/
}
/*****************************************************************************/
