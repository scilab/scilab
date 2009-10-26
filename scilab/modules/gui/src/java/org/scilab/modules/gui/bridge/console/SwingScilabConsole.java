/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.console;

import java.awt.Dimension;
import java.awt.MouseInfo;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;

import javax.swing.JPanel;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyleContext;
import javax.swing.text.StyledDocument;

import org.scilab.modules.console.OneCharKeyEventListener;
import org.scilab.modules.console.SciConsole;
import org.scilab.modules.console.SciHistoryManager;
import org.scilab.modules.console.SciOutputView;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.localization.Messages;

import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.util.StringConstants;

/**
 * Swing implementation for Scilab Console in GUIs
 * @author Vincent COUVERT
 */
public class SwingScilabConsole extends SciConsole implements SimpleConsole {

	private static final long serialVersionUID = 1L;
		
	/**
	 * Constructor
	 */
	public SwingScilabConsole() {
		super(ConfigManager.getUserConfigFile());
		
		MouseListener contextMenu = new MouseListener() {

			public void mouseClicked(MouseEvent arg0) {
				if (arg0.getButton() == MouseEvent.BUTTON3) {
					SwingScilabContextMenu menu = new SwingScilabContextMenu();
					
					SwingScilabMenuItem cutMenu = new SwingScilabMenuItem();
					cutMenu.setText(Messages.gettext("Cut"));
					cutMenu.setCallback(ScilabCallBack.createCallback(
							"org.scilab.modules.gui.bridge.CallScilabBridge.cutConsoleSelection",
							ScilabCallBack.JAVA));
					cutMenu.setMnemonic('U');
					
					SwingScilabMenuItem copyMenu = new SwingScilabMenuItem();
					copyMenu.setText(Messages.gettext("Copy"));
					copyMenu.setCallback(ScilabCallBack.createCallback(
							"org.scilab.modules.gui.bridge.CallScilabBridge.copyConsoleSelection",
							ScilabCallBack.JAVA));
					copyMenu.setMnemonic('C');
					
					SwingScilabMenuItem pasteMenu = new SwingScilabMenuItem();
					pasteMenu.setText(Messages.gettext("Paste"));
					pasteMenu.setCallback(ScilabCallBack.createCallback(
							"org.scilab.modules.gui.bridge.CallScilabBridge.pasteClipboardIntoConsole",
							ScilabCallBack.JAVA));
					pasteMenu.setMnemonic('P');
					
					SwingScilabMenuItem clearHistoryMenu = new SwingScilabMenuItem();
					clearHistoryMenu.setText(Messages.gettext("Clear History"));
					clearHistoryMenu.setCallback(ScilabCallBack.createCallback(
							"org.scilab.modules.gui.bridge.CallScilabBridge.clearHistory", 
							ScilabCallBack.JAVA));
					clearHistoryMenu.setMnemonic('H');

					SwingScilabMenuItem clearMenu = new SwingScilabMenuItem();
					clearMenu.setText(Messages.gettext("Clear Console"));
					clearMenu.setCallback(ScilabCallBack.createCallback(
							"org.scilab.modules.gui.bridge.CallScilabBridge.clear",
							ScilabCallBack.JAVA));
					clearMenu.setMnemonic('O');

					SwingScilabMenuItem selectMenu = new SwingScilabMenuItem();
					selectMenu.setText(Messages.gettext("Select All"));
					selectMenu.setCallback(ScilabCallBack.createCallback(
							"org.scilab.modules.gui.bridge.CallScilabBridge.selectAllConsoleContents", 
							ScilabCallBack.JAVA));
					selectMenu.setMnemonic('S');

					menu.add(cutMenu);
					menu.add(copyMenu);
					menu.add(pasteMenu);

					menu.addSeparator();

					menu.add(clearHistoryMenu);
					menu.add(clearMenu);

					menu.addSeparator();
					
					menu.add(selectMenu);

					menu.setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
					menu.setVisible(true);
				}
			}

			public void mouseEntered(MouseEvent arg0) {
			}

			public void mouseExited(MouseEvent arg0) {
			}

			public void mousePressed(MouseEvent arg0) {
			}

			public void mouseReleased(MouseEvent arg0) {
			}
			
		};
		
		((JTextPane) getConfiguration().getOutputView()).addMouseListener(contextMenu);
		((JTextPane) getConfiguration().getInputCommandView()).addMouseListener(contextMenu);
		((JPanel) getConfiguration().getPromptView()).addMouseListener(contextMenu);
		
		((JTextPane) getConfiguration().getInputCommandView()).requestFocus();
	}
	
