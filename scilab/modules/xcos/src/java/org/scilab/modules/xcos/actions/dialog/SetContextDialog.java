/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.actions.dialog;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyVetoException;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.logging.Logger;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ScrollPaneConstants;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Dialog associated with the {@link SetContextAction}.
 *
 * Note that this dialog break the Data Abstraction Coupling metric because of
 * the numbers of graphical components involved in the GUI creation. For the
 * same reason (GUI class), constants are not used on this code.
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: ClassFanOutComplexity
// CSOFF: MagicNumber
@SuppressWarnings(value = { "serial" })
public class SetContextDialog extends JDialog {
    private static final Dimension PREFERRED_SIZE = new Dimension(480, 320);
    private static final String SHARED_NEW_LINE = "\n";

    private final ScicosParameters parameters;
    private final XcosDiagram rootGraph;

    private JTextArea contextArea;

    /**
     * Default constructor
     *
     * @param parent
     *            the parent component
     * @param graph
     *            THe current graph
     * @param parameters
     *            the Scicos parameters
     */
    public SetContextDialog(Component parent, XcosDiagram graph, ScicosParameters parameters) {
        this.parameters = parameters;

        ImageIcon scilabIcon = new ImageIcon(FindIconHelper.findIcon("scilab"));
        Image imageForIcon = scilabIcon.getImage();
        setLayout(new GridBagLayout());
        setIconImage(imageForIcon);
        setTitle(XcosMessages.SET_CONTEXT);
        setModal(true);
        setLocationRelativeTo(parent);
        rootGraph = graph;
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        ScilabSwingUtilities.closeOnEscape(this);

        initComponents();
    }

    /**
     * Initialize the components
     */
    private void initComponents() {
        JLabel textLabel = new JLabel(XcosMessages.SET_CONTEXT_LABEL_TEXT);
        contextArea = new JTextArea();

        /*
         * Construct a text from a String array context
         */
        VectorOfString v = parameters.getContext(new JavaController());
        final int len = v.size();
        for (int i = 0; i < len; i++) {
            contextArea.append(v.get(i) + SHARED_NEW_LINE);
        }

        JScrollPane contextAreaScroll = new JScrollPane(contextArea, ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
                ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        JButton cancelButton = new JButton(XcosMessages.CANCEL);
        JButton okButton = new JButton(XcosMessages.OK);
        okButton.setPreferredSize(cancelButton.getPreferredSize());

        JPanel buttonPane = new JPanel();
        buttonPane.setLayout(new BoxLayout(buttonPane, BoxLayout.LINE_AXIS));
        buttonPane.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
        buttonPane.add(okButton);
        buttonPane.add(Box.createRigidArea(new Dimension(10, 0)));
        buttonPane.add(cancelButton);

        /*
         * Perform layout
         */
        GridBagConstraints gbc = new GridBagConstraints();

        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weightx = 1.0;
        gbc.insets = new Insets(10, 10, 10, 10);

        add(textLabel, gbc);

        gbc.gridy = GridBagConstraints.RELATIVE;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weighty = 1.0;
        add(contextAreaScroll, gbc);

        gbc.gridheight = 1;
        gbc.gridwidth = 1;

        gbc.anchor = GridBagConstraints.LAST_LINE_END;
        gbc.fill = GridBagConstraints.NONE;
        gbc.weighty = 0;
        gbc.insets = new Insets(5, 0, 10, 10);
        add(buttonPane, gbc);

        installActionListeners(cancelButton, okButton);

        setPreferredSize(PREFERRED_SIZE);
    }

    /**
     * Install the action listener on the buttons
     *
     * @param cancelButton
     *            the cancel button
     * @param okButton
     *            the OK button
     */
    private void installActionListeners(JButton cancelButton, JButton okButton) {
        /*
         * The cancel button just exit without doing anything
         */
        cancelButton.addActionListener((ActionEvent e) -> {
            dispose();
        });

        /*
         * The ok button parse the contextArea, reconstruct the real context and
         * set the scicosParameters before exiting.
         */
        okButton.addActionListener(new ActionListenerImpl());
    }

    private class ActionListenerImpl implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            try {
                String context = contextArea.getText();
                final VectorOfString v = new VectorOfString();

                // Force a carriage return if needed so the last line is not ignored
                if (context.length() > 0 && !context.endsWith("\n")) {
                    context += '\n';
                }
                int off = 0;
                int next;
                while ((next = context.indexOf('\n', off)) != -1) {
                    v.add(context.substring(off, next));
                    off = next + 1;
                }
                parameters.setContext(new JavaController(), v);

                /*
                 * Validate the context
                 */
                try {
                    File f = File.createTempFile(ScilabDirectHandler.CONTEXT, ".sce");
                    try (FileWriter writer = new FileWriter(f)) {
                        writer.write(context, 0, context.length());
                    }

                    ScilabInterpreterManagement.putCommandInScilabQueue("var = script2var(mgetl(\"" + f.getAbsolutePath() + "\"), struct()); mdelete(\"" + f.getAbsolutePath() + "\");");
                } catch (IOException ex) {
                    Logger.getLogger(SetContextAction.class.getName()).severe(ex.toString());
                }

                dispose();
            } catch (PropertyVetoException e2) {
                Logger.getLogger(SetContextAction.class.getName()).severe(e2.toString());
            }
        }
    }
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
// CSON: MagicNumber
