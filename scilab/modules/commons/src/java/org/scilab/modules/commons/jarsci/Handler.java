/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.commons.jarsci;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;

import org.scilab.modules.commons.ScilabConstants;

/**
 * A {@link URLStreamHandler} that handles images resource.
 */
public class Handler extends URLStreamHandler {

    private static String JARPATH = new File(ScilabConstants.SCI, "/modules/helptools/jar/scilab_images.jar").getAbsoluteFile().toURI().toString();

    public Handler() {

    }

    @Override
    protected URLConnection openConnection(URL u) throws IOException {
        String path = u.getPath();
        if (path.startsWith("./")) {
            path = path.substring(1);
        } else {
            path = "/" + path;
        }

        URL jar = new URL("jar:" + JARPATH + "!" + path);
        return jar.openConnection();
    }
}
