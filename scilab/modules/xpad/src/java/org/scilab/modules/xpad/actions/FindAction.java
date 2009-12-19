/* Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2009 - DIGITEO - Allan CORNET 
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS 
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
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.LayoutStyle;
import javax.swing.SwingConstants;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultHighlighter;
import javax.swing.text.Highlighter;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.style.SearchManager;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;


/**
 * FindAction
 * @author Sylvestre KOUMAR
 * @author Allan CORNET 
 * @author Antoine ELIAS
 *
 */
public final class FindAction extends DefaultAction {

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -5499974793656106222L;

	private static boolean windowAlreadyExist;

	private static JFrame frame;

	private JButton buttonClose;
	private JButton buttonFind;
	private ButtonGroup buttonGroup1;
	private ButtonGroup buttonGroup2;
	private JButton buttonReplace;
	private JButton buttonReplaceAll;
	private JButton buttonReplaceFind;
	private JCheckBox checkCase;
	private JCheckBox checkRegular;
	private JCheckBox checkWarp;
	private JCheckBox checkWhole;
	private JComboBox comboFind;
	private JComboBox comboReplace;
	private JLabel labelFind;
	private JLabel labelReplace;
	private JLabel labelStatus;
	private JPanel panelButton;
	private JPanel panelDirection;
	private JPanel panelFind;
	private JPanel panelFrame;
	private JPanel panelOption;
	private JPanel panelOptions;
	private JPanel panelScope;
	private JRadioButton radioAll;
	private JRadioButton radioBackward;
	private JRadioButton radioForward;
	private JRadioButton radioSelection;

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
		} else {
			frame.setVisible(true);
			buttonFind.requestFocus();
		}

		try {
			// If some text is selected, it is used in find.
			//if more than one line is selected set radio button "selected lines" at true
			// else find and replace action is applied to the entire document
			int startPos = getEditor().getTextPane().getSelectionStart();
			int endPos = getEditor().getTextPane().getSelectionEnd();
			int startLine = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(startPos);
			int endLine = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(endPos);

			if(startPos != endPos) {
				if(startLine != endLine) {
					radioSelection.setSelected(true);
					comboFind.setSelectedIndex(-1);
					comboReplace.setSelectedIndex(-1);
				} else {
					radioAll.setSelected(true);
					comboFind.getEditor().setItem(getEditor().getTextPane().getDocument().getText(startPos, endPos - startPos));
					comboFind.getEditor().selectAll();
				}
			} else {
				radioAll.setSelected(true);
				comboFind.setSelectedIndex(-1);
				comboReplace.setSelectedIndex(-1);
			}
			FindAction.windowAlreadyExist = true;
			updateFindReplaceButtonStatus();
		} catch (BadLocationException e) {
			e.printStackTrace();
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
		frame.setMinimumSize(new Dimension(475, 450));
		frame.setTitle(XpadMessages.FIND_REPLACE);
		frame.setResizable(false);
		frame.pack();
		frame.setLocationRelativeTo(Xpad.getEditor());
		frame.setVisible(true);

		buttonGroup1 = new ButtonGroup();
		buttonGroup2 = new ButtonGroup();
		panelFrame = new JPanel();
		panelOption = new JPanel();
		panelDirection = new JPanel();
		radioForward = new JRadioButton();
		radioBackward = new JRadioButton();
		panelScope = new JPanel();
		radioAll = new JRadioButton();
		radioSelection = new JRadioButton();
		panelOptions = new JPanel();
		checkCase = new JCheckBox();
		checkWhole = new JCheckBox();
		checkRegular = new JCheckBox();
		checkWarp = new JCheckBox();
		panelFind = new JPanel();
		labelFind = new JLabel();
		labelReplace = new JLabel();
		comboFind = new JComboBox();
		comboReplace = new JComboBox();
		panelButton = new JPanel();
		buttonFind = new JButton();
		buttonReplaceFind = new JButton();
		buttonReplace = new JButton();
		buttonReplaceAll = new JButton();
		buttonClose = new JButton();
		labelStatus = new JLabel();

		panelFrame.setLayout(new java.awt.BorderLayout());

		panelOption.setMaximumSize(new java.awt.Dimension(457, 225));
		panelOption.setMinimumSize(new java.awt.Dimension(457, 225));
		panelOption.setPreferredSize(new java.awt.Dimension(457, 225));

		panelDirection.setBorder(BorderFactory.createTitledBorder(XpadMessages.DIRECTION));

		buttonGroup1.add(radioForward);
		radioForward.setText(XpadMessages.FORWARD);

		buttonGroup1.add(radioBackward);
		radioBackward.setText(XpadMessages.BACKWARD);

		GroupLayout panelDirectionLayout = new GroupLayout(panelDirection);
		panelDirection.setLayout(panelDirectionLayout);
		panelDirectionLayout.setHorizontalGroup(
				panelDirectionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelDirectionLayout.createSequentialGroup()
						.addContainerGap()
						.addGroup(panelDirectionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
								.addComponent(radioForward)
								.addComponent(radioBackward))
								.addContainerGap(83, Short.MAX_VALUE))
		);
		panelDirectionLayout.setVerticalGroup(
				panelDirectionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelDirectionLayout.createSequentialGroup()
						.addComponent(radioForward)
						.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
						.addComponent(radioBackward)
						.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);

		panelScope.setBorder(BorderFactory.createTitledBorder(XpadMessages.SCOPE));
		panelScope.setMaximumSize(new java.awt.Dimension(457, 225));

		buttonGroup2.add(radioAll);
		radioAll.setText(XpadMessages.SELECT_ALL);

		buttonGroup2.add(radioSelection);
		radioSelection.setText(XpadMessages.SELECTED_LINES);

		GroupLayout panelScopeLayout = new GroupLayout(panelScope);
		panelScope.setLayout(panelScopeLayout);
		panelScopeLayout.setHorizontalGroup(
				panelScopeLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelScopeLayout.createSequentialGroup()
						.addContainerGap()
						.addGroup(panelScopeLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
								.addComponent(radioAll)
								.addComponent(radioSelection))
								.addContainerGap(69, Short.MAX_VALUE))
		);
		panelScopeLayout.setVerticalGroup(
				panelScopeLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelScopeLayout.createSequentialGroup()
						.addComponent(radioAll)
						.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
						.addComponent(radioSelection)
						.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);

		panelOptions.setBorder(BorderFactory.createTitledBorder(XpadMessages.OPTIONS));

		checkCase.setText(XpadMessages.CASE_SENSITIVE);
		checkWhole.setText(XpadMessages.WHOLE_WORD);
		checkRegular.setText(XpadMessages.REGULAR_EXPRESSIONS);
		checkWarp.setText(XpadMessages.WORD_WRAP);

		checkWarp.setSelected(true);
		GroupLayout panelOptionsLayout = new GroupLayout(panelOptions);
		panelOptions.setLayout(panelOptionsLayout);
		panelOptionsLayout.setHorizontalGroup(
				panelOptionsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelOptionsLayout.createSequentialGroup()
						.addContainerGap()
						.addGroup(panelOptionsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
								.addComponent(checkCase)
								.addComponent(checkWhole))
								.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED, 54, Short.MAX_VALUE)
								.addGroup(panelOptionsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
										.addComponent(checkWarp)
										.addComponent(checkRegular))
										.addContainerGap())
		);
		panelOptionsLayout.setVerticalGroup(
				panelOptionsLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelOptionsLayout.createSequentialGroup()
						.addGroup(panelOptionsLayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
								.addComponent(checkCase)
								.addComponent(checkRegular))
								.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
								.addGroup(panelOptionsLayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
										.addComponent(checkWhole)
										.addComponent(checkWarp))
										.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);

		GroupLayout panelOptionLayout = new GroupLayout(panelOption);
		panelOption.setLayout(panelOptionLayout);
		panelOptionLayout.setHorizontalGroup(
				panelOptionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(GroupLayout.Alignment.TRAILING, panelOptionLayout.createSequentialGroup()
						.addContainerGap()
						.addGroup(panelOptionLayout.createParallelGroup(GroupLayout.Alignment.TRAILING)
								.addComponent(panelOptions, GroupLayout.Alignment.LEADING, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
								.addGroup(panelOptionLayout.createSequentialGroup()
										.addComponent(panelDirection, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
										.addGap(18, 18, 18)
										.addComponent(panelScope, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
										.addContainerGap())
		);
		panelOptionLayout.setVerticalGroup(
				panelOptionLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelOptionLayout.createSequentialGroup()
						.addContainerGap()
						.addGroup(panelOptionLayout.createParallelGroup(GroupLayout.Alignment.LEADING, false)
								.addComponent(panelDirection, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
								.addComponent(panelScope, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
								.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
								.addComponent(panelOptions, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
								.addContainerGap(24, Short.MAX_VALUE))
		);

		panelFrame.add(panelOption, java.awt.BorderLayout.CENTER);

		panelFind.setPreferredSize(new java.awt.Dimension(457, 80));

		labelFind.setText(XpadMessages.FIND);
		labelReplace.setText(XpadMessages.REPLACE);

		comboFind.setEditable(true);
		comboReplace.setEditable(true);
		
		GroupLayout panelFindLayout = new GroupLayout(panelFind);
		panelFind.setLayout(panelFindLayout);
		panelFindLayout.setHorizontalGroup(
				panelFindLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelFindLayout.createSequentialGroup()
						.addContainerGap()
						.addGroup(panelFindLayout.createParallelGroup(GroupLayout.Alignment.LEADING, false)
								.addComponent(labelReplace, GroupLayout.Alignment.TRAILING, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
								.addComponent(labelFind, GroupLayout.Alignment.TRAILING, GroupLayout.DEFAULT_SIZE, 148, Short.MAX_VALUE))
								.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
								.addGroup(panelFindLayout.createParallelGroup(GroupLayout.Alignment.TRAILING)
										.addComponent(comboFind, 0, 284, Short.MAX_VALUE)
										.addComponent(comboReplace, 0, 284, Short.MAX_VALUE))
										.addContainerGap())
		);
		panelFindLayout.setVerticalGroup(
				panelFindLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelFindLayout.createSequentialGroup()
						.addContainerGap()
						.addGroup(panelFindLayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
								.addComponent(labelFind)
								.addComponent(comboFind, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
								.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
								.addGroup(panelFindLayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
										.addComponent(comboReplace, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
										.addComponent(labelReplace))
										.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);

		panelFrame.add(panelFind, java.awt.BorderLayout.PAGE_START);

		panelButton.setPreferredSize(new java.awt.Dimension(457, 140));

		buttonFind.setText(XpadMessages.FIND_BUTTON);
		buttonReplaceFind.setText(XpadMessages.FIND_REPLACE);
		buttonReplace.setText(XpadMessages.REPLACE);
		buttonReplaceAll.setText(XpadMessages.REPLACE_ALL);
		buttonClose.setText(XpadMessages.CLOSE);
		labelStatus.setText("");

		GroupLayout panelButtonLayout = new GroupLayout(panelButton);
		panelButton.setLayout(panelButtonLayout);
		panelButtonLayout.setHorizontalGroup(
				panelButtonLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelButtonLayout.createSequentialGroup()
						.addContainerGap()
						.addGroup(panelButtonLayout.createParallelGroup(GroupLayout.Alignment.TRAILING)
								.addGroup(panelButtonLayout.createSequentialGroup()
										.addComponent(buttonReplace)
										.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
										.addComponent(buttonReplaceAll))
										.addGroup(panelButtonLayout.createSequentialGroup()
												.addComponent(labelStatus, GroupLayout.DEFAULT_SIZE, 244, Short.MAX_VALUE)
												.addGap(18, 18, 18)
												.addComponent(buttonClose))
												.addGroup(panelButtonLayout.createSequentialGroup()
														.addComponent(buttonFind)
														.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
														.addComponent(buttonReplaceFind, GroupLayout.PREFERRED_SIZE, 182, GroupLayout.PREFERRED_SIZE)))
														.addContainerGap())
		);

		panelButtonLayout.linkSize(SwingConstants.HORIZONTAL, new java.awt.Component[] {buttonClose, buttonFind, buttonReplace, buttonReplaceAll, buttonReplaceFind});

		panelButtonLayout.setVerticalGroup(
				panelButtonLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addGroup(panelButtonLayout.createSequentialGroup()
						.addGroup(panelButtonLayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
								.addComponent(buttonReplaceFind)
								.addComponent(buttonFind))
								.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
								.addGroup(panelButtonLayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
										.addComponent(buttonReplaceAll)
										.addComponent(buttonReplace))
										.addGap(18, 18, 18)
										.addGroup(panelButtonLayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
												.addComponent(buttonClose)
												.addComponent(labelStatus))
												.addContainerGap(29, Short.MAX_VALUE))
		);

		panelFrame.add(panelButton, java.awt.BorderLayout.PAGE_END);

		GroupLayout layout = new GroupLayout(frame.getContentPane());
		frame.getContentPane().setLayout(layout);
		layout.setHorizontalGroup(
				layout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addComponent(panelFrame, GroupLayout.DEFAULT_SIZE, 468, Short.MAX_VALUE)
		);
		layout.setVerticalGroup(
				layout.createParallelGroup(GroupLayout.Alignment.LEADING)
				.addComponent(panelFrame, GroupLayout.DEFAULT_SIZE, 440, Short.MAX_VALUE)
		);
		
		
		buttonReplaceFind.setEnabled(false);
		buttonReplace.setEnabled(false);
		buttonFind.setEnabled(false);
		buttonReplaceAll.setEnabled(false);

		radioForward.setSelected(true);
		radioAll.setSelected(true);

		updateRecentSearch();
		updateRecentReplace();
		
		/*behaviour of buttons*/
		radioSelection.addActionListener(new ActionListener() {

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

					public void focusGained(FocusEvent e) {
						if (radioSelection.isSelected()) {

							Highlighter highlight = getEditor().getTextPane().getHighlighter();
							highlight.removeAllHighlights();

							radioSelection.setSelected(false);
							radioAll.setSelected(true);

							getEditor().getTextPane().removeFocusListener(this);
						}	
					}


					public void focusLost(FocusEvent e) {}
				});


			}
		});



		buttonFind.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				findText();
			}
		});

		buttonReplace.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JTextPane xpadTextPane =  getEditor().getTextPane();
				ScilabStyleDocument doc = (ScilabStyleDocument) xpadTextPane.getStyledDocument();
				boolean mergeMode = doc.getShouldMergeEdits();
				doc.setShouldMergeEdits(true);
				replaceOnlyText();
				doc.setShouldMergeEdits(mergeMode);
			}	

		});

		buttonReplaceFind.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JTextPane xpadTextPane =  getEditor().getTextPane();
				ScilabStyleDocument doc = (ScilabStyleDocument) xpadTextPane.getStyledDocument();
				boolean mergeMode = doc.getShouldMergeEdits();
				doc.setShouldMergeEdits(true);
				replaceText();
				findText();
				doc.setShouldMergeEdits(mergeMode);
			}
		});

		buttonReplaceAll.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JTextPane xpadTextPane =  getEditor().getTextPane();
				ScilabStyleDocument doc = (ScilabStyleDocument) xpadTextPane.getStyledDocument();
				String text = null;

				boolean wholeWordSelected  = checkWhole.isSelected() &&  checkWhole.isEnabled();
				boolean regexpSelected  = checkRegular.isSelected();

				// save current caret position to restore it at the end
				int currentCaretPos = xpadTextPane.getCaretPosition();

				if (radioSelection.isSelected()) {
					ScilabStyleDocument scilabDocument = (ScilabStyleDocument) xpadTextPane.getStyledDocument();
					text = searchManager.getSelectedDocumentLines(scilabDocument, startSelectedLines, endSelectedLines);
				} else {
					text = doc.getText();
				}

				Pattern pattern = null;

				oldWord = (String) comboFind.getEditor().getItem();
				newWord = (String) comboReplace.getEditor().getItem();


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
				if (replacedText.compareTo(text) != 0) {
					// only touch document if any replacement took place
					try {
						boolean mergeMode = doc.getShouldMergeEdits();
						doc.setShouldMergeEdits(true);
						doc.replace(startSelectedLines, text.length(), replacedText, null);
						doc.setShouldMergeEdits(mergeMode);
					} catch (BadLocationException e1) {
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

				
		/*comboReplace*/
		comboReplace.getEditor().getEditorComponent().addMouseListener(new MouseListener() {
			public void mouseReleased(MouseEvent e) {}
			public void mousePressed(MouseEvent e) {
				closeComboPopUp();
			}
			public void mouseExited(MouseEvent e) {}
			public void mouseEntered(MouseEvent e) {}
			public void mouseClicked(MouseEvent e) {}
		});
		
		comboReplace.getEditor().getEditorComponent().addKeyListener(new KeyListener() {
			public void keyTyped(KeyEvent e) {}
			public void keyReleased(KeyEvent e) {
				if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
					closeFindReplaceWindow();
				}
				
				if (e.getKeyCode() == KeyEvent.VK_ENTER) {
					replaceText();
					findText();
				}

				updateFindReplaceButtonStatus();
			}
			public void keyPressed(KeyEvent e) {}
		});

		comboReplace.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent arg0) {
				updateFindReplaceButtonStatus();
			}
		});
		
		comboReplace.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				updateFindReplaceButtonStatus();
			}
		});
		
		/*comboFind*/
		comboFind.getEditor().getEditorComponent().addMouseListener(new MouseListener() {
			public void mouseReleased(MouseEvent arg0) {}
			public void mousePressed(MouseEvent arg0) {
				closeComboPopUp();
			}
			public void mouseExited(MouseEvent arg0) {}
			public void mouseEntered(MouseEvent arg0) {}
			public void mouseClicked(MouseEvent arg0) {}
		});

		comboFind.addActionListener(new ActionListener() {
			
			public void actionPerformed(ActionEvent arg0) {
				updateFindReplaceButtonStatus();
			}
		});
		
		comboFind.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent arg0) {
				updateFindReplaceButtonStatus();
			}
		});

		comboFind.getEditor().getEditorComponent().addKeyListener(new KeyListener() {
			
			public void keyTyped(KeyEvent e) {}
			public void keyReleased(KeyEvent e) {
				if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
					closeFindReplaceWindow();
				}
				
				if (e.getKeyCode() == KeyEvent.VK_ENTER) {
					findText();
				}

				updateFindReplaceButtonStatus();
				
			}
			
			public void keyPressed(KeyEvent e) {}
		});
		
		
		frame.addWindowListener(new WindowListener() {
			public void windowClosed(WindowEvent e) {}
			public void windowDeiconified(WindowEvent e) {}
			public void windowActivated(WindowEvent e) {}
			
			public void windowClosing(WindowEvent e) {
				FindAction.windowAlreadyExist = false;
				frame.dispose();

			}
			
			public void windowDeactivated(WindowEvent e) {}
			public void windowIconified(WindowEvent e) {}
			public void windowOpened(WindowEvent e) {}
		});
		
	}

	private void closeComboPopUp() {
		if(comboFind.isPopupVisible()) {
			comboFind.hidePopup();
		}

		if(comboReplace.isPopupVisible()) {
			comboReplace.hidePopup();
		}
	}

	private void updateRecentSearch() {
		Object old = comboFind.getEditor().getItem();
		comboFind.removeAllItems();
		ArrayList<String> recentFind = ConfigXpadManager.getRecentSearch();
		for(String item : recentFind) {
			comboFind.addItem(item);
		}

		comboFind.getEditor().setItem(old);
	}

	private void updateRecentReplace() {
		Object old = comboReplace.getEditor().getItem();
		comboReplace.removeAllItems();
		ArrayList<String> recentReaplce = ConfigXpadManager.getRecentReplace();
		for(String item : recentReaplce) {
			comboReplace.addItem(item);
		}

		comboReplace.getEditor().setItem(old);
	}

	protected void updateFindReplaceButtonStatus() {
		String textFind = (String) comboFind.getEditor().getItem();
		String textReplace = (String) comboReplace.getEditor().getItem();
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
			buttonReplace.setEnabled(false);
			buttonReplaceAll.setEnabled(false);
			buttonReplaceFind.setEnabled(false);
		}



		// permit to choose "whole word" only if the input is a single word

		Pattern patternWholeWord = Pattern.compile("\\w*");
		Matcher matcherWholeWord = patternWholeWord.matcher(textFind);

		checkWhole.setEnabled(false);

		if (matcherWholeWord.find()) {
			if ((matcherWholeWord.end() - matcherWholeWord.start()) == textFind.length()) {
				checkWhole.setEnabled(true);
			}

		}

		// if we search a regexp, we first need to know if the regexp is valid or not
		if (checkRegular.isSelected()) {
			try {
				Pattern.compile(textFind);
				labelStatus.setText("");
				buttonFind.setEnabled(true);
				buttonReplaceAll.setEnabled(true);
			} catch (PatternSyntaxException pse) {

				labelStatus.setText(String.format(XpadMessages.INVALID_REGEXP, textFind));

				buttonFind.setEnabled(false);
				buttonReplaceAll.setEnabled(false);

			}


		}

		if (buttonReplace.isEnabled() && oldWord != null && oldWord.compareTo(textFind) != 0) {
			buttonReplace.setEnabled(false);
			buttonReplaceFind.setEnabled(false);
		}
	}

	/**
	 * findText
	 */
	private void findText() {

		boolean wrapSearchSelected = checkWarp.isSelected();
		boolean forwardSearch = radioForward.isSelected();
		boolean backwardSearch = radioBackward.isSelected();
		boolean caseSensitiveSelected  = checkCase.isSelected();
		boolean wholeWordSelected  = checkWhole.isSelected() &&  checkWhole.isEnabled();
		boolean regexpSelected  = checkRegular.isSelected();

		boolean onlySelectedLines = radioSelection.isSelected();

		int saveStart 	= startFindSelection;
		int saveEnd 	= endFindSelection;

		String exp = (String) comboFind.getEditor().getItem();
		if(exp.compareTo("") == 0) {
			return;
		}
		wordToFind = exp;
		ConfigXpadManager.saveRecentSearch(wordToFind);
		updateRecentSearch();


		JTextPane xpadTextPane =  getEditor().getTextPane();
		ScilabStyleDocument scilabStyle = ((ScilabStyleDocument) xpadTextPane.getStyledDocument());

		/*mainly used in case of selected text, otherwise currentPosStart =  currentPosEnd*/
		int currentCaretPos = 0;

		//Get the word we have to find
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

		labelStatus.setText("");

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
						labelStatus.setText(XpadMessages.PASSED_BEGIN_OF_DOCUMENT);
						nextIndex = offsets.size() - 1;
					} else {
						labelStatus.setText(XpadMessages.PASSED_END_OF_DOCUMENT);
						nextIndex = 0;
					}
				} else {
					if (backwardSearch) {
						labelStatus.setText(XpadMessages.BEGIN_OF_DOCUMENT);
					} else {
						labelStatus.setText(XpadMessages.END_OF_DOCUMENT);
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
			labelStatus.setText(String.format(XpadMessages.STRING_NOT_FOUND, wordToFind));

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
		replaceText();
		offsets.clear();
		buttonReplace.setEnabled(false);
		buttonReplaceFind.setEnabled(false);
	}

	/**
	 * replaceText
	 */

	private void replaceText() {

		boolean regexpSelected  = checkRegular.isSelected();

		String find = (String) comboFind.getEditor().getItem();
		String replace = (String) comboReplace.getEditor().getItem();

		if(find.compareTo("") == 0 || replace.compareTo("") == 0) {
			return;
		}
		oldWord = (String) find;
		newWord = (String) replace;

		ConfigXpadManager.saveRecentSearch(find);
		ConfigXpadManager.saveRecentReplace(replace);
		updateRecentSearch();
		updateRecentReplace();
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
			Highlighter highlight = Xpad.getEditor().getTextPane().getHighlighter();
			highlight.removeAllHighlights();
			frame.dispose();
			FindAction.windowAlreadyExist = false;

		}
	}
}
