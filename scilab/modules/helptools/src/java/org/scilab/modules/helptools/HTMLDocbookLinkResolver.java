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

package org.scilab.modules.helptools;

import java.io.IOException;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import org.scilab.modules.commons.ScilabCommonsUtils;

/**
 * Class the convert a DocBook xml file
 * @author Calixte DENIZET
 */
public class HTMLDocbookLinkResolver extends DefaultHandler {

    private static boolean isCaseInsensitiveOS = System.getProperty("os.name").toLowerCase().contains("windows");

    private Map<String, String> mapId = new LinkedHashMap<>();
    private List<String> listIdIgnoreCase = new ArrayList<>();
    private Map<String, String> toc = new LinkedHashMap<>();
    private Map<String, String> mapIdPurpose = new LinkedHashMap<>();
    private Map<String, String> mapIdRefname = new LinkedHashMap<>();
    private Map<String, TreeId> mapTreeId = new HashMap<>();
    private Map<String, String> mapIdDeclaringFile = new HashMap<>();
    private TreeId tree = new TreeId(null, "root");

    private TreeId currentLeaf = tree;

    private String current;
    private String lastId;
    private Locator locator;
    private String currentFileName;
    private boolean waitForRefname;
    private boolean waitForRefpurpose;
    private boolean waitForTitle;
    private boolean getContents;
    private boolean idInRefentry;
    private final File in;
    private StringBuilder buffer = new StringBuilder(256);

    /**
     * Constructor
     * @param in the input file path
     */
    public HTMLDocbookLinkResolver(String in) throws IOException, SAXException {
        this.in = new File(in);
        resolvLinks();
        mapIdDeclaringFile = null;
    }

    /**
     * @return the map id
     */
    public Map<String, String> getMapId() {
        return mapId;
    }

    /**
     * @return the map id-&gt;title
     */
    public Map<String, String> getMapIdPurpose() {
        return mapIdPurpose;
    }

    /**
     * @return the map id-&gt;title
     */
    public Map<String, String> getMapIdRefname() {
        return mapIdRefname;
    }

    /**
     * @return the tocs
     */
    public Map<String, String> getToc() {
        return toc;
    }

    /**
     * @return the document tree
     */
    public TreeId getTree() {
        return tree;
    }

    /**
     * @return the document tree
     */
    public Map<String, TreeId> getMapTreeId() {
        return mapTreeId;
    }

    /**
     * {@inheritDoc}
     */
    public InputSource resolveEntity(String publicId, String systemId) throws SAXException, IOException {
        currentFileName = systemId;
        return super.resolveEntity(publicId, systemId);
    }

    /**
     * {@inheritDoc}
     */
    public void startDocument() throws SAXException { }

    /**
     * {@inheritDoc}
     */
    public void endDocument() throws SAXException { }

    /**
     * {@inheritDoc}
     */
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
        int len = attributes.getLength();
        String id = null;
        for (int i = 0; i < len; i++) {
            if (attributes.getLocalName(i).equals("id")) {
                id = attributes.getValue(i);
                break;
            }
        }

        if (localName.equals("refentry")) {
            idInRefentry = false;
        }

