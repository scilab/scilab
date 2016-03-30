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

package org.scilab.modules.helptools;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;
import java.io.IOException;
import java.util.regex.Matcher;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;

/**
 * @author Calixte Denizet
 */
public class SciDocConfiguration {

    private static final String SCIVAR = "\\$\\{SCI\\}";
    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final String CONF = SCI + "/modules/helptools/etc/SciDocConf.xml";
    private static final String VERSION = SCI + "/Version.incl";
    private static final String ERROR_READ = "Problem to read the file ";

    private Document doc;

    /**
     * Default constructor
     * @param filename the configuration file
     */
    public SciDocConfiguration() {
        readDocument(CONF);
    }

    /**
     * Read the document
     * @param filename the configuration file
     */
    public void readDocument(String filename) {
        try {
            DocumentBuilderFactory factory = ScilabDocumentBuilderFactory.newInstance();
            doc = factory.newDocumentBuilder().parse(new File(filename));
        } catch (ParserConfigurationException e) {
            System.err.println(ERROR_READ + filename);
        } catch (SAXException e) {
            System.err.println(ERROR_READ + filename);
        } catch (IOException e) {
            System.err.println(ERROR_READ + filename);
        }
    }

    /**
     * @return the file path
     */
    public String getCSS() {
        String str = ((Element) doc.getDocumentElement().getElementsByTagName("css").item(0)).getAttribute("doc");
        return str.replaceAll(SCIVAR, Matcher.quoteReplacement(SCI));
    }

    /**
     * @param kind should be javahelp, chm, html or web
     * @return the template file path
     */
    public String getTemplate(String kind) {
        String str = ((Element) doc.getDocumentElement().getElementsByTagName("templates").item(0)).getAttribute(kind);
        return str.replaceAll(SCIVAR, Matcher.quoteReplacement(SCI));
    }

    /**
     * @return the Scilab's doc web url
     */
    public String getWebSiteURL() {
        return ((Element) doc.getDocumentElement().getElementsByTagName("website").item(0)).getAttribute("url");
    }

    /**
     * @return the path of the file containing Scilab macros
     */
    public String[] getMacros() {
        return getPathList("macros");
    }

    /**
     * @return the path of the file containing Scilab primitives
     */
    public String[] getBuiltins() {
        return getPathList("builtins");
    }

    /**
     * @param kind should be "macros" or "builtins"
     * @return the path of the file containing Scilab macros or built-in function.
     */
    private String[] getPathList(String kind) {
        Element macros = (Element) doc.getDocumentElement().getElementsByTagName(kind).item(0);
        NodeList pathList = macros.getElementsByTagName("file");
        String[] paths = new String[pathList.getLength()];
        for (int i = 0; i < paths.length; i++) {
            paths[i] = ((Element) pathList.item(i)).getAttribute("path").replaceAll(SCIVAR, Matcher.quoteReplacement(SCI));
        }

        return paths;
    }

    /**
     * @return the version string
     */
    public String getVersion() {
        String str = null;
        NodeList list = doc.getDocumentElement().getElementsByTagName("version");
        if (list != null && list.getLength() > 0) {
            Element el = (Element) list.item(0);
            if (el != null) {
                str = el.getAttribute("id");
            }
        }

        return getVersion(str);
    }

    /**
     * Get the version in Version.incl if ver==null or ver=="".
     * @param ver the actual version
     * @return the version in Version.incl
     */
    private static String getVersion(String ver) {
        String ret = "";
        if (ver == null || ver.isEmpty()) {
            try {
                BufferedReader reader = new BufferedReader(new FileReader(VERSION));
                ret = reader.readLine().split("=")[1];
                reader.close();
            } catch (IOException e) {
                System.err.println(e);
            }
        } else {
            ret = ver;
        }

        return ret;
    }
}