	/**
	 * Displays data in the console
	 * @param dataToDisplay the data to be displayed
	 * @see fr.scilab.console.HelpBrowser#display(java.lang.String)
	 */
	public void display(String dataToDisplay) {
		this.getConfiguration().getOutputView().append(dataToDisplay);
	}

	/**
	 * This method is used to display the prompt
	 */
	public void displayPrompt() {
		
		final InputCommandView inputCmdView = this.getConfiguration().getInputCommandView();
		// Show the prompt
		this.getConfiguration().getPromptView().setVisible(true);

		// Show the input command view and its hidden components
		inputCmdView.setEditable(true);

		((JTextPane) inputCmdView).setCaretColor(((JTextPane) inputCmdView).getForeground());
		
	   	SwingUtilities.invokeLater(new Runnable() {
    		public void run() {
    			((JTextPane) inputCmdView).getCaret().setVisible(true);
			}	
		});	
		
		// Remove last line returned given by Scilab (carriage return)
		try {
			StyledDocument outputStyledDoc = this.getConfiguration().getOutputViewStyledDocument();			
			int lastEOL = outputStyledDoc.getText(0, outputStyledDoc.getLength()).lastIndexOf(StringConstants.NEW_LINE);

			// Condition added to avoid a "javax.swing.text.BadLocationException: Invalid remove" exception
			if (lastEOL > 1 && (outputStyledDoc.getLength() - lastEOL) == 1) { 
				outputStyledDoc.remove(lastEOL, outputStyledDoc.getLength() - lastEOL);
			}
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
		
		updateScrollPosition();
	}

	/**
	 * Reads one user input char
	 * @return the data entered by the user
	 * @see fr.scilab.console.HelpBrowser#getCharWithoutOutput()
	 */
	public int getCharWithoutOutput() {
		int retChar;
		
		updateScrollPosition();
		
		// Avoids reading of an empty buffer
		try {
			((SciConsole) this).getCanReadUserInputValue().acquire();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		this.getConfiguration().getPromptView().setVisible(false);
		this.getConfiguration().getInputCommandView().setEditable(false);
		
		// Add a keylistener which will set the returned char
		OneCharKeyEventListener keyListener = new OneCharKeyEventListener(this);
		((JTextPane) this.getConfiguration().getInputCommandView()).addKeyListener(keyListener);
		((JTextPane) this.getConfiguration().getOutputView()).addKeyListener(keyListener);
		
		// Reads the buffer
		retChar = this.getUserInputValue();
		((SciConsole) this).getCanReadUserInputValue().release();

		// Remove the "more" message and replace it by an empty line
		this.clear(-1);
		this.display(StringConstants.NEW_LINE);

		// Remove the key listener
		((JTextPane) this.getConfiguration().getInputCommandView()).removeKeyListener(keyListener);
		((JTextPane) this.getConfiguration().getOutputView()).removeKeyListener(keyListener);
		
		this.getConfiguration().getPromptView().setVisible(true);
		this.getConfiguration().getInputCommandView().setEditable(true);

		// Send back the focus the the input view
		this.getConfiguration().getInputCommandView().reset();
		this.getConfiguration().getInputCommandView().requestFocus();

                final JTextPane cmdView = (JTextPane) this.getConfiguration().getInputCommandView();
	   	SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        cmdView.getCaret().setVisible(true);
                    }
                });
		
