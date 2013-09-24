/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor;

import org.scilab.modules.gui.window.SimpleWindow;

/**
 * Variable editor Interface
 * Describes actions exposed to scilab.
 *
 */
public interface VariableEditor extends SimpleWindow {

    /**
     * Close Variable Browser.
     */
    void close();

    /**
     * Set Data stored in JTable.
     * @param name the name of the Scilab var
     * @param type the type of the var
     * @param data : the data to be stored.
     */
    void updateData(String name, String type, Object[][] data);

    /**
     * Set Data stored in JTable.
     * @param name the name of the Scilab var
     * @param type the type of the var
     * @param data : the data to be stored.
     */
    void updateData(String name, String type, Object[][] data, double[] rowsIndex, double[] colsIndex);

    /**
     * Set the name of the variable being edited in Editvar
     * @param variableName : name of the variable being edited.
     */
    void setVariableName(String variableName);

    /**
     * Get the name of the variable being edited in Editvar
     * @return the variable name
     */
    String getVariablename();
}
