/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.util.Map;

public interface Handler {

    /**
     * Decode an evaluated Xcos context
     *
     * @return the decoded context
     */
    public abstract Map<String, String> readContext();

    /**
     * Encode an Xcos context
     *
     * @param context
     *            the context
     */
    public abstract void writeContext(String[] context);

}