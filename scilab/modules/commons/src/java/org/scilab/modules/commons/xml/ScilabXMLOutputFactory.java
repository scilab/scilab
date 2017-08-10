/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
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

import javax.xml.stream.XMLOutputFactory;
/**
 * Class to provide a way to be sure that the default {@link XMLOutputFactory} provided by Java framework will be used.
 */
public class ScilabXMLOutputFactory {

    private static final String DEFAULT_FACTORYID = "javax.xml.stream.XMLOutputFactory";
    private static final String DEFAULT_IMPL = "com.sun.xml.internal.stream.XMLOutputFactoryImpl";

    /**
     * Use default internal XMLOutputFactoryImpl
     * @return the key corresponding to the currently used implementation
     */
    public static String useDefaultTransformerFactoryImpl() {
        String factory = System.getProperty(DEFAULT_FACTORYID);
        System.setProperty(DEFAULT_FACTORYID, DEFAULT_IMPL);

        return factory;
    }

    /**
     * Restore the previous factory
     * @param factory the key as returned by useDefaultXMLOutputFactoryImpl
     */
    public static void restoreTransformerFactoryImpl(String factory) {
        if (factory == null) {
            System.clearProperty(DEFAULT_FACTORYID);
        } else {
            System.setProperty(DEFAULT_FACTORYID, factory);
        }
    }

    /**
     * @return a new instance using the default XMLOutputFactory implementation
     */
    public static XMLOutputFactory newInstance() {
        String factory = useDefaultTransformerFactoryImpl();
        XMLOutputFactory xof = XMLOutputFactory.newInstance();
        restoreTransformerFactoryImpl(factory);

        return xof;
    }

}
