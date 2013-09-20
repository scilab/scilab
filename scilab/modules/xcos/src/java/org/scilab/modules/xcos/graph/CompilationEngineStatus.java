/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.logging.Logger;

import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxEventSource.mxIEventListener;

/**
 * Contains the current Scicos engine status.
 */
public class CompilationEngineStatus implements mxIEventListener, PropertyChangeListener {
    private static final Logger LOG = Logger.getLogger(CompilationEngineStatus.class.getName());

    private boolean compilationNeeded;

    /**
     * Default constructor.
     */
    public CompilationEngineStatus() {
        setCompilationNeeded(true);
    }

    /**
     * @param status
     *            true, when the associated diagram need to be compiled, false
     *            otherwise.
     */
    public void setCompilationNeeded(boolean status) {
        // compilationNeeded = status;

        compilationNeeded = true;
    }

    /**
     * @return always true as we don't use scicos internal modification checking
     */
    public boolean isCompilationNeeded() {
        return compilationNeeded;
    }

    /*
     * Property change listener
     */

    /**
     * Listener used for any interesting diagram change.
     *
     * @param sender
     *            the associated diagram
     * @param evt
     *            the current event.
     * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
     *      com.mxgraph.util.mxEventObject)
     */
    @Override
    public void invoke(Object sender, mxEventObject evt) {
        setCompilationNeeded(true);
    }

    /**
     * Property change listener used to update compilation status when the
     * context has changed.
     *
     * @param evt
     *            the current event
     * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
     */
    @Override
    public void propertyChange(PropertyChangeEvent evt) {
        setCompilationNeeded(true);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
