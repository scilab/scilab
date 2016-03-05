/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
