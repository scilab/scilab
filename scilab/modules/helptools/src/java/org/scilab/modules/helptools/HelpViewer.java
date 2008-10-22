/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Hussein SHAFIE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.helptools;

import java.io.File;
import java.net.URI;
import java.net.MalformedURLException;
import java.net.URL;
import java.awt.Point;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.KeyEvent;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.SwingUtilities;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JButton;
import javax.swing.JTextField;
import javax.swing.JPanel;
import javax.swing.JOptionPane;
import javax.swing.JFrame;
import javax.help.HelpSetException;
import javax.help.BadIDException;
import javax.help.HelpSet;
import javax.help.HelpBroker;

public class HelpViewer implements ActionListener {
    private HelpBroker helpBroker;
    private JFrame frame;
    private JTextField field;

    public HelpViewer(File[] jarFiles)
        throws HelpSetException {
        helpBroker = createHelpBroker(jarFiles);

        frame = new JFrame("View Help");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel form = (JPanel) frame.getContentPane();
        initForm(form);

        frame.pack();
        frame.setVisible(true);

        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                Rectangle r = frame.getBounds(null);
                helpBroker.setLocation(new Point(r.x, r.y + r.height));
                helpBroker.setDisplayed(true);
            }
        });
    }

    private void initForm(JPanel form) {
        form.setLayout(new GridBagLayout());

        GridBagConstraints constraints = new GridBagConstraints();
        constraints.insets.top = 5;
        constraints.insets.bottom = 5;
        constraints.insets.left = 5;

        JLabel label = new JLabel("Function name:");
        constraints.gridx = 0; 
        constraints.gridy = 0; 
        form.add(label, constraints);
        
        field = new JTextField(40);
        field.addActionListener(this);
        field.setActionCommand("show");
        ++constraints.gridx;
        constraints.fill = GridBagConstraints.HORIZONTAL;
        constraints.weightx = 1;
        constraints.insets.left = 2;
        form.add(field, constraints);

        JButton button = new JButton("Show");
		//        button.setIcon(
		//            new ImageIcon(HelpViewer.class.getResource("enter.gif")));
        button.setIconTextGap(2);
        button.setHorizontalTextPosition(JButton.LEFT);
        button.setVerticalTextPosition(JButton.CENTER);
        button.setMnemonic(KeyEvent.VK_S);
        button.setDisplayedMnemonicIndex(0);
        button.addActionListener(this);
        button.setActionCommand("show");
        ++constraints.gridx;
        constraints.fill = GridBagConstraints.NONE;
        constraints.weightx = 0;
        form.add(button, constraints);

        button = new JButton("Quit");
        button.setMnemonic(KeyEvent.VK_Q);
        button.setDisplayedMnemonicIndex(0);
        button.addActionListener(this);
        button.setActionCommand("quit");
        ++constraints.gridx;
        constraints.insets.left = 20;
        constraints.insets.right = 5;
        form.add(button, constraints);
    }

    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();

        if ("show".equals(command)) {
            show();
        } else if ("quit".equals(command)) {
            quit();
        }
    }

    private boolean show() {
        String id = field.getText().trim();
		System.out.println("id : "+id);
        if (id.length() == 0) {
            field.selectAll();
            field.requestFocus();
            return false;
        }

        if (!helpBroker.isDisplayed())
			System.out.println("display : "+id);
            helpBroker.setDisplayed(true);

        try {
			System.out.println("try: : "+id);
            helpBroker.setCurrentID(id);
            return true;
        } catch (BadIDException e) {
            JOptionPane.showMessageDialog(frame, 
                                          "'" + id + "', unknown function", 
                                          "Error", JOptionPane.ERROR_MESSAGE);
            return false;
        }
    }

    private int quit() {
        return 0;
    }

    // -----------------------------------------------------------------------
    
    private static HelpBroker createHelpBroker(File[] jarFiles)
        throws HelpSetException {
        HelpBroker helpBroker = null;

        for (int i = 0; i < jarFiles.length; ++i) {
            URI jarURI = jarFiles[i].toURI();

            StringBuilder buffer = new StringBuilder("jar:");
            buffer.append(jarURI);
            buffer.append("!/");
            buffer.append(rootName(jarURI));
            buffer.append("/jhelpset.hs");

            URL helpSetURL = null;
            try {
                helpSetURL = new URL(buffer.toString());
            } catch (MalformedURLException cannotHappen) {
                cannotHappen.printStackTrace();
                continue;
            }

            HelpSet helpSet = new HelpSet(/*classLoader*/ null, helpSetURL);
            if (helpBroker == null)
                helpBroker = helpSet.createHelpBroker();
            else
                helpBroker.getHelpSet().add(helpSet);
        }

        helpBroker.setSize(new Dimension(800, 600));

        return helpBroker;
    }

    private static final String rootName(URI uri) {
        String path = uri.toString();

        String baseName;
        int pos = path.lastIndexOf('/');
        if (pos < 0)
            baseName = path;
        else
            baseName = path.substring(pos+1);

        String rootName;
        pos = baseName.lastIndexOf('.');
        if (pos < 0)
            rootName = baseName;
        else
            rootName = baseName.substring(0, pos);

        return rootName;
    }

    // -----------------------------------------------------------------------
    
    public static int main(String[] args) {
        File[] jarFiles = null;

        if (args.length > 0) {
            jarFiles = new File[args.length];

            for (int i = 0; i < args.length; ++i) {
                String arg = args[i];

                File file = new File(arg);
                if (!arg.endsWith("_help.jar") || !file.isFile()) {
                    System.err.println("*** error: '" + file + 
                                       "', not a JavaHelp .jar file");
                    jarFiles = null;
                    break;
                }

                jarFiles[i] = file;
            }
        }

        if (jarFiles == null) {
            System.err.println(
                "Usage: java org.scilab.modules.helptools.HelpViewer" +
                " XXX_help.jar ... ZZZ_help.jar\n" +
                "Allows to browse the contents of specified\n" +
                "JavaHelp[tm] .jar files.\n" +
                "Specified .jar files must have been created using sci2jh.\n" +
                "The name of a .jar file must end with '_help.jar'.");
            return 1;
        }

        HelpViewer helpViewer;
        try {
            helpViewer = new HelpViewer(jarFiles);
        } catch (Exception e) {
            System.err.println("*** error: cannot create help viewer: " +
                               Helpers.reason(e));
			e.printStackTrace();
            return 2;
        }
        return 0;
    }
}
