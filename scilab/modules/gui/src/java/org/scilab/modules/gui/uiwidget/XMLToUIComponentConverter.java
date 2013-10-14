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

package org.scilab.modules.gui.uiwidget;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Stack;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.scilab.modules.gui.uiwidget.components.TextData;
import org.xml.sax.Attributes;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

/**
 * UIWidget XML parser class
 * @author Calixte DENIZET
 */
public class XMLToUIComponentConverter extends DefaultHandler {

    private final static Map<String, String> mapUriToPackage = new HashMap<String, String>();

    private final Stack<UIComponent> stack;
    private final Stack<UIFakeComponent> stackFake;
    private final String in;
    private StringBuilder buffer;
    private Map<String, Map<String, String>> style;
    private String modeleNS;
    protected Locator locator;
    protected UIComponent rootUIComponent;

    /**
     * Default constructor
     * @param in a string containing file path or URL
     */
    public XMLToUIComponentConverter(String in) throws IOException {
        this.in = in;
        this.stack = new Stack<UIComponent>();
        this.stackFake = new Stack<UIFakeComponent>();
        this.buffer = new StringBuilder(128);

        registerURI("http://forge.scilab.org/uicontrol2", "org.scilab.modules.gui.uiwidget.components");
    }

    /**
     * Register an URI for a given Java package
     * @param uri the URI
     * @param pack the Java package name containing UIComponents
     */
    public static void registerURI(String uri, String pack) {
        mapUriToPackage.put(uri, pack);
    }

    /**
     * Get the style associated with this XML file
     * @return the CSS style
     */
    public Map<String, Map<String, String>> getStyle() {
        return style;
    }

    /**
     * Start the conversion
     */
    public void convert() throws Exception {
        convert(this.in);
    }

    /**
     * Start the conversion
     * @param in the path or URL to the given XML
     */
    protected void convert(String in) throws Exception {
        if (in != null) {
            File f = new File(in);
            if (f.exists() && f.canRead()) {
                boolean added = UIWidgetTools.addBaseDir(f.getAbsoluteFile().getParentFile());
                try {
                    convert(f);
                } catch (Exception e) {
                    throw e;
                } finally {
                    if (added) {
                        UIWidgetTools.removeBaseDir(f.getAbsoluteFile().getParentFile());
                    }
                }
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
     * @param f a File
     */
    protected void convert(File f) throws SAXException {
        try {
            convert(new FileInputStream(f));
        } catch (FileNotFoundException e) {
            throw new SAXException(String.format("File not found: %s", f.getAbsolutePath()));
        }
    }

    /**
     * Start the conversion
     * @param in an InputStream
     */
    protected void convert(InputStream is) throws SAXException {
        SAXParser parser;

        try {
            SAXParserFactory factory;
            factory = SAXParserFactory.newInstance();
            factory.setValidating(false);
            factory.setNamespaceAware(true);
            factory.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
            parser = factory.newSAXParser();
        } catch (Exception e) {
            throw new SAXException(String.format("Cannot initialize the XML parser: %s", e.getMessage()));
        }

        try {
            parser.parse(is, this);
        } catch (IOException e) {
            clearStack();
            throw new SAXException("I/O error: cannot parse the file");
        } catch (SAXException e) {
            clearStack();
            if (locator != null) {
                throw new SAXException(String.format("Parse error at line %d: %s", locator.getLineNumber(), e.getMessage()));
            } else {
                throw new SAXException(String.format("Parse error: %s", e.getMessage()));
            }
        } finally {
            try {
                is.close();
            } catch (IOException e) { }
        }
    }

    /**
     * Clear the stack
     */
    private void clearStack() {
        while (!stack.empty()) {
            UIComponent c = stack.pop();
            if (c != null) {
                c.remove();
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setDocumentLocator(Locator locator) {
        this.locator = locator;
    }

    /**
     * @return the document locator
     */
    public Locator getDocumentLocator() {
        return locator;
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
                } else {
                    rootUIComponent = c;
                }
            }
        }
    }

    /**
     * Create an UIComponent
     * @param pack Java package name
     * @param name Java class name
     * @param attributes the XML attributes
     */
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
            if (locator != null) {
                throw new SAXException(String.format("Error at line %d on tag %s: %s", locator.getLineNumber(), name, e.getMessage()));
            } else {
                throw new SAXException(String.format("Error on tag %s: %s", name, e.getMessage()));
            }
        }
    }

    /**
     * Create an UIComponent based on a model
     * @param uri the uri where the model is located
     * @param name the model name
     * @param attributes the XML attributes
     */
    protected void createUIComponentOnModele(String uri, String name, Attributes attributes) throws SAXException {
        UIComponent parent = null;
        if (!stack.empty()) {
            parent = stack.peek();
        }

        try {
            UIComponent ui = UIModele.get(uri, name, parent, UIComponent.getMapFromAttributes(attributes), style);
            if (ui != null) {
                stack.push(ui);
            }
        } catch (UIWidgetException e) {
            if (locator != null) {
                throw new SAXException(String.format("Error at line %d on tag %s: %s", locator.getLineNumber(), name, e.getMessage()));
            } else {
                throw new SAXException(String.format("Error on tag %s: %s", name, e.getMessage()));
            }
        }
    }

    /**
     * Create a fake UIComponent
     * @param pack Java package name
     * @param name Java class name
     * @param attributes the XML attributes
     */
    protected void createUIFakeComponent(String pack, String name, Attributes attributes) {
        UIFakeComponent parent = null;
        if (!stackFake.empty()) {
            parent = stackFake.peek();
        }

        stackFake.push(new UIFakeComponent(pack, name, attributes));
    }

    /**
     * Create a fake UIComponent based on a model
     * @param uri the uri where the model is located
     * @param name the model name
     * @param attributes the XML attributes
     */
    protected void createUIFakeComponentOnModele(String uri, String name, Attributes attributes) {
        stackFake.push(UIModele.get(uri, name, attributes));
    }

    protected void handleInclude(String file) throws SAXException {
        if (file != null && !file.isEmpty()) {
            String ff = UIWidgetTools.getFile(file).getAbsolutePath();

            try {
                convert(ff);
            } catch (Exception e) {
                System.err.println(e);
                throw new SAXException("Cannot parse the file: " + ff);
            }
        }
    }

    /**
     * Handle a CSS style definition
     * @param css a CSS string
     */
    protected void handleStyle(String css) {
        if (css != null && !css.isEmpty()) {
            String[] files = css.split("[ ,;]");
            for (String f : files) {
                File file = UIWidgetTools.getFile(f);
                if (file.exists() && file.canRead()) {
                    Map<String, Map<String, String>> s = StyleParser.parseFile(file.getAbsolutePath());
                    if (style == null) {
                        style = s;
                    } else {
                        mergeStyles(style, s);
                    }
                }
            }
        }
    }

    /**
     * Merge two CSS style maps, style1 is modified and contains the result.
     * @param style1 first map
     * @param style2 second map
     */
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
