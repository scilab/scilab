/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.beans.PropertyVetoException;
import java.beans.VetoableChangeListener;
import java.beans.VetoableChangeSupport;
import java.io.Serializable;

import org.scilab.modules.xcos.preferences.XcosOptions;

/**
 * Contains Scicos specific parameters.
 *
 * This class is a java beans and follow standard properties accessors.
 *
 * @see <a
 *      href="http://java.sun.com/docs/books/tutorial/javabeans/properties/bound.html">JavaBeans
 *      Bound Properties</a>
 */
@SuppressWarnings(value = { "serial" })
public class ScicosParameters implements Serializable, Cloneable {
    /*
     * Default instance values from :
     * SCI/modules/scicos/macros/scicos_scicos/scicos_params.sci
     *
     * These values are preferences and thus can be updated by user settings.
     */

    /**
     * The default integration time
     */
    public static double FINAL_INTEGRATION_TIME = 100000;
    /**
     * The default integrator absolute tolerance
     */
    public static double INTEGRATOR_ABSOLUTE_TOLERANCE = 1e-6;
    /**
     * The default integrator relative tolerance
     */
    public static double INTEGRATOR_RELATIVE_TOLERANCE = 1e-6;
    /**
     * The default tolerance on time to use
     */
    public static double TOLERANCE_ON_TIME = 1e-10;
    /**
     * The default maximum integration time to use
     */
    public static double MAX_INTEGRATION_TIME_INTERVAL = FINAL_INTEGRATION_TIME + 1;
    /**
     * The default real time scaling
     */
    public static double REAL_TIME_SCALING = 0.0;
    /**
     * Select the solver
     */
    public static double SOLVER = 0.0;
    /**
     * The default maximum simulation step size.
     */
    public static double MAXIMUM_STEP_SIZE = 0.0;
    /**
     * The default level of information display.
     */
    public static int DEBUG_LEVEL = 0;
    /**
     * The context is any Scilab expression evaluated at the start of the
     * simulation.
     */
    public static final String[] CONTEXT = new String[] {};
    /**
     * The current Scicos simulator version.
     */
    public static final String SCICOS_VERSION = "scicos4.3";

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
     * Instance data
     */
    private double finalIntegrationTime;
    private double integratorAbsoluteTolerance;
    private double integratorRelativeTolerance;
    private double toleranceOnTime;
    private double maxIntegrationTimeInterval;
    private double realTimeScaling;
    private double solver;
    private double maximumStepSize;
    private int debugLevel;
    private String[] context;
    private final String version;

    /*
     * Beans support, used to follow instance modification and validate changes.
     */
    private final PropertyChangeSupport pcs = new PropertyChangeSupport(this);
    private final VetoableChangeSupport vcs = new VetoableChangeSupport(this);

    /**
     * Default constructor
     *
     * Initialize parameters with their default values.
     */
    public ScicosParameters() {
        /*
         * This call will update static values from the configuration.
         */
        XcosOptions.getSimulation();

        finalIntegrationTime = FINAL_INTEGRATION_TIME;
        integratorAbsoluteTolerance = INTEGRATOR_ABSOLUTE_TOLERANCE;
        integratorRelativeTolerance = INTEGRATOR_RELATIVE_TOLERANCE;
        toleranceOnTime = TOLERANCE_ON_TIME;
        maxIntegrationTimeInterval = MAX_INTEGRATION_TIME_INTERVAL;
        realTimeScaling = REAL_TIME_SCALING;
        solver = SOLVER;
        maximumStepSize = MAXIMUM_STEP_SIZE;
        debugLevel = DEBUG_LEVEL;
        context = CONTEXT;
        version = SCICOS_VERSION;
    }

    /**
     * @return integration time
     */
    public double getFinalIntegrationTime() {
        return finalIntegrationTime;
    }

    /**
     * @param finalIntegrationTime
     *            set integration time
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setFinalIntegrationTime(double finalIntegrationTime) throws PropertyVetoException {
        double oldValue = this.finalIntegrationTime;
        vcs.fireVetoableChange(FINAL_INTEGRATION_TIME_CHANGE, oldValue, finalIntegrationTime);
        this.finalIntegrationTime = finalIntegrationTime;
        pcs.firePropertyChange(FINAL_INTEGRATION_TIME_CHANGE, oldValue, finalIntegrationTime);
    }

    /**
     * @return integrator absolute tolerance
     */
    public double getIntegratorAbsoluteTolerance() {
        return integratorAbsoluteTolerance;
    }

    /**
     * @param integratorAbsoluteTolerance
     *            set integrator absolute tolerance
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setIntegratorAbsoluteTolerance(double integratorAbsoluteTolerance) throws PropertyVetoException {
        double oldValue = this.integratorAbsoluteTolerance;
        vcs.fireVetoableChange(INTEGRATOR_ABSOLUTE_TOLERANCE_CHANGE, oldValue, integratorAbsoluteTolerance);
        this.integratorAbsoluteTolerance = integratorAbsoluteTolerance;
        pcs.firePropertyChange(INTEGRATOR_ABSOLUTE_TOLERANCE_CHANGE, oldValue, integratorAbsoluteTolerance);
    }

    /**
     * @return integrator relative tolerance
     */
    public double getIntegratorRelativeTolerance() {
        return integratorRelativeTolerance;
    }

