/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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
