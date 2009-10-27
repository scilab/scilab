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

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextPane;

import org.scilab.modules.localization.Messages;

public class ScilabAboutBox {

	/**
	 * Constructor
	 */
	protected ScilabAboutBox() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	public static void displayAndWait() {

		final JFrame aboutBox = new JFrame();
		aboutBox.setTitle(Messages.gettext("About Scilab..."));

		aboutBox.setLayout(null);

		ImageIcon icon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/aboutscilab.jpg");
		icon.setImage(icon.getImage().getScaledInstance(icon.getIconWidth() / 2, icon.getIconHeight() / 2, 0));

		JLabel label = new JLabel(icon);
		label.setLocation(0, 0);
		label.setSize(icon.getIconWidth(), icon.getIconHeight());

		ImagePanel text = new ImagePanel();
		text.setText("\n\n\n\n\n\n" + 
				"\n\t" + Messages.gettext("Copyright 1989 - 2009") +
				"\n\t" + Messages.gettext("The Scilab Consortium / DIGITEO") + 
				"\n\t" + Messages.gettext("INRIA / ENPC / Contributors") + 
				"\n\t" + Messages.gettext("All rights reserved") + 
				"\n\t" + Messages.gettext("CeCILL License"));

		aboutBox.add(text);
		
		text.setOpaque(false);
		text.setEditable(false);
		text.setSize(icon.getIconWidth(), icon.getIconHeight());
		text.setLocation(0, 0);
		text.setVisible(true);

		JButton close = new JButton(Messages.gettext("Close"));
		close.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				aboutBox.dispose();
			}
		});
		aboutBox.add(close, 0);
		aboutBox.setAlwaysOnTop(true);

		aboutBox.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		aboutBox.setLocationRelativeTo(null);
		aboutBox.setSize(icon.getIconWidth(), icon.getIconHeight());
		aboutBox.setPreferredSize(new Dimension(icon.getIconWidth(), icon.getIconHeight()));
		close.setSize(60, 30);
		close.setLocation(aboutBox.getWidth() - 70, aboutBox.getHeight() - 40);
		aboutBox.setUndecorated(true);
		aboutBox.setVisible(true);
		aboutBox.setResizable(false);
	}

}

final class ImagePanel extends JTextPane {

	private static final long serialVersionUID = -4896563054481112056L;

	Image background = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/aboutscilab.jpg").getImage();

	public void paintComponent(Graphics g) {
		g.drawImage(background, 0, 0, (int)getSize().getWidth(), (int)getSize().getHeight(), this);
		super.paintComponent(g);

	}
}
