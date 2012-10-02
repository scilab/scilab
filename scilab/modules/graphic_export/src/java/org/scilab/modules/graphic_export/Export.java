/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.HashMap;
import java.util.Map;
import java.util.WeakHashMap;

import org.apache.batik.dom.GenericDOMImplementation;
import org.apache.batik.svggen.SVGGeneratorContext;
import org.apache.batik.svggen.SVGGraphics2D;
import org.apache.fop.Version;
import org.apache.fop.svg.PDFDocumentGraphics2D;
import org.apache.xmlgraphics.java2d.GraphicContext;
import org.apache.xmlgraphics.java2d.ps.AbstractPSDocumentGraphics2D;
import org.apache.xmlgraphics.java2d.ps.EPSDocumentGraphics2D;
import org.apache.xmlgraphics.java2d.ps.PSDocumentGraphics2D;
import org.apache.xmlgraphics.ps.DSCConstants;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvas;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvasFactory;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;
import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.graphic_export.convertToPPM.PPMEncoder;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.w3c.dom.DOMImplementation;
import org.w3c.dom.Document;

/**
 * Main class to export
 * Dependancies are put in inner classes to avoid the deps loading.
 *
 * @author Calixte DENIZET
 */
public class Export {

    public static final int SUCCESS = 0;
    public static final int IOEXCEPTION_ERROR = 1;
    public static final int INVALID_FILE = 2;
    public static final int MEMORY_ERROR = 3;
    public static final int UNKNOWN_ERROR = 4;

    private static final float DEFAULT_JPEG_COMPRESSION = 0.95f;

    private static final String CLASSPATH_PDF_PS_EPS_EXPORT_NAME = "pdf_ps_eps_graphic_export";
    private static final String CLASSPATH_SVG_EXPORT_NAME = "svg_graphic_export";

    private static final Map<DrawerVisitor, Exporter> visitorsToExp = new WeakHashMap<DrawerVisitor, Exporter>();

    private static final Map<String, Integer> extToType = new HashMap<String, Integer>();
    static {
        extToType.put("bmp", 1);
        extToType.put("gif", 2);
        extToType.put("jpeg", 3);
        extToType.put("jpg", 3);
        extToType.put("png", 4);
        extToType.put("ppm", 5);
        extToType.put("eps", 6);
        extToType.put("pdf", 7);
        extToType.put("svg", 8);
        extToType.put("ps", 9);
        extToType.put("pos", 9);
    }

    private static boolean svgLoaded;
    private static boolean pdfLoaded;

    public enum TYPE { PNG, JPEG, GIF, BMP, PPM, SVG, PS, EPS, PDF }
    private static final TYPE[] types = new TYPE[] {TYPE.PNG, TYPE.BMP, TYPE.GIF, TYPE.JPEG, TYPE.PNG, TYPE.PPM, TYPE.EPS, TYPE.PDF, TYPE.SVG, TYPE.PS};

    /**
     * @param type the image type
     * @return true if bitmap image format
     */
    public static boolean isBitmapFormat(TYPE type) {
        return type == TYPE.PNG || type == TYPE.JPEG || type == TYPE.GIF || type == TYPE.BMP || type == TYPE.PPM;
    }

    public static int getType(String ext) {
        Integer type = extToType.get(ext.toLowerCase());
        if (type == null) {
            return -1;
        }

        return type;
    }

    /**
     * Export in drawing in a Graphics2D
     * @param uid the figure uid
     * @param type the export type
     * @param fileName the file name
     * @param params the export paramaters
     * @return the export status
     */
    public static int export(String uid, int type, String fileName, ExportParams params, boolean headless) {
        // Check that the fileName contains an extension
        int dotPosition = fileName.lastIndexOf('.'); // position of the dot
        boolean extensionFound = false;
        if (dotPosition > 0 && dotPosition <= fileName.length() - 2) {
            extensionFound = true;
        }

        String extendedFilename = fileName;
        if (!extensionFound) { // Add default extension if no one found
            String[] extensions = {"png", "bmp", "gif", "jpeg", "png", "ppm", "eps", "pdf", "svg", "ps"};
            extendedFilename = fileName + "." + extensions[type];
        }

        DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
        if (visitor != null) {
            Canvas canvas = visitor.getCanvas();
            if (canvas instanceof JoGLCanvas && isBitmapFormat(types[type])) {
                try {
                    return exportBitmap(uid, type, extendedFilename, true, params);
                } catch (OutOfMemoryError e) {
                    return MEMORY_ERROR;
                } catch (Throwable e) {
                    return UNKNOWN_ERROR;
                }
            }
        }

        return exportVectorial(uid, type, extendedFilename, params, headless);
    }


