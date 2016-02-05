/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Hussein SHAFIE
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
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

import java.io.IOException;
import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.URISyntaxException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Map;
import java.util.Iterator;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.zip.GZIPInputStream;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import org.xml.sax.SAXParseException;
import org.xml.sax.SAXException;
import org.xml.sax.Locator;
import org.xml.sax.Attributes;
import org.xml.sax.helpers.DefaultHandler;

import org.apache.batik.transcoder.TranscoderException;
import org.apache.batik.transcoder.ErrorHandler;
import org.apache.batik.transcoder.TranscoderInput;
import org.apache.batik.transcoder.TranscoderOutput;
import org.apache.batik.transcoder.Transcoder;
import org.apache.batik.transcoder.XMLAbstractTranscoder;
import org.apache.batik.transcoder.image.ImageTranscoder;
import org.apache.batik.transcoder.image.PNGTranscoder;

import net.sourceforge.jeuclid.context.Parameter;
import net.sourceforge.jeuclid.MutableLayoutContext;
import net.sourceforge.jeuclid.context.LayoutContextImpl;
import net.sourceforge.jeuclid.converter.Converter;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.ParseException;
import java.awt.Color;

/**
 * Preprocess before building the documentation
 */
public class CopyConvert extends DefaultHandler implements ErrorHandler {

    private static final String MATHML_NS =
        "http://www.w3.org/1998/Math/MathML";

    /**
     * The tmp directory
     */
    public static final File TMPDIR = new File(System.getenv("TMPDIR"));

    private boolean verbose;
    private String printFormat;

    private File outDir;
    private PrintWriter out;
    private SAXParser parser;
    private Locator locator;

    private ArrayList<String[]> inScopePrefixes;
    private PrintWriter mainOut;
    private int graphicsCounter;
    private int embeddedGraphicsIsMathML;

    private int initTeX;
    private File latex;
    private File dvips;
    private File gs;
    private File ps2pdf;

    private LaTeXElement latexElem;
    private boolean isLatexConverted = true;

    /**
     * Enables the verbose mode
     *
     * @param verbose true if enable
     */
    public void setVerbose(boolean verbose) {
        this.verbose = verbose;
    }

    /**
     * Set the print format
     *
     * @param printFormat  The print format
     */
    public void setPrintFormat(String printFormat) {
        this.printFormat = printFormat;
    }



    /**
     * Set if the LaTeX label are converted in PNG or not to use with
     * the jlatexmath-fop plugin.
     * @param isLatexConverted true if LaTeX label are converted in PNG
     */
    public void setLatexConverted(boolean isLatexConverted) {
        this.isLatexConverted = isLatexConverted;
    }

    // -----------------------------------------------------------------------

    /**
     * Run the copy/convert process
     *
     * @param inFile Input file
     * @param outputFile Output file
     */
    public void run(File inFile, File outputFile)
    throws SAXParseException, SAXException, IOException {
        File outFile = outputFile.getCanonicalFile();
        outDir = outFile.getParentFile();
        if (!outDir.isDirectory() && !outDir.mkdirs()) {
            throw new IOException("Cannot create directory '" + outDir + "'");
        }

        try {
            SAXParserFactory factory = SAXParserFactory.newInstance();
            factory.setNamespaceAware(true);
            // We need qNames and xmlns*.
            // FIXME: xmlns:db prefix is not handled by the thirdparty's stylesheet (not the right version)
            //            factory.setFeature(
            //                "http://xml.org/sax/features/namespace-prefixes", true);
            factory.setValidating(false);
            //factory.setXIncludeAware(false);

            parser = factory.newSAXParser();
        } catch (Exception e) {
            throw new SAXParseException(
                "Cannot create a properly configured SAX parser: " + Helpers.reason(e), locator);
        }

        inScopePrefixes = new ArrayList<String[]>();
        mainOut = null;
        graphicsCounter = 0;
        embeddedGraphicsIsMathML = -1;
        initTeX = -1;

        out = new PrintWriter(
            new OutputStreamWriter(new FileOutputStream(outFile), "UTF-8"));
        try {
            parser.parse(inFile, this);
            out.flush();
            if (out.checkError()) {
                throw new IOException("Error writing '" + outFile + "'");
            }
        } catch (SAXException e) {
            if (locator != null) {
                throw new SAXParseException("Cannot parse " + inFile + " " + Helpers.reason(e), locator);
            } else {
                throw new SAXException("Cannot parse " + inFile + " " + Helpers.reason(e));
            }
        } finally {
            out.close();
        }
    }

