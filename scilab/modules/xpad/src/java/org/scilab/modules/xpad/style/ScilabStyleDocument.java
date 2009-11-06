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

public class ScilabStyleDocument extends DefaultStyledDocument implements DocumentListener {
	
	private String eventType;
	
	private boolean contentModified;
	private Xpad editor;	
	/*if you want to add a new style just add it in the xml*/
	private ArrayList<String> listStylesName;
	//private final String[] allStyles = {"Operator", "Command","String","Bool" ,"Comment"};
	private Style defaultStyle;

	private UpdateManager updateManager = new UpdateManager();
	private IndentManager indentManager = new IndentManager();
	private ColorizationManager colorizationManager = new ColorizationManager();	
	private CommentManager commentManager = new CommentManager();
	private SearchManager searchManager = new SearchManager();
	private TabManager tabManager = new TabManager();
	public EncodingManager getEncodingManager() {
		return encodingManager;
	}

	public void setEncodingManager(EncodingManager encodingManager) {
		this.encodingManager = encodingManager;
	}



	private EncodingManager encodingManager = new EncodingManager();
	
	public TabManager getTabManager() {
		return tabManager;
	}

	public void setTabManager(TabManager tabManager) {
		this.tabManager = tabManager;
	}

	public SearchManager getSearchManager() {
		return searchManager;
	}

	public void setSearchManager(SearchManager searchManager) {
		this.searchManager = searchManager;
	}

	public ColorizationManager getColorizationManager() {
		return colorizationManager;
	}

	public CommentManager getCommentManager() {
		return commentManager;
	}

	public void setCommentManager(CommentManager commentManager) {
		this.commentManager = commentManager;
	}

	public void setColorizationManager(ColorizationManager colorizationManager) {
		this.colorizationManager = colorizationManager;
	}

	public IndentManager getIndentManager() {
		return indentManager;
	}

	public void setIndentManager(IndentManager indentManager) {
		this.indentManager = indentManager;
	}




	public UpdateManager getUpdateManager() {
		return updateManager;
	}

	public void setUpdateManager(UpdateManager updateManager) {
		this.updateManager = updateManager;
	}

	
	
	
	
	//private XpadStyles xpadStyles; 
	



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
			if (!indentManager.isIndentInprogress()) { 
				((UndoableEdit) (updateManager.getShouldMergeEdits() ?  updateManager.getCompoundEdit(): this)).addEdit(e.getEdit());
				eventType = "";
			}
		}

	}
    };
    	

	public ScilabStyleDocument(Xpad editor) {
		super();
		setAsynchronousLoadPriority(2);
		eventType = new String();
		
		this.editor = editor;
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
		
		this.addDocumentListener(new DocumentListener() {
			
				public void changedUpdate(DocumentEvent documentEvent) {
				}
			      
				public void insertUpdate(DocumentEvent documentEvent) {
					handleEvent(documentEvent);
			    }
			    
			    public void removeUpdate(DocumentEvent documentEvent) {
			    	handleEvent(documentEvent);
			    }
			    
			    private void handleEvent(DocumentEvent documentEvent) {
			        DocumentEvent.EventType type = documentEvent.getType();
			        if (type.equals(DocumentEvent.EventType.INSERT) || type.equals(DocumentEvent.EventType.REMOVE) ) {
			         
			        	int index = getEditor().getTabPane().getSelectedIndex();
			        	if (!isContentModified()) {
			        		getEditor().getTabPane().setTitleAt(index, "*" + getEditor().getTabPane().getTitleAt(index ) );
			        	}
			        	setContentModified(true);
			        }  

			   }
		});
	}


	public ScilabStyleDocument getScilabDocument(){
		return this;
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
	
	public String getFullDocument() {
		/*
		int startOffset;
		int endOffset;
		String textLine = "";
		StringBuffer text = new StringBuffer();
		
		//We read the document and put the document into the String text
		for (int i = 0; i < this.getLength();) {
			startOffset = this.getParagraphElement(i).getStartOffset();
			endOffset = this.getParagraphElement(i).getEndOffset();

			try {
				//Get the document line by line
				textLine = this.getText(startOffset, endOffset - startOffset);
			} catch (BadLocationException ex) {
				ex.printStackTrace();
			}
			i = endOffset;
			text.append(textLine);
		}
		return text.toString();*/
		String textLine = "";
		try {
			//Get the document line by line
			textLine = this.getText(0, getLength());
		} catch (BadLocationException ex) {
			ex.printStackTrace();
		}
		return textLine;
	}

	public void insertUpdate(DocumentEvent e) {
		
		if (e != null) {
			eventType = e.getType().toString();
		}
		
		DEBUG("--- Calling insertUpdate");
		if (!updateManager.isUpdater()) {

			if (colorizationManager.isAutoColorize()) {
				DEBUG("--- Calling insertUpdate -> colorize");
			    SwingUtilities.invokeLater(colorizationManager.new ColorUpdater(this, e));
			}
			if (colorizationManager.isAutoColorize()) {
			    DEBUG("--- Calling insertUpdate -> indent");
			    SwingUtilities.invokeLater(indentManager.new IndentUpdater(this,e));
			}
		}
	}
	

	
	
	public void removeUpdate(DocumentEvent e) {
		
		eventType = e.getType().toString();
		//System.err.println("--- Calling ScilabStyleDocument.removeUpdate");
		if (!updateManager.isUpdater()) {
			if (colorizationManager.isAutoColorize()) {
			    SwingUtilities.invokeLater(colorizationManager.new ColorUpdater(getScilabDocument(), e));
			}
		}
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
		editor.updateTabTitle();
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
