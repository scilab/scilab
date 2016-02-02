/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
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
