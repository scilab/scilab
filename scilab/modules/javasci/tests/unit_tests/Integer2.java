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
class Integer2 
{
/*****************************************************************************/
public static void main(String[] args) 
{
  SciIntegerArray F = new SciIntegerArray("F",2,2, new int [] {1,2,3,4} );
  SciIntegerArray G = new SciIntegerArray("G",2,2, new int [] {5,6,7,8} );
  SciIntegerArray DimH = new SciIntegerArray("DimH",1,2);

	Scilab.Exec("disp('F=');disp(F);");
	Scilab.Exec("disp('G=');disp(G);");
  
  Scilab.Exec("H=F*G;");
  Scilab.Exec("DimH=size(H);");
      
  int DxDy[]=DimH.getData();
  
  SciIntegerArray H = new SciIntegerArray("H",(int)DxDy[0],(int)DxDy[1]);
  Scilab.Exec("H=F*G;");
  
  H.Get();
  
  Scilab.Exec("disp('H=');disp(H);");
  Scilab.Finish();
}
/*****************************************************************************/
}
/*****************************************************************************/
