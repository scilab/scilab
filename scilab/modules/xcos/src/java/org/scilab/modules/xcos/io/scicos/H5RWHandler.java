/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.io.File;
import java.util.LinkedHashMap;
import java.util.Map;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement useful methods to easily import or export Scicos data.
 */
// CSOFF: ClassDataAbstractionCoupling
public class H5RWHandler implements Handler {
    private static final String CONTEXT = "context";
    private static final String SCS_M = "scs_m";
    private static final Log LOG = LogFactory.getLog(H5RWHandler.class);

    private final String h5File;

    /**
     * Constructor a new instance with a file.
     *
     * @param hdf5file
     *            the file path.
     */
    public H5RWHandler(File hdf5file) {
        h5File = hdf5file.getAbsolutePath();
    }

    /**
     * Construct a new instance with the file path.
     *
     * @param hdf5file
     *            the file path.
     */
    public H5RWHandler(String hdf5file) {
        h5File = hdf5file;
    }

    /*
     * Read methods
     */

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.io.scicos.RWHandler#readBlock()
     */
    @Override
    public BasicBlock readBlock() throws ScicosFormatException {
        return readBlock(null);
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.io.scicos.RWHandler#readBlock(org.scilab.modules.xcos.block.BasicBlock)
     */
    @Override
    public BasicBlock readBlock(BasicBlock into) throws ScicosFormatException {
        final ScilabMList data = new ScilabMList();
        final BlockElement element = new BlockElement();
        BasicBlock instance;

        if (LOG.isTraceEnabled()) {
            LOG.trace("Reading block from " + h5File);
        }

        try {
            int fileId = H5Read.openFile(h5File);
            H5Read.readDataFromFile(fileId, data);
            H5Read.closeFile(fileId);

            instance = element.decode(data, into);
            StyleMap style = new StyleMap(instance.getInterfaceFunctionName());
            style.putAll(instance.getStyle());

            instance.setStyle(style.toString());

        } catch (HDF5Exception e) {
            LOG.error(e);
            instance = null;
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of reading block from " + h5File);
        }

        return instance;
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.io.scicos.RWHandler#readContext()
     */
    @Override
    public Map<String, String> readContext() {
        final ScilabList list = new ScilabList();
        final Map<String, String> result = new LinkedHashMap<String, String>();

        if (LOG.isTraceEnabled()) {
            LOG.trace("Reading context from " + h5File);
        }

        try {
            int handle = H5Read.openFile(h5File);
            if (handle >= 0) {
                H5Read.readDataFromFile(handle, list);
            }
        } catch (HDF5Exception e) {
            LOG.error(e);
            return result;
        }

        // We are starting at 2 because a struct is composed of
        // - the fields names (ScilabString)
        // - the dimension
        // - variables values...
        for (int index = 2; index < list.size(); index++) {
            String key = ((ScilabString) list.get(0)).getData()[0][index];
            String value = list.get(index).toString();

            result.put(key, value);
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of reading context from " + h5File);
        }

        return result;
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.io.scicos.RWHandler#readDiagram()
     */
    @Override
    public XcosDiagram readDiagram() throws VersionMismatchException {
        return readDiagram(null);
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.io.scicos.RWHandler#readDiagram(org.scilab.modules.xcos.graph.XcosDiagram)
     */
    @Override
    public XcosDiagram readDiagram(XcosDiagram instance) {
        final ScilabMList data = new ScilabMList();
        final DiagramElement element = new DiagramElement();

        XcosDiagram diagram;
        if (instance == null) {
            diagram = new XcosDiagram();
        } else {
            diagram = instance;
        }

        diagram.getModel().beginUpdate();

        if (LOG.isTraceEnabled()) {
            LOG.trace("Reading diagram from " + h5File);
        }

        try {
            int fileId = H5Read.openFile(h5File);

            H5Read.readDataFromFile(fileId, data);
            H5Read.closeFile(fileId);
            element.decode(data, diagram);
        } catch (ScicosFormatException e) {
            if (e instanceof VersionMismatchException) {
                /*
                 * On version mismatch alert the user but the current instance
                 * contains the partially decoded data so continue.
                 */
                diagram.error(XcosMessages.UNKNOW_VERSION
                              + ((VersionMismatchException) e).getWrongVersion()
                              + "\n" + XcosMessages.TRY_TO_CONTINUE);
            } else {
                // rethrow
                throw new RuntimeException(e);
            }
        } catch (HDF5Exception e) {
            throw new RuntimeException(e);
        } catch (Throwable t) {
            t.printStackTrace();
        } finally {
            diagram.getModel().endUpdate();
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of reading diagram from " + h5File);
        }

        return diagram;
    }

    /*
     * Write methods
     */

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.io.scicos.RWHandler#writeBlock(org.scilab.modules.xcos.block.BasicBlock)
     */
    @Override
    public void writeBlock(BasicBlock block) {
        final BlockElement element = new BlockElement();
        final ScilabType data = element.encode(block, null);

        if (LOG.isTraceEnabled()) {
            LOG.trace("Writing block to " + h5File);
        }

        try {
            int fileId = H5Write.createFile(h5File);

            H5Write.writeInDataSet(fileId, SCS_M, data);

            H5Write.closeFile(fileId);
        } catch (HDF5Exception e) {
            LOG.error(e);
        } catch (java.lang.NullPointerException e) {
            LOG.error(e);
            LOG.debug(data);
        } catch (java.lang.IndexOutOfBoundsException e) {
            LOG.error(e);
            LOG.debug(data);
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of writing block to " + h5File);
        }
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.io.scicos.RWHandler#writeContext(java.lang.String[])
     */
    @Override
    public void writeContext(String[] context) {
        final ScilabString string = new ScilabString(context);

        if (LOG.isTraceEnabled()) {
            LOG.trace("Writing context to " + h5File);
        }

        try {
            int fileId = H5Write.createFile(h5File);

            H5Write.writeInDataSet(fileId, CONTEXT, string);

            H5Write.closeFile(fileId);
        } catch (HDF5Exception e) {
            LOG.error(e);
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of writing context to " + h5File);
        }
    }

    /* (non-Javadoc)
     * @see org.scilab.modules.xcos.io.scicos.RWHandler#writeDiagram(org.scilab.modules.xcos.graph.XcosDiagram)
     */
    @Override
    public void writeDiagram(XcosDiagram diagram) {
        final DiagramElement element = new DiagramElement();
        final ScilabType data = element.encode(diagram, null);

        if (LOG.isTraceEnabled()) {
            LOG.trace("Writing diagram to " + h5File);
        }

        try {
            int fileId = H5Write.createFile(h5File);

            H5Write.writeInDataSet(fileId, SCS_M, data);

            H5Write.closeFile(fileId);
        } catch (HDF5Exception e) {
            // important error which need a backtrace.
            LOG.error(e);
            e.printStackTrace();
        } catch (java.lang.NullPointerException e) {
            LOG.error(e);
            LOG.debug(data);
        } catch (java.lang.IndexOutOfBoundsException e) {
            LOG.error(e);
            LOG.debug(data);
        }

        if (LOG.isTraceEnabled()) {
            LOG.trace("End of writing diagram to " + h5File);
        }
    }
}
// CSON: ClassDataAbstractionCoupling
