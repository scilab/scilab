/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2014 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.io;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.Collection;
import java.util.EnumSet;
import java.util.Iterator;
import java.util.Set;
import java.util.logging.Logger;

import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.stream.XMLOutputFactory;
import javax.xml.stream.XMLStreamWriter;
import javax.xml.transform.TransformerException;

import org.scilab.modules.commons.xml.ScilabXMLOutputFactory;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.JavaXMIResource;
import org.scilab.modules.xcos.View;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.io.sax.XcosSAXHandler;
import org.scilab.modules.xcos.io.spec.XcosPackage;
import org.scilab.modules.xcos.io.writer.IndentingXMLStreamWriter;
import org.scilab.modules.xcos.io.writer.XcosWriter;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;

/**
 * All the filetype recognized by Xcos.
 */
public enum XcosFileType {
    /**
     * Represent the Xcos (a la ODT) format.
     */
    ZCOS("zcos", XcosMessages.FILE_ZCOS) {
        @Override
        public void load(String file, XcosDiagram into)
        throws TransformerException, IOException, SAXException,
            ParserConfigurationException {
            LOG.entering("XcosFileType.ZCOS", "load");

            View xcosView = JavaController.lookup_view(Xcos.class.getName());
            try {
                JavaController.unregister_view(xcosView);

                XcosPackage p = new XcosPackage(new File(file));
                p.setContent(into);
                p.load();
            } finally {
                JavaController.register_view(Xcos.class.getName(), xcosView);
            }

            LOG.exiting("XcosFileType.ZCOS", "load");
        }

        @Override
        public void save(String file, XcosDiagram from) throws Exception {
            LOG.entering("XcosFileType.ZCOS", "save");

            XcosPackage p = new XcosPackage(new File(file));
            p.setContent(from);
            p.store();

            LOG.exiting("XcosFileType.ZCOS", "save");
        }
    },
    /**
     * Represent the Xcos XML flat format.
     */
    XCOS("xcos", XcosMessages.FILE_XCOS) {
        @Override
        public void load(String file, XcosDiagram into)
        throws IOException {
            View xcosView = JavaController.lookup_view(Xcos.class.getName());
            try {
                JavaController.unregister_view(xcosView);

                XcosSAXHandler handler = new XcosSAXHandler(into, null);
                XMLReader reader = XMLReaderFactory.createXMLReader();
                reader.setContentHandler(handler);
                reader.setErrorHandler(handler);

                LOG.entering("XMLReader", "parse");
                reader.parse(new InputSource(file));
                LOG.exiting("XMLReader", "parse");
            } catch (SAXException e) {
                e.printStackTrace();
                throw new RuntimeException(e);
            } catch (Exception e) {
                e.printStackTrace();
                throw e;
            } finally {
                JavaController.register_view(Xcos.class.getName(), xcosView);
            }
        }

        @Override
        public void save(String file, XcosDiagram from) throws Exception {
            final OutputStream result = new FileOutputStream(file);

            final XMLOutputFactory factory = ScilabXMLOutputFactory.newInstance();
            final XMLStreamWriter writer = factory.createXMLStreamWriter(result, "UTF-8");
            try {
                LOG.entering("XMLStreamWriter", "write");
                new XcosWriter(null, new IndentingXMLStreamWriter(writer)).write(from.getUID(), from.getKind());
                LOG.exiting("XMLStreamWriter", "write");
            } finally {
                writer.close();
            }
        }
    },
    /**
     * Represent an XMI (compatible with Eclipse EMF) serialization of the model
     */
    XMI("xmi", XcosMessages.FILE_XMI) {

        @Override
        public void load(String file, XcosDiagram into) throws Exception {
            JavaXMIResource.load(file, into.getUID());
        }

        @Override
        public void save(String file, XcosDiagram from) throws Exception {
            JavaXMIResource.save(file, from.getUID());
        }
    },
    /**
     * Represent the old Scicos text format.
     */
    COSF("cosf", XcosMessages.FILE_COSF) {
        @Override
        public void load(String file, XcosDiagram into) throws Exception {
            XcosDiagram diagram = XcosCellFactory.createDiagramFromCOSF(new JavaController(), file);
            into.addCell(diagram.getDefaultParent(), into.getDefaultParent());
        }

        @Override
        public void save(String file, XcosDiagram from) throws Exception {
            throw new UnsupportedOperationException();
        }
    };

    private static final String BEFORE_EXT = " (*.";
    private static final String AFTER_EXT = ")";
    private static final Logger LOG = Logger.getLogger(XcosFileType.class
                                      .getName());

    private final String extension;
    private final String description;

    /**
     * Default constructor
     *
     * @param extension
     *            file extension (without the dot)
     * @param description
     *            file description
     */
    XcosFileType(String extension, String description) {
        this.extension = extension;
        this.description = description + BEFORE_EXT + extension + AFTER_EXT;
    }

    /**
     * @return the extension prefixed with a dot
     */
    public String getDottedExtension() {
        return "." + extension;
    }

    /**
     * @return the raw extension
     */
    public String getExtension() {
        return extension;
    }

    /**
     * @return the mask of this file
     */
    public String getFileMask() {
        return "*." + getExtension();
    }

    /**
     * @return the file description
     */
    public String getDescription() {
        return description;
    }

