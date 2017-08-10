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
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.charset.Charset;
import java.util.Iterator;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.helptools.image.ImageConverter;
import org.xml.sax.SAXException;

/**
 * Class to convert DocBook to JavaHelp
 * @author Calixte DENIZET
 */
public class JavaHelpDocbookTagConverter extends HTMLDocbookTagConverter {

    private static final String XMLSTRING = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n";

    private StringBuilder buffer = new StringBuilder(8192);

    /**
     * Constructor
     * @param inName the name of the input stream
     * @param sciDocMain provide useful doc generation properties
     * @param imgConvert the shared image converter for all generation
     */
    public JavaHelpDocbookTagConverter(String inName, SciDocMain sciDocMain, ImageConverter imgConvert) throws IOException, SAXException {
        super(inName, sciDocMain, imgConvert);

        urlBase = "scilab://";
        linkToTheWeb = false;

        if (!isToolbox) {
            this.outImages = ScilabConstants.SCI.getPath() + "/modules/helptools/images";
            File dir = new File(this.outImages);
            if (!dir.exists()) {
                dir.mkdir();
            }
        }

        prependToProgramListing = "<table border=\"0\" width=\"100%\"><tr><td width=\"98%\">";
        appendToProgramListing = "</td><td valign=\"top\"><a href=\"scilab://scilab.execexample/\"><img src=\"" + getBaseImagePath() + "ScilabExecute.png\" border=\"0\"/></a></td><td valign=\"top\"><a href=\"scilab://scilab.editexample/\"><img src=\"" + getBaseImagePath() + "ScilabEdit.png\" border=\"0\"/></a></td><td></td></tr></table>";
        appendForExecToProgramListing = "</td><td valign=\"top\"><a href=\"scilab://scilab.execexample/\"><img src=\"" + getBaseImagePath() + "ScilabExecute.png\" border=\"0\"/></a></td><td></td></tr></table>";
    }

    @Override
    public void install() {
        super.install();

        ScilabCommonsUtils.copyFile(new File(FindIconHelper.findIcon("media-playback-start")), new File(outImages + "/ScilabExecute.png"));
        ScilabCommonsUtils.copyFile(new File(FindIconHelper.findIcon("accessories-text-editor")), new File(outImages + "/ScilabEdit.png"));
        ScilabCommonsUtils.copyFile(new File(FindIconHelper.findIcon("dialog-information")), new File(outImages + "/ScilabNote.png"));
        ScilabCommonsUtils.copyFile(new File(FindIconHelper.findIcon("dialog-warning")), new File(outImages + "/ScilabWarning.png"));
        ScilabCommonsUtils.copyFile(new File(FindIconHelper.findIcon("dialog-warning")), new File(outImages + "/ScilabCaution.png"));
        ScilabCommonsUtils.copyFile(new File(FindIconHelper.findIcon("dialog-information")), new File(outImages + "/ScilabTip.png"));
        ScilabCommonsUtils.copyFile(new File(FindIconHelper.findIcon("emblem-important")), new File(outImages + "/ScilabImportant.png"));
        if (!isToolbox) {
            ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/pages/error.html"), new File(outName + "/ScilabErrorPage.html"));
            File homepage = new File(SCI + "/modules/helptools/data/pages/homepage-" + language + ".html");
            if (!homepage.isFile()) {
                /* could not find the localized homepage. Switch to english */
                homepage = new File(SCI + "/modules/helptools/data/pages/homepage-en_US.html");
            }
            ScilabCommonsUtils.copyFile(homepage, new File(outName + "/ScilabHomePage.html"));

            File changes = new File(SCI + "/modules/helptools/data/pages/CHANGES.html");
            if (changes.isFile()) {
                ScilabCommonsUtils.copyFile(changes, new File(outName + "/CHANGES.html"));
            }

            File homepageImage = new File(SCI + "/modules/helptools/data/pages/ban-" + language + ".png");
            if (!homepageImage.isFile()) {
                homepageImage = new File(SCI + "/modules/helptools/data/pages/ban-en_US.png");
            }
            ScilabCommonsUtils.copyFile(homepageImage, new File(outImages + "/ban_en_US.png"));
        }
    }

