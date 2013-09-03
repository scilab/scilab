/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
