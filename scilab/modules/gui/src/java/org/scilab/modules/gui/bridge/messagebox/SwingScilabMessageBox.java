/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.messagebox;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import javax.swing.AbstractButton;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.DefaultListModel;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.ListSelectionModel;

import org.scilab.modules.gui.messagebox.SimpleMessageBox;

/**
 * Swing implementation of a Scilab MessageBox
 * @author Vincent COUVERT
 */
public class SwingScilabMessageBox extends JDialog implements SimpleMessageBox, ActionListener {

	private static final long serialVersionUID = 7939976395338222763L;
	
	private static final int WINDOW_WIDTH = 650;
	private static final int MESSAGE_HEIGHT = 200;
	private static final int LISTBOX_HEIGHT = 200;
	
	private static final int X_MDIALOG_MARGIN = 5;
	private static final int X_MDIALOG_TEXTFIELD_SIZE = 10;
	
	private static final int X_MESSAGE_TYPE = 0;
	private static final int X_DIALOG_TYPE = 1;
	private static final int X_CHOOSE_TYPE = 2;
	private static final int X_MDIALOG_TYPE = 3;
	private static final int X_CHOICES_TYPE = 4;
	
	private static final String SCIDIR = System.getenv("SCI");

	/**
	 * Offset around object and its ScrollPane
	 */
	private static final int OFFSET = 5;

	/**
	 * New line character for mutli-line text components
	 */
	private static final String NEW_LINE = "\n";
	
	/**
	 * Separator used for x_choices 
	 */
	private static final String SEPARATOR = "[--sep--]";
	
	/**
	 * Icons
	 */
	private Icon scilabIcon = new ImageIcon(SCIDIR + "/modules/gui/images/icons/scilab.png");
	private Icon passwdIcon = new ImageIcon(SCIDIR + "/modules/gui/images/icons/emblem-readonly.png");
	private Icon hourglassIcon = new ImageIcon(SCIDIR + "/modules/gui/images/icons/process-working.png");

	private int elementId;
	
	private Icon messageIcon; //= new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
	
	private int scilabDialogType = X_MESSAGE_TYPE;

	private JButton btnOK = new JButton("OK");
	private JButton btnCancel = new JButton("Cancel");

	/**
	 * Used for x_dialog
	 */
	private JTextArea textArea;
	private String initialValue;
	private int initialValueSize;
	private String userValue;
	
	/**
	 * Used for x_choose
	 */
	private JList listBox;
	private String[] listboxItems;
	private int selectedItem;
	
	/**
	 * Used for x_mdialog
	 */
	private String[] lineLabels;
	private String[] columnLabels;
	private String[] defaultInput;
	private JTextField[] textFields;
	private String[] userValues;
	
	/**
	 * Used for x_choose & x_message
	 */
	private String[] buttonsLabels;
	private int selectedButton; 
	
	/**
	 * Used for x_choices
	 */
	private int[] defaultSelectedButtons;
	private ButtonGroup[] buttonGroups;
	private int[] userSelectedButtons;
	
	/**
	 * Used for all Message Boxes
	 */
	private String message;
	private String title;
	private Image imageForIcon = ((ImageIcon) scilabIcon).getImage();
	private int messageType = -1;
	private Object[] objs;
	private Object[] buttons;
	private boolean modal = true;
	
	/**
	 * Default constructor
	 */
	public SwingScilabMessageBox() {
		super(new JFrame());
		((JFrame) getOwner()).setIconImage(imageForIcon); // Java 1.5 compatible
	}

	/**
	 * Set the element id for this MessageBox
	 * @param id the id of the corresponding MessageBox object
	 */
	public void setElementId(int id) {
		this.elementId = id;
	}
	
	/**
	 * Get the element id for MessageBox
	 * @return id the id of the corresponding MessageBox object
	 */
	public int getElementId() {
		return this.elementId;
	}
	
	/**
	 * Set the title of the MessageBox
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		this.title = title;
	}
	
	/**
	 * Set the message of the MessageBox
	 * @param message the message to set
	 */
	public void setMessage(String message) {
		this.message = message;
	}

	/**
	 * Set the message of the MessageBox (multi-line)
	 * @param mess the message to set
	 */
	public void setMessage(String[] mess) {
		int line = 0;
		message = "<HTML>";
		for (line = 0; line < mess.length - 1; line++) {
			message += mess[line] + "<br>";
		}
		message += mess[line] + "</HTML>";
	}

