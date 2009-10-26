/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xpad.actions;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.ArrayList;
import java.util.Hashtable;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;

import org.scilab.modules.gui.bridge.colorchooser.SwingScilabColorChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class SetColorsAction extends DefaultAction {

	private static JFrame frame;
	private static boolean windowAlreadyExist;
	
	private ArrayList<JLabel> stylesNamesLabelList;
	private ArrayList<JButton> changeColorButtonList;
	private ArrayList<String> listStylesName;
	private int numberOfStyles;
	
	Hashtable<String, Color> allStylesColor;
	
    private SetColorsAction(Xpad editor) {
	super(XpadMessages.SET_COLORS, editor);
    }
    
    
    public void doAction() {
    	
    	if (!SetColorsAction.windowAlreadyExist) {
    		SetColorsAction.windowAlreadyExist = true;
        	changeColorsBox();
    	}

    }
    
    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.SET_COLORS, null, new SetColorsAction(editor), null);
    }
    
    private void changeColorsBox () {
    	
		frame = new JFrame();
		JPanel panel = new JPanel(new GridBagLayout());
		frame.setContentPane(panel);
		
		
		
		JPanel changePanel = new JPanel(new GridBagLayout());
		JPanel validationPanel = new JPanel(new GridBagLayout());
		
		
		GridBagConstraints gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;		
		gbc.insets = new Insets(10, 5, 10, 5);
		
		 listStylesName  =  ConfigXpadManager.getAllStyleName();
		 allStylesColor   = ConfigXpadManager.getAllForegroundColors();
		 numberOfStyles = listStylesName.size();
		 stylesNamesLabelList = new ArrayList<JLabel>(numberOfStyles);
		 changeColorButtonList = new ArrayList<JButton>(numberOfStyles);
		 
		 /*listener which will be addded to each "change color" buttons*/
		 ActionListener changeColorListener = new ActionListener() {

				public void actionPerformed(ActionEvent e) {
					
					boolean isSourceNotFound = true;
					int i = 0;
					
					
					/*loop which button has called the action, is there a more direct way?*/
					while (i < numberOfStyles && isSourceNotFound) {
						isSourceNotFound = (e.getSource() != changeColorButtonList.get(i));
						i++;
					}
					
					/*to avoid the extra i++*/
					i--; 
					
					/*launch a color chooser window*/
					Color previousColor = ConfigXpadManager.getAllForegroundColors().get(listStylesName.get(i));
			    	SwingScilabColorChooser _colorChooser = new SwingScilabColorChooser(previousColor);
			    	_colorChooser.displayAndWait();
			    	Color newColor = _colorChooser.getSelectedColor();
			    	
			    	if (newColor != null) {
			    		allStylesColor.put(listStylesName.get(i), newColor);
			    		stylesNamesLabelList.get(i).setForeground(newColor);
			    	}
			    	/*update label color*/
			    	

					frame.setFocusable(true);
				}
			};
	
		/*generate all the button for each style from the xml*/
		
		for (int i = 0; i < numberOfStyles; i++) {
		    
			
			Color thisStyleColor = allStylesColor.get(listStylesName.get(i));
			gbc.gridy = i;
			
			/* create label*/
			gbc.anchor = GridBagConstraints.WEST;
			gbc.gridx = 0;
			gbc.gridwidth = 3;
			
			JLabel styleNameLabel = new JLabel(listStylesName.get(i), JLabel.TRAILING);
		    styleNameLabel.setForeground(thisStyleColor);

			changePanel.add(styleNameLabel, gbc);
		    
		    /*create  button*/
		    gbc.anchor = GridBagConstraints.EAST;
		    gbc.gridx = 4;
		    gbc.gridwidth = GridBagConstraints.REMAINDER;
		    
		    JButton changeStyleColorButton  = new JButton("Change color");
		    changeStyleColorButton.addActionListener(changeColorListener);

		    changePanel.add(changeStyleColorButton, gbc);


		    /**/
		    stylesNamesLabelList.add(styleNameLabel);
		    changeColorButtonList.add(changeStyleColorButton);

		}
		
		/*ok cancel and reset to default button*/

		JButton okButton  = new JButton(XpadMessages.OK);
		JButton cancelButton  = new JButton(XpadMessages.CANCEL);
		JButton defaultButton  = new JButton(XpadMessages.DEFAULT);
		
		
		gbc.gridwidth = 1;
		
		gbc.gridx = 1;
		validationPanel.add(okButton, gbc);
		gbc.gridx = 2;
		validationPanel.add(cancelButton, gbc);
		gbc.gridx = 3;
		validationPanel.add(defaultButton, gbc);
		

		
		/*add both panel*/

		gbc.gridy = 0;
		panel.add(changePanel, gbc);
		gbc.gridy = 1;
		panel.add(validationPanel, gbc);
		
		
		/*set actions*/
		
		
		okButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				
				/*apply all the new colors to the editor*/
				int numberOfTab = getEditor().getTabPane().getComponentCount();
				for (int j = 0; j < numberOfTab; j++) {
					
					JTextPane textPane = (JTextPane) ((JScrollPane) getEditor().getTabPane().getComponentAt(j)).getViewport().getComponent(0) ;

				
					for (int i = 0; i < numberOfStyles; i++) {
						
						Color thisStyleColor = allStylesColor.get(listStylesName.get(i));		
				    	Style tempStyle = textPane.getStyledDocument().getStyle(listStylesName.get(i));
	
				    	StyleConstants.setForeground(tempStyle, thisStyleColor);				    
		
					}
					/*without this line we would have needed to type a character to see the update*/	
			    	((ScilabStyleDocument) textPane.getStyledDocument()).insertUpdate(null);
				}
		    	/*save the change in the xml*/
				ConfigXpadManager.saveAllForegroundColors(allStylesColor);
				SetColorsAction.windowAlreadyExist = false;
				frame.dispose();
			}
		});
		
		
		cancelButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				SetColorsAction.windowAlreadyExist = false;
				frame.dispose();
			}
		});
		
		
		defaultButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				allStylesColor = ConfigXpadManager.getAllDefaultForegroundColors();
				
				/*
				 * reset all style colors to their default value, will be applied only when
				 * clicking on ok button 
				 */
				for (int i = 0; i < numberOfStyles; i++) {
				    	
					Color thisStyleColor = allStylesColor.get(listStylesName.get(i));
		
					stylesNamesLabelList.get(i).setForeground(thisStyleColor);


				}
				

			}
		});
		
		//display the frame and set some properties
		
		frame.addWindowListener(new WindowListener() {
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
				SetColorsAction.windowAlreadyExist = false;
				frame.dispose();
				
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
		
		frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		frame.setTitle("Change Colors");
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);	
		
    }

	public static void closeSetColorsWindow(){
    	if (SetColorsAction.windowAlreadyExist) {
    		frame.dispose();
    		SetColorsAction.windowAlreadyExist = false;
        	
    	}
		
	}

}
