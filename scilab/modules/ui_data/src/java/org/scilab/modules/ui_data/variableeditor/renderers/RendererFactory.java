/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
        } else if (type.equals(EditVar.SPARSE)) {
            renderer = new ScilabSparseRenderer();
        } else if (type.equals(EditVar.COMPLEXSPARSE)) {
            renderer = new ScilabComplexSparseRenderer();
        } else if (type.equals(EditVar.BOOLEANSPARSE)) {
            renderer = new ScilabBooleanSparseRenderer();
        }

        return renderer;
    }
}
