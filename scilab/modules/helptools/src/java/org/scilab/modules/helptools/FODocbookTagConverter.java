/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
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
import java.io.FileNotFoundException;
import java.io.IOException;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.scilab.modules.helptools.image.ImageConverter;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;

/**
 * Implement a docbook to fo converter.
 *
 * FIXME: Currently it use an externel xsl stylesheet to convert the tag. It should implement the Scilab subset by extending {@link DocbookTagConverter}.
 */
public class FODocbookTagConverter implements Converter {

    private static final String SCI = System.getenv("SCI");
    public static final boolean IS_WINDOWS = (File.pathSeparatorChar == ';');
    private static final String CANNOT_COPY_CONVERT = "Cannot copy/convert '";
    private static final String TO_WITH_QUOTES = "' to '";
    private static final String COLON_WITH_QUOTES = "': ";

    private final String inName;
    private final SciDocMain sciDocMain;

    private final File docbookPath;
    private final File styleDoc;

    public FODocbookTagConverter(String inName, SciDocMain sciDocMain, final ImageConverter imgConvert) throws IOException {

        this.inName = inName;
        this.sciDocMain = sciDocMain;

        String localDocbookPath = System.getenv("DOCBOOK_ROOT");
        if (localDocbookPath == null) {
            localDocbookPath = SCI + "/thirdparty/docbook";
        }
        docbookPath = new File(localDocbookPath);
        if (!docbookPath.isDirectory()) {
            throw new FileNotFoundException("Could not find variable DOCBOOK_ROOT defining Docbook root directory");
        }

        styleDoc = new File(new File(docbookPath, "fo"), "docbook.xsl");
        if (!styleDoc.isFile()) {
            throw new FileNotFoundException("Could not find " + styleDoc);
        }
    }

    @Override
    public void registerAllExternalXMLHandlers() {
    }

    @Override
    public void convert() throws SAXException, IOException {
        final File processedStyle = generateExtendedStyle();

        final File sourceDocProcessed = preProcessMaster(inName);
        if (!sourceDocProcessed.isFile()) {
            throw new FileNotFoundException("Unable to parse generated master file : " + inName + " .");
        }

        final StreamResult outputTarget = new StreamResult(new File(Helpers.getTemporaryNameFo(sciDocMain.getOutputDirectory())));
        final StreamSource xmlSource = new StreamSource(sourceDocProcessed);

        /*
         * We rely on the saxon implementation to compile xsl files (the default JVM implementation failed).
         *
         * Supported version :
         *  * Saxon-HE 9.5 (and may be 8.x too) which handle xinclude, XSLT-2 and has better performances
         *  * Saxon 6.5 if on the classpath
         *  * JVM Apache-xerces as a fallback but may probably fail to compile docbook.xsl
         */
        TransformerFactory tfactory;
        try {
            tfactory = TransformerFactory.newInstance("net.sf.saxon.TransformerFactoryImpl", null);
        } catch (TransformerFactoryConfigurationError e) {
            // switch back to the default implementation which may be saxon 6.5 if found on the classpath or the JVM default implementation otherwise
            tfactory = TransformerFactory.newInstance();
        }

        try {
            final Transformer transform = tfactory.newTransformer(new StreamSource(processedStyle));

            transform.setParameter("base.dir", sciDocMain.getOutputDirectory());

            // FO specific parameters
            transform.setParameter("use.extensions", "1");
            transform.setParameter("graphicsize.extension", "0");
            transform.setParameter("paper.type", "A4");
            transform.setParameter("generate.toc", "book toc,title,figure,table,example,equation part toc,title reference toc,title");
            transform.setParameter("toc.section.depth", "3");
            transform.setParameter("section.autolabel", "1");
            transform.setParameter("variablelist.as.blocks", "1");
            transform.setParameter("shade.verbatim", "1");
            transform.setParameter("img.src.path", sciDocMain.getOutputDirectory());

            transform.transform(xmlSource, outputTarget);

            /* Delete the master temp file to avoid to be shipped with the rest */
            sourceDocProcessed.delete();
            processedStyle.delete();

        } catch (TransformerException e) {
            e.printStackTrace();
        }


    }

    @Override
    public void install() throws IOException {

    }

    /**
     * Preprocess the extendedStyle.xsl file
     * Basically, we load the xsl and replace STYLE_DOC by the actual path
     * to the xsl file since docbook cannot replace env variables
     * @return the path to the preprocessed file
     */
    private File generateExtendedStyle() {
        File mainStyleDoc = new File(SCI + "/modules/helptools/schema/extendedStyle.xsl");
        try {
            String contentMainStyleDoc = Helpers.loadString(mainStyleDoc, "UTF-8");

            /* STYLE_DOC is a predefined variable */
            contentMainStyleDoc = contentMainStyleDoc.replaceAll("STYLE_DOC", this.styleDoc.toURI().toString());

            File temporaryStyleFile = File.createTempFile("style_", ".xsl");

            Helpers.saveString(contentMainStyleDoc, temporaryStyleFile, "UTF-8");
            return temporaryStyleFile;
        } catch (java.io.IOException e) {
            System.err.println("Could not convert " + mainStyleDoc);
            return null;
        }
    }

    /**
     * Replace links by the contents of the XML files in the master
     * @param masterXML name of the master file
     * @param styleSheet CSS to be used
     * @return the new master file
     */
    private File preProcessMaster(String masterXML) {

        String filename = new File(masterXML).getName();
        /* Create the output file which will be created by copyconvert.run into the working directory  */
        File masterXMLTransformed = new File(sciDocMain.getOutputDirectory()
                                             + File.separator + filename.substring(0, filename.lastIndexOf(".")) + "-processed.xml");

        CopyConvert copyConvert = new CopyConvert();
        copyConvert.setVerbose(true);
        copyConvert.setPrintFormat(sciDocMain.getFormat().name());
        copyConvert.setLatexConverted(false);

        try {
            copyConvert.run(new File(masterXML), masterXMLTransformed);
        } catch (SAXParseException e) {
            System.err.println(CANNOT_COPY_CONVERT + masterXML + TO_WITH_QUOTES
                               + masterXMLTransformed + COLON_WITH_QUOTES + Helpers.reason(e));
            System.err.println("Line: " + e.getLineNumber());
            System.err.println("Column: " + e.getColumnNumber());
            System.err.println("Public ID: " + e.getPublicId());
            System.err.println("System Id: " + e.getSystemId());
            return null;
        } catch (SAXException e) {
            System.err.println(CANNOT_COPY_CONVERT + masterXML + TO_WITH_QUOTES
                               + masterXMLTransformed + COLON_WITH_QUOTES + Helpers.reason(e));
            return null;

        } catch (IOException e) {
            System.err.println(CANNOT_COPY_CONVERT + masterXML + TO_WITH_QUOTES
                               + masterXMLTransformed + COLON_WITH_QUOTES + Helpers.reason(e));
            return null;
        }

        return masterXMLTransformed;

    }
}
