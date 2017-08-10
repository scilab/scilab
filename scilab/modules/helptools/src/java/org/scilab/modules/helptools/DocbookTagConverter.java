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

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.helptools.external.ExternalXMLHandler;
import org.scilab.modules.helptools.image.Image;
import org.scilab.modules.helptools.image.ImageConverter;
import org.scilab.modules.helptools.image.LaTeXImageConverter;
import org.scilab.modules.helptools.image.MathMLImageConverter;
import org.scilab.modules.helptools.image.SVGImageConverter;
import org.scilab.modules.helptools.image.ScilabImageConverter;
import org.scilab.modules.helptools.image.XcosImageConverter;
import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

/**
 * Class the convert a DocBook xml file
 * @author Calixte DENIZET
 */
public abstract class DocbookTagConverter extends DefaultHandler implements Converter {

    private static final String DOCBOOKURI = "http://docbook.org/ns/docbook";
    private static final Class<?>[] argsType = new Class[] {Map.class, String.class};
    protected static final String SCI = ScilabConstants.SCI.getPath();

    private Map<String, Method> mapMeth = new HashMap<String, Method>();
    private Map<String, ExternalXMLHandler> externalHandlers = new HashMap<String, ExternalXMLHandler>();
    private List<DocbookTagConverter> converters;
    private final ImageConverter imgConvert;
    private final File in;
    private DocbookElement baseElement = new DocbookElement(null, null, null);
    private Stack<DocbookElement> stack = new Stack<DocbookElement>();
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
     * The file which is parsed
     */
    protected String currentBaseName;

    /**
     * Useful to locate the errors
     */
    protected Locator locator;

    /**
     * Constructor
     * @param in the input file path
     */
    public DocbookTagConverter(String in, final ImageConverter imgConvert) throws IOException {
        if (in != null && !in.isEmpty()) {
            this.in = new File(in);
        } else {
            this.in = null;
        }

        this.imgConvert = imgConvert;
        imgConvert.registerExternalImageConverter(new LaTeXImageConverter(this));
        imgConvert.registerExternalImageConverter(new MathMLImageConverter(this));
        imgConvert.registerExternalImageConverter(new SVGImageConverter(this));
        imgConvert.registerExternalImageConverter(new ScilabImageConverter(this));
        imgConvert.registerExternalImageConverter(new XcosImageConverter(this));
    }

    /**
     * Constructor
     * @param in the inputstream
     * @param inName the name of the input file
     */
    public DocbookTagConverter(String inName, DocbookElement elem, final ImageConverter imgConvert) throws IOException {
        this(inName, imgConvert);
        baseElement = elem;
    }

    public ImageConverter getImageConverter() {
        return imgConvert;
    }

    public String getBaseImagePath() {
        return "";
    }

    /**
     * Hook used by {@link ImageConverter}s to emit format specific code from any docbook image element
     * @param code the code to translate
     * @param fileName the filename
     * @param attrs the attribute of the image
     * @return a {@link Converter} specific code which embed the image and the source code
     */
    public abstract String generateImageCode(String code, String fileName, Map<String, String> attrs);

    /**
     * Hook used by {@link ImageConverter}s to emit format specific code from any docbook image element
     * @param img image information
     * @param fileName the filename
     * @param attrs the attribute of the image
     * @return a {@link Converter} specific code which embed the image and the source code
     */
    public abstract String generateImageCode(Image img, String fileName, Map<String, String> attrs);

    /**
     * Hook used by {@link ImageConverter}s to emit format specific code from any docbook image element
     * @param fileName the filename
     * @param attrs the attribute of the image
     * @return a {@link Converter} specific code which embed the image and the source code
     */
    public abstract String generateImageCode(String fileName, Map<String, String> attrs);

