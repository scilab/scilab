/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * Copyright (C) 2010 - DIGITEO - Vincent Couvert
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.messagebox;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import javax.swing.AbstractButton;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.DefaultListModel;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.GroupLayout.ParallelGroup;
import javax.swing.GroupLayout.SequentialGroup;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.JToggleButton;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.text.html.HTMLEditorKit;
import javax.swing.text.html.StyleSheet;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.messagebox.SimpleMessageBox;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.localization.Messages;

/**
 * Swing implementation of a Scilab MessageBox
 * @author Vincent COUVERT
 */
public class SwingScilabMessageBox extends JDialog implements SimpleMessageBox, ActionListener {

    private static final long serialVersionUID = 7939976395338222763L;

    private static final int X_MESSAGE_TYPE = 0;
    private static final int X_DIALOG_TYPE = 1;
    private static final int X_CHOOSE_TYPE = 2;
    private static final int X_MDIALOG_TYPE = 3;
    private static final int X_CHOICES_TYPE = 4;

    /**
     * New line character for mutli-line text components
     */
    private static final String NEW_LINE = "\n";

    /**
     * Separator used for x_choices
     */
    private static final String SEPARATOR = "[--sep--]";

    /**
     * The minimal fixed width of any mdialog inputs
     */
    private static final int X_MDIALOG_TEXTFIELD_SIZE = 10;

    /**
     * Icons
     */
    private final Icon scilabIcon = new ImageIcon(FindIconHelper.findIcon("scilab", "48x48"));
    private final Icon passwdIcon = new ImageIcon(FindIconHelper.findIcon("emblem-readonly", "48x48"));
    private final Icon hourglassIcon = new ImageIcon(FindIconHelper.findIcon("process-working"));

    private int elementId;

    private Icon messageIcon;

    private int scilabDialogType = X_MESSAGE_TYPE;

    private Component parentWindow;

    private final JButton btnOK = new JButton(Messages.gettext("OK"));
    private final JButton btnCancel = new JButton(Messages.gettext("Cancel"));

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
    private Component[] textFields;
    private String[] userValues;

    /**
     * Used for x_choose
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
    private final Image imageForIcon = ((ImageIcon) scilabIcon).getImage();
    private int messageType = -1;
    private Component[] objs;
    private Object[] buttons;
    private boolean modal = true;
    private JCheckBox checkbox;

    /**
     * Default constructor
     */
    public SwingScilabMessageBox() {
        super();
        setIconImage(imageForIcon);
    }

    /**
     * Set the element id for this MessageBox
     * @param id the id of the corresponding MessageBox object
     */
    @Override
    public void setElementId(int id) {
        this.elementId = id;
    }

    /**
     * Get the element id for MessageBox
     * @return id the id of the corresponding MessageBox object
     */
    @Override
    public int getElementId() {
        return this.elementId;
    }

    /**
     * Set the title of the MessageBox
     * @param title the title to set
     */
    @Override
    public void setTitle(String title) {
        this.title = title;
    }

    /**
     * Set the message of the MessageBox
     * @param message the message to set
     */
    @Override
    public void setMessage(String message) {
        this.message = message;
    }

    /**
     * Set the message of the MessageBox (multi-line)
     * @param mess the message to set
     */
    @Override
    public void setMessage(String[] mess) {
        int line = 0;
        message = "<HTML>";
        for (line = 0; line < mess.length - 1; line++) {
            message += "<div>" + mess[line] + "</div>";
        }
        message += mess[line] + "</HTML>";
    }

    /**
     * Set a checkbox in the messagebox
     * @param message text of the checkbox
     * @param action the associated action
     * @param checked if true the checkbox is checked
     */
    public void setCheckbox(String message, Action action) {
        checkbox = new JCheckBox(action);
        checkbox.setText(message);
    }

