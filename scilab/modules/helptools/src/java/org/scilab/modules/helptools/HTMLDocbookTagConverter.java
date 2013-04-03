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

import java.io.File;
import java.io.IOException;
import java.lang.Character.UnicodeBlock;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.Date;
import java.util.Map;
import java.util.Stack;
import java.util.regex.Pattern;

import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.helptools.image.ImageConverter;
import org.scilab.modules.helptools.image.LaTeXImageConverter;
import org.scilab.modules.helptools.image.MathMLImageConverter;
import org.scilab.modules.helptools.image.ScilabImageConverter;
import org.scilab.modules.helptools.image.SVGImageConverter;
import org.scilab.modules.helptools.image.XcosImageConverter;
import org.scilab.modules.helptools.scilab.ScilabLexer;
import org.scilab.modules.helptools.scilab.HTMLScilabCodeHandler;
import org.scilab.modules.helptools.scilab.AbstractScilabCodeHandler;
import org.scilab.modules.helptools.XML.XMLLexer;
import org.scilab.modules.helptools.XML.HTMLXMLCodeHandler;
import org.scilab.modules.helptools.c.CLexer;
import org.scilab.modules.helptools.c.HTMLCCodeHandler;
import org.scilab.modules.helptools.java.JavaLexer;
import org.scilab.modules.localization.Messages;

/**
 * Class to convert DocBook to HTML
 * @author Calixte DENIZET
 */
public class HTMLDocbookTagConverter extends DocbookTagConverter implements TemplateFiller {

    public static enum GenerationType { WEB, JAVAHELP, CHM, HTML };

    private static final String SCILAB_URI = "http://www.scilab.org";
    private static final String LATEXBASENAME = "_LaTeX_";
    private static final String VERSION = Messages.gettext("Version");
    private static final String DESCRIPTION = Messages.gettext("Description");

    private StringBuilder buffer = new StringBuilder(8192);
    private int latexCompt = 1;
    private String imageDir;
    private String urlBase;
    private boolean linkToTheWeb;
    private boolean hasExamples;
    private int warnings;
    private int nbFiles;
    protected String outName;
    protected String outImages;

    protected Map<String, String> mapId;
    protected Map<String, String> tocitem;
    protected HTMLDocbookLinkResolver.TreeId tree;
    protected Map<String, HTMLDocbookLinkResolver.TreeId> mapTreeId;
    protected Map<String, String> mapIdPurpose;
    protected Map<String, String> mapIdRefname;

    protected TemplateHandler templateHandler;

    protected ScilabLexer scilabLexer;
    protected XMLLexer xmlLexer;
    protected CLexer cLexer;
    protected JavaLexer javaLexer;

    protected String bookTitle = "";
    protected String partTitle = "";
    protected String chapterTitle = "";
    protected String sectionTitle = "";
    protected String fileSubtitle = "";

    protected String refpurpose = "";
    protected String refname = "";
    protected String version;
    protected String appendToProgramListing;
    protected String appendForExecToProgramListing;
    protected String prependToProgramListing;
    protected String currentId;
    protected String indexFilename = "index" /*UUID.randomUUID().toString()*/ + ".html";
    protected String language;

    protected boolean isToolbox;
    protected final GenerationType type;

    /**
     * Constructor
     * @param inName the name of the input stream
     * @param outName the output directory
     * @param primConf the file containing the primitives of Scilab
     * @param macroConf the file containing the macros of Scilab
     * @param template the template to use
     * @param version the version
     * @param imageDir the image directory (relative to outName)
     * @param isToolbox is true when compile a toolbox' help
     * @param urlBase the base url for external link
     */
    public HTMLDocbookTagConverter(String inName, String outName, String[] primConf, String[] macroConf, String template, String version, String imageDir, boolean isToolbox, String urlBase, String language, GenerationType type) throws IOException, SAXException {
        super(inName);

        this.version = version;
        this.imageDir = imageDir;
        this.outName = outName + File.separator;
        this.outImages = this.outName;
        HTMLDocbookLinkResolver resolver = new HTMLDocbookLinkResolver(inName);
        mapId = resolver.getMapId();
        tocitem = resolver.getToc();
        tree = resolver.getTree();
        mapTreeId = resolver.getMapTreeId();
        mapIdPurpose = resolver.getMapIdPurpose();
        mapIdRefname = resolver.getMapIdRefname();

        scilabLexer = new ScilabLexer(primConf, macroConf);
        this.urlBase = urlBase;
        this.linkToTheWeb = urlBase != null && !urlBase.equals("scilab://");
        this.isToolbox = isToolbox;
        this.language = language;
        this.type = type;
        if (isToolbox) {// we generate a toolbox's help
            HTMLScilabCodeHandler.setLinkWriter(new AbstractScilabCodeHandler.LinkWriter() {
                public String getLink(String id) {
                    if (id.length() > 0 && id.charAt(0) == '%') {
                        id = id.replace("%", "percent");
                    }
                    String link = mapId.get(id);
                    if (link == null) {
                        return HTMLDocbookTagConverter.this.urlBase + id;
                    } else {
                        return link;
                    }
                }
            });
        } else {// we generate Scilab's help
            HTMLScilabCodeHandler.setLinkWriter(new AbstractScilabCodeHandler.LinkWriter() {
                public String getLink(String id) {
                    if (id.length() > 0 && id.charAt(0) == '%') {
                        id = id.replace("%", "percent");
                    }
                    return mapId.get(id);
                }
            });
        }

        xmlLexer = new XMLLexer();
        cLexer = new CLexer();
        javaLexer = new JavaLexer();
        File tpl = new File(template);
        templateHandler = new TemplateHandler(this, tpl, language);
        ImageConverter.registerExternalImageConverter(LaTeXImageConverter.getInstance(this));
        ImageConverter.registerExternalImageConverter(MathMLImageConverter.getInstance(this));
        ImageConverter.registerExternalImageConverter(SVGImageConverter.getInstance(this));
        ImageConverter.registerExternalImageConverter(ScilabImageConverter.getInstance(this));
        ImageConverter.registerExternalImageConverter(XcosImageConverter.getInstance(this));
    }

