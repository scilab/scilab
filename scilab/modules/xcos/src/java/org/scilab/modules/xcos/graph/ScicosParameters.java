/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.graph;

import java.beans.PropertyVetoException;
import java.beans.VetoableChangeListener;
import java.beans.VetoableChangeSupport;
import java.io.Serializable;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.preferences.XcosOptions;

/**
 * Contains Scicos specific parameters.
 *
 * This class is a java beans and follow standard properties accessors.
 *
 * @see <a href="http://java.sun.com/docs/books/tutorial/javabeans/properties/bound.html">JavaBeans Bound Properties</a>
 */
@SuppressWarnings(value = { "serial" })
public class ScicosParameters implements Serializable, Cloneable {

    /*
     * Bean properties
     */
    /**
     * Property bound to finalIntegrationTime modification.
     */
    public static final String FINAL_INTEGRATION_TIME_CHANGE = "finalIntegrationTime";
    /**
     * Property bound to integratorAbsoluteTolerance modification.
     */
    public static final String INTEGRATOR_ABSOLUTE_TOLERANCE_CHANGE = "integratorAbsoluteTolerance";
    /**
     * Property bound to integratorRelativeTolerance modification.
     */
    public static final String INTEGRATOR_RELATIVE_TOLERANCE_CHANGE = "integratorRelativeTolerance";
    /**
     * Property bound to toleranceOnTime modification.
     */
    public static final String TOLERANCE_ON_TIME_CHANGE = "toleranceOnTime";
    /**
     * Property bound to maxIntegrationTimeInterval modification.
     */
    public static final String MAX_INTEGRATION_TIME_INTERVAL_CHANGE = "maxIntegrationTimeInterval";
    /**
     * Property bound to realTimeScaling modification.
     */
    public static final String REAL_TIME_SCALING_CHANGE = "realTimeScaling";
    /**
     * Property bound to solver modification.
     */
    public static final String SOLVER_CHANGE = "solver";
    /**
     * Property bound to maximumStepSize modification.
     */
    public static final String MAXIMUM_STEP_SIZE_CHANGE = "maximumStepSize";
    /**
     * Property bound to debugLevel modification.
     */
    public static final String DEBUG_LEVEL_CHANGE = "debugLevel";
    /**
     * Property bound to context modification.
     */
    public static final String CONTEXT_CHANGE = "context";

    /*
     * Index inside the VectorOfDouble object
     */
    public static final int FINAL_INTEGRATION_TIME = 0;
    public static final int INTEGRATOR_ABSOLUTE_TOLERANCE = 1;
    public static final int INTEGRATOR_RELATIVE_TOLERANCE = 2;
    public static final int TOLERANCE_ON_TIME = 3;
    public static final int MAX_INTEGRATION_TIME_INTERVAL = 4;
    public static final int REAL_TIME_SCALING = 5;
    public static final int SOLVER = 6;
    public static final int MAXIMUM_STEP_SIZE = 7;

    /*
     * Default values from the preferences
     */
    public static VectorOfDouble DEFAULT_PARAMETERS;
    public static int DEFAULT_DEBUG_LEVEL;

    /**
     * Reference to the diagram
     */
    private final long uid;
    private final Kind kind;

    /*
     * Beans support, used to follow instance modification and validate changes.
     */
    private final VetoableChangeSupport vcs = new VetoableChangeSupport(this);

    /**
     * Default constructor
     *
     * Initialize parameters with their default values.
     */
    public ScicosParameters(final long uid, final Kind kind) {
        this.uid = uid;
        this.kind = kind;

        /*
         * This call will update static values from the configuration.
         */
        XcosOptions.getSimulation();
    }

    public long getUID() {
        return uid;
    }

    public Kind getKind() {
        return kind;
    }

    /**
     * Returns the properties values
     *
     * @param controller
     *            the controller instance
     * @return the current properties
     */
    public VectorOfDouble getProperties(final JavaController controller) {
        VectorOfDouble v = new VectorOfDouble(7);
        controller.getObjectProperty(getUID(), getKind(), ObjectProperties.PROPERTIES, v);
        return v;
    }

    /**
     * Set the properties
     *
     * @param controller
     *            the controller instance
     * @param v
     *            the values to set
     * @throws PropertyVetoException
     *             in case of the values are not valid
     */
    public void setProperties(final JavaController controller, VectorOfDouble v) throws PropertyVetoException {
        VectorOfDouble oldValue = getProperties(controller);
        vcs.fireVetoableChange(FINAL_INTEGRATION_TIME_CHANGE, oldValue, v);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.PROPERTIES, v);
    }

    /**
     * @return current context
     */
    public VectorOfString getContext(final JavaController controller) {
        VectorOfString v = new VectorOfString();
        controller.getObjectProperty(getUID(), getKind(), ObjectProperties.DIAGRAM_CONTEXT, v);
        return v;
    }

    /**
     * Set the associated context if there is noticeable changes.
     *
     * @param controller
     *            the controller
     * @param v
     *            set context
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setContext(final JavaController controller, VectorOfString v) throws PropertyVetoException {
        VectorOfString oldValue = getContext(controller);
        vcs.fireVetoableChange(CONTEXT_CHANGE, oldValue, v);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.DIAGRAM_CONTEXT, v);
    }

    /**
     * @param controller
     *            the controller instance
     * @return current version
     */
    public String getVersion(final JavaController controller) {
        String[] v = new String[1];
        controller.getObjectProperty(getUID(), getKind(), ObjectProperties.VERSION_NUMBER, v);
        return v[0];
    }

    /**
     * @return current debug level
     */
    public int getDebugLevel(final JavaController controller) {
        int[] v = new int[1];
        controller.getObjectProperty(getUID(), getKind(), ObjectProperties.DEBUG_LEVEL, v);
        return v[0];
    }

    /**
     * @param debugLevel
     *            change debug level
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setDebugLevel(final JavaController controller, int debugLevel) throws PropertyVetoException {
        int oldValue = getDebugLevel(controller);
        vcs.fireVetoableChange(DEBUG_LEVEL_CHANGE, oldValue, debugLevel);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.DIAGRAM_CONTEXT, debugLevel);
    }

    /*
     * VetoableChangeSupport proxy methods
     */

    /**
     * Each setXXX method fire a vetoable change event. This method register a new listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void addVetoableChangeListener(VetoableChangeListener listener) {
        this.vcs.addVetoableChangeListener(listener);
    }

    /**
     * Each setXXX method fire a vetoable change event. This method register a new listener for a specific event. Each event name is equal to the field name.
     *
     * @param propertyName
     *            the property name
     * @param listener
     *            A listener
     */
    public void addVetoableChangeListener(String propertyName, VetoableChangeListener listener) {
        this.vcs.addVetoableChangeListener(propertyName, listener);
    }

    /**
     * Each setXXX method fire a vetoable change event. This method remove a listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void removeVetoableChangeListener(VetoableChangeListener listener) {
        this.vcs.removeVetoableChangeListener(listener);
    }

    /**
     * Each setXXX method fire a vetoable change event. This method remove a listener for a specific event. Each event name is equal to the field name.
     *
     * @param propertyName
     *            the property name
     * @param listener
     *            A listener
     */
    public void removeVetoableChangeListener(String propertyName, VetoableChangeListener listener) {
        this.vcs.removeVetoableChangeListener(propertyName, listener);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