    /**
     * DefaultValues
     * Display this MessageBox and wait for user selection
     */
    @Override
    public void displayAndWait() {
        JButton defaultButton = null;

        // Set the title & icon
        //setIconImage(imageForIcon); // Not Java 1.5 compatible
        super.setTitle(title);

        // Create the message to display
        JTextPane messageLabel = new JTextPane();
        messageLabel.setContentType("text/html");
        messageLabel.setOpaque(false);
        messageLabel.setBorder(null);
        messageLabel.setEditable(false);

        // Update the stylesheet so that the font matches JLabel font
        Font labelFont = UIManager.getFont("Label.font");
        HTMLEditorKit editorKit = (HTMLEditorKit) messageLabel.getEditorKit();
        StyleSheet styles = editorKit.getStyleSheet();
        String css = "body {font-family:\"" + labelFont.getName()
                     + "\"; font-size:\"" + labelFont.getSize() + "pt\"}";
        styles.addRule(css);
        editorKit.setStyleSheet(styles);
        messageLabel.setEditorKit(editorKit);

        messageLabel.setText(message);

        /* Add a link to make HTML links active */
        messageLabel.addHyperlinkListener(new HyperlinkListener() {
            @Override
            public void hyperlinkUpdate(HyperlinkEvent e) {
                if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
                    WebBrowser.openUrl(e.getURL(), e.getDescription());
                }
            }
        });

        JScrollPane messageScrollPane = new JScrollPane(messageLabel);
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
            objs = new Component[2];

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
                    // Store information of current line
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
            JPanel panel = new JPanel(new GridBagLayout());
            GridBagConstraints gbc = new GridBagConstraints();
            gbc.gridx = 0;
            gbc.gridy = 0;
            gbc.fill = GridBagConstraints.HORIZONTAL;
            buttonGroups = new ButtonGroup[numberOfLines];

            // Initialize return value
            userSelectedButtons = new int[numberOfLines];

            int curItemIndex = 0;
            int lineNumber = 0;
            int buttonNumber = 0;
            for (curItemIndex = 0; curItemIndex < lineLabels.length; curItemIndex++) {
                // Add the label of the line
                gbc.weightx = 1; // Labels will use remaining space when resizing
                panel.add(new JLabel(lineLabels[curItemIndex]), gbc);
                gbc.gridx++; // Increment the column index

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
                    gbc.weightx = 0; // Button size will not change when resizing
                    panel.add(button, gbc);
                    gbc.gridx++; // Increment the column index

                    // Increment item index
                    curItemIndex++;

                }
                // Add empty labels if number of buttons in the line is lesser than maximum number of buttons found in a line
                for (int emptyLabelsIndex = buttonsPerLines.get(lineNumber); emptyLabelsIndex < numberOfColumns; emptyLabelsIndex++) {
                    panel.add(new JLabel(), gbc);
                    gbc.gridx++; // Increment the column index
                }

                // Store the group to get the user selection when returning
                buttonGroups[lineNumber] = group;