    public static boolean containsCJK(CharSequence seq) {
        if (seq == null) {
            return false;
        }

        for (int i = 0; i < seq.length(); i++) {
            Character.UnicodeBlock block = Character.UnicodeBlock.of(seq.charAt(i));
            if (block == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS
                    || block == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A
                    || block == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B
                    || block == Character.UnicodeBlock.CJK_COMPATIBILITY_FORMS
                    || block == Character.UnicodeBlock.CJK_COMPATIBILITY_IDEOGRAPHS
                    || block == Character.UnicodeBlock.CJK_RADICALS_SUPPLEMENT
                    || block == Character.UnicodeBlock.CJK_SYMBOLS_AND_PUNCTUATION
                    || block == Character.UnicodeBlock.ENCLOSED_CJK_LETTERS_AND_MONTHS) {
                return true;
            }
        }

        return false;
    }

    public static boolean containsCyrillic(CharSequence seq) {
        if (seq == null) {
            return false;
        }

        for (int i = 0; i < seq.length(); i++) {
            Character.UnicodeBlock block = Character.UnicodeBlock.of(seq.charAt(i));
            if (block == Character.UnicodeBlock.CYRILLIC || block == Character.UnicodeBlock.CYRILLIC_SUPPLEMENTARY) {
                return true;
            }
        }

        return false;
    }

    /**
     * Replace special chars
     */
    public static final String replaceEntity(final String str) {
        return str.replaceAll("&", "&amp;").replaceAll("<", "&#0060;").replaceAll(">", "&#0062;").replaceAll("\"", "&#0034;").replaceAll("\'", "&#0039;");
    }

    /**
     * Get the type of the generation
     * @return the generation type
     */
    public final GenerationType getGenerationType() {
        return type;
    }

    public String getBaseImagePath() {
        return "";
    }

    public String getLanguage() {
        return language;
    }

    /**
     * @return the buffer used
     */
    public StringBuilder getBuffer() {
        return buffer;
    }

    /**
     * @param tag the tag name
     * @param contents the contents to enclose between opening and closing tags
     */
    public String encloseContents(final String tag, final String contents) {
        buffer.setLength(0);
        buffer.append("<");
        buffer.append(tag);
        buffer.append(">");
        if (contents != null) {
            buffer.append(contents);
        }
        buffer.append("</");
        buffer.append(tag);
        buffer.append(">");

        return buffer.toString();
    }

    /**
     * @param tag the tag name
     * @param attrs the attributes {attr1, value1, attr2, value2, ...}
     * @param contents the contents to enclose between opening and closing tags
     */
    public String encloseContents(final String tag, final String[] attrs, final String contents) {
        buffer.setLength(0);
        buffer.append("<");
        buffer.append(tag);
        if (attrs != null) {
            for (int i = 0; i < attrs.length; i += 2) {
                if (attrs[i + 1] != null && !attrs[i + 1].isEmpty()) {
                    buffer.append(" ");
                    buffer.append(attrs[i]);
                    buffer.append("=\"");
                    buffer.append(attrs[i + 1]);
                    buffer.append("\"");
                }
            }
        }

        buffer.append(">");
        if (contents != null) {
            buffer.append(contents);
        }
        buffer.append("</");
        buffer.append(tag);
        buffer.append(">");

        return buffer.toString();
    }

