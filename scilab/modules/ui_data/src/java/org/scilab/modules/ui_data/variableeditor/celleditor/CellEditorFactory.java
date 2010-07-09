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

public class CellEditorFactory {
	
	public static DefaultCellEditor createCellEditor(Object[][] data){
		
		if (data instanceof String[][]) {
			return new ScilabStringCellEditor();
		} else if (data instanceof Double[][][]) {
			return new ScilabComplexCellEditor();
		} else if (data instanceof Double[][]) {
			return new ScilabDoubleCellEditor();
		} else if (data instanceof Boolean[][]) {
			return new ScilabBooleanCellEditor();
		}
		return new ScilabDoubleCellEditor();
	}

}