    /**
     * Export in drawing in a Graphics2D
     * @param uid the figure uid
     * @param type the export type
     * @param fileName the file name
     * @param params the export paramaters
     * @return the export status
     */
    public static int exportVectorial(String uid, int type, String fileName, ExportParams params, boolean headless) {
        if (fileName == null) {
            return INVALID_FILE;
        }

        File f = new File(fileName);
        int ret = Utils.checkWritePermission(f);
        if (ret != SUCCESS) {
            return ret;
        }

        try {
            return exportVectorial(uid, types[type], f, params, headless);
        } catch (IOException e) {
            return IOEXCEPTION_ERROR;
        }
    }

    /**
     * Export in drawing in a Graphics2D
     * @param uid the figure uid
     * @param type the export type
     * @param file the file where to export
     * @param params the export paramaters
     */
    public static int exportVectorial(String uid, TYPE type, File file, ExportParams params, boolean headless) throws IOException {
        Figure figure = (Figure) GraphicController.getController().getObjectFromId(uid);

        if (!headless) {
            Exporter exporter = getExporter(type);
            Integer[] dims = figure.getAxesSize();
            int width = dims[0];
            int height = dims[1];

            Graphics2D g2d = exporter.getGraphics2D(width, height, file, params);
            params.setParamsOnGraphics(g2d);

            Canvas canvas = G2DCanvasFactory.createCanvas(g2d, width, height);
            DrawerVisitor oldVisitor = DrawerVisitor.getVisitor(uid);
            DrawerVisitor visitor = new DrawerVisitor(null, canvas, figure) {
                    @Override
                    public void updateObject(String id, int property) {
                        // Don't update during the export
                    }
                };

            try {
                canvas.setMainDrawer(visitor);
                canvas.redraw();
                exporter.write();
            } catch (OutOfMemoryError e) {
                return MEMORY_ERROR;
            } catch (Throwable e) {
                return UNKNOWN_ERROR;
            } finally {
                GraphicController.getController().unregister(visitor);
                DrawerVisitor.changeVisitor(figure, oldVisitor);
                exporter.dispose();
            }
        } else {
            DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
            G2DCanvas canvas = (G2DCanvas) visitor.getCanvas();
            canvas.enableDraw();
            Exporter exporter = null;
            try {
                canvas.redraw();
                exporter = visitorsToExp.get(visitor);
                if (exporter != null) {
                    exporter.file = file;
                    exporter.write();
                }
            } catch (OutOfMemoryError e) {
                return MEMORY_ERROR;
            } catch (Throwable e) {
                return UNKNOWN_ERROR;
            } finally {
                if (exporter != null) {
                    exporter.dispose();
                    visitorsToExp.remove(visitor);
                }
                DrawerVisitor.changeVisitor(figure, null);
            }
        }

        return SUCCESS;
    }

    /**
     * Export in getting a buffered image from JoGL
     * @param uid the figure uid
     * @param type the export type
     * @param fileName the file name
     * @param fromScreen if true, then use the screen view
     * @param params the export paramaters
     * @return the export status
     */
    public static int exportBitmap(String uid, int type, String fileName, boolean fromScreen, ExportParams params) {
        if (fileName == null) {
            return INVALID_FILE;
        }

        File f = new File(fileName);
        int ret = Utils.checkWritePermission(f);
        if (ret != SUCCESS) {
            return ret;
        }

        try {
            exportBitmap(uid, types[type], f, fromScreen, params);
        } catch (IOException e) {
            return IOEXCEPTION_ERROR;
        }

        return SUCCESS;
    }

