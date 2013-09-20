/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.textbox;

import org.scilab.modules.gui.text.Text;

/**
 * Interface for textbox associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface TextBox extends Text {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleTextBox getAsSimpleTextBox();

}
