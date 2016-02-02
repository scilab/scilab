/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
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

package org.scilab.modules.graphic_export;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.geom.AffineTransform;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Path2D;
import java.awt.geom.PathIterator;
import java.awt.image.BufferedImage;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Type;
import java.nio.charset.Charset;
import java.nio.charset.CharsetEncoder;
import java.text.AttributedCharacterIterator;
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
import org.apache.xmlgraphics.ps.PSGenerator;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvas;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvasFactory;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;
import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.graphic_export.convertToPPM.PPMEncoder;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
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
    public static final int FILENOTFOUND_ERROR = 5;
    public static final int NOWRITER_ERROR = 5;

    private static final float DEFAULT_JPEG_COMPRESSION = 0.95f;

    private static final String CLASSPATH_PDF_PS_EPS_EXPORT_NAME = "pdf_ps_eps_graphic_export";
    private static final String CLASSPATH_SVG_EXPORT_NAME = "svg_graphic_export";
    private static final String CLASSPATH_EMF_EXPORT_NAME = "emf_graphic_export";

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
        extToType.put("emf", 10);
    }

    private static boolean emfLoaded;
    private static boolean svgLoaded;
    private static boolean pdfLoaded;

    public enum TYPE { PNG, JPEG, GIF, BMP, PPM, SVG, PS, EPS, PDF, EMF }
    private static final TYPE[] types = new TYPE[] {TYPE.PNG, TYPE.BMP, TYPE.GIF, TYPE.JPEG, TYPE.PNG, TYPE.PPM, TYPE.EPS, TYPE.PDF, TYPE.SVG, TYPE.PS, TYPE.EMF};

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
    public static int export(int uid, int type, String fileName, ExportParams params, boolean headless) {
        // Check that the fileName contains an extension
        int dotPosition = fileName.lastIndexOf('.'); // position of the dot
        boolean extensionFound = false;
        if (dotPosition > 0 && dotPosition <= fileName.length() - 2) {
            extensionFound = true;
        }

        String extendedFilename = fileName;
        if (!extensionFound) { // Add default extension if no one found
            String[] extensions = {"png", "bmp", "gif", "jpeg", "png", "ppm", "eps", "pdf", "svg", "ps", "emf"};
            extendedFilename = fileName + "." + extensions[type];
        }

        DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
        if (visitor != null) {
            Canvas canvas = visitor.getCanvas();
            if (canvas instanceof JoGLCanvas && isBitmapFormat(types[type]) && visitor.getFigure().getVisible()) {
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
    public static int exportVectorial(int uid, int type, String fileName, ExportParams params, boolean headless) {
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
            if (e instanceof FileNotFoundException) {
                return FILENOTFOUND_ERROR;
            }
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
    public static int exportVectorial(int uid, TYPE type, File file, ExportParams params, boolean headless) throws IOException {
        Figure figure = (Figure) GraphicController.getController().getObjectFromId(uid);

        if (!headless) {
            Exporter exporter = getExporter(type);
            Integer[] dims = figure.getAxesSize();
            int width = dims[0];
            int height = dims[1];

            Graphics2D g2d = exporter.getGraphics2D(width, height, file, params);
            if (g2d == null) {
                return FILENOTFOUND_ERROR;
            }

            params.setParamsOnGraphics(g2d);

            Canvas canvas = G2DCanvasFactory.createCanvas(g2d, width, height);
            DrawerVisitor oldVisitor = DrawerVisitor.getVisitor(uid);
            DrawerVisitor visitor = new DrawerVisitor(null, canvas, figure) {
                @Override
                public void updateObject(Integer id, int property) {
                    // Don't update during the export
                }

                @Override
                public void visit(Figure figure) {
                    // Fix for bug 13676: allow vectorial export even if the figure is invisible
                    synchronized (figure) {
                        super.visit(figure);
                        if (!figure.getVisible()) {
                            askAcceptVisitor(figure.getChildren());
                        }
                    }
                }
            };

            try {
                canvas.setMainDrawer(visitor);
                canvas.redraw();
                exporter.write();
            } catch (OutOfMemoryError e) {
                return MEMORY_ERROR;
            } catch (IOException e) {
                throw e;
            } catch (Throwable e) {
                return UNKNOWN_ERROR;
            } finally {
                GraphicController.getController().unregister(visitor);
                DrawerVisitor.changeVisitor(figure, oldVisitor);
                exporter.dispose();
                exporter = null;
                visitorsToExp.remove(visitor);
                canvas.destroy();
            }
        } else {
            DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
            if (visitor.getCanvas() instanceof G2DCanvas) {
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
                } catch (IOException e) {
                    throw e;
                } catch (Throwable e) {
                    return UNKNOWN_ERROR;
                } finally {
                    if (exporter != null) {
                        exporter.dispose();
                        exporter = null;
                        visitorsToExp.remove(visitor);
                    }
                    DrawerVisitor.changeVisitor(figure, null);
                    GraphicController.getController().unregister(visitor);
                    canvas.destroy();
                }
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
    public static int exportBitmap(int uid, int type, String fileName, boolean fromScreen, ExportParams params) {
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
            if (e instanceof FileNotFoundException) {
                return FILENOTFOUND_ERROR;
            }
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
    public static void exportBitmap(int uid, TYPE type, File file, boolean fromScreen, ExportParams params) throws IOException {
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
                    public void updateObject(Integer id, int property) {
                        // Don't update during the export
                    }

                    @Override
                    public void deleteObject(Integer id) {
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
                PNGExporter exporter = (PNGExporter) getExporter(type);
                exporter.setImage(file, image, params);
                int exportStatus = exporter.write();
                exporter.dispose();
                if (isBitmapFormat(type) && exportStatus == Export.NOWRITER_ERROR) {
                    // If export fails because no writer was found for bitmap format
                    // ==> Retry without Alpha channel in image
                    // Needed after JoGL 2.2.4 version
                    image = joglCanvas.getImage(false);
                    exporter = (PNGExporter) getExporter(type);
                    exporter.setImage(file, image, params);
                    exporter.write();
                    exporter.dispose();
                }

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
    public static void setVisitor(int uid, int type, final ExportParams params) {
        final Exporter exporter = getExporter(types[type]);
        Figure figure = (Figure) GraphicController.getController().getObjectFromId(uid);
        final Integer[] dims = figure.getAxesSize();
        int width = dims[0];
        int height = dims[1];

        final Graphics2D g2d = exporter.getGraphics2D(width, height, null, params);
        params.setParamsOnGraphics(g2d);

        final G2DCanvas canvas = G2DCanvasFactory.createCanvas(g2d, width, height);
        canvas.disableDraw();
        DrawerVisitor visitor = new DrawerVisitor(null, canvas, figure) {
            @Override
            public void deleteObject(Integer id) {
                // Don't delete during the export
            }

            @Override
            public void updateObject(Integer id, int property) {
                if (needUpdate(id, property)) {
                    axesDrawer.update(id, property);
                    if (property == GraphicObjectProperties.__GO_AXES_SIZE__) {
                        Integer[] size = getFigure().getAxesSize();
                        if (size[0] != dims[0] || size[1] != dims[1]) {
                            Graphics2D newg2d = exporter.getGraphics2D(size[0], size[1], null, params);
                            params.setParamsOnGraphics(newg2d);
                            canvas.setGraphics(newg2d, size[0], size[1]);
                            dims[0] = size[0];
                            dims[1] = size[1];

                            g2d.dispose();
                        }
                    } else if (property == GraphicObjectProperties.__GO_ANTIALIASING__) {
                        canvas.setAntiAliasingLevel(getFigure().getAntialiasing());
                    }
                }
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
            case EMF :
                if (!emfLoaded) {
                    ScilabCommonsUtils.loadOnUse(CLASSPATH_EMF_EXPORT_NAME);
                    emfLoaded = true;
                }
                return new EMFExporter();
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
        abstract int write() throws IOException;

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
        public int write() throws IOException {
            return ExportBitmap.writeFile(image, "png", file);
        }

        @Override
        public void dispose() {
            if (g2d != null) {
                g2d.dispose();
            }
        }
    }

    /**
     * GIF Exporter
     */
    private static class GIFExporter extends PNGExporter {

        public GIFExporter() { }

        @Override
        public int write() throws IOException {
            return ExportBitmap.writeFile(image, "gif", file);
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
        public int write() throws IOException {
            return ExportBitmap.writeFile(image, "bmp", file);
        }
    }

    /**
     * JPEG Exporter
     */
    private static class JPEGExporter extends BMPExporter {

        public JPEGExporter() { }

        @Override
        public int write() throws IOException {
            if (params.compressionQuality == -1) {
                return ExportBitmap.writeJPEG(image, DEFAULT_JPEG_COMPRESSION, file);
            } else {
                return ExportBitmap.writeJPEG(image, params.compressionQuality, file);
            }
        }
    }

    /**
     * PPM Exporter
     */
    private static class PPMExporter extends BMPExporter {

        public PPMExporter() { }

        @Override
        public int write() throws IOException {
            OutputStream out = new BufferedOutputStream(new FileOutputStream(file));
            PPMEncoder encoder = new PPMEncoder(image, out);
            int[] pixels = image.getRGB(0, 0, image.getWidth(), image.getHeight(), null, 0, image.getWidth());
            encoder.encodeStart(image.getWidth(), image.getHeight());
            encoder.encodePixels(0, 0, image.getWidth(), image.getHeight(), pixels, 0, image.getWidth());
            out.flush();
            out.close();
            return Export.SUCCESS;
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
            final SVGGeneratorContext ctx = SVGGeneratorContext.createDefault(document);
            ctx.setComment("Generated by Scilab with Batik SVG Generator");
            // TODO: better handle of LaTeX fonts (should remove the 'true' below and include the font in the SVG)
            // same thing for PDF & co...
            ctx.setEmbeddedFontsOn(true);
            g2d = new SVGGraphics2D(ctx, false) {

                @Override
                public void drawString(String s, float x, float y) {
                    textAsShapes = getFont().getFontName().startsWith("jlm");
                    super.drawString(s, x, y);
                }

                @Override
                public void drawString(AttributedCharacterIterator ati, float x, float y) {
                    textAsShapes = getFont().getFontName().startsWith("jlm");
                    super.drawString(ati, x, y);
                }
            };
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
        public int write() throws IOException {
            boolean useCSS = true;
            OutputStream svgs = new BufferedOutputStream(new FileOutputStream(file));
            Writer out = new OutputStreamWriter(svgs, "UTF-8");
            g2d.stream(out, useCSS);
            svgs.flush();
            svgs.close();
            return Export.SUCCESS;
        }

        @Override
        public void dispose() {
            if (g2d != null) {
                g2d.dispose();
            }
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
        public int write() throws IOException {
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
            return Export.SUCCESS;
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
                        gen.writeln("/ReEncode { /MyEncoding exch def exch findfont dup length dict begin {def} forall /Encoding MyEncoding def currentdict end definefont } def");
                        gen.writeln("/Helvetica /HelveticaLatin1 ISOLatin1Encoding ReEncode");
                        gen.writeln("/Times /TimesLatin1 ISOLatin1Encoding ReEncode");
                        gen.writeln("/DP {/Points exch def N Points 0 get Points 1 get M 2 2 Points length 1 sub {/i exch def Points i get Points i 1 add get L}for} def");
                    }

                    @Override
                    public void drawString(String s, float x, float y) {
                        if (s != null && !s.isEmpty()) {
                            CharsetEncoder encoder = Charset.forName("ISO-8859-1").newEncoder();
                            if (encoder.canEncode(s)) {
                                Font font = getFont();
                                boolean sserif = font.getName().equals("SansSerif");
                                boolean serif = font.getName().equals("Serif");
                                if (sserif || serif) {
                                    try {
                                        preparePainting();
                                        establishColor(getColor());
                                        gen.writeln((sserif ? "/HelveticaLatin1" : "/TimesLatin1") + " " + gen.formatDouble(getFont().getSize()) + " F");

                                        gen.saveGraphicsState();
                                        Shape imclip = getClip();
                                        writeClip(imclip);

                                        AffineTransform trans = getTransform();
                                        boolean newTransform = gen.getCurrentState().checkTransform(trans) && !trans.isIdentity();

                                        if (newTransform) {
                                            gen.concatMatrix(trans);
                                        }

                                        gen.writeln(gen.formatDouble(x)
                                                    + " " + gen.formatDouble(y)
                                                    + " M 1 -1 scale");

                                        StringBuffer buf = new StringBuffer("(");
                                        for (int i = 0; i < s.length(); i++) {
                                            PSGenerator.escapeChar(s.charAt(i), buf);
                                        }
                                        buf.append(") t");

                                        gen.writeln(buf.toString());

                                        gen.restoreGraphicsState();
                                    } catch (IOException e) {
                                        System.err.println(e);
                                    }

                                    return;
                                }
                            }

                            super.drawString(s, x, y);
                        }
                    }

                    @Override
                    public boolean shouldBeClipped(Shape clip, Shape s) {
                        if (clip == null || s == null) {
                            return false;
                        }

                        return clip.getBounds2D().intersects(s.getBounds2D());
                    }

                    @Override
                    public int processShape(Shape s, boolean b) throws IOException {
                        if (s instanceof Ellipse2D.Double) {
                            Ellipse2D.Double ell = (Ellipse2D.Double) s;
                            if (ell.height == ell.width) {
                                gen.writeln(gen.formatDouble(ell.x + ell.width / 2)
                                            + " " + gen.formatDouble(ell.y + ell.height / 2)
                                            + " " + gen.formatDouble(ell.width / 2)
                                            + " " + gen.formatDouble(0d)
                                            + " " + gen.formatDouble(360d)
                                            + " arc cp");

                                return PathIterator.WIND_NON_ZERO;
                            }
                        } else if (s instanceof Path2D) {
                            StringBuilder buffer = new StringBuilder();
                            double[] coords = new double[6];
                            PathIterator it = ((Path2D) s).getPathIterator(new AffineTransform());
                            if (!it.isDone()) {
                                int type = it.currentSegment(coords);
                                if (type == PathIterator.SEG_MOVETO) {
                                    buffer.append("[").append(gen.formatDouble(coords[0])).append(" ").append(gen.formatDouble(coords[1]));
                                    it.next();
                                } else {
                                    return super.processShape(s, b);
                                }
                            } else {
                                return super.processShape(s, b);
                            }

                            for (; !it.isDone(); it.next()) {
                                int type = it.currentSegment(coords);
                                if (type == PathIterator.SEG_LINETO) {
                                    buffer.append(" ").append(gen.formatDouble(coords[0])).append(" ").append(gen.formatDouble(coords[1]));
                                } else {
                                    return super.processShape(s, b);
                                }
                            }
                            buffer.append("] DP");
                            gen.writeln(buffer.toString());
                            return PathIterator.WIND_NON_ZERO;
                        }

                        return super.processShape(s, b);
                    }
                };
                g2d.setGraphicContext(new GraphicContext());
            } catch (IOException e) { }

            return g2d;
        }

        @Override
        public int write() throws IOException {
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
            return Export.SUCCESS;
        }

        @Override
        public void dispose() {
            if (g2d != null) {
                g2d.dispose();
            }
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
                        gen.writeln("/ReEncode { /MyEncoding exch def exch findfont dup length dict begin {def} forall /Encoding MyEncoding def currentdict end definefont } def");
                        gen.writeln("/Helvetica /HelveticaLatin1 ISOLatin1Encoding ReEncode");
                        gen.writeln("/Times /TimesLatin1 ISOLatin1Encoding ReEncode");

                        // DP macro is used to draw an array as a polyline
                        gen.writeln("/DP {/Points exch def Points 0 get Points 1 get M 2 2 Points length 1 sub {/i exch def Points i get Points i 1 add get L}for} def");
                    }

                    @Override
                    public boolean shouldBeClipped(Shape clip, Shape s) {
                        if (clip == null || s == null) {
                            return false;
                        }

                        return clip.getBounds2D().intersects(s.getBounds2D());
                    }

                    @Override
                    public void drawString(String s, float x, float y) {
                        if (s != null && !s.isEmpty()) {
                            CharsetEncoder encoder = Charset.forName("ISO-8859-1").newEncoder();
                            if (encoder.canEncode(s)) {
                                Font font = getFont();
                                boolean sserif = font.getName().equals("SansSerif");
                                boolean serif = font.getName().equals("Serif");
                                if (sserif || serif) {
                                    try {
                                        preparePainting();
                                        establishColor(getColor());
                                        gen.writeln((sserif ? "/HelveticaLatin1" : "/TimesLatin1") + " " + gen.formatDouble(getFont().getSize()) + " F");

                                        gen.saveGraphicsState();
                                        Shape imclip = getClip();
                                        writeClip(imclip);

                                        AffineTransform trans = getTransform();
                                        boolean newTransform = gen.getCurrentState().checkTransform(trans) && !trans.isIdentity();

                                        if (newTransform) {
                                            gen.concatMatrix(trans);
                                        }

                                        gen.writeln(gen.formatDouble(x)
                                                    + " " + gen.formatDouble(y)
                                                    + " M 1 -1 scale");

                                        StringBuffer buf = new StringBuffer("(");
                                        for (int i = 0; i < s.length(); i++) {
                                            PSGenerator.escapeChar(s.charAt(i), buf);
                                        }
                                        buf.append(") t");

                                        gen.writeln(buf.toString());

                                        gen.restoreGraphicsState();
                                    } catch (IOException e) {
                                        System.err.println(e);
                                    }

                                    return;
                                }
                            }

                            super.drawString(s, x, y);
                        }
                    }

                    @Override
                    public int processShape(Shape s, boolean b) throws IOException {
                        if (s instanceof Ellipse2D.Double) {
                            Ellipse2D.Double ell = (Ellipse2D.Double) s;
                            if (ell.height == ell.width) {
                                gen.writeln(gen.formatDouble(ell.x + ell.width / 2)
                                            + " " + gen.formatDouble(ell.y + ell.height / 2)
                                            + " " + gen.formatDouble(ell.width / 2)
                                            + " " + gen.formatDouble(0d)
                                            + " " + gen.formatDouble(360d)
                                            + " arc cp");

                                return PathIterator.WIND_NON_ZERO;
                            }
                        } else if (s instanceof Path2D) {
                            StringBuilder buffer = new StringBuilder();
                            double[] coords = new double[6];
                            PathIterator it = ((Path2D) s).getPathIterator(new AffineTransform());
                            if (!it.isDone()) {
                                int type = it.currentSegment(coords);
                                if (type == PathIterator.SEG_MOVETO) {
                                    buffer.append("[").append(gen.formatDouble(coords[0])).append(" ").append(gen.formatDouble(coords[1]));
                                    it.next();
                                } else {
                                    return super.processShape(s, b);
                                }
                            } else {
                                return super.processShape(s, b);
                            }

                            for (; !it.isDone(); it.next()) {
                                int type = it.currentSegment(coords);
                                if (type == PathIterator.SEG_LINETO) {
                                    buffer.append(" ").append(gen.formatDouble(coords[0])).append(" ").append(gen.formatDouble(coords[1]));
                                } else {
                                    return super.processShape(s, b);
                                }
                            }
                            buffer.append("] DP");
                            gen.writeln(buffer.toString());
                            return PathIterator.WIND_NON_ZERO;
                        }

                        return super.processShape(s, b);
                    }

                };
                g2d.setupDocument(out, width, height);
                g2d.setGraphicContext(new GraphicContext());
            } catch (IOException e) { }

            return g2d;
        }
    }

    /**
     * EMF Exporter
     */
    private static class EMFExporter extends Exporter {

        private OutputStream out;
        private Class<Graphics2D> g2dClass;
        private Constructor<Graphics2D> g2dCtor;
        private Graphics2D g2d;
        private ByteArrayOutputStream buffer;

        public EMFExporter() {
            try {
                g2dClass = (Class<Graphics2D>) Class.forName("org.freehep.graphicsio.emf.EMFGraphics2D");
            } catch (ClassNotFoundException e) {
                throw new RuntimeException("This Scilab build does not provide EMF support");
            }

            final Constructor[] ctors = g2dClass.getDeclaredConstructors();
            Constructor ctor = null;
            for (int i = 0; i < ctors.length; i++) {
                ctor = ctors[i];
                final Type[] args = ctor.getGenericParameterTypes();
                if (args.length != 2) {
                    continue;
                }
                if (args[0] != OutputStream.class) {
                    continue;
                }
                if (args[1] != Dimension.class) {
                    continue;
                }

                g2dCtor = ctor;
                break;
            }
        }

        @Override
        public Graphics2D getGraphics2D(int width, int height, File file, final ExportParams params) {
            this.file = file;
            try {
                if (file == null) {
                    buffer = new ByteArrayOutputStream();
                    out = new BufferedOutputStream(buffer);
                } else {
                    out = new BufferedOutputStream(new FileOutputStream(file));
                }
                if (params.orientation == ExportParams.LANDSCAPE) {
                    g2d = g2dCtor.newInstance(out, new Dimension(height, width));
                    g2dClass.getMethod("startExport").invoke(g2d);
                    AffineTransform transf = AffineTransform.getRotateInstance(Math.PI / 2);
                    transf.preConcatenate(AffineTransform.getTranslateInstance(height, 0));
                    g2d.setTransform(transf);
                } else {
                    g2d = g2dCtor.newInstance(out, new Dimension(width, height));
                    g2dClass.getMethod("startExport").invoke(g2d);
                }
            } catch (IOException e) {
            } catch (IllegalAccessException e) {
            } catch (IllegalArgumentException e) {
            } catch (InvocationTargetException e) {
            } catch (NoSuchMethodException e) {
            } catch (SecurityException e) {
            } catch (InstantiationException e) {
            }

            return g2d;
        }

        @Override
        public int write() throws IOException {
            if (g2d != null) {
                try {
                    g2dClass.getMethod("endExport").invoke(g2d);
                    g2dClass.getMethod("closeStream").invoke(g2d);
                } catch (IllegalAccessException e) {
                } catch (IllegalArgumentException e) {
                } catch (InvocationTargetException e) {
                } catch (NoSuchMethodException e) {
                } catch (SecurityException e) {
                }
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
            return Export.SUCCESS;
        }

        @Override
        public void dispose() {
            if (g2d != null) {
                g2d.dispose();
            }
        }
    }
}