    /**
     * {@inheritDoc}
     */
    public void endDocument() throws SAXException {
        try {
            FileOutputStream outToc = new FileOutputStream("jhelptoc.xml");
            FileOutputStream outMap = new FileOutputStream("jhelpmap.jhm");
            FileOutputStream outSet = new FileOutputStream("jhelpset.hs");
            FileOutputStream outIndex = new FileOutputStream("jhelpidx.xml");
            OutputStreamWriter writerIndex = new OutputStreamWriter(outIndex, Charset.forName("UTF-8"));
            OutputStreamWriter writerSet = new OutputStreamWriter(outSet, Charset.forName("UTF-8"));
            OutputStreamWriter writerMap = new OutputStreamWriter(outMap, Charset.forName("UTF-8"));
            OutputStreamWriter writerToc = new OutputStreamWriter(outToc, Charset.forName("UTF-8"));
            writerMap.append(XMLSTRING);
            writerMap.append("<!DOCTYPE map PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp Map Version 1.0//EN\" \"http://java.sun.com/products/javahelp/map_1_0.dtd\">\n");
            writerMap.append(convertMapId());
            writerMap.flush();
            writerMap.close();
            outMap.flush();
            outMap.close();

            writerToc.append(XMLSTRING);
            writerToc.append("<!DOCTYPE toc PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp TOC Version 1.0//EN\" \"http://java.sun.com/products/javahelp/toc_1_0.dtd\">\n");
            writerToc.append(convertTocItem());
            writerToc.flush();
            writerToc.close();
            outToc.flush();
            outToc.close();

            writerSet.append(XMLSTRING);
            String str = "<!DOCTYPE helpset\n  PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp HelpSet Version 1.0//EN\" \"http://java.sun.com/products/javahelp/helpset_1_0.dtd\">\n<helpset version=\"1.0\">\n<title>TITLE</title>\n<maps>\n<homeID>top</homeID>\n<mapref location=\"jhelpmap.jhm\"/>\n</maps>\n<view>\n<name>TOC</name>\n<label>Table Of Contents</label>\n<type>javax.help.TOCView</type>\n<data>jhelptoc.xml</data>\n</view>\n<view>\n<name>Index</name>\n<label>Index</label>\n<type>javax.help.IndexView</type>\n<data>jhelpidx.xml</data>\n</view>\n<view>\n<name>Search</name>\n<label>Search</label>\n<type>javax.help.SearchView</type>\n<data engine=\"com.sun.java.help.search.DefaultSearchEngine\">JavaHelpSearch</data>\n</view>\n</helpset>".replaceFirst("TITLE", bookTitle);
            writerSet.append(str);
            writerSet.flush();
            writerSet.close();
            outSet.flush();
            outSet.close();

            writerIndex.append(XMLSTRING);
            writerIndex.append("<!DOCTYPE index PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp Index Version 1.0//EN\" \"http://java.sun.com/products/javahelp/index_1_0.dtd\">\n<index version=\"1.0\"/>");
            writerIndex.flush();
            writerIndex.close();
            outIndex.flush();
            outIndex.close();

            if (!isToolbox) {
                getImageConverter().saveMD5s(ScilabConstants.SCI.getPath() + "/modules/helptools/etc");
            }
        } catch (IOException e) {
            fatalExceptionOccurred(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    protected String makeRemoteLink(String link) {
        return "file://SCI/modules/" + link;
    }

    public String getBaseImagePath() {
        if (isToolbox) {
            return super.getBaseImagePath();
        }

        return "jarsci:";
    }

    private String convertMapId() {
        buffer.setLength(0);
        buffer.append("<map version=\"1.0\">\n<mapID target=\"index\" url=\"index.html\"/>\n");
        if (!isToolbox) {
            buffer.append("<mapID target=\"whatsnew\" url=\"ScilabHomePage.html\"/>\n");
        }
        Iterator<String> iter = mapId.keySet().iterator();
        while (iter.hasNext()) {
            String id = iter.next();
            buffer.append("<mapID target=\"");
            buffer.append(id);
            buffer.append("\" url=\"");
            buffer.append(mapId.get(id));
            buffer.append("\"/>\n");
        }
        buffer.append("</map>");

        return buffer.toString();
    }

    private void convertTreeId(HTMLDocbookLinkResolver.TreeId leaf) {
        if (leaf.children != null) {
            for (HTMLDocbookLinkResolver.TreeId c : leaf.children) {
                buffer.append("<tocitem target=\"");
                buffer.append(c.id);
                buffer.append("\" text=\"");
                buffer.append(tocitem.get(c.id));
                if (c.children == null) {
                    buffer.append("\"/>\n");
                } else {
                    buffer.append("\">\n");
                    convertTreeId(c);
                    buffer.append("</tocitem>\n");
                }
            }
        }
    }

    private String convertTocItem() {
        buffer.setLength(0);
        buffer.append("<toc version=\"1.0\">\n<tocitem target=\"index\" text=\"" + bookTitle + "\">\n");
        if (!isToolbox) {
            buffer.append("<tocitem target=\"whatsnew\" text=\"Scilab Home\"/>\n");
        }
        convertTreeId(tree);
        buffer.append("</tocitem>\n</toc>");

        return buffer.toString();
    }
}
