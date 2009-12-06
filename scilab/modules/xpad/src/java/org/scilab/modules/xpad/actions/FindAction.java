/* Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2009 - DIGITEO - Allan CORNET 
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
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.event.CaretListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultHighlighter;
import javax.swing.text.Highlighter;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.style.SearchManager;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * FindAction
 * @author Sylvestre KOUMAR
 * @author Allan CORNET 
 *
 */
public final class FindAction extends DefaultAction {

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -5499974793656106222L;

	private static boolean windowAlreadyExist;

	private static JFrame frame;
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
	private JLabel statusBar;
	private String oldWord;
	private String newWord;
	private String wordToFind;


	private ArrayList<Integer[]> offsets;

	private int startSelectedLines;
	private int endSelectedLines;

	private int startFindSelection;
	private int endFindSelection;

	private SearchManager searchManager = new SearchManager();

	/**
	 * Constructor
	 * @param editor Xpad
	 */
	private FindAction(Xpad editor) {
		super(XpadMessages.FIND_REPLACE +  XpadMessages.DOTS, editor);
	}

	/**
	 * doAction
	 */
	public void doAction() {
		if (!FindAction.windowAlreadyExist) {
			findReplaceBox();

			// If some text is selected, set radio button "selected lines" at true
			// else find and replace action is applied tor the entire document
			if (getEditor().getTextPane().getSelectionStart() != getEditor().getTextPane().getSelectionEnd()) {
				buttonSelection.setSelected(true);
			} else {
				buttonAll.setSelected(true);
			}

			FindAction.windowAlreadyExist = true;
		} else {
			frame.setVisible(true);
			buttonFind.requestFocus();
		}
	}

	/**
	 * createMenu
	 * @param editor Xpad
	 * @return MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.FIND_REPLACE + XpadMessages.DOTS, null, new FindAction(editor),
				KeyStroke.getKeyStroke(KeyEvent.VK_F, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * createButton
	 * @param editor Xpad
	 * @return PushButton
	 */
	public static PushButton createButton(Xpad editor) {
		return createButton(XpadMessages.FIND_REPLACE +  "...", "edit-find-replace.png", new FindAction(editor));
	}


	/**
	 * findReplaceBox
	 */
	public void findReplaceBox() {

		//Find & Replace Frame
		frame = new JFrame();
		frame.setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());
		frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		frame.setPreferredSize(new Dimension(300, 510));
		frame.setMinimumSize(new Dimension(350, 550));
		frame.setTitle(XpadMessages.FIND_REPLACE);
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
		gbc.insets = new Insets(5,2,5,2);

		//Find & Replace label, text field
		JLabel labelFind = new JLabel(XpadMessages.FIND);
		JLabel labelReplace = new JLabel(XpadMessages.REPLACE_WITH);
		textfieldFind = new JTextField();
		textfieldFind.setPreferredSize(new Dimension(150, 25));
		textfieldFind.setMinimumSize(new Dimension(100, 25));

		textfieldReplace = new JTextField();
		textfieldReplace.setPreferredSize(new Dimension(150, 25));
		textfieldReplace.setMinimumSize(new Dimension(100, 25));

		Font font = new Font("Times", Font.PLAIN, 12);

		textfieldFind.setFont(font);
		textfieldReplace.setFont(font);

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
		direction.setBorder(BorderFactory.createTitledBorder(XpadMessages.DIRECTION));

		buttonForward = new JRadioButton(XpadMessages.FORWARD);
		buttonBackward = new JRadioButton(XpadMessages.BACKWARD);

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
		scope.setBorder(BorderFactory.createTitledBorder(XpadMessages.SCOPE));

		buttonAll = new JRadioButton(XpadMessages.ALL);
		buttonSelection = new JRadioButton(XpadMessages.SELECTED_LINES);

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
		options.setBorder(BorderFactory.createTitledBorder(XpadMessages.OPTIONS));

		caseSensitive = new JCheckBox(XpadMessages.CASE_SENSITIVE);
		wrap = new JCheckBox(XpadMessages.WRAP_SEARCH, true);
		wholeWord = new JCheckBox(XpadMessages.WHOLE_WORD);
		regularExp = new JCheckBox(XpadMessages.REGULAR_EXPRESSIONS);

		gbc.anchor = GridBagConstraints.WEST;

