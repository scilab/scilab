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

package org.scilab.modules.xcos.actions.dialog;

import java.awt.Choice;
import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyVetoException;
import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.InputVerifier;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Dialog associated with the {@link SetupAction}.
 * 
 * Note that this dialog break the Data Abstraction Coupling metric because of
 * the numbers of graphical components involved in the GUI creation. For the
 * same reason (GUI class), constants are not used on this code.
 */
//CSOFF: ClassDataAbstractionCoupling
//CSOFF: ClassFanOutComplexity
//CSOFF: MagicNumber
public class SetupDialog extends JDialog {
	private static final DecimalFormatSymbols FORMAT_SYMBOL = DecimalFormatSymbols.getInstance();
	private static final DecimalFormat CURRENT_FORMAT = new DecimalFormat("0.0####E00", FORMAT_SYMBOL);
	private static final BigDecimal MAX_DOUBLE = BigDecimal.valueOf(Double.MAX_VALUE);
	
	/**
	 * Validate the user entry and format it.
	 * 
	 * Without formatting the entry, bug #7143 appears on jdk6.
	 */
	private static final InputVerifier VALIDATE_POSITIVE_DOUBLE = new InputVerifier() {

		@Override
	    public boolean verify(javax.swing.JComponent arg0) {
		boolean ret = false;
		JFormattedTextField textField = (JFormattedTextField) arg0;
		try {
        		BigDecimal value = new BigDecimal(textField.getText());
        		if (value.compareTo(BigDecimal.ZERO) >= 0 
        				&& value.compareTo(MAX_DOUBLE) <= 0) {
        		    ret = true;
        		}
        		
        		// bug #7143 workaround
        		textField.setValue(value);
		} catch (NumberFormatException e) {
		    return ret;
		}
		return ret;
		
	    };
	};
	
	/**
	 * Initialize static final fields
	 */
	static {
    	FORMAT_SYMBOL.setDecimalSeparator('.');
    	CURRENT_FORMAT.setDecimalFormatSymbols(FORMAT_SYMBOL);
    	CURRENT_FORMAT.setParseIntegerOnly(false);
    	CURRENT_FORMAT.setParseBigDecimal(true);
	}
	
	
	private final ScicosParameters parameters;
	
	private JFormattedTextField integration;
	private JFormattedTextField rts;
	private JFormattedTextField integrator;
	private JFormattedTextField integratorRel;
	private JFormattedTextField toleranceOnTime;
	private JFormattedTextField maxIntegrationTime;
	private Choice solverChoice;
	private JSpinner maxStepSize;
	
	/**
	 * Instanciate a new dialog.
	 * 
	 * @param parent the current selected graph component
	 * @param parameters the current parameters
	 */
	public SetupDialog(Component parent, ScicosParameters parameters) {
		super();
		
		this.parameters = parameters;
		
		Icon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
		Image imageForIcon = ((ImageIcon) scilabIcon).getImage();
		setLayout(new GridBagLayout());
		setIconImage(imageForIcon);
		setTitle(XcosMessages.SETUP_TITLE);
		setModal(true);
		setLocationRelativeTo(parent);
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		
		initComponents();
	}