    /**
     * Export in getting a buffered image from JoGL
     * @param uid the figure uid
     * @param type the export type
     * @param file the file where to export
     * @param fromScreen if true, then use the screen view
     * @param params the export paramaters
     */
    public static void exportBitmap(String uid, TYPE type, File file, boolean fromScreen, ExportParams params) throws IOException {
        if (isBitmapFormat(type)) {
            JoGLCanvas joglCanvas = null;
            if (fromScreen) {
                DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
                Canvas canvas = visitor.getCanvas();
                if (canvas instanceof JoGLCanvas) {
                    joglCanvas = (JoGLCanvas) canvas;
                }
            } else {
                Figure figure = (Figure) GraphicController.getController().getObjectFromId(uid);
                Integer[] dims = figure.getAxesSize();
                DrawerVisitor oldVisitor = DrawerVisitor.getVisitor(uid);
                joglCanvas = (JoGLCanvas) JoGLCanvasFactory.createCanvas(dims[0], dims[1]);
                DrawerVisitor visitor = new DrawerVisitor(null, joglCanvas, figure) {
                        @Override
                        public void updateObject(String id, int property) {
                            // Don't update during the export
                        }

                        @Override
                        public void deleteObject(String id) {
                            // Don't delete during the export
                        }
                    };
                joglCanvas.setMainDrawer(visitor);
                joglCanvas.redraw();
                GraphicController.getController().unregister(visitor);
                DrawerVisitor.changeVisitor(figure, oldVisitor);
            }

            if (joglCanvas != null) {
                BufferedImage image = joglCanvas.getImage();
                //joglCanvas.destroy();
                PNGExporter exporter = (PNGExporter) getExporter(type);
                exporter.setImage(file, image, params);
                exporter.write();
            }
        }
    }

    /**
     * Export in drawing in a Graphics2D
     * @param uid the figure uid
     * @param type the export type
     * @param file the file where to export
     * @param params the export paramaters
     */
    public static void setVisitor(String uid, int type, ExportParams params) {
        Exporter exporter = getExporter(types[type]);
        Figure figure = (Figure) GraphicController.getController().getObjectFromId(uid);
        Integer[] dims = figure.getAxesSize();
        int width = dims[0];
        int height = dims[1];

        Graphics2D g2d = exporter.getGraphics2D(width, height, null, params);
        params.setParamsOnGraphics(g2d);

        G2DCanvas canvas = G2DCanvasFactory.createCanvas(g2d, width, height);
        canvas.disableDraw();
        DrawerVisitor visitor = new DrawerVisitor(null, canvas, figure) {
                @Override
                public void deleteObject(String id) {
                    // Don't delete during the export
                }
            };
        visitor.setDrawingTools(canvas.getDrawingTools());
        canvas.setMainDrawer(visitor);
        visitorsToExp.put(visitor, exporter);
    }

    /**
     * Get an exporter from a type
     * @param type the exporter type.
     * @return the corresponding exporter/
     */
    private static Exporter getExporter(TYPE type) {
        switch (type) {
        case PNG :
            return new PNGExporter();
        case GIF :
            return new GIFExporter();
        case JPEG :
            return new JPEGExporter();
        case BMP :
            return new BMPExporter();
        case PPM :
            return new PPMExporter();
        case SVG :
            if (!svgLoaded) {
                ScilabCommonsUtils.loadOnUse(CLASSPATH_SVG_EXPORT_NAME);
                svgLoaded = true;
            }
            return new SVGExporter();
        case PDF :
            loadPDF();
            return new PDFExporter();
        case PS :
            loadPDF();
            return new PSExporter();
        case EPS :
            loadPDF();
            return new EPSExporter();
        default :
            break;
        }

        return null;
    }

    /**
     * Load the PDF/PS/EPS dependencies
     */
    private static final void loadPDF() {
        if (!pdfLoaded) {
            ScilabCommonsUtils.loadOnUse(CLASSPATH_PDF_PS_EPS_EXPORT_NAME);
            pdfLoaded = true;
        }
    }

    /**
     * Interface to export
     */
    private static abstract class Exporter {

        protected File file;

        /**
         * @param width graphics width
         * @param height graphisc height
         * @param file the file
         * @param params the export parameters
         */
        abstract Graphics2D getGraphics2D(int width, int height, File file, ExportParams params);

        /**
         * Write the file
         */
        abstract void write() throws IOException;

        abstract void dispose();
    }

    /**
     * PNG Exporter
     */
    private static class PNGExporter extends Exporter {

        protected BufferedImage image;
        protected Graphics2D g2d;
        protected ExportParams params;

        public PNGExporter() { }

        public void setImage(File file, BufferedImage image, ExportParams params) {
            this.file = file;
            this.image = image;
            this.params = params;
        }

