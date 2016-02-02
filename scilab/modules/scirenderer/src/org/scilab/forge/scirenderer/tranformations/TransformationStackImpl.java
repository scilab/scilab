/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