		gbc.gridwidth = GridBagConstraints.REMAINDER;
		options.add(caseSensitive, gbc);
		options.add(wrap, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		options.add(wholeWord, gbc);
		options.add(regularExp, gbc);

		//Find & Replace buttons
		buttonFind = new JButton(XpadMessages.FIND_BUTTON);
		buttonReplaceFind = new JButton(XpadMessages.REPLACE_FIND);
		buttonReplace = new JButton(XpadMessages.REPLACE);
		buttonReplaceAll = new JButton(XpadMessages.REPLACE_ALL);
		buttonClose = new JButton(XpadMessages.CLOSE);

		buttonFind.setPreferredSize(buttonReplaceFind.getPreferredSize());
		buttonReplaceFind.setPreferredSize(buttonReplaceFind.getPreferredSize());
		buttonReplace.setPreferredSize(buttonReplaceFind.getPreferredSize());
		buttonReplaceAll.setPreferredSize(buttonReplaceFind.getPreferredSize());
		buttonClose.setPreferredSize(buttonReplaceFind.getPreferredSize());

		gbc.gridwidth = 1;
		panel.add(buttonFind, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		buttonReplaceFind.setEnabled(false);
		panel.add(buttonReplaceFind, gbc);
		gbc.gridwidth = 1;

		buttonReplace.setEnabled(false);
		panel.add(buttonReplace, gbc);
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(buttonReplaceAll, gbc);
		gbc.gridwidth = 1;

		gbc.anchor = GridBagConstraints.EAST;

		gbc.gridwidth = GridBagConstraints.REMAINDER;
		panel.add(buttonClose, gbc);

		// status bar
		statusBar = new JLabel("");
		gbc.anchor = GridBagConstraints.SOUTHWEST;
		panel.add(statusBar, gbc);

		buttonFind.setEnabled(false);
		buttonReplaceAll.setEnabled(false);

		/* behaviour of textfieldFind */
		textfieldFind.addKeyListener(new KeyListener() {
			public void keyPressed(KeyEvent e) {
				if (e.getKeyCode() == KeyEvent.VK_ENTER) {
					findText();
				}
			}

			public void keyReleased(KeyEvent e) {
			}

			public void keyTyped(KeyEvent e) {
			}

		});

		/* behaviour of textfieldReplace */
		textfieldReplace.addKeyListener(new KeyListener() {
			public void keyPressed(KeyEvent e) {
				if (e.getKeyCode() == KeyEvent.VK_ENTER) {
					findText();
				}
			}

			public void keyReleased(KeyEvent e) {
			}

			public void keyTyped(KeyEvent e) {
			}

		});

		/*behaviour of buttons*/
		buttonSelection.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				JTextPane xpadTextPane =  getEditor().getTextPane();		
				startSelectedLines = xpadTextPane.getSelectionStart();
				endSelectedLines = xpadTextPane.getSelectionEnd();					

				startSelectedLines = ((ScilabStyleDocument) xpadTextPane.getStyledDocument()).
				getParagraphElement(startSelectedLines).getStartOffset();
				endSelectedLines =   ((ScilabStyleDocument) xpadTextPane.getStyledDocument()).
				getParagraphElement(endSelectedLines).getEndOffset();

				Highlighter hl = xpadTextPane.getHighlighter();
				hl.removeAllHighlights();
				try {
					hl.addHighlight(startSelectedLines, endSelectedLines, 
							new DefaultHighlighter.DefaultHighlightPainter(new Color(205,183,158)));
				} catch (BadLocationException exc) {
					exc.printStackTrace();
				}
				getEditor().getTextPane().addFocusListener(new FocusListener() {


					public void focusGained(FocusEvent arg0) {
						if (buttonSelection.isSelected()) {

							Highlighter highlight = getEditor().getTextPane().getHighlighter();
							highlight.removeAllHighlights();

							buttonSelection.setSelected(false);
							buttonAll.setSelected(true);

							getEditor().getTextPane().removeFocusListener(this);
						}	
					}


					public void focusLost(FocusEvent arg0) {
						// TODO Auto -generated method stub

					}
				});


			}
		});



		buttonFind.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				findText();
			}
		});

		buttonReplace .addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {

				replaceOnlyText();
			}	

		});

		buttonReplaceFind.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				replaceText();
				findText();

			}
		});

		buttonReplaceAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JTextPane xpadTextPane =  getEditor().getTextPane();
				String text = null;

				boolean wholeWordSelected  = wholeWord.isSelected() &&  wholeWord.isEnabled();
				boolean regexpSelected  = regularExp.isSelected();

				// save current caret position to restore it at the end
				int currentCaretPos = xpadTextPane.getCaretPosition();

				if (buttonSelection.isSelected()) {
					ScilabStyleDocument scilabDocument = (ScilabStyleDocument) xpadTextPane.getStyledDocument();
					text = searchManager.getSelectedDocumentLines(scilabDocument, startSelectedLines, endSelectedLines);
				} else {
					text = xpadTextPane.getText();
				}

				Pattern pattern = null;

				oldWord = textfieldFind.getText();
				newWord = textfieldReplace.getText();


				if (regexpSelected) {
					oldWord = "(?m)" + oldWord;
					pattern = Pattern.compile(oldWord, Pattern.MULTILINE);


				} else {
					if (wholeWordSelected) {
						String patternWordBoundary = "\\b"; 
						oldWord = patternWordBoundary + oldWord + patternWordBoundary;
						pattern = Pattern.compile(oldWord);

					} else {

						pattern = Pattern.compile(oldWord, Pattern.LITERAL);
					}
				}

				Matcher matcher = pattern.matcher(text);
				String replacedText = matcher.replaceAll(newWord);
				if (!replacedText.equals(text)) {
					// only touch document if any replacement took place
					ScilabStyleDocument doc = (ScilabStyleDocument) xpadTextPane.getStyledDocument();
					try {
						boolean mergeMode = doc.getShouldMergeEdits();
						doc.setShouldMergeEdits(true);
						doc.replace(startSelectedLines, text.length(), replacedText, null);
						doc.setShouldMergeEdits(mergeMode);
					} catch (BadLocationException e1) {
						// TODO Auto -generated catch block
						e1.printStackTrace();
					}
				}
				xpadTextPane.setCaretPosition(currentCaretPos);
			}
		});

		buttonClose.addActionListener(new ActionListener() {


			public void actionPerformed(ActionEvent e) {
				FindAction.windowAlreadyExist = false;

				frame.dispose();
			}
		});

		textfieldReplace.addCaretListener(new CaretListener() {
			public void caretUpdate(javax.swing.event.CaretEvent e) {
				String textReplace = ((JTextField) e.getSource()).getText();
				String textFind = textfieldFind.getText();

				if ((textReplace.compareTo("") != 0) && (textFind.compareTo("") != 0)) {
					buttonReplace.setEnabled(true);
					buttonReplaceAll.setEnabled(true);
					buttonReplaceFind.setEnabled(true);
				} else {
					buttonReplace.setEnabled(false);
					buttonReplaceAll.setEnabled(false);
					buttonReplaceFind.setEnabled(false);
				}
			}
		});


		textfieldFind.addCaretListener(new CaretListener() {
			public void caretUpdate(javax.swing.event.CaretEvent e) {
				String textFind = ((JTextField) e.getSource()).getText();
				String textReplace = textfieldReplace.getText();

				if (textFind.compareTo("") != 0) {
					buttonFind.setEnabled(true);
					if (textReplace.compareTo("") != 0) {
						buttonReplace.setEnabled(true);
						buttonReplaceAll.setEnabled(true);
						buttonReplaceFind.setEnabled(true);
					} else {
						buttonReplace.setEnabled(false);
						buttonReplaceAll.setEnabled(false);
						buttonReplaceFind.setEnabled(false);
					}
				} else {
					buttonFind.setEnabled(false);
				}



				// permit to choose "whole word" only if the input is a single word

				Pattern patternWholeWord = Pattern.compile("\\w*");
				Matcher matcherWholeWord = patternWholeWord.matcher(textFind);

				wholeWord.setEnabled(false);

				if (matcherWholeWord.find()) {
					if ((matcherWholeWord.end() - matcherWholeWord.start()) == textFind.length()) {
						wholeWord.setEnabled(true);
					}

				}

				// if we search a regexp, we first need to know if the regexp is valid or not
				if (regularExp.isSelected()) {
					try {
						Pattern.compile(textFind);
						statusBar.setText("");
						buttonFind.setEnabled(true);
						buttonReplaceAll.setEnabled(true);
					} catch (PatternSyntaxException pse) {

						statusBar.setText(String.format(XpadMessages.INVALID_REGEXP, textFind));

						buttonFind.setEnabled(false);
						buttonReplaceAll.setEnabled(false);

					}


				}

				if (buttonReplace.isEnabled() && oldWord.compareTo(textFind) != 0) {
					buttonReplace.setEnabled(false);
					buttonReplaceFind.setEnabled(false);
				}
			}
		});

		frame.addWindowListener(new WindowListener() {
			public void windowClosed(WindowEvent arg0) {
				// TODO Auto -generated method stub

			}
			public void windowDeiconified(WindowEvent arg0) {
				// TODO Auto -generated method stub

			}
			public void windowActivated(WindowEvent arg0) {
				// TODO Auto -generated method stub

			}
			public void windowClosing(WindowEvent arg0) {
				FindAction.windowAlreadyExist = false;
				frame.dispose();

			}
			public void windowDeactivated(WindowEvent arg0) {
				// TODO Auto -generated method stub

			}
			public void windowIconified(WindowEvent arg0) {

			}
			public void windowOpened(WindowEvent arg0) {
				// TODO Auto -generated method stub

			}
		});

	}
	
	/**
	 * findText
	 */
	private void findText() {

		boolean wrapSearchSelected = wrap.isSelected();
		boolean forwardSearch = buttonForward.isSelected();
		boolean backwardSearch = buttonBackward.isSelected();
		boolean caseSensitiveSelected  = caseSensitive.isSelected();
		boolean wholeWordSelected  = wholeWord.isSelected() &&  wholeWord.isEnabled();
		boolean regexpSelected  = regularExp.isSelected();

		boolean onlySelectedLines = buttonSelection.isSelected();

		int saveStart 	= startFindSelection;
		int saveEnd 	= endFindSelection;



		JTextPane xpadTextPane =  getEditor().getTextPane();
		ScilabStyleDocument scilabStyle = ((ScilabStyleDocument) xpadTextPane.getStyledDocument());

		/*mainly used in case of selected text, otherwise currentPosStart =  currentPosEnd*/
		int currentCaretPos = 0;

		//Get the word we have to find
		wordToFind = textfieldFind.getText();
		oldWord = wordToFind;

		Highlighter highlight = xpadTextPane.getHighlighter();
		highlight.removeAllHighlights();

		/*case we want to search only into the selected lines*/


		currentCaretPos =  xpadTextPane.getSelectionStart();

		if (forwardSearch) {
			currentCaretPos =  xpadTextPane.getSelectionEnd();
		}
		if (backwardSearch) {
			currentCaretPos =  xpadTextPane.getSelectionStart() - 1;
		}

		if (onlySelectedLines) {
			offsets = searchManager.findWord(scilabStyle, wordToFind, startSelectedLines, endSelectedLines - 1, caseSensitiveSelected, 
					wholeWordSelected, regexpSelected);
		} else {
			offsets = searchManager.findWord(scilabStyle, wordToFind, 0, scilabStyle.getLength(), caseSensitiveSelected, 
					wholeWordSelected, regexpSelected);
		}

		statusBar.setText("");

		// if nothing has been found all this things are not needed
		if (offsets.size() > 0) {

			//find actual position of the caret in the array
			int nextIndex = -1;
			for (int i = 0; i < offsets.size(); i++) {
				if (offsets.get(i)[0] >= currentCaretPos) {
					nextIndex = i;
					break;
				}
			}

			//if backwardSearch, the next position is the previous one
			if (backwardSearch && nextIndex != -1) {
				nextIndex -= 1;
			}


			if (nextIndex == -1) {
				if (wrapSearchSelected) {
					// return to the end or the beginning of the document
					if (backwardSearch) {
						statusBar.setText(XpadMessages.PASSED_BEGIN_OF_DOCUMENT);
						nextIndex = offsets.size() - 1;
					} else {
						statusBar.setText(XpadMessages.PASSED_END_OF_DOCUMENT);
						nextIndex = 0;
					}
				} else {
					if (backwardSearch) {
						statusBar.setText(XpadMessages.BEGIN_OF_DOCUMENT);
					} else {
						statusBar.setText(XpadMessages.END_OF_DOCUMENT);
					}
				}
			}

			//tips to keep last find if we have reach the end/begin of file
			if (nextIndex == -1) {
				if (backwardSearch) {
					if (saveStart >= offsets.get(0)[0]) {
						nextIndex = 0;
					}
				} else if (saveStart <= offsets.get(offsets.size() - 1)[0]) {
					nextIndex = offsets.size() - 1;
				}
			}

			//Here we highlight all the matching words
			for (int i = 0; i < offsets.size(); i++) {
				try {
					if (i != nextIndex) {
						highlight.addHighlight(offsets.get(i)[0], offsets.get(i)[1], 
								new DefaultHighlighter.DefaultHighlightPainter(Color.green));
					}
					//TODO add a mechanism to change the foreground color too, if not if the text matched is in green too ...
				} catch (BadLocationException e1) {
					e1.printStackTrace();
				}
			}

			if (onlySelectedLines) {
				try {
					highlight.addHighlight(startSelectedLines, endSelectedLines, 
							new DefaultHighlighter.DefaultHighlightPainter(new Color(205, 183, 158)));
				} catch (BadLocationException exc) {
					exc.printStackTrace();
				}
			}

			int nextStart  = 0;
			int nextEnd  = 0;
			if (nextIndex != -1) {
				nextStart = offsets.get(nextIndex)[0];
				nextEnd = offsets.get(nextIndex)[1];
			} else {
				nextStart = saveStart;
				nextEnd = saveEnd;
			}

			//select current index
			//xpadTextPane.setCaretPosition(offsets.get(nextIndex)[0]);
			xpadTextPane.select(nextStart, nextEnd);

			// used by replace and replace/find
			startFindSelection = nextStart;
			endFindSelection = nextEnd;

			buttonReplace.setEnabled(true);
			buttonReplaceFind.setEnabled(true);

			//Here we highlight differently the match next after the caret position
			/*if we typed on the textPanel all highlights will disappear*/

			if (getEditor().getTextPane().getKeyListeners().length == 0) {

				getEditor().getTextPane().addKeyListener(new KeyListener() {

					public void keyReleased(KeyEvent e) { }
					public void keyTyped(KeyEvent e) { }

					public void keyPressed(KeyEvent e) { 

						getEditor().getTextPane().getHighlighter().removeAllHighlights();
						getEditor().getTextPane().removeKeyListener(this);
					}

				});
			}
		} else { // nothing has been found
			statusBar.setText(String.format(XpadMessages.STRING_NOT_FOUND, wordToFind));

			startFindSelection = -1;
			endFindSelection = -1;
			buttonReplace.setEnabled(false);
			buttonReplaceFind.setEnabled(false);

		}
	}

	/**
	 * replaceOnlyText
	 */
	private void replaceOnlyText() {

		boolean regexpSelected  = regularExp.isSelected();

		oldWord = textfieldFind.getText();
		newWord = textfieldReplace.getText();
		JTextPane xpadTextPane =  getEditor().getTextPane();
		int currentPosStart = startFindSelection;
		int currentPosEnd = endFindSelection;


		/*
		 * we replace only the current result and then disable replace and replace find button
		 * same behaviour as find and replace in eclipse
		 */

		if (regexpSelected) {
			Pattern patternOldWord = Pattern.compile(oldWord, Pattern.MULTILINE);
			Matcher matcher;
			try {
				matcher = patternOldWord.matcher(xpadTextPane.getText(currentPosStart, currentPosEnd - currentPosStart));
				newWord = matcher.replaceAll(newWord);
			} catch (BadLocationException ex) {
				ex.printStackTrace();
			}
		}


		try {
			((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).
				replace(currentPosStart, currentPosEnd - currentPosStart, newWord, null);

		} catch (BadLocationException ex) {
			ex.printStackTrace();
		}
		getEditor().getTextPane().getHighlighter().removeAllHighlights();
		offsets.clear();
		buttonReplace.setEnabled(false);
		buttonReplaceFind.setEnabled(false);
	}

	/**
	 * replaceText
	 */

	private void replaceText() {

		boolean regexpSelected  = regularExp.isSelected();

		oldWord = textfieldFind.getText();
		newWord = textfieldReplace.getText();
		JTextPane xpadTextPane =  getEditor().getTextPane();
		int currentPosStart = startFindSelection;
		int currentPosEnd = endFindSelection;

		/*
		 * we replace only the current result and then disable replace and replace find button
		 * same behaviour as find and replace in eclipse
		 */

		if (regexpSelected) {
			Pattern patternOldWord = Pattern.compile(oldWord, Pattern.MULTILINE);
			Matcher matcher;
			try {
				matcher = patternOldWord.matcher(xpadTextPane.getText(currentPosStart, currentPosEnd - currentPosStart));
				newWord = matcher.replaceAll(newWord);
			} catch (BadLocationException ex) {
				ex.printStackTrace();
			}
		}


		try {
			ScilabStyleDocument doc = (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument();
			doc.replace(currentPosStart, currentPosEnd - currentPosStart, newWord, null);

		} catch (BadLocationException ex) {
			ex.printStackTrace();
		}
	}

	/**
	 * closeFindReplaceWindow
	 */
	public static void closeFindReplaceWindow() {
		if (FindAction.windowAlreadyExist) {
			frame.dispose();
			FindAction.windowAlreadyExist = false;

		}
	}
}
