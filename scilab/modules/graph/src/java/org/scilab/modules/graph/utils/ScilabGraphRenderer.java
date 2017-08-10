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

import java.awt.Color;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.io.Writer;

import org.apache.batik.dom.GenericDOMImplementation;
import org.apache.batik.svggen.CachedImageHandlerBase64Encoder;
import org.apache.batik.svggen.GenericImageHandler;
import org.apache.batik.svggen.SVGGeneratorContext;
import org.apache.batik.svggen.SVGGraphics2D;
import org.apache.batik.svggen.SVGGraphics2DIOException;
import org.scilab.modules.graph.ScilabCanvas;
import org.w3c.dom.DOMImplementation;
import org.w3c.dom.Document;

import com.mxgraph.canvas.mxGraphics2DCanvas;
import com.mxgraph.canvas.mxICanvas;
import com.mxgraph.util.mxCellRenderer;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxCellRenderer.CanvasFactory;
import com.mxgraph.view.mxGraph;

/**
 * This class implement Export operation by rendering graphs.
 *
 * Implement custom rendering as {@link mxCellRenderer} does.
 */
public class ScilabGraphRenderer {

    /**
     * This class is a static singleton
     */
    private ScilabGraphRenderer() {
    }

    /**
     * Create an SVG Document containing a rendered graph.
     *
     * Use the Batik svg-generator engine to perform rendering.
     *
     * @param graph
     *            The graph to render
     * @param cells
     *            the cells to render
     * @param scale
     *            the scale
     * @param background
     *            the current background
     * @param clip
     *            the current clip
     * @param filename
     *            the file where to stream.
     */
    public static void createSvgDocument(mxGraph graph, Object[] cells,
                                         double scale, Color background, mxRectangle clip, String filename) {

        // Get a DOMImplementation.
        DOMImplementation domImpl = GenericDOMImplementation
                                    .getDOMImplementation();

        // Create an instance of org.w3c.dom.Document.
        String svgNS = "http://www.w3.org/2000/svg";
        Document doc = domImpl.createDocument(svgNS, "svg", null);

        // Adding comments and customs
        SVGGeneratorContext ctx = SVGGeneratorContext.createDefault(doc);
        ctx.setComment("Generated with Batik SVG Generator");
        ctx.setEmbeddedFontsOn(true);
        GenericImageHandler ihandler = new CachedImageHandlerBase64Encoder();
        ctx.setGenericImageHandler(ihandler);

        // Create an instance of the SVG Generator.
        final SVGGraphics2D svgGenerator = new SVGGraphics2D(ctx, true);

        // Render the graph using the SVGGraphics2D object
        mxCellRenderer.drawCells(graph, cells, scale, clip,
        new CanvasFactory() {
            private mxGraphics2DCanvas canvas;

            /**
             * Create a new canvas for drawing
             *
             * @param width
             *            the canvas width
             * @param height
             *            the canvas height
             * @return a new canvas used for drawing
             */
            @Override
            public mxICanvas createCanvas(int width, int height) {
                if (canvas == null) {
                    canvas = new ScilabCanvas();
                    canvas.setGraphics(svgGenerator);
                }

                return canvas;
            }
        });

        // Finally, stream out SVG to the standard output using
        // UTF-8 encoding.
        boolean useCSS = true; // we want to use CSS style attributes
        Writer out;
        try {
            out = new OutputStreamWriter(new FileOutputStream(filename), "UTF-8");
            svgGenerator.stream(out, useCSS);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (SVGGraphics2DIOException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
