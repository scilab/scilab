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

import javax.swing.event.UndoableEditEvent;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.undo.CompoundEdit;
import javax.swing.undo.UndoManager;
import javax.swing.undo.UndoableEdit;

import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class ScilabStyleDocument extends DefaultStyledDocument {

	
	private boolean contentModified;
	/*if you want to add a new style just add it in the xml*/
	private ArrayList<String> listStylesName;
	//private final String[] allStyles = {"Operator", "Command","String","Bool" ,"Comment"};
	private Style defaultStyle;

	// Editor's default encoding is UTF-8
	private String encoding = "UTF-8";
	private boolean updater= true;
	private boolean autoIndent =true;
	private boolean autoColorize = true;
	private volatile boolean shouldMergeEdits;
	private CompoundEdit compoundEdit;
	
	private String eolStyle = System.getProperty("line.separator");

	public String getEncoding() {
		return encoding;
	}
	public void setEncoding(String encoding) {
		this.encoding = encoding;
	}

	/**
	 * set end of line value
	 * @param eol
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
	
	public boolean getAutoColorize() {
		return autoColorize;
	}

	 public void setAutoColorize(boolean b) {
		 autoColorize = b;
	 }
	
	 public boolean isUpdater() {
		 return updater;
	 }
	 public boolean getAutoIndent() {
		 DEBUG("getAutoIndent("+autoIndent+")");
		 return autoIndent;
	 }
	 public void setAutoIndent(boolean b) {
		 DEBUG("setAutoIndent("+b+")");
		 autoIndent = b;
	 }

	 public void setUpdater(boolean updaterDisabled) {
		 this.updater = updaterDisabled;
	 }

	 
    private UndoManager undo = new UndoManager() {
    	public void undoableEditHappened(UndoableEditEvent e) {
				
    			((UndoableEdit) (shouldMergeEdits ?  compoundEdit: this)).addEdit(e.getEdit());
	
	}
    };
    	

	public ScilabStyleDocument() {
		super();
		setAsynchronousLoadPriority(2);
		
		Hashtable< String, Color> stylesColorsTable =  ConfigXpadManager.getAllForegroundColors();
		Hashtable< String, Boolean> stylesIsBoldTable = ConfigXpadManager.getAllisBold() ;
		listStylesName  =  ConfigXpadManager.getAllStyleName();

		//xpadStyles = XpadStyles.getInstance();
		//addDocumentListener(this); // TODO: check
		addUndoableEditListener(undo);
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
		
		contentModified=false;

	}
	public Style getStyle(String styleString){
		Style style = super.getStyle(styleString);
		if(style == null) {
			super.getStyle("Default");
		}
		return style;
	 }
	// TODO: check usefulness of this method
	public ScilabStyleDocument getScilabDocument(){
		return this;
	}

	public String getText(){
		try {
			return getText(0, getLength());
		} catch(javax.swing.text.BadLocationException e) {
			return "";
		}
	}
	

	public void setShouldMergeEdits(boolean b) {
	
		if(shouldMergeEdits) {
			if(!b) { // ending compound editing with a new CaretEdit
				compoundEdit.end();
				undo.addEdit(compoundEdit);
				compoundEdit = null;
				
			}
		} else {
			if(b) { // starting compound editing
				compoundEdit = new CompoundEdit();
			}
		}
		shouldMergeEdits = b;
		
	}
	
	public boolean getShouldMergeEdits() {
		return shouldMergeEdits;
	}
	
	public boolean getColorize() {
		//DEBUG("setColorize("+autoColorize+")");
		return autoColorize;
	}

	public void setColorize(boolean b) {
		//DEBUG("setColorize("+b+")");
		autoColorize = b;
	}


	private final void DEBUG(String msg) {
		//System.err.println("[DEBUG] "+msg);
	}



	public UndoManager getUndoManager() {
		return undo;
	}

	public void disableUndoManager() {
		this.removeUndoableEditListener(undo);
	}
	
	public void enableUndoManager() {
        this.addUndoableEditListener(undo);
	}

	public boolean isContentModified() {
		return contentModified;
	}
	
	public void setContentModified(boolean contentModified) {
		this.contentModified = contentModified;
	}
	
}