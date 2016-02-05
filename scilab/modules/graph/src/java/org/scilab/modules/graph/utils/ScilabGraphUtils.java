/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph.utils;

import java.awt.geom.Dimension2D;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.WeakHashMap;
import java.util.logging.Logger;

import javax.swing.Icon;

import org.apache.batik.bridge.BridgeContext;
import org.apache.batik.bridge.DocumentLoader;
import org.apache.batik.bridge.GVTBuilder;
import org.apache.batik.bridge.UserAgent;
import org.apache.batik.bridge.UserAgentAdapter;
import org.apache.batik.anim.dom.SAXSVGDocumentFactory;
import org.apache.batik.gvt.GraphicsNode;
import org.apache.batik.util.XMLResourceDescriptor;
import org.scilab.forge.jlatexmath.ParseException;
import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.modules.graph.view.SupportedLabelType;
import org.w3c.dom.Document;

import com.mxgraph.util.mxUtils;

/**
 * Utilities functions for ScilabGraph
 */
public final class ScilabGraphUtils extends mxUtils {
    /**
     * Cache for the generated SVG components
     */
    private static Map<URL, WeakReference<GraphicsNode>> generatedSVGComponents = new HashMap<URL, WeakReference<GraphicsNode>>();
    /**
     * Cache for the generated SVG document sizes
     */
    private static Map<URL, Dimension2D> generatedSVGSizes = new HashMap<URL, Dimension2D>();

    /**
     * Cache for the generated latex icons
     */
    private static Map<Float, Map<String, TeXIcon>> generatedLatexIcons = new WeakHashMap<Float, Map<String, TeXIcon>>();

    /**
     * Table conversion between escaped/unescaped HTML symbols
     */
    private static final String[][] HTML_ESCAPE_TABLE = { { "&lt;", "<" }, { "&gt;", ">" }, { "&amp;", "&" }, { "&quot;", "\"" }, { "&agrave;", "\u00e0" },
        { "&Agrave;", "\u00c0" }, { "&acirc;", "\u00e2" }, { "&auml;", "\u00e4" }, { "&Auml;", "\u00c4" }, { "&Acirc;", "\u00c2" },
        { "&aring;", "\u00e5" }, { "&Aring;", "\u00c5" }, { "&aelig;", "\u00e6" }, { "&AElig;", "\u00c6" }, { "&ccedil;", "\u00e7" },
        { "&Ccedil;", "\u00c7" }, { "&eacute;", "\u00e9" }, { "&Eacute;", "\u00c9" }, { "&egrave;", "\u00e8" }, { "&Egrave;", "\u00c8" },
        { "&ecirc;", "\u00ea" }, { "&Ecirc;", "\u00ca" }, { "&euml;", "\u00eb" }, { "&Euml;", "\u00cb" }, { "&iuml;", "\u00ef" }, { "&Iuml;", "\u00cf" },
        { "&ocirc;", "\u00f4" }, { "&Ocirc;", "\u00d4" }, { "&ouml;", "\u00f6" }, { "&Ouml;", "\u00d6" }, { "&oslash;", "\u00f8" },
        { "&Oslash;", "\u00d8" }, { "&szlig;", "\u00df" }, { "&ugrave;", "\u00f9" }, { "&Ugrave;", "\u00d9" }, { "&ucirc;", "\u00fb" },
        { "&Ucirc;", "\u00db" }, { "&uuml;", "\u00fc" }, { "&Uuml;", "\u00dc" }, { "&nbsp;", " " }, { "&reg;", "\u00a9" }, { "&copy;", "\u00ae" },
        { "&euro;", "\u20a0" }
    };

