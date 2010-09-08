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

package org.scilab.modules.ui_data.variableeditor.celleditor;

public class ScilabStringCellEditor extends ScilabGenericCellEditor {
   
    private static final long serialVersionUID = 3866054279955696250L;
    
    @Override
    protected String getDataAsScilabString() {
        return "\"\""+super.getDataAsScilabString()+"\"\"";
    }
}

