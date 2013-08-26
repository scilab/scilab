/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import javax.swing.tree.TreeModel;

/**
 * The tree table model interface
 * @author Calixte DENIZET
 */
public interface ScilabTreeTableModel extends TreeModel {

    /**
     * @return the number of available columns.
     */
    public int getColumnCount();

    /**
     * @param column the column number
     * @return the name for column number
     */
    public String getColumnName(int column);

    /**
     * @param column the column number
     * @return the type for column number
     */
    public Class getColumnClass(int column);

    /**
     * @param node the node
     * @param column the column number
     * @return the value to be displayed for node at column number
     */
    public Object getValueAt(Object node, int column);

    /**
     * @param node the node
     * @param column the column number
     * @return true if the cell is editable
     */
    public boolean isCellEditable(Object node, int column);

    /**
     * @param value the value to set
     * @param node the node
     * @param column the column number
     */
    public void setValueAt(Object value, Object node, int column);
}
