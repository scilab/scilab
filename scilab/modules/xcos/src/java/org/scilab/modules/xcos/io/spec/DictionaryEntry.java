/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.spec;

import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class DictionaryEntry implements Entry {

    @Override
    public String getMediaType() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String getFullPath() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void setup(XcosPackage p) {
        // TODO Auto-generated method stub

    }

    @Override
    public void load(ZipEntry entry, InputStream stream) {
        // TODO Auto-generated method stub

    }

    @Override
    public void store(ZipOutputStream stream) {
        // TODO Auto-generated method stub

    }

}
