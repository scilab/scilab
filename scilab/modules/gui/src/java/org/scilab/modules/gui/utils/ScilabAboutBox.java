/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;

import org.scilab.modules.localization.Messages;

public class ScilabAboutBox {

	private static Icon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
	private static Image imageForIcon = ((ImageIcon) scilabIcon).getImage();
	/**
	 * Constructor
	 */
	protected ScilabAboutBox() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	public static void displayAndWait() {
		
		String[] contents = {Messages.gettext("The Scilab Consortium (DIGITEO)"),
				Messages.gettext("Copyright (c) 1989-2009 (INRIA)"),
				Messages.gettext("Copyright (c) 1989-2007 (ENPC)"),
				"",
				Messages.gettext("CeCILL License")};
		
		String filename = System.getenv("SCI") + "/ACKNOWLEDGEMENTS"; // Source version
		if (!new File(filename).exists()) {
			filename = System.getenv("SCI") + "/../../ACKNOWLEDGEMENTS"; // Linux binary version
		}

		createAboutBox(Messages.gettext("About Scilab..."), contents, filename, Messages.gettext("Acknowledgements"));

	}
	
	public static void createAboutBox(String aboutTitle, String[] contents, String ackFile, String ackTitle) {
		
		ImageIcon icon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/aboutscilab.png");
		icon.setImage(icon.getImage().getScaledInstance(icon.getIconWidth() / 2, icon.getIconHeight() / 2, 0));

		final JFrame ackBox = new JFrame();
		ackBox.setIconImage(imageForIcon);
		final JFrame aboutBox = new JFrame();
		aboutBox.setIconImage(imageForIcon);
		aboutBox.setTitle(Messages.gettext(aboutTitle));

		aboutBox.setLayout(null);
		aboutBox.setAlwaysOnTop(true);
		aboutBox.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		aboutBox.setLocationRelativeTo(null);
		aboutBox.setSize(icon.getIconWidth(), icon.getIconHeight() + 20);

		ImagePanel text = new ImagePanel();
		
		String theText = new String("\n\n\n\n\n\n");
		for (int i = 0; i < contents.length; i++) {
			theText += "\n\t" + contents[i];
		}
		
		text.setText(theText);

		text.setOpaque(false);
		text.setEditable(false);
		text.setSize(icon.getIconWidth(), icon.getIconHeight());
		text.setVisible(true);

		aboutBox.setContentPane(text);
		aboutBox.setResizable(false);
		
		JButton close = new JButton(Messages.gettext("Close"));
		close.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				aboutBox.dispose();
				ackBox.dispose();
			}
		});
		aboutBox.add(close, 0);
		close.setSize(80, 30);
		close.setLocation(text.getWidth() - 90, text.getHeight() - 40);
		
		if (ackFile != null && ackTitle != null) {
		JButton acknowledgements = new JButton(ackTitle);
		final String filename = ackFile;
		final String finalAckTitle = ackTitle;
		acknowledgements.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
								
				StringBuilder contents = new StringBuilder();
			    try {
			      BufferedReader input = new BufferedReader( new InputStreamReader(new FileInputStream(filename), "UTF8"));
			      try {
			        String line = null;
			        while (( line = input.readLine()) != null){
			          contents.append(line);
			          contents.append(System.getProperty("line.separator"));
			        }
			      }
			      finally {
			        input.close();
			      }
			    }
			    catch (IOException ex){
			      ex.printStackTrace();
			    }
			    
				ackBox.setTitle(finalAckTitle);

				ackBox.setLayout(null);
				ackBox.setAlwaysOnTop(true);
				ackBox.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
				ackBox.setLocationRelativeTo(null);
				ackBox.setSize(600, 400);
				
				JTextPane text = new JTextPane();
				text.setText(contents.toString());
				text.setCaretPosition(0);

				JScrollPane pane = new JScrollPane(text);
				pane.setSize(600, 400);
				ackBox.setContentPane(pane);
				
				text.setEditable(false);

				ackBox.setLocation((int) Toolkit.getDefaultToolkit().getScreenSize().getWidth() / 2 - (aboutBox.getWidth() / 2), 
						(int) Toolkit.getDefaultToolkit().getScreenSize().getHeight() / 2 - (aboutBox.getHeight() / 2));
				ackBox.setVisible(true);
			}
		});
		aboutBox.add(acknowledgements, 0);
		acknowledgements.setSize(120, 30);
		acknowledgements.setLocation(10, text.getHeight() - 40);
		}
		
		aboutBox.setLocation((int) Toolkit.getDefaultToolkit().getScreenSize().getWidth() / 2 - (aboutBox.getWidth() / 2), 
				(int) Toolkit.getDefaultToolkit().getScreenSize().getHeight() / 2 - (aboutBox.getHeight() / 2));
		aboutBox.setVisible(true);
		
	}

}

final class ImagePanel extends JTextPane {

	private static final long serialVersionUID = -4896563054481112056L;

	Image background = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/aboutscilab.png").getImage();

	public void paintComponent(Graphics g) {
		g.drawImage(background, 0, 0, (int)getSize().getWidth(), (int)getSize().getHeight(), this);
		super.paintComponent(g);

	}
}
