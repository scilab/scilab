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

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;


public class SetupAction extends DefaultAction {

	private static boolean windowAlreadyExist ;
	private JFrame mainFrame ;
	private XcosDiagram diagram ;
	private JSpinner integrationSpinner;
	private JSpinner rtsSpinner;
	private JSpinner integratorAbsSpinner;
	private JSpinner integratorRelSpinner;
	private JSpinner toleranceOnTimeSpinner;
	private JSpinner maxIntegrationTimeSpinner;
	private JSpinner solverSpinner;
	private JSpinner maxStepSizeSpinner;

	public SetupAction(ScilabGraph scilabGraph) {
		super(XcosMessages.SETUP,scilabGraph);
	}

	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.SETUP, null, new SetupAction(scilabGraph));
	}

	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.SETUP, null, new SetupAction(scilabGraph), null);
	}

	public void actionPerformed(ActionEvent e) {
		setupBox(e);
	}


	public void setupBox (ActionEvent e) {

		mainFrame = new JFrame();
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		mainFrame.setLayout(new GridBagLayout());

		diagram = (XcosDiagram)getGraph(e);


		JLabel integrationLabel = new JLabel("Final inegration time");
		SpinnerNumberModel spinnerModel = new SpinnerNumberModel(diagram.getFinalIntegrationTime() ,null,null, 0.01);
		integrationSpinner = new JSpinner( );
		integrationSpinner.setModel(spinnerModel);
		integrationSpinner.setEditor(new JSpinner.NumberEditor(integrationSpinner,"0.00"));

		JLabel rtsLabel = new JLabel("Real time scaling");
		spinnerModel = new SpinnerNumberModel(diagram.getRealTimeScaling() ,null,null, 0.1);
		rtsSpinner = new JSpinner( );
		rtsSpinner.setModel(spinnerModel);
		rtsSpinner.setEditor(new JSpinner.NumberEditor(rtsSpinner,"0.0"));

		JLabel integratorAbsLabel = new JLabel("Integrator absolute tolerance");
		spinnerModel = new SpinnerNumberModel(diagram.getIntegratorAbsoluteTolerance(),null,null, 0.00001);
		integratorAbsSpinner = new JSpinner( );
		integratorAbsSpinner.setModel(spinnerModel);
		integratorAbsSpinner.setEditor(new JSpinner.NumberEditor(integratorAbsSpinner,"0.00000"));

		JLabel integratorRelLabel = new JLabel("Integrator relative tolerance");
		spinnerModel = new SpinnerNumberModel(diagram.getIntegratorRelativeTolerance(),null,null, 0.0000001);
		integratorRelSpinner = new JSpinner( );
		integratorRelSpinner.setModel(spinnerModel);
		integratorRelSpinner.setEditor(new JSpinner.NumberEditor(integratorRelSpinner,"0.0000000"));

		JLabel toleranceOnTimeLabel = new JLabel("Tolerance on time");
		spinnerModel = new SpinnerNumberModel(diagram.getToleranceOnTime(),null,null,1.000E-11);
		toleranceOnTimeSpinner = new JSpinner( );
		toleranceOnTimeSpinner.setModel(spinnerModel);
		toleranceOnTimeSpinner.setEditor(new JSpinner.NumberEditor(toleranceOnTimeSpinner,"0.000E00"));

		JLabel maxIntegrationTimeLabel = new JLabel("Max integration time interval");
		spinnerModel = new SpinnerNumberModel(diagram.getMaxIntegrationTimeinterval(),null,null, 1);
		maxIntegrationTimeSpinner = new JSpinner( );
		maxIntegrationTimeSpinner.setModel(spinnerModel);
		maxIntegrationTimeSpinner.setEditor(new JSpinner.NumberEditor(maxIntegrationTimeSpinner,"0"));

		JLabel solverLabel = new JLabel("Solver 0 (CVODE)/100 (IDA)");
		spinnerModel = new SpinnerNumberModel(diagram.getSolver(),0,100, 1);
		solverSpinner = new JSpinner( );
		solverSpinner.setModel(spinnerModel);
		solverSpinner.setEditor(new JSpinner.NumberEditor(solverSpinner,"0"));//0.####E0

		JLabel maxStepSizeLabel = new JLabel("maximum step size (0 means no limit)");
		spinnerModel = new SpinnerNumberModel((int)diagram.getMaximumStepSize(),0,null, 1);
		maxStepSizeSpinner = new JSpinner( );
		maxStepSizeSpinner.setModel(spinnerModel);
		maxStepSizeSpinner.setEditor(new JSpinner.NumberEditor(maxStepSizeSpinner,"0"));//0.####E0

		JButton cancelButton = new JButton(XcosMessages.CANCEL);
		JButton okButton = new JButton(XcosMessages.OK);
		JButton defaultButton = new JButton("Default");
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


		mainFrame.add( integrationLabel, gbc);

		gbc.gridy = 5;
		mainFrame.add( rtsLabel , gbc);

		gbc.gridy = 6;
		mainFrame.add( integratorAbsLabel , gbc);

		gbc.gridy = 7;
		mainFrame.add( integratorRelLabel , gbc);

		gbc.gridy = 8;
		mainFrame.add( toleranceOnTimeLabel , gbc);

		gbc.gridy = 9;
		mainFrame.add( maxIntegrationTimeLabel , gbc);

		gbc.gridy = 10;
		mainFrame.add( solverLabel , gbc);

		gbc.gridy = 11;
		mainFrame.add( maxStepSizeLabel , gbc);
		
		gbc.gridy = 12;
		mainFrame.add( setContextButton , gbc);


		gbc.gridx = 1;
		gbc.gridy = 4;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(5, 10, 0, 10);
		mainFrame.add(integrationSpinner, gbc);

		gbc.gridy = 5;
		mainFrame.add( rtsSpinner , gbc);

		gbc.gridy = 6;
		mainFrame.add( integratorAbsSpinner , gbc);

		gbc.gridy = 7;
		mainFrame.add( integratorRelSpinner , gbc);

		gbc.gridy = 8;
		mainFrame.add( toleranceOnTimeSpinner , gbc);

		gbc.gridy = 9;
		mainFrame.add( maxIntegrationTimeSpinner , gbc);

		gbc.gridy = 10;
		mainFrame.add( solverSpinner , gbc);

		gbc.gridy = 11;
		mainFrame.add( maxStepSizeSpinner , gbc);


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
				SetupAction.windowAlreadyExist= false ;
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
				solverSpinner.setValue(0.0);
				maxStepSizeSpinner.setValue(0.0);

				diagram.setFinalIntegrationTime((Double)integrationSpinner.getValue() ) ;
				diagram.setRealTimeScaling((Double)rtsSpinner.getValue())   ;
				diagram.setIntegratorAbsoluteTolerance((Double)integratorAbsSpinner.getValue() );
				diagram.setIntegratorRelativeTolerance((Double)integratorRelSpinner.getValue() ) ;
				diagram.setToleranceOnTime((Double)toleranceOnTimeSpinner.getValue())  ;
				diagram.setMaxIntegrationTimeinterval((Double)maxIntegrationTimeSpinner.getValue())  ;
				diagram.setSolver((Double)solverSpinner.getValue()) ;
				diagram.setMaximumStepSize( ((Double)maxStepSizeSpinner.getValue()).doubleValue())  ;
			}
		});

		okButton.addActionListener(new ActionListener() {




			public void actionPerformed(ActionEvent e) {



				diagram.setFinalIntegrationTime((Double)integrationSpinner.getValue() ) ;
				diagram.setRealTimeScaling((Double)rtsSpinner.getValue())   ;
				diagram.setIntegratorAbsoluteTolerance((Double)integratorAbsSpinner.getValue() );
				diagram.setIntegratorRelativeTolerance((Double)integratorRelSpinner.getValue() ) ;
				diagram.setToleranceOnTime((Double)toleranceOnTimeSpinner.getValue())  ;
				diagram.setMaxIntegrationTimeinterval((Double)maxIntegrationTimeSpinner.getValue())  ;
				diagram.setSolver((Double)solverSpinner.getValue()) ;
				diagram.setMaximumStepSize( ((Integer)maxStepSizeSpinner.getValue()).doubleValue())  ;

				SetupAction.windowAlreadyExist= false ;
				mainFrame.dispose();
			}
		});

		setContextButton.addActionListener(new ActionListener() {


			

			public void actionPerformed(ActionEvent e) {
				SetContextAction.setContextBox(diagram);
			}
		});

		//display the frame and set some properties

		mainFrame.addWindowListener( new WindowListener(){
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
				SetupAction.windowAlreadyExist = false ;
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

		} );



		mainFrame.setTitle("Set Parameters");
		mainFrame.pack();
		mainFrame.setLocationRelativeTo(null);
		mainFrame.setVisible(true);	
	}



}