        @Override
        public Graphics2D getGraphics2D(int width, int height, File file, ExportParams params) {
            this.file = file;
            this.params = params;
            image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
            g2d = image.createGraphics();

            return g2d;
        }

        @Override
        public void write() throws IOException {
            ExportBitmap.writeFile(image, "png", file);
        }

        @Override
        public void dispose() {
            g2d.dispose();
        }
    }

    /**
     * GIF Exporter
     */
    private static class GIFExporter extends PNGExporter {

        public GIFExporter() { }

        @Override
        public void write() throws IOException {
            ExportBitmap.writeFile(image, "gif", file);
        }
    }

    /**
     * BMP Exporter
     */
    private static class BMPExporter extends PNGExporter {

        public BMPExporter() { }

        @Override
        public Graphics2D getGraphics2D(int width, int height, File file, ExportParams params) {
            this.file = file;
            this.params = params;
            image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
            g2d = image.createGraphics();

            return g2d;
        }

        @Override
        public void write() throws IOException {
            ExportBitmap.writeFile(image, "bmp", file);
        }
    }

    /**
     * JPEG Exporter
     */
    private static class JPEGExporter extends BMPExporter {

        public JPEGExporter() { }

        @Override
        public void write() throws IOException {
            if (params.compressionQuality == -1) {
                ExportBitmap.writeJPEG(image, DEFAULT_JPEG_COMPRESSION, file);
            } else {
                ExportBitmap.writeJPEG(image, params.compressionQuality, file);
            }
        }
    }

    /**
     * PPM Exporter
     */
    private static class PPMExporter extends BMPExporter {

        public PPMExporter() { }

        @Override
        public void write() throws IOException {
            OutputStream out = new BufferedOutputStream(new FileOutputStream(file));
            PPMEncoder encoder = new PPMEncoder(image, out);
            int[] pixels = image.getRGB(0, 0, image.getWidth(), image.getHeight(), null, 0, image.getWidth());
            encoder.encodeStart(image.getWidth(), image.getHeight());
            encoder.encodePixels(0, 0, image.getWidth(), image.getHeight(), pixels, 0, image.getWidth());
            out.flush();
            out.close();
        }
    }

    /**
     * SVG Exporter
     */
    private static class SVGExporter extends Exporter {

        private SVGGraphics2D g2d;
        private ExportParams params;

        public SVGExporter() { }

        @Override
        public Graphics2D getGraphics2D(int width, int height, File file, ExportParams params) {
            this.file = file;
            this.params = params;
            DOMImplementation domImpl = GenericDOMImplementation.getDOMImplementation();
            Document document = domImpl.createDocument("http://www.w3.org/2000/svg", "svg", null);
            SVGGeneratorContext ctx = SVGGeneratorContext.createDefault(document);
            ctx.setComment("Generated by Scilab with Batik SVG Generator");
            // TODO: better handle of LaTeX fonts (should remove the 'true' below and include the font in the SVG)
            // same thing for PDF & co...
            ctx.setEmbeddedFontsOn(true);
            g2d = new SVGGraphics2D(ctx, true);
            if (params.orientation == ExportParams.LANDSCAPE) {
                g2d.setSVGCanvasSize(new Dimension(height, width));
                AffineTransform transf = AffineTransform.getRotateInstance(Math.PI / 2);
                transf.preConcatenate(AffineTransform.getTranslateInstance(height, 0));
                g2d.setTransform(transf);
            } else {
                g2d.setSVGCanvasSize(new Dimension(width, height));
            }
            return g2d;
        }

        @Override
        public void write() throws IOException {
            boolean useCSS = true;
            OutputStream svgs = new BufferedOutputStream(new FileOutputStream(file));
            Writer out = new OutputStreamWriter(svgs, "UTF-8");
            g2d.stream(out, useCSS);
            svgs.flush();
            svgs.close();
        }

        @Override
        public void dispose() {
            g2d.dispose();
        }
    }

    /**
     * PDF Exporter
     */
    private static class PDFExporter extends Exporter {

        private OutputStream out;
        private PDFDocumentGraphics2D g2d;
        private ExportParams params;
        private ByteArrayOutputStream buffer;

        public PDFExporter() { }

