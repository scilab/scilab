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
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.NumberFormat;
import java.util.Locale;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
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

import sun.misc.FormattedFloatingDecimal;

/**
 * Setup dialog dor Xcos
 * @author Allan SIMON
 */
public class SetupAction extends DefaultAction {

	private static final long serialVersionUID = 1L;
	
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
		final DecimalFormat currentFormat = new DecimalFormat("0.0####E00", formatSymbol);
		currentFormat.setDecimalFormatSymbols(formatSymbol);
		currentFormat.setParseIntegerOnly(false);
		currentFormat.setParseBigDecimal(true);

		mainFrame = new JFrame();
		windowAlreadyExist = true;

		mainFrame.setLayout(new GridBagLayout());
		mainFrame.setIconImage(imageForIcon);

		diagram = (XcosDiagram) getGraph(e);

		JLabel integrationLabel = new JLabel(XcosMessages.FINAL_INTEGRATION_TIME);
		integration = new JFormattedTextField(currentFormat);
		integration.setValue(diagram.getFinalIntegrationTime());

		JLabel rtsLabel = new JLabel(XcosMessages.REAL_TIME_SCALING);
		rts = new JFormattedTextField(currentFormat);
		rts.setValue(diagram.getRealTimeScaling());

		JLabel integratorAbsLabel = new JLabel(XcosMessages.INTEGRATOR_ABSOLUTE_TOLERANCE);
		integrator = new JFormattedTextField(currentFormat);
		integrator.setValue(diagram.getIntegratorAbsoluteTolerance());

		JLabel integratorRelLabel = new JLabel(XcosMessages.INTEGRATOR_RELATIVE_TOLERANCE);
		integratorRel = new JFormattedTextField(currentFormat);
		integratorRel.setValue(diagram.getIntegratorRelativeTolerance());

		JLabel toleranceOnTimeLabel = new JLabel(XcosMessages.TOLERANCE_ON_TIME);
		toleranceOnTime = new JFormattedTextField(currentFormat);
		toleranceOnTime.setValue(diagram.getToleranceOnTime());

		JLabel maxIntegrationTimeLabel = new JLabel(XcosMessages.MAX_INTEGRATION_TIME_INTERVAL);
		maxIntegrationTime = new JFormattedTextField(currentFormat);
		maxIntegrationTime.setValue(diagram.getMaxIntegrationTimeinterval());

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

				integration.setValue(100000.0);//TODO thou shall not let value hardcoded!
				rts.setValue(0.0);
				integrator.setValue(1e-4);
				integratorRel.setValue(1e-6);
				toleranceOnTime.setValue(1e-10);
				maxIntegrationTime.setValue(100001.0);
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

				if (diagram.getFinalIntegrationTime() != (Double) integration.getValue()) {
					diagram.setFinalIntegrationTime((Double) integration.getValue());
					diagram.setModified(true);
				}
				if (diagram.getRealTimeScaling() != (Double) rts.getValue()) {
					diagram.setRealTimeScaling((Double) rts.getValue());
					diagram.setModified(true);
				}
				if (diagram.getIntegratorAbsoluteTolerance() != (Double) integrator.getValue()) {
					diagram.setIntegratorAbsoluteTolerance((Double) integrator.getValue());
					diagram.setModified(true);
				}
				if (diagram.getIntegratorRelativeTolerance() != (Double) integratorRel.getValue()) {
					diagram.setIntegratorRelativeTolerance((Double) integratorRel.getValue());
					diagram.setModified(true);
				}
				if (diagram.getToleranceOnTime() != (Double) toleranceOnTime.getValue()) {
					diagram.setToleranceOnTime((Double) toleranceOnTime.getValue());
					diagram.setModified(true);
				}
				if (diagram.getMaxIntegrationTimeinterval() != (Double) maxIntegrationTime.getValue()) {
					diagram.setMaxIntegrationTimeinterval((Double) maxIntegrationTime.getValue());
					diagram.setModified(true);
				}
				if (diagram.getMaximumStepSize() != ((Integer) maxStepSize.getValue()).doubleValue()) {
					diagram.setMaximumStepSize(((Integer) maxStepSize.getValue()).doubleValue());
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
