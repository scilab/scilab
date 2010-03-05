/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.style;

import java.awt.Color;
import java.util.ArrayList;
import java.util.Hashtable;

import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.Element;

import org.scilab.modules.xpad.utils.ConfigXpadManager;

/**
 * Class for Scilab Style Documents
 * @author Sylvestre Koumar
 *
 */
public class ScilabStyleDocument extends DefaultStyledDocument {


	private boolean contentModified;
	/*if you want to add a new style just add it in the xml*/
	private ArrayList<String> listStylesName;
	//private final String[] allStyles = {"Operator", "Command","String","Bool" ,"Comment"};
	private Style defaultStyle;

	// Editor's default encoding is UTF-8
	private String encoding = "UTF-8";
	private boolean updater = true;
	private boolean autoIndent;
	private boolean autoColorize;
	private volatile boolean shouldMergeEdits = false;
	private boolean undoManagerEnabled;

	private String eolStyle = System.getProperty("line.separator");

	/**
	 * get Encoding
	 * @return String encoding
	 */
	public String getEncoding() {
		return encoding;
	}

	/**
	 * set Encoding 
	 * @param encoding encoding
	 */
	public void setEncoding(String encoding) {
		this.encoding = encoding;
	}

	/**
	 * set end of line value
	 * @param eol String
	 */
	public void setEOL(String eol) {
		this.eolStyle = eol;
	}

	/**
	 * get end of line
	 * @return end of line
	 */
	public String getEOL() {
		return this.eolStyle;
	}

	/**
	 * getAutoColorize
	 * @return true or false
	 */
	public boolean getAutoColorize() {
		return autoColorize;
	}

	/**
	 * setAutoColorize
	 * @param b boolean
	 */
	public void setAutoColorize(boolean b) {
		autoColorize = b;
	}

	/**
	 * isUpdater
	 * @return boolean
	 */
	public boolean isUpdater() {
		return updater;
	}
	
	/**
	 * getAutoIndent
	 * @return boolean
	 */
	public boolean getAutoIndent() {
		DEBUG("getAutoIndent(" + autoIndent + ")");
		return autoIndent;
	}
	
	/**
	 * setAutoIndent
	 * @param b boolean
	 */
	public void setAutoIndent(boolean b) {
		DEBUG("setAutoIndent(" + b + ")");
		autoIndent = b;
	}

	/**
	 * setUpdater
	 * @param updaterDisabled boolean
	 */
	public void setUpdater(boolean updaterDisabled) {
		this.updater = updaterDisabled;
	}

	private CompoundUndoManager undo = new CompoundUndoManager();	

	/**
	 * Default Constructor
	 */
	public ScilabStyleDocument() {
		super();
		setAsynchronousLoadPriority(2);

		autoIndent = ConfigXpadManager.getAutoIndent();
		autoColorize = ConfigXpadManager.getAutoColorize();
		encoding = ConfigXpadManager.getDefaultEncoding();

		Hashtable< String, Color> stylesColorsTable =  ConfigXpadManager.getAllForegroundColors();
		Hashtable< String, Boolean> stylesIsBoldTable = ConfigXpadManager.getAllisBold();
		listStylesName  =  ConfigXpadManager.getAllStyleName();

		//xpadStyles = XpadStyles.getInstance();
		//addDocumentListener(this); // TODO: check
		addUndoableEditListener(undo);
		undoManagerEnabled = true;
		defaultStyle = this.addStyle("Default", null);
		StyleConstants.setBold(defaultStyle, stylesIsBoldTable.get("Default"));
		StyleConstants.setFontFamily(defaultStyle, ConfigXpadManager.getFont().getFontName());
		StyleConstants.setForeground(defaultStyle, stylesColorsTable.get("Default"));
		StyleConstants.setFontSize(defaultStyle, ConfigXpadManager.getFontSize());
		StyleConstants.setLeftIndent(defaultStyle, 0);

		/* set default style settings*/
		/*that way if we want to had a new style, we just need to had an element to the xml*/
		for (int i = 0; i < listStylesName.size(); ++i) {
			Style otherStyle = this.addStyle(listStylesName.get(i), defaultStyle);
			StyleConstants.setBold(otherStyle, stylesIsBoldTable.get(listStylesName.get(i)));
			StyleConstants.setForeground(otherStyle, stylesColorsTable.get(listStylesName.get(i)));
		}

		contentModified = false;

	}
	
