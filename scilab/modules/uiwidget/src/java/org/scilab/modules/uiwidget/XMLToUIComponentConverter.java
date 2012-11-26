/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.IOException;
import java.net.URL;
import java.net.MalformedURLException;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Stack;

import javax.swing.UIManager;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import org.scilab.modules.uiwidget.components.TextData;

public class XMLToUIComponentConverter extends DefaultHandler {

    private final static Map<String, String> mapUriToPackage = new HashMap<String, String>();

    private final Stack<UIComponent> stack;
    private final Stack<UIFakeComponent> stackFake;
    private final String in;
    private StringBuilder buffer;
    private Map<String, Map<String, String>> style;
    private String modeleNS;

    public XMLToUIComponentConverter(String in) throws IOException {
        this.in = in;
        this.stack = new Stack<UIComponent>();
        this.stackFake = new Stack<UIFakeComponent>();
        this.buffer = new StringBuilder(128);

        registerURI("http://forge.scilab.org/uicontrol2", "org.scilab.modules.uiwidget.components");
    }

    public static void registerURI(String uri, String pack) {
        mapUriToPackage.put(uri, pack);
    }

    public Map<String, Map<String, String>> getStyle() {
        return style;
    }

    /**
     * Start the conversion
     * @throws SAXException if a problem is encountered during the parsing
     * @throws IOException if an IO problem is encountered
     */
    public void convert() throws SAXException, IOException {
        convert(this.in);
    }

    /**
     * Start the conversion
     * @throws SAXException if a problem is encountered during the parsing
     * @throws IOException if an IO problem is encountered
     */
    public void convert(String in) throws SAXException, IOException {
        if (in != null) {
            File f = new File(in);
            if (f.exists() && f.canRead()) {
                convert(f);
            } else {
                try {
                    URL url = new URL(in);
                    InputStream is = url.openStream();
                    convert(is);
                } catch (MalformedURLException e) {
                    throw new IOException("Invalid path " + in);
                }
            }
        }
    }

    /**
     * Start the conversion
     * @throws SAXException if a problem is encountered during the parsing
     * @throws IOException if an IO problem is encountered
     */
    public void convert(File f) throws SAXException, IOException {
        convert(new FileInputStream(f));
    }

    /**
     * Start the conversion
     * @throws SAXException if a problem is encountered during the parsing
     * @throws IOException if an IO problem is encountered
     */
    public void convert(InputStream is) throws SAXException, IOException {
        SAXParserFactory factory = SAXParserFactory.newInstance();
        factory.setValidating(false);
        factory.setNamespaceAware(true);

        try {
            factory.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
            SAXParser parser = factory.newSAXParser();
            parser.parse(is, this);
            is.close();
        } catch (ParserConfigurationException e) {
            System.err.println(e);
        } catch (SAXException e) {
            System.err.println(e);
        } catch (IOException e) {
            System.err.println(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void startDocument() throws SAXException {

    }

    /**
     * {@inheritDoc}
     */
    public void endDocument() throws SAXException {
        if (!stack.empty()) {
            stack.clear();
            throw new SAXException("Stack not empty");
        }
    }

    /**
     * {@inheritDoc}
     */
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
        if (localName.equals("interface")) {
            handleStyle(attributes.getValue("style"));
        } else if (localName.equals("modele")) {
            modeleNS = attributes.getValue("namespace");
        } else if (localName.equals("include")) {
            handleInclude(attributes.getValue("path"));
        } else {
            String pack = mapUriToPackage.get(uri);
            if (pack != null) {
                if (modeleNS != null) {
                    createUIFakeComponent(pack, localName, attributes);
                } else {
                    createUIComponent(pack, localName, attributes);
                }
            } else {
                if (modeleNS != null) {
                    createUIFakeComponentOnModele(uri, localName, attributes);
                } else {
                    createUIComponentOnModele(uri, localName, attributes);
                }
            }

            if (localName.equals("Text")) {
                buffer.setLength(0);
            }

            if (modeleNS != null) {
                String modeleName = attributes.getValue("modele-name");
                if (modeleName != null && !modeleName.isEmpty()) {
                    UIModele.add(modeleNS, modeleName, stackFake.peek());
                }
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void characters(char[] ch, int start, int length) throws SAXException {
        buffer.append(ch, start, length);
    }

    /**
     * {@inheritDoc}
     */
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if (localName.equals("modele")) {
            modeleNS = null;
            stackFake.clear();
        } else if (!localName.equals("interface") && !localName.equals("include")) {
            if (modeleNS != null) {
                UIFakeComponent c = stackFake.pop();
                if (!stackFake.isEmpty()) {
                    stackFake.peek().add(c);
                }
            } else {
                final UIComponent c = stack.pop();

                UIAccessTools.execOnEDT(new Runnable() {
                    public void run() {
                        c.finish();
                    }
                });

                if (c instanceof TextData) {
                    ((TextData) c).addText(buffer.toString());
                }

                if (!stack.isEmpty()) {
                    final UIComponent top = stack.peek();
                    UIAccessTools.execOnEDT(new Runnable() {
                        public void run() {
                            try {
                                top.add(c);
                            } catch (Exception e) {
                                System.err.println(e);
                            }
                        }
                    });
                }
            }
        }
    }

    protected void createUIComponent(String pack, String name, Attributes attributes) throws SAXException {
        UIComponent parent = null;
        if (!stack.empty()) {
            parent = stack.peek();
        }

        try {
            UIComponent ui = UIComponent.getUIComponent(pack, name, attributes, parent, style);
            if (ui != null) {
                stack.push(ui);
            }
        } catch (UIWidgetException e) {
            System.err.println("(Warning) Cannot create the component " + name + ": ignored");
        }
    }

    protected void createUIComponentOnModele(String uri, String name, Attributes attributes) throws SAXException {
        UIComponent parent = null;
        if (!stack.empty()) {
            parent = stack.peek();
        }

        try {
            UIComponent ui = UIModele.get(uri, name, parent, style);
            if (ui != null) {
                stack.push(ui);
            }
        } catch (UIWidgetException e) {
            System.err.println("(Warning) Cannot create the component " + name + ": ignored");
        }
    }

    protected void createUIFakeComponent(String pack, String name, Attributes attributes) {
        UIFakeComponent parent = null;
        if (!stackFake.empty()) {
            parent = stackFake.peek();
        }

        stackFake.push(new UIFakeComponent(pack, name, attributes));
    }

    protected void createUIFakeComponentOnModele(String uri, String name, Attributes attributes) {
        stackFake.push(UIModele.get(uri, name));
    }

    protected void handleInclude(String file) throws SAXException {
        if (file != null && !file.isEmpty()) {
            try {
                convert(file);
            } catch (IOException e) {
                System.err.println(e);
            }
        }
    }

    protected void handleStyle(String css) {
        if (css != null && !css.isEmpty()) {
            String[] files = css.split(" ,;");
            for (String f : files) {
                File file = new File(f);
                if (file.exists() && file.canRead()) {
                    Map<String, Map<String, String>> s = StyleParser.parseFile(f);
                    if (style == null) {
                        style = s;
                    } else {
                        mergeStyles(style, s);
                    }
                }
            }
        }
    }

    private static void mergeStyles(Map<String, Map<String, String>> style1, Map<String, Map<String, String>> style2) {
        Set<String> keys2 = style2.keySet();
        for (String key2 : keys2) {
            Map<String, String> map1 = style1.get(key2);
            Map<String, String> map2 = style2.get(key2);
            if (map1 == null) {
                style1.put(key2, map2);
            } else {
                map1.putAll(map2);
            }
        }
    }
}
