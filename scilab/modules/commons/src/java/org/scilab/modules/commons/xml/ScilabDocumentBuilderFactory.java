/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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