	/**
	 * Initialize the dialog components.
	 * 
	 * As this method perform a complex initialization, It doesn't pass NCSS. 
	 */
	//CSOFF: JavaNCSS
	private void initComponents() {
		JLabel integrationLabel = new JLabel(XcosMessages.FINAL_INTEGRATION_TIME);
		integration = new JFormattedTextField(CURRENT_FORMAT);
		integration.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		integration.setValue(new BigDecimal(parameters.getFinalIntegrationTime()));

		JLabel rtsLabel = new JLabel(XcosMessages.REAL_TIME_SCALING);
		rts = new JFormattedTextField(CURRENT_FORMAT);
		rts.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		rts.setValue(new BigDecimal(parameters.getRealTimeScaling()));

		JLabel integratorAbsLabel = new JLabel(XcosMessages.INTEGRATOR_ABSOLUTE_TOLERANCE);
		integrator = new JFormattedTextField(CURRENT_FORMAT);
		integrator.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		integrator.setValue(new BigDecimal(parameters.getIntegratorAbsoluteTolerance()));

		JLabel integratorRelLabel = new JLabel(XcosMessages.INTEGRATOR_RELATIVE_TOLERANCE);
		integratorRel = new JFormattedTextField(CURRENT_FORMAT);
		integratorRel.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		integratorRel.setValue(new BigDecimal(parameters.getIntegratorRelativeTolerance()));

		JLabel toleranceOnTimeLabel = new JLabel(XcosMessages.TOLERANCE_ON_TIME);
		toleranceOnTime = new JFormattedTextField(CURRENT_FORMAT);
		toleranceOnTime.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		toleranceOnTime.setValue(new BigDecimal(parameters.getToleranceOnTime()));

		JLabel maxIntegrationTimeLabel = new JLabel(XcosMessages.MAX_INTEGRATION_TIME_INTERVAL);
		maxIntegrationTime = new JFormattedTextField(CURRENT_FORMAT);
		maxIntegrationTime.setInputVerifier(VALIDATE_POSITIVE_DOUBLE);
		maxIntegrationTime.setValue(new BigDecimal(parameters.getMaxIntegrationTimeInterval()));

		JLabel solverLabel = new JLabel(XcosMessages.SOLVER_CHOICE);
		solverChoice = new Choice();
		solverChoice.addItem(XcosMessages.CVODE);
		solverChoice.addItem(XcosMessages.IDA);
		if (parameters.getSolver() == 0.0) {
			solverChoice.select(0);
		} else {
			solverChoice.select(1);
		}

		JLabel maxStepSizeLabel = new JLabel(XcosMessages.MAXIMUN_STEP_SIZE);
		SpinnerNumberModel spinnerModel = new SpinnerNumberModel((int) parameters.getMaximumStepSize(), 0, null, 1);
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


		add(integrationLabel, gbc);

		gbc.gridy = 5;
		add(rtsLabel, gbc);

		gbc.gridy = 6;
		add(integratorAbsLabel, gbc);

		gbc.gridy = 7;
		add(integratorRelLabel, gbc);

		gbc.gridy = 8;
		add(toleranceOnTimeLabel, gbc);

		gbc.gridy = 9;
		add(maxIntegrationTimeLabel, gbc);

		gbc.gridy = 10;
		add(solverLabel, gbc);

		gbc.gridy = 11;
		add(maxStepSizeLabel, gbc);

		gbc.gridy = 12;
		add(setContextButton, gbc);


		gbc.gridx = 1;
		gbc.gridy = 4;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(5, 10, 0, 10);
		add(integration, gbc);

		gbc.gridy = 5;
		add(rts, gbc);

		gbc.gridy = 6;
		add(integrator, gbc);

		gbc.gridy = 7;
		add(integratorRel, gbc);

		gbc.gridy = 8;
		add(toleranceOnTime, gbc);

		gbc.gridy = 9;
		add(maxIntegrationTime, gbc);

		gbc.gridy = 10;
		add(solverChoice, gbc);

		gbc.gridy = 11;
		add(maxStepSize, gbc);


		gbc.gridx = 1;
		gbc.gridy = 14;
		gbc.gridheight = 1;
		gbc.gridwidth = 1;
		gbc.weightx = 1.;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(5, 0, 10, 5);
		add(okButton, gbc);


		gbc.gridx = 2;
		gbc.weightx = 0.;
		gbc.insets = new Insets(5, 0, 10, 10);
		add(cancelButton, gbc);


		gbc.gridx = 3;
		gbc.weightx = 0.;
		gbc.insets = new Insets(5, 0, 10, 10);
		add(defaultButton, gbc);

		installActionListeners(cancelButton, okButton, defaultButton,
				setContextButton);
	}
	//CSON: JavaNCSS

	/**
	 * Install the action listeners on the buttons
	 * 
	 * @param cancelButton the cancel button (Cancel)
	 * @param okButton the OK button (Validate)
	 * @param defaultButton the default button (Reset)
	 * @param setContextButton the context button (Shortcut to set context)
	 */
	private void installActionListeners(JButton cancelButton, JButton okButton,
			JButton defaultButton, JButton setContextButton) {
		cancelButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				dispose();
			}
		});

		defaultButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				integration.setValue(new BigDecimal(ScicosParameters.FINAL_INTEGRATION_TIME));
				rts.setValue(new BigDecimal(ScicosParameters.REAL_TIME_SCALING));
				integrator.setValue(new BigDecimal(ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE));
				integratorRel.setValue(new BigDecimal(ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE));
				toleranceOnTime.setValue(new BigDecimal(ScicosParameters.TOLERANCE_ON_TIME));
				maxIntegrationTime.setValue(new BigDecimal(ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL));
				solverChoice.select((int) (ScicosParameters.SOLVER / 100.0));
				maxStepSize.setValue((int) ScicosParameters.MAXIMUM_STEP_SIZE);
			}
		});

		okButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
			    if (((JButton) e.getSource()).hasFocus()) {
			    try {
				    /*
				     * FIXME This logic must be deported to a vetoable handler
				     */
					if (solverChoice.getSelectedItem().equals(XcosMessages.CVODE)) {
						parameters.setSolver(0);
					} else {
						parameters.setSolver(100);
					}

					parameters.setFinalIntegrationTime(((BigDecimal) integration.getValue()).doubleValue());
					parameters.setRealTimeScaling(((BigDecimal) rts.getValue()).doubleValue());
					parameters.setIntegratorAbsoluteTolerance(((BigDecimal) integrator.getValue()).doubleValue());
					parameters.setIntegratorRelativeTolerance(((BigDecimal) integratorRel.getValue()).doubleValue());
					parameters.setToleranceOnTime(((BigDecimal) toleranceOnTime.getValue()).doubleValue());
					parameters.setMaxIntegrationTimeInterval(((BigDecimal) maxIntegrationTime.getValue()).doubleValue());
					parameters.setMaximumStepSize(((Integer) maxStepSize.getValue()).doubleValue());
				
					dispose();
				
			    } catch (PropertyVetoException ex) {
					LogFactory.getLog(SetupAction.class).error(ex);
				}
			    }
			}
		});

		setContextButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				final SetContextDialog dialog = new SetContextDialog(SetupDialog.this, parameters);
				
				dialog.pack();
				dialog.setVisible(true);
			}
		});
	}
}
//CSON: ClassDataAbstractionCoupling
//CSON: ClassFanOutComplexity
//CSON: MagicNumber
