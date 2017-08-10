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

package org.scilab.modules.jvm;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.File;
import java.util.HashSet;
import java.util.Set;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.commons.xml.ScilabXPathFactory;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;

/**
 * Utility class to ease the jar loading management.
 */
public final class LoadClassPath {
    /** Scilab environment variable */
    private static final String SCI = System.getenv("SCI");
    /** The classpath.xml file path */
    private static final String CLASSPATH_PATH = SCI + "/etc/classpath.xml";
    /** The expression to get the jars */
    private static final String XPATH_EXPRS = "//classpaths/path[@load='onUse']/load";

    /** Cache the already loaded libraries */
    private static Set<String> loadedModules = new HashSet<String>();

    /**
     * Load the module on the classpath
     *
     * The module have to be declared in the $SCI/etc/classpath.xml file.
     *
     * @param module the module to be loaded
     */
    public static void loadOnUse(String module) {
        if (loadedModules.contains(module)) {
            return;
        }

        // define XPath expression
        String xpathExpression = XPATH_EXPRS + "[@on='" + module + "']";

        // Initialize xpath
        XPathFactory factory = ScilabXPathFactory.newInstance();
        XPath xpath = factory.newXPath();

        // initialize document factory
        DocumentBuilderFactory domFactory = ScilabDocumentBuilderFactory.newInstance();
        domFactory.setValidating(false);
        domFactory.setNamespaceAware(true);

        // Parse Classpath file
        Document doc = null;
        try {
            DocumentBuilder builder = domFactory.newDocumentBuilder();
            doc = builder.parse(new File(CLASSPATH_PATH));
        } catch (ParserConfigurationException e) {
            System.err.println("Error: " + e.getLocalizedMessage());
            e.printStackTrace();
            return;
        } catch (SAXException e) {
            System.err.println("Error: " + e.getLocalizedMessage());
            e.printStackTrace();
            return;
        } catch (IOException e) {
            System.err.println("Error: " + e.getLocalizedMessage());
            e.printStackTrace();
            return;
        }

        // Load JARs
        try {
            NodeList result = (NodeList) xpath.evaluate(xpathExpression, doc,
                              XPathConstants.NODESET);

            for (int i = 0; i < result.getLength(); i++) {
                Node n = result.item(i).getParentNode();

                String jar = n.getAttributes().getNamedItem("value").getNodeValue();
                ClassPath.addFile(jar.replace("$SCILAB", SCI), 0);
                loadedModules.add(module);
            }

        } catch (XPathExpressionException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * This class is a static singleton
     */
    private LoadClassPath() { }
}
