/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.tranformations;

import java.util.Stack;

import javax.swing.event.EventListenerList;

/**
 * @author Pierre Lando
 */
public class TransformationStackImpl implements TransformationStack {

    /**
     * We use swing EventListenerList for facility.
     */
    private final EventListenerList listeners;

    private final Stack<Transformation> stack;

    public TransformationStackImpl() {
        listeners = new EventListenerList();
        stack = new Stack<Transformation>();
    }

    @Override
    public void addListener(TransformationStackListener listener) {
        listeners.add(TransformationStackListener.class, listener);
    }

    @Override
    public void removeListener(TransformationStackListener listener) {
        listeners.remove(TransformationStackListener.class, listener);
    }

    @Override
    public Transformation peek() {
        if (stack.isEmpty()) {
            return TransformationFactory.getIdentity();
        } else {
            return stack.peek();
        }
    }

    @Override
    public void push(Transformation transformation) {
        if (transformation != null) {
            stack.push(transformation);
            fireChanged(TransformationStackEvent.PUSHED, transformation);
        }
    }

    @Override
    public void pushRightMultiply(Transformation transformation) {
        push(peek().rightTimes(transformation));
    }

    @Override
    public void pushLeftMultiply(Transformation transformation) {
        push(peek().leftTimes(transformation));
    }

    @Override
    public Transformation pop() {
        Transformation value = stack.pop();
        if (value == null) {
            value = TransformationFactory.getIdentity();
        }
        fireChanged(TransformationStackEvent.POPPED, value);
        return value;
    }

    @Override
    public void clear() {
        stack.clear();
        fireChanged(TransformationStackEvent.CLEARED, peek());
    }

    /**
     * Fire a change event.
     * @param event the event.
     * @param top the new top transformation.
     */
    protected void fireChanged(TransformationStackEvent event, Transformation top) {
        for (TransformationStackListener listener : listeners.getListeners(TransformationStackListener.class)) {
            listener.changed(this, event, top);
        }
    }
}
