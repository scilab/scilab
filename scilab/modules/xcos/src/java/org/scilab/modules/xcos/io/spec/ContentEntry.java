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

import javax.xml.stream.XMLOutputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.sax.SAXResult;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.commons.xml.ScilabXMLOutputFactory;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.sax.XcosSAXHandler;
import org.scilab.modules.xcos.io.writer.XcosWriter;
import org.w3c.dom.Element;

public class ContentEntry implements Entry {
    private static final Logger LOG = Logger.getLogger(ContentEntry.class.getName());

    private XcosDiagram content;
    private XcosPackage pack;

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
        content = p.getContent();
    }

    @Override
    public void load(ZipEntry entry, InputStream stream) throws IOException {
        try {
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();

            final StreamSource src = new StreamSource(stream);
            final SAXResult result = new SAXResult(new XcosSAXHandler(content, pack.getDictionary()));

            LOG.entering("Transformer", "transform");
            aTransformer.transform(src, result);
            LOG.exiting("Transformer", "transform");

        } catch (TransformerConfigurationException e) {
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        } catch (TransformerException e) {
            e.printStackTrace();
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        } catch (Exception e) {
            e.printStackTrace();
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
            final XMLOutputFactory factory = ScilabXMLOutputFactory.newInstance();
            final StreamResult result = new StreamResult(stream);
            final XMLStreamWriter writer = factory.createXMLStreamWriter(result);

            LOG.entering("XMLStreamWriter", "write");
            new XcosWriter(pack.getDictionary(), writer).write(content.getUID(), content.getKind());
            writer.close();
            LOG.exiting("XMLStreamWriter", "write");

            /*
             * Add an entry to the manifest file
             */
            final Element e = pack.getManifest().createElement("manifest:file-entry");
            e.setAttribute("manifest:media-type", getMediaType());
            e.setAttribute("manifest:full-path", getFullPath());
            pack.getManifest().getFirstChild().appendChild(e);
        } catch (XMLStreamException e) {
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessage());
        } finally {
            stream.closeEntry();
        }
    }
}