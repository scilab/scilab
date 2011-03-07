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

package org.scilab.modules.xcos.modelica.view;

import static org.scilab.modules.xcos.modelica.TerminalAccessor.FIXED;
import static org.scilab.modules.xcos.modelica.TerminalAccessor.WEIGHT;
import static org.scilab.modules.xcos.modelica.TerminalAccessor.getData;

import java.awt.GridLayout;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import javax.swing.BoxLayout;
import javax.swing.JPanel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import org.scilab.modules.xcos.modelica.ModelStatistics;
import org.scilab.modules.xcos.modelica.ModelicaController;
import org.scilab.modules.xcos.modelica.ModelicaController.ComputationMethod;
import org.scilab.modules.xcos.modelica.ModelicaMessages;
import org.scilab.modules.xcos.modelica.TerminalAccessor;
import org.scilab.modules.xcos.modelica.TerminalTableModel;
import org.scilab.modules.xcos.modelica.listener.FixDerivativesAction;
import org.scilab.modules.xcos.modelica.listener.FixStatesAction;
import org.scilab.modules.xcos.modelica.listener.SolveAction;
import org.scilab.modules.xcos.modelica.listener.StatisticsUpdater;
import org.scilab.modules.xcos.modelica.model.Struct;
import org.scilab.modules.xcos.modelica.model.Terminal;

/**
 * The main view of the modelica initialize dialog
 */
// CSOFF: FanOutComplexity
// CSOFF: DataAbstractionCoupling
public final class MainPanel extends JPanel {
	private static final int ERROR_COLUMNS = 40;
	private static final int[] EXTENDED_STATUS_LAYOUT_DATA = new int[] {2, 4};

	private final ModelicaController controller;

	private javax.swing.JPanel controlBar;
	private javax.swing.JPanel dummySolvePane;
	private javax.swing.JScrollPane treeScrollPane;
	private javax.swing.JScrollPane tableScrollPane;
	private javax.swing.JSplitPane splitPanel;
	private javax.swing.JTable table;
	private TerminalTableModel tableModel;
	private javax.swing.JTree tree;
	private LabelWithValue equation;
	private LabelWithValue input;
	private LabelWithValue unknowns;
	private LabelWithValue reduced;
	private LabelWithValue diffSt;
	private LabelWithValue fixedParams;
	private LabelWithValue relaxedParams;
	private LabelWithValue fixedVars;
	private LabelWithValue relaxedVars;
	private LabelWithValue discrete;
	private LabelWithValue error;
	private javax.swing.JCheckBox embeddedParametersButton;
	private javax.swing.JCheckBox generateJacobianButton;
	private javax.swing.JButton solveButton;
	private javax.swing.JLabel solver;
	private javax.swing.JComboBox solverComboBox;
	private javax.swing.JPanel variableStatusBar;
	private javax.swing.JPanel extendedStatus;
	private javax.swing.JPanel globalStatus;
	private javax.swing.JButton fixStates;
	private javax.swing.JButton fixDerivatives;

	/**
	 * Default constructor
	 * 
	 * @param controller the modelica controller
	 */
	public MainPanel(ModelicaController controller) {
		this.controller = controller;
		initComponents();
		installListeners();

		// register the model to the controller
		controller.setModel(tableModel);
		
		fireChange();
	}
	
	/**
	 * Init the component
	 */
	private void initComponents() {
		allocateFields();
		setLayoutsAndBorders();
		
		initGlobalStatus();
		variableStatusBar.add(globalStatus);

		initExtendedStatus();
		variableStatusBar.add(extendedStatus);

		add(variableStatusBar, java.awt.BorderLayout.PAGE_START);

		tree.setModel(createTreeModel());
		treeScrollPane.setViewportView(tree);
		splitPanel.setLeftComponent(treeScrollPane);
		
		table.setModel(tableModel);
		table.setAutoCreateRowSorter(true);
		tableScrollPane.setViewportView(table);
		
		splitPanel.setRightComponent(tableScrollPane);

		add(splitPanel, java.awt.BorderLayout.CENTER);

		initControlBar();
		add(controlBar, java.awt.BorderLayout.PAGE_END);
	}
	
