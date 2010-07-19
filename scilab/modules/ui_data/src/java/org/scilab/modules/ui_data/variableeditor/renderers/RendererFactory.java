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

public class RendererFactory {

    public static DefaultTableCellRenderer createRenderer(Object[][] data){
        if (data instanceof String[][]) {
            return new ScilabStringRenderer();
        } else if (data instanceof Double[][][]) {
        	return new ScilabComplexRenderer();
        } else if (data instanceof Double[][]) {
            return new ScilabDoubleRenderer();
        } else if (data instanceof Boolean[][]) {
            return new ScilabBooleanRenderer();
        }
        return new ScilabStringRenderer();

    }

    public static DefaultTableCellRenderer createRenderer(Object data){
        if (data instanceof String) {
            return new ScilabStringRenderer();
        } else if (data instanceof Double[]) {
            return new ScilabComplexRenderer();
        } else if (data instanceof Double) {
            return new ScilabDoubleRenderer();
        } else if (data instanceof Boolean) {
            return new ScilabBooleanRenderer();
        }
        return new ScilabStringRenderer();

    }
}
