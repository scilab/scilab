/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.io.File;
import java.io.IOException;
import java.net.URL;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JToggleButton;

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
    private static Icon scilabIcon = new ImageIcon(ScilabSwingUtilities.findIcon("scilab", "256x256"));
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

        createAboutBox(Messages.gettext("About Scilab..."), filename);

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
        final JFrame aboutBox = new JFrame();
        aboutBox.setIconImage(imageForIcon);
        aboutBox.setTitle(Messages.gettext(aboutTitle));

        aboutBox.setAlwaysOnTop(true);
        aboutBox.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        aboutBox.setLocationRelativeTo(null);

        ScilabSwingUtilities.closeOnEscape(aboutBox);

        aboutBox.setContentPane(new AboutPanel(aboutBox, ackFile));
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
    }

    @SuppressWarnings("serial")
    private static final class AboutPanel extends javax.swing.JPanel {
        private static final String SCIDIR = System.getenv("SCI");
        private static final String IMAGEPATH = SCIDIR + "/modules/gui/images/icons/aboutscilab.png";
        private static final Font FONT = new Font(Font.SANS_SERIF, Font.PLAIN, 10);

        final JFrame parent;
        final String ackFile;

        final ImageIcon background = new ImageIcon(IMAGEPATH);

        /** Creates new form NewJPanel */
        public AboutPanel(final JFrame parent, final String ackFile) {
            this.parent = parent;
            this.ackFile = ackFile;

            initComponents();
        }

        private void initComponents() {

            topPane = new javax.swing.JPanel() {

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
            close = new javax.swing.JButton();
            acknowledgements = new javax.swing.JToggleButton();
            ackScrollPane = new javax.swing.JScrollPane();
            ackText = new javax.swing.JTextPane();

            setLayout(new java.awt.BorderLayout());

            close.setText(Messages.gettext("Close"));
            close.setFont(FONT);
            close.addActionListener(new java.awt.event.ActionListener() {
                @Override
                public void actionPerformed(java.awt.event.ActionEvent evt) {
                    closeActionPerformed(evt);
                }
            });

            acknowledgements.setText(Messages.gettext("Acknowledgements"));
            acknowledgements.setFont(FONT);
            acknowledgements.addActionListener(new java.awt.event.ActionListener() {
                @Override
                public void actionPerformed(java.awt.event.ActionEvent evt) {
                    acknowledgementsActionPerformed(evt);
                }
            });

            javax.swing.GroupLayout topPaneLayout = new javax.swing.GroupLayout(topPane);
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
            
			topPaneLayout.setHorizontalGroup(topPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING).addGroup(
			        topPaneLayout.createSequentialGroup()
			        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
				      .addGroup(
				    		  	topPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
				    		  		.addComponent(close)
				    		  		.addComponent(acknowledgements)).addGap(5)));
			
			topPaneLayout.setVerticalGroup(topPaneLayout.createSequentialGroup().addGroup(
					topPaneLayout.createSequentialGroup()
						.addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
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
            add(topPane, java.awt.BorderLayout.PAGE_START);

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

        private void closeActionPerformed(java.awt.event.ActionEvent evt) {
            parent.dispose();
        }

        private void acknowledgementsActionPerformed(java.awt.event.ActionEvent evt) {
            if (acknowledgements.isSelected()) {
                add(ackScrollPane, java.awt.BorderLayout.CENTER);
            } else {
                remove(ackScrollPane);
            }
            validate();

            parent.pack();
        }

        private javax.swing.JScrollPane ackScrollPane;
        private javax.swing.JTextPane ackText;
        private JToggleButton acknowledgements;
        private javax.swing.JButton close;
        private javax.swing.JPanel topPane;
    }
}
