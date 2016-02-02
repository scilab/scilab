/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
package org.scilab.modules.ui_data.variablebrowser;

/**
 * Interface for Variable browser.
 */
public interface SimpleVariableBrowser {

    /**
     * Set columns title
     * @param columnNames : the columns Title
     */
    void setColumnNames(String[] columnNames);

    /**
     * Set data to be displayed
     * @param data : the data displayed in JTable
     */
    void setData(Object[][] data);

    /**
     * Update the row filtering
     */
    void updateRowFiltering();

}