                // Increment current line number
                lineNumber++;
                gbc.gridx = 0; // New line --> Back to first column
                gbc.gridy++; // Increment the row index
            }

            // Display the panel
            panel.doLayout();

            // Editable text zone
            JScrollPane scrollPane = new JScrollPane(panel);

            objs[1] = scrollPane;

            // And now the buttons
            buttons = new Object[2];
            btnOK.addActionListener(this);
            btnCancel.addActionListener(this);
            /* Test added for bug 4347 fix */
            if (isWindows()) {
                buttons[0] = btnOK;
                buttons[1] = btnCancel;

            } else {
                buttons[0] = btnCancel;
                buttons[1] = btnOK;
            }
            defaultButton = btnOK;
        } else if (scilabDialogType == X_MDIALOG_TYPE) {
            // Create a MessageBox for Scilab x_mdialog

            // All objects in the MessageBox:
            //  - Message
            //  - Editable zone
            objs = new Component[2];

            objs[0] = messageScrollPane;

            int numberOfColumns = 0;
            if (columnLabels == null) {
                numberOfColumns = 2;
            } else {
                numberOfColumns = columnLabels.length + 1;
            }

            // Allocate the Java Components
            final JLabel[] colLabels;
            final JLabel[] rowLabels;
            if (columnLabels != null) {
                // Optional first line
                colLabels = new JLabel[columnLabels.length + 1];

                // the first column label is on top of the row label
                colLabels[0] = new JLabel("");

                // create the components on each column
                for (int col = 0; col < columnLabels.length; col++) {
                    colLabels[col + 1] = new JLabel(columnLabels[col]);
                }
            } else {
                // there is no first line
                colLabels = null;
            }
            rowLabels = new JLabel[lineLabels.length];
            for (int row = 0; row < lineLabels.length; row++) {
                // add a gap (a space) between the column's labels and data
                rowLabels[row] = new JLabel(lineLabels[row] + " ");
            }

            // Prepare return value
            if (columnLabels == null) {
                userValues = new String[lineLabels.length];
                textFields = new Component[lineLabels.length];
            } else {
                userValues = new String[lineLabels.length * columnLabels.length];
                textFields = new Component[lineLabels.length * columnLabels.length];
            }

            // set the initial values
            for (int line = 0; line < lineLabels.length; line++) {
                for (int col = 0; col < numberOfColumns - 1; col++) {
                    final String initial = defaultInput[col * lineLabels.length + line];

                    final Component c;
                    if ("%T".equalsIgnoreCase(initial)) {
                        c = new JCheckBox();
                        ((JCheckBox) c).setSelected(true);
                    } else if ("%F".equalsIgnoreCase(initial)) {
                        c = new JCheckBox();
                        ((JCheckBox) c).setSelected(false);
                    } else {
                        c = new JTextField(initial);

                        // force an initial width when the initial text is too small
                        if (initial.length() < X_MDIALOG_TEXTFIELD_SIZE) {
                            ((JTextField) c).setColumns(X_MDIALOG_TEXTFIELD_SIZE);
                        }
                    }

                    textFields[col * lineLabels.length + line] = c;
                    rowLabels[line].setLabelFor(c);
                }
            }

            // Prepare the layout
            JPanel panel = new JPanel();
            GroupLayout layout = new GroupLayout(panel);
            panel.setLayout(layout);

            final SequentialGroup horizontalGroup = layout.createSequentialGroup().addContainerGap();
            final SequentialGroup verticalGroup = layout.createSequentialGroup().addContainerGap();

            // Horizontal layout
            {
                final ParallelGroup localGroup = layout.createParallelGroup(Alignment.LEADING);

                if (colLabels != null) {
                    localGroup.addComponent(colLabels[0]);
                }
                // special case for the row label
                for (int row = 0; row < lineLabels.length; row++) {
                    localGroup.addComponent(rowLabels[row]);
                }
                horizontalGroup.addGroup(localGroup);
                horizontalGroup.addGap(18, 18, 18);
            }
            for (int col = 0 ; col < numberOfColumns - 1; col++) {
                if (col > 0) {
                    horizontalGroup.addPreferredGap(ComponentPlacement.RELATED);
                }

                final ParallelGroup localGroup = layout.createParallelGroup(Alignment.CENTER);

                if (colLabels != null) {
                    // center the labels to be rendered like a spreadsheet
                    localGroup.addComponent(colLabels[col + 1]);
                }

                // common case for the initial values
                for (int row = 0; row < lineLabels.length; row++) {
                    localGroup.addComponent(textFields[col * lineLabels.length + row]);
                }

                horizontalGroup.addGroup(localGroup);
            }
            layout.setHorizontalGroup(
                layout.createParallelGroup(Alignment.LEADING)
                .addGroup(horizontalGroup.addContainerGap()));

            // Vertical layout
            if (colLabels != null) {
                final ParallelGroup localGroup = layout.createParallelGroup(Alignment.BASELINE);
                for (int col = 0 ; col < numberOfColumns; col++) {
                    localGroup.addComponent(colLabels[col]);
                }
                verticalGroup.addGroup(localGroup);
                verticalGroup.addGap(18, 18, 18);
            }
            for (int row = 0; row < lineLabels.length; row++) {
                if (row > 0) {
                    verticalGroup.addPreferredGap(ComponentPlacement.RELATED);
                }

                final ParallelGroup localGroup = layout.createParallelGroup(Alignment.BASELINE);
                localGroup.addComponent(rowLabels[row]);
                for (int col = 0 ; col < numberOfColumns - 1; col++) {
                    localGroup.addComponent(textFields[col * lineLabels.length + row]);
                }
                verticalGroup.addGroup(localGroup);
            }
            layout.setVerticalGroup(
                layout.createParallelGroup(Alignment.LEADING)
                .addGroup(verticalGroup.addContainerGap()));

            // Editable text zone
            JScrollPane scrollPane = new JScrollPane(panel);

            objs[1] = scrollPane;

            // And now the buttons
            buttons = new Object[2];
            btnOK.addActionListener(this);
            btnCancel.addActionListener(this);
            /* Test added for bug 4347 fix */
            if (isWindows()) {
                buttons[0] = btnOK;
                buttons[1] = btnCancel;

            } else {
                buttons[0] = btnCancel;
                buttons[1] = btnOK;
            }
            defaultButton = btnOK;
        } else if (scilabDialogType == X_CHOOSE_TYPE) {
            // Create a MessageBox for Scilab x_choose

            // All objects in the MessageBox:
            //  - Message
            //  - Listbox
            objs = new Component[2];

            // Add the message
            objs[0] = messageScrollPane;

            // Add the listBox
            objs[1] = createXchooseListBox();

            // And now the buttons
            buttons = new Object[2];
            if (buttonsLabels != null) {
                btnCancel.setText(buttonsLabels[0]);
            }
            btnOK.addActionListener(this);
            btnCancel.addActionListener(this);
            buttons[0] = btnCancel;
            buttons[1] = btnOK;
        } else if (scilabDialogType == X_DIALOG_TYPE) {
            // Create a MessageBox for Scilab x_dialog

            // All objects in the MessageBox:
            //  - Message
            //  - Editable zone
            objs = new Component[2];

            objs[0] = messageScrollPane;

            // Editable text zone
            textArea = new JTextArea(initialValue);
            textArea.setRows(initialValueSize);
            JScrollPane scrollPane = new JScrollPane(textArea);

            objs[1] = scrollPane;

            // And now the buttons
            buttons = new Object[2];
            btnOK.addActionListener(this);
            btnCancel.addActionListener(this);
            /* Test added for bug 4347 fix */
            if (isWindows()) {
                buttons[0] = btnOK;
                buttons[1] = btnCancel;

            } else {
                buttons[0] = btnCancel;
                buttons[1] = btnOK;
            }
            defaultButton = btnOK;
        } else {
            // Create a MessageBox for Scilab x_message

            // All objects in the MessageBox:
            //  - Message
            int nb = checkbox == null ? 1 : 2;
            objs = new Component[nb];

            // Add the message
            objs[0] = messageScrollPane;
            if (nb == 2) {
                objs[1] = checkbox;
            }

            // And now the buttons
            if (buttonsLabels == null) {
                buttons = new Object[1];
                btnOK.addActionListener(this);
                buttons[0] = btnOK;
                defaultButton = btnOK;
                //messageType = JOptionPane.INFORMATION_MESSAGE;
            } else {
                buttons = new Object[buttonsLabels.length];
                for (int buttonNb = 0; buttonNb < buttonsLabels.length; buttonNb++) {
                    JButton currentButton = new JButton(buttonsLabels[buttonNb]);
                    currentButton.addActionListener(this);
                    if (buttonNb == 0) {
                        defaultButton = currentButton;
                    }
                    /* Test added for bug 4347 fix */
                    if (isWindows()) {
                        buttons[buttonNb] = currentButton;
                    } else {
                        buttons[buttonsLabels.length - buttonNb - 1] = currentButton;
                    }
                }
            }
        }
        // Display
        ((JScrollPane) objs[0]).setBorder(BorderFactory.createEmptyBorder());
        final JPanel message = new JPanel(new BorderLayout());

        /*
         * If there is only one object to display then center it
         * If 2 objects then add a header and a centered component
         * If more then add a header and append all other objects to a centered container
         */
        if (objs.length > 1) {
            message.add(objs[0], BorderLayout.NORTH);
        } else {
            message.add(objs[0], BorderLayout.CENTER);
        }

        if (objs.length == 2) {
            message.add(objs[1], BorderLayout.CENTER);
        }

        if (objs.length > 2) {
            // seems that this case is never called as x_message is no more available into Scilab.
            final JPanel content = new JPanel();
            for (int i = 1; i < objs.length; i++) {
                content.add(objs[i]);
            }
            message.add(content, BorderLayout.CENTER);
        }

        if (messageType != -1) {
            setContentPane(new JOptionPane(message, messageType, JOptionPane.CANCEL_OPTION, null, buttons));
        } else {
            if (messageIcon == null) {
                messageIcon = scilabIcon;
            }
            setContentPane(new JOptionPane(message, messageType, JOptionPane.CANCEL_OPTION, messageIcon, buttons));
        }
        pack();
        super.setModal(modal); /* Must call the JDialog class setModal */

        if (parentWindow == null) {
            if (ScilabConsole.isExistingConsole()) {
                setLocationRelativeTo((Component) ScilabConsole.getConsole().getAsSimpleConsole());
            }
        } else {
            setLocationRelativeTo(parentWindow);
        }

        ScilabSwingUtilities.closeOnEscape(this);

        if (defaultButton != null) {
            getRootPane().setDefaultButton(defaultButton);
            defaultButton.requestFocusInWindow();
        }

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

    @Override
    public void actionPerformed(ActionEvent ae) {
        if (ae.getSource() == btnOK) {
            // For a x_dialog, get the user answer
            if (scilabDialogType == X_DIALOG_TYPE) {
                userValue = textArea.getText();
            } else if (scilabDialogType == X_MDIALOG_TYPE) {
                for (int textFieldIndex = 0; textFieldIndex < textFields.length; textFieldIndex++) {
                    final Component c = textFields[textFieldIndex];
                    if (c instanceof JTextField) {
                        userValues[textFieldIndex] = ((JTextField) c).getText();
                    } else if (c instanceof JCheckBox) {
                        if (((JCheckBox) c).isSelected()) {
                            userValues[textFieldIndex] = "%t";
                        } else {
                            userValues[textFieldIndex] = "%f";
                        }
                    }
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
            } else if (scilabDialogType == X_CHOOSE_TYPE) {
                selectedItem = listBox.getSelectedIndex() + 1;
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
    @Override
    public int getSelectedButton() {
        return selectedButton;
    }

    /**
     * Set the indices of the default selected buttons (x_choices)
     * @param indices the indices of the default selected buttons
     */
    @Override
    public void setDefaultSelectedButtons(int[] indices) {
        defaultSelectedButtons = indices;
        scilabDialogType = X_CHOICES_TYPE;
    }

    /**
     * Get the indices of the selected buttons (x_choices)
     * @return the indices of the selected buttons
     */
    @Override
    public int[] getUserSelectedButtons() {
        return userSelectedButtons;
    }

    /**
     * Set the labels of the buttons in the MessageBox
     * @param labels the labels of the buttons
     */
    @Override
    public void setButtonsLabels(String[] labels) {
        buttonsLabels = labels;
    }

    /**
     * Set the initial values of the editable zone in the MessageBox
     * @param value the initial values
     */
    @Override
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
    @Override
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
    @Override
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
    @Override
    public void setListBoxItems(String[] items) {
        listboxItems = items;

        scilabDialogType = X_CHOOSE_TYPE;
    }

    /**
     * Get the index of the selected item in the listbox in the MessageBox
     * @return the index
     */
    @Override
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

            @Override
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

            @Override
            public void mouseEntered(MouseEvent arg0) {
            }

            @Override
            public void mouseExited(MouseEvent arg0) {
            }

            @Override
            public void mousePressed(MouseEvent arg0) {
            }

            @Override
            public void mouseReleased(MouseEvent arg0) {
            }

        });
        ((DefaultListModel) listBox.getModel()).clear();
        for (int i = 0; i < listboxItems.length; i++) {
            ((DefaultListModel) listBox.getModel()).addElement(listboxItems[i]);
        }
        listBox.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        JScrollPane scrollPane = new JScrollPane(listBox);
        return  scrollPane;
    }

    /**
     * Set the name of the lines labels in the editable zone in the MessageBox
     * @param labels the labels
     */
    @Override
    public void setLineLabels(String[] labels) {
        scilabDialogType = X_MDIALOG_TYPE;
        lineLabels = labels;
    }

    /**
     * Set the name of the columns labels in the editable zone in the MessageBox
     * @param labels the labels
     */
    @Override
    public void setColumnLabels(String[] labels) {
        columnLabels = labels;
    }

    /**
     * Set the default values of a multi-value editable zone in the MessageBox
     * @param values the values
     */
    @Override
    public void setDefaultInput(String[] values) {
        defaultInput = values;
    }

    /**
     * Set a MessageBox modal or not
     * @param status true to set the MessageBox modal and false else
     */
    @Override
    public void setModal(boolean status) {
        modal = status;
    }


    /**
     * Set the MessageBox icon
     * @param name the name of the icon
     */
    @Override
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

    private boolean isWindows() {
        return System.getProperty("os.name").toLowerCase().contains("windows");
    }

    /**
     * Set the component used to set the location of the MessageBox (default is Scilab Console)
     * @param parent the parent
     */
    public void setParentForLocation(SimpleTab parent) {
        if (parent != null) {
            parentWindow = (Component) parent;
        } else {
            parentWindow = null;
        }
    }

    /**
     * Set the component used to set the location of the MessageBox (default is Scilab Console)
     * @param parent
     */
    public void setParentForLocation(Component parent) {
        parentWindow = parent;
    }

    /**
     * Set the component used to set the location of the MessageBox (default is Scilab Console)
     * @param parent
     */
    public void setParentForLocation(Tab parent) {
        setParentForLocation((Component) parent.getAsSimpleTab());
    }
}