        if (localName.equals("title")) {
            if (waitForTitle) {
                getContents = true;
                buffer.setLength(0);
            }
        } else if (localName.equals("refname")) {
            if (waitForRefname) {
                getContents = true;
                buffer.setLength(0);
            }
        } else if (localName.equals("refpurpose")) {
            if (waitForRefpurpose) {
                getContents = true;
                buffer.setLength(0);
            }
        } else if ((id != null && localName.equals("refentry")) || localName.equals("section")
                   || localName.equals("part") || localName.equals("chapter") || (!idInRefentry && localName.equals("refnamediv"))) {
            if (id == null) {
                throw new SAXException(errorMsg());
            }
            current = makeFileName(id);
            listIdIgnoreCase.add(id.toLowerCase());
            lastId = id;
            if (mapIdDeclaringFile.containsKey(id)) {
                String prev = mapIdDeclaringFile.get(id);
                throw new SAXException("The id " + id + " in file " + currentFileName + " was previously declared in " + prev);
            } else {
                mapIdDeclaringFile.put(id, currentFileName);
            }
            mapId.put(id, current);
            waitForTitle = localName.charAt(0) != 'r';
            waitForRefname = !waitForTitle;
            idInRefentry = waitForRefname;
            waitForRefpurpose = waitForRefname;
            TreeId leaf = new TreeId(currentLeaf, id);
            currentLeaf.add(leaf);
            currentLeaf = leaf;
        } else if (id != null && current != null) {
            if (mapIdDeclaringFile.containsKey(id)) {
                String prev = mapIdDeclaringFile.get(id);
                throw new SAXException("The id " + id + " in file " + currentFileName + " was previously declared in " + prev);
            } else {
                mapIdDeclaringFile.put(id, currentFileName);
            }
            mapId.put(id, current + "#" + id);
        }
    }

    /**
     * Make a file name which take into account that under Windows the file name
     * is case insensitive and the xml:id is case sensitive.
     * @param id the xml:id
     * @return an unique file name
     */
    public String makeFileName(String id) {
        if (isCaseInsensitiveOS && listIdIgnoreCase.contains(id.toLowerCase())) {
            return id + "-" + ScilabCommonsUtils.getMD5(id) + ".html";
        }

        return id + ".html";
    }

    /**
     * {@inheritDoc}
     */
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if (getContents) {
            if (localName.equals("refpurpose")) {
                mapIdPurpose.put(lastId, buffer.toString().trim());
                waitForRefpurpose = false;
                getContents = false;
            } else if (localName.equals("title") || localName.equals("refname")) {
                toc.put(lastId, buffer.toString().trim());
                mapIdRefname.put(lastId, buffer.toString().trim());
                getContents = false;
                waitForRefname = false;
                waitForTitle = false;
            }
        }
        if (localName.equals("refentry") || localName.equals("section") || localName.equals("part") || localName.equals("chapter")) {
            currentLeaf = currentLeaf.parent;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setDocumentLocator(Locator locator) {
        this.locator = locator;
    }

    /**
     * {@inheritDoc}
     */
    public void characters(char[] ch, int start, int length) throws SAXException {
        if (getContents) {
            int end = start + length;
            int save = start;
            for (int i = start; i < end; i++) {
                switch (ch[i]) {
                    case '\'' :
                        buffer.append(ch, save, i - save);
                        buffer.append("&#0039;");
                        save = i + 1;
                        break;
                    case '\"' :
                        buffer.append(ch, save, i - save);
                        buffer.append("&quot;");
                        save = i + 1;
                        break;
                    case '<' :
                        buffer.append(ch, save, i - save);
                        buffer.append("&lt;");
                        save = i + 1;
                        break;
                    case '>' :
                        buffer.append(ch, save, i - save);
                        buffer.append("&gt;");
                        save = i + 1;
                        break;
                    case '&' :
                        buffer.append(ch, save, i - save);
                        buffer.append("&amp;");
                        save = i + 1;
                        break;
                    default :
                        break;
                }
            }

            if (save < end) {
                buffer.append(ch, save, end - save);
            }
        }
    }

    /**
     * Start the conversion
     * @throws SAXException if a problem is encountered during the parsing
     * @throws IOException if an IO problem is encountered
     */
    protected void resolvLinks() throws SAXException, IOException {
        SAXParserFactory factory = SAXParserFactory.newInstance();
        factory.setValidating(false);
        factory.setNamespaceAware(true);
        factory.setXIncludeAware(true);

        try {
            SAXParser parser = factory.newSAXParser();
            parser.parse(in, this);
        } catch (ParserConfigurationException e) {
            System.err.println(e);
        }
    }

    /**
     * @return the error message
     */
    private String errorMsg() {
        String str;
        if (currentFileName != null) {
            str = currentFileName;
        } else {
            try {
                str = in.getCanonicalPath();
            } catch (IOException e) {
                e.printStackTrace();
                str = null;
            }
        }

        return "No id attribute in <refentry> or <refnamediv> in file " + str + " at line " + locator.getLineNumber();
    }

    public class TreeId {

        String id;
        TreeId parent;
        int pos;
        List<TreeId> children;

        TreeId(TreeId parent, String id) {
            this.parent = parent;
            this.id = id;
            if (parent == null) {
                mapTreeId.clear();
            }
            mapTreeId.put(id, this);
        }

        void add(TreeId child) {
            if (children == null) {
                children = new ArrayList<>();
            }
            child.pos = children.size();
            children.add(child);
        }

        boolean isRoot() {
            return parent == null;
        }

        TreeId getPrevious() {
            if (pos > 0) {
                return parent.children.get(pos - 1);
            }
            return parent;
        }

        TreeId getNext() {
            TreeId l = this;
            while (l.parent != null && l.pos == l.parent.children.size() - 1) {
                l = l.parent;
            }

            if (l.parent == null) {
                return null;
            }
            return l.parent.children.get(l.pos + 1);
        }
    }
}