        @Override
        public Graphics2D getGraphics2D(int width, int height, File file, ExportParams params) {
            this.file = file;
            this.params = params;
            try {
                if (file == null) {
                    buffer = new ByteArrayOutputStream();
                    out = new BufferedOutputStream(buffer);
                } else {
                    out = new BufferedOutputStream(new FileOutputStream(file));
                }
                g2d = new PDFDocumentGraphics2D(true);
                g2d.setupDefaultFontInfo();
                g2d.getPDFDocument().getInfo().setProducer("Generated by Scilab with Apache FOP Version " + Version.getVersion());
                g2d.setGraphicContext(new GraphicContext());
                if (params.orientation == ExportParams.LANDSCAPE) {
                    g2d.setupDocument(out, height, width);
                    g2d.setSVGDimension(height, width);
                    double s = PDFDocumentGraphics2D.NORMAL_PDF_RESOLUTION / g2d.getDeviceDPI();
                    AffineTransform transf = AffineTransform.getRotateInstance(Math.PI / 2);
                    transf.preConcatenate(AffineTransform.getTranslateInstance(height / s, 0));
                    g2d.setTransform(transf);
                } else {
                    g2d.setupDocument(out, width, height);
                    g2d.setSVGDimension(width, height);
                }
            } catch (IOException e) { }

            return g2d;
        }

        @Override
        public void write() throws IOException {
            if (g2d != null) {
                g2d.finish();
            }
            if (buffer != null && file != null) {
                FileOutputStream fos = new FileOutputStream(file);
                buffer.writeTo(fos);
                buffer.close();
                fos.flush();
                fos.close();
            }
            if (out != null) {
                out.close();
            }
        }

        @Override
        public void dispose() {
            g2d.dispose();
        }
    }

    /**
     * PS Exporter
     */
    private static class PSExporter extends Exporter {

        protected OutputStream out;
        protected AbstractPSDocumentGraphics2D g2d;
        protected ExportParams params;
        protected ByteArrayOutputStream buffer;

        public PSExporter() { }

        @Override
        public Graphics2D getGraphics2D(int width, int height, File file, final ExportParams params) {
            this.file = file;
            this.params = params;
            try {
                if (file == null) {
                    buffer = new ByteArrayOutputStream();
                    out = new BufferedOutputStream(buffer);
                } else {
                    out = new BufferedOutputStream(new FileOutputStream(file));
                }
                g2d = new PSDocumentGraphics2D(true, out, width, height) {
                        @Override
                        protected void writePageHeader() throws IOException {
                            super.writePageHeader();
                            if (params.orientation == ExportParams.LANDSCAPE) {
                                gen.writeDSCComment(DSCConstants.PAGE_ORIENTATION, "Landscape");
                            } else {
                                gen.writeDSCComment(DSCConstants.PAGE_ORIENTATION, "Portrait");
                            }
                        }
                    };
                g2d.setGraphicContext(new GraphicContext());
            } catch (IOException e) { }

            return g2d;
        }

        @Override
        public void write() throws IOException {
            if (g2d != null) {
                g2d.finish();
            }
            if (buffer != null && file != null) {
                FileOutputStream fos = new FileOutputStream(file);
                buffer.writeTo(fos);
                buffer.close();
                fos.flush();
                fos.close();
            }
            if (out != null) {
                out.close();
            }
        }

        @Override
        public void dispose() {
            g2d.dispose();
        }
    }

    /**
     * EPS Exporter
     */
    private static class EPSExporter extends PSExporter {

        public EPSExporter() { }

        @Override
        public Graphics2D getGraphics2D(int width, int height, File file, final ExportParams params) {
            this.file = file;
            this.params = params;
            try {
                if (file == null) {
                    buffer = new ByteArrayOutputStream();
                    out = new BufferedOutputStream(buffer);
                } else {
                    out = new BufferedOutputStream(new FileOutputStream(file));
                }
                g2d = new EPSDocumentGraphics2D(true) {
                        @Override
                        protected void writePageHeader() throws IOException {
                            super.writePageHeader();
                            if (params.orientation == ExportParams.LANDSCAPE) {
                                gen.writeDSCComment(DSCConstants.PAGE_ORIENTATION, "Landscape");
                            } else {
                                gen.writeDSCComment(DSCConstants.PAGE_ORIENTATION, "Portrait");
                            }
                        }
                    };
                g2d.setupDocument(out, width, height);
                g2d.setGraphicContext(new GraphicContext());
            } catch (IOException e) { }

            return g2d;
        }
    }
}
