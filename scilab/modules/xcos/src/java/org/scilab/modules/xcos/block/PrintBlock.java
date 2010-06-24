/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabType;

/**
 * A {@link BasicBlock} which print getExprs()[0][0] as bloc value.
 */
public class PrintBlock extends BasicBlock {
	private final String format;
	
	/**
     * Constructor
     */
    public PrintBlock() {
	super();
	format = "%s";
    }
    
    /**
     * @param format The string to format the value
     */
    public PrintBlock(String format) {
	super();
	this.format = format;
    }
    
    /**
     * Initialize the block with the default values
     */
    @Override
    protected void setDefaultValues() {
    	super.setDefaultValues();
    }
    
    
    /**
     * @param exprs new exprs
     */
	@Override
    public void setExprs(ScilabType exprs) {
    	super.setExprs(exprs);
    	
    	final String data = ((ScilabString) getExprs()).getData()[0][0];
    	final String str = String.format(format, data);
    	
		setValue(str);
    }    
}