    /**
     * Find a filetype by the filename extension
     *
     * @param theFile
     *            Current file
     * @return The determined filetype
     */
    public static XcosFileType findFileType(String theFile) {
        int dotPos = theFile.lastIndexOf('.');
        String extension = "";
        XcosFileType retValue = null;

        if (dotPos > 0 && dotPos <= theFile.length() - 2) {
            extension = theFile.substring(dotPos + 1);
        }

        for (XcosFileType currentFileType : XcosFileType.values()) {
            if (extension.compareToIgnoreCase(currentFileType.extension) == 0) {
                retValue = currentFileType;
                break;
            }
        }

        return retValue;
    }

    /**
     * Find a filetype by the filename extension
     *
     * @param f
     *            Current file
     * @return The determined filetype
     */
    public static XcosFileType findFileType(File f) {
        XcosFileType retValue = findFileType(f.getName());

        /* Validate xml header */
        if (f.exists() && (retValue == XCOS || retValue == ZCOS)) {
            retValue = checkHeader(f);
        }

        return retValue;
    }

    /**
     * Find a filetype by a file filter
     *
     * @param filter
     *            Current filter
     * @return The determined filetype
     */
    public static XcosFileType findFileType(FileFilter filter) {
        final FileFilter[] filters = getSavingFilters();

        int index = 0;
        for (FileFilter fileFilter : filters) {
            if (fileFilter.getDescription().equals(filter.getDescription())) {
                break;
            }

            index++;
        }

        // the first filter is the "All supported file type", start from -1
        if (index > 0) {
            return XcosFileType.values()[index - 1];
        } else {
            return null;
        }
    }

    /**
     * Check the Xcos file header
     *
     * @param theFile
     *            the file to check
     * @return the found file type
     */
    private static XcosFileType checkHeader(final File theFile) {
        XcosFileType retValue = null;

        final byte[] xmlMagic = "<?xml".getBytes();
        final byte[] readMagic = new byte[xmlMagic.length];

        /*
         * Check if the file is an xml one
         */
        FileInputStream stream = null;
        try {
            stream = new FileInputStream(theFile);
            int length;
            length = stream.read(readMagic);
            if (length == xmlMagic.length && Arrays.equals(xmlMagic, readMagic)) {
                retValue = XCOS;
            }
        } catch (IOException e) {
            retValue = null;
        } finally {
            if (stream != null) {
                try {
                    stream.close();
                } catch (IOException e) {
                    Logger.getLogger(XcosFileType.class.getName()).severe(
                        e.toString());
                }
            }
        }

        /*
         * Check if the file is a valid zip file
         */
        if (retValue == null) {
            try {
                new XcosPackage(theFile).checkHeader();
                retValue = ZCOS;
            } catch (IOException | ParserConfigurationException | TransformerException e) {
            }
        }

        return retValue;
    }

    /**
     * Load a file into an XcosDiagram instance
     *
     * @param file
     *            the file to load
     * @param into
     *            the diagram instance to fill
     * @throws Exception
     *             in case of problem
     */
    public abstract void load(final String file, final XcosDiagram into)
    throws Exception;

    /**
     * Save to a file the XcosDiagram instance
     *
     * @param file
     *            the file to save to
     * @param from
     *            the diagram instance to save
     * @throws Exception
     *             in case of problem
     */
    public abstract void save(final String file, final XcosDiagram from)
    throws Exception;

    /**
     * @return the file filters which can be used to load a file
     */
    public static FileFilter[] getLoadingFilters() {
        Collection<XcosFileType> used = EnumSet.allOf(XcosFileType.class);
        Iterator<XcosFileType> it = used.iterator();

        final FileFilter[] filters = new FileFilter[used.size() + 1];

        final String[] extensions = new String[used.size()];
        final String[] descriptions = new String[used.size()];
        for (int i = 0; i < extensions.length; i++) {
            final XcosFileType type = it.next();

            descriptions[i] = type.getDescription();
            extensions[i] = type.getExtension();
        }

        /*
         * One file filter for all valid extensions
         */
        filters[0] = new FileNameExtensionFilter(
            XcosMessages.ALL_SUPPORTED_FORMATS, extensions);

        /*
         * Then one file filter per enum value.
         */
        for (int i = 0; i < descriptions.length; i++) {
            filters[i + 1] = new FileNameExtensionFilter(descriptions[i],
                    extensions[i]);
        }

        return filters;
    }

    /**
     * @return the file filters which can be used to save a file
     */
    public static FileFilter[] getSavingFilters() {
        Collection<XcosFileType> used = getAvailableSaveFormats();
        Iterator<XcosFileType> it = used.iterator();

        final FileFilter[] filters = new FileFilter[used.size() + 1];

        final String[] extensions = new String[used.size()];
        final String[] descriptions = new String[used.size()];
        for (int i = 0; i < extensions.length; i++) {
            final XcosFileType type = it.next();

            descriptions[i] = type.getDescription();
            extensions[i] = type.getExtension();
        }

        /*
         * One file filter for all valid extensions
         */
        filters[0] = new FileNameExtensionFilter(
            XcosMessages.ALL_SUPPORTED_FORMATS, extensions);

        /*
         * Then one file filter per enum value.
         */
        for (int i = 0; i < descriptions.length; i++) {
            filters[i + 1] = new FileNameExtensionFilter(descriptions[i],
                    extensions[i]);
        }

        return filters;
    }

    /**
     * @return the possible file format
     */
    public static Set<XcosFileType> getAvailableSaveFormats() {
        final Set<XcosFileType> values = EnumSet.noneOf(XcosFileType.class);
        values.add(XcosFileType.XCOS);
        values.add(XcosFileType.ZCOS);
        values.add(XcosFileType.XMI);
        return values;
    }
}
