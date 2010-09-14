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

package org.scilab.modules.ui_data.rowheader;

import javax.swing.AbstractListModel;
import javax.swing.table.AbstractTableModel;

/**
 * @author Bruno JOFRET
 */
public class RowHeaderModel extends AbstractListModel {

    private int size;

    /**
     * Constructor
     * @param tableModel the model
     */
    public RowHeaderModel(AbstractTableModel tableModel) {
        size = tableModel.getRowCount();
    }

    /**
     * {@inheritDoc}
     */
    public int getSize() {
        return this.size;
    }

    /**
     * {@inheritDoc}
     */
    public Object getElementAt(int index) {
        return (Integer) index + 1;
    }
}
