/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.helptools;

/**
 * Common class for container only converters
 */
public abstract class ContainerConverter implements Converter {
    protected final String outputDirectory;
    protected final String language;

    public ContainerConverter(String outputDirectory, String language) {
        this.outputDirectory = outputDirectory;
        this.language = language;
    }

    /*
     * Empty method declaration for optional ones
     */

    @Override
    public void registerAllExternalXMLHandlers() {
    }
}
