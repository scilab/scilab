/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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
import java.text.DecimalFormatSymbols;
import java.util.Locale;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.JSpinner.NumberEditor;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Setup dialog dor Xcos
 * @author Allan SIMON
 */
public class SetupAction extends DefaultAction {

	private static final long serialVersionUID = 1L;
	
	private boolean windowAlreadyExist;
	private JFrame mainFrame;
	private XcosDiagram diagram;
	private JSpinner integrationSpinner;
	private JSpinner rtsSpinner;
	private JSpinner integratorAbsSpinner;
	private JSpinner integratorRelSpinner;
	private JSpinner toleranceOnTimeSpinner;
	private JSpinner maxIntegrationTimeSpinner;
	private Choice   solverChoice;
	private JSpinner maxStepSizeSpinner;

	/**
	 * Constructor
	 * @param scilabGraph Associated Scilab Graph
	 */
	public SetupAction(ScilabGraph scilabGraph) {
		super(XcosMessages.SETUP, scilabGraph);
	}

	/**
	 * Create corresponding menu
	 * @param scilabGraph Associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.SETUP, null, new SetupAction(scilabGraph), null);
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
		DecimalFormatSymbols formatSymbol = new DecimalFormatSymbols();
		formatSymbol.setDecimalSeparator('.');

		mainFrame = new JFrame();
		windowAlreadyExist = true;

		mainFrame.setLayout(new GridBagLayout());
		mainFrame.setIconImage(imageForIcon);

		diagram = (XcosDiagram) getGraph(e);

		JLabel integrationLabel = new JLabel(XcosMessages.FINAL_INTEGRATION_TIME);
		SpinnerNumberModel spinnerModel = new SpinnerNumberModel(0.0, null, null, 0.01);
		integrationSpinner = new JSpinner();
		integrationSpinner.setModel(spinnerModel);
		integrationSpinner.setEditor(new JSpinner.NumberEditor(integrationSpinner, "0.00"));
		((NumberEditor) integrationSpinner.getEditor()).getFormat().setDecimalFormatSymbols(formatSymbol);
		spinnerModel.setValue(diagram.getFinalIntegrationTime());

		JLabel rtsLabel = new JLabel(XcosMessages.REAL_TIME_SCALING);
		spinnerModel = new SpinnerNumberModel(0.1, null, null, 0.1);
		rtsSpinner = new JSpinner();
		rtsSpinner.setModel(spinnerModel);
		rtsSpinner.setEditor(new JSpinner.NumberEditor(rtsSpinner, "0.0"));
		((NumberEditor) rtsSpinner.getEditor()).getFormat().setDecimalFormatSymbols(formatSymbol);
		spinnerModel.setValue(diagram.getRealTimeScaling());

		JLabel integratorAbsLabel = new JLabel(XcosMessages.INTEGRATOR_ABSOLUTE_TOLERANCE);
		spinnerModel = new SpinnerNumberModel(0.0, null, null, 0.00001);
		integratorAbsSpinner = new JSpinner();
		integratorAbsSpinner.setModel(spinnerModel);
		integratorAbsSpinner.setEditor(new JSpinner.NumberEditor(integratorAbsSpinner, "0.00000"));
		((NumberEditor) integratorAbsSpinner.getEditor()).getFormat().setDecimalFormatSymbols(formatSymbol);
		spinnerModel.setValue(diagram.getIntegratorAbsoluteTolerance());

		JLabel integratorRelLabel = new JLabel(XcosMessages.INTEGRATOR_RELATIVE_TOLERANCE);
		spinnerModel = new SpinnerNumberModel(0.0, null, null, 0.0000001);
		integratorRelSpinner = new JSpinner();
		integratorRelSpinner.setModel(spinnerModel);
		integratorRelSpinner.setEditor(new JSpinner.NumberEditor(integratorRelSpinner, "0.0000000"));
		((NumberEditor) integratorRelSpinner.getEditor()).getFormat().setDecimalFormatSymbols(formatSymbol);
		spinnerModel.setValue(diagram.getIntegratorRelativeTolerance());

		JLabel toleranceOnTimeLabel = new JLabel(XcosMessages.TOLERANCE_ON_TIME);
		spinnerModel = new SpinnerNumberModel(0.0, null, null, 1.000E-11);
		toleranceOnTimeSpinner = new JSpinner();
		toleranceOnTimeSpinner.setModel(spinnerModel);
		toleranceOnTimeSpinner.setEditor(new JSpinner.NumberEditor(toleranceOnTimeSpinner, "0.000E00"));
		((NumberEditor) toleranceOnTimeSpinner.getEditor()).getFormat().setDecimalFormatSymbols(formatSymbol);
		spinnerModel.setValue(diagram.getToleranceOnTime());

		JLabel maxIntegrationTimeLabel = new JLabel(XcosMessages.MAX_INTEGRATION_TIME_INTERVAL);
		spinnerModel = new SpinnerNumberModel(0, null, null, 1);
		maxIntegrationTimeSpinner = new JSpinner();
		maxIntegrationTimeSpinner.setModel(spinnerModel);
		maxIntegrationTimeSpinner.setEditor(new JSpinner.NumberEditor(maxIntegrationTimeSpinner, "0"));
		((NumberEditor) maxIntegrationTimeSpinner.getEditor()).getFormat().setDecimalFormatSymbols(formatSymbol);
		spinnerModel.setValue(diagram.getMaxIntegrationTimeinterval());

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
		spinnerModel = new SpinnerNumberModel((int) diagram.getMaximumStepSize(), 0, null, 1);
		maxStepSizeSpinner = new JSpinner();
		maxStepSizeSpinner.setModel(spinnerModel);
		maxStepSizeSpinner.setEditor(new JSpinner.NumberEditor(maxStepSizeSpinner, "0"));

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
		mainFrame.add(integrationSpinner, gbc);

		gbc.gridy = 5;
		mainFrame.add(rtsSpinner, gbc);

		gbc.gridy = 6;
		mainFrame.add(integratorAbsSpinner, gbc);

		gbc.gridy = 7;
		mainFrame.add(integratorRelSpinner, gbc);

		gbc.gridy = 8;
		mainFrame.add(toleranceOnTimeSpinner, gbc);

		gbc.gridy = 9;
		mainFrame.add(maxIntegrationTimeSpinner, gbc);

		gbc.gridy = 10;
		mainFrame.add(solverChoice, gbc);

		gbc.gridy = 11;
		mainFrame.add(maxStepSizeSpinner, gbc);


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

				integrationSpinner.setValue(100000.0);//TODO thou shall not let value hardcoded!
				rtsSpinner.setValue(0.0);
				integratorAbsSpinner.setValue(1e-4);
				integratorRelSpinner.setValue(1e-6);
				toleranceOnTimeSpinner.setValue(1e-10);
				maxIntegrationTimeSpinner.setValue(100001.0);
				solverChoice.select(0);
				maxStepSizeSpinner.setValue(0);

				diagram.setFinalIntegrationTime((Double) integrationSpinner.getValue());
				diagram.setRealTimeScaling((Double) rtsSpinner.getValue());
				diagram.setIntegratorAbsoluteTolerance((Double) integratorAbsSpinner.getValue());
				diagram.setIntegratorRelativeTolerance((Double) integratorRelSpinner.getValue());
				diagram.setToleranceOnTime((Double) toleranceOnTimeSpinner.getValue());
				diagram.setMaxIntegrationTimeinterval((Double) maxIntegrationTimeSpinner.getValue());
				diagram.setSolver(0);
				diagram.setMaximumStepSize(((Integer) maxStepSizeSpinner.getValue()).doubleValue());
			}
		});

		okButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {

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

				if (diagram.getFinalIntegrationTime() != (Double) integrationSpinner.getValue()) {
					diagram.setFinalIntegrationTime((Double) integrationSpinner.getValue());
					diagram.setModified(true);
				}
				if (diagram.getRealTimeScaling() != (Double) rtsSpinner.getValue()) {
					diagram.setRealTimeScaling((Double) rtsSpinner.getValue());
					diagram.setModified(true);
				}
				if (diagram.getIntegratorAbsoluteTolerance() != (Double) integratorAbsSpinner.getValue()) {
					diagram.setIntegratorAbsoluteTolerance((Double) integratorAbsSpinner.getValue());
					diagram.setModified(true);
				}
				if (diagram.getIntegratorRelativeTolerance() != (Double) integratorRelSpinner.getValue()) {
					diagram.setIntegratorRelativeTolerance((Double) integratorRelSpinner.getValue());
					diagram.setModified(true);
				}
				if (diagram.getToleranceOnTime() != (Double) toleranceOnTimeSpinner.getValue()) {
					diagram.setToleranceOnTime((Double) toleranceOnTimeSpinner.getValue());
					diagram.setModified(true);
				}
				if (diagram.getMaxIntegrationTimeinterval() != (Double) maxIntegrationTimeSpinner.getValue()) {
					diagram.setMaxIntegrationTimeinterval((Double) maxIntegrationTimeSpinner.getValue());
					diagram.setModified(true);
				}
				if (diagram.getMaximumStepSize() != ((Integer) maxStepSizeSpinner.getValue()).doubleValue()) {
					diagram.setMaximumStepSize(((Integer) maxStepSizeSpinner.getValue()).doubleValue());
					diagram.setModified(true);
				}

				windowAlreadyExist = false;
				mainFrame.dispose();
			}
		});

		setContextButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				diagram.getContextAction().actionPerformed(e);
			}
		});

		//display the frame and set some properties

		mainFrame.addWindowListener(new WindowListener() {
			public void windowClosed(WindowEvent arg0) {
				// TODO Auto-generated method stub

			}
			public void windowDeiconified(WindowEvent arg0) {
				// TODO Auto-generated method stub

			}
			public void windowActivated(WindowEvent arg0) {
				// TODO Auto-generated method stub

			}
			public void windowClosing(WindowEvent arg0) {
				windowAlreadyExist = false;
				mainFrame.dispose();

			}
			public void windowDeactivated(WindowEvent arg0) {
				// TODO Auto-generated method stub

			}
			public void windowIconified(WindowEvent arg0) {

			};
			public void windowOpened(WindowEvent arg0) {
				// TODO Auto-generated method stub

			}

		});

		mainFrame.setTitle(XcosMessages.SETUP_TITLE);
		mainFrame.pack();
		mainFrame.setLocationRelativeTo(getGraph(e).getAsComponent());
		mainFrame.setVisible(true);	
	}
}