    // -----------------------------------------------------------------------
    // ContentHandler
    // -----------------------------------------------------------------------

    /**
     * Set the document Locator
     *
     * @param locator  The locator
     */
    public void setDocumentLocator(Locator locator) {
        this.locator = locator;
    }

    public void startDocument() throws SAXException {
        out.write("<?xml version='1.0' encoding='UTF-8'?>\n");
    }

    public void endDocument() throws SAXException {
    }


    public void startPrefixMapping(String prefix, String uri)
    throws SAXException {
        inScopePrefixes.add(new String[] {prefix, uri });
    }

    public void endPrefixMapping(String prefix)
    throws SAXException {
        int count = inScopePrefixes.size();
        for (int i = count - 1; i >= 0; --i) {
            String[] pair = inScopePrefixes.get(i);
            if (pair[0].equals(prefix)) {
                inScopePrefixes.remove(i);
                break;
            }
        }
    }

    public void startElement(String uri, String localName, String qName,
                             Attributes atts)
    throws SAXException {
        out.write('<');

        if ("latex".equals(localName)) {
            latexElem = new LaTeXElement(atts, isLatexConverted);
            return;
        }

        out.write(qName);

        boolean isImage = "imagedata".equals(localName);
        boolean isGraphicsFile = false;

        int count = atts.getLength();
        for (int i = 0; i < count; ++i) {
            String attValue = atts.getValue(i);

            if (isImage && "fileref".equals(atts.getLocalName(i))) {
                isGraphicsFile = true;

                URL url = null;

                try {
                    URL base = new URL(locator.getSystemId());
                    url = new URL(base, attValue);
                } catch (MalformedURLException e) {
                    reportError("Malformed URL '" + attValue + "'");
                }

                if (url != null) {
                    File graphicsFile = null;

                    try {
                        graphicsFile = new File(url.toURI());
                    } catch (URISyntaxException e) {
                        reportError("Malformed URI '" + url + "'");
                    } catch (IllegalArgumentException e) {
                        reportError("'" + url + "', not a 'file:' URL");
                    }

                    if (graphicsFile != null) {
                        String converted =
                            copyConvertGraphics(graphicsFile);
                        if (converted != null) {
                            // Substitute the basename of the converted file
                            // to attValue.
                            attValue = converted;
                        }
                    }
                }
            }

            out.write(' ');
            out.write(atts.getQName(i));
            out.write("='");
            Helpers.escapeXML(attValue, out);
            out.write('\'');
        }

        if (mainOut != null) {
            if (embeddedGraphicsIsMathML < 0) {
                // Is embedded graphics SVG or MathML?
                embeddedGraphicsIsMathML = MATHML_NS.equals(uri) ? 1 : 0;
            }

            // Declare namespace prefixes in the extracted file.
            declarePrefixes(atts);
        }

        if (!isImage || isGraphicsFile) {
            out.write('>');
        } else {
            File extractedFile =
                new File(outDir, "graphics-" + (++graphicsCounter) + ".tmp");
            try {
                PrintWriter extracted = new PrintWriter(
                    new OutputStreamWriter(new FileOutputStream(extractedFile),
                                           "UTF-8"));

                out.flush();
                mainOut = out;

                out = extracted;
                out.write("<?xml version='1.0' encoding='UTF-8'?>\n");

                embeddedGraphicsIsMathML = -1;
            } catch (IOException e) {
                reportError("Cannot create file '" + extractedFile + "': "
                            + Helpers.reason(e));
                // Keep embedded graphics.
                out.write('>');
            }
        }
    }

