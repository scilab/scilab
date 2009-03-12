/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released into the public domain
 *
 */

import javasci.Scilab;
import javasci.SciString;

class Example4 {

  public static void main(String[] args) 
  {
    SciString S1 = new SciString("VariableSentence","Who is the best ?");
    
    S1.disp();
    
    SciString S2 = new SciString("VariableSentence2",S1);

    Scilab.Exec("VariableSentence2='Scilab is the best.';");
    S2.disp();
    
    SciString S3 = new SciString("VariableSentence2");
    S3.disp();

	Scilab.Finish();
    
  }
}

