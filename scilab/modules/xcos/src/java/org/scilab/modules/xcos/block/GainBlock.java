/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2011 - DIGITEO - Clément DAVID 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block;


/**
 * @deprecated use "displayedLabel" = "%s" style property. 
 */
@Deprecated
public final class GainBlock extends BasicBlock {

     private static final long serialVersionUID = 7762548847345910801L;

    /**
     * Constructor
     */
    public GainBlock() {
	super();
    }
    
    /**
     *  Initialize the block with the default values
     */  
    @Override
    protected void setDefaultValues() {
		super.setDefaultValues();
    	setInterfaceFunctionName("GAINBLK_f");
    }
}
