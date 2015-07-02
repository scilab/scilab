/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enteprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.commons.xml;

import javax.xml.xpath.XPathFactory;

/**
 * Class to provide a way to be sure that the default PathFactory provided by Java framework will be used.
 */
public class ScilabXPathFactory {

    private static final String XPATHFACTORYPROPERTY = "javax.xml.xpath.XPathFactory";
    private static final String XPATHFACTORYIMPL = "com.sun.org.apache.xpath.internal.jaxp.XPathFactoryImpl";

    /**
     * Use default internal XPathFactoryImpl
     * @return the key corresponding to the currently used implementation
     */
    public static String useDefaultTransformerFactoryImpl() {
        String factory = System.getProperty(XPATHFACTORYPROPERTY);
        System.setProperty(XPATHFACTORYPROPERTY, XPATHFACTORYIMPL);

        return factory;
    }

    /**
     * Restore the previous factory
     * @param factory the key as returned by useDefaultPathFactoryImpl
     */
    public static void restoreTransformerFactoryImpl(String factory) {
        if (factory == null) {
            System.clearProperty(XPATHFACTORYPROPERTY);
        } else {
            System.setProperty(XPATHFACTORYPROPERTY, factory);
        }
    }

    /**
     * @return a new instance using the default XPathFactory implementation
     */
    public static XPathFactory newInstance() {
        String factory = useDefaultTransformerFactoryImpl();
        XPathFactory xpf = XPathFactory.newInstance();
        restoreTransformerFactoryImpl(factory);

        return xpf;
    }
}