		return retChar;
	}

	/**
	 * Draw a console
	 */
	public void draw() {
		super.setVisible(true);
		super.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab console
	 * @return the size of the console
	 */
	public Size getDims() {
		return new Size(super.getWidth(), super.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
	 * @return the position of the console
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Gets the visibility status of a console
	 * @return the visibility status of the console (true if the console is visible, false if not)
	 */
	public boolean isVisible() {
		return super.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab console
	 * @param newSize the size we want to set to the console
	 */
	public void setDims(Size newSize) {
		this.setPreferredSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
	 * @param newPosition the position we want to set to the console
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Sets the visibility status of a Scilab console
	 * @param newVisibleState the visibility status we want to set to the console
	 */
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
	}

	/**
	 * Clears the Console
	 */
	public void clear() {
		super.clear();
	}

	/**
	  * Sets the prompt displayed in the console
	  * @param prompt the prompt to be displayed in the console
	  */
	public void setPrompt(String prompt) {
		this.getConfiguration().getPromptView().setDefaultPrompt(prompt);
	}
	
	/**
	 * Clear the commands history 
	 */
	public void clearHistory() {
		((SciHistoryManager) this.getConfiguration().getHistoryManager()).reset();
	}
	
	/**
	 * Paste clipboard contents in Console input line
	 */
	public void pasteClipboard() {
		// Gets the contents of the clipboard
		Toolkit toolkit = Toolkit.getDefaultToolkit();
		Clipboard systemClipboard = toolkit.getSystemClipboard();

		// Verify that clibpboard data is of text type
		boolean dataAvailable;
		try {
			dataAvailable = systemClipboard.isDataFlavorAvailable(DataFlavor.stringFlavor);
		} catch (IllegalStateException exception) {
			return;
		}

		// Exit if text data not available
		if (!dataAvailable) {
			return;
		}

		// Read data
		String clipboardContents = null;
		try {
			clipboardContents = (String) systemClipboard.getData(DataFlavor.stringFlavor);
		} catch (UnsupportedFlavorException e1) {
			// Should never be here
			e1.printStackTrace();
		} catch (IOException e1) {
			// Should never be here
			e1.printStackTrace();
		}
		
		JTextPane input = ((JTextPane) this.getConfiguration().getInputCommandView());
		StyledDocument doc = input.getStyledDocument();
		
		// If some text selected then it is replaced
		if (input.getSelectedText() != null) {
			try {
				doc.remove(input.getSelectionStart(), input.getSelectionEnd() - input.getSelectionStart());
			} catch (BadLocationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		// Insert clipboard contents
		try {
			doc.insertString(((JTextPane) this.getConfiguration().getInputCommandView()).getCaretPosition(),
					clipboardContents, doc.getStyle(StyleContext.DEFAULT_STYLE));
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * Select all the console contents
	 */
	public void selectAll() {
		JTextPane output = (JTextPane) this.getConfiguration().getOutputView();
		output.setSelectionStart(0);
		output.setSelectionEnd(output.getText().length());
		// TODO should also select the prompt and the input
	}
	
	/**
	 * Put the console selected text in the clipboard
	 */
	public void copyToClipboard() {
		JTextPane output = (JTextPane) this.getConfiguration().getOutputView();
		JTextPane input = (JTextPane) this.getConfiguration().getInputCommandView();
		
		String selection = "";
		if (output.getSelectedText() != null) {
			selection += output.getSelectedText();
		}
		// TODO should also copy the prompt
		if (input.getSelectedText() != null) {
			selection += input.getSelectedText();
		}
		
		Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(selection), null);

	}
	
	/**
	 * Cut selected text in the Console input line
	 */
	public void cutSelection() {
		JTextPane input = (JTextPane) this.getConfiguration().getInputCommandView();
		StyledDocument doc = input.getStyledDocument();
		
		// If some text selected then it is replaced
		if (input.getSelectedText() != null) {
			try {
				/* Put the selection in the clipboard */
				Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(input.getSelectedText()), null);
				/* Remove selected text */
				doc.remove(input.getSelectionStart(), input.getSelectionEnd() - input.getSelectionStart());
			} catch (BadLocationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * Set the maximum number of lines stored in the Output
	 * @param nbLines the number of lines 
	 */
	public void setMaxOutputSize(int nbLines) {
		((SciOutputView) this.getConfiguration().getOutputView()).setMaxSize(nbLines);
	}

}
