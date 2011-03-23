/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.datatable;

import javax.swing.table.TableModel;
/**
 * 
 * Encapsulate Swing Table model for reusability.
 *
 * @param <Type> in order to have strongly/lazy typed JTable
 */
public interface ScilabTable<Type> extends TableModel {

    /**
     * Set data in the Model
     * @param data : the data
     */
    void setData(Type[][] data);

}
