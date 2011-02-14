/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.modelica;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JDialog;
import javax.swing.WindowConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.EventListenerList;
import javax.xml.bind.JAXBException;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.modelica.model.Model;
import org.scilab.modules.xcos.modelica.model.Model.Identifiers;
import org.scilab.modules.xcos.modelica.model.Struct;
import org.scilab.modules.xcos.modelica.model.Terminal;
import org.scilab.modules.xcos.modelica.view.MainPanel;

/**
 * Implement a controller which wrap the marshaled model trough interface.
 */
public final class ModelicaController {
	private static final String VARIABLE = "variable";
	private static final String FIXED_PARAMETER = "fixed_parameter";
	private static final Pattern DERIVATIVE_REGEX = Pattern
			.compile("(\\p{Graph}*)__der_(\\p{Graph}*)");

	/**
	 * Contains available computation methods.
	 */
	public static enum ComputationMethod {
		/** the kinsol method */
		Kinsol("Kinsol"),
		/** the ida method */
		Ida("Ida (init)"),
		/** the fsolve method */
		Fsolve("Fsolve"),
		/** the optim method */
		Optim("Optim"),
		/** the Nelder & Mead method */
		NelderMead("Nelder_Mead"),
		/** the hompack method */
		Hompack77("Hompack77");

		private final String name;

		/**
		 * Default constructor
		 * 
		 * @param name
		 *            the name of the method
		 */
		private ComputationMethod(String name) {
			this.name = name;
		}

		/**
		 * @return the name
		 * @see java.lang.Enum#toString()
		 */
		@Override
		public String toString() {
			return name;
		}
	}

	private final Model root;
	private ModelStatistics statistics = new ModelStatistics();

	private boolean compileNeeded;
	private String error;
	private boolean valid;
	private boolean parameterEmbedded;
	private boolean jacobianEnable;
	private ComputationMethod computeMethod;

	private EventListenerList listenerList = new EventListenerList();
	private transient ChangeEvent changeEvent; // = null

