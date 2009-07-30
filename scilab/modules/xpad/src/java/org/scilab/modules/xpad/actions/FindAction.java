/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.actions;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultHighlighter;
import javax.swing.text.Highlighter;

import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;

public class FindAction extends DefaultAction {

	private JFrame frame;
	private JTextField textfieldFind;
	private JTextField textfieldReplace;
	private JRadioButton buttonForward;
	private JRadioButton buttonBackward;
	private JRadioButton buttonAll;
	private JRadioButton buttonSelection;
	private ButtonGroup groupDirection;
	private ButtonGroup groupScope;
	private JCheckBox caseSensitive;
	private JCheckBox wrap;
	private JCheckBox wholeWord;
	private JCheckBox regularExp;
	private JButton buttonFind;
	private JButton buttonReplaceFind;
	private JButton buttonReplace;
	private JButton buttonReplaceAll;
	private JButton buttonClose;

	private String oldword;
	private String newWord;

	private String wordToFind;


	public FindAction(Xpad editor) {
		super("Find/Replace...", editor);
		setMnemonic('F');
	}

	public void doAction() {

		findReplaceBox();
	}

	public void findReplaceBox() {

		//Find & Replace Frame
		frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		frame.setPreferredSize(new Dimension(300, 600));
		frame.setTitle("Find/Replace");
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);		
		JPanel panel = new JPanel();
		panel.setLayout(new GridBagLayout());
		frame.setContentPane(panel);

		JPanel direction = new JPanel();
		direction.setLayout(new GridBagLayout());
		JPanel scope = new JPanel();
		scope.setLayout(new GridBagLayout());
		JPanel options = new JPanel();
		options.setLayout(new GridBagLayout());

		GridBagConstraints gbc = new GridBagConstraints();
		gbc.anchor = GridBagConstraints.WEST;		
		gbc.insets = new Insets(10,5,10,5);

		//Find & Replace label, text field
		JLabel labelFind = new JLabel("Find :");
		JLabel labelReplace = new JLabel("Replace with :");
		textfieldFind = new JTextField();
		textfieldFind.setPreferredSize(new Dimension(150, 20));
		textfieldReplace = new JTextField();
		textfieldReplace.setPreferredSize(new Dimension(150, 20));

