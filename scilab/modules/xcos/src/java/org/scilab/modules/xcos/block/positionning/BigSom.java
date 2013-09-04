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
import org.scilab.modules.xcos.block.listener.SumPortLabelingListener;

/**
 * Implement the BIGSOM_f function.
 *
 * This class just install a {@link SumPortLabelingListener} on each instance.
 */
@SuppressWarnings(value = { "serial" })
public class BigSom extends BasicBlock {
    /**
     * Default constructor
     */
    public BigSom() {
        super();

        getParametersPCS().addPropertyChangeListener("realParameters",
                SumPortLabelingListener.getInstance());
    }

}
