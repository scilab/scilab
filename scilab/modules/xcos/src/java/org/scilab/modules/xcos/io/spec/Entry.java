/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.spec;

import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

/**
 * Common interface for classes which can load/store a ZipEntry from the
 * {@link XcosPackage}
 */
public interface Entry {
    /*
     * Entry identification used to select the right entry instance.
     */

    /**
     * Get the media-type which this entry can load/store.
     *
     * @return The supported media type
     */
    public String getMediaType();

    /**
     * Get the full-path which this entry can load/store.
     *
     * @return The supported full path
     */
    public String getFullPath();

    /**
     * Method used to update any internal state before loading or saving data.
     *
     * This allow a specific entry to get only related data from the main
     * Package object.
     *
     * @param p
     *            the main package
     */
    public void setup(final XcosPackage p);

    /*
     * Methods to load and store data
     */

    /**
     * Methods used to load data
     *
     * @param entry
     *            the current entry to load
     * @param stream
     *            the current stream with data
     */
    public void load(ZipEntry entry, InputStream stream) throws IOException;

    /**
     * Methods used to store data
     *
     * @param stream
     *            the current stream to put the entry and data
     */
    public void store(ZipOutputStream stream) throws IOException;
}
