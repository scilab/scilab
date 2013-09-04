/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block.positionning;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.listener.ProdPortLabelingListener;

/**
 * Implement the PRODUCT_f block.
 *
 * This class just install a {@link ProdPortLabelingListener} on each instance.
 */
@SuppressWarnings(value = { "serial" })
public class Product extends BasicBlock {
    /**
     * Default constructor
     */
    public Product() {
        super();

        getParametersPCS().addPropertyChangeListener("integerParameters",
                ProdPortLabelingListener.getInstance());
    }
}
