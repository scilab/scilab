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

package org.scilab.modules.ui_data.rowheader;

import javax.swing.JList;
import javax.swing.JTable;

/**
 * Class for the RowHeader
 * @author Allan SIMON
 */
public class RowHeader extends JList {

    /**
     * Constructor
     * @param rowHeaderModel the model for row header
     * @param table the table
     */
    public RowHeader(RowHeaderModel rowHeaderModel, JTable table) {
        super(rowHeaderModel);
        setFixedCellWidth(50);
        setFixedCellHeight(table.getRowHeight()
                           + table.getRowMargin()
                           - table.getIntercellSpacing().height);
        setCellRenderer(new RowHeaderRenderer());
    }
}
