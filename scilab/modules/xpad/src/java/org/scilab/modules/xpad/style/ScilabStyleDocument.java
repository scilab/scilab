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
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.UndoableEditEvent;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Element;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.undo.CompoundEdit;
import javax.swing.undo.UndoManager;
import javax.swing.undo.UndoableEdit;

import org.scilab.modules.xpad.CaretEdit;
import org.scilab.modules.xpad.ScilabKeywords;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.actions.ColorizeAction;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import javax.swing.undo.UndoManager;
import java.nio.charset.Charset;

public class ScilabStyleDocument extends DefaultStyledDocument implements DocumentListener {
	
	private String eventType;
	
	private boolean contentModified;
	private Xpad editor;	
	/*if you want to add a new style just add it in the xml*/
	private ArrayList<String> listStylesName;
	//private final String[] allStyles = {"Operator", "Command","String","Bool" ,"Comment"};
	private Style defaultStyle;

	private String encoding = Charset.defaultCharset().toString();
	private boolean updater;
	private boolean autoIndent;
	private boolean autoColorize;
	private volatile boolean shouldMergeEdits;
	private CompoundEdit compoundEdit;

	public String getEncoding(){
		return encoding;
	}
	public void setEncoding(String encoding){
		this.encoding = encoding;
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

	 public CompoundEdit getCompoundEdit() {
		 return compoundEdit;
	 }
	 
	 public void setShouldMergeEdits(boolean b) {
		 if (shouldMergeEdits) {
			 if (!b) { // ending compound editing
				 compoundEdit.end();
				 undo.addEdit(compoundEdit);
				 compoundEdit = null;
			 }
		 } else {
			 if (b) { // starting compound editing
				 compoundEdit = new CompoundEdit();
			 }
		 }
		 shouldMergeEdits = b;
	 }
	 
	 public boolean getShouldMergeEdits() {
		 return shouldMergeEdits;
	 }
	 
    private UndoManager undo = new UndoManager() {
    	public void undoableEditHappened(UndoableEditEvent e) {
				
    		if ((eventType.equals(DocumentEvent.EventType.INSERT.toString()) 
    				|| eventType.equals(DocumentEvent.EventType.REMOVE.toString()))
    				&& (e.getEdit().canUndo())) {
    			/*
				if ( EventType.equals(DocumentEvent.EventType.REMOVE.toString())){
					System.out.println("remove");
					System.out.println(indentInprogress);
				}
    			 */
    			((UndoableEdit) (shouldMergeEdits ?  compoundEdit: this)).addEdit(e.getEdit());
				eventType = "";
    		}

	}
    };
    	

	public ScilabStyleDocument() {
		super();
		setAsynchronousLoadPriority(2);
		eventType = new String();
		
		Hashtable< String, Color> stylesColorsTable =  ConfigXpadManager.getAllForegroundColors();
		Hashtable< String, Boolean> stylesIsBoldTable = ConfigXpadManager.getAllisBold() ;
		listStylesName  =  ConfigXpadManager.getAllStyleName();

		//xpadStyles = XpadStyles.getInstance();
		addDocumentListener(this);
		this.addUndoableEditListener(this.getUndoManager());
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

	// TODO: check usefulness of this method
	public ScilabStyleDocument getScilabDocument(){
		return this;
	}

	public String getText(){
		String res ="";
		try{
			res = getText(0, getLength());
		}catch(javax.swing.text.BadLocationException e){
			res= "";
		}
		return res;
	}
	
	
	/**
	 * DOCUMENT COLORISATION END
	 */
	


		
	/**
	 * DOCUMENT TABIFY ACTION
	 */
	
	
	/**
	 * FIND AND REPLACE END
	 */


	/**
	 * UTILITARIAN FUNCTIONS START
	 */

	
	/**
	 * UTILITARIAN FUNCTIONS END
	 */

	private final void DEBUG(String msg) {
		//System.err.println("[DEBUG] "+msg);
	}
	

	public void insertUpdate(DocumentEvent e) {
		
		if (e != null) {
			eventType = e.getType().toString();
		}
		
		DEBUG("--- Calling insertUpdate");
		/* TODO : move to DocumentListener s in ColorizeAction and IndentAction
		if (!updateManager.isUpdater()) {

			if (autoColorize) {
				DEBUG("--- Calling insertUpdate -> colorize");
			    SwingUtilities.invokeLater(ColorizationManager.new ColorUpdater(this, e));
			}
			if (autoColorize) {
			    DEBUG("--- Calling insertUpdate -> indent");
			    SwingUtilities.invokeLater(IndentManager.new IndentUpdater(this,e));
			}
		}
		*/
	}
	

	
	
	public void removeUpdate(DocumentEvent e) {
		
		eventType = e.getType().toString();
		//System.err.println("--- Calling ScilabStyleDocument.removeUpdate");
		/* TODO: put in a DocumentListener in ColorizeAction
		if (!updateManager.isUpdater()) {
			if (colorizationManager.isAutoColorize()) {
			    SwingUtilities.invokeLater(colorizationManager.new ColorUpdater(getScilabDocument(), e));
			}
		}
		*/
	}

	public void changedUpdate(DocumentEvent arg0) {
		
		eventType = arg0.getType().toString();
		// TODO Auto-generated method stub
	}

	public UndoManager getUndoManager() {
		return undo;
	}
	
	public boolean isContentModified() {
		return contentModified;
	}
	
	public void setContentModified(boolean contentModified) {
		this.contentModified = contentModified;
	}


	public Xpad getEditor() {
		return editor;
	}

	public void setEditor(Xpad editor) {
		this.editor = editor;
	}    

	public void disableUndoManager(){
		this.removeUndoableEditListener(undo);
	}
	
	public void enableUndoManager(){
        this.addUndoableEditListener(undo);
	}

}