	/**
	 * Default constructor
	 * 
	 * @param root
	 *            the root of the model
	 */
	public ModelicaController(Model root) {
		this.root = root;

		// update validity state
		statistics.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				final ModelStatistics stats = (ModelStatistics) e.getSource();

				// Validate equation == unknowns
				setValid(stats.getEquations() == stats.getUnknowns());
			}
		});

		// set error message on validity update
		addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				if (!isSquare()) {
					setError(ModelicaMessages.UNKNOWNS_NEQ_EQUATIONS);
				}
			}
		});

		setDefaultValues();
	}

	/**
	 * Show a dialog for a file
	 * 
	 * @param f
	 *            the file
	 */
	public static void showDialog(final File f) {
		JDialog dialog = new JDialog();
		dialog.setTitle(ModelicaMessages.MODELICA_SETTINGS);
		dialog.setAlwaysOnTop(false);

		ModelicaController controller;
		try {
			controller = new ModelicaController(Modelica.getInstance().load(f));
			dialog.add(new MainPanel(controller));

			dialog.setVisible(true);
			dialog.pack();
			dialog.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		} catch (JAXBException e) {
			LogFactory.getLog(ModelicaController.class).error(e);
		}
	}

	/*
	 * Accessors
	 */

	/**
	 * @return the root
	 */
	public Model getRoot() {
		return root;
	}

	/**
	 * @return the compileNeeded flag
	 */
	public boolean isCompileNeeded() {
		return compileNeeded;
	}

	/**
	 * @param compileNeeded
	 *            the compileNeeded to set
	 */
	public void setCompileNeeded(boolean compileNeeded) {
		if (this.compileNeeded != compileNeeded) {
			this.compileNeeded = compileNeeded;

			fireChange();
		}
	}

	/**
	 * @return the parameterEmbedded
	 */
	public boolean isParameterEmbedded() {
		return parameterEmbedded;
	}

	/**
	 * @param parameterEmbedded
	 *            the parameterEmbedded to set
	 */
	public void setParameterEmbedded(boolean parameterEmbedded) {
		if (this.parameterEmbedded != parameterEmbedded) {
			this.parameterEmbedded = parameterEmbedded;

			fireChange();
		}
	}

	/**
	 * @return the jacobianEnable
	 */
	public boolean isJacobianEnable() {
		return jacobianEnable;
	}

	/**
	 * @param jacobianEnable
	 *            the jacobianEnable to set
	 */
	public void setJacobianEnable(boolean jacobianEnable) {
		if (this.jacobianEnable != jacobianEnable) {
			this.jacobianEnable = jacobianEnable;

			fireChange();
		}
	}

	/**
	 * @return the validity flag
	 */
	public boolean isSquare() {
		return valid;
	}

	/**
	 * @param valid
	 *            the valid to set
	 */
	public void setValid(boolean valid) {
		if (this.valid != valid) {
			this.valid = valid;

			fireChange();
		}
	}

	/**
	 * @return the error
	 */
	public String getError() {
		return error;
	}

	/**
	 * @return the error state
	 */
	public boolean isError() {
		return error == null || error.isEmpty();
	}

	/**
	 * @param error
	 *            the error to set
	 */
	public void setError(String error) {
		final boolean update;
		if (this.error == null) {
			if (error != null) {
				update = true;
			} else {
				update = false;
			}
		} else {
			if (!this.error.equals(error)) {
				update = true;
			} else {
				update = false;
			}
		}

		if (update) {
			this.error = error;

			fireChange();
		}
	}

	/**
	 * @return the computeMethod
	 */
	public ComputationMethod getComputeMethod() {
		return computeMethod;
	}

	/**
	 * @param computeMethod
	 *            the computeMethod to set
	 */
	public void setComputeMethod(ComputationMethod computeMethod) {
		if (this.computeMethod != null
				&& !this.computeMethod.equals(computeMethod)) {
			this.computeMethod = computeMethod;

			fireChange();
		}
	}

	/**
	 * Update the model info from all the {@link Terminal} data
	 * 
	 * This method parse the whole {@link Struct} tree.
	 * 
	 * @return the calculated info
	 */
	public ModelStatistics getStatistics() {
		if (!statistics.isEmpty()) {
			statistics.clear();
		}

		// top level statistics
		statistics.setEquations(root.getEquations().getEquation().size());

		// identifiers statistics
		updateIdentifiers(root.getIdentifiers());

		// Struct tree statistics
		for (Struct struct : getRoot().getElements().getStruct()) {
			updateInfo(struct);
		}

		return statistics;
	}

	/**
	 * Update the statistics from the identifiers
	 * 
	 * @param identifiers
	 *            the identifiers root
	 */
	private void updateIdentifiers(Identifiers identifiers) {
		if (identifiers != null) {
			statistics.setRelaxedVariables(identifiers.getImplicitVariable()
					.size());
		}
	}

	/**
	 * Update the info from the struct children.
	 * 
	 * @param struct
	 *            the current node
	 */
	private void updateInfo(Struct struct) {
		for (Object child : struct.getSubnodes().getTerminalOrStruct()) {
			if (child instanceof Terminal) {
				final Terminal terminal = (Terminal) child;

				final String kind = TerminalAccessor.getData(
						TerminalAccessor.KIND, terminal);
				final Double weight = TerminalAccessor.getData(
						TerminalAccessor.WEIGHT, terminal);
				final String id = TerminalAccessor.getData(TerminalAccessor.ID,
						terminal);

				incrementTerminalWeight(kind, weight.doubleValue());

				if (id != null && DERIVATIVE_REGEX.matcher(id).matches()) {
					statistics.incDerivativeStates();
				}
			} else { // child instanceof Struct

				// recursive call
				updateInfo((Struct) child);
			}
		}
	}

	/**
	 * Increment statistics for a {@link Terminal} node.
	 * 
	 * @param kind
	 *            the kind of statistics to increment
	 * @param weight
	 *            the weight of the data.
	 */
	private void incrementTerminalWeight(final String kind, final double weight) {
		if (FIXED_PARAMETER.equals(kind)) {
			if (weight == 1) {
				statistics.incFixedParameters();
			} else {
				statistics.incRelaxedParameters();
			}
		} else if (VARIABLE.equals(kind)) {
			if (weight == 1) {
				statistics.incFixedVariables();
			} else {
				statistics.incRelaxedVariables();
			}
		} else if ("discrete_variable".equals(kind)) {
			statistics.incDiscreteStates();
		} else if ("input".equals(kind)) {
			statistics.incInputs();
		}
	}

	/*
	 * Actions
	 */

	/**
	 * Set the default initial values
	 */
	private void setDefaultValues() {
		for (final Struct struct : getRoot().getElements().getStruct()) {
			setDefaultValues(struct);
		}
	}

	/**
	 * set the default initial values for a {@link Struct} node.
	 * 
	 * @param struct
	 *            the node
	 */
	@SuppressWarnings("unused")
	private void setDefaultValues(final Struct struct) {
		for (final Object child : struct.getSubnodes().getTerminalOrStruct()) {
			if (child instanceof Terminal) {
				final Terminal terminal = (Terminal) child;

				/*
				 * The first access will update the value.
				 */
				final Boolean notUsed1 = TerminalAccessor.getData(TerminalAccessor.FIXED, terminal);
				final Double notUsed2 = TerminalAccessor.getData(TerminalAccessor.WEIGHT, terminal);
				final Double notUsed3 = TerminalAccessor.getData(TerminalAccessor.INITIAL, terminal);

				final Double notUsed4 = TerminalAccessor.getData(TerminalAccessor.MAX, terminal);
				final Double notUsed5 = TerminalAccessor.getData(TerminalAccessor.MIN, terminal);

			} else {
				// recursive call
				setDefaultValues((Struct) child);
			}
		}
	}

	/**
	 * Update the weight in the current tree
	 * 
	 * @param derivative
	 *            the derivative weight
	 * @param state
	 *            the state weight
	 */
	public void updateWeight(double derivative, double state) {
		for (final Struct struct : getRoot().getElements().getStruct()) {
			updateWeight(struct, derivative, state);
		}

		// clear and update statistics
		getStatistics().fireChange();

		// update the listeners
		fireChange();
	}

	/**
	 * Update the weight for a {@link Struct} subtree
	 * 
	 * @param struct
	 *            the current struct
	 * @param derivative
	 *            the derivative weight
	 * @param state
	 *            the state weight
	 */
	private void updateWeight(final Struct struct, double derivative,
			double state) {
		final List<String> localVarName = new ArrayList<String>();

		for (final Object child : struct.getSubnodes().getTerminalOrStruct()) {
			if (child instanceof Terminal) {
				final Terminal terminal = (Terminal) child;

				final String id = TerminalAccessor.getData(TerminalAccessor.ID,
						terminal);
				final Boolean fixed = TerminalAccessor.getData(
						TerminalAccessor.FIXED, terminal);

				final Matcher matcher = DERIVATIVE_REGEX.matcher(id);

				if (matcher.matches()) {
					// update the derivative weight
					TerminalAccessor.WEIGHT.setData(derivative, terminal);
					TerminalAccessor.SELECTED.setData(Boolean.TRUE, terminal);

					// store the associated variable to the local list.
					String relatedVar = matcher.group(1) + matcher.group(2);
					localVarName.add(relatedVar);

				} else if (localVarName.contains(id) && !fixed.booleanValue()) {
					// update the variable if the associated derivative has been
					// updated.
					TerminalAccessor.WEIGHT.setData(state, terminal);
					TerminalAccessor.SELECTED.setData(Boolean.TRUE, terminal);
				} else {
					TerminalAccessor.SELECTED.setData(Boolean.FALSE, terminal);
				}
			} else {
				// recursive call
				updateWeight((Struct) child, derivative, state);
			}
		}
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
	public void removeFooListener(ChangeListener l) {
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
