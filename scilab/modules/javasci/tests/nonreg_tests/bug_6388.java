/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import javasci.* ; 
class  bug_6388 {

  public static void main(String[] args) {
    SciDouble U=new SciDouble("u",1.0);
    SciDouble V=new SciDouble("v");
    Scilab.Exec("v = u + 1;");
    System.out.println("Name: " + V.getName());
    System.out.println("Value: " + V.getData());   
    V.disp();
    Scilab.Finish();
  }
}