    /**
     * @param integratorRelativeTolerance
     *            integrator relative tolerance
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setIntegratorRelativeTolerance(double integratorRelativeTolerance) throws PropertyVetoException {
        double oldValue = this.integratorRelativeTolerance;
        vcs.fireVetoableChange(INTEGRATOR_RELATIVE_TOLERANCE_CHANGE, oldValue, integratorRelativeTolerance);
        this.integratorRelativeTolerance = integratorRelativeTolerance;
        pcs.firePropertyChange(INTEGRATOR_RELATIVE_TOLERANCE_CHANGE, oldValue, integratorRelativeTolerance);
    }

    /**
     * @return max step size
     */
    public double getMaximumStepSize() {
        return maximumStepSize;
    }

    /**
     * @param maximumStepSize
     *            set max step size
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setMaximumStepSize(double maximumStepSize) throws PropertyVetoException {
        double oldValue = this.maximumStepSize;
        vcs.fireVetoableChange(MAXIMUM_STEP_SIZE_CHANGE, oldValue, maximumStepSize);
        this.maximumStepSize = maximumStepSize;
        pcs.firePropertyChange(MAXIMUM_STEP_SIZE_CHANGE, oldValue, maximumStepSize);
    }

    /**
     * @return max integration time
     */
    public double getMaxIntegrationTimeInterval() {
        return maxIntegrationTimeInterval;
    }

    /**
     * @param maxIntegrationTimeinterval
     *            set max integration time
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setMaxIntegrationTimeInterval(double maxIntegrationTimeinterval) throws PropertyVetoException {
        double oldValue = this.maxIntegrationTimeInterval;
        vcs.fireVetoableChange(MAX_INTEGRATION_TIME_INTERVAL_CHANGE, oldValue, maxIntegrationTimeinterval);
        this.maxIntegrationTimeInterval = maxIntegrationTimeinterval;
        pcs.firePropertyChange(MAX_INTEGRATION_TIME_INTERVAL_CHANGE, oldValue, maxIntegrationTimeinterval);
    }

    /**
     * @return real time scaling
     */
    public double getRealTimeScaling() {
        return realTimeScaling;
    }

    /**
     * @param realTimeScaling
     *            set real time scaling
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setRealTimeScaling(double realTimeScaling) throws PropertyVetoException {
        double oldValue = this.realTimeScaling;
        vcs.fireVetoableChange(REAL_TIME_SCALING_CHANGE, oldValue, realTimeScaling);
        this.realTimeScaling = realTimeScaling;
        pcs.firePropertyChange(REAL_TIME_SCALING_CHANGE, oldValue, realTimeScaling);
    }

    /**
     * <ul>
     * <li>0 : LSodar : Method: DYNAMIC, Nonlinear solver= DYNAMIC
     * <li>1 : Sundials/CVODE : Method: BDF, Nonlinear solver= FUNCTIONAL
     * <li>2 : Sundials/CVODE : Method: BDF, Nonlinear solver= FUNCTIONAL
     * <li>3 : Sundials/CVODE : Method: ADAMS, Nonlinear solver= NEWTON
     * <li>4 : Sundials/CVODE : Method: ADAMS, Nonlinear solver= FUNCTIONAL
     * <li>5 : DOPRI5 : Method: Dormand-Prince 4(5)
     * <li>6 : RK45 : Method: Runge-Kutta 4(5)
     * <li>7 : Implicit RK45 : Method: Runge-Kutta 4(5), Nonlinear solver= Fixed-point
     * <li>100 : Sundials/IDA : Method: BDF, Nonlinear solver= NEWTON
     * <li>101 : DDaskr : Method: BDF, Nonlinear solver= NEWTON
     * <li>102 : DDaskr : Method: BDF, Nonlinear solver= GMRES
     *
     *
     * @return solver value
     */
    public double getSolver() {
        return solver;
    }