    private void declarePrefixes(Attributes atts) {
        HashMap<String, String> uniquePrefixes = new HashMap<String, String>();

        int count = inScopePrefixes.size();
        for (int i = 0; i < count; ++i) {
            String[] pair = inScopePrefixes.get(i);

            uniquePrefixes.put(pair[0], pair[1]);
        }

        Iterator<Map.Entry<String, String>> iter =
            uniquePrefixes.entrySet().iterator();
        while (iter.hasNext()) {
            Map.Entry<String, String> e = iter.next();

            String prefix = e.getKey();
            String namespace = e.getValue();

            String xmlns;
            if (prefix.length() == 0) {
                xmlns = "xmlns";
            } else {
                xmlns = "xmlns:" + prefix;
            }

            if (atts.getValue(xmlns) == null) { // If not already declared.
                out.write(' ');
                out.write(xmlns);
                out.write("='");
                Helpers.escapeXML(namespace, out);
                out.write('\'');
            }
        }
    }

    public void endElement(String uri, String localName, String qName)
    throws SAXException {
        if ("latex".equals(localName)) {
            latexElem.generate(locator);
            latexElem = null;
            return;
        }

        if ("imagedata".equals(localName) && mainOut != null) {
            String rootName = "graphics-" + graphicsCounter;
            String baseName = rootName + ".tmp";
            File extractedFile = new File(outDir, baseName);

            out.flush();
            if (out.checkError()) {
                reportError("Error writing '" + extractedFile + "'");
                out.close();
                extractedFile = null;
            } else {
                out.close(); // Close now otherwise rename fails on Windows

                File extractedFile2 = new File(
                    outDir,
                    rootName + ((embeddedGraphicsIsMathML == 1) ? ".mml" : ".svg"));
                if (!extractedFile.renameTo(extractedFile2)) {
                    reportError("Cannot rename '" + extractedFile + "' to '"
                                + extractedFile2 + "'");
                    extractedFile = null;
                } else {
                    extractedFile = extractedFile2;
                }
            }

            out = mainOut;
            mainOut = null;

            if (extractedFile != null) {
                reportInfo("Extracted embedded graphics from '" + extractedFile + "' to '" + extractedFile + "'.");

                String converted = copyConvertGraphics(extractedFile);
                if (converted != null) {
                    baseName = converted;

                    if (!extractedFile.delete()) {
                        reportError("Cannot delete '" + extractedFile + "'");
                    }
                }
            }

            out.write(" fileref='");
            Helpers.escapeXML(baseName, out);
            out.write("'/>");
        } else {
            out.write("</");
            out.write(qName);
            out.write('>');
        }
    }

    public void characters(char[] ch, int start, int length)
    throws SAXException {
        if (latexElem != null) {
            latexElem.setLaTeX(new String(ch, start, length));
            return;
        }

        Helpers.escapeXML(ch, start, length, out);
    }

    public void processingInstruction(String target, String data)
    throws SAXException {
        // The DocBook XSL style sheets make use of processing-instructions.
        out.write("<?");
        out.write(target);
        out.write(' ');
        out.write(data);
        out.write("?>");
    }

    public void ignorableWhitespace(char[] ch, int start, int length)
    throws SAXException { }

    public void skippedEntity(String name)
    throws SAXException { }

    // -----------------------------------------------------------------------
    // Copy & convert graphics file
    // -----------------------------------------------------------------------

