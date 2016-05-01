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

package org.scilab.modules.ui_data.variableeditor.celleditor;

import javax.swing.DefaultCellEditor;

import org.scilab.modules.ui_data.EditVar;

/**
 * Renderer factory
 * @author Allan SIMON
 * @author Bruno JOFRET
 */
public final class CellEditorFactory {

    /**
     * Constructor
     */
    private CellEditorFactory() { }

    /**
     * @param type the type
     * @return the CellEditor
     */
    public static DefaultCellEditor createCellEditor(String type) {
        DefaultCellEditor editor = new ScilabStringCellEditor();
        if (type.equals(EditVar.STRING)) {
            editor = new ScilabStringCellEditor();
        } else if (type.equals(EditVar.COMPLEX)) {
            editor = new ScilabComplexCellEditor();
        } else if (type.equals(EditVar.DOUBLE)) {
            editor = new ScilabDoubleCellEditor();
        } else if (type.equals(EditVar.BOOLEAN)) {
            editor = new ScilabBooleanCellEditor();
        } else if (type.equals(EditVar.INTEGER)) {
            editor = new ScilabIntegerCellEditor();
        } else if (type.equals(EditVar.SPARSE)) {
            editor = new ScilabDoubleCellEditor();
        } else if (type.equals(EditVar.COMPLEXSPARSE)) {
            editor = new ScilabComplexCellEditor();
        } else if (type.equals(EditVar.BOOLEANSPARSE)) {
            editor = new ScilabBooleanCellEditor();
        }

        return editor;
    }
}
