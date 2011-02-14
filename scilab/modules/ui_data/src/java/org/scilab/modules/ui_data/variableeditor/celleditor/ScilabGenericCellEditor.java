/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.celleditor;

import java.awt.Component;
import java.util.Map;
import java.util.WeakHashMap;
import java.util.Iterator;

import javax.swing.DefaultCellEditor;
import javax.swing.JFormattedTextField;
import javax.swing.JTable;

/**
 * @author Calixte DENIZET
 */
public class ScilabGenericCellEditor extends DefaultCellEditor {

    private static final long serialVersionUID = -5713675180587317165L;

    private Map<Coords, String> expressions = new WeakHashMap();

    /**
     * Constructor
     */
    public ScilabGenericCellEditor() {
        super(new JFormattedTextField());
    }

    /**
     * @param value the value
     * @return the Scilab's representation of the value
     */
    public String getDataAsScilabString(Object value) {
        return "";
    }

    /**
     * {@inheritDoc}
     */
    public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int col) {
        String exp = getExpression(row, col);
        if (exp == null) {
            return super.getTableCellEditorComponent(table, getDataAsScilabString(value), isSelected, row, col);
        } else {
            return super.getTableCellEditorComponent(table, "=" + exp, isSelected, row, col);
        }
    }

    /**
     * @param map the expressions
     */
    public void setExpressions(Map<Coords, String> map) {
        expressions = map;
    }

    /**
     * @return the expressions
     */
    public Map<Coords, String> getExpressions() {
        return expressions;
    }

    /**
     * @param exp the expression
     * @param row the row
     * @param col the col
     */
    public void addExpression(String exp, int row, int col) {
        expressions.put(new Coords(row, col), exp);
    }

    /**
     * @param row the row
     * @param col the col
     * @return the expression
     */
    public String getExpression(int row, int col) {
        return expressions.get(new Coords(row, col));
    }

    /**
     * @param row the row
     * @param col the col
     */
    public void removeExpression(int row, int col) {
        expressions.remove(new Coords(row, col));
    }

    /**
     * @param var the variable name
     * @return all the expressions in one scilab expression
     */
    public String getAllExpressions(String var) {
        String exp = "";
        Iterator<Coords> iter = expressions.keySet().iterator();
        while (iter.hasNext()) {
            Coords c = iter.next();
            exp += var + "(" + (c.x + 1) + "," + (c.y + 1) + ")=" + expressions.get(c) + ";";
        }

        return exp;
    }

    /**
     * Inner class to handle the coords (row, col)
     */
    private class Coords {
        protected int x;
        protected int y;
        private int hashcode;

        /**
         * Constructor
         * @param x the row
         * @param y the col
         */
        Coords(int x, int y) {
            this.x = x;
            this.y = y;
            /* this formula is an explicit bijection between NxN and N. */
            this.hashcode = x + (x + y) * (x + y + 1) / 2;
        }

        /**
         * {@inheritDoc}
         */
        public int hashCode() {
            return hashcode;
        }

        /**
         * {@inheritDoc}
         */
        public boolean equals(Object o) {
            return (o instanceof Coords) && ((Coords) o).x == x && ((Coords) o).y == y;
        }
    }
}