    private String copyConvertGraphics(File graphicsFile) {
        if (!graphicsFile.isFile()) {
            reportError("'" + graphicsFile + "' not found");
            return null;
        }

        String baseName = graphicsFile.getName();
        String rootName = Helpers.setFileExtension(baseName, null);
        String ext = Helpers.getFileExtension(baseName);

        if ("tex".equalsIgnoreCase(ext)) {
            if ("ps".equalsIgnoreCase(printFormat)) {
                baseName = rootName + "_tex.eps";
            } else if ("pdf".equalsIgnoreCase(printFormat)) {
                baseName = rootName + "_tex.pdf";
            } else {
                baseName = rootName + "_tex.png";
            }
            File convertedFile = new File(outDir, baseName);

            if (!convertedFile.exists() || convertedFile.lastModified() < graphicsFile.lastModified()) {
                reportInfo("Converting TeX '" + graphicsFile + "' to '"
                           + convertedFile + "'...");

                if (!convertTeX(graphicsFile, convertedFile)) {
                    baseName = null;
                }
            }
        } else if ("mml".equalsIgnoreCase(ext)) {
            baseName = rootName + "_mml.png";
            File convertedFile = new File(outDir, baseName);

            if (!convertedFile.exists() || convertedFile.lastModified() < graphicsFile.lastModified()) {
                reportInfo("Converting MathML '" + graphicsFile + "' to '"
                           + convertedFile + "'...");

                if (!convertMathML(graphicsFile, convertedFile)) {
                    baseName = null;
                }
            }
        } else if ("svg".equalsIgnoreCase(ext) || "svgz".equalsIgnoreCase(ext)) {
            baseName = rootName + "_svg.png";
            File convertedFile = new File(outDir, baseName);

            if (!convertedFile.exists() || convertedFile.lastModified() < graphicsFile.lastModified()) {
                reportInfo("Converting SVG '" + graphicsFile + "' to '"
                           + convertedFile + "'...");

                if (!convertSVG("svgz".equalsIgnoreCase(ext), graphicsFile, convertedFile)) {
                    baseName = null;
                }
            }
        } else {
            // Just copy the file ---

            File outFile = new File(outDir, baseName);
            if (!outFile.exists() || outFile.lastModified() < graphicsFile.lastModified()) {
                reportInfo("Copying '" + graphicsFile + "' to '"
                           + outFile + "'...");

                try {
                    Helpers.copyFile(graphicsFile, outFile);
                } catch (IOException e) {
                    reportError("Cannot copy '" + graphicsFile + "' to '"
                                + outFile + "': " + Helpers.reason(e));
                }
            }
        }

        return baseName;
    }

    private boolean convertTeX(File inFile, File outFile) {
        if (!initTeX()) {
            return false;
        }

        try {
            doConvertTeX(inFile, outFile);
            return true;
        } catch (Exception e) {
            reportError("Cannot convert '" + inFile + "' to '"
                        + outFile + "': " + Helpers.reason(e));
            return false;
        }
    }

    private boolean initTeX() {
        if (initTeX == -1) {
            latex = Helpers.findInPath("latex");
            if (latex == null) {
                reportError("Don't find executable 'latex' in PATH");
            }

            dvips = Helpers.findInPath("dvips");
            if (dvips == null) {
                reportError("Don't find executable 'dvips' in PATH");
            }

            String appName = Helpers.IS_WINDOWS ? "gswin32c" : "gs";
            gs = Helpers.findInPath(appName);
            if (gs == null) {
                reportError("Don't find executable '" + appName + "' in PATH");
            }

            ps2pdf = Helpers.findInPath("ps2pdf");
            if (ps2pdf == null) {
                reportError("Don't find executable 'ps2pdf' in PATH");
            }

            if (latex == null || dvips == null || gs == null || ps2pdf == null) {
                initTeX = 0;
            } else {
                initTeX = 1;
            }
        }
        return (initTeX == 1);
    }

