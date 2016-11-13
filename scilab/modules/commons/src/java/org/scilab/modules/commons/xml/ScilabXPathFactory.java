/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enteprises - Clement DAVID
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

package org.scilab.modules.commons.xml;

import javax.xml.xpath.XPathFactory;
import javax.xml.xpath.XPathFactoryConfigurationException;

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
        XPathFactory xpf;
        try {
            xpf = XPathFactory.newInstance(XPathFactory.DEFAULT_OBJECT_MODEL_URI, XPATHFACTORYIMPL, ScilabXPathFactory.class.getClassLoader());
        } catch (XPathFactoryConfigurationException e) {
            e.printStackTrace();
            return XPathFactory.newInstance();
        }
        restoreTransformerFactoryImpl(factory);
        return xpf;
    }
}
