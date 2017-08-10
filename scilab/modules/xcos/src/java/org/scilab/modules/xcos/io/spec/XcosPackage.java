/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.io.spec;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.Charset;
import java.util.Arrays;
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
import org.scilab.modules.commons.ScilabCommons;

import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.commons.xml.ScilabXPathFactory;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;

/**
 * This class handle xcos format package (eg. ZIP) management
 *
 * You can load/save from/to a file a specific package without storing any data.
 */
public class XcosPackage {
    private static final String MIMETYPE = "mimetype";
    private static final String META_INF_MANIFEST_XML = "META-INF/manifest.xml";

    private static final String VERSION = "0.2";
    private static final String MIME = "application/x-scilab-xcos";
    private static final byte[] MIME_BYTES = MIME.getBytes();

    private static final String INVALID_MIMETYPE = "Invalid mimetype";

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
    private final long time;

    /**
     * Entries encoder/decoder stored in the encoding order
     * <p>
     * take care:
     * <ul>
     * <li>the order is the encoding order (from start to end)
     * <li>decoding will be performed from the end to the start
     */
    private final Entry[] availableEntries = new Entry[] { new ContentEntry(), new DictionaryEntry() };

    /*
     * Data to store or load into
     */
    private XcosDiagram content;
    private final ScilabList dictionary;

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
        this.dictionary = new ScilabList();

        manifest = ScilabDocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
        final Element root = manifest.createElementNS("urn:scilab:xcos:xmlns:manifest:0.1", "manifest:manifest");
        manifest.appendChild(root);

        time = ScilabCommons.getScilabVersionTimestamp();
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

        // Decode using the specified order (from end to start)
        for (int i = availableEntries.length - 1; 0 <= i;) {
            Entry e = availableEntries[i];

            // open the file on each entry to manage non well ordered (but still
            // valid) zip files
            final FileInputStream fis = new FileInputStream(file);
            try (ZipInputStream zin = new ZipInputStream(fis)) {

                ZipEntry entry;
                while ((entry = zin.getNextEntry()) != null) {
                    final String path = entry.getName();
                    if (path.equals(e.getFullPath())) {
                        // decode the current entry
                        e.setup(this);
                        e.load(entry, new EntryInputStream(zin));

                        // try to decode the next entry (for well ordered zip,
                        // the more common case)
                        i--;
                        if (0 <= i) {
                            e = availableEntries[i];
                        }
                    }
                }

            }
        }
    }

    /**
     * Check an xcos file as a ZIP package.
     *
     * @throws IOException
     *             on I/O Exception or invalid format
     * @throws TransformerException
     *             on invalid XSLT engine configuration
     */
    public void checkHeader() throws IOException, TransformerException {
        final FileInputStream fis = new FileInputStream(file);
        try (ZipInputStream zin = new ZipInputStream(fis)) {

            ZipEntry entry;
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
        }

        if (hasInvalidManifest()) {
            throw new IOException("META-INF/manifest.xml entry not found");
        }
    }

    public void store() throws IOException {
        final FileOutputStream fos = new FileOutputStream(file);
        try (ZipOutputStream zout = new ZipOutputStream(fos,  Charset.forName("UTF-8"))) {

            // add the header (standard package)
            storeHeader(zout);
            zout.flush();

            // store the entries in encoding order
            for (final Entry entry : availableEntries) {
                entry.setup(this);

                final ZipEntry zentry = new ZipEntry(entry.getFullPath());
                zentry.setTime(getTime());
                zout.putNextEntry(zentry);
                zout.flush();

                entry.store(zout);
                zout.flush();
            }

            // store the manifest file
            storeTrailer(zout);
            zout.flush();

        } catch (Exception e) {
            e.printStackTrace();
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
        entry.setTime(getTime());
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
        entry.setTime(getTime());
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

    public ScilabList getDictionary() {
        return dictionary;
    }

    public long getTime() {
        return time;
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
            final XPathFactory factory = ScilabXPathFactory.newInstance();
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
