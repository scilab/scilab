/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget;

import javax.swing.SwingUtilities;

public class Test {

    public static void main(String[] args) throws Exception {
        final XMLToUIComponentConverter converter = new XMLToUIComponentConverter(args[0]);
        long t = System.currentTimeMillis();
        converter.convert();
        System.out.println(System.currentTimeMillis() - t);
    }
}