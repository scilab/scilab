/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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
