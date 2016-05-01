/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