    private void doConvertTeX(File inFile, File outFile)
    throws IOException, InterruptedException {
        File latexFile = wrapTeX(inFile);

        File tmpDir = latexFile.getParentFile();

        String latexBaseName = latexFile.getName();
        String latexRootName = Helpers.setFileExtension(latexBaseName, null);

        File auxFile = new File(tmpDir, latexRootName + ".aux");
        File logFile = new File(tmpDir, latexRootName + ".log");
        File dviFile = new File(tmpDir, latexRootName + ".dvi");
        File epsFile = new File(tmpDir, latexRootName + ".eps");

        try {
            StringBuilder cmd = new StringBuilder();
            cmd.append('\"');
            cmd.append(latex);
            cmd.append("\" --interaction batchmode ");
            cmd.append(latexBaseName);

            shellExec(cmd.toString(), tmpDir);

            if (!dviFile.isFile()) {
                throw new IOException("'" + inFile + "' has TeX errors");
            }

            cmd = new StringBuilder();
            cmd.append('\"');
            cmd.append(dvips);
            cmd.append("\" -E -o \"");
            cmd.append(epsFile);
            cmd.append("\" \"");
            cmd.append(dviFile);
            cmd.append("\"");

            shellExec(cmd.toString(), tmpDir);

            if ("ps".equalsIgnoreCase(printFormat)) {
                Helpers.copyFile(epsFile, outFile);
            } else if ("pdf".equalsIgnoreCase(printFormat)) {
                cmd = new StringBuilder();
                cmd.append('\"');
                cmd.append(ps2pdf);
                cmd.append("\" -dEPSCrop \"");
                cmd.append(epsFile);
                cmd.append("\" \"");
                cmd.append(outFile);
                cmd.append("\"");

                shellExec(cmd.toString(), tmpDir);
            } else {
                cmd = new StringBuilder();
                cmd.append('\"');
                cmd.append(gs);
                cmd.append("\" -q -dBATCH -dNOPAUSE -sDEVICE=png16m");
                cmd.append(" -r96 -dTextAlphaBits=4 -dGraphicsAlphaBits=4");
                cmd.append(" -dEPSCrop \"-sOutputFile=");
                cmd.append(outFile);
                cmd.append("\" \"");
                cmd.append(epsFile);
                cmd.append("\"");

                shellExec(cmd.toString(), tmpDir);
            }
        } finally {
            // Delete all temporary files ---
            latexFile.delete();
            if (auxFile.isFile()) {
                auxFile.delete();
            }
            if (logFile.isFile()) {
                logFile.delete();
            }
            if (dviFile.isFile()) {
                dviFile.delete();
            }
            if (epsFile.isFile()) {
                epsFile.delete();
            }
        }
    }

    private File wrapTeX(File inFile)
    throws IOException {
        String tex = Helpers.loadString(inFile, "ISO-8859-1");

        File latexFile = File.createTempFile("CopyConvert", ".tex", TMPDIR);

        StringBuilder buffer = new StringBuilder();
        buffer.append("\\documentclass[12pt]{article}\n");
        buffer.append("\\usepackage[latin1]{inputenc}\n");
        buffer.append("\\pagestyle{empty}\n");
        buffer.append("\\begin{document}\n");
        buffer.append(tex);
        buffer.append('\n');
        buffer.append("\\end{document}\n");

        Helpers.saveString(buffer.toString(), latexFile, "ISO-8859-1");

        return latexFile;
    }

    private void shellExec(String command, File workDir)
    throws IOException, InterruptedException {
        int status =
            Helpers.shellExec(command, /*envp*/ null, workDir, verbose);
        if (status != 0) {
            throw new RuntimeException("command '" + command
                                       + "' has exited with non-zero status "
                                       + status);
        }
    }

    private boolean convertMathML(File inFile, File outFile) {
        MutableLayoutContext context = new LayoutContextImpl(LayoutContextImpl
                .getDefaultLayoutContext());
        context.setParameter(Parameter.ANTIALIAS, "true");
        // Workaround a XEP problem. FOP 1 is OK.
        context.setParameter(Parameter.MATHBACKGROUND, "#FFFFFF");
        context.setParameter(Parameter.MATHSIZE, "18");

        try {
            Converter.getInstance().convert(inFile, outFile, "image/png", context);

            return true;
        } catch (IOException e) {
            reportError("Cannot convert '" + inFile + "' to '" + outFile + "': " + Helpers.reason(e));
            return false;
        }
    }

