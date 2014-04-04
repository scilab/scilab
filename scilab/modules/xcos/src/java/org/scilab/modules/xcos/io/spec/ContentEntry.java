/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2014 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.spec;

import java.io.IOException;
import java.io.InputStream;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.codec.XcosCodec;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

public class ContentEntry implements Entry {
    private static final Logger LOG = Logger.getLogger(ContentEntry.class.getName());

    private Document manifest;
    private XcosDiagram content;
    private XcosPackage pack;
    private ScilabList dictionary;

    public void setDictionary(ScilabList dictionary) {
        this.dictionary = dictionary;
    }

    @Override
    public String getMediaType() {
        return "text/xml";
    }

    @Override
    public String getFullPath() {
        return "content.xml";
    }

    @Override
    public void setup(XcosPackage p) {
        pack = p;
        manifest = p.getManifest();
        content = p.getContent();
    }

    @Override
    public void load(ZipEntry entry, InputStream stream) throws IOException {
        try {
            final XcosCodec codec = new XcosCodec();
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();

            final StreamSource src = new StreamSource(stream);
            final DOMResult result = new DOMResult(codec.getDocument());

            LOG.entering("Transformer", "transform");
            aTransformer.transform(src, result);
            LOG.exiting("Transformer", "transform");

            LOG.entering("XcosCodec", "decode");
            codec.setElementIdAttributes();
            if (dictionary == null) {
                codec.decode(result.getNode().getFirstChild(), content);
            } else {
                XcosCodec.enableBinarySerialization(dictionary);
                codec.decode(result.getNode().getFirstChild(), content);
                XcosCodec.disableBinarySerialization();
            }
            LOG.exiting("XcosCodec", "decode");

        } catch (TransformerConfigurationException e) {
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        } catch (TransformerException e) {
            e.printStackTrace();
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        }
    }

    @Override
    public void store(ZipOutputStream stream) throws IOException {
        /*
         * Append a ZipEntry
         */
        final ZipEntry entry = new ZipEntry(getFullPath());
        stream.putNextEntry(entry);

        /*
         * Store content
         */
        try {
            final XcosCodec codec = new XcosCodec();
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();

            LOG.entering("XcosCodec", "encode");
            Object lock = XcosCodec.enableBinarySerialization(null);
            final Node doc;
            final ScilabList dictionary;
            synchronized (lock) {
                doc = codec.encode(content);
                dictionary = XcosCodec.disableBinarySerialization();
            }
            LOG.exiting("XcosCodec", "encode");

            final DOMSource src = new DOMSource(doc);
            final StreamResult result = new StreamResult(stream);

            LOG.entering("Transformer", "transform");
            aTransformer.transform(src, result);
            LOG.exiting("Transformer", "transform");

            DictionaryEntry dictEntry = new DictionaryEntry(dictionary);
            dictEntry.setup(pack);
            dictEntry.store(stream);

            /*
             * Add an entry to the manifest file
             */
            final Element e = manifest.createElement("manifest:file-entry");
            e.setAttribute("manifest:media-type", getMediaType());
            e.setAttribute("manifest:full-path", getFullPath());
            manifest.getFirstChild().appendChild(e);
        } catch (TransformerConfigurationException e) {
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        } catch (TransformerException e) {
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        }
    }
}