    /**
     * @param tag the tag name
     * @param clazz the css class name
     * @param contents the contents to enclose between opening and closing tags
     */
    public String encloseContents(final String tag, final String clazz, final String contents) {
        buffer.setLength(0);
        buffer.append("<");
        buffer.append(tag);
        buffer.append(" class=\"");
        buffer.append(clazz);
        buffer.append("\">");
        if (contents != null) {
            buffer.append(contents);
        }
        buffer.append("</");
        buffer.append(tag);
        buffer.append(">");

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public boolean isEscapable(final String tagName, final String uri) {
        return !"latex".equals(tagName) && !"screen".equals(tagName) && !"programlisting".equals(tagName) && !"code".equals(tagName) && !"synopsis".equals(tagName) && !(uri.equals(SCILAB_URI) && tagName.equals("image"));
    }

    /**
     * {@inheritDoc}
     */
    public boolean isTrimable(final String tagName) {
        return !"screen".equals(tagName) && !"programlisting".equals(tagName) && !"code".equals(tagName) && !"synopsis".equals(tagName);
    }

    /**
     * @param fileName the file to create
     * @param subtitle the subtitle of the file
     * @param contents the contents of the file
     */
    public void createHTMLFile(final String id, final String fileName, final String subtitle, final String contents) {
        if (!hasError) {
            fileSubtitle = subtitle;
            nbFiles++;
            templateHandler.generateFileFromTemplate(outName + fileName, id, contents);
        }
    }

    /**
     * {@inheritDoc}
     */
    public String makeTitle(final String id) {
        if (refname.length() > 0) {
            return tocitem.get(id);
        }

        return "";
    }

    /**
     * {@inheritDoc}
     */
    public String makeSubtitle(final String id) {
        return fileSubtitle;
    }

    /**
     * {@inheritDoc}
     */
    public String makePrevious(final String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"previous\">");
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "";
        }
        HTMLDocbookLinkResolver.TreeId prev = leaf.getPrevious();
        if (prev.parent != null) {
            buffer.append("<a href=\"");
            buffer.append(mapId.get(prev.id));
            buffer.append("\">&lt;&lt; ");
            buffer.append(tocitem.get(prev.id));
            buffer.append("</a></span>\n");

            return buffer.toString();
        }

