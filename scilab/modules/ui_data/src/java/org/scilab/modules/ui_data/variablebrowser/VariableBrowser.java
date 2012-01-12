/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.variablebrowser;

/**
 * Variable browser Interface
 * Describes actions exposed to scilab.
 *
 */
public interface VariableBrowser {
   
    /**
     * Close Variable Browser.
     */
    void close();
    
    /**
     * Set Data stored in JTable.
     * @param data : the data to be displayed.
     */
    void setData(Object[][] data);
    
    /**
     * Update row filtering
     */
    void updateRowFiltering();
    
}
