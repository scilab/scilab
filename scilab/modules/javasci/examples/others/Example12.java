/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

import javasci.Scilab; 
import javasci.SciBoolean; 

/**
 * @deprecated
 */
class Example12 {

    public static void main(String[] args) {

        boolean b;
      
        SciBoolean a = new SciBoolean("A");
        Scilab.Exec("A=%t;");
        System.out.println("----------------------------------------------------");
        a.disp();
        System.out.println("----------------------------------------------------");
        b=a.getData();
      
        System.out.println(b);
    
        System.out.println("----------------------------------------------------");
        SciBoolean d = new SciBoolean("D",true);
        d.Send();
        SciBoolean p = new SciBoolean("P");
        Scilab.Exec("P=~D;");
        p.disp();
        System.out.println("----------------------------------------------------");
        SciBoolean dp = new SciBoolean("D");
        dp.disp();
    
        Scilab.Finish();
    }
}

