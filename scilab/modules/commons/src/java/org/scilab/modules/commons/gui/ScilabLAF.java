/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scialb Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.commons.gui;

import javax.swing.AbstractButton;

public final class ScilabLAF {

    public static void setDefaultProperties(AbstractButton button) {
        if (button != null) {
            button.setFocusable(false);
            button.setContentAreaFilled(true);
            button.setOpaque(false);
        }
    }
}
