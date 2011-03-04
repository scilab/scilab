/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.StringReader;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import org.scilab.modules.helptools.external.ExternalXMLHandler;

/**
 * Class the convert a DocBook xml file
 * @author Calixte DENIZET
 */
public class DocbookTagConverter extends DefaultHandler {

    private static final String DOCBOOKURI = "http://docbook.org/ns/docbook";
    private static final Class[] argsType = new Class[]{Map.class, String.class};

    private Map<String, Method> mapMeth = new HashMap();
    private Map<String, ExternalXMLHandler> externalHandlers = new HashMap();
    private List<DocbookTagConverter> converters;
    private final File in;
    private DocbookElement baseElement = new DocbookElement(null, null);
    private Stack<DocbookElement> stack = new Stack();
    private String errors = "";

    /**
     * True when an error is met during the parsing
     */
    protected boolean hasError;

    /**
     * The file which is parsed
     */
    protected String currentFileName;

    /**
     * Useful to locate the errors
     */
    protected Locator locator;

    /**
     * Constructor
     * @param in the input file path
     */
    public DocbookTagConverter(String in) throws IOException {
        if (in != null && !in.isEmpty()) {
            this.in = new File(in);
        } else {
            this.in = null;
        }
    }

    /**
     * Constructor
     * @param in the inputstream
     * @param inName the name of the input file
     */
    public DocbookTagConverter(String inName, DocbookElement elem) throws IOException {
        this(inName);
        baseElement = elem;
    }

    /**
     * Handle the tag
     * @param tag the tag to handle
     * @param attributes the attributes as a Map
     * @param contents the contents between the matching tags
     * @return the HTML code
     * @throws UnhandledDocbookTagException if an handled tga is encountered
     */
    public String handleDocbookTag(String tag, Map attributes, String contents) throws SAXException {
        if (tag != null && tag.length() > 0) {
            Method method = mapMeth.get(tag);
            if (method == null) {
                String name = "handle" + Character.toString(Character.toUpperCase(tag.charAt(0))) + tag.substring(1);
                try {
                    method = this.getClass().getMethod(name, argsType);
                    mapMeth.put(tag, method);
                } catch (NoSuchMethodException e) {
                    throw new UnhandledDocbookTagException(tag);
                }
            }
            try {
                return (String) method.invoke(this, new Object[]{attributes, contents});
            } catch (IllegalAccessException e) {
                throw new UnhandledDocbookTagException(tag);
            } catch (InvocationTargetException e) {
                throw new SAXException("Problem with tag " + tag + "\n" + e.getCause());
            }
        } else {
            throw new UnhandledDocbookTagException(tag);
        }
    }

    /**
     * Register an Docbook tag converter. The aim is to parse the xml one time.
     * @param c the converter to register
     */
    public void registerConverter(DocbookTagConverter c) {
        if (converters == null) {
            converters = new ArrayList();
        }

        converters.add(c);
        c.setDocumentLocator(locator);
    }

    /**
     * @param c the converter to remove
     */
    public void removeConverter(DocbookTagConverter c) {
        if (converters != null) {
            converters.remove(c);
        }
    }

    /**
     * @return all the registered converters
     */
    public DocbookTagConverter[] getConverters() {
        if (converters == null) {
            return null;
        }

        return converters.toArray(new DocbookTagConverter[0]);
    }

    /**
     * Register an XMLHandler for external XML
     * @param h the external XML handler
     */
    public void registerExternalXMLHandler(ExternalXMLHandler h) {
        if (externalHandlers.get(h.getURI()) == null) {
            externalHandlers.put(h.getURI(), h);
        }
    }

    /**
     * @param tagName the tag name
     * @return true if the contents of the tag must be escaped
     */
    public boolean isEscapable(String tagName) {
        return true;
    }

    /**
     * @param tagName the tag name
     * @return true if the contents of the tag must be trimed
     */
    public boolean isTrimable(String tagName) {
        return true;
    }

    /**
     * @return the parent tag name
     */
    public String getParentTagName() {
        return stack.peek().getName();
    }

    /**
     * @return the parent tag contents
     */
    public String getParentContent() {
        return stack.peek().getStringBuilder().toString();
    }

    /**
     * Start the conversion
     * @throws SAXException if a problem is encountered during the parsing
     * @throws IOException if an IO problem is encountered
     */
    public void convert() throws SAXException, IOException {
        SAXParserFactory factory = SAXParserFactory.newInstance();
        factory.setValidating(false);
        factory.setNamespaceAware(true);

        try {
            factory.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
            SAXParser parser = factory.newSAXParser();
            parser.parse(in, this);
        } catch (ParserConfigurationException e) {
            exceptionOccured(e);
        } catch (SAXException e) {
            System.err.println(e);
        } catch (IOException e) {
            System.err.println(e);
        }

        if (hasError) {
            throw new SAXException(errors);
        }
    }

    /**
     * {@inheritDoc}
     */
    public InputSource resolveEntity(String publicId, String systemId) throws SAXException, IOException {
        currentFileName = systemId;
        if (converters != null) {
            for (DocbookTagConverter conv : converters) {
                conv.resolveEntity(publicId, systemId);
            }
        }

        return super.resolveEntity(publicId, systemId);
    }