	/**
	 * Create a tree model associated with the current 
	 * @return the model of a tree
	 */
	private TreeModel createTreeModel() {
		final DefaultMutableTreeNode rootNode = new DefaultMutableTreeNode(controller.getRoot());
		final TreeModel model = new DefaultTreeModel(rootNode);
		
		for (Struct struct : controller.getRoot().getElements().getStruct()) {
			rootNode.add(createNodes(struct));
		}
		
		return model;
	}

	/**
	 * Recursively create the nodes for the data
	 * 
	 * @param struct the data
	 * @return the parent node
	 */
	private MutableTreeNode createNodes(Struct struct) {
		DefaultMutableTreeNode structNode = new DefaultMutableTreeNode(struct);
		
		for (Object child : struct.getSubnodes().getTerminalOrStruct()) {
			// recursive call
			if (child instanceof Struct) {
				structNode.add(createNodes((Struct) child));
			}
		}
		
		return structNode;
	}
	
	/**
	 * @param path the current path
	 * @return the terminals associated with the path
	 */
	private List<Terminal> getTerminals(TreePath path) {
		final List<Terminal> ret;
		
		// the root is not a Struct instance and thus return an empty list.  
		if (path.getPathCount() > 1) {
			ret = new ArrayList<Terminal>();
			final Struct struct = (Struct) ((DefaultMutableTreeNode) path
					.getLastPathComponent()).getUserObject();
			
			for (Object child : struct.getSubnodes().getTerminalOrStruct()) {
				if (child instanceof Terminal) {
					ret.add((Terminal) child);
				}
			}
			
		} else {
			ret = Collections.emptyList();
		}
		return ret;
	}
	
	/**
	 * Init the control bar
	 */
	private void initControlBar() {
		javax.swing.JPanel controlTop = new javax.swing.JPanel();
		javax.swing.JPanel controlBottom = new javax.swing.JPanel();

		solver.setText(ModelicaMessages.SOLVER + " :");
		controlTop.add(solver);

		solverComboBox.setModel(new javax.swing.DefaultComboBoxModel(
				ModelicaController.ComputationMethod.values()));
		solverComboBox.setToolTipText(ModelicaMessages.INITIAL_COMPUTING_METHOD);

		controlTop.add(solverComboBox);

		embeddedParametersButton.setText(ModelicaMessages.PARAMETER_EMBEDDING);
		embeddedParametersButton.setToolTipText(ModelicaMessages.PARAMETER_EMBEDDING_EXPLAINED);
		controlTop.add(embeddedParametersButton);
		generateJacobianButton.setText(ModelicaMessages.GENERATE_JACOBIAN);
		generateJacobianButton.setEnabled(false);
		controlTop.add(generateJacobianButton);

		controlBar.add(controlTop);

		solveButton.setAction(new SolveAction(controller));
		controlBottom.add(solveButton);
		controlBottom.add(dummySolvePane);

		error.setTitle(ModelicaMessages.ERROR);
		error.setColumns(ERROR_COLUMNS);
		controlBottom.add(error);

		controlBar.add(controlBottom);
	}

	/**
	 * 
	 */
	private void initExtendedStatus() {
		diffSt.setTitle(ModelicaMessages.DIFF_ST);
		extendedStatus.add(diffSt);
		fixedParams.setTitle(ModelicaMessages.FIXED_PARAMS);
		extendedStatus.add(fixedParams);
		relaxedParams.setTitle(ModelicaMessages.RELAXED_PARAMS);
		extendedStatus.add(relaxedParams);
		fixedVars.setTitle(ModelicaMessages.FIXED_VARIABLES);
		extendedStatus.add(fixedVars);
		relaxedVars.setTitle(ModelicaMessages.RELAXED_VARIABLES);
		extendedStatus.add(relaxedVars);
		discrete.setTitle(ModelicaMessages.DISCRETE);
		extendedStatus.add(discrete);
		input.setTitle(ModelicaMessages.INPUT);
		extendedStatus.add(input);
	}

