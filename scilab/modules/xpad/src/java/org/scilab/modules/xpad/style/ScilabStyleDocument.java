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
import java.util.Collections;
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
import javax.swing.undo.UndoManager;
import javax.swing.text.DocumentFilter;
import javax.swing.text.AttributeSet;

import org.scilab.modules.xpad.ScilabKeywords;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.actions.ColorizeAction;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class ScilabStyleDocument extends DefaultStyledDocument implements DocumentListener {

	private UndoManager undo = new UndoManager() {
		public void undoableEditHappened(UndoableEditEvent e) {
					
			if ( (EventType.equals(DocumentEvent.EventType.INSERT.toString()) 
						|| EventType.equals(DocumentEvent.EventType.REMOVE.toString()) )
				&& (e.getEdit().canUndo()) ){
				/*
				if ( EventType.equals(DocumentEvent.EventType.REMOVE.toString())){
					System.out.println("remove");
					System.out.println(indentInprogress);
				}
				*/
				if (!indentInprogress){ 
				undo.addEdit(e.getEdit());
				
				EventType = "";
				}
			}

					
		}
	};

	private volatile boolean autoIndent         = true;
	private volatile boolean autoColorize       = true;
	private volatile boolean colorizeInprogress = false;
	private volatile boolean indentInprogress   = false;
	private volatile boolean updaterDisabled    = false;

	private String EventType;
	
	//private final String[] quotations = {"[^A-Z](\"|')[^{\n}]*?(\"|')"};
	private final String[] quotations = {"(\"|')([^\\n])*?(\"|')"};
	private final String[] bools = {"%T", "%F", "%t", "%f"};
	private final String[] comments = {"//[^{\n}]*"};
	private final String[] operators = {"=", "\\+", "-", "\\*", "/", "\\\\", "\\^", 
			"\\./", "\\.\\\\", "\\.\\^", 
			"\\.\\*\\.", "\\./\\.", "\\.\\\\\\.",
			"==", "<", ">", "<=", ">=", "~=", "@=",
			"&", "\\|", "@", "~",
	"\\.\\.[\\.]*"};
	
	private Pattern patternIn;
	private Pattern patternInOut;
	private Pattern patternOut;
	private Pattern patternComment;
	private Pattern patternQuote;
	
	private Pattern patternSpace;

	private final String IN = "IN";
	private final String OUT = "OUT";
	private final int BOOLS = 0;
	private final int COMMANDS = 1;
	private final int COMMENTS = 2;
	private final int FUNCTIONS = 3;
	private final int MACROS = 4;
	private final int OPERATORS = 5;
	private final int QUOTATIONS = 6;
	
	private int currentLevelIdent = 0;
	private String currentStringIndent = "";
	private String tabulation = "  ";
	
	
	
	private int lineStartPosition;
	private int lineEndPosition;
	private boolean singleLine = false;
	private int currentLine;
	
	private boolean contentModified;
	
	//private XpadStyles xpadStyles; 
	
	Hashtable<String, String[]> keywords;
	String[] commands;
	String[] functions;
	String[] macros;
	
	Xpad editor;

	/*if you want to add a new style just add it in the xml*/
	private ArrayList<String> listStylesName;
	//private final String[] allStyles = {"Operator", "Command","String","Bool" ,"Comment"};
	private Style defaultStyle;


	public ScilabStyleDocument(Xpad editor) {
		super();
		setAsynchronousLoadPriority(2);
		EventType = new String();
		
		patternIn = Pattern.compile("(\\b(if|while|for|select|function)\\b)"); // do should change nothing to indent
		patternInOut = Pattern.compile("\\b(else|elseif|case)\\b");
		patternOut = Pattern.compile("\\b(end|endfunction)\\b");
		patternComment = Pattern.compile("(.*?)//");
		patternQuote = Pattern.compile("(\"|')[^\\n]*?(\"|')");
		
		patternSpace = Pattern.compile("\\s*");
		
		this.editor = editor;
		Hashtable< String, Color>stylesColorsTable =  ConfigXpadManager.getAllForegroundColors();
		Hashtable< String, Boolean>stylesIsBoldTable = ConfigXpadManager.getAllisBold() ;
		listStylesName  =  ConfigXpadManager.getAllStyleName();

		//xpadStyles = XpadStyles.getInstance();
		addDocumentListener(this);
		addUndoableEditListener(undo);
		defaultStyle = this.addStyle("Default", null);
		StyleConstants.setBold(defaultStyle, stylesIsBoldTable.get("Default"));
		StyleConstants.setFontFamily(defaultStyle, ConfigXpadManager.getFont().getFontName() );
		StyleConstants.setForeground(defaultStyle, stylesColorsTable.get("Default"));
		StyleConstants.setFontSize(defaultStyle, ConfigXpadManager.getFontSize());
		StyleConstants.setLeftIndent(defaultStyle, 0);

		/* set default style settings*/
		/*that way if we want to had a new style, we just need to had an element to the xml*/
		for(int i = 0; i < listStylesName.size(); ++i) {
			Style otherStyle = this.addStyle(listStylesName.get(i), defaultStyle);
			StyleConstants.setBold(otherStyle, stylesIsBoldTable.get(listStylesName.get(i)));
			StyleConstants.setForeground(otherStyle, stylesColorsTable.get(listStylesName.get(i)));
		}
		
		loadingsForColorisation();
		setContentModified(false );
		
		this.addDocumentListener( new DocumentListener(){
			
				public void changedUpdate(DocumentEvent documentEvent){
				}
			      
				public void insertUpdate(DocumentEvent documentEvent){
					handleEvent(documentEvent);
			    }
			    
			    public void removeUpdate(DocumentEvent documentEvent){
			    	handleEvent(documentEvent);
			    }
			    
			    private void handleEvent(DocumentEvent documentEvent){
			        DocumentEvent.EventType type = documentEvent.getType();
			        if (type.equals(DocumentEvent.EventType.INSERT) || type.equals(DocumentEvent.EventType.REMOVE) ) {
			         
			        	int index = getEditor().getTabPane().getSelectedIndex();
			        	if ( ! isContentModified()){
			        		getEditor().getTabPane().setTitleAt( index  , "*" + getEditor().getTabPane().getTitleAt(index ) );
			        	}
			        	setContentModified(true);
			        }  

			   }
		});
		setDocumentFilter( new DocumentFilter(){
			public void insertString(DocumentFilter.FilterBypass fb, int offset, String text, AttributeSet attr) throws BadLocationException {
				boolean isTabOnly = true;
				for(int i=0; isTabOnly && i != text.length(); ++i)
				{
					isTabOnly = isTabOnly && (text.charAt(i)=='\t');
				}
				if(isTabOnly)
				{
					for(int i=0; i!= text.length(); ++i)
					{
						tabifyLine(getDefaultRootElement().getElementIndex(offset));
					}
				}
				else
				{
					fb.insertString(offset, text, attr);
				}
			}
			public void replace(DocumentFilter.FilterBypass fb, int offset, int length,
				String text, AttributeSet attr) throws BadLocationException {
				boolean isTabOnly = true;
				for(int i=0; isTabOnly && i != text.length(); ++i)
				{
					isTabOnly = isTabOnly && (text.charAt(i)=='\t');
				}
				if(isTabOnly)
				{
					for(int i=0; i!= text.length(); ++i)
					{
						tabifyLines(getDefaultRootElement().getElementIndex(offset)
								, getDefaultRootElement().getElementIndex(offset+length));
					}
				}
				else
				{
					fb.replace(offset, length, text, attr);
				}
			}
		});
	}


	
	/**
	 * DOCUMENT COLORISATION START
	 */
	
	public void loadingsForColorisation() {
		// Scilab keywords to be colored
		keywords = getScilabKeywords();    	
		commands = (String[])keywords.get("command");
		functions = (String[])keywords.get("function");
		macros = (String[])keywords.get("macro");

		// Regexp for Scilab keywords  (for commands, functions & macros) 
		for (int i = 0; i < commands.length; i++) {
			commands[i] = "\\b" + commands[i] + "\\b"; 
		}    	
		for (int i = 0; i < functions.length; i++) {
			functions[i] = "\\b" + functions[i] + "\\b"; 
		}    	
		for (int i = 0; i < macros.length; i++) {
			macros[i] = "\\b" + macros[i] + "\\b"; 
		}
	}
	
	
	
	public void colorize(int lineStartPosition, int lineEndPosition) {
	    DEBUG("--> Calling colorize("+lineStartPosition+", "+lineEndPosition+")");	
	    Timer timer = new Timer();
		DEBUG("Colorize [before parse] : " + timer.top());
		singleLine = false;
		
		// We parse all words which are susceptible to be colored
		ArrayList<ArrayList<Integer>> boundaries_list = parse(bools, commands, comments, functions, macros, operators, quotations, lineStartPosition, lineEndPosition);
		DEBUG("Colorize [after parse] : " + timer.top());
		if (!colorizeInprogress) {
			//colorizeInprogress = true;
			this.removeUndoableEditListener(undo);
			this.addUndoableEditListener(null);
			resetStyle(lineStartPosition, lineEndPosition);
			try {
				applyStyle(boundaries_list.get(BOOLS), getStyle("Bool"));
				applyStyle(boundaries_list.get(COMMANDS), getStyle("Command"));
				applyStyle(boundaries_list.get(FUNCTIONS), getStyle("Function"));
				applyStyle(boundaries_list.get(MACROS), getStyle("Macro"));
				applyStyle(boundaries_list.get(OPERATORS), getStyle("Operator"));
				applyStyle(boundaries_list.get(QUOTATIONS), getStyle("String"));
				applyStyle(boundaries_list.get(COMMENTS), getStyle("Comment"));
			} catch (BadLocationException e) {
				e.printStackTrace();
			}
			finally {
				this.addUndoableEditListener(undo);
				//colorizeInprogress = false;
			}
		}
		DEBUG("Colorize [after all applyStyle] : " + timer.top());
	}

	private void resetStyle(int line_start, int line_end) {
	    	DEBUG("resetStyle("+line_start+", "+line_end+")");
		// Reset Color
		this.setCharacterAttributes(line_start, line_end-line_start, this.getStyle("Default"), false);
	}

	
	private void applyStyle(ArrayList<Integer> boundaries, Style style) throws BadLocationException {
		for(int i = 0; i < boundaries.size(); i=i+2)	{
			this.setCharacterAttributes(boundaries.get(i), boundaries.get(i+1)-boundaries.get(i), style, false);
		}
	}
	
	public boolean getColorize() {
		//DEBUG("setColorize("+autoColorize+")");
		return autoColorize;
	}

	public void setColorize(boolean b) {
		//DEBUG("setColorize("+b+")");
		autoColorize = b;
	}
	/**
	 * DOCUMENT COLORISATION END
	 */

	public String getTabulation(){ return tabulation ; }

	/**
	 * DOCUMENT INDENTATION START
	 */
	public void indent(int startPosition, int endPosition) {
		if (!indentInprogress) {
			disableUpdaters();
			indentInprogress = true;
			//resetStyle(startPosition, startPosition);
			try {
				applyIdent_trueone (startPosition, endPosition);
			} catch (BadLocationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//applySelectionIndent();
			
			indentInprogress = false;
			enableUpdaters();
		}
	}

	public String indentLine (String lineToIndent , String baseSpaces  ){
		
		 int lineLength =  lineToIndent.length();
		 Matcher matcherSpace = patternSpace.matcher(lineToIndent);
		 if ( matcherSpace.find()){
			 lineToIndent = lineToIndent.replaceFirst(matcherSpace.group() , "");
		 }
		 
		 
		Matcher matcherComment = patternComment.matcher(lineToIndent);
		String lineWithoutComment = lineToIndent;
		
		if (matcherComment.find()){
			lineWithoutComment = matcherComment.group(1);
		}
		Matcher matcherQuote = patternQuote.matcher(lineWithoutComment);
		
		while (matcherQuote.find()){
			lineWithoutComment = matcherQuote.replaceAll("");
		}

		Matcher matcherIn = patternIn.matcher(lineWithoutComment);
		Matcher matcherInOut = patternInOut.matcher(lineWithoutComment);	
		Matcher matcherOut = patternOut.matcher(lineWithoutComment);
		
		// if it's a middle keyword we remove a tab only for this line and then return to normal indent
		if ( matcherInOut.find(0)){
			currentStringIndent = currentStringIndent.replaceFirst(getTabulation(), "");
			lineToIndent = baseSpaces + currentStringIndent +lineToIndent;
			currentStringIndent += getTabulation();
			
		// if close keyword we remove a tab 
		}else if (matcherOut.find(0)) {
			currentStringIndent = currentStringIndent.replaceFirst(getTabulation(), "");
			lineToIndent = baseSpaces + currentStringIndent +lineToIndent;
		}else {
			lineToIndent = baseSpaces + currentStringIndent +lineToIndent;
		}

		// we make the difference of open/close keywords of the line we've just indent to know
		// how to indent the next one 
		while (matcherIn.find()){
			currentStringIndent += getTabulation();
		}
		while (matcherOut.find()){
			currentStringIndent = currentStringIndent.replaceFirst(getTabulation(), "");
		}
		//System.out.println(lineToIndent);
		//System.out.println(indentLevel);
		
		 //lineToIndent + System.getProperty("line.separator");
		
		
		return lineToIndent;
	}
	
	public void beautifier(int startPosition, int endPosition)throws BadLocationException{
		
		int currentStartOffset = getParagraphElement(startPosition).getStartOffset();
		
		int endOfFirstLine = getParagraphElement(startPosition).getEndOffset();
		/*if (   endOfFirstLine - currentStartOffset <= 1){// prevented intending regions starting with an empty line.
			return;
		}*/
		String firtstLine =  getText(currentStartOffset, endOfFirstLine-currentStartOffset-1);
		Matcher matcherSpace = patternSpace.matcher(firtstLine);
		// set the reference for indent
		String baseSpaces = matcherSpace.find() ? matcherSpace.group() :"";
		Element currentElement = null, lastElement = getParagraphElement(endPosition);
		
		do{
			currentElement = getParagraphElement(currentStartOffset);
			String toIndent= getText(currentStartOffset, currentElement.getEndOffset()- currentStartOffset-1);//-1 to remove \n			
			String indented = indentLine(toIndent, baseSpaces);
			replace(currentStartOffset, toIndent.length(), indented, null);
			currentStartOffset+= indented.length()+1;
		}while(currentElement!=lastElement);
		currentStringIndent = "";
		currentLevelIdent = 0;
	}
	


	public synchronized void applyIdent_trueone (int startPosition, int endPosition)throws BadLocationException{
		
		//System.out.println(startPosition);
		//System.out.println(endPosition);
		String previousSpace = "";
		String currentSpace ="";
		String previousLineContent = "";
		
		int finalPosition = getEditor().getTextPane().getText().length();

		
		int currentLineStart = getParagraphElement(startPosition).getStartOffset();
		int currentLineLength =  getParagraphElement(startPosition).getEndOffset()-currentLineStart-1;
		String currentLineContent = this.getText(currentLineStart, currentLineLength);
		
		/* compute number of space characters in the previous line */
		if (startPosition > 1){
			int previousLineStart = getParagraphElement(startPosition-currentLineLength -1).getStartOffset();
			int previousLineLength =  getParagraphElement(startPosition-currentLineLength - 1).getEndOffset()-previousLineStart-1;
			
			previousLineContent = this.getText(previousLineStart, previousLineLength);
			
			Pattern patternSpace = Pattern.compile("\\s*"); 
			Matcher matcherSpace = patternSpace.matcher(previousLineContent);
			
			if ( matcherSpace.find()){
				previousSpace =  matcherSpace.group();
			}
		}
		/* compute number of space characters in the current  line*/
		Pattern patternSpace = Pattern.compile("\\s*");
		Matcher matcherSpace = patternSpace.matcher(currentLineContent);
		
		if ( matcherSpace.find()){
			currentSpace = matcherSpace.group();
			
		}

		/*regexp to find open / middle / close keywords */
		String nextLineContent = null;
		{
			int start = getParagraphElement(endPosition).getStartOffset();
			int length = getParagraphElement(endPosition).getEndOffset() - start;
			nextLineContent = this.getText(start, length-1);
		}
		Pattern patternIn = Pattern.compile("(\\b(if|while|for|select|function)\\b)"); // do should change nothing to indent	
		Pattern patternInOut = Pattern.compile("\\b(else|elseif|case)\\b");
		Pattern patternOut = Pattern.compile("\\b(end|endfunction)\\b");
		Pattern patternComment = Pattern.compile("(.*?)//");
		
		Matcher matcherComment = patternComment.matcher(currentLineContent);
		String lineWithoutComment = currentLineContent;
		
		// we will search for keywords only on the uncomment part
		if (matcherComment.find()){
			lineWithoutComment = matcherComment.group(1);
		}
		
		Matcher matcherIn = patternIn.matcher(lineWithoutComment);
		Matcher matcherInOut = patternInOut.matcher(lineWithoutComment);	
		Matcher matcherOut = patternOut.matcher(lineWithoutComment);
			
		String tabToAdd = "";
		String tabToRemove = "";


		/*apply change */
			
			// i =  number_of_open_keyword -  number_of_close_keyword
			int i = 0;
			
			while (matcherIn.find()){
				i++;
			}
			while (matcherOut.find()){
				i--;
			}
		/*update current line*/
			/* strcuture keyword which are supposed to open a structure (if, function ...  ) */
		if ( matcherIn.find(0)){
			currentLevelIdent ++;

			/* strcuture keyword which are supposed to be inside a structure (else / elseif  ) */
		}else if (matcherInOut.find()) { 
			
			Matcher matcherPrevIn = patternIn.matcher(previousLineContent);
			
			if (!matcherPrevIn.find(0)){
			
				this.replace(currentLineStart, currentSpace.length(),previousSpace.replaceFirst(getTabulation(), "") , null);
				endPosition -= (currentSpace.length() - previousSpace.replaceFirst(getTabulation(), "").length() );
				currentSpace += getTabulation();
			}else{
				if ( currentSpace.replaceFirst(getTabulation(), "").length() ==   previousSpace.length())  {
					this.replace(currentLineStart, currentSpace.length(),previousSpace , null);
					endPosition -= (currentSpace.length() - previousSpace.length() );
					
				}
					previousSpace += getTabulation();
			}
			
			
			/* strcuture keyword which are supposed to close a structure (end, endfunction ...  ) */
		}else if (  matcherOut.find(0) ){
			this.replace(currentLineStart, currentSpace.length(),previousSpace.replaceFirst(getTabulation(), "") , null);
			endPosition -= (currentSpace.length() - previousSpace.replaceFirst(getTabulation(), "").length() );
			currentLevelIdent --;
			
		}else {

			//this.insertString(endPosition, currentSpace , null);
		}
		

		
		// update next line
		// more open keywords than close ones
		if ( i > 0 ){
			for (int j =0; j < i; j++){
				tabToAdd += getTabulation();
			}
			this.insertString(endPosition, tabToAdd + currentSpace , null);
		// less open keywords than close ones	
		}else if (i < 0 ) {
			for (int j =  0; j > i; j--){
				previousSpace =  previousSpace.replaceFirst(getTabulation(), "");
			}
			this.insertString(endPosition, previousSpace , null);
		// no open/close keyword
		}else {
			if (!matcherInOut.find(0)){
				this.insertString(endPosition, currentSpace , null);
			}else{
				this.insertString(endPosition, previousSpace , null);
			}
		
		}
				
	
		
	}
	
	
	
/*	
	
	public void applyIndent(int start, int end, String previous_tab) throws BadLocationException {
	    DEBUG("applyIndent("+start+", "+end+", {"+previous_tab+"})");
		String indentedText = "";
		String text_to_indent = ""; // the entire document
		ArrayList<String> command_list = new ArrayList<String>(); // list of commands in the document

		try {
			// Get the document text to indent
			text_to_indent = this.getText(start, end-start);
			DEBUG("Indent text : {"+text_to_indent+"}");
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
		


		// Get all line break positions &
		// each line of the document
		ArrayList<Integer> line_break = new ArrayList<Integer>(); // positions of line break
		ArrayList<String> all_lines = new ArrayList<String>(); // the document line by line
		String line = "";
		for (int i = 0; i < text_to_indent.length(); i++) {
			line = line.concat(text_to_indent.charAt(i)+"");

			if (text_to_indent.charAt(i)=='\n') {
				line_break.add(i);
				all_lines.add(line);
				line = "";
			}
			if (i==text_to_indent.length()-1) {
				all_lines.add(line);
				line = "";
			}
		}

		ArrayList<String> all_lines_without_spaces = new ArrayList<String>(); // the document line by line
		// without spaces
		for (int i = 0; i < all_lines.size(); i++) {
			String no_space_line = removeFirstSpaces(all_lines.get(i));
			all_lines_without_spaces.add(no_space_line);
		}

		boolean got_select = false;

		for (int i = 0; i < all_lines_without_spaces.size(); i++) {
			
			// Get commands for each lines
			command_list = getOperatorList(all_lines_without_spaces.get(i));

			// Check if in one line all operators are matching,
			// so we can know if the next line needs indentation or not
			// ex: if %T then function foo(1) endfunction end => doesn't need indentation
			// Warning: ArrayList_match looks like [IN, if, IN function, OUT, endfunction, OUT, end]
			ArrayList<String> vector_match = matchingOperators(command_list);


			indentedText += previous_tab;

			// Here start the indentation process
			if (vector_match.size() > 0) {

				// If we have 'IN' command
				if (vector_match.get(0).equals(IN)) {
					
					if ((vector_match.get(1).toLowerCase().equals("else")) || 
							(vector_match.get(1).toLowerCase().equals("elseif"))) {
						
						if (autoIndent == true && all_lines_without_spaces.size() == 2) {
							indentedText = previous_tab + all_lines_without_spaces.get(i);
							if (indentedText.length() >= 2) {
								String text_tab = indentedText.substring(0, 2);
								if (text_tab.equals("  ")) {
									indentedText = indentedText.substring(2, indentedText.length());
								}
							}
							indentedText += previous_tab;
							break;
						}
						
					} else if (vector_match.get(1).toLowerCase().equals("select")) {
						
						got_select = true;
						
						if (autoIndent == true && all_lines_without_spaces.size() == 2) {
							indentedText = previous_tab + all_lines_without_spaces.get(i);
							indentedText += previous_tab + TABULATION;
							break;
						}

						// If we have 'case' command, case needs 'select' to be correct
					} else if ((vector_match.get(1).toLowerCase().equals("case"))) {
						
						if (autoIndent == true && all_lines_without_spaces.size() == 2) {
							
							String tmp_cas_tab = "";
							tmp_cas_tab = getLineTabulations(indentedText);
							
							if (tmp_cas_tab.length() == 2) {
								System.out.println("222222222222222");
							} else {
								System.out.println(tmp_cas_tab.length());
							}
							
							
							
							if (previous_tab.length() == 2) {
								indentedText += all_lines_without_spaces.get(i);
								indentedText += previous_tab + TABULATION;
							} else if (previous_tab.length() >= 2) {
								String tmp_tab = previous_tab.substring(0, 2);
								if (tmp_tab.equals("  ")) {
									indentedText = previous_tab.substring(2, previous_tab.length());
									indentedText += all_lines_without_spaces.get(i);
									indentedText += previous_tab.substring(2, previous_tab.length()) + TABULATION;
								}
							}
							
//							String case_tab = "";
//							if (previous_tab.length() > 2) {
//								case_tab = TABULATION;
//							} else {
//								case_tab = "";
//							}
//							
//							indentedText += case_tab + all_lines_without_spaces.get(i);
//							indentedText += previous_tab + TABULATION;
							break;
						}
						
					} else {
						
						if (autoIndent == true && all_lines_without_spaces.size() == 2) {
							indentedText = previous_tab + all_lines_without_spaces.get(i);
							indentedText += previous_tab + TABULATION;
							break;
						}
					}

					// If we have "OUT' operator
				} else if (vector_match.get(0).equals(OUT)) {
					
					if (autoIndent == true && all_lines_without_spaces.size() == 2) {
						
						if (previous_tab.length() >= 2 && got_select == false) {
							String tmp_tab = previous_tab.substring(0, 2);
							if (tmp_tab.equals("  ")) {
								indentedText = previous_tab.substring(2, previous_tab.length());
								indentedText += all_lines_without_spaces.get(i);
								indentedText += previous_tab.substring(2, previous_tab.length());
							}
						}
						break;
					}
				} 
			} else {
				
				if (autoIndent == true && all_lines_without_spaces.size() == 2) {
					indentedText = previous_tab + all_lines_without_spaces.get(i);
					indentedText += previous_tab;
					break;
				}
			}
			vector_match.clear();

		} // end for

		// Display the indentation if there is one
		// update coloration
		if (indentedText.compareTo(text_to_indent) != 0) {
		    this.replace(start, end-start, indentedText, null);
		    colorize(start, start + indentedText.length());
		}
		else {
		    DEBUG("Text is equal !!!!");
		}

	}
	
	


	
	public void applySelectionIndent() {
		DEBUG("applySelectionIndent");
		editor = getEditor();
		String tab = "";
		int selection_start = 0;
		int selection_end = 0;
		int level_one = 0;
		
		// Get start & end offsets of the selected text
		selection_start = editor.getTextPane().getSelectionStart();
		selection_end = editor.getTextPane().getSelectionEnd();
		
		
		// Get start offsets of the first selected line & end offsets of the last selected line
		lineStartPosition =  this.getParagraphElement(selection_start).getStartOffset();
		lineEndPosition = this.getParagraphElement(selection_end).getEndOffset()-1;
		
//		System.out.println(lineStartPosition);
//		System.out.println(lineEndPosition);
		
		
		
		int previous_line_start = 0;
		int previous_line_end = 0;
		String previous_line;
		
		int current_line_start = 0;
		int current_line_end = 0;
		
		if (lineStartPosition == 0) {
			tab = "";
		} else {
			// Get previous line
			previous_line_start =  this.getParagraphElement(lineStartPosition-1).getStartOffset();
			previous_line_end = this.getParagraphElement(lineStartPosition-1).getEndOffset()-1;
			previous_line = "";
			
			current_line_start = this.getParagraphElement(selection_start).getStartOffset();
			current_line_end = this.getParagraphElement(selection_end).getEndOffset()-1;
			
			try {
				previous_line = this.getText(previous_line_start, previous_line_end-previous_line_start);
				System.out.println("previous_line = "+previous_line);
				
				// Get previous line's tabulation
				tab = getLineTabulations(previous_line);
				// Check if we have commands and if they match in the previous line
				ArrayList<String> previous_command_list = getOperatorList(previous_line);
				ArrayList<String> previous_vector_match = matchingOperators(previous_command_list);
				
				if (!autoIndent) {

					if (previous_vector_match.size() > 0) {
						if (previous_vector_match.get(0).equals(IN)) {
							level_one = 1;
						} 
					} else {
						level_one = 0;
					}
				}
				
			} catch (BadLocationException e) {
				e.printStackTrace();
			}
		}
				
		try {
		    lineStartPosition = previous_line_start;
		    lineEndPosition = current_line_start;

		    System.out.println("-------> "+lineStartPosition);
		    System.out.println("-------> "+lineEndPosition);
		    System.out.println("TAB = {"+tab+"}");

		    applyIndent(lineStartPosition, lineEndPosition, tab);
		    tab = "";

		} catch (BadLocationException e) {
			e.printStackTrace();
		}

	}
	
	public String getLineTabulations(String line) {
	    int j = 0;
	    String spaces = "";
	    DEBUG("getLineTabulations("+line+")");
	    DEBUG("getLineTabulations("+line.length()+")");
	    while (j < line.length() && ((line.charAt(j) == '\t') || (line.charAt(j) == ' '))) {
		spaces = spaces + line.charAt(j);
		++j;
	    }
	    return spaces;
	}
	*/
	/*
	 * Remove all spaces or tabulations at the begining a string
	 * This function is used for the indentation only
	 * ex: '   hello world' will be tranformed into 'hello world'
	 */
	/*
	public String removeFirstSpaces(String line) {
		int spaces = 0;

		for (int j = 0; j < line.length(); j++) {
			if ((line.charAt(j) == '\t') || (line.charAt(j) == ' ')) {
				spaces++;
			} else {
				break;
			}				
		}			
		line = line.substring(spaces, line.length());	

		return line;
	}
*/
	/*
	 * Get all commands given in the string
	 * This function is used for the indentation only
	 */
	/*
	public ArrayList<String> getOperatorList(String text) {
		ArrayList<String> operator_list = new ArrayList<String>();
		ArrayList<String> op = new ArrayList<String>();

		ArrayList<Integer> comments_boundaries = new ArrayList<Integer>();
		ArrayList<Integer> quotations_boundaries = new ArrayList<Integer>();
		ArrayList<Integer> tmp_comm_and_quot = new ArrayList<Integer>();

		ArrayList<Integer> commands_boundaries = new ArrayList<Integer>();

		String[] commands_in = {"if", "else", "elseif", "while", "for", "do", "select", "case", "function"};
		String[] commands_out = {"end", "endfunction"};

		String[] allCommands = {"if", "else", "elseif", "while", "for", "do", "select", "case", "function", "end", "endfunction"};

		// Regexp for Scilab commands
		for (int i = 0; i < allCommands.length; i++) {
			allCommands[i] = "\\b" + allCommands[i] + "\\b";
		}

		// Find command boundaries in the given text
		commands_boundaries = findBoundaries(allCommands, 0, text);
		comments_boundaries = findBoundaries(comments, 0, text);
		quotations_boundaries = findBoundaries(quotations, 0, text);
		
		// Remove comments which are into quotations
		comments_boundaries = startNotIn(comments_boundaries, quotations_boundaries);
		// Remove quotations which are into comments
		quotations_boundaries = startNotIn(quotations_boundaries, comments_boundaries);

		// Union of comments & quotations to remove keywords
		tmp_comm_and_quot.addAll(comments_boundaries);
		tmp_comm_and_quot.addAll(quotations_boundaries);

		// Remove commads which are into quotations or comments
		commands_boundaries = startNotIn(commands_boundaries, tmp_comm_and_quot);

		// Sort commands_boudaries
		Collections.sort(commands_boundaries);
		
		// The function applyIndent needs a vector in this format, ex: IN,IF,OUT,END
		for (int i = 0; i < commands_boundaries.size(); i=i+2) {
			op.add(text.substring(commands_boundaries.get(i), commands_boundaries.get(i+1)));
		}
		
		for (int i = 0; i < op.size(); i++) {
			for (int j = 0; j < commands_in.length; j++) {
				if (op.get(i).toLowerCase().equals(commands_in[j])) {
					operator_list.add(IN);
					operator_list.add(op.get(i));
				}
			}
			for (int j = 0; j < commands_out.length; j++) {
				if (op.get(i).toLowerCase().equals(commands_out[j])) {
					operator_list.add(OUT);
					operator_list.add(op.get(i));
				}
			}
		}
		
		return operator_list;
	}
*/
	/*
	 * Check if in one line all commands are matching,
	 * by this way we can know if the next line needs indentation or not
	 * ex: if %T then function foo(1) endfunction end => doesn't need indentation
	 * ex: if %T then foo(1) end for => need indentation
	 * This function is used for the indentation only
	 * WARNING: command_list looks like [IN, if, IN function, OUT, endfunction, OUT, end]
	 */
	
	/*
	public ArrayList<String> matchingOperators(ArrayList<String> command_list) {

		int tmp_size = command_list.size();

		for (int i = 0; i < command_list.size() - 3; i=i+2) {
			if ((command_list.get(i+1).toLowerCase().equals("function") && 
					command_list.get(i+3).toLowerCase().equals("endfunction"))
					||
					(command_list.get(i+3).toLowerCase().equals("end") && 
							!(command_list.get(i+1).toLowerCase().equals("function")) &&
							!(command_list.get(i+1).toLowerCase().equals("endfunction")) && 
							!(command_list.get(i+1).toLowerCase().equals("end")) )) {
				command_list.remove(i+3);					
				command_list.remove(i+2);
				command_list.remove(i+1);
				command_list.remove(i);
				i = i -2;
			} 
		}
		if (tmp_size == command_list.size()) {
			return command_list;
		}
		return matchingOperators(command_list);
	}*/
	/**
	 * DOCUMENT INDENTATION END
	 */


	/**
	 * DOCUMENT COMMENT ACTION
	 */
	
	/*
	 * Comment the current line when no selection has been done
	 */
	
	public int commentLine(int line)
	{
		// No selection : comment the current line
		
		String comment_str = "//";
		int offset         = comment_str.length();
		int start          = this.getDefaultRootElement().getElement(line).getStartOffset();
		
		try
		{
			// Replacement
			this.replace(start, 0, comment_str, null);
		}
		catch (BadLocationException e){
			e.printStackTrace();
		}
		
		return offset;
	}
	
	/*
	 * Comment several lines
	 */
	
	public void commentLines(int line_start, int line_end)
	{
		try
		{
			String comment_str = "//";
			int start          = this.getDefaultRootElement().getElement(line_start).getStartOffset();
			int end            = this.getDefaultRootElement().getElement(line_end).getEndOffset();
			Pattern pattern    = Pattern.compile("^",Pattern.MULTILINE);
			Matcher matcher    = pattern.matcher(this.getText(start,end-start));
			this.replace(start,end-start,matcher.replaceAll(comment_str), null);	
		}
		catch (BadLocationException e){
			e.printStackTrace();
		}
	}
	
	/*
	 * Comment a part of a line
	 */
	
	public int commentText(int position_start)
	{
		String comment_str = "//";
		int offset         = comment_str.length();
		
		try
		{
			// Replacement
			this.replace(position_start, 0, comment_str, null);
		}
		catch (BadLocationException e)
		{
			e.printStackTrace();
		}
		
		return offset;
	}
	
	/**
	 * DOCUMENT COMMENT ACTION END
	 */

	/**
	 * DOCUMENT UN-COMMENT ACTION
	 */
	
	public int uncommentLine(int line)
	{
		int start   = this.getDefaultRootElement().getElement(line).getStartOffset();
		int end     = this.getDefaultRootElement().getElement(line).getEndOffset();			
		int offset  = 0;
		
		try
		{
			String text     = this.getText(start, end-start);
			Pattern pattern = Pattern.compile("^(\\s)*//");
			Matcher matcher = pattern.matcher(text);
			
			if(matcher.find())
			{
				this.replace(start+matcher.end()-2, 2, "", null);
				offset = 2;
			}
		}
		catch (BadLocationException e){
			e.printStackTrace();
		}
		
		return offset;
	}
	
	/*
	 * Un-Comment several lines
	 */
	
	public void uncommentLines(int line_start, int line_end)
	{
		Pattern pattern = Pattern.compile("^(\\s)*//");
		
		for (int i = line_start; i <= line_end; i++)
		{
			int start   = this.getDefaultRootElement().getElement(i).getStartOffset();
			int end     = this.getDefaultRootElement().getElement(i).getEndOffset();			
			
			try
			{
				// Get the text line
				String text     = this.getText(start, end-start);
				Matcher matcher = pattern.matcher(text);
				
				if(matcher.find())
				{
					this.replace(start+matcher.end()-2, 2, "", null);
				}
			}
			catch (BadLocationException e){
				e.printStackTrace();
			}
		}
	}
	
	/*
	 * Un-Comment a part of a line
	 */
	
	public int uncommentText(int position_start, int position_end)
	{
		Pattern pattern = Pattern.compile("^//");
		int offset      = 0;
		
		try
		{
			// Get the text line
			String text     = this.getText(position_start,position_end-position_start);
			Matcher matcher = pattern.matcher(text);
			
			if(matcher.find())
			{
				this.replace(position_start,2,"", null);
				offset = 2;
			}
		}
		catch (BadLocationException e)
		{
			e.printStackTrace();
		}
		
		return offset;
	}
	
	/**
	 * DOCUMENT UN-COMMENT ACTION END
	 */
	
	
	/**
	 * DOCUMENT TABIFY ACTION
	 */
	
	/*
	 * Insert a tab just after the caret position
	 */
	
	public synchronized void insertTab(int position)
	{
		try
		{
			this.replace(position, 0, getTabulation(), null);
		}
		catch (BadLocationException e)
		{
			e.printStackTrace();
		}
	}
	
	/*
	 * Tabify a line
	 */
	
	public synchronized int tabifyLine(int line)
	{
		insertTab(getDefaultRootElement().getElement(line).getStartOffset());	
		return getTabulation().length();
	}
	
	/*
	 * Tabify several lines
	 */
	
	public synchronized int tabifyLines(int line_start, int line_end)
	{
		boolean indentMode= getAutoIndent();
		setAutoIndent(false);
		for(int currentLine = line_start; currentLine <= line_end; ++currentLine){ // tabifying should not insert/remove lines
			tabifyLine(currentLine);
		}
		setAutoIndent(indentMode);
		return getTabulation().length();
	}
		
	/**
	 * DOCUMENT UNTABIFY ACTION
	 */
	
	/*
	 * Delete a tab just before the caret position
	 */
	
	public synchronized int deleteTab(int position)
	{
		String tab = getTabulation(); 
		int tabLength = tab.length();
		try{
			String nextChars = getText(position, tabLength);
			if(nextChars.equals(tab)){
				remove(position, tabLength);
			}
		}
		catch (BadLocationException e)
		{
			e.printStackTrace();
		}
		
		return tabLength;
	}
	
	/*
	 * Delete a tab at the beginning of the line "line"
	 */
	
	public synchronized int untabifyLine(int line)
	{
		deleteTab(getDefaultRootElement().getElement(line).getStartOffset());
		return getTabulation().length();
	}
	
	/*
	 * Delete tabs at the beginning of several lines
	 */
	public boolean canUntabifyLines(int line_start, int line_end)
	{
		boolean result = true;
		String tab = getTabulation();
		int tabLength = tab.length();
		try {
			for (int i = line_start; result && (i <= line_end); i++)
			{
				result = result && (tab.equals(getText(getDefaultRootElement().getElement(i).getStartOffset(), tabLength)));
			}
		}
		catch( javax.swing.text.BadLocationException e){
			System.err.println("untabifying lines "+line_start+" to "+line_end+" "+e);
		}
		return result;
	}
	
	public synchronized int untabifyLines(int line_start, int line_end)
	{	
		int res=0;
		if(canUntabifyLines(line_start, line_end))
		{
			boolean indentMode= getAutoIndent();
			setAutoIndent(false);
			for (int i = line_start; i <= line_end; i++)
			{
				untabifyLine(i);
			}
			setAutoIndent(indentMode);
			res= getTabulation().length();
		}
		return res;
	}
	
	/**
	 * FIND AND REPLACE START
	 */
	public ArrayList<Integer[]> findWord(String word, boolean caseSensitive , boolean wholeWord , boolean useRegexp ) {
		String fullText = getFullDocument(); 
		int lastIndex = 0;
		int wordSize = word.length();
		ArrayList<Integer[]> offsetList = new ArrayList<Integer[]>();

		//If we don't give any word to find
		if ( (word != null) && !(word.equals("")) ) {
			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b";
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}


			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord){
				word = "(?m)" + word;
				Pattern pattern = Pattern.compile(word);
				Matcher matcher = pattern.matcher(fullText);

				while (matcher.find()) {
					offsetList.add(new Integer[] {matcher.start() ,matcher.end()});
				}
				// ... for other case
			}else {
				while ((lastIndex = fullText.indexOf(word, lastIndex)) != -1) {
					int endIndex = lastIndex + wordSize;
					offsetList.add(new Integer[] {lastIndex,endIndex} );
					lastIndex = endIndex;
				}
			}
		}
		return offsetList;
	}
	
	public ArrayList<Integer[]> findWord(String word,int currentSelectStart ,int currentSelectEnd, boolean caseSensitive , boolean wholeWord , boolean useRegexp ) {
		String fullText = getSelectedDocumentLines(currentSelectStart, currentSelectEnd);
		
		int offset = this.getParagraphElement(currentSelectStart).getStartOffset();
		int lastIndex = 0;
		int wordSize = word.length();;
		ArrayList<Integer[]> offsetList = new ArrayList<Integer[]>();

		//If we don't give any word to find
		if ( (word != null) && !(word.equals("")) ) {
			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b";
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		


			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord){
				word = "(?m)" + word;
				Pattern pattern = Pattern.compile(word);
				Matcher matcher = pattern.matcher(fullText);

				while (matcher.find()) {
					offsetList.add(new Integer[] {matcher.start()+offset ,matcher.end()+offset});
				}
				// ... for other case
			}else {
				while ((lastIndex = fullText.indexOf(word, lastIndex)) != -1) {
					int endIndex = lastIndex + wordSize;
					offsetList.add(new Integer[] {lastIndex+offset ,endIndex+offset } );
					lastIndex = endIndex;
				}
			}
		}
		return offsetList;
	}

	/**
	 * Get the next expression matching the search after the caret current position
	 * @param word , the word or regexp to find
	 * @param currentPos, the position where the search start
	 * @param caseSensitive , whether the search is sensitive or not to case
	 * @param wholeWord  , whether the search will only look to separate word or not
	 * @param useRegexp  , whether the string to search should be interpreted as a regexp or not
	 */
	public int[] findNextWord (String word ,int currentPos, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		String fullText = getFullDocument();
		int index = -1;
		int end = -1;



		if ( (word != null) && (!word.equals(""))  ) {
			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b";
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}
			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord){
				word = "(?m)" + word;
				Pattern pattern = Pattern.compile(word);
				Matcher matcher = pattern.matcher(fullText.substring(currentPos));

				if (matcher.find()) {
					index = matcher.start()+currentPos;
					end = matcher.end()+currentPos;
				}else{
					index = -1;
					end  = -1;
				}

				// ... for other case
			}else {
				index = fullText.indexOf(word,currentPos);
				end = index + word.length();
			}
		}

			return new int [] {index , end };
	}

	public int[] findNextWord (String word ,int currentPos,int currentSelectStart ,int currentSelectEnd, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		
		String fullText = getSelectedDocumentLines(currentSelectStart, currentSelectEnd);
		int offset = this.getParagraphElement(currentSelectStart).getStartOffset();
		System.out.println(currentPos);
		currentPos -=  offset;
		
		int index = -1;
		int end = -1;


		if ( (word != null) && (!word.equals(""))  ) {
			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b";
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}

			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord){
				word = "(?m)" + word;
				Pattern pattern = Pattern.compile(word);
				Matcher matcher = pattern.matcher(fullText.substring(currentPos));

				if (matcher.find()) {
					index = matcher.start()+currentPos+offset;
					end = matcher.end()+currentPos+offset;
				}else{
					index = -1;
					end  = -1;
				}

				// ... for other case
			}else {
			
				index = fullText.indexOf(word,currentPos);
				if (index != -1) index += offset;
				end = index + word.length();
			}
		}

			return new int [] {index , end };
	}
	
	
	
	/**
	 * Get the previous expression matching the search before the caret current position
	 * @param word , the word or regexp to find
	 * @param currentPos, the position where the search start
	 * @param caseSensitive , whether the search is sensitive or not to case
	 * @param wholeWord  , whether the search will only look to separate word or not
	 * @param useRegexp  , whether the string to search should be interpreted as a regexp or not
	 */
	public int[] findPreviousWord (String word , int currentPos, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		String fullText = getFullDocument();
		int index = -1;
		int end = -1;
		Pattern pattern;

		if ( (word != null) && (!word.equals(""))  ) {

			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b";
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}

			//We find matching words ...
			// ... for regexp or whole words

			if (useRegexp || wholeWord){
				word = "(?m)" + word;
				 pattern = Pattern.compile(word);
			}else{// ... for other case
				// we use regexp in both case cause of a nasty bug when you have string like 
				//121212  and you search "121" forward then backward
				word = "(?m)" + word;
				pattern = Pattern.compile(word , Pattern.LITERAL );
				
			}
				Matcher matcher = pattern.matcher(fullText.substring(0,currentPos));

				boolean found = false;
				while (matcher.find()) {
					index = matcher.start();
					end = matcher.end();
					found = true;
				}

				if(!found){
					index = -1;
					end = -1;
				}
		}


		/*if nothing index and end will both be equal to -1*/
		return new int [] {index , end };


	}
	
	
	public int[] findPreviousWord (String word , int currentPos,int currentSelectStart ,int currentSelectEnd, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		String fullText = getSelectedDocumentLines(currentSelectStart, currentSelectEnd);
		int offset = this.getParagraphElement(currentSelectStart).getStartOffset();
		System.out.println(currentPos);
		currentPos -=  offset;
		int index = -1;
		int end = -1;
		Pattern pattern;

		if ( (word != null) && (!word.equals(""))  ) {

			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b";
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		
			word = "(?m)" + word;

			//We find matching words ...
			// ... for regexp or whole words

			if (useRegexp || wholeWord){
				 pattern = Pattern.compile(word);
			}else{// ... for other case
				// we use regexp in both case cause of a nasty bug when you have string like 
				//121212  and you search "121" forward then backward
				pattern = Pattern.compile(word , Pattern.LITERAL );
				
			}
				System.out.println(currentPos);
				Matcher matcher = pattern.matcher(fullText.substring(0,currentPos));

				boolean found = false;
				while (matcher.find()) {
					index = matcher.start() + offset;
					end = matcher.end() + offset;
					found = true;
				}

				if(!found){
					index = -1;
					end = -1;
				}
		}


		/*if nothing index and end will both be equal to -1*/
		return new int [] {index , end };


	}
	
	
	/**
	 * FIND AND REPLACE END
	 */


	/**
	 * UTILITARIAN FUNCTIONS START
	 */
	/*
	 * Get all Scilab's keywords into a hashtable
	 */
	public Hashtable<String, String[]> getScilabKeywords() {
		//Get all Scilab keywords with SWIG
		String[] commands =  ScilabKeywords.GetCommandsName();
		String[] functions =  ScilabKeywords.GetFunctionsName();
		String[] macros =  ScilabKeywords.GetMacrosName();
		//String[] variables =  ScilabKeywords.GetVariablesName();

		Hashtable<String, String[]> keywords = new Hashtable<String, String[]>();

		for (int i = 0; i < macros.length; i++) {
			keywords.put("macro", macros);
		}
		for (int i = 0; i < commands.length; i++) {
			keywords.put("command", commands);
		}
		for (int i = 0; i < functions.length; i++) {
			keywords.put("function", functions);
		}
		return keywords;
	}

	/*
	 * Parse all Scilab keywords
	 * This function is used for the syntactic colorization
	 */
	private ArrayList<ArrayList<Integer>> parse(String[] bools, String[] commands, String[] comments, 
			String[] functions, String[] macros, String[] operators, String[] quotations, int start, int end) {

	    	Timer timer = new Timer();
	    	ArrayList<ArrayList<Integer>>  boundaries_list = null;
	    	ArrayList<Integer> boolsBoundaries, commandsBoundaries, 
		commentsBoundaries, functionsBoundaries, 
		macrosBoundaries, operatorsBoundaries, 
		quotationsBoundaries;
		try {
		boolsBoundaries = findBoundaries(bools, start, this.getText(start, end - start));
		commandsBoundaries = findBoundaries(commands, start, this.getText(start, end - start));
		commentsBoundaries = findBoundaries(comments, start, this.getText(start, end - start));
		functionsBoundaries = findBoundaries(functions, start, this.getText(start, end - start));
		macrosBoundaries = findBoundaries(macros, start, this.getText(start, end - start));
		//macrosBoundaries = new ArrayList<Integer>();
		operatorsBoundaries = findBoundaries(operators, start, this.getText(start, end - start));
		quotationsBoundaries = findBoundaries(quotations, start, this.getText(start, end - start));	

		DEBUG("parse -> findAllBoundaries "+timer.top());
		boundaries_list = organizeBoundaries(boolsBoundaries, commandsBoundaries, commentsBoundaries, functionsBoundaries, 
				macrosBoundaries, operatorsBoundaries, quotationsBoundaries);
		DEBUG("parse -> organizeBoundaries "+timer.top());
		}
		catch (Exception e) {
		    e.printStackTrace();
		}
		finally {
		    return boundaries_list;
		}
	}

	/*
	 * Get start & end position for each keywords
	 * String[] keyword is a type of keywords(ex: operators, commands, macros..)
	 * String text is where we make the search, 
	 * if text is null we apply the research to the entire document
	 */
	private ArrayList<Integer> findBoundaries(String[] keyword, int start, String text) {
	    Timer timer = new Timer();
	    DEBUG("   findBoundaries start : " + timer.top());
	    
		Pattern pattern;
		ArrayList<Integer> bound = new ArrayList<Integer>();
		
		for(int i = 0; i < keyword.length; i++)	{
			pattern = Pattern.compile(keyword[i], Pattern.DOTALL);
			    Matcher matcher = pattern.matcher(text);
			    while(matcher.find()){
				//System.err.println("Match Found : "+(matcher.start())+","+(matcher.end()/*-matcher.start()*/));
				bound.add(new Integer(matcher.start() + start));	
				bound.add(new Integer(matcher.end() + start));
			    }
		}
		
		DEBUG("   findBoundaries end : " + timer.top());
		return bound;
	}

	/*
	 * When we have all boundaries for each type of keywords
	 * we filter 'bad' boundaries
	 * ex: if we have quotations into comments (and the opposite), keywords into quotations or comments 
	 */
	private ArrayList<ArrayList<Integer>> organizeBoundaries(ArrayList<Integer> boolsBoundaries, ArrayList<Integer> commandsBoundaries,
			ArrayList<Integer> commentsBoundaries, ArrayList<Integer> functionsBoundaries, 
			ArrayList<Integer> macrosBoundaries, ArrayList<Integer> operatorsBoundaries, 
			ArrayList<Integer> quotationsBoundaries) {

		ArrayList<Integer> tmp_comm_and_quot = new ArrayList<Integer>();
		ArrayList<ArrayList<Integer>> vector_list = new ArrayList<ArrayList<Integer>>();

		// Remove comments which are into quotations
		commentsBoundaries = startNotIn(commentsBoundaries, quotationsBoundaries);
		// Remove quotations which are into comments
		quotationsBoundaries = startNotIn(quotationsBoundaries, commentsBoundaries);

		// Union of comments & quotations to remove keywords
		tmp_comm_and_quot.addAll(commentsBoundaries);
		tmp_comm_and_quot.addAll(quotationsBoundaries);

		// Remove keywords which are into comments & quotations
		boolsBoundaries = strictlyNotIn(boolsBoundaries, tmp_comm_and_quot);
		commandsBoundaries = strictlyNotIn(commandsBoundaries, tmp_comm_and_quot);
		functionsBoundaries = strictlyNotIn(functionsBoundaries, tmp_comm_and_quot);
		macrosBoundaries = strictlyNotIn(macrosBoundaries, tmp_comm_and_quot);
		operatorsBoundaries = strictlyNotIn(operatorsBoundaries, tmp_comm_and_quot);

		vector_list.add(boolsBoundaries);
		vector_list.add(commandsBoundaries);
		vector_list.add(commentsBoundaries);
		vector_list.add(functionsBoundaries);
		vector_list.add(macrosBoundaries);
		vector_list.add(operatorsBoundaries);
		vector_list.add(quotationsBoundaries);

		return vector_list;
	}

	private ArrayList<Integer> strictlyNotIn(ArrayList<Integer> v1, ArrayList<Integer> v2) {
		int v1_start = 0;
		int v1_end = 0;
		int v2_start = 0;
		int v2_end = 0;

		ArrayList<Integer> vector_strictlyNotIn = new ArrayList<Integer>();

		// Remove interval from v1 which are include in interval of v2
		for(int i=0; i < v1.size(); i=i+2) {
			boolean dropMe = false;
			v1_start = v1.get(i);
			v1_end = v1.get(i+1);

			for(int j=0; j < v2.size(); j=j+2) {
				v2_start = v2.get(j);
				v2_end = v2.get(j+1);

				if(((v1_start >= v2_start) && (v1_start <= v2_end)) && ((v1_end >= v2_start) && (v1_end <= v2_end))) {
					dropMe = true;
				}
			}
			if (!dropMe) {
				vector_strictlyNotIn.add(v1.get(i));
				vector_strictlyNotIn.add(v1.get(i+1));
			}

			//System.out.println("vector_strictlyNotIn"+vector_strictlyNotIn);
		}
		return vector_strictlyNotIn;
	}

	private ArrayList<Integer> startNotIn(ArrayList<Integer> v1, ArrayList<Integer> v2) {
		int v1_start = 0;
		int v2_start = 0;
		int v2_end = 0;
		ArrayList<Integer> vector_startNotIn = new ArrayList<Integer>();

		for(int i=0; i < v1.size(); i=i+2) {
			boolean dropMe = false;
			v1_start = v1.get(i);

			for(int j=0; j < v2.size(); j=j+2) {
				v2_start = v2.get(j);
				v2_end = v2.get(j+1);

				if(((v1_start >= v2_start) && (v1_start <= v2_end))) {
					dropMe = true;
				}
			}
			if (!dropMe) {
				vector_startNotIn.add(v1.get(i));
				vector_startNotIn.add(v1.get(i+1));
			}

			//System.out.println("vector_startNotIn"+vector_startNotIn);
		}
		return vector_startNotIn;
	}
	/**
	 * UTILITARIAN FUNCTIONS END
	 */

	private final void DEBUG(String msg) {
		//System.err.println("[DEBUG] "+msg);
	}
	public void disableUpdaters() {
		updaterDisabled = true;
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
			textLine = this.getText(0,getLength());
		} catch (BadLocationException ex) {
			ex.printStackTrace();
		}
		return textLine;
	}
	


	public String getSelectedDocumentLines(int start , int end ) {
		int startOffset;
		int endOffset;

		String text = "";

		startOffset = this.getParagraphElement(start).getStartOffset();
		endOffset = this.getParagraphElement(end).getEndOffset();
		//We read the document and put the document into the String text

		try {
			//Get the document line by line
			text = this.getText(startOffset, endOffset - startOffset);
		} catch (BadLocationException ex) {
			ex.printStackTrace();
		}


		return text;
	}

	public void enableUpdaters() {
		updaterDisabled = false;
	}

	private class IndentUpdater implements Runnable {
	    private DocumentEvent event = null;
	    
	    public IndentUpdater(DocumentEvent event) {
		super();
		this.event = event;
	    }
	    
	    public void run() { 
		//IndentAction.getXpadEditor();
		//editor = getEditor();
		//int  caretPosition = editor.getTextPane().getCaretPosition();
		
		try {
			//if (caretPosition != 0) {
			    //System.err.println("Text inserted = {"+event.getDocument().getText(event.getOffset(), event.getLength())+"}");
				//if (editor.getTextPane().getText(caretPosition-1, 1).equals("\n")) {
			if (event.getDocument().getText(event.getOffset(), event.getLength()).contains("\n")) {
			    indent(event.getOffset(), event.getOffset() + event.getLength());
			}
			//}
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
		
		//indent();
	    }
	}
	
	
	private class ColorUpdater implements Runnable {
	    private DocumentEvent event = null;
	    
	    public ColorUpdater(DocumentEvent event) {
		super();
		this.event = event;
	    }
	    
	    public void run() {
			//colorize();
			ColorizeAction.getXpadEditor();
			editor = getEditor();
			
			lineStartPosition =  editor.getTextPane().getStyledDocument().getParagraphElement(event.getOffset()).getStartOffset();
			lineEndPosition = editor.getTextPane().getStyledDocument().getParagraphElement(event.getOffset()+event.getLength()).getEndOffset()-1;
			
			if (lineStartPosition != lineEndPosition) {
				colorize(lineStartPosition, lineEndPosition);
			}
			/*
			// Get the current line (position of the caret) 
			int  caretPosition = editor.getTextPane().getCaretPosition();
			currentLine = editor.getTextPane().getStyledDocument().getDefaultRootElement().getElementIndex(caretPosition);

			// Get current line's start & end offsets
			lineStartPosition =  editor.getTextPane().getStyledDocument().getParagraphElement(caretPosition).getStartOffset();
			lineEndPosition = editor.getTextPane().getStyledDocument().getParagraphElement(caretPosition).getEndOffset()-1;
			
			// If we add a line (by pressing return)
			if (lineStartPosition == lineEndPosition) {
				lineStartPosition = lineStartPosition + lineEndPosition;
				lineEndPosition = lineEndPosition + lineEndPosition;
			} 
			
			if (lineStartPosition != lineEndPosition) {
				colorize(lineStartPosition, lineEndPosition);
			}
			*/
	    }
	}
	
	public void insertUpdate(DocumentEvent e) {
		
		if (e != null)
		{
			EventType = e.getType().toString();
		}
		
		DEBUG("--- Calling insertUpdate");
		if (!updaterDisabled) {

			if (autoColorize) {
				DEBUG("--- Calling insertUpdate -> colorize");
			    SwingUtilities.invokeLater(new ColorUpdater(e));
			}
			if (autoIndent) {
			    DEBUG("--- Calling insertUpdate -> indent");
			    SwingUtilities.invokeLater(new IndentUpdater(e));
			}
		}
	}
	

	
	
	public void removeUpdate(DocumentEvent e) {
		
		EventType = e.getType().toString();
		//System.err.println("--- Calling ScilabStyleDocument.removeUpdate");
		if (!updaterDisabled) {
			if (autoColorize) {
			    SwingUtilities.invokeLater(new ColorUpdater(e));
			}
		}
	}

	public void changedUpdate(DocumentEvent arg0) {
		
		EventType = arg0.getType().toString();
		// TODO Auto-generated method stub
	}

	public void setAutoIndent(boolean b) {
		//DEBUG("setAutoIndent("+b+")");
		autoIndent = b;
	}
	public boolean getAutoIndent() {
		//DEBUG("getAutoIndent("+autoIndent+")");
		return autoIndent;
	}
	public UndoManager getUndoManager() {
		return undo;
	}

	
	public boolean isContentModified(){
		return contentModified;
	}
	
	public void setContentModified(boolean contentModified){
		this.contentModified = contentModified;
	}

	public int getLineToColor() {
		return currentLine;
	}

	public void setLineToColor(int lineToColor) {
		this.currentLine = lineToColor;
	}

	
	public int getLineEndPosition() {
		return lineEndPosition;
	}

	public void setLineEndPosition(int lineEndPosition) {
		this.lineEndPosition = lineEndPosition;
	}

	public int getLineStartPosition() {
		return lineStartPosition;
	}

	public void setLineStartPosition(int lineStartPosition) {
		this.lineStartPosition = lineStartPosition;
	}
	
	public Xpad getEditor() {
		return editor;
	}

	public void setEditor(Xpad editor) {
		this.editor = editor;
	}

	/*
	 * DEBUG Class : Should be remove
	 * TODO : remove me
	 * FIXME : remove me
	 */
	private class Timer {
	    private long time = 0;
	    
	    public Timer() { start(); }
	    
	    public void start() { time = System.currentTimeMillis(); }
	    
	    public long top() {
		long result = System.currentTimeMillis() - time;
		start();
		return result;
	    }
	}

	
}
