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
        }

        return editor;
    }
}