    /**
     * {@inheritDoc}
     */
    public void startDocument() throws SAXException {
        stack.push(baseElement.getNewInstance(null, null));
        if (converters != null) {
            for (DocbookTagConverter conv : converters) {
                conv.startDocument();
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void endDocument() throws SAXException {
        stack.clear();

        if (converters != null) {
            for (DocbookTagConverter conv : converters) {
                conv.endDocument();
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
        if (uri.equals(DOCBOOKURI)) {
            int len = attributes.getLength();
            Map<String, String> map = new HashMap(len);
            for (int i = 0; i < len; i++) {
                map.put(attributes.getLocalName(i), attributes.getValue(i));
            }
            stack.push(baseElement.getNewInstance(localName, map));
        } else {
            ExternalXMLHandler h = externalHandlers.get(uri);
            if (h == null) {
                exceptionOccured(new SAXException("uri " + uri + " not handled"));
                return;
            }
            StringBuilder buf = h.startExternalXML(localName, attributes);
            if (buf != null) {
                DocbookElement elem = baseElement.getNewInstance(localName, null);
                elem.setStringBuilder(buf);
                stack.push(elem);
            }
        }

        if (converters != null) {
            for (DocbookTagConverter conv : converters) {
                conv.startElement(uri, localName, qName, attributes);
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if (uri.equals(DOCBOOKURI)) {
            DocbookElement elem = stack.pop();
            if (!elem.getName().equals(localName)) {
                exceptionOccured(new SAXException("tag " + elem.getName() + " is closed with tag " + localName));
                return;
            }
            try {
                DocbookElement elemp = stack.peek();
                elemp.setParent(elem);
                StringBuilder buf = elem.getStringBuilder();
                if (isTrimable(elem.getName())) {
                    buf = trim(buf);
                }
                String str = handleDocbookTag(elem.getName(), elem.getAttributes(), buf.toString());
                if (str != null) {
                    elemp.getStringBuilder().append(str);
                }
            } catch (SAXException e) {
                exceptionOccured(e);
                return;
            }
        } else {
            ExternalXMLHandler h = externalHandlers.get(uri);
            if (h == null) {
                exceptionOccured(new SAXException("uri " + uri + " not handled"));
                return;
            }
            String str = h.endExternalXML(localName);
            if (str != null) {
                stack.pop();
                stack.peek().getStringBuilder().append(str);
            }
        }

        if (converters != null) {
            for (DocbookTagConverter conv : converters) {
                conv.endElement(uri, localName, qName);
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void characters(char[] ch, int start, int length) throws SAXException {
        int end = start + length;

        if (isEscapable(stack.peek().getName())) {
            StringBuilder buf = stack.peek().getStringBuilder();
            int save = start;
            for (int i = start; i < end; i++) {
                switch (ch[i]) {
                case '\'' :
                    buf.append(ch, save, i - save);
                    buf.append("&#0039;");
                    save = i + 1;
                    break;
                case '\"' :
                    buf.append(ch, save, i - save);
                    buf.append("&#0034;");
                    save = i + 1;
                    break;
                case '<' :
                    buf.append(ch, save, i - save);
                    buf.append("&lt;");
                    save = i + 1;
                    break;
                case '>' :
                    buf.append(ch, save, i - save);
                    buf.append("&gt;");
                    save = i + 1;
                    break;
                case '&' :
                    buf.append(ch, save, i - save);
                    buf.append("&amp;");
                    save = i + 1;
                    break;
                default :
                    break;
                }
            }

            if (save < end) {
                buf.append(ch, save, end - save);
            }
        } else {
            stack.peek().getStringBuilder().append(ch, start, length);
        }

        if (converters != null) {
            for (DocbookTagConverter conv : converters) {
                conv.characters(ch, start, length);
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
     * @return the used stack
     */
    protected Stack<DocbookElement> getStack() {
        return stack;
    }

    /**
     * Handle an exception depending on the presence of a locator
     * @param e the exception to handle
     * @throws SAXException if problem
     */
    protected void fatalExceptionOccured(Exception e) throws SAXException {
        throw new SAXException(errors + "\nFATAL error:\n" + e.getMessage());
    }

    /**
     * Handle an exception depending on the presence of a locator
     * @param e the exception to handle
     * @throws SAXException if problem
     */
    protected void exceptionOccured(Exception e){
        if (!hasError) {
            hasError = true;
        }
        errors += makeErrorMessage(e);
    }

    private String makeErrorMessage(Exception e) {
        String sId = "";
        if (currentFileName != null) {
            sId = "SystemID:" + currentFileName;
        }

        String file;
        try {
            file = in.getCanonicalPath();
        } catch (IOException e1) {
            e1.printStackTrace();
            file = null;
        }
        if (locator != null) {
            return "\nCannot parse " + file + ":\n" + e.getMessage() + "\n" + sId + " at line " + locator.getLineNumber();
        } else {
            return "\nCannot parse " + file + ":\n" + e.getMessage() + "\n" + sId;
        }
    }

    /**
     * @param the StringBuilder to trim
     * @return a trimed StringBuilder
     */
    private static StringBuilder trim(StringBuilder buf) {
        int start = 0;
        int i = 0;
        int end = buf.length();
        char c;
        for (; i < end; i++) {
            c = buf.charAt(i);
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                break;
            }
        }
        buf.delete(0, i);
        end = end - i;
        for (i = end - 1; i >= 0; i--) {
            c = buf.charAt(i);
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                break;
            }
        }
        buf.setLength(i + 1);

        return buf;
    }
}
