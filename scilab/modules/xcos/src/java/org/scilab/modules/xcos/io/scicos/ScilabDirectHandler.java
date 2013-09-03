/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.logging.Logger;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Scilab data direct access.
 */
public class ScilabDirectHandler implements Handler {
    /**
     * Context Scilab variable name
     */
    public static final String CONTEXT = "context";
    /**
     * Diagram Scilab variable name
     */
    public static final String SCS_M = "scs_m";
    /**
     * Block Scilab variable name
     */
    public static final String BLK = "blk";

    private static final Logger LOG = Logger.getLogger(ScilabDirectHandler.class.getPackage().getName());
    private static final ScilabDirectHandler INSTANCE = new ScilabDirectHandler();

    private final Semaphore lock = new Semaphore(1, true);

    private ScilabDirectHandler() {
    }

    /*
     * Lock management to avoid multiple actions
     */

    /**
     * Get the current instance of a ScilabDirectHandler.
     *
     * Please note that after calling {@link #acquire()} and performing action,
     * you should release the instance using {@link #release()}.
     *
     * <p>
     * It is recommended practice to <em>always</em> immediately follow a call
     * to {@code getInstance()} with a {@code try} block, most typically in a
     * before/after construction such as:
     *
     * <pre>
     * class X {
     *
     *     // ...
     *
     *     public void m() {
     *         final ScilabDirectHandler handler = ScilabDirectHandler.getInstance();
     *         try {
     *             // ... method body
     *         } finally {
     *             handler.release();
     *         }
     *     }
     * }
     * </pre>
     *
     * @see #release()
     * @return the instance or null if another operation is in progress
     */
    public static ScilabDirectHandler acquire() {
        LOG.finest("lock request");

        try {
            final boolean status = INSTANCE.lock.tryAcquire(0, TimeUnit.SECONDS);
            if (!status) {
                return null;
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        LOG.finest("lock acquired");

        return INSTANCE;
    }

    /**
     * Release the instance
     */
    public void release() {
        LOG.finest("lock release");

        INSTANCE.lock.release();
    }

    /*
     * Handler implementation
     */

    @Override
    public BasicBlock readBlock() throws ScicosFormatException {
        return readBlock(null);
    }

    @Override
    public synchronized BasicBlock readBlock(final BasicBlock instance) throws ScicosFormatException {
        LOG.entering("ScilabDirectHandler", "readBlock");
        final BlockElement element = new BlockElement(null);

        LOG.finer("object allocated");

        ScilabType data;
        try {
            data = Scilab.getInCurrentScilabSession(BLK);
        } catch (JavasciException e) {
            return null;
        }

        // fail safely
        if (data == null) {
            return null;
        }
        if (data.isEmpty()) {
            LOG.finer("data not available");
            return null;
        }

        LOG.finer("data available");

        final BasicBlock block = element.decode(data, instance);
        final StyleMap style = new StyleMap(block.getInterfaceFunctionName());
        style.putAll(block.getStyle());

        block.setStyle(style.toString());

        LOG.exiting("ScilabDirectHandler", "readBlock");
        return block;
    }

    @Override
    public synchronized Map<String, String> readContext() {
        LOG.entering("ScilabDirectHandler", "readContext");
        final ScilabMList list;
        final Map<String, String> result = new LinkedHashMap<String, String>();

        final ScilabType data;
        try {
            data = Scilab.getInCurrentScilabSession(CONTEXT);
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }
        if (data instanceof ScilabMList) {
            list = (ScilabMList) data;
            LOG.finer("data available");
        } else {
            list = new ScilabMList();
            LOG.finer("data unavailable");
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

        LOG.exiting("ScilabDirectHandler", "readContext");
        return result;
    }

    @Override
    public XcosDiagram readDiagram() throws VersionMismatchException {
        return readDiagram(null);
    }

    @Override
    public XcosDiagram readDiagram(final XcosDiagram instance) {
        return readDiagram(instance, SCS_M);
    }

    public synchronized XcosDiagram readDiagram(final XcosDiagram instance, final String variable) {
        LOG.entering("ScilabDirectHandler", "readDiagram");
        final DiagramElement element = new DiagramElement();

        XcosDiagram diagram;
        if (instance == null) {
            diagram = new XcosDiagram();
        } else {
            diagram = instance;
        }

        LOG.finer("object allocated");

        ScilabType data;
        try {
            data = Scilab.getInCurrentScilabSession(variable);
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }

        // fail safely
        if (data.isEmpty()) {
            LOG.finer("data not available");
            return null;
        }

        LOG.finer("data available");

        try {
            diagram = element.decode(data, diagram);
        } catch (ScicosFormatException e) {
            if (e instanceof VersionMismatchException) {
                /*
                 * On version mismatch alert the user but the current instance
                 * contains the partially decoded data so continue.
                 */
                diagram.error(XcosMessages.UNKNOW_VERSION + ((VersionMismatchException) e).getWrongVersion() + "\n" + XcosMessages.TRY_TO_CONTINUE);
            } else {
                // rethrow
                throw new RuntimeException(e);
            }
        } catch (Throwable t) {
            t.printStackTrace();
        } finally {
            diagram.getModel().endUpdate();
        }

        LOG.exiting("ScilabDirectHandler", "readDiagram");
        return diagram;
    }

    @Override
    public void writeBlock(final BasicBlock block) {
        LOG.entering("ScilabDirectHandler", "writeBlock");

        final BlockElement element = new BlockElement(block.getParentDiagram());
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
