/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Choice;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.InputVerifier;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Setup dialog for Xcos
 */
public class SetupAction extends SimulationNotRunningAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.SETUP;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;

	private static final DecimalFormatSymbols FORMAT_SYMBOL = new DecimalFormatSymbols();
	private static final DecimalFormat CURRENT_FORMAT = new DecimalFormat("0.0####E00;0", FORMAT_SYMBOL);
	
	private static final InputVerifier VALIDATE_POSITIVE_DOUBLE = new InputVerifier() {
	    public boolean verify(javax.swing.JComponent arg0) {
		boolean ret = false;
		JFormattedTextField textField = (JFormattedTextField) arg0;
		try {
        		BigDecimal value = new BigDecimal(textField.getText());
        		if (value.compareTo(new BigDecimal(0)) >= 0) {
        		    ret = true;
        		}
		} catch (NumberFormatException e) {
		    return ret;
		}
		return ret;
		
	    };
	};
	
	static {
    	FORMAT_SYMBOL.setDecimalSeparator('.');
    	CURRENT_FORMAT.setDecimalFormatSymbols(FORMAT_SYMBOL);
    	CURRENT_FORMAT.setParseIntegerOnly(false);
    	CURRENT_FORMAT.setParseBigDecimal(true);
	}
	
	private boolean windowAlreadyExist;
	private JFrame mainFrame;
	private XcosDiagram diagram;
	private JFormattedTextField integration;
	private JFormattedTextField rts;
	private JFormattedTextField integrator;
	private JFormattedTextField integratorRel;
	private JFormattedTextField toleranceOnTime;
	private JFormattedTextField maxIntegrationTime;
	private Choice   solverChoice;
	private JSpinner maxStepSize;
	
	/**
	 * Constructor
	 * @param scilabGraph Associated Scilab Graph
	 */
	public SetupAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create corresponding menu
	 * @param scilabGraph Associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, SetupAction.class);
	}

	/**
	 * Action !
	 * @param e the event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		setupBox(e);
	}


	/**
	 * Create the setup Window
	 * @param e the event
	 */
	public void setupBox(ActionEvent e) {

		/** Avoid to have this window created two times */
		if (windowAlreadyExist) {
			mainFrame.setVisible(true);
			return;
		}

		Icon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
		Image imageForIcon = ((ImageIcon) scilabIcon).getImage();

		mainFrame = new JFrame();
		windowAlreadyExist = true;

		mainFrame.setLayout(new GridBagLayout());
		mainFrame.setIconImage(imageForIcon);

		diagram = (XcosDiagram) getGraph(e);

		JLabel integrationLabel = new JLabel(XcosMessages.FINAL_INTEGRATION_TIME);
		integration = new JFormattedTextField(CURRENT_FORMAT);
		integration.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		integration.setValue(new BigDecimal(diagram.getFinalIntegrationTime()));

		JLabel rtsLabel = new JLabel(XcosMessages.REAL_TIME_SCALING);
		rts = new JFormattedTextField(CURRENT_FORMAT);
		rts.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		rts.setValue(new BigDecimal(diagram.getRealTimeScaling()));

		JLabel integratorAbsLabel = new JLabel(XcosMessages.INTEGRATOR_ABSOLUTE_TOLERANCE);
		integrator = new JFormattedTextField(CURRENT_FORMAT);
		integrator.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		integrator.setValue(new BigDecimal(diagram.getIntegratorAbsoluteTolerance()));

		JLabel integratorRelLabel = new JLabel(XcosMessages.INTEGRATOR_RELATIVE_TOLERANCE);
		integratorRel = new JFormattedTextField(CURRENT_FORMAT);
		integratorRel.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		integratorRel.setValue(new BigDecimal(diagram.getIntegratorRelativeTolerance()));

		JLabel toleranceOnTimeLabel = new JLabel(XcosMessages.TOLERANCE_ON_TIME);
		toleranceOnTime = new JFormattedTextField(CURRENT_FORMAT);
		toleranceOnTime.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		toleranceOnTime.setValue(new BigDecimal(diagram.getToleranceOnTime()));

		JLabel maxIntegrationTimeLabel = new JLabel(XcosMessages.MAX_INTEGRATION_TIME_INTERVAL);
		maxIntegrationTime = new JFormattedTextField(CURRENT_FORMAT);
		maxIntegrationTime.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		maxIntegrationTime.setValue(new BigDecimal(diagram.getMaxIntegrationTimeinterval()));

		JLabel solverLabel = new JLabel(XcosMessages.SOLVER_CHOICE);
		solverChoice = new Choice();
		solverChoice.addItem(XcosMessages.CVODE);
		solverChoice.addItem(XcosMessages.IDA);
		if (diagram.getSolver() == 0.0) {
			solverChoice.select(0);
		} else {
			solverChoice.select(1);
		}



		JLabel maxStepSizeLabel = new JLabel(XcosMessages.MAXIMUN_STEP_SIZE);
		SpinnerNumberModel spinnerModel = new SpinnerNumberModel((int) diagram.getMaximumStepSize(), 0, null, 1);
		maxStepSize = new JSpinner();
		maxStepSize.setModel(spinnerModel);
		maxStepSize.setEditor(new JSpinner.NumberEditor(maxStepSize, "0"));

		JButton cancelButton = new JButton(XcosMessages.CANCEL);
		JButton okButton = new JButton(XcosMessages.OK);
		JButton defaultButton = new JButton(XcosMessages.DEFAULT);
		JButton setContextButton = new JButton(XcosMessages.SET_CONTEXT);
		okButton.setPreferredSize(cancelButton.getPreferredSize());

		GridBagConstraints gbc = new GridBagConstraints();

		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.gridheight = 1;
		gbc.gridwidth = 1;
		gbc.insets = new Insets(0, 10, 0, 0);




		gbc.gridx = 0;
		gbc.gridy = 4;
		gbc.gridheight = 1;
		gbc.gridwidth = 1;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(0, 10, 0, 0);


		mainFrame.add(integrationLabel, gbc);

		gbc.gridy = 5;
		mainFrame.add(rtsLabel, gbc);

		gbc.gridy = 6;
		mainFrame.add(integratorAbsLabel, gbc);

		gbc.gridy = 7;
		mainFrame.add(integratorRelLabel, gbc);

		gbc.gridy = 8;
		mainFrame.add(toleranceOnTimeLabel, gbc);

		gbc.gridy = 9;
		mainFrame.add(maxIntegrationTimeLabel, gbc);

		gbc.gridy = 10;
		mainFrame.add(solverLabel, gbc);

		gbc.gridy = 11;
		mainFrame.add(maxStepSizeLabel, gbc);

		gbc.gridy = 12;
		mainFrame.add(setContextButton, gbc);


		gbc.gridx = 1;
		gbc.gridy = 4;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(5, 10, 0, 10);
		mainFrame.add(integration, gbc);

		gbc.gridy = 5;
		mainFrame.add(rts, gbc);

		gbc.gridy = 6;
		mainFrame.add(integrator, gbc);

		gbc.gridy = 7;
		mainFrame.add(integratorRel, gbc);

		gbc.gridy = 8;
		mainFrame.add(toleranceOnTime, gbc);

		gbc.gridy = 9;
		mainFrame.add(maxIntegrationTime, gbc);

		gbc.gridy = 10;
		mainFrame.add(solverChoice, gbc);

		gbc.gridy = 11;
		mainFrame.add(maxStepSize, gbc);


		gbc.gridx = 1;
		gbc.gridy = 14;
		gbc.gridheight = gbc.gridwidth = 1;
		gbc.weightx = 1.;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(5, 0, 10, 5);
		mainFrame.add(okButton, gbc);


		gbc.gridx = 2;
		gbc.weightx = 0.;
		gbc.insets = new Insets(5, 0, 10, 10);
		mainFrame.add(cancelButton, gbc);


		gbc.gridx = 3;
		gbc.weightx = 0.;
		gbc.insets = new Insets(5, 0, 10, 10);
		mainFrame.add(defaultButton, gbc);


		cancelButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				windowAlreadyExist = false;
				mainFrame.dispose();
			}
		});

		defaultButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {

				integration.setValue(new BigDecimal(100000.0)); //TODO thou shall not let value hardcoded!
				rts.setValue(new BigDecimal(0.0));
				integrator.setValue(new BigDecimal(1e-4));
				integratorRel.setValue(new BigDecimal(1e-6));
				toleranceOnTime.setValue(new BigDecimal(1e-10));
				maxIntegrationTime.setValue(new BigDecimal(100001.0));
				solverChoice.select(0);
				maxStepSize.setValue(0);

				diagram.setFinalIntegrationTime((Double) integration.getValue());
				diagram.setRealTimeScaling((Double) rts.getValue());
				diagram.setIntegratorAbsoluteTolerance((Double) integrator.getValue());
				diagram.setIntegratorRelativeTolerance((Double) integratorRel.getValue());
				diagram.setToleranceOnTime((Double) toleranceOnTime.getValue());
				diagram.setMaxIntegrationTimeinterval((Double) maxIntegrationTime.getValue());
				diagram.setSolver(0);
				diagram.setMaximumStepSize(((Integer) maxStepSize.getValue()).doubleValue());
			}
		});

		okButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
			    if (((JButton) e.getSource()).hasFocus()) {
				if (solverChoice.getSelectedItem().equals(XcosMessages.CVODE)) {
					if (diagram.getSolver() != 0) {
						diagram.setSolver(0);
						diagram.setModified(true);
					}
				} else {
					if (diagram.getSolver() != 100) {
						diagram.setSolver(100);
						diagram.setModified(true);
					}
				}

				if (diagram.getFinalIntegrationTime() != ((BigDecimal) integration.getValue()).doubleValue()) {
					diagram.setFinalIntegrationTime(((BigDecimal) integration.getValue()).doubleValue());
					diagram.setModified(true);
				}
				if (diagram.getRealTimeScaling() != ((BigDecimal) rts.getValue()).doubleValue()) {
					diagram.setRealTimeScaling(((BigDecimal) rts.getValue()).doubleValue());
					diagram.setModified(true);
				}
				if (diagram.getIntegratorAbsoluteTolerance() != ((BigDecimal) integrator.getValue()).doubleValue()) {
					diagram.setIntegratorAbsoluteTolerance(((BigDecimal) integrator.getValue()).doubleValue());
					diagram.setModified(true);
				}
				if (diagram.getIntegratorRelativeTolerance() != ((BigDecimal) integratorRel.getValue()).doubleValue()) {
					diagram.setIntegratorRelativeTolerance(((BigDecimal) integratorRel.getValue()).doubleValue());
					diagram.setModified(true);
				}
				if (diagram.getToleranceOnTime() != ((BigDecimal) toleranceOnTime.getValue()).doubleValue()) {
					diagram.setToleranceOnTime(((BigDecimal) toleranceOnTime.getValue()).doubleValue());
					diagram.setModified(true);
				}
				if (diagram.getMaxIntegrationTimeinterval() != ((BigDecimal) maxIntegrationTime.getValue()).doubleValue()) {
					diagram.setMaxIntegrationTimeinterval(((BigDecimal) maxIntegrationTime.getValue()).doubleValue());
					diagram.setModified(true);
				}
				if (diagram.getMaximumStepSize() != ((Integer) maxStepSize.getValue()).doubleValue()) {
					diagram.setMaximumStepSize(((Integer) maxStepSize.getValue()).doubleValue());
					diagram.setModified(true);
				}

				windowAlreadyExist = false;
				mainFrame.dispose();
			    }
			}
		});

		setContextButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				diagram.getContextAction().actionPerformed(e);
			}
		});

		//display the frame and set some properties

		mainFrame.addWindowListener(new WindowListener() {
			public void windowClosed(WindowEvent e) { }
			public void windowDeiconified(WindowEvent e) { }
			public void windowActivated(WindowEvent e) { }
			
			public void windowClosing(WindowEvent e) {
				windowAlreadyExist = false;
				mainFrame.dispose();
			}
			
			public void windowDeactivated(WindowEvent e) { }
			public void windowIconified(WindowEvent e) { }
			public void windowOpened(WindowEvent e) { }
		});

		mainFrame.setTitle(XcosMessages.SETUP_TITLE);
		mainFrame.pack();
		mainFrame.setLocationRelativeTo(getGraph(e).getAsComponent());
		mainFrame.setVisible(true);	
	}
}
