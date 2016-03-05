/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.gui.utils;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.net.URL;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.JToggleButton;
import javax.swing.GroupLayout;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.localization.Messages;

/**
 * Scilab about box
 *
 * @author Vincent COUVERT
 */
public class ScilabAboutBox {

    public static final String SCIDIR = System.getenv("SCI");
    public static final String IMAGEPATH = SCIDIR + "/modules/gui/images/icons/aboutscilab.png";
    private static Icon scilabIcon = new ImageIcon(FindIconHelper.findIcon("scilab", "256x256"));
    private static Image imageForIcon = ((ImageIcon) scilabIcon).getImage();

    /**
     * Constructor
     */
    protected ScilabAboutBox() {
        throw new UnsupportedOperationException(); /*
                                                    * Prevents calls from
                                                    * subclass
                                                    */
    }

    /**
     * Display the about box
     */
    public static void displayAndWait() {
        String filename = SCIDIR + "/ACKNOWLEDGEMENTS"; // Source version
        if (!new File(filename).exists()) {
            filename = SCIDIR + "/../../ACKNOWLEDGEMENTS"; // Linux binary
            // version
        }

        if (SwingUtilities.isEventDispatchThread()) {
            createAboutBox(Messages.gettext("About Scilab..."), filename);
        } else {
            final String fname = filename;
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        createAboutBox(Messages.gettext("About Scilab..."), fname);
                    }
                });
            } catch (Exception e) { }
        }
    }

    /**
     * Create the about box
     *
     * @param aboutTitle
     *            title for the Window
     * @param ackFile
     *            path to acknowledgements file
     */
    public static void createAboutBox(final String aboutTitle, final String ackFile) {
        Window win = null;
        if (ScilabConsole.isExistingConsole()) {
            win = (Window) SwingUtilities.getAncestorOfClass(Window.class, (Component) ScilabConsole.getConsole().getAsSimpleConsole());
        }

        final JDialog aboutBox = new JDialog(win);
        aboutBox.setIconImage(imageForIcon);
        aboutBox.setTitle(Messages.gettext(aboutTitle));

        //aboutBox.setAlwaysOnTop(true);
        aboutBox.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

        ScilabSwingUtilities.closeOnEscape(aboutBox);

        AboutPanel ap = new AboutPanel(aboutBox, ackFile);
        aboutBox.setContentPane(ap);
        aboutBox.setResizable(false);

        /*
         * Pack then replace
         */
        aboutBox.pack();
        if (ScilabConsole.isExistingConsole()) {
            aboutBox.setLocationRelativeTo((Component) ScilabConsole.getConsole().getAsSimpleConsole());
        } else {
            aboutBox.setLocation((int) Toolkit.getDefaultToolkit().getScreenSize().getWidth() / 2 - (aboutBox.getWidth() / 2), (int) Toolkit
                                 .getDefaultToolkit().getScreenSize().getHeight()
                                 / 2 - (aboutBox.getHeight() / 2));
        }
        aboutBox.setVisible(true);
        ap.close.requestFocus();
    }

    @SuppressWarnings("serial")
    private static final class AboutPanel extends JPanel {
        private static final String SCIDIR = System.getenv("SCI");
        private static final String IMAGEPATH = SCIDIR + "/modules/gui/images/icons/aboutscilab.png";
        private static final Font FONT = new Font(Font.SANS_SERIF, Font.PLAIN, 10);

        final JDialog parent;
        final String ackFile;

        final ImageIcon background = new ImageIcon(IMAGEPATH);

        /** Creates new form NewJPanel */
        public AboutPanel(final JDialog parent, final String ackFile) {
            this.parent = parent;
            this.ackFile = ackFile;

            initComponents();
        }

        private void initComponents() {
            topPane = new JPanel() {

                /*
                 * Override methods to paint the background image
                 */

                @Override
                public Dimension getPreferredSize() {
                    return new Dimension(background.getIconWidth(), background.getIconHeight());
                }

                @Override
                public Dimension getMaximumSize() {
                    return getPreferredSize();
                }

                @Override
                public Dimension getMinimumSize() {
                    return getPreferredSize();
                }

                @Override
                public void paintComponent(Graphics g) {
                    g.drawImage(background.getImage(), 0, 0, (int) getSize().getWidth(), (int) getSize().getHeight(), this);
                }
            };
            close = new JButton();
            acknowledgements = new JToggleButton();
            ackScrollPane = new JScrollPane();
            ackText = new JTextPane();
            ackText.setContentType("text/plain; charset=utf-8");

            setLayout(new BorderLayout());

            close.setText(Messages.gettext("Close"));
            close.setFont(FONT);
            close.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent evt) {
                    closeActionPerformed(evt);
                }
            });

            acknowledgements.setText(Messages.gettext("Acknowledgements"));
            acknowledgements.setFont(FONT);
            acknowledgements.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent evt) {
                    acknowledgementsActionPerformed(evt);
                }
            });

            GroupLayout topPaneLayout = new GroupLayout(topPane);
            topPane.setLayout(topPaneLayout);

            /*
              topPaneLayout.setHorizontalGroup(topPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(
              javax.swing.GroupLayout.Alignment.TRAILING,
              topPaneLayout.createSequentialGroup().addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addComponent(acknowledgements)
              .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED).addComponent(close).addContainerGap()));
              topPaneLayout.setVerticalGroup(topPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(
              javax.swing.GroupLayout.Alignment.TRAILING,
              topPaneLayout.createSequentialGroup().addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
              .addGroup(
              topPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE).addComponent(close)
              .addComponent(acknowledgements)).addContainerGap()));
            */

            topPaneLayout.setHorizontalGroup(topPaneLayout.createParallelGroup(GroupLayout.Alignment.TRAILING).addGroup(
                                                 topPaneLayout.createSequentialGroup()
                                                 .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                                 .addGroup(
                                                         topPaneLayout.createParallelGroup(GroupLayout.Alignment.TRAILING)
                                                         .addComponent(close)
                                                         .addComponent(acknowledgements)).addGap(5)));

            topPaneLayout.setVerticalGroup(topPaneLayout.createSequentialGroup().addGroup(
                                               topPaneLayout.createSequentialGroup()
                                               .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                               .addComponent(acknowledgements).addGap(5)
                                               .addComponent(close).addGap(5)));
            /*
              javax.swing.GroupLayout.ParallelGroup horzGroup = topPaneLayout.createParallelGroup();
              javax.swing.GroupLayout.ParallelGroup vertGroup = topPaneLayout.createParallelGroup();

              horzGroup.addGroup(topPaneLayout.createSequentialGroup()
              .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addComponent(acknowledgements)
              .addComponent(close));
              topPaneLayout.setHorizontalGroup(horzGroup);

              vertGroup.addGroup(topPaneLayout.createSequentialGroup()
              .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addComponent(acknowledgements)
              .addComponent(acknowledgements)
              .addComponent(close));
              topPaneLayout.setVerticalGroup(vertGroup);
            */
            add(topPane, BorderLayout.PAGE_START);

            ackScrollPane.setViewportView(ackText);
            ackScrollPane.setPreferredSize(topPane.getPreferredSize());

            ackScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

            try {
                URL url = new File(ackFile).toURI().toURL();
                ackText.setPage(url);
            } catch (IOException e) {
                acknowledgements.setVisible(false);
            }
            ackText.setCaretPosition(0);
        }

        private void closeActionPerformed(ActionEvent evt) {
            parent.dispose();
        }

        private void acknowledgementsActionPerformed(ActionEvent evt) {
            if (acknowledgements.isSelected()) {
                add(ackScrollPane, BorderLayout.CENTER);
            } else {
                remove(ackScrollPane);
            }
            validate();

            parent.pack();
        }

        private JScrollPane ackScrollPane;
        private JTextPane ackText;
        private JToggleButton acknowledgements;
        JButton close;
        private JPanel topPane;
    }
}
