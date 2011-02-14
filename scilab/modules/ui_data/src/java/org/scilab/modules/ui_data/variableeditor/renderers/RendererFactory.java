/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.renderers;

import javax.swing.table.DefaultTableCellRenderer;

import org.scilab.modules.ui_data.EditVar;

/**
 * Renderer factory
 * @author Allan SIMON
 * @author Bruno JOFRET
 */
public final class RendererFactory {

    /**
     * Constructor
     */
    private RendererFactory() { }

    /**
     * @param type the type
     * @return the CellRenderer
     */
    public static DefaultTableCellRenderer createRenderer(String type) {
        DefaultTableCellRenderer renderer = new ScilabStringRenderer();
        if (type.equals(EditVar.STRING)) {
            renderer = new ScilabStringRenderer();
        } else if (type.equals(EditVar.COMPLEX)) {
            renderer = new ScilabComplexRenderer();
        } else if (type.equals(EditVar.DOUBLE)) {
            renderer = new ScilabDoubleRenderer();
        } else if (type.equals(EditVar.BOOLEAN)) {
            renderer = new ScilabBooleanRenderer();
        } else if (type.equals(EditVar.INTEGER)) {
            renderer = new ScilabIntegerRenderer();
        }

        return renderer;
    }
}
