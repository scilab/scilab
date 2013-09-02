/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.spec;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.logging.Logger;
import java.util.zip.CRC32;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;

/**
 * This class handle xcos format package (eg. ZIP) management
 *
 * You can load/save from/to a file a specific package without storing any data.
 */
public class XcosPackage {
    private static final Logger LOG = Logger.getLogger(XcosPackage.class.getName());
    private static final String MIMETYPE = "mimetype";
    private static final String META_INF_MANIFEST_XML = "META-INF/manifest.xml";

    private static final String VERSION = "0.2";
    private static final String MIME = "application/x-scilab-xcos";
    private static final byte[] MIME_BYTES = MIME.getBytes();

    private static final String INVALID_MIMETYPE = "Invalid mimetype";

    private static final String DICTIONARY_PATH = "dictionary/dictionary.ser";

    /**
     * Specific InputStream implementation to use entry closing instead of
     * stream closing.
     */
    private static class EntryInputStream extends InputStream {
        private final ZipInputStream zin;

        public EntryInputStream(final ZipInputStream zin) {
            this.zin = zin;
        }

        @Override
        public int available() throws IOException {
            return zin.available();
        }

        @Override
        public int read() throws IOException {
            return zin.read();
        }

        @Override
        public int read(byte[] b, int off, int len) throws IOException {
            return zin.read(b, off, len);
        }

        @Override
        public long skip(long n) throws IOException {
            return zin.skip(n);
        }

        @Override
        public void close() throws IOException {
            zin.closeEntry();
        }
    }

    /*
     * XPath expression cache
     */
    private static XPathExpression XPATH_VERSION;

    /*
     * Package level data
     */
    private final File file;
    private Document manifest;

    /**
     * Entries encoder/decoder stored in the encoding order
     */
    private Entry[] availableEntries;

    /*
     * Data to store or load into
     */
    private XcosDiagram content;
    private ScilabList dictionary;

    /*
     * External methods, to save/load a file
     */

    /**
     * Default constructor
     *
     * @param file
     *            the file to use on this package (see {@link #load()} and
     *            {@link #store()})
     * @throws ParserConfigurationException
     *             on invalid DOM engine configuration
     */
    public XcosPackage(final File file) throws ParserConfigurationException {
        this.file = file;

        manifest = ScilabDocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
        final Element root = manifest.createElementNS("urn:scilab:xcos:xmlns:manifest:0.1", "manifest:manifest");
        manifest.appendChild(root);

        // take care: the order is the encoding order
        availableEntries = new Entry[] { new ContentEntry() };
    }

    private boolean hasInvalidManifest() {
        return manifest == null || manifest.getFirstChild() == null || !manifest.getFirstChild().hasChildNodes();
    }

    /**
     * Load the {@link #file} into the internals
     *
     * @throws IOException
     *             on I/O Exception or invalid format
     * @throws SAXException
     *             on SAX error
     * @throws ParserConfigurationException
     *             on invalid DOM engine configuration
     * @throws TransformerException
     */
    public void load() throws IOException, SAXException, ParserConfigurationException, TransformerException {
        if (hasInvalidManifest()) {
            checkHeader();
        }

        final ScilabList dictionary = getDictionary();

        final FileInputStream fis = new FileInputStream(file);
        final ZipInputStream zin = new ZipInputStream(fis);
        // input stream without close operation
        final EntryInputStream ein = new EntryInputStream(zin);

        ZipEntry entry;
        try {
            while ((entry = zin.getNextEntry()) != null) {
                final Node root = manifest.getFirstChild();
                for (Node n = root.getFirstChild(); n != null; n = n.getNextSibling()) {
                    // node precondition
                    if (n.getNodeType() != Node.ELEMENT_NODE) {
                        continue;
                    }
                    if (!n.hasAttributes()) {
                        continue;
                    }

                    final String media = n.getAttributes().getNamedItem("manifest:media-type").getNodeValue();
                    final String path = n.getAttributes().getNamedItem("manifest:full-path").getNodeValue();

                    // path should be the entry one, if not continue
                    if (!path.equals(entry.getName()) || path.equals(DICTIONARY_PATH)) {
                        continue;
                    }

                    // select the right entry decoder
                    for (final Entry e : availableEntries) {
                        if (media.equals(e.getMediaType()) && path.matches(e.getFullPath())) {
                            if (dictionary != null) {
                                ((ContentEntry) e).setDictionary(dictionary);
                            }
                            e.setup(this);
                            e.load(entry, ein);
                            break;
                        }
                    }
                }
            }

        } finally {
            zin.close();
        }
    }

    public ScilabList getDictionary() throws IOException {
        LOG.entering("XcosPackage", "getDictionary");
        final FileInputStream fis = new FileInputStream(file);
        final ZipInputStream zin = new ZipInputStream(fis);

        ZipEntry entry;
        BufferedInputStream bis = new BufferedInputStream(zin);

        try {
            while ((entry = zin.getNextEntry()) != null) {
                final String name = entry.getName();
                if (name.equals(DICTIONARY_PATH)) {
                    DictionaryEntry e = new DictionaryEntry();
                    e.setup(this);
                    e.load(entry, bis);
                    return e.getDictionary();
                }
            }
        } finally {
            bis.close();
            zin.close();

            LOG.exiting("XcosPackage", "getDictionary");
        }

        return null;
    }

