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
import java.net.URI;
import java.net.URISyntaxException;
import java.util.Date;
import java.util.Map;
import java.util.Stack;
import java.util.regex.Pattern;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.helptools.XML.HTMLXMLCodeHandler;
import org.scilab.modules.helptools.XML.XMLLexer;
import org.scilab.modules.helptools.c.CLexer;
import org.scilab.modules.helptools.c.HTMLCCodeHandler;
import org.scilab.modules.helptools.external.HTMLMathMLHandler;
import org.scilab.modules.helptools.external.HTMLSVGHandler;
import org.scilab.modules.helptools.external.HTMLScilabHandler;
import org.scilab.modules.helptools.image.Image;
import org.scilab.modules.helptools.image.ImageConverter;
import org.scilab.modules.helptools.java.JavaLexer;
import org.scilab.modules.helptools.scilab.AbstractScilabCodeHandler;
import org.scilab.modules.helptools.scilab.HTMLScilabCodeHandler;
import org.scilab.modules.helptools.scilab.ScilabLexer;
import org.scilab.modules.localization.Messages;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

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

    private final StringBuilder buffer = new StringBuilder(8192);
    private int latexCompt = 1;
    private boolean hasExamples;
    private int warnings;
    private int nbFiles;

    private final String imageDir;
    protected String urlBase;
    protected boolean linkToTheWeb;
    protected final String outName;
    protected String outImages;

    protected final Map<String, String> mapId;
    protected final Map<String, String> tocitem;
    protected final HTMLDocbookLinkResolver.TreeId tree;
    protected final Map<String, HTMLDocbookLinkResolver.TreeId> mapTreeId;
    protected final Map<String, String> mapIdPurpose;
    protected final Map<String, String> mapIdRefname;

    protected final TemplateHandler templateHandler;

    protected final ScilabLexer scilabLexer;
    protected final XMLLexer xmlLexer;
    protected final CLexer cLexer;
    protected final JavaLexer javaLexer;

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
    protected final String language;

    protected final boolean isToolbox;
    protected final Backend type;

    /**
     * Constructor
     * @param inName the name of the input stream
     * @param sciDocMain provide useful doc generation properties
     * @param imgConvert the shared image converter for all generation
     */
    public HTMLDocbookTagConverter(String inName, SciDocMain sciDocMain, ImageConverter imgConvert) throws IOException, SAXException {
        super(inName, imgConvert);

        this.version = sciDocMain.getConf().getVersion();
        this.imageDir = sciDocMain.getImagedir();
        this.outName = sciDocMain.getOutputDirectory() + File.separator;
        this.outImages = this.outName;

        imgConvert.setDocbookTagConverter(this);
        HTMLDocbookLinkResolver resolver = new HTMLDocbookLinkResolver(inName);

        mapId = resolver.getMapId();
        tocitem = resolver.getToc();
        tree = resolver.getTree();
        mapTreeId = resolver.getMapTreeId();
        mapIdPurpose = resolver.getMapIdPurpose();
        mapIdRefname = resolver.getMapIdRefname();

        this.isToolbox = sciDocMain.isToolbox();
        this.language = sciDocMain.getLanguage();
        this.type = sciDocMain.getFormat();

        if (isToolbox) {
            urlBase = sciDocMain.getConf().getWebSiteURL() + language + "/";
            linkToTheWeb = true;
        } else {
            urlBase = null;
            linkToTheWeb = false;
        }

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

        scilabLexer = new ScilabLexer(sciDocMain.getConf().getBuiltins(), sciDocMain.getConf().getMacros());
        xmlLexer = new XMLLexer();
        cLexer = new CLexer();
        javaLexer = new JavaLexer();

        final String template = sciDocMain.getConf().getTemplate(sciDocMain.getFormat().toString().toLowerCase());
        File tpl = new File(template);
        if (!tpl.isFile()) {
            final String msg = "Could not find template document: " + template;
            System.err.println(msg);
            throw new RuntimeException();
        }
        templateHandler = new TemplateHandler(this, tpl, language);
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
    public final Backend getGenerationType() {
        return type;
    }

    public String getLanguage() {
        return language;
    }

    @Override
    public void registerAllExternalXMLHandlers() {
        super.registerAllExternalXMLHandlers();

        registerExternalXMLHandler(new HTMLMathMLHandler(outImages, imageDir));
        registerExternalXMLHandler(new HTMLSVGHandler(outImages, imageDir));
        registerExternalXMLHandler(new HTMLScilabHandler(outImages, imageDir));
    }

    @Override
    public void install() {
        super.install();

        ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/css/scilab_code.css"), new File(outName + "/scilab_code.css"));
        ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/css/xml_code.css"), new File(outName + "/xml_code.css"));
        ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/css/c_code.css"), new File(outName + "/c_code.css"));
        ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/css/style.css"), new File(outName + "/style.css"));
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
     * @param code the code to translate
     * @param img image information
     * @param fileName the filename
     * @param attrs the attribute of the image
     * @return the HTML code to insert the image
     */
    public String generateCode(Image img, String fileName, Map<String, String> attrs, final String tooltipString) {
        String style = attrs.get("style");
        String top = "";
        boolean display = style != null && style.equals("display");

        if (!display) {
            top = "top:" + img.descent + "px;";
        }

        String alignAttr = attrs.get("align");
        String align = "";
        String div = "div";
        if (alignAttr != null) {
            align = " style=\'text-align:" + alignAttr + "\'";
        } else if (display) {
            align = " style=\'text-align:center\'";
        } else {
            div = "span";
        }

        return "<" + div + align + "><img src=\'" + fileName + "\' style=\'position:relative;" + top  + "width:" + img.width + "px;height:" + img.height + "px\'/></" + div + ">";
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

    private boolean isLinkedImage() {
        Stack<DocbookElement> stack = getStack();
        return stack.size() >= 3 && stack.get(stack.size() - 3).getName().equals("link");
    }

    @Override
    public String generateImageCode(String code, String fileName, Map<String, String> attrs) {
        String alignAttr = attrs.get("align");
        boolean addDiv = getGenerationType() != Backend.JAVAHELP || !isLinkedImage();
        final StringBuilder buffer = new StringBuilder(128);
        if (addDiv && alignAttr != null) {
            buffer.append("<div style=\'text-align:").append(alignAttr).append("\'>");
        }
        buffer.append("<img src=\'").append(fileName).append("\'/>");
        if (addDiv && alignAttr != null) {
            buffer.append("</div>");
        }

        if (getGenerationType() == Backend.WEB) {
            /* Prepare the code for the html inclusion */
            code = convertCode(code);
            /* Provide a tooltip */
            return "<div rel='tooltip' title='" + code + "'>" + buffer.toString() + "</div>";
        } else {
            /* No tooltip in the javahelp browser ...
             * too limited html capabilities */
            return buffer.toString();
        }
    }

    @Override
    public String generateImageCode(Image img, String fileName, Map<String, String> attrs) {
        String style = attrs.get("style");
        String top = "";
        boolean display = style != null && style.equals("display");

        if (!display) {
            top = "top:" + img.descent + "px;";
        }

        String alignAttr = attrs.get("align");
        String align = "";
        String div = "div";
        if (alignAttr != null) {
            align = " style=\'text-align:" + alignAttr + "\'";
        } else if (display) {
            align = " style=\'text-align:center\'";
        } else {
            div = "span";
        }

        if (getGenerationType() == Backend.JAVAHELP && isLinkedImage()) {
            // Java HTML renderer is not good... so when  the image is linked, we remove the div
            return "<img src=\'" + fileName + "\' style=\'position:relative;" + top  + "width:" + img.width + "px;height:" + img.height + "px\'/>>";
        } else {
            return "<" + div + align + "><img src=\'" + fileName + "\' style=\'position:relative;" + top  + "width:" + img.width + "px;height:" + img.height + "px\'/></" + div + ">";
        }
    }

    @Override
    public String generateImageCode(String fileName, Map<String, String> attrs) {
        String alignAttr = attrs.get("align");
        boolean addDiv = getGenerationType() != Backend.JAVAHELP || !isLinkedImage();
        final StringBuilder buffer = new StringBuilder(128);
        if (addDiv && alignAttr != null) {
            buffer.append("<div style=\'text-align:").append(alignAttr).append("\'>");
        }
        buffer.append("<img src=\'").append(fileName).append("\'/>");
        if (addDiv && alignAttr != null) {
            buffer.append("</div>");
        }

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
        } else if (parent.equals("refsect1")) {
            return encloseContents("h3", clazz, contents);
        } else if (parent.equals("refsect2")) {
            return encloseContents("h4", clazz, contents);
        } else if (parent.equals("refsect3")) {
            return encloseContents("h5", clazz, contents);
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
     * Handle a literallayout
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleLiterallayout(final Map<String, String> attributes, final String contents) throws SAXException {

        //replace \n by <br>
        String s = contents.replace("\n", "<BR>");
        //replace spaces by &nbsp;
        s = s.replace(" ", "&nbsp;");

        return encloseContents("code", "literallayout", s);
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
     * Handle a refsect1
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefsect1(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "refsect1", contents);
        } else {
            return encloseContents("div", "refsect1", contents);
        }
    }

    /**
     * Handle a refsect2
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefsect2(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "refsect2", contents);
        } else {
            return encloseContents("div", "refsect2", contents);
        }
    }

    /**
     * Handle a refsect3
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleRefsect3(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + encloseContents("div", "refsect3", contents);
        } else {
            return encloseContents("div", "refsect3", contents);
        }
    }

    /**
     * Handle an anchor
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleAnchor(final Map<String, String> attributes, final String contents) throws SAXException {
        String id = attributes.get("id");
        if (id != null) {
            return "<a name=\"" + id + "\">" + contents + "</a>";
        } else {
            return contents;
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
            if (this.type == Backend.JAVAHELP || this.type == Backend.HTML) {
                id = resolvScilabLink(link);
            } else {
                return contents;
            }
        } else if (type != null && type.equals("remote")) {
            id = makeRemoteLink(link);
        } else {
            id = mapId.get(link);
        }

        if (id == null) {
            if (isToolbox) {
                if (this.type == Backend.HTML) {
                    id = urlBase + link;
                    if (linkToTheWeb) {
                        id += ".html";
                    }
                }
                if (this.type == Backend.JAVAHELP) {
                    id = urlBase + link;
                }
            } else {
                warnings++;
                System.err.println("Warning (should be fixed): invalid internal link to " + link + " in " + currentFileName + "\nat line " + locator.getLineNumber());
                return null;
            }
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

        return getImageConverter().getImageByCode(currentFileName, contents, attributes, "image/latex", f, imageDir + "/" + f.getName(), getBaseImagePath(), locator.getLineNumber(), language, isLocalized);
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
        String align   = attributes.get("align");
        String valign  = attributes.get("valign");
        String bgcolor = attributes.get("bgcolor");
        String colspan = attributes.get("colspan");
        String rowspan = attributes.get("rowspan");
        String style   = attributes.get("style");   /*  for style="white-space:nowrap" */

        return encloseContents("td", new String[] {"align", align, "valign", valign, "bgcolor", bgcolor, "colspan", colspan, "rowspan", rowspan, "style", style}, contents);
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

            return getImageConverter().getImageByFile(attributes, path, fileref, outImages, imageDir, getBaseImagePath());
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
     * Handle an textobject (as an alternative to imageobject)
     * @param attributes the tag attributes
     * @param contents the tag contents
     * @return the HTML code
     * @throws SAXEception if an error is encountered
     */
    public String handleTextobject(final Map<String, String> attributes, final String contents) throws SAXException {
        return "<!-- " + contents + " -->";
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
        String align = attributes.get("align");
        String valign = attributes.get("valign");
        String style   = attributes.get("style");   /*  for style="white-space:nowrap" */

        return encloseContents("th", new String[] {"align", align, "valign", valign, "style", style}, contents);
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
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabNote.png\"/></td><td valign=\"top\">" + encloseContents("div", "note", contents) + "</td></tr></table>";
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
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabWarning.png\"/></td><td valign=\"top\">" + encloseContents("div", "warning", contents) + "</td></tr></table>";
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
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabCaution.png\"/></td><td valign=\"top\">" + encloseContents("div", "caution", contents) + "</td></tr></table>";
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
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabTip.png\"/></td><td valign=\"top\">" + encloseContents("div", "tip", contents) + "</td></tr></table>";
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
        String code = "<table><tr><td valign=\"top\"><img src=\"" + getBaseImagePath() + "ScilabImportant.png\"/></td><td valign=\"top\">" + encloseContents("div", "important", contents) + "</td></tr></table>";
        if (id != null) {
            return "<a name=\"" + id + "\"></a>" + code;
        } else {
            return code;
        }
    }

    private static final String convertCode(String code) {
        if (code == null || code.length() == 0) {
            return "";
        }

        StringBuffer buffer = new StringBuffer(2 * code.length());
        int start = 0;
        int end = code.length() - 1;
        char c = code.charAt(0);

        // first we trim
        while (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (start < end) {
                c = code.charAt(++start);
            } else {
                break;
            }
        }
        c = code.charAt(end);
        while (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (end > 0) {
                c = code.charAt(--end);
            } else {
                break;
            }
        }

        // replace chars by their html entities equivalent
        for (int j = start; j <= end; j++) {
            c = code.charAt(j);
            switch (c) {
                case '&':
                    buffer.append("&amp;");
                    break;
                case '<':
                    buffer.append("&lt;");
                    break;
                case '>':
                    buffer.append("&gt;");
                    break;
                case '\n':
                    buffer.append("<br />");
                    break;
                case '\'':
                    buffer.append("&#039;");
                    break;
                case '\"':
                    buffer.append("&quot;");
                    break;
                default:
                    buffer.append(c);
            }
        }

        return buffer.toString();
    }
}
