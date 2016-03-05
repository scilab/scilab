/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.io.scicos;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.logging.Logger;

import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;

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
    public void writeContext(final String[] context) {
        LOG.entering("ScilabDirectHandler", "writeContext");

        try {
            Scilab.putInCurrentScilabSession(CONTEXT, new ScilabString(context));
        } catch (JavasciException e) {
            throw new RuntimeException(e);
        }

        LOG.exiting("ScilabDirectHandler", "writeContext");
    }
}
