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

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import org.scilab.modules.types.ScilabList;
import org.w3c.dom.Element;

public class DictionaryEntry implements Entry {
    private static final Logger LOG = Logger.getLogger(DictionaryEntry.class.getName());

    private XcosPackage pack;

    public DictionaryEntry() {
    }

    @Override
    public String getMediaType() {
        return "bin/ser";
    }

    @Override
    public String getFullPath() {
        return "dictionary/dictionary.ser";
    }

    @Override
    public void setup(XcosPackage p) {
        this.pack = p;
    }

    @Override
    public void load(ZipEntry entry, InputStream stream) throws IOException {
        try (ObjectInputStream ois = new ObjectInputStream(stream)) {

            pack.getDictionary().clear();
            pack.getDictionary().addAll((ScilabList) ois.readObject());
        } catch (IOException | ClassNotFoundException e) {
            Logger.getLogger(DictionaryEntry.class.getName()).severe(e.getMessage());
        }
    }

    @Override
    public void store(ZipOutputStream stream) throws IOException {
        /*
         * Store content
         */
        LOG.entering("ObjectOutputStream", "writeObject");
        ObjectOutputStream oos = new ObjectOutputStream(new BufferedOutputStream(stream));
        oos.writeObject(pack.getDictionary());
        oos.flush();
        LOG.exiting("ObjectOutputStream", "writeObject");

        /*
         * Add an entry to the manifest file
         */
        final Element e = pack.getManifest().createElement("manifest:file-entry");
        e.setAttribute("manifest:media-type", getMediaType());
        e.setAttribute("manifest:full-path", getFullPath());
        pack.getManifest().getFirstChild().appendChild(e);
    }
}