    /**
     * Check an xcos file as a ZIP package.
     *
     * @param file
     *            the file to read
     * @throws IOException
     *             on I/O Exception or invalid format
     * @throws TransformerException
     *             on invalid XSLT engine configuration
     */
    public void checkHeader() throws IOException, TransformerException {
        final FileInputStream fis = new FileInputStream(file);
        final ZipInputStream zin = new ZipInputStream(fis);

        ZipEntry entry;
        try {
            while ((entry = zin.getNextEntry()) != null) {
                // extract data
                // open output streams

                final String name = entry.getName();
                if (MIMETYPE.equals(name)) {
                    if (entry.getSize() != MIME_BYTES.length) {
                        throw new IOException(INVALID_MIMETYPE);
                    }

                    final byte[] buf = new byte[MIME_BYTES.length];
                    zin.read(buf);

                    if (!Arrays.equals(MIME_BYTES, buf)) {
                        throw new IOException(INVALID_MIMETYPE);
                    }
                } else if (META_INF_MANIFEST_XML.equals(name)) {
                    final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
                    final Transformer aTransformer = tranFactory.newTransformer();

                    // take care: to avoid closing input stream as the
                    // transformer will close the input stream.
                    final StreamSource src = new StreamSource(new EntryInputStream(zin));
                    final DOMResult result = new DOMResult();
                    aTransformer.transform(src, result);
                    manifest = (Document) result.getNode();
                }
            }
        } finally {
            zin.close();
        }

        if (hasInvalidManifest()) {
            throw new IOException("META-INF/manifest.xml entry not found");
        }
    }

    public void store() throws IOException {
        final FileOutputStream fos = new FileOutputStream(file);
        final ZipOutputStream zout = new ZipOutputStream(fos);

        try {
            // add the header (standard package)
            storeHeader(zout);

            // store the entries in encoding order
            for (final Entry entry : availableEntries) {
                entry.setup(this);
                entry.store(zout);
            }

            // store the manifest file
            storeTrailer(zout);
        } finally {
            zout.close();
        }
    }

    private void storeHeader(final ZipOutputStream zout) throws IOException {
        /*
         * Store header
         */
        final ZipEntry entry = new ZipEntry(MIMETYPE);
        entry.setSize(MIME_BYTES.length);
        final CRC32 crc = new CRC32();
        crc.update(MIME_BYTES);
        entry.setCrc(crc.getValue());
        entry.setMethod(ZipEntry.STORED);
        zout.putNextEntry(entry);
        zout.write(MIME_BYTES);

        /*
         * Add an entry to the manifest
         */
        final Element e = manifest.createElement("manifest:file-entry");
        e.setAttribute("manifest:media-type", MIME);
        e.setAttribute("manifest:version", VERSION);
        e.setAttribute("manifest:full-path", "/");
        manifest.getFirstChild().appendChild(e);
    }

    private void storeTrailer(final ZipOutputStream zout) throws IOException {
        /*
         * Append the entry
         */
        final ZipEntry entry = new ZipEntry(META_INF_MANIFEST_XML);
        zout.putNextEntry(entry);

        /*
         * Store the content
         */
        try {
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();
            aTransformer.setOutputProperty(OutputKeys.INDENT, "yes");
            aTransformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "1");

            final DOMSource src = new DOMSource(manifest);
            final StreamResult result = new StreamResult(zout);
            aTransformer.transform(src, result);
        } catch (TransformerConfigurationException e) {
            throw new IOException(e);
        } catch (TransformerException e) {
            throw new IOException(e);
        }
    }

    /*
     * Getters/setters for data
     */
    public Document getManifest() {
        return manifest;
    }

    public void setContent(XcosDiagram content) {
        this.content = content;
    }

    public XcosDiagram getContent() {
        return content;
    }

    /*
     * Utilities
     */

    /**
     * Get the manifest package version
     *
     * @return the package version or the default version in case of version not
     *         found
     */
    public String getPackageVersion() {
        // cache the xpath expression
        if (XPATH_VERSION == null) {
            final XPathFactory factory = XPathFactory.newInstance();
            final XPath xpath = factory.newXPath();

            try {
                XPATH_VERSION = xpath.compile("//manifest/file-entry[media-type='" + MIME + "']/@version");
            } catch (XPathExpressionException e) {
                e.printStackTrace();
            }
        }

        // evaluate the version
        if (XPATH_VERSION != null) {
            try {
                return XPATH_VERSION.evaluate(manifest, XPathConstants.STRING).toString();
            } catch (XPathExpressionException e) {
                e.printStackTrace();
            }
        }

        // fallback to the default
        return VERSION;
    }
}