    /**
     * Return a cached or a new instance of a {@link GraphicsNode} generated
     * from the SVG file.
     *
     * @param filename
     *            the file to parse
     * @return the corresponding graphic node
     */
    public static GraphicsNode getSVGComponent(URL filename) {
        WeakReference<GraphicsNode> nodeRef;
        GraphicsNode node;

        nodeRef = generatedSVGComponents.get(filename);
        if (nodeRef != null) {
            node = nodeRef.get();
        } else {
            node = null;
        }

        if (node == null) {
            try {
                String xmlParser = XMLResourceDescriptor.getXMLParserClassName();
                SAXSVGDocumentFactory df = new SAXSVGDocumentFactory(xmlParser);
                Document doc = df.createDocument(filename.toString());
                UserAgent userAgent = new UserAgentAdapter();
                DocumentLoader loader = new DocumentLoader(userAgent);
                BridgeContext ctx = new BridgeContext(userAgent, loader);
                ctx.setDynamicState(BridgeContext.STATIC);
                GVTBuilder builder = new GVTBuilder();

                node = builder.build(ctx, doc);

                generatedSVGComponents.put(filename, node.getWeakReference());
                generatedSVGSizes.put(filename, ctx.getDocumentSize());
            } catch (IOException e) {
                Logger.getLogger(ScilabGraphUtils.class.getName()).severe(e.toString());
            }
        }
        return node;
    }

    /**
     * Get the document size for a given URL.
     *
     * This method use the Document size cache to get the svg element dimension
     * and not the real size of the graphical tree.
     *
     * @param filename
     *            the file
     * @return the dimension of the file
     */
    public static Dimension2D getSVGDocumentSizes(URL filename) {
        Dimension2D ret = generatedSVGSizes.get(filename);

        // Generate the GraphicsNode if not available
        if (ret == null) {
            getSVGComponent(filename);
            ret = generatedSVGSizes.get(filename);
        }
        return ret;
    }

    /**
     * Return a cached or a new instance of a TexIcon generated from the text.
     *
     * @param text
     *            the latex formula
     * @return the TeXIcon
     * @throws ParseException
     *             when the text is not a valid formula
     */
    public static Icon getTexIcon(String text, float size) throws ParseException {
        TeXIcon icon;
        String escapedText = SupportedLabelType.Latex.escape(text);

        Map<String, TeXIcon> iconMap = generatedLatexIcons.get(size);
        if (iconMap == null) {
            iconMap = new WeakHashMap<String, TeXIcon>();
            generatedLatexIcons.put(size, iconMap);
        }

        icon = iconMap.get(escapedText);
        if (icon == null) {
            TeXFormula tex = new TeXFormula(escapedText);
            icon = tex.createTeXIcon(TeXConstants.STYLE_DISPLAY, size);
            iconMap.put(escapedText, icon);
        }
        return icon;
    }

    /**
     * Remove the blank char on the beginning of the sequence. This method
     * modify the {@link StringBuilder} in place.
     *
     * @param seq
     *            the sequence
     */
    public static void removeBlanks(StringBuilder seq) {
        int i;
        for (i = 0; i < seq.length(); i++) {
            char car = seq.charAt(i);
            if (car != ' ' && car != '\n') {
                break;
            }
        }

        seq.delete(0, i);
    }

    /**
     * Unescape html
     *
     * This method modify the {@link StringBuilder} in place.
     *
     * @param escapedText
     *            the working text
     * @param index
     *            the beginning index
     */
    public static void unescape(StringBuilder escapedText, int index) {
        int start, end, table_index;

        start = escapedText.indexOf("&", index);
        if (start > -1) {
            end = escapedText.indexOf(";", start);
            // we don't start from the beginning
            // the next time, to handle the case of
            // the &
            index = start + 1;

            if (end > start) {
                String temp = escapedText.substring(start, end + 1);
                // search in HTML_ESCAPE_TABLE[][] if temp is there
                table_index = 0;
                while (table_index < HTML_ESCAPE_TABLE.length) {
                    if (HTML_ESCAPE_TABLE[table_index][0].equals(temp)) {
                        break;
                    } else {
                        table_index++;
                    }
                }
                if (table_index < HTML_ESCAPE_TABLE.length) {
                    escapedText.replace(start, end + 1, HTML_ESCAPE_TABLE[table_index][1]);
                    unescape(escapedText, index); // recursive call
                }
            }
        }
        return;
    }

    /**
     * This class is a static singleton thus it must not be instantiated
     */
    private ScilabGraphUtils() {
    }
}