	/**
	 * Get document style
	 * @param styleString style
	 * @return style
	 */
	public Style getStyle(String styleString) {
		Style style = super.getStyle(styleString);
		if (style == null) {
			super.getStyle("Default");
		}
		return style;
	}
	
	// TODO: check usefulness of this method
	/**
	 * getScilabDocument
	 * @return ScilabStyleDocument
	 */
	public ScilabStyleDocument getScilabDocument() {
		return this;
	}

	/**
	 * Get document text
	 * @return String
	 */
	public String getText() {
		try {
			return getText(0, getLength());
		} catch (BadLocationException e) {
			return "";
		}
	}

	/**
	 * setShouldMergeEdits
	 * @param b boolean
	 */
	public void setShouldMergeEdits(boolean b) {

		if (shouldMergeEdits) {
			if (!b) { // ending compound editing with a new CaretEdit
				undo.endCompoundEdit();
			}
		} else {
			if (b) { // starting compound editing
				undo.startCompoundEdit();
			}
		}
		shouldMergeEdits = b;

	}

	/**
	 * getShouldMergeEdits
	 * @return boolean
	 */
	public boolean getShouldMergeEdits() {
		return shouldMergeEdits;
	}

	/**
	 * getColorize
	 * @return boolean
	 */
	public boolean getColorize() {
		//DEBUG("setColorize("+autoColorize+")");
		return autoColorize;
	}

	/**
	 * setColorize
	 * @param b boolean
	 */
	public void setColorize(boolean b) {
		//DEBUG("setColorize("+b+")");
		autoColorize = b;
	}

	/**
	 * DEBUG
	 * @param msg string
	 */
	private final void DEBUG(String msg) {
		//System.err.println("[DEBUG] "+msg);
	}

	/**
	 * getUndoManager
	 * @return CompoundUndoManager
	 */
	public CompoundUndoManager getUndoManager() {
		return undo;
	}

	/**
	 * disableUndoManager
	 */
	public void disableUndoManager() {
		if (undoManagerEnabled) {
			this.removeUndoableEditListener(undo);
			undoManagerEnabled = false;
		}
	}

	/**
	 * enableUndoManager
	 */
	public void enableUndoManager() {
		if (!undoManagerEnabled) {
			undoManagerEnabled = true;
			this.addUndoableEditListener(undo);
			undoManagerEnabled = true;
		}
	}

	/**
	 * isContentModified
	 * @return boolean
	 */
	public boolean isContentModified() {
		return contentModified && !undo.isAtReference();
	}

	/**
	 * setContentModified
	 * @param contentModified boolean
	 */
	public void setContentModified(boolean contentModified) {
		this.contentModified = contentModified;
		if (contentModified == false) {
			undo.setReference();
		}
	}
	/**
	 * dump document on stderr with line positions 
	 */
	public void dump() {
		readLock();
		try {
			Element root = getDefaultRootElement();
			for (int i = 0; i != root.getElementCount(); ++i) {
				Element e = root.getElement(i);
				int start = e.getStartOffset();
				int end = e.getEndOffset();
				System.err.println("line " + i + " from: " + start + "to: " + end + ":|" + getText(start, end - start) + "|");
			}
		} catch (BadLocationException e) {
			System.err.println(e);
		}
		readUnlock();

	}

	/**
	 * lock
	 */
	public void lock() {
		super.writeLock();
	}
	
	/**
	 * unlock
	 */
	public void unlock() {
		super.writeUnlock();
	}

}
