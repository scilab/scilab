
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.bridge.messagebox;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ListSelectionModel;

import org.scilab.modules.gui.messagebox.SimpleMessageBox;

/**
 * Swing implementation of a Scilab MessageBox
 * @author Vincent COUVERT
 */
public class SwingScilabMessageBox extends JDialog implements SimpleMessageBox, ActionListener {

	private static final long serialVersionUID = 7939976395338222763L;
	
	private static final int WINDOW_WIDTH = 400;
	private static final int MESSAGE_HEIGHT = 100;
	private static final int LISTBOX_HEIGHT = 200;
	
	/**
	 * Offset around object and its ScrollPane
	 */
	private static final int OFFSET = 5;
	
	/**
	 * Grey value for JLabel message background
	 */
	private static final int GREY_VALUE = 230;

	/**
	 * New line character for mutli-line text components
	 */
	private static final String NEW_LINE = "\n";
	
	private int elementId;
	
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
	 * Used for x_choose & x_message
	 */
	private String[] buttonsLabels;
	private int selectedButton; 
	
	/**
	 * Used for all Message Boxes
	 */
	private String message;
	private String title;
	private Image imageForIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage();
	private int messageType = JOptionPane.QUESTION_MESSAGE;
	private Object[] objs;
	private Object[] buttons;
	
	/**
	 * Default constructor
	 */
	public SwingScilabMessageBox() {
		super();
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
	 * Display this MessageBox and wait for user selection 
	 */
	public void displayAndWait() {
		
		// Set the title & icon
		setIconImage(imageForIcon);
		super.setTitle(title);

		// Create the message to display
		JLabel messageLabel = new JLabel(message);
		JScrollPane messageScrollPane = new JScrollPane(messageLabel);
		int scrollWidth = (int) Math.min(WINDOW_WIDTH, messageLabel.getPreferredSize().getWidth() + OFFSET);
		int scrollHeight = (int) Math.min(MESSAGE_HEIGHT, messageLabel.getPreferredSize().getHeight() + OFFSET);
		messageScrollPane.setPreferredSize(new Dimension(scrollWidth, scrollHeight));
		messageScrollPane.getViewport().setBackground(new Color(GREY_VALUE, GREY_VALUE, GREY_VALUE));

		if (initialValue == null && listboxItems == null) {
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
				messageType = JOptionPane.INFORMATION_MESSAGE;
			} else {
				buttons = new Object[buttonsLabels.length];
				for (int buttonNb = 0; buttonNb < buttonsLabels.length; buttonNb++) {
					JButton currentButton = new JButton(buttonsLabels[buttonNb]);
					currentButton.addActionListener(this);
					buttons[buttonNb] = currentButton;
				}
			}

		} else if (listboxItems != null) {
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

		} else {
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
		}
		// Display
		setContentPane(new JOptionPane(objs, messageType, JOptionPane.CANCEL_OPTION, null, buttons));
		pack();
		setModal(true);
		setVisible(true);
		doLayout();
	}
	
	/**
	 * Action management
	 * @param ae the action event
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent ae) {
		if (ae.getSource() == btnOK) {
			// For a x_dialog, get the user answer
			if (textArea != null) {
				userValue = textArea.getText();
			}
			selectedButton = 1;
		} else if (ae.getSource() == btnCancel) {
			selectedButton = 2;
		} else if (buttonsLabels != null) { // User defined buttons
			for (int index = 0; index < buttonsLabels.length; index++) {
				if (((JButton) ae.getSource()).getText() == buttonsLabels[index]) {
					selectedButton = index + 1;
					break;
				}
			}
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
	}

	/**
	 * Set the value of the editable zone in the MessageBox
	 * @return the value
	 */
	public String[] getValue() {
		return userValue.split(NEW_LINE);
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
}
