/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.xml;

import javax.xml.transform.TransformerFactory;

/**
 * Class to provide a way to be sure that the default TransformerFactory provided by Java framework will be used.
 * @author Calixte DENIZET
 */
public class ScilabTransformerFactory {

    private static final String TRANSFORMERFACTORYPROPERTY = "javax.xml.transform.TransformerFactory";
    private static final String TRANSFORMERFACTORYIMPL = "com.sun.org.apache.xalan.internal.xsltc.trax.TransformerFactoryImpl";

    /**
     * Use default internal TransformerFactoryImpl
     * @return the key corresponding to the currently used implementation
     */
    public static String useDefaultTransformerFactoryImpl() {
        String factory = System.getProperty(TRANSFORMERFACTORYPROPERTY);
        System.setProperty(TRANSFORMERFACTORYPROPERTY, TRANSFORMERFACTORYIMPL);

        return factory;
    }

    /**
     * Restore the previous factory
     * @param factory the key as returned by useDefaultTransformerFactoryImpl
     */
    public static void restoreTransformerFactoryImpl(String factory) {
        if (factory == null) {
            System.clearProperty(TRANSFORMERFACTORYPROPERTY);
        } else {
            System.setProperty(TRANSFORMERFACTORYPROPERTY, factory);
        }
    }

    /**
     * @return a new instance using the default TransforferFactory implementation
     */
    public static TransformerFactory newInstance() {
        String factory = useDefaultTransformerFactoryImpl();
        TransformerFactory tf = TransformerFactory.newInstance();
        restoreTransformerFactoryImpl(factory);

        return tf;
    }
}
