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

package org.scilab.forge.scidoc;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import org.xml.sax.SAXException;

import org.scilab.forge.scidoc.image.ImageConverter;
import org.scilab.forge.scidoc.image.LaTeXImageConverter;
import org.scilab.forge.scidoc.image.MathMLImageConverter;
import org.scilab.forge.scidoc.image.SVGImageConverter;
import org.scilab.forge.scidoc.scilab.ScilabLexer;
import org.scilab.forge.scidoc.scilab.HTMLScilabCodeHandler;

/**
 * Class to convert DocBook to HTML
 * @author Calixte DENIZET
 */
public class HTMLDocbookTagConverter extends DocbookTagConverter implements TemplateFiller {

    private static final String LATEXBASENAME = "Equation_LaTeX_";

    private StringBuffer buffer = new StringBuffer(8192);
    private int latexCompt;
    private String imageDir;
    private String outName;
    private boolean hasExamples;
    private int warnings;
    private int nbFiles;

    protected Map<String, String> mapId;
    protected Map<String, String> tocitem;
    protected HTMLDocbookLinkResolver.TreeId tree;
    protected Map<String, HTMLDocbookLinkResolver.TreeId> mapTreeId;

    protected TemplateHandler templateHandler;

    protected ScilabLexer scilabLexer;

    protected String bookTitle = "";
    protected String partTitle = "";
    protected String chapterTitle = "";
    protected String sectionTitle = "";
    protected String fileSubtitle = "";

    protected String refpurpose = "";
    protected String refname = "";
    protected String version;

    protected String indexFilename = "index" /*UUID.randomUUID().toString()*/ + ".html";

    /**
     * Constructor
     * @param in the input stream
     * @param inName the name of the input stream
     * @param primConf the file containing the primitives of Scilab
     * @param macroConf the file containing the macros of Scilab
     * @param out the output stream
     */
    public HTMLDocbookTagConverter(String inName, String outName, String primConf, String macroConf, String template, String version, String imageDir) throws IOException, SAXException {
        super(inName);

        this.version = version;
        this.imageDir = imageDir;
        this.outName = outName + File.separator;
        HTMLDocbookLinkResolver resolver = new HTMLDocbookLinkResolver(inName);
        mapId = resolver.getMapId();
        tocitem = resolver.getToc();
        tree = resolver.getTree();
        mapTreeId = resolver.getMapTreeId();
        scilabLexer = new ScilabLexer(primConf, macroConf);
        File tpl = new File(template);
        templateHandler = new TemplateHandler(this, tpl);
        ImageConverter.registerExternalImageConverter(LaTeXImageConverter.getInstance());
        ImageConverter.registerExternalImageConverter(MathMLImageConverter.getInstance());
        ImageConverter.registerExternalImageConverter(SVGImageConverter.getInstance());
    }

    /**
     * @return the buffer used
     */
    public StringBuffer getBuffer() {
        return buffer;
    }