    private boolean convertSVG(boolean gunzip, File inFile, File outFile) {
        Transcoder transcoder = new PNGTranscoder();
        transcoder.addTranscodingHint(
            ImageTranscoder.KEY_FORCE_TRANSPARENT_WHITE,
            Boolean.TRUE);

        try {
            InputStream in = new FileInputStream(inFile);
            if (gunzip) {
                in = new GZIPInputStream(in);
            }

            try {
                TranscoderInput input = new TranscoderInput(in);
                input.setURI(inFile.toURI().toASCIIString());

                // Failing to do so causes Batik to report this error: SAX2
                // driver class org.apache.xerces.parsers.SAXParser not found
                // which is normal because we don't want to bundle a copy of
                // Xerces.

                transcoder.addTranscodingHint(
                    XMLAbstractTranscoder.KEY_XML_PARSER_CLASSNAME,
                    parser.getXMLReader().getClass().getName());
                transcoder.addTranscodingHint(
                    XMLAbstractTranscoder.KEY_XML_PARSER_VALIDATING,
                    Boolean.FALSE);

                OutputStream outf = new FileOutputStream(outFile);

                try {
                    TranscoderOutput output = new TranscoderOutput(outf);
                    transcoder.transcode(input, output);
                    outf.flush();
                } finally {
                    outf.close();
                }
            } finally {
                in.close();
            }
            return true;
        } catch (Exception e) {
            reportError("Cannot convert '" + inFile + "' to '"
                        + outFile + "': " + Helpers.reason(e));
            return false;
        }
    }

    // -----------------------------------
    // Batik's ErrorHandler
    // -----------------------------------

    public void warning(TranscoderException e)
    throws TranscoderException {
        String msg = "SVG transcoder warning: " + Helpers.reason(e);
        reportWarning(msg);
    }

    public void error(TranscoderException e)
    throws TranscoderException {
        String msg = "SVG transcoder error: " + Helpers.reason(e);
        reportError(msg);
    }

    public void fatalError(TranscoderException e)
    throws TranscoderException {
        String msg = "SVG transcoder fatal error: " + Helpers.reason(e);
        reportError(msg);
    }

    // -----------------------------------------------------------------------
    // Utilities
    // -----------------------------------------------------------------------

    private void reportInfo(String message) {
        if (verbose) {
            System.out.println(message);
        }
    }

    private static void reportWarning(String message) {
        System.err.println("warning: " + message);
    }

    private static void reportError(String message) {
        System.err.println("*** error: " + message);
    }

    // -----------------------------------------------------------------------
    // Main
    // -----------------------------------------------------------------------

    public static int main(String[] args) {
        boolean verbose = false;
        String printFormat = null;
        boolean usage = false;
        int l = 0;

        for (; l < args.length; ++l) {
            String arg = args[l];

            if ("-v".equalsIgnoreCase(arg)) {
                verbose = true;
            } else if ("-ps".equalsIgnoreCase(arg)) {
                printFormat = "ps";
            } else if ("-pdf".equalsIgnoreCase(arg)) {
                printFormat = "pdf";
            } else {
                if (arg.startsWith("-")) {
                    usage = true;
                }
                break;
            }
        }

        if (!usage && l + 2 != args.length) {
            usage = true;
        }

        if (usage) {
            System.err.println(
                "Usage: java org.scilab.modules.helptools.CopyConvert"
                + "  {-ps|-pdf}? {-v}? in_xml_file out_xml_file\n"
                + "Creates out_xml_file as a ``flattened'' copy of\n"
                + "in_xml_file.\n"
                + "All graphics files referenced by <imagedata fileref='XXX'/>\n"
                + "elements are copied, possibly after being converted to PNG,\n"
                + "to the directory containing in_xml_file.\n"
                + "(This directory is assumed to be a temporary \n"
                + "working directory.)\n"
                + "Options are:\n"
                + "  -ps  Target format is PostScript rather than HTML.\n"
                + "  -pdf Target format is PDF rather than HTML.\n"
                + "  -v   Verbose.");
            return 1;
        }

        File inFile = new File(args[l]);
        File outFile = new File(args[l + 1]);

        CopyConvert copyConvert = new CopyConvert();
        copyConvert.verbose = verbose;
        copyConvert.printFormat = printFormat;

        try {
            copyConvert.run(inFile, outFile);
        } catch (Exception e) {
            reportError("Cannot copy/convert '" + inFile + "' to '" + outFile
                        + "': " + Helpers.reason(e));
            return 2;
        }
        return 0;
    }