		panel.add(labelFind, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(textfieldFind, gbc);
		gbc.gridwidth = 1;

		panel.add(labelReplace, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(textfieldReplace, gbc);
		gbc.gridwidth = 1;

		panel.add(direction, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(scope, gbc);
		gbc.gridwidth = 1;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(options, gbc);

		//Find & Replace direction
		direction.setBorder(BorderFactory.createTitledBorder("Direction"));

		buttonForward = new JRadioButton("Forward");
		buttonBackward = new JRadioButton("Backward");

		groupDirection = new ButtonGroup();
		groupDirection.add(buttonForward);
		groupDirection.add(buttonBackward);
		buttonForward.setSelected(true);

		gbc.gridwidth = GridBagConstraints.REMAINDER;
		direction.add(buttonForward, gbc);
		gbc.gridwidth = 1;

		gbc.gridwidth = GridBagConstraints.REMAINDER;
		direction.add(buttonBackward, gbc);
		gbc.gridwidth = 1;

		//Find & Replace scope
		scope.setBorder(BorderFactory.createTitledBorder("Scope"));

		buttonAll = new JRadioButton("All");
		buttonSelection = new JRadioButton("Selected lines");

		groupScope = new ButtonGroup();
		groupScope.add(buttonAll);
		groupScope.add(buttonSelection);
		buttonAll.setSelected(true);

		gbc.gridwidth = GridBagConstraints.REMAINDER;
		scope.add(buttonAll, gbc);
		gbc.gridwidth = 1;

		gbc.gridwidth = GridBagConstraints.REMAINDER;
		scope.add(buttonSelection, gbc);
		gbc.gridwidth = 1;

		//Find & Replace options
		options.setBorder(BorderFactory.createTitledBorder("Options"));

		caseSensitive = new JCheckBox("Case sensitive");
		wrap = new JCheckBox("Wrap search");
		wholeWord = new JCheckBox("Whole word");
		regularExp = new JCheckBox("Regular expressions");

		gbc.anchor = GridBagConstraints.WEST;

		gbc.gridwidth = GridBagConstraints.REMAINDER;
		options.add(caseSensitive, gbc);
		options.add(wrap, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		options.add(wholeWord, gbc);
		options.add(regularExp, gbc);

		//Find & Replace buttons
		buttonFind = new JButton("Find");
		buttonReplaceFind = new JButton("Replace/Find");
		buttonReplace = new JButton("Replace");
		buttonReplaceAll = new JButton("Replace All");
		buttonClose = new JButton("Close");

		buttonFind.setPreferredSize(buttonReplaceFind.getPreferredSize());
		buttonReplace.setPreferredSize(buttonReplaceFind.getPreferredSize());
		buttonReplaceAll.setPreferredSize(buttonReplaceFind.getPreferredSize());
		buttonClose.setPreferredSize(buttonReplaceFind.getPreferredSize());

		gbc.gridwidth = 1;
		panel.add(buttonFind, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(buttonReplaceFind, gbc);
		gbc.gridwidth = 1;

		panel.add(buttonReplace, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(buttonReplaceAll, gbc);
		gbc.gridwidth = 1;

		gbc.anchor = GridBagConstraints.EAST;

		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(buttonClose, gbc);


		buttonFind.addActionListener(new ActionListener() {
			int click = 0;
			Vector<Integer> offsets;

			public void actionPerformed(ActionEvent e) {

				//Get the word we have to find
				wordToFind = textfieldFind.getText();

				//Find all matching words and return their starting position into a vector
				offsets = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).findWord(wordToFind);

				Highlighter highlight = getEditor().getTextPane().getHighlighter();
				highlight.removeAllHighlights();

				//Here we highlight all the matching words
				for (int i = 0; i < offsets.size(); i++) {
					try {
						highlight.addHighlight(offsets.elementAt(i), offsets.elementAt(i)+wordToFind.length(), new DefaultHighlighter.DefaultHighlightPainter(Color.green));
					} catch (BadLocationException e1) {
						e1.printStackTrace();
					}
				}

				//Here we highlight matching word one by one
				if (click == offsets.size()) {
					System.out.println("You have reached the end of the document");
					click = 0;
				}

				boolean changeHighlight = false;
				Highlighter.Highlight myHighlight = null;
				Highlighter.Highlight[] highlights = getEditor().getTextPane().getHighlighter().getHighlights();

				for(int i=0; i < highlights.length; i++) {
					myHighlight = highlights[i];
					//Should equal zero
					if(myHighlight.getStartOffset() == offsets.elementAt(click)) {
						changeHighlight = true;
						break;
					}
				}

				if(changeHighlight) {
					Highlighter hl = getEditor().getTextPane().getHighlighter();
					try {
						Xpad editor = getEditor();
						editor.getTextPane().setCaretPosition(offsets.elementAt(click));
						editor.getTextPane().select(offsets.elementAt(click), offsets.elementAt(click)+wordToFind.length());
						highlight.addHighlight(offsets.elementAt(click), offsets.elementAt(click)+wordToFind.length(), DefaultHighlighter.DefaultPainter);
						hl.changeHighlight(myHighlight, myHighlight.getStartOffset()+wordToFind.length(), myHighlight.getEndOffset());
					} catch (BadLocationException e1) {
						e1.printStackTrace();
					}
				}
				click++;
			}
		});

		buttonReplaceAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				oldword = textfieldFind.getText();
				newWord = textfieldReplace.getText();

				//Call replace
				
			}
		});

		buttonClose.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				frame.dispose();
			}
		});
	}

}