        return "";
    }

    /**
     * {@inheritDoc}
     */
    public String makePath(final String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"path\">");
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "";
        }
        String str = tocitem.get(id);
        leaf = leaf.parent;
        while (leaf != null && !leaf.isRoot()) {
            str = "<a href=\"" + mapId.get(leaf.id) + "\">" + tocitem.get(leaf.id) + "</a> &gt; " + str;
            leaf = leaf.parent;
        }

        str = "<a href=\"" + indexFilename  + "\">" + bookTitle + "</a> &gt;&gt; " + str;
        buffer.append(str);
        buffer.append("</span>\n");

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public String makeTop(final String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"top\">");
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "";
        }

        leaf = leaf.parent;
        if (leaf != null) {
            buffer.append("<a href=\"");
            if (!leaf.isRoot()) {
                buffer.append(mapId.get(leaf.id));
                buffer.append("\">");
                buffer.append(tocitem.get(leaf.id));
            } else {
                buffer.append(indexFilename);
                buffer.append("\">");
                buffer.append(bookTitle);
            }
            buffer.append("</a></span>\n");
        } else {
            return "";
        }

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public String makeNext(final String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"next\">");
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "";
        }
        HTMLDocbookLinkResolver.TreeId next = leaf.getNext();
        if (next != null) {
            buffer.append("<a href=\"");
            buffer.append(mapId.get(next.id));
            buffer.append("\">");
            buffer.append(tocitem.get(next.id));
            buffer.append(" &gt;&gt;</a></span>\n");

            return buffer.toString();
        }

        return "";
    }

    /**
     * {@inheritDoc}
     */
    public String makeStart(final String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"start\">");
        buffer.append("<a href=\"");
        buffer.append(indexFilename);
        buffer.append("\">");
        buffer.append(bookTitle);
        buffer.append("</a></span>\n");

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public String makeTocList(final String id) {
        buffer.setLength(0);
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "";
        }

        HTMLDocbookLinkResolver.TreeId parent = leaf.parent;
        buffer.append("<ul class=\"toc\">\n");
        String str = "";
        while (parent != null && !parent.isRoot()) {
            str = "<li class=\"parent\"><a href=\"" + mapId.get(parent.id) + "\">" + tocitem.get(parent.id) + "</a></li>\n" + str;
            parent = parent.parent;
        }
        buffer.append("<li class=\"root\"><a href=\"");
        buffer.append(indexFilename);
        buffer.append("\">");
        buffer.append(bookTitle);
        buffer.append("</a></li>\n");
        buffer.append(str);

        parent = leaf.parent;

        for (HTMLDocbookLinkResolver.TreeId c : parent.children) {
            if (c == leaf) {
                buffer.append("<li class=\"list-active\"><a href=\"");
            } else {
                buffer.append("<li><a href=\"");
            }
            buffer.append(mapId.get(c.id));
            buffer.append("\">");
            buffer.append(tocitem.get(c.id));
            buffer.append("</a></li>\n");
        }
        buffer.append("</ul>\n");

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public String makeLastModified(final String id) {
        buffer.setLength(0);
        try {
            buffer.append("<span class=\"lastmodified\">");
            buffer.append(new Date(new File(new URI(currentFileName)).lastModified()).toString());
            buffer.append("</span>\n");
        } catch (URISyntaxException e) {
            e.printStackTrace();
        }
        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public String makeGenerationDate(final String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"generationdate\">");
        buffer.append(new Date(System.currentTimeMillis()).toString());
        buffer.append("</span>\n");

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public String makeVersion(final String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"version\">");
        buffer.append(version);
        buffer.append("</span>\n");

        return buffer.toString();
    }

    public InputSource resolveEntity(String publicId, String systemId) throws SAXException, IOException {
        latexCompt = 1;
        return super.resolveEntity(publicId, systemId);
    }

    /**
     * Handle a refentry
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefentry(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            currentId = id;
        }
        String fileName = mapId.get(currentId);
        String needsExampleAttr = attributes.get("needs-examples");
        createHTMLFile(currentId, fileName, refpurpose, contents);
        if (!hasExamples && (needsExampleAttr == null || !needsExampleAttr.equals("no"))) {
            warnings++;
            //System.err.println("Warning (should be fixed): no example in " + currentFileName);
        } else {
            hasExamples = false;
        }
        String rp = encloseContents("span", "refentry-description", refpurpose);
        String str = encloseContents("li", encloseContents("a", new String[] {"href", fileName, "class", "refentry"}, refname) + " &#8212; " + rp);
        refpurpose = "";
        refname = "";
        currentId = null;

        return str;
    }

    /**
     * Handle a section
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSection(final Map<String, String> attributes, final String contents) throws SAXException {
        String fileName = attributes.get("id") + ".html";
        String str = encloseContents("ul", "list-refentry", contents);
        String title = encloseContents("h3", "title-section", sectionTitle);
        createHTMLFile(attributes.get("id"), fileName, sectionTitle, title + "\n" + str);

        str = encloseContents("li", encloseContents("a", new String[] {"href", fileName, "class", "section"}, sectionTitle) + "\n" + str);
        sectionTitle = "";

        return str;
    }

    /**
     * Handle a book
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleBook(final Map<String, String> attributes, final String contents) throws SAXException {
        String str = encloseContents("ul", "list-part", contents);
        String btitle;
        if (bookTitle.trim().equalsIgnoreCase("Scilab")) {
            btitle = version;
        } else {
            btitle = bookTitle;
        }
        String title = encloseContents("h3", "book-title", btitle);
        createHTMLFile("index", indexFilename, btitle, title + "\n" + str);

        if (warnings != 0) {
            System.err.println("Total files without example: " + warnings);
            System.err.println("Total generated html files: " + nbFiles);
        }

        return encloseContents("li", encloseContents("a", new String[] {"href", indexFilename, "class", "part"}, bookTitle) + "\n" + str);
    }

    /**
     * Handle a part
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handlePart(final Map<String, String> attributes, final String contents) throws SAXException {
        String fileName = attributes.get("id") + ".html";
        String str = encloseContents("ul", "list-chapter", contents);
        String title = encloseContents("h3", "title-part", partTitle);
        createHTMLFile(attributes.get("id"), fileName, partTitle, title + "\n" + str);
        str = encloseContents("li", encloseContents("a", new String[] {"href", fileName, "class", "part"}, partTitle) + "\n" + str);
        partTitle = "";

        return str;
    }

    /**
     * Handle a chapter
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleChapter(final Map<String, String> attributes, final String contents) throws SAXException {
        String fileName = attributes.get("id") + ".html";
        String str = encloseContents("ul", "list-refentry", contents);
        String title = encloseContents("h3", "title-chapter", chapterTitle);
        createHTMLFile(attributes.get("id"), fileName, chapterTitle, title + "\n" + str);

        str = encloseContents("li", encloseContents("a", new String[] {"href", fileName, "class", "chapter"}, chapterTitle) + "\n" + str);
        chapterTitle = "";

        return str;
    }

    // partiellement merdique car le style de title depend du noeud pere.
    /**
     * Handle a title
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTitle(final Map<String, String> attributes, final String contents) throws SAXException {
        String clazz = "title";
        String parent = getParentTagName();
        if (parent.equals("chapter")) {
            chapterTitle = contents;
        } else if (parent.equals("part")) {
            partTitle = contents;
        } else if (parent.equals("info")) {
            bookTitle = contents;
        } else if (parent.equals("section")) {
            sectionTitle = contents;
        } else if (parent.equals("refsection") && Pattern.matches("^[ \\t]*ex[ea]mpl[eo].*", contents.toLowerCase())) {
            hasExamples = true;
            return encloseContents("h3", clazz, contents);
        } else {
            return encloseContents("h3", clazz, contents);
        }

        return null;
    }

    /**
     * Handle a para
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handlePara(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("p", "para", contents);
    }

    /**
     * Handle a literal
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleLiteral(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("code", "literal", contents);
    }

    /**
     * Handle a varname
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleVarname(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("code", "varname", contents);
    }

    /**
     * Handle a command
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleCommand(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("code", "command", contents);
    }

    /**
     * Handle a code
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleCode(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("code", "scilabcode", scilabLexer.convert(HTMLScilabCodeHandler.getInstance(refname, currentFileName), contents));//encloseContents("code", "code", contents);
    }

    /**
     * Handle a function
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleFunction(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("code", "function", contents);
    }

    /**
     * Handle a constant
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleConstant(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("code", "constant", contents);
    }

    /**
     * Handle an option
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleOption(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("code", "option", contents);
    }

    /**
     * Handle a refnamediv
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefnamediv(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            currentId = id;
        }

        return encloseContents("div", "refnamediv", contents);
    }

    /**
     * Handle a refname
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefname(final Map<String, String> attributes, final String contents) throws SAXException {
        refname = contents;
        return encloseContents("h1", "refname", contents);
    }

    /**
     * Handle a refpurpose
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefpurpose(final Map<String, String> attributes, final String contents) throws SAXException {
        refpurpose = contents;
        return encloseContents("p", "refpurpose", contents);
    }

    /**
     * Handle a refsynopsisdiv
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefsynopsisdiv(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "refsynopsisdiv", contents);
        } else {
            return encloseContents("div", "refsynopsisdiv", contents);
        }
    }

    /**
     * Handle a synopsis
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSynopsis(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String str = encloseContents("div", "synopsis", encloseContents("pre", SynopsisLexer.convert(refname, contents)));
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + str;
        } else {
            return str;
        }
    }

    /**
     * Handle a info
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleInfo(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "info", contents);
        } else {
            return encloseContents("div", "info", contents);
        }
    }

    /**
     * Handle a refsection
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefsection(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "refsection", contents);
        } else {
            return encloseContents("div", "refsection", contents);
        }
    }

    /**
     * Handle a progamlisting
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleProgramlisting(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String role = attributes.get("role");
        String str;
        if (role == null) {
            String code = encloseContents("pre", "scilabcode", scilabLexer.convert(HTMLScilabCodeHandler.getInstance(refname, currentFileName), contents));
            if (prependToProgramListing != null) {
                code = prependToProgramListing + code;
            }
            if (appendToProgramListing != null) {
                code += appendToProgramListing;
            }
            str = encloseContents("div", "programlisting", code);
        } else {
            if (role.equals("xml")) {
                str = encloseContents("div", "programlisting", encloseContents("pre", "xmlcode", xmlLexer.convert(HTMLXMLCodeHandler.getInstance(), contents)));
            } else if (role.equals("c") || role.equals("cpp") || role.equals("code_gateway")) {
                hasExamples = true;
                str = encloseContents("div", "programlisting", encloseContents("pre", "ccode", cLexer.convert(HTMLCCodeHandler.getInstance(), contents)));
            } else if (role.equals("java")) {
                hasExamples = true;
                str = encloseContents("div", "programlisting", encloseContents("pre", "ccode", javaLexer.convert(HTMLCCodeHandler.getInstance(), contents)));
            } else if (role.equals("exec")) {
                String code = encloseContents("pre", "scilabcode", scilabLexer.convert(HTMLScilabCodeHandler.getInstance(refname, currentFileName), contents));
                if (prependToProgramListing != null) {
                    code = prependToProgramListing + code;
                }
                if (appendForExecToProgramListing != null) {
                    code += appendForExecToProgramListing;
                }
                str = encloseContents("div", "programlisting", code);
            } else if (role.equals("no-scilab-exec")) {
                hasExamples = true;
                String code = encloseContents("pre", "scilabcode", scilabLexer.convert(HTMLScilabCodeHandler.getInstance(refname, currentFileName), contents));
                str = encloseContents("div", "programlisting", code);
            } else {
                String code = encloseContents("pre", "scilabcode", scilabLexer.convert(HTMLScilabCodeHandler.getInstance(refname, currentFileName), contents));
                if (prependToProgramListing != null) {
                    code = prependToProgramListing + code;
                }
                if (appendToProgramListing != null) {
                    code += appendToProgramListing;
                }
                str = encloseContents("div", "programlisting", code);
            }
        }
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + str;
        } else {
            return str;
        }
    }

    /**
     * Handle a screen
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleScreen(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String str = encloseContents("div", "screen", encloseContents("pre", contents));
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + str;
        } else {
            return str;
        }
    }

    /**
     * Handle a pubdate
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handlePubdate(final Map<String, String> attributes, final String contents) throws SAXException {
        return null;
    }

    /**
     * Handle a simplelist
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSimplelist(final Map<String, String> attributes, final String contents) throws SAXException {
        String style = "itemizedlist";

        return encloseContents("ul", style, contents);
    }

    /**
     * Handle a member
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleMember(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("li", "member", contents);
    }

    /**
     * Handle a link
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleLink(final Map<String, String> attributes, final String contents) throws SAXException {
        String link = attributes.get("linkend");
        if (link == null) {
            throw new SAXException("No linkend attribute in tag link");
        }

        String type = attributes.get("type");
        String id;
        if (type != null && type.equals("scilab")) {
            id = resolvScilabLink(link);
        } else if (type != null && type.equals("remote")) {
            id = makeRemoteLink(link);
        } else {
            id = mapId.get(link);
        }

        if (id == null) {
            warnings++;
            System.err.println("Warning (should be fixed): invalid internal link to " + link + " in " + currentFileName + "\nat line " + locator.getLineNumber());
            return null;
        }

        Stack<DocbookElement> stack = getStack();
        String refnameTarget = mapIdRefname.get(link);
        String str;
        if (contents != null && !contents.isEmpty()) {
            str = contents;
        } else {
            str = refnameTarget;
        }

        if (str == null) {
            warnings++;
            System.err.println("Warning (should be fixed): empty link (no text will be printed) to " + link + " in " + currentFileName + "\nat line " + locator.getLineNumber());
        }

        String href = encloseContents("a", new String[] {"href", id, "class", "link"}, str);

        int s = stack.size();
        if (s >= 3) {
            DocbookElement elem = stack.get(s - 3);
            if (elem.getName().equals("refsection")) {
                String role = elem.getAttributes().get("role");
                if (role != null && role.equals("see also")) {
                    String purpose = mapIdPurpose.get(link);
                    if (purpose != null) {
                        return href + " &#8212; " + purpose;
                    } else {
                        return href;
                    }
                }
            }
        }

        return href;
    }

    /**
     * Rewrite a link when its type is "scilab"
     * @param link the link
     * @return the modified link with protocol scilab:// for example
     */
    protected String resolvScilabLink(String link) {
        int pos = link.indexOf("/");
        if (pos == -1) {
            return null;
        }
        String first = link.substring(0, pos);
        String second = link.substring(pos + 1);
        String[] toks = first.split("\\.");
        if (toks == null || toks.length != 2) {
            return null;
        }

        if (!linkToTheWeb) {
            return urlBase + link;
        } else {
            if (toks[0].equals("scilab") && toks[1].equals("help")) {
                return urlBase + second + ".html";
            } else {
                return "#";
            }
        }
    }

    /**
     * Make a remote link
     * @param link the link
     * @return the good link
     */
    protected String makeRemoteLink(String link) {
        return link;
    }

    /**
     * Handle an ulink
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleUlink(final Map<String, String> attributes, final String contents) throws SAXException {
        String link = attributes.get("url");
        if (link == null) {
            throw new SAXException("No url attribute in tag ulink");
        }

        return encloseContents("a", new String[] {"href", link, "class", "ulink"}, contents);
    }

    /**
     * Handle a xref
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleXref(final Map<String, String> attributes, final String contents) throws SAXException {
        String link = attributes.get("linkend");
        if (link == null) {
            throw new SAXException("No linkend attribute in tag link");
        }

        String id = mapId.get(link);
        if (id == null) {
            warnings++;
            System.err.println("Warning (should be fixed): invalid internal link to " + link + " in " + currentFileName + "\nat line " + locator.getLineNumber());
            return null;
        }

        return encloseContents("a", new String[] {"href", id, "class", "xref"}, contents);
    }

    /**
     * Handle a latex (not really a docbook tag...)
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleLatex(final Map<String, String> attributes, final String contents) throws SAXException {
        boolean isLocalized = "true".equals(attributes.get("localized"));
        File f;
        if (isLocalized) {
            f = new File(outImages + "/" + imageDir, LATEXBASENAME + currentBaseName + "_" + language + "_" + (latexCompt++) + ".png");
        } else {
            if ("ru_RU".equals(language) && HTMLDocbookTagConverter.containsCyrillic(contents)) {
                System.err.println("Warning: LaTeX code in " + getCurrentFileName() + " contains cyrillic character. The tag <latex> should contain the attribute scilab:localized=\"true\"");
            } else if ("ja_JP".equals(language) && HTMLDocbookTagConverter.containsCJK(contents)) {
                System.err.println("Warning: LaTeX code in " + getCurrentFileName() + " contains CJK character. The tag <latex> should contain the attribute scilab:localized=\"true\"");
            }
            f = new File(outImages + "/" + imageDir, LATEXBASENAME + currentBaseName + "_" + (latexCompt++) + ".png");
        }

        String parent = getParentTagName();
        if (parent.equals("para") && !attributes.containsKey("style")) {
            attributes.put("style", "text");
        }
        String fs = attributes.get("fontsize");
        if (fs == null) {
            attributes.put("fontsize", "16");
        }
        return ImageConverter.getImageByCode(currentFileName, contents, attributes, "image/latex", f, imageDir + "/" + f.getName(), getBaseImagePath(), locator.getLineNumber(), language, isLocalized);
    }

    /**
     * Handle a term
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTerm(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("span", "term", contents);
        } else {
            return encloseContents("span", "term", contents);
        }
    }

    /**
     * Handle a listitem
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleListitem(final Map<String, String> attributes, final String contents) throws SAXException {
        String parent = getParentTagName();
        if (parent.equals("varlistentry")) {
            return encloseContents("dd", contents);
        }
        return encloseContents("li", contents);
    }

    /**
     * Handle a varlistentry
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleVarlistentry(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("dt", contents);
    }

    /**
     * Handle a variablelist
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleVariablelist(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("dl", contents);
    }

    /**
     * Handle an itemizedlist
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleItemizedlist(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("ul", "itemizedlist", contents);
        } else {
            return encloseContents("ul", "itemizedlist", contents);
        }
    }

    /**
     * Handle an emphasis
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleEmphasis(final Map<String, String> attributes, final String contents) throws SAXException {
        String role = attributes.get("role");
        if (role != null) {
            if (role.equals("bold")) {
                return encloseContents("b", contents);
            }
            if (role.equals("italic")) {
                return encloseContents("i", contents);
            }
        }

        return encloseContents("em", contents);
    }

    /**
     * Handle a tr
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTr(final Map<String, String> attributes, final String contents) throws SAXException {
        String bgcolor = attributes.get("bgcolor");
        return encloseContents("tr", new String[] {"bgcolor", bgcolor}, contents);
    }

    /**
     * Handle a td
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTd(final Map<String, String> attributes, final String contents) throws SAXException {
        String align = attributes.get("align");
        String bgcolor = attributes.get("bgcolor");
        return encloseContents("td", new String[] {"align", align, "bgcolor", bgcolor}, contents);
    }

    /**
     * Handle an informaltable
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleInformaltable(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String bgcolor = attributes.get("bgcolor");
        String border = attributes.get("border");
        String cellpadding = attributes.get("cellpadding");
        String width = attributes.get("width");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("table", new String[] {"class", "informaltable", "border", border, "cellpadding", cellpadding, "width", width}, contents);
        } else {
            return encloseContents("table", new String[] {"class", "informaltable", "border", border, "cellpadding", cellpadding, "width", width}, contents);
        }
    }

    /**
     * Handle an imagedata
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleImagedata(final Map<String, String> attributes, final String contents) throws SAXException {
        String fileref = attributes.get("fileref");
        if (fileref == null) {
            if (contents == null || contents.length() == 0) {
                throw new SAXException("No fileref attribute or no data in tag imagedata");
            }

            return contents;
        }

        try {
            String path = new File(new URI(currentFileName)).getParent();
            File file = ImageConverter.imageExists(path, fileref);
            if (file != null) {
                throw new SAXException("The given fileref is not on an existing image file:\n" + fileref + " [" + file + "]");
            }

            return ImageConverter.getImageByFile(attributes, path, fileref, outImages, imageDir, getBaseImagePath());
        }  catch (URISyntaxException e) {
            System.err.println(e);
        }

        return null;
    }

    /**
     * Handle an imageobject
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleImageobject(final Map<String, String> attributes, final String contents) throws SAXException {
        return contents;
    }

    /**
     * Handle an inlinemediaobject
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleInlinemediaobject(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("span", contents);
    }

    /**
     * Handle a screenshot
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleScreenshot(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "screenshot", contents);
        } else {
            return encloseContents("div", "screenshot", contents);
        }
    }

    /**
     * Handle a mediaobject
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleMediaobject(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String c = contents.replaceFirst("top:([0-9]+)px;", "");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "mediaobject", c);
        } else {
            return encloseContents("div", "mediaobject", c);
        }
    }

    /**
     * Handle an informalequation
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleInformalequation(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "informalequation", contents);
        } else {
            return encloseContents("div", "informalequation", contents);
        }
    }

    /**
     * Handle an orderedlist
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleOrderedlist(final Map<String, String> attributes, final String contents) throws SAXException {
        String numeration = "1";
        String numAttr = attributes.get("numeration");
        if (numAttr != null) {
            if (numAttr.equals("loweralpha")) {
                numeration = "a";
            } else if (numAttr.equals("upperalpha")) {
                numeration = "A";
            } else if (numAttr.equals("lowerroman")) {
                numeration = "i";
            } else if (numAttr.equals("upperroman")) {
                numeration = "I";
            }
        }

        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("ol", new String[] {"type", numeration}, contents);
        } else {
            return encloseContents("ol", new String[] {"type", numeration}, contents);
        }
    }

    /**
     * Handle a subscript
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSubscript(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("sub", contents);
    }

    /**
     * Handle a superscript
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSuperscript(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("sup", contents);
    }

    /**
     * Handle a replaceable
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleReplaceable(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("span", "replaceable", contents);
    }

    /**
     * Handle a question
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleQuestion(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("dt", encloseContents("strong", contents));
    }

    /**
     * Handle an answer
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleAnswer(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("dd", contents);
    }

    /**
     * Handle a qandaentry
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleQandaentry(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("dl", contents);
    }

    /**
     * Handle a qandaset
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleQandaset(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("div", "qandaset", contents);
    }

    /**
     * Handle a caption
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleCaption(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("caption", encloseContents("b", contents));
    }

    /**
     * Handle a tbody
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTbody(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("tbody", "tbody", contents);
    }

    /**
     * Handle a table
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTable(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String bgcolor = attributes.get("bgcolor");
        String border = attributes.get("border");
        String cellpadding = attributes.get("cellpadding");

        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("table", new String[] {"class", "doctable", "bgcolor", bgcolor, "border", border, "cellpadding", cellpadding}, contents);
        } else {
            return encloseContents("table", new String[] {"class", "doctable", "bgcolor", bgcolor, "border", border, "cellpadding", cellpadding}, contents);
        }
    }

    /**
     * Handle a surname
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSurname(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("span", "surname", contents);
    }

    /**
     * Handle a firstname
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleFirstname(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("span", "firstname", contents);
    }

    /**
     * Handle a bibliomset
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleBibliomset(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "bibliomset", contents);
        } else {
            return encloseContents("div", "bibliomset", contents);
        }
    }

    /**
     * Handle a bibliomixed
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleBibliomixed(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "bibliomixed", contents);
        } else {
            return encloseContents("div", "bibliomixed", contents);
        }
    }

    /**
     * Handle a th
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTh(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("th", contents);
    }

    /**
     * Handle a revhistory
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRevhistory(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String str = "<table class=\"revhistory\"><tr class=\"title\"><td>" + VERSION + "</td><td>" + DESCRIPTION + "</td></tr>" + contents + "</table>";
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + str;
        } else {
            return str;
        }
    }

    /**
     * Handle a revision
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRevision(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("tr", contents);
    }

    /**
     * Handle a revnumber
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRevnumber(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("td", "revnumber", contents);
    }

    /**
     * Handle a revremark
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRevremark(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("td", "revremark", contents);
    }

    /**
     * Handle a revdescription
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRevdescription(final Map<String, String> attributes, final String contents) throws SAXException {
        return encloseContents("td", "revdescription", contents);
    }

    /**
     * Handle a note
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleNote(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabNote.png\"/></td><td valign=\"top\">" + encloseContents("div", "note", contents) + "</tr></table>";
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + code;
        } else {
            return code;
        }
    }

    /**
     * Handle a warning
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleWarning(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabWarning.png\"/></td><td valign=\"top\">" + encloseContents("div", "warning", contents) + "</tr></table>";
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + code;
        } else {
            return code;
        }
    }

    /**
     * Handle a caution
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleCaution(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabCaution.png\"/></td><td valign=\"top\">" + encloseContents("div", "caution", contents) + "</tr></table>";
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + code;
        } else {
            return code;
        }
    }

    /**
     * Handle a tip
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTip(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabTip.png\"/></td><td valign=\"top\">" + encloseContents("div", "tip", contents) + "</tr></table>";
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + code;
        } else {
            return code;
        }
    }

    /**
     * Handle a important
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleImportant(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabImportant.png\"/></td><td valign=\"top\">" + encloseContents("div", "important", contents) + "</tr></table>";
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + code;
        } else {
            return code;
        }
    }
}
