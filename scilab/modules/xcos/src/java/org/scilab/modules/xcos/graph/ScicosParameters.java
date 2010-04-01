/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;

/**
 * Contains Scicos specific parameters.
 * 
 * This class is a java beans and follow standard properties accessors.
 * 
 * @see http://java.sun.com/docs/books/tutorial/javabeans/properties/bound.html
 */
public class ScicosParameters {
	/*
	 * Default instance values from :
	 * SCI/modules/scicos/macros/scicos_scicos/scicos_params.sci
	 */
	/**
	 * The default integration time
	 */
	public static final double FINAL_INTEGRATION_TIME = 100000;
	/**
	 * The default integrator absolute tolerance
	 */
	public static final double INTEGRATOR_ABS_TOLERANCE = 1e-4;
	/**
	 * The default integrator relative tolerance
	 */
	public static final double INTEGRATOR_RELATIVE_TOLERANCE = 1e-6;
	/**
	 * The default tolerance on time to use
	 */
	public static final double TOLERANCE_ON_TIME = 1e-10;
	/**
	 * The default maximum integration time to use
	 */
	public static final double MAX_INTEGRATION_TIME_INTERVAL = FINAL_INTEGRATION_TIME + 1;
	/**
	 * The default real time scaling
	 */
	public static final double REAL_TIME_SCALING = 0.0;
	/**
	 * Select the solver
	 */
	public static final double SOLVER = 0.0;
	/**
	 * The default maximum simulation step size.
	 */
	public static final double MAX_STEP_SIZE = 0.0;
	/**
	 * The default level of information display.
	 */
	public static final int DEBUG_LEVEL = 0;
	/**
	 * The context is any Scilab expression evaluated at the start of the
	 * simulation.
	 */
	public static final String[] CONTEXT = new String[] {""};
	/**
	 * The current Scicos simulator version.
	 */
	public static final String SCICOS_VERSION = "scicos4.2";

	/*
	 * Instance data
	 */
	private double finalIntegrationTime;
	private double integratorAbsoluteTolerance;
	private double integratorRelativeTolerance;
	private double toleranceOnTime;
	private double maxIntegrationTimeinterval;
	private double realTimeScaling;
	private double solver;
	private double maximumStepSize;
	private int debugLevel;
	private String[] context;
	private String version;

	/*
	 * Beans support, used to follow instance modification
	 */
	private final PropertyChangeSupport pcs = new PropertyChangeSupport(this);

	/**
	 * Default constructor
	 * 
	 * Initialize parameters with their default values.
	 */
	public ScicosParameters() {
		finalIntegrationTime = FINAL_INTEGRATION_TIME;
		integratorAbsoluteTolerance = INTEGRATOR_ABS_TOLERANCE;
		integratorRelativeTolerance = INTEGRATOR_RELATIVE_TOLERANCE;
		toleranceOnTime = TOLERANCE_ON_TIME;
		maxIntegrationTimeinterval = MAX_INTEGRATION_TIME_INTERVAL;
		realTimeScaling = REAL_TIME_SCALING;
		solver = SOLVER;
		maximumStepSize = MAX_STEP_SIZE;
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
	 */
	public void setFinalIntegrationTime(double finalIntegrationTime) {
		double oldValue = this.finalIntegrationTime;
		this.finalIntegrationTime = finalIntegrationTime;
		pcs.firePropertyChange("finalIntegrationTime", oldValue,
				finalIntegrationTime);
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
	 */
	public void setIntegratorAbsoluteTolerance(
			double integratorAbsoluteTolerance) {
		double oldValue = this.integratorAbsoluteTolerance;
		this.integratorAbsoluteTolerance = integratorAbsoluteTolerance;
		pcs.firePropertyChange("integratorAbsoluteTolerance", oldValue,
				integratorAbsoluteTolerance);
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
	 */
	public void setIntegratorRelativeTolerance(
			double integratorRelativeTolerance) {
		double oldValue = this.integratorRelativeTolerance;
		this.integratorRelativeTolerance = integratorRelativeTolerance;
		pcs.firePropertyChange("integratorRelativeTolerance", oldValue,
				integratorRelativeTolerance);
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
	 */
	public void setMaximumStepSize(double maximumStepSize) {
		double oldValue = this.maximumStepSize;
		this.maximumStepSize = maximumStepSize;
		pcs.firePropertyChange("maximumStepSize", oldValue, maximumStepSize);
	}

	/**
	 * @return max integration time
	 */
	public double getMaxIntegrationTimeinterval() {
		return maxIntegrationTimeinterval;
	}

	/**
	 * @param maxIntegrationTimeinterval
	 *            set max integration time
	 */
	public void setMaxIntegrationTimeinterval(double maxIntegrationTimeinterval) {
		double oldValue = this.maxIntegrationTimeinterval;
		this.maxIntegrationTimeinterval = maxIntegrationTimeinterval;
		pcs.firePropertyChange("maxIntegrationTimeinterval", oldValue,
				maxIntegrationTimeinterval);
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
	 */
	public void setRealTimeScaling(double realTimeScaling) {
		double oldValue = this.realTimeScaling;
		this.realTimeScaling = realTimeScaling;
		pcs.firePropertyChange("realTimeScaling", oldValue, realTimeScaling);
	}

	/**
	 * @return solver
	 */
	public double getSolver() {
		return solver;
	}

	/**
	 * @param solver
	 *            set solver
	 */
	public void setSolver(double solver) {
		double oldValue = this.solver;
		this.solver = solver;
		pcs.firePropertyChange("solver", oldValue, solver);
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
	 */
	public void setToleranceOnTime(double toleranceOnTime) {
		double oldValue = this.toleranceOnTime;
		this.toleranceOnTime = toleranceOnTime;
		pcs.firePropertyChange("toleranceOnTime", oldValue, toleranceOnTime);
	}

	/**
	 * @param context
	 *            set context
	 */
	public void setContext(String[] context) {
		String[] oldValue = this.context;
		this.context = context;
		pcs.firePropertyChange("context", oldValue, context);
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
	 */
	public void setDebugLevel(int debugLevel) {
		int oldValue = this.debugLevel;
		this.debugLevel = debugLevel;
		pcs.firePropertyChange("debugLevel", oldValue, debugLevel);
	}

	/*
	 * PropertyChangeSupport proxy method
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
	public void addPropertyChangeListener(String propertyName,
			PropertyChangeListener listener) {
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
	public void removePropertyChangeListener(String propertyName,
			PropertyChangeListener listener) {
		this.pcs.removePropertyChangeListener(propertyName, listener);
	}
}
