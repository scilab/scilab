/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

import javasci.Scilab;
import javasci.SciComplex;


/**
 * @deprecated
 */
class Example10 {

    public static void main(String[] args) {
      
        SciComplex a = new SciComplex("A");
        Scilab.Exec("A=1. + 80*%i");
        System.out.println("----------------------------------------------------");
        a.disp();
        System.out.println("----------------------------------------------------");
      
        System.out.println("A= "+a.getRealPartData()+"  "+a.getImaginaryPartData()+"  ");
        System.out.println("----------------------------------------------------");
        SciComplex s = new SciComplex("S",4,8);
        s.disp();
        System.out.println("----------------------------------------------------");
        System.out.println("S= "+s.getRealPartData()+"  "+s.getImaginaryPartData()+"  ");
        System.out.println("----------------------------------------------------");
        SciComplex Ap = new SciComplex("A");
        Ap.disp();
    }
}