	/**
	 * 
	 */
	private void initGlobalStatus() {
		javax.swing.JPanel globalTop = new javax.swing.JPanel();
		javax.swing.JPanel globalBottom = new javax.swing.JPanel();

		equation.setTitle(ModelicaMessages.EQUATIONS);
		globalTop.add(equation);
		unknowns.setTitle(ModelicaMessages.UNKNOWNS);
		globalTop.add(unknowns);
		reduced.setTitle(ModelicaMessages.REDUCED);
		globalTop.add(reduced);

		globalStatus.add(globalTop);

		fixStates.setAction(new FixStatesAction(controller));
		globalBottom.add(fixStates);
		fixDerivatives.setAction(new FixDerivativesAction(controller));
		globalBottom.add(fixDerivatives);

		globalStatus.add(globalBottom);
	}

	/**
	 * Setup the layouts
	 */
	private void setLayoutsAndBorders() {
		setLayout(new java.awt.BorderLayout());

		globalStatus.setBorder(javax.swing.BorderFactory
				.createTitledBorder(ModelicaMessages.GLOBAL));
		globalStatus
				.setLayout(new BoxLayout(globalStatus, BoxLayout.PAGE_AXIS));

		extendedStatus.setBorder(javax.swing.BorderFactory
				.createTitledBorder(ModelicaMessages.EXTENDED));
		extendedStatus.setLayout(new GridLayout(EXTENDED_STATUS_LAYOUT_DATA[0], EXTENDED_STATUS_LAYOUT_DATA[1]));
		
		splitPanel.setContinuousLayout(true);
		
		controlBar.setLayout(new BoxLayout(controlBar, BoxLayout.PAGE_AXIS));
	}

	/**
	 * Allocator for the fields
	 */
	private void allocateFields() {
		variableStatusBar = new javax.swing.JPanel();
		dummySolvePane = new javax.swing.JPanel();
		globalStatus = new javax.swing.JPanel();
		extendedStatus = new javax.swing.JPanel();
		equation = new LabelWithValue();
		unknowns = new LabelWithValue();
		reduced = new LabelWithValue();
		diffSt = new LabelWithValue();
		fixedParams = new LabelWithValue();
		relaxedParams = new LabelWithValue();
		fixedVars = new LabelWithValue();
		relaxedVars = new LabelWithValue();
		discrete = new LabelWithValue();
		input = new LabelWithValue();
		splitPanel = new javax.swing.JSplitPane();
		treeScrollPane = new javax.swing.JScrollPane();
		tree = new javax.swing.JTree();
		tableScrollPane = new javax.swing.JScrollPane();
		table = new javax.swing.JTable();
		controlBar = new javax.swing.JPanel();
		solver = new javax.swing.JLabel();
		solverComboBox = new javax.swing.JComboBox();
		embeddedParametersButton = new javax.swing.JCheckBox();
		generateJacobianButton = new javax.swing.JCheckBox();
		solveButton = new javax.swing.JButton();
		fixStates = new javax.swing.JButton();
		fixDerivatives = new javax.swing.JButton();
		error = new LabelWithValue();
		tableModel = new TerminalTableModel();
	}

