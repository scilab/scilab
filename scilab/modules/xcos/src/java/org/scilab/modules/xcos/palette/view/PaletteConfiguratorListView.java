/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.palette.view;

import java.awt.Color;

import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the edit category view
 */
@SuppressWarnings(value = { "serial" })
public class PaletteConfiguratorListView extends JTable {
    private static final String[] COLUMN_TITLE = { XcosMessages.ENABLE,
                                                   XcosMessages.PAL_NAME
                                                 };

    /**
     * Construct a new view with model
     *
     * @param model
     *            the model
     */
    public PaletteConfiguratorListView(final PaletteListModel model) {
        super(model);
        setBackground(Color.white);
    }

    /**
     * The default model
     */
    public static class PaletteListModel extends AbstractTableModel {
        private final Category category;

        /**
         * Default constructor with data
         *
         * @param category
         *            the data
         */
        public PaletteListModel(final Category category) {
            super();

            this.category = category;
        }

        /**
         * @param rowIndex
         *            the current row
         * @param columnIndex
         *            the current column
         * @return always true
         * @see javax.swing.table.AbstractTableModel#isCellEditable(int, int)
         */
        @Override
        public boolean isCellEditable(final int rowIndex, final int columnIndex) {
            return true;
        }

        /**
         * @param column
         *            the column
         * @return the name
         * @see javax.swing.table.AbstractTableModel#getColumnName(int)
         */
        @Override
        public String getColumnName(final int column) {
            return COLUMN_TITLE[column];
        }

        /**
         * @param columnIndex
         *            the column
         * @return the class
         * @see javax.swing.table.AbstractTableModel#getColumnClass(int)
         */
        @Override
        public Class<?> getColumnClass(final int columnIndex) {
            if (columnIndex == 0) {
                return Boolean.class;
            } else {
                return String.class;
            }
        }

        /**
         * @return always two
         * @see javax.swing.table.TableModel#getColumnCount()
         */
        @Override
        public int getColumnCount() {
            return COLUMN_TITLE.length;
        }

        /**
         * @return the number of {@link PaletteNode}
         * @see javax.swing.table.TableModel#getRowCount()
         */
        @Override
        public int getRowCount() {
            return category.getNode().size();
        }

        /**
         * @param rowIndex
         *            the row
         * @param columnIndex
         *            the column
         * @return the associated data
         * @see javax.swing.table.TableModel#getValueAt(int, int)
         */
        @Override
        public Object getValueAt(final int rowIndex, final int columnIndex) {
            final PaletteNode p = category.getNode().get(rowIndex);

            Object ret;
            if (columnIndex == 0) {
                ret = p.isEnable();
            } else {
                ret = p.toString();
            }

            return ret;
        }

        /**
         * @param aValue
         *            the value to set
         * @param rowIndex
         *            the row
         * @param columnIndex
         *            the column
         * @see javax.swing.table.AbstractTableModel#setValueAt(java.lang.Object,
         *      int, int)
         */
        @Override
        public void setValueAt(final Object aValue, final int rowIndex,
                               final int columnIndex) {
            final PaletteNode p = category.getNode().get(rowIndex);

            if (columnIndex == 0) {
                p.setEnable((Boolean) aValue);
            } else {
                p.setName((String) aValue);
            }

            // Refresh the data
            PaletteManagerView.updateTree();
        }
    }
}
