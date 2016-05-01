/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2014 - Scilab Enterprises - Clement DAVID
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

import java.io.IOException;
import java.io.InputStream;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import javax.xml.stream.XMLOutputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;
import javax.xml.transform.stream.StreamResult;
import org.scilab.modules.commons.xml.ScilabXMLOutputFactory;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.sax.XcosSAXHandler;
import org.scilab.modules.xcos.io.writer.XcosWriter;
import org.w3c.dom.Element;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;

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
            XcosSAXHandler handler = new XcosSAXHandler(content, pack.getDictionary());
            XMLReader reader = XMLReaderFactory.createXMLReader();
            reader.setContentHandler(handler);
            reader.setErrorHandler(handler);

            LOG.entering("XMLReader", "parse");
            reader.parse(new InputSource(stream));
            LOG.exiting("XMLReader", "parse");
        } catch (SAXException e) {
            e.printStackTrace();
            throw new RuntimeException(e);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public void store(ZipOutputStream stream) throws IOException {
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