	/**
	 * DefaultValues
	 * Display this MessageBox and wait for user selection 
	 */
	public void displayAndWait() {
		
		// Set the title & icon
		//setIconImage(imageForIcon); // Not Java 1.5 compatible
		super.setTitle(title);

		// Create the message to display
		JLabel messageLabel = new JLabel(message);
		JScrollPane messageScrollPane = new JScrollPane(messageLabel);
		int scrollWidth = (int) Math.min(WINDOW_WIDTH, messageLabel.getPreferredSize().getWidth() + OFFSET);
		int scrollHeight = (int) Math.min(MESSAGE_HEIGHT, messageLabel.getPreferredSize().getHeight() + OFFSET);
		messageScrollPane.setPreferredSize(new Dimension(scrollWidth, scrollHeight));
		// Make the scroll Pane transparent
		messageScrollPane.setOpaque(false);
		messageScrollPane.getViewport().setOpaque(false);
		/* Make the Border of the ScrollPane invisible */
		messageScrollPane.setBorder(BorderFactory.createEmptyBorder());
		messageScrollPane.setViewportBorder(BorderFactory.createEmptyBorder());
		
		if (scilabDialogType == X_CHOICES_TYPE) {
			// Create a MessageBox for Scilab x_choices

			// All objects in the MessageBox:
			//  - Message
			//  - Editable zone
			objs = new Object[2]; 

			objs[0] = messageScrollPane;
			
			// Search the max number of objects in a line
			int curNumber = 0;
			int numberOfLines = 0;
			int numberOfColumns = 0;
			List<Integer> buttonsPerLines = new ArrayList<Integer>();
			for (int itemIndex = 0; itemIndex < lineLabels.length; itemIndex++) {
				if (!lineLabels[itemIndex].equals(SEPARATOR)) {
					curNumber++;
				} else {
					if (curNumber > numberOfColumns) {
						numberOfColumns = curNumber;
					}
					// Store informations of current line
					buttonsPerLines.add(curNumber);
					curNumber = 0;
					numberOfLines++;
				}
			}
			// Store information of last line
			// Because no separator after last line items
			buttonsPerLines.add(curNumber);
			numberOfLines++; 
			
			// Create the panel with button groups
			JPanel panel = new JPanel(new GridLayout(numberOfLines, numberOfColumns));
			buttonGroups = new ButtonGroup[numberOfLines];
			
			// Initialize return value
			userSelectedButtons = new int[numberOfLines];
			
			int curItemIndex = 0;
			int lineNumber = 0;
			int buttonNumber = 0;
			for (curItemIndex = 0; curItemIndex < lineLabels.length; curItemIndex++) {
				// Add the label of the line 
				panel.add(new JLabel(lineLabels[curItemIndex]));
				buttonNumber = 0;
				curItemIndex++;
				// Add the button group
				ButtonGroup group = new ButtonGroup();
				while (curItemIndex < lineLabels.length &&  !lineLabels[curItemIndex].equals(SEPARATOR)) {
					// Add a toggle button
					JToggleButton button = new JToggleButton(lineLabels[curItemIndex]);
					buttonNumber++;

					// Select this button if default
					if (buttonNumber == defaultSelectedButtons[lineNumber]) {
						button.setSelected(true);
					}
					// Select this button if default selection is a non existing button
					// And this button is the last of the line
					if (buttonNumber == (buttonsPerLines.get(lineNumber) - 1) 
							&& defaultSelectedButtons[lineNumber] > (buttonsPerLines.get(lineNumber) - 1)) {
						button.setSelected(true);
					}
					// Add the button to the group (for toggle)
					// And to the panel (for display)
					group.add(button);
					panel.add(button);
					
					// Increment item index
					curItemIndex++;
					
				}
				// Add empty labels if number of buttons in the line is lesser than maximum number of buttons found in a line 
				for (int emptyLabelsIndex = buttonsPerLines.get(lineNumber); emptyLabelsIndex < numberOfColumns; emptyLabelsIndex++) {
					panel.add(new JLabel());
				}
				
				// Store the group to get the user selection when returning
				buttonGroups[lineNumber] = group;
				
				// Increment current line number
				lineNumber++;
			}

			// Display the panel
			panel.doLayout();

			// Editable text zone
			JScrollPane scrollPane = new JScrollPane(panel);
		
			scrollWidth = (int) Math.min(WINDOW_WIDTH, panel.getPreferredSize().getWidth() + OFFSET);
			scrollHeight = (int) Math.min(LISTBOX_HEIGHT, panel.getPreferredSize().getHeight() + OFFSET);
			scrollPane.setPreferredSize(new Dimension(scrollWidth, scrollHeight));

			objs[1] = scrollPane;
			
			// And now the buttons
			buttons = new Object[2];
			btnOK.addActionListener(this);
			btnCancel.addActionListener(this);
			buttons[0] = btnCancel;
			buttons[1] = btnOK;
		} else if (scilabDialogType == X_MDIALOG_TYPE) {
			// Create a MessageBox for Scilab x_mdialog

			// All objects in the MessageBox:
			//  - Message
			//  - Editable zone
			objs = new Object[2]; 

			objs[0] = messageScrollPane;

			int numberOfColumns = 0;
			int numberOfLines = 0;
			if (columnLabels == null) {
				numberOfColumns = 2;
				numberOfLines = lineLabels.length;
			} else {
				numberOfColumns = columnLabels.length + 1;
				numberOfLines = lineLabels.length + 1;
			}
			//JPanel panel = new JPanel(new GridLayout(numberOfLines, numberOfColumns));
			GridBagLayout layout = new GridBagLayout();
			JPanel panel = new JPanel(layout);
			GridBagConstraints constraints = new GridBagConstraints();
			constraints.gridx = 0;
			constraints.gridy = 0;
			constraints.fill = GridBagConstraints.HORIZONTAL;
			constraints.insets = new Insets(X_MDIALOG_MARGIN, X_MDIALOG_MARGIN, X_MDIALOG_MARGIN, X_MDIALOG_MARGIN);

			int line = 0;
			int col = 0;
			// Optional first line
			if (columnLabels != null) {
				// Column label for "Row labels" column
				panel.add(new JLabel(""), constraints);
				constraints.gridx++; 
				for (col = 0; col < columnLabels.length; col++) {
					panel.add(new JLabel(columnLabels[col]), constraints);
					constraints.gridx++; 
				}
				constraints.gridy++;
			}
			
			
			// Prepare return value
			if (columnLabels == null) {
				userValues = new String[lineLabels.length];
				textFields = new JTextField[lineLabels.length];
			} else {
				userValues = new String[lineLabels.length * columnLabels.length];
				textFields = new JTextField[lineLabels.length * columnLabels.length];
			}
			for (line = 0; line < lineLabels.length; line++) {
				constraints.gridx = 0; 
				panel.add(new JLabel(lineLabels[line]), constraints);
				constraints.gridx++; 
				for (col = 0; col < numberOfColumns - 1; col++) {
					textFields[col * lineLabels.length + line] = new JTextField(defaultInput[col * lineLabels.length + line]);
					panel.add(textFields[col * lineLabels.length + line], constraints);
					textFields[col * lineLabels.length + line].setColumns(X_MDIALOG_TEXTFIELD_SIZE);
					constraints.gridx++; 
				}
				constraints.gridy++;
			}

			panel.doLayout();

			// Editable text zone
			JScrollPane scrollPane = new JScrollPane(panel);
		
			scrollWidth = (int) Math.min(WINDOW_WIDTH, panel.getPreferredSize().getWidth() + OFFSET);
			scrollHeight = (int) Math.min(LISTBOX_HEIGHT, panel.getPreferredSize().getHeight() + OFFSET);
			scrollPane.setPreferredSize(new Dimension(scrollWidth, scrollHeight));

			objs[1] = scrollPane;

			// And now the buttons
			buttons = new Object[2];
			btnOK.addActionListener(this);
			btnCancel.addActionListener(this);
			buttons[0] = btnCancel;
			buttons[1] = btnOK;

		} else if (scilabDialogType == X_CHOOSE_TYPE) {
			// Create a MessageBox for Scilab x_choose
			
			// All objects in the MessageBox:
			//  - Message
			//  - Listbox
			objs = new Object[2]; 

			// Add the message
			objs[0] = messageScrollPane;

			// Add the listBox
			objs[1] = createXchooseListBox();

			// And now the buttons
			buttons = new Object[1];
			if (buttonsLabels != null) {
				btnCancel.setText(buttonsLabels[0]);
			}
			btnCancel.addActionListener(this);
			buttons[0] = btnCancel;
		} else if (scilabDialogType == X_DIALOG_TYPE) {
			// Create a MessageBox for Scilab x_dialog

			// All objects in the MessageBox:
			//  - Message
			//  - Editable zone
			objs = new Object[2]; 

			objs[0] = messageScrollPane;

			// Editable text zone
			textArea = new JTextArea(initialValue);
			textArea.setRows(initialValueSize);
			JScrollPane scrollPane = new JScrollPane(textArea);
			scrollWidth = (int) Math.min(WINDOW_WIDTH, textArea.getPreferredSize().getWidth() + OFFSET);
			scrollHeight = (int) Math.min(LISTBOX_HEIGHT, textArea.getPreferredSize().getHeight() + OFFSET);
			scrollPane.setPreferredSize(new Dimension(scrollWidth, scrollHeight));

			objs[1] = scrollPane;

			// And now the buttons
			buttons = new Object[2];
			btnOK.addActionListener(this);
			btnCancel.addActionListener(this);
			buttons[0] = btnCancel;
			buttons[1] = btnOK;
		} else {
			// Create a MessageBox for Scilab x_message

			// All objects in the MessageBox:
			//  - Message
			objs = new Object[1]; 

			// Add the message
			objs[0] = messageScrollPane;

			// And now the buttons
			if (buttonsLabels == null) {
				buttons = new Object[1];
				btnOK.addActionListener(this);
				buttons[0] = btnOK;
				//messageType = JOptionPane.INFORMATION_MESSAGE;
			} else {
				buttons = new Object[buttonsLabels.length];
				for (int buttonNb = 0; buttonNb < buttonsLabels.length; buttonNb++) {
					JButton currentButton = new JButton(buttonsLabels[buttonNb]);
					currentButton.addActionListener(this);
					buttons[buttonsLabels.length - buttonNb - 1] = currentButton;
				}
			}
		}
		// Display
		((JScrollPane) objs[0]).setBorder(BorderFactory.createEmptyBorder());
		if (messageType != -1) {
			setContentPane(new JOptionPane(objs, messageType, JOptionPane.CANCEL_OPTION, null, buttons));
		} else {
			if (messageIcon == null) {
				messageIcon = scilabIcon;
			}
			setContentPane(new JOptionPane(objs, messageType, JOptionPane.CANCEL_OPTION, messageIcon, buttons));
		}
		pack();
		super.setModal(modal); /* Must call the JDialog class setModal */
		setVisible(true);
		doLayout();
		
		// If the dialog is not modal and Scilab waits for an answer, have to wait...
		if (!modal && scilabDialogType != X_MESSAGE_TYPE) {
			synchronized (btnOK) {
				try {
					btnOK.wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	/**
	 * Action management
	 * @param ae the action event
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */

	public void actionPerformed(ActionEvent ae) {
		if (ae.getSource() == btnOK) {
			// For a x_dialog, get the user answer
			if (scilabDialogType == X_DIALOG_TYPE) {
				userValue = textArea.getText();
			} else if (scilabDialogType == X_MDIALOG_TYPE) {
				for (int textFieldIndex = 0; textFieldIndex < textFields.length; textFieldIndex++) {
					userValues[textFieldIndex] = textFields[textFieldIndex].getText();
				}
				userValue = ""; /* To make getValueSize return a non zero value */
			} else if (scilabDialogType == X_CHOICES_TYPE) {
				
				// Get the selected button index of each button group
				for (int groupNum = 0; groupNum < buttonGroups.length; groupNum++) {
					Enumeration<AbstractButton> theButtons = buttonGroups[groupNum].getElements();
					for (int btnNum = 0; btnNum < buttonGroups[groupNum].getButtonCount(); btnNum++) {
			            JToggleButton b = (JToggleButton) theButtons.nextElement();
			            if (b.getModel() == buttonGroups[groupNum].getSelection()) {
			            	userSelectedButtons[groupNum] = btnNum + 1;
			            }
			        }
			    }
				userValue = ""; /* To make getValueSize return a non zero value */
			}
			selectedButton = 1;
		} else if (ae.getSource() == btnCancel) {
			selectedButton = 2;
		} else if (buttonsLabels != null) { // User defined buttons
			for (int index = 0; index < buttonsLabels.length; index++) {
				if (((JButton) ae.getSource()).getText().equals(buttonsLabels[index])) {
					selectedButton = index + 1;
					break;
				}
			}
		}
		// Notify btnOK for not modal Dialogs
		synchronized (btnOK) {
			btnOK.notify();
		}
		// Destroy the Dialog
		dispose();
	}
	/**
	 * Get the index of the button clicked
	 * @return the index of the button clicked
	 */
	public int getSelectedButton() {
		return selectedButton;
	}
	
	/**
	 * Set the indices of the default selected buttons (x_choices)
	 * @param indices the indices of the default selected buttons
	 */
	public void setDefaultSelectedButtons(int[] indices) {
		defaultSelectedButtons = indices;
		scilabDialogType = X_CHOICES_TYPE;
	}

	/**
	 * Get the indices of the selected buttons (x_choices)
	 * @return the indices of the selected buttons
	 */
	public int[] getUserSelectedButtons() {
		return userSelectedButtons;
	}

	/**
	 * Set the labels of the buttons in the MessageBox
	 * @param labels the labels of the buttons
	 */
	public void setButtonsLabels(String[] labels) {
		buttonsLabels = labels;
	}
	
	/**
	 * Set the initial values of the editable zone in the MessageBox
	 * @param value the initial values
	 */
	public void setInitialValue(String[] value) {
		int line = 0;
		initialValue = "";
		for (line = 0; line < value.length - 1; line++) {
			initialValue += value[line] + NEW_LINE;
		}
		initialValue += value[line];
		initialValueSize = value.length;
		
		scilabDialogType = X_DIALOG_TYPE;
	}

	/**
	 * Get the value of the editable zone in the MessageBox
	 * @return the value
	 */
	public String[] getValue() {
		if (scilabDialogType == X_MDIALOG_TYPE) {
			return userValues;
		} else { 
			return userValue.split(NEW_LINE);
		}
	}
	
	/**
	 * Get the size of the value of the editable zone in the MessageBox
	 * @return the value size
	 */
	public int getValueSize() {
		if (userValue == null) {
			return 0;
		} else {
			return userValue.split(NEW_LINE).length;
		}
	}

	/**
	 * Set the items of the listbox in the MessageBox
	 * @param items the items to set
	 */
	public void setListBoxItems(String[] items) {
		listboxItems = items;
		
		scilabDialogType = X_CHOOSE_TYPE;
	}
	
	/**
	 * Get the index of the selected item in the listbox in the MessageBox
	 * @return the index
	 */
	public int getSelectedItem() {
		return selectedItem;
	}

	/**
	 * Create the listbox to be displayed in a x_choose dialog
	 * @return the scrollpane containing the listbox
	 */
	private JScrollPane createXchooseListBox() {
		// Add the ListBox
		listBox = new JList();
		listBox.setLayoutOrientation(JList.VERTICAL);
		listBox.setModel(new DefaultListModel());
		listBox.addMouseListener(new MouseListener() {

			public void mouseClicked(MouseEvent arg0) {
				if (arg0.getClickCount() == 2) {
					selectedItem = listBox.getSelectedIndex() + 1;
					// Notify btnOK for not modal Dialogs
					synchronized (btnOK) {
						btnOK.notify();
					}
					dispose();
				}
			}

			public void mouseEntered(MouseEvent arg0) {
			}

			public void mouseExited(MouseEvent arg0) {
			}

			public void mousePressed(MouseEvent arg0) {
			}

			public void mouseReleased(MouseEvent arg0) {
			}
			
		});
		((DefaultListModel) listBox.getModel()).clear();
		for (int i = 0; i < listboxItems.length; i++) {
			((DefaultListModel) listBox.getModel()).addElement(listboxItems[i]);
		}
		listBox.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		JScrollPane scrollPane = new JScrollPane(listBox);
		int scrollWidth = (int) Math.min(WINDOW_WIDTH, listBox.getPreferredSize().getWidth() + OFFSET);
		int scrollHeight = LISTBOX_HEIGHT;
		scrollPane.setPreferredSize(new Dimension(scrollWidth, scrollHeight));
		return  scrollPane;
	}
	
	/**
	 * Set the name of the lines labels in the editable zone in the MessageBox
	 * @param labels the labels
	 */
	public void setLineLabels(String[] labels) {
		scilabDialogType = X_MDIALOG_TYPE;
		lineLabels = labels;
	}
	
	/**
	 * Set the name of the columns labels in the editable zone in the MessageBox
	 * @param labels the labels
	 */
	public void setColumnLabels(String[] labels) {
		columnLabels = labels;
	}
	
	/**
	 * Set the default values of a multi-value editable zone in the MessageBox
	 * @param values the values
	 */
	public void setDefaultInput(String[] values) {
		defaultInput = values;
	}
	
	/**
	 * Set a MessageBox modal or not
	 * @param status true to set the MessageBox modal and false else
	 */
	public void setModal(boolean status) {
		modal = status;
	}

	
	/**
	 * Set the MessageBox icon
	 * @param name the name of the icon
	 */
	public void setIcon(String name) {
		if (name.equals("error")) {
			messageType = JOptionPane.ERROR_MESSAGE;
		} else if (name.equals("hourglass")) {
			messageIcon = hourglassIcon;
		} else if (name.equals("info")) {
			messageType = JOptionPane.INFORMATION_MESSAGE;
		} else if (name.equals("passwd")) {
			messageIcon = passwdIcon;
		} else if (name.equals("question")) {
			messageType = JOptionPane.QUESTION_MESSAGE;		
		} else if (name.equals("warning")) {
			messageType = JOptionPane.WARNING_MESSAGE;			
		} else {
			messageIcon = scilabIcon;
		}
	}

}
