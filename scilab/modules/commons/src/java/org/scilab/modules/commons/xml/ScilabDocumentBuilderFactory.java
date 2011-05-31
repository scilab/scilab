/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.xml;

import javax.xml.parsers.DocumentBuilderFactory;

/**
 * Class to provide a way to be sure that the default DocumentBuilderFactory provided by Java framework will be used.
 * @author Calixte DENIZET
 */
public class ScilabDocumentBuilderFactory {

    private static final String DOCUMENTBUILDERFACTORYPROPERTY = "javax.xml.parsers.DocumentBuilderFactory";
    private static final String DOCUMENTBUILDERFACTORYIMPL = "com.sun.org.apache.xerces.internal.jaxp.DocumentBuilderFactoryImpl";

    /**
     * Use default internal DocumentBuilderFactoryImpl
     * @return the key corresponding to the currently used implementation
     */
    public static String useDefaultDocumentBuilderFactoryImpl() {
        String factory = System.getProperty(DOCUMENTBUILDERFACTORYPROPERTY);
        System.setProperty(DOCUMENTBUILDERFACTORYPROPERTY, DOCUMENTBUILDERFACTORYIMPL);

        return factory;
    }

    /**
     * Restore the previous factory
     * @param factory the key as returned by useDefaultDocumentBuilderFactoryImpl
     */
    public static void restoreDocumentBuilderFactoryImpl(String factory) {
        if (factory == null) {
            System.clearProperty(DOCUMENTBUILDERFACTORYPROPERTY);
        } else {
            System.setProperty(DOCUMENTBUILDERFACTORYPROPERTY, factory);
        }
    }

    /**
     * @return a new instance using the default DocumentBuilderFactory implementation
     */
    public static DocumentBuilderFactory newInstance() {
        String factory = useDefaultDocumentBuilderFactoryImpl();
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        restoreDocumentBuilderFactoryImpl(factory);

        return dbf;
    }
}