    protected class LaTeXElement {
        int size = 18;
        Color fg = null, bg = null;
        int disp = TeXConstants.STYLE_DISPLAY;
        String code = "mediaobject><imageobject><imagedata";
        String LaTeX = "";
        boolean exported;
        String attribs, align = "";

        protected LaTeXElement(Attributes attrs, boolean exported) {
            this.exported = exported;
            int n = attrs.getLength();
            String fgS = "", bgS = "";
            String dispS = "display";

            for (int i = 0; i < n; i++) {
                String attr = attrs.getValue(i);
                String name = attrs.getLocalName(i);
                if ("align".equals(name)) {
                    code += " align=\'" + attr + "\'";
                    align = attr;
                } else if ("style".equals(name)) {
                    if ("text".equals(attr)) {
                        disp = TeXConstants.STYLE_TEXT;
                    } else if ("script".equals(attr)) {
                        disp = TeXConstants.STYLE_SCRIPT;
                    } else if ("script_script".equals(attr)) {
                        disp = TeXConstants.STYLE_SCRIPT_SCRIPT;
                    }
                    dispS = attr;
                } else if ("size".equals(name)) {
                    try {
                        size = Integer.parseInt(attr);
                    } catch (NumberFormatException e) {
                        size = 16;
                    }
                } else if ("fg".equals(name)) {
                    fg = Color.decode(attr);
                    fgS = attr;
                } else if ("bg".equals(name)) {
                    bg = Color.decode(attr);
                    bgS = attr;
                }
            }

            attribs = " align='" + align + "'";
            attribs += " size='" + size + "'";
            attribs += " style='" + dispS + "'";
            if (bg != null) {
                attribs += " bg='" + bgS + "'";
            }
            if (fg != null) {
                attribs += " fg='" + fgS + "'";
            }
        }

        protected void setLaTeX(String str) {
            LaTeX += str;
        }

        protected void generate(Locator loc) throws SAXParseException {
            if (exported) {
                generatePNG(loc);
                return;
            }
            out.write("latex" + attribs  + " xmlns=\"http://forge.scilab.org/p/jlatexmath\"><![CDATA[" + LaTeX + "]]></latex>");
        }

        protected void generatePNG(Locator loc) throws SAXParseException {
            TeXFormula tf;
            try {
                tf = new TeXFormula(LaTeX);
            } catch (ParseException e) {
                throw new SAXParseException(
                    "\nThere was a problem in parsing the LaTeX' formula : \n"
                    + e.getMessage(), locator);
            }
            File f = new File(outDir, "graphics-" + (++graphicsCounter)
                              + "_latex.png");
            reportInfo("Converting LaTeX formula to " + f + "'...");
            tf.createPNG(disp, size, f.getPath(), bg, fg);

            String end = "";
            if (align.length() == 0) {
                code = "inline" + code;
                end = "inline";
            }

            out.write(code + " fileref='graphics-" + graphicsCounter
                      + "_latex.png'/>" + "</imageobject></" + end
                      + "mediaobject>");
        }
    }

}
