/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.logging.Logger;

import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.stream.StreamSource;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosCodec;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;

/**
 * All the filetype recognized by Xcos.
 */
public enum XcosFileType {
    /**
     * Represent the Xcos XML format.
     */
    XCOS("xcos", XcosMessages.FILE_XCOS) {
        @Override
        public String exportToHdf5(String file) throws TransformerException, IOException {
            XcosDiagram diag = new XcosDiagram();
            load(file, diag);

            final String exportedFile = FileUtils.createTempFile();
            new H5RWHandler(exportedFile).writeDiagram(diag);
            return exportedFile;
        }

        @Override
        public void load(String file, XcosDiagram into) throws TransformerException {
            final XcosCodec codec = new XcosCodec();
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();

            final StreamSource src = new StreamSource(file);
            final DOMResult result = new DOMResult();
            aTransformer.transform(src, result);

            codec.decode(result.getNode().getFirstChild(), into);
        }
    },
    /**
     * Represent the old Scicos text format.
     */
    COSF("cosf", XcosMessages.FILE_COSF) {
        @Override
        public String exportToHdf5(String arg0) {
            return loadScicosDiagram(arg0);
        }

        @Override
        public void load(String file, XcosDiagram into) throws Exception {
            final String h5File = exportToHdf5(file);
            HDF5.load(h5File, into);
        }
    },
    /**
     * Represent the old Scicos binary format.
     */
    COS("cos", XcosMessages.FILE_COS) {
        @Override
        public String exportToHdf5(String arg0) {
            return loadScicosDiagram(arg0);
        }

        @Override
        public void load(String file, XcosDiagram into) throws Exception {
            final String h5File = exportToHdf5(file);
            HDF5.load(h5File, into);
        }
    },
    /**
     * Represent the Scilab I/O format.
     */
    HDF5("h5", XcosMessages.FILE_HDF5) {
        @Override
        public String exportToHdf5(String arg0) {
            return arg0;
        }

        @Override
        public void load(String file, XcosDiagram into) {
            new H5RWHandler(file).readDiagram(into);
        }
    };

    private static final String BEFORE_EXT = " (*.";
    private static final String AFTER_EXT = ")";

    private String extension;
    private String description;

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

        /* Validate xml header */
        if (retValue == XCOS) {
            retValue = checkXmlHeader(theFile);
        }

        return retValue;
    }

    /**
     * Check the XML header
     *
     * @param theFile
     *            the file to check
     * @return the found file type
     */
    private static XcosFileType checkXmlHeader(String theFile) {
        XcosFileType retValue = null;

        final byte[] xmlMagic = "<?xml".getBytes();
        final byte[] readMagic = new byte[xmlMagic.length];

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
                    Logger.getLogger(XcosFileType.class.getName()).severe(e.toString());
                }
            }
        }
        return retValue;
    }

    /**
     * @return the Xcos default filetype
     */
    public static XcosFileType getDefault() {
        return XcosFileType.XCOS;
    }

    /**
     * @return the Scilab default filetype
     */
    public static XcosFileType getScilabFileType() {
        return XcosFileType.HDF5;
    }

    /**
     * Convert the file passed as an argument to Hdf5.
     *
     * @param file
     *            The file to convert
     * @return The created file
     * @throws Exception
     *             in case of problem
     */
    public abstract String exportToHdf5(String file) throws Exception;

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
    public abstract void load(final String file, final XcosDiagram into) throws Exception;

    /**
     * @return the valid file filters
     */
    public static FileFilter[] getValidFilters() {
        final FileFilter[] filters = new FileFilter[values().length];

        for (int i = 0; i < filters.length; i++) {
            final XcosFileType type = values()[i];
            filters[i] = new FileNameExtensionFilter(type.getDescription(), type.getExtension());
        }
        return filters;
    }

    /**
     * Get a valid file mask (useable by file selector)
     *
     * @return A valid file mask
     */
    public static String[] getValidFileMask() {
        String[] result = new String[XcosFileType.values().length - 1];

        for (int i = 0; i < result.length; i++) {
            result[i] = XcosFileType.values()[i].getFileMask();
        }

        return result;
    }

    /**
     * Get a valid file description (useable by file selector)
     *
     * @return A valid file mask
     */
    public static String[] getValidFileDescription() {
        String[] result = new String[XcosFileType.values().length - 1];

        for (int i = 0; i < result.length; i++) {
            result[i] = XcosFileType.values()[i].getDescription() + BEFORE_EXT + XcosFileType.values()[i].getExtension() + AFTER_EXT;
        }

        return result;
    }

    /**
     * Convert a Scicos diagram (scs_m scilab script) to an hdf5 file
     *
     * @param filename
     *            The file to execute in scilab.
     * @return The exported data in hdf5.
     */
    public static String loadScicosDiagram(String filename) {
        String tempOutput = null;
        try {
            tempOutput = FileUtils.createTempFile();

            StringBuilder cmd = new StringBuilder();
            cmd.append("scs_m = importScicosDiagram(\"");
            cmd.append(filename);
            cmd.append("\");");
            cmd.append("result = export_to_hdf5(\"");
            cmd.append(tempOutput);
            cmd.append("\", \"scs_m\");");

            cmd.append("if result <> %t then deletefile(\"");
            cmd.append(tempOutput);
            cmd.append("\"); end; ");

            try {
                ScilabInterpreterManagement.synchronousScilabExec(cmd.toString());
            } catch (InterpreterException e) {
                e.printStackTrace();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return tempOutput;
    }
}
