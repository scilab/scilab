/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.modelica;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.EventListenerList;

/**
 * Contains the statistics for the current model.
 */
public final class ModelStatistics {
    private long equations;
    private long fixedParameters;
    private long relaxedParameters;
    private long fixedVariables;
    private long relaxedVariables;
    private long expectedRelaxedVariables;
    private long discreteStates;
    private long inputs;
    private long outputs;
    private long derivativeStates;

    private final EventListenerList listenerList = new EventListenerList();
    private transient ChangeEvent changeEvent; // = null

    /**
     * Default constructor
     */
    public ModelStatistics() {
    }

    /**
     * @return the diffStates
     *
     */
    public long getDerivativeStates() {
        return derivativeStates;
    }

    /**
     * @return the discreteStates
     */
    public long getDiscreteStates() {
        return discreteStates;
    }

    /**
     * @return the equations
     */
    public long getEquations() {
        return equations;
    }

    /**
     * @return the fixedParameters
     */
    public long getFixedParameters() {
        return fixedParameters;
    }

    /**
     * @return the fixedVariables
     */
    public long getFixedVariables() {
        return fixedVariables;
    }

    /**
     * @return the inputs
     */
    public long getInputs() {
        return inputs;
    }

    /**
     * @return the outputs
     */
    public long getOutputs() {
        return outputs;
    }

    /**
     * @return the relaxedParameters
     */
    public long getRelaxedParameters() {
        return relaxedParameters;
    }

    /**
     * @return the relaxedVariables
     */
    public long getRelaxedVariables() {
        return relaxedVariables;
    }

    /**
     * @return the calculated unknowns
     */
    public long getUnknowns() {
        return getRelaxedParameters() + getRelaxedVariables();
    }

    /**
     * Increment the diffStates
     */
    public void incDerivativeStates() {
        incDerivativeStates(1);
    }

    /**
     * Increment the diffStates
     *
     * @param increment
     *            the increment
     */
    public void incDerivativeStates(long increment) {
        derivativeStates += increment;
    }

    /**
     * Increment the discreteStates
     */
    public void incDiscreteStates() {
        incDiscreteStates(1);
    }

    /**
     * Increment the discreteStates
     *
     * @param increment
     *            the increment
     */
    public void incDiscreteStates(long increment) {
        discreteStates += increment;
    }

    /**
     * Increment the equations
     */
    public void incEquations() {
        incEquations(1);
    }

    /**
     * Increment the equations
     *
     * @param increment
     *            the increment
     */
    public void incEquations(long increment) {
        equations += increment;
    }

    /**
     * Increment the fixedParameters
     */
    public void incFixedParameters() {
        incFixedParameters(1);
    }

    /**
     * Increment the fixedParameters
     *
     * @param increment
     *            the increment
     */
    public void incFixedParameters(long increment) {
        fixedParameters += increment;
    }

    /**
     * Increment the fixedVariables
     */
    public void incFixedVariables() {
        incFixedVariables(1);
    }

    /**
     * Increment the fixedVariables
     *
     * @param increment
     *            the increment
     */
    public void incFixedVariables(long increment) {
        fixedVariables += increment;
    }

    /**
     * Increment the inputs
     */
    public void incInputs() {
        incInputs(1);
    }

    /**
     * Increment the inputs
     *
     * @param increment
     *            the increment
     */
    public void incInputs(long increment) {
        inputs += increment;
    }

    /**
     * Increment the outputs
     */
    public void incOutputs() {
        incOutputs(1);
    }

    /**
     * Increment the outputs
     *
     * @param increment
     *            the increment
     */
    public void incOutputs(long increment) {
        outputs += increment;
    }

    /**
     * Increment the relaxedParameters
     */
    public void incRelaxedParameters() {
        incRelaxedParameters(1);
    }

    /**
     * Increment the relaxedParameters
     *
     * @param increment
     *            the increment
     */
    public void incRelaxedParameters(long increment) {
        relaxedParameters += increment;
    }

    /**
     * Increment the relaxedVariables
     */
    public void incRelaxedVariables() {
        incRelaxedVariables(1);
    }

    /**
     * Increment the relaxedVariables
     *
     * @param increment
     *            the increment
     */
    public void incRelaxedVariables(long increment) {
        relaxedVariables += increment;
    }

    /**
     * @param equations
     *            the equations to set
     */
    public void setEquations(long equations) {
        this.equations = equations;
    }

    /**
     * @param relaxedVariables
     *            the relaxedVariables to set
     */
    public void setRelaxedVariables(long relaxedVariables) {
        this.expectedRelaxedVariables = relaxedVariables;
    }

    /*
     * Utility methods
     */

    /**
     * @return true if the statistics are all zeros, false otherwise.
     */
    // CSOFF: BooleanExpressionComplexity
    public boolean isEmpty() {
        return equations == 0 && fixedParameters == 0 && relaxedParameters == 0
               && fixedVariables == 0 && relaxedVariables == 0
               && discreteStates == 0 && inputs == 0 && outputs == 0
               && derivativeStates == 0;
    }

    // CSON: BooleanExpressionComplexity

    /**
     * Clear the statistics
     */
    public void clear() {
        equations = 0;
        fixedParameters = 0;
        relaxedParameters = 0;
        fixedVariables = 0;
        relaxedVariables = 0;
        discreteStates = 0;
        inputs = 0;
        outputs = 0;
        derivativeStates = 0;
    }

    /*
     * Change listener support
     */

    /**
     * Add a listener
     *
     * @param l
     *            the listener
     */
    public void addChangeListener(ChangeListener l) {
        listenerList.add(ChangeListener.class, l);
    }

    /**
     * Remove a listener
     *
     * @param l
     *            the listener
     */
    public void removeChangeListener(ChangeListener l) {
        listenerList.remove(ChangeListener.class, l);
    }

    /**
     * Notify all listeners that have registered interest for notification on
     * this event type. The event instance is lazily created using the
     * parameters passed into the fire method.
     */
    public void fireChange() {
        // Guaranteed to return a non-null array
        Object[] listeners = listenerList.getListenerList();
        // Process the listeners last to first, notifying
        // those that are interested in this event
        for (int i = listeners.length - 2; i >= 0; i -= 2) {
            if (listeners[i] == ChangeListener.class) {
                // Lazily create the event:
                if (changeEvent == null) {
                    changeEvent = new ChangeEvent(this);
                }
                ((ChangeListener) listeners[i + 1]).stateChanged(changeEvent);
            }
        }
    }
}
