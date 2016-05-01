/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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