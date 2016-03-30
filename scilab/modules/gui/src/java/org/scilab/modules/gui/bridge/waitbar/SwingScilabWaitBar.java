/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.bridge.waitbar;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.waitbar.SimpleWaitBar;

/**
 * Swing implementation of a Scilab Wait Bars
 * @author Vincent COUVERT
 */
public class SwingScilabWaitBar extends JFrame implements SwingViewObject, SimpleWaitBar {

    private static final long serialVersionUID = -5208590743368628657L;

    private static final int WIDTH = 450;
    private static final int HEIGHT = 150;

    private Integer uid;

    private int elementId;

    private final JProgressBar progressBar;

    private final JLabel messageLabel;

    private final ImageIcon scilabIcon = new ImageIcon(FindIconHelper.findIcon("scilab"));

    /**
     * Default constructor
     */
    public SwingScilabWaitBar() {
        super();

        setIconImage(scilabIcon.getImage());

        GridBagLayout gridbag = new GridBagLayout();

        Insets insets = new Insets(8, 4, 8, 4);

        JPanel pane = new JPanel(gridbag);
        pane.setOpaque(true);
        setContentPane(pane);


        /* Scilab icon */
        GridBagConstraints iconConstraints = new GridBagConstraints();
        iconConstraints.gridx = 0; // Top Left
        iconConstraints.gridy = 0;
        iconConstraints.fill = GridBagConstraints.BOTH;
        iconConstraints.anchor = GridBagConstraints.FIRST_LINE_START;
        iconConstraints.insets = insets;
        JLabel icon = new JLabel();
        gridbag.setConstraints(icon, iconConstraints);
        icon.setIcon(scilabIcon);
        pane.add(icon);
        icon.setVisible(true);

        /* Message */
        GridBagConstraints messageConstraints = new GridBagConstraints();
        messageConstraints.gridx = 1;
        messageConstraints.gridy = 0;
        messageConstraints.fill = GridBagConstraints.BOTH;
        messageConstraints.insets = insets;
        messageLabel = new JLabel();
        gridbag.setConstraints(messageLabel, messageConstraints);
        pane.add(messageLabel);
        messageLabel.setVisible(true);


        /* ProgressBar */
        progressBar = new JProgressBar();
        GridBagConstraints progressBarConstraints = new GridBagConstraints();
        progressBarConstraints.gridx = 0;
        progressBarConstraints.gridy = 1;
        progressBarConstraints.gridwidth = 2;
        progressBarConstraints.fill = GridBagConstraints.BOTH;
        progressBarConstraints.insets = insets;
        progressBarConstraints.weightx = 1.0;
        gridbag.setConstraints(progressBar, progressBarConstraints);
        pane.add(progressBar);
        progressBar.setVisible(true);

        pane.setVisible(true);
        pane.doLayout();

        this.setSize(WIDTH, HEIGHT);
        this.setPreferredSize(new Dimension(WIDTH, HEIGHT));
        this.setMinimumSize(new Dimension(WIDTH, HEIGHT));

        if (ScilabConsole.isExistingConsole()) {
            setLocationRelativeTo((Component) ScilabConsole.getConsole().getAsSimpleConsole());
        }

        this.setVisible(true);
        this.doLayout();
        this.pack();

    }

    /**
     * Set the element id for this MessageBox
     * @param id the id of the corresponding MessageBox object
     */
    @Override
    public void setElementId(int id) {
        elementId = id;
    }

    /**
     * Get the element id for this MessageBox
     * @return id the id of the corresponding MessageBox object
     */
    @Override
    public int getElementId() {
        return elementId;
    }

    /**
     * Set the title of the WaitBar
     * @param title the title to set
     */
    @Override
    public void setTitle(String title) {
        super.setTitle(title);
    }

    /**
     * Set the message of the WaitBar
     * @param message the message to set
     */
    @Override
    public void setMessage(String[] message) {
        int line = 0;
        StringBuffer msg = new StringBuffer("<HTML>");
        for (line = 0; line < message.length - 1; line++) {
            msg.append(message[line] + "<br>");
        }
        msg.append(message[line] + "</HTML>");
        messageLabel.setText(msg.toString());
    }

    /**
     * Set the current value of the WaitBar
     * @param value the value to set
     */
    @Override
    public void setValue(int value) {
        progressBar.setValue(value);
    }

    /**
     * Close the WaitBar
     */
    @Override
    public void close() {
        this.dispose();
    }

    /**
     * Indicates if the total execution time is known
     * @param status true if the total progress time in unknown
     */
    @Override
    public void setIndeterminateMode(boolean status) {
        progressBar.setIndeterminate(status);
        progressBar.setStringPainted(!status);

    }

    /**
     * Set the UID
     * @param id the UID
     */
    public void setId(Integer id) {
        uid = id;
    }

    /**
     * Get the UID
     * @return the UID
     */
    public Integer getId() {
        return uid;
    }

    /**
     * Generic update method
     * @param property property name
     * @param value property value
     */
    public void update(int property, Object value) {
        if (property == __GO_UI_MESSAGE__) {
            setMessage((String[]) value);
        } else if (property == __GO_UI_VALUE__) {
            setValue((Integer) value);
        }
    }
}
