/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import javasci.* ; 
import javax.swing.UIManager;



class  bug_3673 {

  public static void main(String[] args) {

		 String previousLookAndFeel = UIManager.getLookAndFeel().toString();

     new SciDouble("foobar");
     
     String newLookAndFeel = UIManager.getLookAndFeel().toString();
     if (newLookAndFeel.equals(previousLookAndFeel)) {
    }
    else
    {
    	 System.out.println("bug 3673 not fixed.");
    }
    Scilab.Finish();
  }
}

