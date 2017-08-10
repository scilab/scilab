/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.modelica;

import static org.scilab.modules.xcos.modelica.TerminalAccessor.getData;
import static org.scilab.modules.xcos.modelica.TerminalAccessor.values;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import javax.swing.event.TableModelEvent;
import javax.swing.table.AbstractTableModel;

import org.scilab.modules.xcos.modelica.TerminalAccessor.ChangeEvent;
import org.scilab.modules.xcos.modelica.model.Terminal;

/**
 * Implement a table model for a {@link Terminal} list
 */
@SuppressWarnings(value = { "serial" })
public final class TerminalTableModel extends AbstractTableModel {
    /**
     * Specialized event with a before commit flag.
     *
     * On each modification, this event is fired twice; one before the change
     * (beforeCommit == true) and one after the change (beforeCommit == false).
     */
    @SuppressWarnings(value = { "serial" })
    public static final class TerminalTableModelEvent extends TableModelEvent {
        private final boolean beforeCommit;

        /**
         * @param source
         *            the source
         * @param firstRow
         *            the first row
         * @param lastRow
         *            the last row
         * @param column
         *            the column
         * @param type
         *            the type
         * @param beforeCommit
         *            the before commit flag
         */
        public TerminalTableModelEvent(TerminalTableModel source, int firstRow,
                                       int lastRow, int column, int type, boolean beforeCommit) {
            super(source, firstRow, lastRow, column, type);
            this.beforeCommit = beforeCommit;
        }

        /**
         * @param terminalTableModel
         *            the source
         * @param rowIndex
         *            the row
         * @param columnIndex
         *            the column
         * @param beforeCommit
         *            before commit status
         */
        public TerminalTableModelEvent(TerminalTableModel terminalTableModel,
                                       int rowIndex, int columnIndex, boolean beforeCommit) {
            this(terminalTableModel, rowIndex, rowIndex, columnIndex, UPDATE,
                 beforeCommit);
        }

        /**
         * @return true if the event has been fired before the data commit,
         *         false otherwise.
         */
        public boolean isBeforeCommit() {
            return beforeCommit;
        }

        /**
         * @return true if the event has been fired false the data commit, false
         *         otherwise.
         */
        public boolean isAfterCommit() {
            return !beforeCommit;
        }
    }

    /**
     * Pass {@link TerminalAccessor} change events to {@link TableModelEvent}.
     */
    private static class ModelChangeListener implements
        TerminalAccessor.ChangeListener {
        private final TerminalTableModel model;

        /**
         * Default Constructor
         *
         * @param model
         *            the model
         */
        public ModelChangeListener(TerminalTableModel model) {
            this.model = model;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void change(ChangeEvent event) {
            final TerminalAccessor source = (TerminalAccessor) event
                                            .getSource();
            final Terminal terminal = event.getTerminal();

            final int columnIndex = Arrays.asList(TerminalAccessor.values())
                                    .indexOf(source);
            final int rowIndex = model.getTerminals().indexOf(terminal);
            if (rowIndex < model.getRowCount()
                    && columnIndex < model.getColumnCount()) {
                model.fireTableChanged(new TableModelEvent(model, rowIndex,
                                       rowIndex, columnIndex));
            }
        }

    }

    private List<Terminal> terminals;

    /**
     * Default constructor
     */
    public TerminalTableModel() {
        setTerminals(null);

        for (int i = 0; i < TerminalAccessor.values().length; i++) {
            final TerminalAccessor row = TerminalAccessor.values()[i];
            row.addChangeListener(new ModelChangeListener(this));
        }
    }

    /**
     * @return the terminals
     */
    public List<Terminal> getTerminals() {
        return terminals;
    }

    /**
     * @param terminals
     *            the terminals to set
     */
    public void setTerminals(List<Terminal> terminals) {
        if (terminals == null) {
            this.terminals = Collections.emptyList();
        } else {
            this.terminals = terminals;
        }

        fireTableDataChanged();
    }

    /*
     * Implement table methods
     */

    /**
     * @return the row count of the terminal
     * @see javax.swing.table.TableModel#getRowCount()
     */
    @Override
    public int getRowCount() {
        return terminals.size();
    }

    /**
     * @return the column count
     * @see javax.swing.table.TableModel#getColumnCount()
     */
    @Override
    public int getColumnCount() {
        return values().length - 1; // the selected data is just a flag
    }

    /**
     * @param column
     *            the current column index
     * @return the name of the column
     * @see javax.swing.table.AbstractTableModel#getColumnName(int)
     */
    @Override
    public String getColumnName(int column) {
        return values()[column].getName();
    }

    /**
     * @param columnIndex
     *            the current column index
     * @return the class of the column
     * @see javax.swing.table.AbstractTableModel#getColumnClass(int)
     */
    @Override
    public Class<?> getColumnClass(int columnIndex) {
        return values()[columnIndex].getKlass();
    }

    /**
     * @param rowIndex
     *            the row index
     * @param columnIndex
     *            the column index
     * @return the editable state
     * @see javax.swing.table.AbstractTableModel#isCellEditable(int, int)
     */
    @Override
    public boolean isCellEditable(int rowIndex, int columnIndex) {
        return values()[columnIndex].isEditable();
    }

    /**
     * @param rowIndex
     *            the row index
     * @param columnIndex
     *            the column index
     * @return the object value
     * @see javax.swing.table.TableModel#getValueAt(int, int)
     */
    @Override
    public Object getValueAt(int rowIndex, int columnIndex) {
        return getData(values()[columnIndex], terminals.get(rowIndex));
    }

    /**
     * @param aValue
     *            the new value
     * @param rowIndex
     *            the row index
     * @param columnIndex
     *            the column index
     * @see javax.swing.table.AbstractTableModel#setValueAt(java.lang.Object,
     *      int, int)
     */
    @Override
    public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
        fireTableChanged(new TerminalTableModelEvent(this, rowIndex,
                         columnIndex, true));
        values()[columnIndex].setData(aValue, terminals.get(rowIndex));
        fireTableChanged(new TerminalTableModelEvent(this, rowIndex,
                         columnIndex, false));
    }
}
