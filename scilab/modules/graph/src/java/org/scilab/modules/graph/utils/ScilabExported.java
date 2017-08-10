/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph.utils;

import java.lang.annotation.ElementType;
import java.lang.annotation.Target;

/**
 * Annotation used to specify that this method is exported to JNI via giws.
 */
@Target( { ElementType.METHOD, ElementType.CONSTRUCTOR })
public @interface ScilabExported {
    /**
     * The module where the giws file and generated implementations are
     * presents, eg "xcos" for SCI/modules/xcos/src/jni/Xcos.giws.xml .
     */
    String module();

    /**
     * Giws file name, eg "Xcos.giws.xml" for
     * SCI/modules/xcos/src/jni/Xcos.giws.xml .
     */
    String filename();
}