    /**
     * <ul>
     * <li>0 : LSodar : Method: DYNAMIC, Nonlinear solver= DYNAMIC
     * <li>1 : Sundials/CVODE : Method: BDF, Nonlinear solver= FUNCTIONAL
     * <li>2 : Sundials/CVODE : Method: BDF, Nonlinear solver= FUNCTIONAL
     * <li>3 : Sundials/CVODE : Method: ADAMS, Nonlinear solver= NEWTON
     * <li>4 : Sundials/CVODE : Method: ADAMS, Nonlinear solver= FUNCTIONAL
     * <li>5 : DOPRI5 : Method: Dormand-Prince 4(5)
     * <li>6 : RK45 : Method: Runge-Kutta 4(5)
     * <li>7 : Implicit RK45 : Method: Runge-Kutta 4(5), Nonlinear solver= FIXED-POINT
     * <li>100 : Sundials/IDA : Method: BDF, Nonlinear solver= NEWTON
     * <li>101 : DDaskr : Method: BDF, Nonlinear solver= NEWTON
     * <li>102 : DDaskr : Method: BDF, Nonlinear solver= GMRES
     *
     *
     * @param solver
     *            set solver
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setSolver(double solver) throws PropertyVetoException {
        double oldValue = this.solver;
        vcs.fireVetoableChange(SOLVER_CHANGE, oldValue, solver);
        this.solver = solver;
        pcs.firePropertyChange(SOLVER_CHANGE, oldValue, solver);
    }

    /**
     * @return tolerance time
     */
    public double getToleranceOnTime() {
        return toleranceOnTime;
    }

    /**
     * @param toleranceOnTime
     *            set tolerance time
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setToleranceOnTime(double toleranceOnTime) throws PropertyVetoException {
        double oldValue = this.toleranceOnTime;
        vcs.fireVetoableChange(TOLERANCE_ON_TIME_CHANGE, oldValue, toleranceOnTime);
        this.toleranceOnTime = toleranceOnTime;
        pcs.firePropertyChange(TOLERANCE_ON_TIME_CHANGE, oldValue, toleranceOnTime);
    }

    /**
     * Set the associated context if there is noticeable changes.
     *
     * @param context
     *            set context
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setContext(String[] context) throws PropertyVetoException {
        if (context == null) {
            throw new IllegalArgumentException("context must not be null");
        }
        String[] oldValue = this.context;

        /*
         * Check for modification
         */
        boolean modified = false;

        if (oldValue.length != context.length) {
            modified = true;
        } else {
            // Same length so compare line per line
            for (int i = 0; i < oldValue.length; i++) {
                String indexedOld = oldValue[i];
                String indexedNew = context[i];

                if (!indexedOld.equals(indexedNew)) {
                    modified = true;
                    break;
                }
            }
        }

        /*
         * Apply context if modified
         */
        if (modified) {
            vcs.fireVetoableChange(CONTEXT_CHANGE, oldValue, context);
            this.context = context;
            pcs.firePropertyChange(CONTEXT_CHANGE, oldValue, context);
        }
    }

    /**
     * @return current context
     */
    public String[] getContext() {
        return context;
    }

    /**
     * @return current version
     */
    public String getVersion() {
        return version;
    }

    /**
     * @return current debug level
     */
    public int getDebugLevel() {
        return debugLevel;
    }

    /**
     * @param debugLevel
     *            change debug level
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setDebugLevel(int debugLevel) throws PropertyVetoException {
        int oldValue = this.debugLevel;
        vcs.fireVetoableChange(DEBUG_LEVEL_CHANGE, oldValue, debugLevel);
        this.debugLevel = debugLevel;
        pcs.firePropertyChange(DEBUG_LEVEL_CHANGE, oldValue, debugLevel);
    }

    /*
     * PropertyChangeSupport proxy methods
     */

    /**
     * Each setXXX method fire a property change event. This method register a
     * new listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void addPropertyChangeListener(PropertyChangeListener listener) {
        this.pcs.addPropertyChangeListener(listener);
    }

    /**
     * Each setXXX method fire a property change event. This method register a
     * new listener for a specific event. Each event name is equal to the field
     * name.
     *
     * @param propertyName
     *            the property name
     * @param listener
     *            A listener
     */
    public void addPropertyChangeListener(String propertyName, PropertyChangeListener listener) {
        this.pcs.addPropertyChangeListener(propertyName, listener);
    }

    /**
     * Each setXXX method fire a property change event. This method remove a
     * listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void removePropertyChangeListener(PropertyChangeListener listener) {
        this.pcs.removePropertyChangeListener(listener);
    }

    /**
     * Each setXXX method fire a property change event. This method remove a
     * listener for a specific event. Each event name is equal to the field
     * name.
     *
     * @param propertyName
     *            the property name
     * @param listener
     *            A listener
     */
    public void removePropertyChangeListener(String propertyName, PropertyChangeListener listener) {
        this.pcs.removePropertyChangeListener(propertyName, listener);
    }

    /*
     * VetoableChangeSupport proxy methods
     */

    /**
     * Each setXXX method fire a vetoable change event. This method register a
     * new listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void addVetoableChangeListener(VetoableChangeListener listener) {
        this.vcs.addVetoableChangeListener(listener);
    }

    /**
     * Each setXXX method fire a vetoable change event. This method register a
     * new listener for a specific event. Each event name is equal to the field
     * name.
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
     * Each setXXX method fire a vetoable change event. This method remove a
     * listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void removeVetoableChangeListener(VetoableChangeListener listener) {
        this.vcs.removeVetoableChangeListener(listener);
    }

    /**
     * Each setXXX method fire a vetoable change event. This method remove a
     * listener for a specific event. Each event name is equal to the field
     * name.
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