	/**
	 * Install the listeners
	 */
	private void installListeners() {
		/*
		 * Update table on tree selection change
		 */
		tree.addTreeSelectionListener(new TreeSelectionListener() {
			@Override
			public void valueChanged(TreeSelectionEvent e) {
				tableModel.setTerminals(getTerminals(e.getPath()));
			}
		});

		/*
		 * On validity change, update the icon
		 */
		controller.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				// Update the error status
				final boolean isNotSquare = !((ModelicaController) e
						.getSource()).isSquare();
				solveButton.setVisible(!isNotSquare);
			}
		});

		/*
		 * On error change, set the message
		 */
		controller.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				error.setError(controller.getError());
			}
		});
		
		/*
		 * On Statistics update, update the labels
		 */
		final ModelStatistics statistics = controller.getStatistics();
		statistics.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				final ModelStatistics stats = (ModelStatistics) e.getSource();

				// set the values
				equation.setText(Long.toString(stats.getEquations()));
				unknowns.setText(Long.toString(stats.getUnknowns()));
				diffSt.setText(Long.toString(stats.getDerivativeStates()));
				fixedParams.setText(Long.toString(stats.getFixedParameters()));
				relaxedParams.setText(Long.toString(stats
						.getRelaxedParameters()));
				discrete.setText(Long.toString(stats.getDiscreteStates()));
				relaxedVars.setText(Long.toString(stats.getRelaxedVariables()));
				fixedVars.setText(Long.toString(stats.getFixedVariables()));
				input.setText(Long.toString(stats.getInputs()));
			}
		});

		/*
		 * Install the table mode listeners
		 */
		installTableModelListeners(statistics);

		/*
		 * Update the controller on method change
		 */
		solverComboBox.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				controller.setComputeMethod((ComputationMethod) e.getItem());
			}
		});
		
		/*
		 * The generate jacobian button must be only available on Solver=="Ida"
		 */
		solverComboBox.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				generateJacobianButton.setEnabled(e.getItem() == ModelicaController.ComputationMethod.Ida);
			}
		});

		/*
		 * Install checkbox listeners
		 */
		embeddedParametersButton.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				controller.setParameterEmbedded(e.getStateChange() == ItemEvent.SELECTED);
			}
		});
		generateJacobianButton.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				controller.setJacobianEnable(e.getStateChange() == ItemEvent.SELECTED);
			}
		});
	}

	/**
	 * Install the listener on the table model
	 * 
	 * @param statistics the statistics
	 */
	private void installTableModelListeners(final ModelStatistics statistics) {
		
		/*
		 * Update the table on a global data change
		 */
		controller.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				tableModel.fireTableDataChanged();
			}
		});
		
		/*
		 * On table modification, update the Statistics
		 */
		tableModel.addTableModelListener(new StatisticsUpdater(statistics));
		
		/*
		 * Update the fixed state
		 */
		tableModel.addTableModelListener(new TableModelListener() {
			@Override
			public void tableChanged(TableModelEvent e) {
				if (e instanceof TerminalTableModel.TerminalTableModelEvent
						&& ((TerminalTableModel.TerminalTableModelEvent) e).isAfterCommit()) {
					final int rowIndex = e.getFirstRow();
					final int columnIndex = e.getColumn();

					if (TerminalAccessor.values()[columnIndex] == WEIGHT) {
						final TerminalTableModel model = (TerminalTableModel) e.getSource();
						final Terminal terminal = model.getTerminals().get(rowIndex);
						
						if (terminal.getKind().equals("fixed_parameter") 
								|| terminal.getKind().equals("variable")) {
							final double data = (Double) getData(WEIGHT, terminal);
							final boolean isFixed = data >= 1.0;
							
							tableModel.setValueAt(isFixed, rowIndex, 
									Arrays.asList(TerminalAccessor.values()).indexOf(FIXED));
						}
						
					}
				}
			}
		});
		

		/*
		 * Set the compilation flag when embedding parameters is invalidated and
		 * a fixed parameter change.
		 */
		tableModel.addTableModelListener(new TableModelListener() {
			@Override
			public void tableChanged(TableModelEvent e) {
				if (e instanceof TerminalTableModel.TerminalTableModelEvent
						&& !embeddedParametersButton.isSelected()) {
					final int rowIndex = e.getFirstRow();
					final int columnIndex = e.getColumn();

					if (TerminalAccessor.values()[columnIndex] == TerminalAccessor.WEIGHT) {
						final TerminalTableModel model = (TerminalTableModel) e
								.getSource();
						final Terminal terminal = model.getTerminals().get(
								rowIndex);
						final Double weight = TerminalAccessor.getData(
								TerminalAccessor.WEIGHT, terminal);

						if (weight.doubleValue() >= 1.0) {
							controller.setCompileNeeded(true);
						}
					}
				}
			}
		});
	}

	/**
	 * Fire all update events at the end of the init
	 */
	private void fireChange() {
		controller.getStatistics().fireChange();
		controller.fireChange();
	}
}
//CSON: FanOutComplexity
//CSON: DataAbstractionCoupling