    /**
     * @param tag the tag name
     * @param contents the contents to enclose between opening and closing tags
     */
    public String encloseContents(String tag, String contents) {
        buffer.setLength(0);
        if (contents == null) {
            contents = null;
        }
        buffer.append("<");
        buffer.append(tag);
        buffer.append(">");
        buffer.append(contents);
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
    public String encloseContents(String tag, String[] attrs ,String contents) {
        buffer.setLength(0);
        if (contents == null) {
            contents = "";
        }
        buffer.append("<");
        buffer.append(tag);
        if (attrs != null) {
            for (int i = 0; i < attrs.length; i += 2) {
                buffer.append(" ");
                buffer.append(attrs[i]);
                buffer.append("=\"");
                buffer.append(attrs[i + 1]);
                buffer.append("\"");
            }
        }

        buffer.append(">");
        buffer.append(contents);
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
    public String encloseContents(String tag, String clazz, String contents) {
        buffer.setLength(0);
        if (contents == null) {
            contents = "";
        }
        buffer.append("<");
        buffer.append(tag);
        buffer.append(" class=\"");
        buffer.append(clazz);
        buffer.append("\">");
        buffer.append(contents);
        buffer.append("</");
        buffer.append(tag);
        buffer.append(">");

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public boolean isEscapable(String tagName) {
        return !"latex".equals(tagName) && !"programlisting".equals(tagName) && !"synopsis".equals(tagName);
    }

    /**
     * {@inheritDoc}
     */
    public boolean isTrimable(String tagName) {
        return !"programlisting".equals(tagName) && !"synopsis".equals(tagName);
    }

    /**
     * @param fileName the file to create
     * @param subtitle the subtitle of the file
     * @param contents the contents of the file
     */
    public void createHTMLFile(String id, String fileName, String subtitle, String contents) {
        fileSubtitle = subtitle;
        nbFiles++;
        templateHandler.generateFileFromTemplate(outName + fileName, id, contents);
    }

    /**
     * {@inheritDoc}
     */
    public String makeTitle(String id) {
        return refname;
    }

    /**
     * {@inheritDoc}
     */
    public String makeSubtitle(String id) {
        return fileSubtitle;
    }

    /**
     * {@inheritDoc}
     */
    public String makePrevious(String id) {
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
    public String makePath(String id) {
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
    public String makeTop(String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"top\">");
        HTMLDocbookLinkResolver.TreeId leaf = mapTreeId.get(id);
        if (leaf == null) {
            return "";
        }

        String str = tocitem.get(id);
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
    public String makeNext(String id) {
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
    public String makeStart(String id) {
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
    public String makeTocList(String id) {
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
    public String makeLastModified(String id) {
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
    public String makeGenerationDate(String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"generationdate\">");
        buffer.append(new Date(System.currentTimeMillis()).toString());
        buffer.append("</span>\n");

        return buffer.toString();
    }

    /**
     * {@inheritDoc}
     */
    public String makeVersion(String id) {
        buffer.setLength(0);
        buffer.append("<span class=\"version\">");
        buffer.append(version);
        buffer.append("</span>\n");

        return buffer.toString();
    }

    /**
     * Handle a refentry
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefentry(Map<String, String> attributes, String contents) throws SAXException {
        String fileName = attributes.get("id") + ".html";
        createHTMLFile(attributes.get("id"), fileName, refpurpose, contents);
        if (!hasExamples) {
            warnings++;
            //System.err.println("Warning (should be fixed): no example in " + currentFileName);
        } else {
            hasExamples = false;
        }
        String rp = encloseContents("span", "refentry-description", refpurpose);
        String str = encloseContents("li", encloseContents("a", new String[]{"href", fileName, "class", "refentry"}, attributes.get("id")) + " &#8212; " + rp);
        refpurpose = "";
        refname = "";

        return str;
    }

    /**
     * Handle a section
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSection(Map<String, String> attributes, String contents) throws SAXException {
        String fileName = attributes.get("id") + ".html";
        String str = encloseContents("ul", "list-refentry", contents);
        String title = encloseContents("h3", "title-section", sectionTitle);
        createHTMLFile(attributes.get("id"), fileName, sectionTitle, title + "\n" + str);

        str = encloseContents("li", encloseContents("a", new String[]{"href", fileName, "class", "section"}, sectionTitle) + "\n" + str);
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
    public String handleBook(Map<String, String> attributes, String contents) throws SAXException {
        String str = encloseContents("ul", "list-part", contents);
        String title = encloseContents("h3", "book-title", bookTitle);
        createHTMLFile("index", indexFilename, bookTitle, title + "\n" + str);

        if (warnings != 0) {
            System.out.println("Total files without example: " + warnings);
            System.out.println("Total generated html files: " + nbFiles);
        }

        return encloseContents("li", encloseContents("a", new String[]{"href", indexFilename, "class", "part"}, bookTitle) + "\n" + str);
    }

    /**
     * Handle a part
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handlePart(Map<String, String> attributes, String contents) throws SAXException {
        String fileName = attributes.get("id") + ".html";
        String str = encloseContents("ul", "list-chapter", contents);
        String title = encloseContents("h3", "title-part", partTitle);
        createHTMLFile(attributes.get("id"), fileName, partTitle, title + "\n" + str);

        str = encloseContents("li", encloseContents("a", new String[]{"href", fileName, "class", "part"}, partTitle) + "\n" + str);
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
    public String handleChapter(Map<String, String> attributes, String contents) throws SAXException {
        String fileName = attributes.get("id") + ".html";
        String str = encloseContents("ul", "list-refentry", contents);
        String title = encloseContents("h3", "title-chapter", chapterTitle);
        createHTMLFile(attributes.get("id"), fileName, chapterTitle, title + "\n" + str);

        str = encloseContents("li", encloseContents("a", new String[]{"href", fileName, "class", "chapter"}, chapterTitle) + "\n" + str);
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
    public String handleTitle(Map<String, String> attributes, String contents) throws SAXException {
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
        } else if (parent.equals("refsection") && contents.toLowerCase().startsWith("example")) {
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
    public String handlePara(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("p", "para", contents);
    }

    /**
     * Handle a literal
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleLiteral(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("code", "literal", contents);
    }

    /**
     * Handle a refnamediv
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefnamediv(Map<String, String> attributes, String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "refnamediv", contents);
        } else {
            return encloseContents("div", "refnamediv", contents);
        }
    }

    /**
     * Handle a refname
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefname(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleRefpurpose(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleRefsynopsisdiv(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleSynopsis(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleInfo(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleRefsection(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleProgramlisting(Map<String, String> attributes, String contents) throws SAXException {
        String id = attributes.get("id");
        String str = encloseContents("div", "programlisting", encloseContents("pre", scilabLexer.convert(HTMLScilabCodeHandler.getInstance(refname, mapId), contents)));
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
    public String handlePubdate(Map<String, String> attributes, String contents) throws SAXException {
        return null;
    }

    /**
     * Handle a simplelist
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSimplelist(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleMember(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("li", "member", contents);
    }

    /**
     * Handle a link
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleLink(Map<String, String> attributes, String contents) throws SAXException {
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

        return encloseContents("a", new String[]{"href", id, "class", "link"}, contents);
    }

    /**
     * Handle an ulink
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleUlink(Map<String, String> attributes, String contents) throws SAXException {
        String link = attributes.get("url");
        if (link == null) {
            throw new SAXException("No url attribute in tag ulink");
        }

        return encloseContents("a", new String[]{"href", link, "class", "ulink"}, contents);
    }

    /**
     * Handle a xref
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleXref(Map<String, String> attributes, String contents) throws SAXException {
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

        return encloseContents("a", new String[]{"href", id, "class", "xref"}, contents);
    }

    /**
     * Handle a latex (not really a docbook tag...)
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleLatex(Map<String, String> attributes, String contents) throws SAXException {
        File f = new File(outName + imageDir, LATEXBASENAME + (latexCompt++) + ".png");
        String parent = getParentTagName();
        if (parent.equals("para") && !attributes.containsKey("style")) {
            attributes.put("style", "text");
        }
        String fs = attributes.get("fontsize");
        if (fs == null) {
            attributes.put("fontsize", "16");
        }
        return ImageConverter.getImageByCode(contents, attributes, "image/latex", f, imageDir + "/" + f.getName());
    }

    /**
     * Handle a term
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTerm(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("span", "term", contents);
    }

    /**
     * Handle a listitem
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleListitem(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleVarlistentry(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("dt", contents);
    }

    /**
     * Handle a variablelist
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleVariablelist(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("dl", contents);
    }

    /**
     * Handle an itemizedlist
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleItemizedlist(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleEmphasis(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleTr(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("tr", contents);
    }

    /**
     * Handle a td
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTd(Map<String, String> attributes, String contents) throws SAXException {
        String align = attributes.get("align");
        if (align == null) {
            return encloseContents("td", new String[]{"align", align}, contents);
        }
        return encloseContents("td", contents);
    }

    /**
     * Handle an informaltable
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleInformaltable(Map<String, String> attributes, String contents) throws SAXException {
        String id = attributes.get("id");
        String border = attributes.get("border");
        if (border == null) {
            border = "";
        }
        String cellpadding = attributes.get("cellpadding");
        if (cellpadding == null) {
            cellpadding = "";
        }
        String width = attributes.get("width");
        if (width == null) {
            width = "";
        }
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("table", new String[]{"class", "informaltable", "border", border, "cellpadding", cellpadding, "width", width}, contents);
        } else {
            return encloseContents("table", new String[]{"class", "informaltable", "border", border, "cellpadding", cellpadding, "width", width}, contents);
        }
    }

    /**
     * Handle an imagedata
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleImagedata(Map<String, String> attributes, String contents) throws SAXException {
        String fileref = attributes.get("fileref");
        if (fileref == null) {
            if (contents == null || contents.length() == 0) {
                throw new SAXException("No fileref attribute or no data in tag imagedata");
            }

            return contents;
        }

        try {
           String path = new File(new URI(currentFileName)).getParent();
           return ImageConverter.getImageByFile(attributes, path, fileref, outName, imageDir);
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
    public String handleImageobject(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    /**
     * Handle an inlinemediaobject
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleInlinemediaobject(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("span", contents);
    }

    /**
     * Handle a mediaobject
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleMediaobject(Map<String, String> attributes, String contents) throws SAXException {
        String id = attributes.get("id");
        contents = contents.replaceFirst("top:([0-9]+)px;", "");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "mediaobject", contents);
        } else {
            return encloseContents("div", "mediaobject", contents);
        }
    }

    /**
     * Handle an informalequation
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleInformalequation(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleOrderedlist(Map<String, String> attributes, String contents) throws SAXException {
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
            return "<a name=\"" + id + "\"></a>" + encloseContents("ol", new String[]{"type", numeration}, contents);
        } else {
            return encloseContents("ol", new String[]{"type", numeration}, contents);
        }
    }

    /**
     * Handle a subscript
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSubscript(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("sub", contents);
    }

    /**
     * Handle a replaceable
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleReplaceable(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("span", "replaceable", contents);
    }

    /**
     * Handle a question
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleQuestion(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("dt", encloseContents("strong", contents));
    }

    /**
     * Handle an answer
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleAnswer(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("dd", contents);
    }

    /**
     * Handle a qandaentry
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleQandaentry(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("dl", contents);
    }

    /**
     * Handle a qandaset
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleQandaset(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("div", "qandaset", contents);
    }

    /**
     * Handle a caption
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleCaption(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("caption", encloseContents("b", contents));
    }

    /**
     * Handle a tbody
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTbody(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("tbody", "tbody", contents);
    }

    /**
     * Handle a table
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTable(Map<String, String> attributes, String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("table", "doctable", contents);
        } else {
            return encloseContents("table", "doctable", contents);
        }
    }

    /**
     * Handle a surname
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleSurname(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("span", "surname", contents);
    }

    /**
     * Handle a firstname
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleFirstname(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("span", "firstname", contents);
    }

    /**
     * Handle a bibliomset
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleBibliomset(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleBibliomixed(Map<String, String> attributes, String contents) throws SAXException {
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
    public String handleTh(Map<String, String> attributes, String contents) throws SAXException {
        return encloseContents("th", contents);
    }
}
