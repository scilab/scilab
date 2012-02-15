/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.util.Map;
import java.util.logging.Logger;

import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;

/**
 * Scilab data direct access.
 */
public class ScilabDirectHandler implements Handler {
    public static final String CONTEXT = "context";
    public static final String SCS_M = "scs_m";
    public static final String BLK = "blk";

    private static final Logger LOG = Logger.getLogger(ScilabDirectHandler.class.getPackage().getName());

    public ScilabDirectHandler() {
    }

    @Override
    public BasicBlock readBlock() throws ScicosFormatException {
        return readBlock(null);
    }

    @Override
    public synchronized BasicBlock readBlock(final BasicBlock instance) throws ScicosFormatException {
        throw new UnsupportedOperationException();
        // LOG.entering("ScilabDirectHandler", "readDiagram");
        // final BlockElement element = new BlockElement();
        //
        // BasicBlock block;
        // if (instance == null) {
        // block = new BasicBlock();
        // } else {
        // block = instance;
        // }
        //
        // LOG.finer("object allocated");
        //
        // ScilabType data;
        // try {
        // data = Scilab.getInCurrentScilabSession(BLK);
        // } catch (JavasciException e) {
        // return null;
        // }
        //
        // LOG.finer("data available");
        //
        // block = element.decode(data, block);
        // final StyleMap style = new
        // StyleMap(block.getInterfaceFunctionName());
        // style.putAll(block.getStyle());
        //
        // block.setStyle(style.toString());
        //
        // LOG.exiting("ScilabDirectHandler", "readBlock");
        // return block;
    }

    @Override
    public synchronized Map<String, String> readContext() {
        throw new UnsupportedOperationException();
        // LOG.entering("ScilabDirectHandler", "readContext");
        // final ScilabList list;
        // final Map<String, String> result = new LinkedHashMap<String,
        // String>();
        //
        // final ScilabType data;
        // try {
        // data = Scilab.getInCurrentScilabSession(CONTEXT);
        // } catch (JavasciException e) {
        // throw new RuntimeException(e);
        // }
        // if (data instanceof ScilabList) {
        // list = (ScilabList) data;
        // LOG.finer("data available");
        // } else {
        // list = new ScilabList();
        // LOG.finer("data unavailable");
        // }
        //
        // // We are starting at 2 because a struct is composed of
        // // - the fields names (ScilabString)
        // // - the dimension
        // // - variables values...
        // for (int index = 2; index < list.size(); index++) {
        // String key = ((ScilabString) list.get(0)).getData()[0][index];
        // String value = list.get(index).toString();
        //
        // result.put(key, value);
        // }
        //
        // LOG.exiting("ScilabDirectHandler", "readContext");
        // return result;
    }

    @Override
    public XcosDiagram readDiagram() throws VersionMismatchException {
        return readDiagram(null);
    }

    @Override
    public synchronized XcosDiagram readDiagram(final XcosDiagram instance) {
        throw new UnsupportedOperationException();
        // LOG.entering("ScilabDirectHandler", "readDiagram");
        // final DiagramElement element = new DiagramElement();
        //
        // XcosDiagram diagram;
        // if (instance == null) {
        // diagram = new XcosDiagram();
        // } else {
        // diagram = instance;
        // }
        //
        // LOG.finer("object allocated");
        //
        // ScilabType data;
        // try {
        // data = Scilab.getInCurrentScilabSession(SCS_M);
        // } catch (JavasciException e) {
        // return null;
        // }
        //
        // LOG.finer("data available");
        //
        // try {
        // diagram = element.decode(data, diagram);
        // } catch (ScicosFormatException e) {
        // if (e instanceof VersionMismatchException) {
        // /*
        // * On version mismatch alert the user but the current instance
        // * contains the partially decoded data so continue.
        // */
        // diagram.error(XcosMessages.UNKNOW_VERSION +
        // ((VersionMismatchException) e).getWrongVersion() + "\n" +
        // XcosMessages.TRY_TO_CONTINUE);
        // } else {
        // // rethrow
        // throw new RuntimeException(e);
        // }
        // } catch (Throwable t) {
        // t.printStackTrace();
        // } finally {
        // diagram.getModel().endUpdate();
        // }
        //
        // LOG.exiting("ScilabDirectHandler", "readDiagram");
        // return diagram;
    }

    @Override
    public void writeBlock(final BasicBlock block) {
        LOG.entering("ScilabDirectHandler", "writeBlock");

        final BlockElement element = new BlockElement();
        final ScilabType data = element.encode(block, null);

        LOG.finer("encoding done");

        try {
            Scilab.putInCurrentScilabSession(BLK, data);
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }

        LOG.exiting("ScilabDirectHandler", "writeBlock");
    }

    @Override
    public void writeContext(final String[] context) {
        LOG.entering("ScilabDirectHandler", "writeContext");

        try {
            Scilab.putInCurrentScilabSession(CONTEXT, new ScilabString(context));
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }

        LOG.exiting("ScilabDirectHandler", "writeContext");
    }

    @Override
    public void writeDiagram(final XcosDiagram diagram) {
        LOG.entering("ScilabDirectHandler", "writeDiagram");

        final DiagramElement element = new DiagramElement();
        final ScilabType data = element.encode(diagram, null);

        LOG.finer("encoding done");

        try {
            Scilab.putInCurrentScilabSession(SCS_M, data);
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }

        LOG.exiting("ScilabDirectHandler", "writeDiagram");
    }
}