    /**
     * Handle the tag
     * @param tag the tag to handle
     * @param attributes the attributes as a Map
     * @param contents the contents between the matching tags
     * @return the HTML code
     * @throws UnhandledDocbookTagException if an handled tga is encountered
     */
    public String handleDocbookTag(String tag, Map<String, String> attributes, String contents) throws SAXException {
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
                return (String) method.invoke(this, new Object[] {attributes, contents});
            } catch (IllegalAccessException e) {
                throw new UnhandledDocbookTagException(tag);
            } catch (InvocationTargetException e) {
                throw new SAXException("Problem with tag " + tag + "\n" + e.getCause());
            }
        } else {
            throw new UnhandledDocbookTagException(tag);
        }
    }

    public String getCurrentFileName() {
        return currentFileName;
    }

    public String getCurrentBaseName() {
        return currentBaseName;
    }

    /**
     * Register an Docbook tag converter. The aim is to parse the xml one time.
     * @param c the converter to register
     */
    public void registerConverter(DocbookTagConverter c) {
        if (converters == null) {
            converters = new ArrayList<DocbookTagConverter>();
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
    protected void registerExternalXMLHandler(ExternalXMLHandler h) {
        if (externalHandlers.get(h.getURI()) == null) {
            externalHandlers.put(h.getURI(), h);
            h.setConverter(this);
        }
    }

    /**
     * Register all needed External XML Handler.
     */
    @Override
    public void registerAllExternalXMLHandlers() {
    }

    /**
     * Unregister all External XML Handler
     */
    public void unregisterAllExternalXMLHandler() {
        for (ExternalXMLHandler h : externalHandlers.values()) {
            h.setConverter(null);
        }
        externalHandlers.clear();
    }

    /**
     * @param tagName the tag name
     * @return true if the contents of the tag must be escaped
     */
    public boolean isEscapable(String tagName, String uri) {
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
        factory.setXIncludeAware(true);

        try {
            factory.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
            SAXParser parser = factory.newSAXParser();
            // Must be uncommented to be able to read comments
            //parser.setProperty("http://xml.org/sax/properties/lexical-handler", this);
            parser.parse(in, this);
        } catch (ParserConfigurationException e) {
            exceptionOccurred(e);
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
     * Install not converted files (templates, images) to the converted directory
     */
    public void install() {
    }

    /**
     * {@inheritDoc}
     */
    public InputSource resolveEntity(String publicId, String systemId) throws SAXException, IOException {
        if (publicId != null) {
            publicId = publicId.replaceAll(" ", "%20");
        }
        if (systemId != null) {
            systemId = systemId.replaceAll(" ", "%20");
        }

        currentFileName = systemId;
        currentBaseName = new File(systemId).getName();

        for (ExternalXMLHandler h : externalHandlers.values()) {
            h.resetCompt();
        }

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
        stack.push(baseElement.getNewInstance(null, null, null));
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
            Map<String, String> map = new HashMap<String, String>(len);
            for (int i = 0; i < len; i++) {
                map.put(attributes.getLocalName(i), attributes.getValue(i));
            }
            stack.push(baseElement.getNewInstance(localName, uri, map));
        } else {
            ExternalXMLHandler h = externalHandlers.get(uri);
            if (h == null) {
                exceptionOccurred(new SAXException("uri " + uri + " not handled"));
                return;
            }
            StringBuilder buf = h.startExternalXML(localName, attributes, locator);
            if (buf != null) {
                DocbookElement elem = baseElement.getNewInstance(localName, uri, null);
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
                exceptionOccurred(new SAXException("tag " + elem.getName() + " is closed with tag " + localName));
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
                exceptionOccurred(e);
                return;
            }
        } else {
            ExternalXMLHandler h = externalHandlers.get(uri);
            if (h == null) {
                exceptionOccurred(new SAXException("uri " + uri + " not handled"));
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

    /*public void comment(char[] ch, int start, int length) throws SAXException {

      }*/

    /**
     * {@inheritDoc}
     */
    public void characters(char[] ch, int start, int length) throws SAXException {
        int end = start + length;

        if (isEscapable(stack.peek().getName(), stack.peek().getURI())) {
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
     * @return the document locator
     */
    public Locator getDocumentLocator() {
        return locator;
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
    protected void fatalExceptionOccurred(Exception e) throws SAXException {
        throw new SAXException(errors + "\nFATAL error:\n" + e.getMessage());
    }

    /**
     * Handle an exception depending on the presence of a locator
     * @param e the exception to handle
     * @throws SAXException if problem
     */
    protected void exceptionOccurred(Exception e) {
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

    /*
     * All Handled tag method definition, these methods are the implementation of "Docbook 5.0 - subset Scilab"
     */

    public abstract String handleAnswer(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleBibliomixed(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleBibliomset(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleBook(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleCaption(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleCaution(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleChapter(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleCode(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleCommand(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleConstant(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleEmphasis(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleFirstname(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleFunction(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleImagedata(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleImageobject(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleImportant(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleInfo(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleInformalequation(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleInformaltable(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleInlinemediaobject(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleItemizedlist(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleLatex(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleLink(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleListitem(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleLiteral(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleLiterallayout(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleMediaobject(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleMember(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleNote(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleOption(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleOrderedlist(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handlePara(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handlePart(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleProgramlisting(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handlePubdate(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleQandaentry(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleQandaset(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleQuestion(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefentry(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefnamediv(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefname(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefpurpose(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefsection(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefsect1(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefsect2(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefsect3(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRefsynopsisdiv(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleReplaceable(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRevdescription(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRevhistory(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRevision(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRevnumber(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleRevremark(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleScreen(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleScreenshot(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleSection(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleSimplelist(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleSubscript(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleSuperscript(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleSurname(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleSynopsis(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleTable(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleTbody(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleTd(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleTerm(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleTh(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleTip(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleTitle(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleTr(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleUlink(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleVariablelist(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleVarlistentry(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleVarname(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleWarning(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleXref(final Map<String, String> attributes, final String contents) throws SAXException;
    public abstract String handleAnchor(final Map<String, String> attributes, final String contents) throws SAXException;
}
