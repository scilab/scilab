/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.ui_data.variableeditor;

/**
 * Interface for Variable editor.
 */
public interface SimpleVariableEditor {

    /**
     * Set data to be displayed
     * @param name the variable name
     * @param type the variable type
     * @param data : the data displayed in JTable
     */
    void setData(String name, String type, Object[][] data);
}
