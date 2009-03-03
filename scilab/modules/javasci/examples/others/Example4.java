/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released into the public domain
 *
 */

import javasci.* ; 


class Example4 {

  public static void main(String[] args) 
  {
    SciString S1 = new SciString("Sentence","Who is the best ?");
    
    S1.disp();
    
    SciString S2 = new SciString("Sentence2",S1);

    Scilab.Exec("Sentence2='Scilab is the best.';");
    S2.disp();
    
    SciString S3 = new SciString("Sentence2");
    S3.disp();

		Scilab.Finish();
    
  }
}

