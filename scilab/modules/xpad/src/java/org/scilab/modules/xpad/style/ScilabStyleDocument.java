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
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.UndoableEditEvent;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.undo.UndoManager;

import org.scilab.modules.xpad.ScilabKeywords;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.actions.ColorizeAction;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class ScilabStyleDocument extends DefaultStyledDocument implements DocumentListener {

	private UndoManager undo = new UndoManager() {
		public void undoableEditHappened(UndoableEditEvent e) {
			//	    System.out.println("UndoableEditEvent ="+e.getClass().getCanonicalName());
			//	    System.out.println("UndoableEdit ="+e.getEdit().toString());
			//	    System.out.println("Source = "+e.getSource().toString());
			//	    System.out.println("filter = "+(e.getEdit() instanceof AttributeUndoableEdit));
			//			if(e.getEdit().getPresentationName().compareTo("addition") == 0
			//					|| e.getEdit().getPresentationName().compareTo("deletion") == 0) {
			undo.addEdit(e.getEdit());
			//			}
		}
	};

	private boolean autoIndent = false;
	private boolean autoColorize = true;
	private boolean colorizeInprogress = false;
	private boolean indentInprogress = false;
	private boolean updaterDisabled = false;

	private final String[] quotations = {"[^A-Z](\"|')[^{\n}]*?(\"|')"};
	//private final String[] quotations = {"(\"|')[^{\n}]*?(\"|')"};
	private final String[] bools = {"%T", "%F", "%t", "%f"};
	private final String[] comments = {"//[^{\n}]*", "/\\*.*?\\*/"};
	private final String[] operators = {"=", "\\+", "-", "\\*", "/", "\\\\", "\\^", 
			"\\./", "\\.\\\\", "\\.\\^", 
			"\\.\\*\\.", "\\./\\.", "\\.\\\\\\.",
			"==", "<", ">", "<=", ">=", "~=", "@=",
			"&", "\\|", "@", "~",
	"\\.\\.[\\.]*"};

	private final String IN = "IN";
	private final String OUT = "OUT";
	private final String TABULATION = "  ";
	private final int BOOLS = 0;
	private final int COMMANDS = 1;
	private final int COMMENTS = 2;
	private final int FUNCTIONS = 3;
	private final int MACROS = 4;
	private final int OPERATORS = 5;
	private final int QUOTATIONS = 6;
	
	private int lineStartPosition;
	private int lineEndPosition;
	private boolean singleLine = false;
	private int currentLine;
	
	Hashtable<String, String[]> keywords;
	String[] commands;
	String[] functions;
	String[] macros;
	
	Xpad editor;

	/*if you want to add a new style just add it in the xml*/
	private ArrayList<String> listStylesName ;
	//private final String[] allStyles = {"Operator", "Command","String","Bool" ,"Comment"} ;
	private Style defaultStyle;


	public ScilabStyleDocument() {
		super();

		Hashtable< String, Color>stylesColorsTable =  ConfigXpadManager.getAllForegroundColors();
		Hashtable< String, Boolean>stylesIsBoldTable = ConfigXpadManager.getAllisBold()  ;
		listStylesName  =  ConfigXpadManager.getAllStyleName();

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
		for(int i = 0 ; i < listStylesName.size() ; ++i) {
			Style otherStyle = this.addStyle(listStylesName.get(i), defaultStyle);
			StyleConstants.setBold(otherStyle, stylesIsBoldTable.get(listStylesName.get(i)));
			StyleConstants.setForeground(otherStyle, stylesColorsTable.get(listStylesName.get(i)));
		}
		
		loadingsForColorisation();
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
	
	public void colorize() {
		
		singleLine = false;
		
		// We parse all words which are susceptible to be colored
		Vector<Vector<Integer>> boundaries_list = parse(bools, commands, comments, functions, macros, operators, quotations, singleLine, 0, 0);

		if (!colorizeInprogress) {
			colorizeInprogress = true;
			this.removeUndoableEditListener(undo);
			this.addUndoableEditListener(null);
			resetStyle();
			try {
				applyStyle(boundaries_list.elementAt(BOOLS), getStyle("Bool"));
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
				colorizeInprogress = false;
			}
		}
	}

	
	public void colorizeSingleLine(Xpad editor, int caretPosition, int lineStartPosition, int lineEndPosition) {

		singleLine = true;

		// We parse all words which are susceptible to be colored
		Vector<Vector<Integer>> boundaries_list = parse(bools, commands, comments, functions, macros, operators, quotations, singleLine, lineStartPosition, lineEndPosition);

		if (!colorizeInprogress) {
			colorizeInprogress = true;
			this.removeUndoableEditListener(undo);
			this.addUndoableEditListener(null);
			resetSingleLineStyle(lineStartPosition, lineEndPosition);
			try {
				applyStyleToSingleLine(boundaries_list.elementAt(BOOLS), getStyle("Bool"), lineStartPosition, lineEndPosition);
				applyStyleToSingleLine(boundaries_list.get(COMMANDS), getStyle("Command"), lineStartPosition, lineEndPosition);
				applyStyleToSingleLine(boundaries_list.get(FUNCTIONS), getStyle("Function"), lineStartPosition, lineEndPosition);
				applyStyleToSingleLine(boundaries_list.get(MACROS), getStyle("Macro"), lineStartPosition, lineEndPosition);
				applyStyleToSingleLine(boundaries_list.get(OPERATORS), getStyle("Operator"), lineStartPosition, lineEndPosition);
				applyStyleToSingleLine(boundaries_list.get(QUOTATIONS), getStyle("String"), lineStartPosition, lineEndPosition);
				applyStyleToSingleLine(boundaries_list.get(COMMENTS), getStyle("Comment"), lineStartPosition, lineEndPosition);
			} catch (BadLocationException e) {
				e.printStackTrace();
			}
			finally {
				this.addUndoableEditListener(undo);
				colorizeInprogress = false;
			}
		}

	}
	
	private void resetStyle() {
		// Reset Color
		this.removeUndoableEditListener(undo);
		this.setCharacterAttributes(0, this.getLength(), this.getStyle("Default"), true);
		this.setParagraphAttributes(0, this.getLength(), this.getStyle("Default"), true);
		this.addUndoableEditListener(undo);
	}
	
	private void resetSingleLineStyle(int line_start, int line_end) {
		// Reset Color
		this.removeUndoableEditListener(undo);
		this.setCharacterAttributes(line_start, line_end-line_start, this.getStyle("Default"), true);
		this.setParagraphAttributes(line_start, line_end-line_start, this.getStyle("Default"), true);
		this.addUndoableEditListener(undo);
	}

	
	private void applyStyle(Vector<Integer> boundaries, Style style) throws BadLocationException {
		for(int i = 0 ; i < boundaries.size() ; i=i+2)	{
			this.setCharacterAttributes(boundaries.elementAt(i), boundaries.elementAt(i+1)-boundaries.elementAt(i), style, false);
			this.setParagraphAttributes(boundaries.elementAt(i), boundaries.elementAt(i+1)-boundaries.elementAt(i), style, false);
		}
	}
	
	private void applyStyleToSingleLine(Vector<Integer> boundaries, Style style, int start, int end) throws BadLocationException {
		for(int i = 0 ; i < boundaries.size() ; i=i+2)	{
			this.setCharacterAttributes(boundaries.elementAt(i), boundaries.elementAt(i+1)-boundaries.elementAt(i), style, false);
			this.setParagraphAttributes(boundaries.elementAt(i), boundaries.elementAt(i+1)-boundaries.elementAt(i), style, false);
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


	/**
	 * DOCUMENT INDENTATION START
	 */
	public void indent() {
		if (!indentInprogress) {
			disableUpdaters();
			indentInprogress = true;
			resetStyle();
			try {
				applyIndent();
				colorize();

			} catch (BadLocationException e) {
				e.printStackTrace();
			}
			indentInprogress = false;
			enableUpdaters();
		}
	}

	public void applyIndent() throws BadLocationException {

		String indentedText = "";
		String tab = "";
		boolean got_case = false;

		String text_to_indent = ""; // the entire document
		Vector<String> command_list = new Vector<String>(); // list of commands in the document

		try {
			// Get the document text to indent
			text_to_indent = this.getText(0, this.getLength());
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
		
		// Get all line break positions &
		// each line of the document
		Vector<Integer> line_break = new Vector<Integer>(); // positions of line break
		Vector<String> all_lines = new Vector<String>(); // the document line by line
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

		Vector<String> all_lines_without_spaces = new Vector<String>(); // the document line by line
		// without spaces
		for (int i = 0; i < all_lines.size(); i++) {
			String no_space_line = removeFirstSpaces(all_lines.elementAt(i));
			all_lines_without_spaces.add(no_space_line);
		}
		
		boolean got_select = false;

		for (int i = 0; i < all_lines_without_spaces.size(); i++) {
			// Get commands for each lines
			command_list = getOperatorList(all_lines_without_spaces.elementAt(i));
			
			// Here start the indentation process
			if (command_list.size() > 0) {
				// Check if in one line all operators are matching,
				// so we can know if the next line needs indentation or not
				// ex: if %T then function foo(1) endfunction end => doesn't need indentation
				// Warning: command_list looks like [IN, if, IN function, OUT, endfunction, OUT, end]
				Vector<String> vector_match = matchingOperators(command_list);			

				// If we have 'IN' command
				if (command_list.elementAt(0).equals(IN)) {
					// No indentation in case of 'else' or 'elseif'
					if ((command_list.elementAt(1).toLowerCase().equals("else")) || 
							(command_list.elementAt(1).toLowerCase().equals("elseif"))) {
						if (indentedText.length() >= 2) {
							indentedText = indentedText.substring(0, indentedText.length()-2);
						}
						// 	If we have 'select' command
					} else if (command_list.elementAt(1).toLowerCase().equals("select")) {
						tab += TABULATION;
						got_select = true;

						// If we have 'case' command
					} else if ((command_list.elementAt(1).toLowerCase().equals("case")) && 
							(got_case == false) &&
							(got_select == true) ) {
						got_case = true;
						tab += TABULATION;

					} else {
						if (got_case == false) {
							tab += TABULATION;
						} else if (indentedText.length() >= 2) {
							indentedText = indentedText.substring(0, indentedText.length()-2);
						}
					}
					indentedText += all_lines_without_spaces.elementAt(i);

					// If we have "OUT' operator
				} else if (command_list.elementAt(0).equals(OUT)) {
					if (indentedText.length() >= 2 && tab.length() >= 2) {	
						indentedText = indentedText.substring(0, indentedText.length()-2);
						tab = tab.substring(0, tab.length()-2);
						if (got_select == true && 
								got_case == true &&
								command_list.elementAt(1).toLowerCase().equals("end")) {
							tab = tab.substring(0, tab.length()-2);
							indentedText = indentedText.substring(0, indentedText.length()-2);
							got_select = false;
							got_case = false;
						}
					}
					indentedText += all_lines_without_spaces.elementAt(i);

					// Line got operators and they match, so no need of indentation
				} else {
					indentedText += all_lines_without_spaces.elementAt(i);
				}
				// Line without operator
			} else {
				indentedText += all_lines_without_spaces.elementAt(i);
			}

			// Add the indentation
			if (i != all_lines_without_spaces.size()-1) {
				indentedText += tab;
			}
			command_list.clear();
		} // end for

		// Display the indentation
		this.replace(0, this.getLength(), indentedText, null);

		// ATTENTION, ces cas ne sont pas traité: 
		//            - gestion des commentaire /* */
		//			  - analyser la ligne du document qui est modifie et non plus le document en entier pour la colorisation
		//				dans le cas d'une quotation ou commentaire(/* */) analyser le document en entier
	}

	/*
	 * Remove all spaces or tabulations at the begining a string
	 * This function is used for the indentation only
	 * ex: '   hello world' will be tranformed into 'hello world'
	 */
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

	/*
	 * Get all commands given in the string
	 * This function is used for the indentation only
	 */
	public Vector<String> getOperatorList(String text) {
		Vector<String> operator_list = new Vector<String>();
		Vector<String> op = new Vector<String>();

		Vector<Integer> comments_boundaries = new Vector<Integer>();
		Vector<Integer> quotations_boundaries = new Vector<Integer>();
		Vector<Integer> tmp_comm_and_quot = new Vector<Integer>();

		Vector<Integer> commands_boundaries = new Vector<Integer>();

		String[] commands_in = {"if", "else", "elseif", "while", "for", "do", "select", "case", "function"};
		String[] commands_out = {"end", "endfunction"};

		String[] allCommands = {"if", "else", "elseif", "while", "for", "do", "select", "case", "function", "end", "endfunction"};

		// Regexp for Scilab commands
		for (int i = 0; i < allCommands.length; i++) {
			allCommands[i] = "\\b" + allCommands[i] + "\\b";
		}

		// Find command boundaries in the given text
		commands_boundaries = findBoundaries(allCommands, false, 0, this.getLength(), text);
		comments_boundaries = findBoundaries(comments, false, 0, this.getLength(), text);
		quotations_boundaries = findBoundaries(quotations, false, 0, this.getLength(), text);
		
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
			op.add(text.substring(commands_boundaries.elementAt(i), commands_boundaries.elementAt(i+1)));
		}
		
		for (int i = 0; i < op.size(); i++) {
			for (int j = 0; j < commands_in.length; j++) {
				if (op.elementAt(i).toLowerCase().equals(commands_in[j])) {
					operator_list.add(IN);
					operator_list.add(op.elementAt(i));
				}
			}
			for (int j = 0; j < commands_out.length; j++) {
				if (op.elementAt(i).toLowerCase().equals(commands_out[j])) {
					operator_list.add(OUT);
					operator_list.add(op.elementAt(i));
				}
			}
		}
		
		return operator_list;
	}

	/*
	 * Check if in one line all commands are matching,
	 * by this way we can know if the next line needs indentation or not
	 * ex: if %T then function foo(1) endfunction end => doesn't need indentation
	 * ex: if %T then foo(1) end for => need indentation
	 * This function is used for the indentation only
	 * WARNING: command_list looks like [IN, if, IN function, OUT, endfunction, OUT, end]
	 */
	public Vector<String> matchingOperators(Vector<String> command_list) {

		int tmp_size = command_list.size();

		for (int i = 0; i < command_list.size() - 3; i=i+2) {
			if ((command_list.elementAt(i+1).toLowerCase().equals("function") && 
					command_list.elementAt(i+3).toLowerCase().equals("endfunction"))
					||
					(command_list.elementAt(i+3).toLowerCase().equals("end") && 
							!(command_list.elementAt(i+1).toLowerCase().equals("function")) &&
							!(command_list.elementAt(i+1).toLowerCase().equals("endfunction")) && 
							!(command_list.elementAt(i+1).toLowerCase().equals("end")) )) {
				command_list.removeElementAt(i+3);					
				command_list.removeElementAt(i+2);
				command_list.removeElementAt(i+1);
				command_list.removeElementAt(i);
				i = i -2;
			} 
		}
		if (tmp_size == command_list.size()) {
			return command_list;
		}
		return matchingOperators(command_list);
	}
	/**
	 * DOCUMENT INDENTATION END
	 */


	/**
	 * DOCUMENT COMMENT ACTION
	 */
	public void commentText(int start_position, int end_position) {

		String text_to_comment = "";
		try {
			// Get the document text to comment
			text_to_comment = this.getText(start_position, end_position-start_position);
		} catch (BadLocationException e) {
			e.printStackTrace();
		}

		Vector<Integer> line_break = new Vector<Integer>(); // positions of line break
		Vector<String> all_lines = new Vector<String>(); // the document line by line
		String line = "";

		if (start_position != end_position) {
			for (int i = 0; i < text_to_comment.length(); i++) {
				line = line.concat(text_to_comment.charAt(i)+"");

				if (text_to_comment.charAt(i)=='\n') {
					line_break.add(i);
					all_lines.add(line);
					line = "";
				}
				if (i==text_to_comment.length()-1) {
					all_lines.add(line);
					line = "";
				}
			}

			String commented_text = "";
			for (int i = 0; i < all_lines.size(); i++) {
				String tmp = "";
				if (!(all_lines.elementAt(i).equals(""))) {
					if (all_lines.elementAt(i).length() >= 2) {
						if (all_lines.elementAt(i).substring(0, 2).equals("//")) {
							tmp = all_lines.elementAt(i).substring(2, all_lines.elementAt(i).length());
						} else {
							tmp = "//" + all_lines.elementAt(i);
						}
					}
				}
				commented_text += tmp;
			}

			// Display the text commented
			try {
				this.replace(start_position, end_position-start_position, commented_text, null);
			} catch (BadLocationException e) {
				e.printStackTrace();
			}
		}

		line_break.clear();
		all_lines.clear();
	}
	/**
	 * DOCUMENT COMMENT ACTION END
	 */


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
				word = "\\b" + word + "\\b" ;
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word ;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		


			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord){
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
		String fullText = getseletecDocumentLines(currentSelectStart, currentSelectEnd) ;
		
		int offset = this.getParagraphElement(currentSelectStart).getStartOffset() ;
		int lastIndex = 0;
		int wordSize = word.length();;
		ArrayList<Integer[]> offsetList = new ArrayList<Integer[]>();

		//If we don't give any word to find
		if ( (word != null) && !(word.equals("")) ) {
			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b" ;
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word ;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		


			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord){
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

	public int[] readText(int s, int e) {
		int startOffset;
		int endOffset;
		String textLine = "";
		String text = "";
		Vector<Integer> min = new Vector<Integer>();
		Vector<Integer> max = new Vector<Integer>();
		int[] interval = new int[2];

		//We read the document
		for (int i = 0; i < this.getLength();) {
			startOffset = this.getParagraphElement(i).getStartOffset();
			endOffset = this.getParagraphElement(i).getEndOffset();
			min.add(startOffset);
			max.add(endOffset);

			try {
				//Get the document line by line
				textLine = this.getText(startOffset, endOffset - startOffset);
			} catch (BadLocationException ex) {
				ex.printStackTrace();
			}
			i = endOffset;
			text += textLine;
		}

		//If we only select a part of a line
		for (int i = 0; i < min.size(); i++) {
			if (s > min.elementAt(i)) {
				interval[0] = min.elementAt(i);
			}
		}
		for (int i = 0; i < max.size(); i++) {
			if (e > max.elementAt(i)) {
				interval[1] = max.elementAt(i);
				interval[1] = interval[1] - 1 ;
			}
		}
		return interval;
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
		int index = -1 ;
		int end = -1 ;



		if ( (word != null) && (!word.equals(""))  ) {
			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b" ;
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word ;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		


			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord){
				Pattern pattern = Pattern.compile(word);
				Matcher matcher = pattern.matcher(fullText.substring(currentPos));

				if (matcher.find()) {
					index = matcher.start()+currentPos;
					end = matcher.end()+currentPos;
				}else{
					index = -1 ;
					end  = -1 ;
				}

				// ... for other case
			}else {
				index = fullText.indexOf(word,currentPos);
				end = index + word.length();
			}
		}

			return new int [] {index , end } ;
	}

	public int[] findNextWord (String word ,int currentPos,int currentSelectStart ,int currentSelectEnd, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		
		String fullText = getseletecDocumentLines(currentSelectStart, currentSelectEnd) ;
		int offset = this.getParagraphElement(currentSelectStart).getStartOffset() ;
		System.out.println(currentPos) ;
		currentPos -=  offset ;
		
		int index = -1 ;
		int end = -1 ;


		if ( (word != null) && (!word.equals(""))  ) {
			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b" ;
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word ;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		


			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord){
				Pattern pattern = Pattern.compile(word);
				Matcher matcher = pattern.matcher(fullText.substring(currentPos));

				if (matcher.find()) {
					index = matcher.start()+currentPos+offset;
					end = matcher.end()+currentPos+offset;
				}else{
					index = -1 ;
					end  = -1 ;
				}

				// ... for other case
			}else {
			
				index = fullText.indexOf(word,currentPos);
				if (index != -1) index += offset ;
				end = index + word.length();
			}
		}

			return new int [] {index , end } ;
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
		int index = -1 ;
		int end = -1 ;
		Pattern pattern;

		if ( (word != null) && (!word.equals(""))  ) {

			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b" ;
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word ;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		


			//We find matching words ...
			// ... for regexp or whole words

			if (useRegexp || wholeWord){
				 pattern = Pattern.compile(word);
			}else{// ... for other case
				// we use regexp in both case cause of a nasty bug when you have string like 
				//121212  and you search "121" forward then backward
				pattern = Pattern.compile(word , Pattern.LITERAL );
				
			}
				Matcher matcher = pattern.matcher(fullText.substring(0,currentPos));

				boolean found = false ;
				while (matcher.find()) {
					index = matcher.start();
					end = matcher.end();
					found = true ;
				}

				if(!found){
					index = -1 ;
					end = -1;
				}
		}


		/*if nothing index and end will both be equal to -1*/
		return new int [] {index , end } ;


	}
	
	
	public int[] findPreviousWord (String word , int currentPos,int currentSelectStart ,int currentSelectEnd, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		String fullText = getseletecDocumentLines(currentSelectStart, currentSelectEnd) ;
		int offset = this.getParagraphElement(currentSelectStart).getStartOffset() ;
		System.out.println(currentPos) ;
		currentPos -=  offset ;
		int index = -1 ;
		int end = -1 ;
		Pattern pattern;

		if ( (word != null) && (!word.equals(""))  ) {

			// prepare word for each kind of search
			if (wholeWord){
				word = "\\b" + word + "\\b" ;
			}
			if (!caseSensitive){
				if (useRegexp || wholeWord ){
					word = "(?i)" + word ;
				}
				else{
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		


			//We find matching words ...
			// ... for regexp or whole words

			if (useRegexp || wholeWord){
				 pattern = Pattern.compile(word);
			}else{// ... for other case
				// we use regexp in both case cause of a nasty bug when you have string like 
				//121212  and you search "121" forward then backward
				pattern = Pattern.compile(word , Pattern.LITERAL );
				
			}
				System.out.println(currentPos) ;
				Matcher matcher = pattern.matcher(fullText.substring(0,currentPos));

				boolean found = false ;
				while (matcher.find()) {
					index = matcher.start() + offset;
					end = matcher.end() + offset;
					found = true ;
				}

				if(!found){
					index = -1 ;
					end = -1;
				}
		}


		/*if nothing index and end will both be equal to -1*/
		return new int [] {index , end } ;


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

		for (int i = 0; i < commands.length; i++) {
			keywords.put("command", commands);
		}
		for (int i = 0; i < functions.length; i++) {
			keywords.put("function", functions);
		}
		for (int i = 0; i < macros.length; i++) {
			keywords.put("macro", macros);
		}
		return keywords;
	}

	/*
	 * Parse all Scilab keywords
	 * This function is used for the syntactic colorization
	 */
	private Vector<Vector<Integer>> parse(String[] bools, String[] commands, String[] comments, 
			String[] functions, String[] macros, String[] operators, String[] quotations, boolean singleLine, int start, int end) {

		Vector<Integer> boolsBoundaries, commandsBoundaries, 
		commentsBoundaries, functionsBoundaries, 
		macrosBoundaries, operatorsBoundaries, 
		quotationsBoundaries;

		Vector<Vector<Integer>> boundaries_list = new Vector<Vector<Integer>>();

		boolsBoundaries = findBoundaries(bools, singleLine, start, end, null);
		commandsBoundaries = findBoundaries(commands, singleLine, start, end, null);
		commentsBoundaries = findBoundaries(comments, singleLine, start, end, null);
		functionsBoundaries = findBoundaries(functions, singleLine, start, end, null);
		macrosBoundaries = findBoundaries(macros, singleLine, start, end, null);
		operatorsBoundaries = findBoundaries(operators, singleLine, start, end, null);
		quotationsBoundaries = findBoundaries(quotations, singleLine, start, end, null);	


		boundaries_list = organizeBoundaries(boolsBoundaries, commandsBoundaries, commentsBoundaries, functionsBoundaries, 
				macrosBoundaries, operatorsBoundaries, quotationsBoundaries);

		return boundaries_list;
	}

	/*
	 * Get start & end position for each keywords
	 * String[] keyword is a type of keywords(ex: operators, commands, macros..)
	 * String text is where we make the search, 
	 * if text is null we apply the research to the entire document
	 */
	private Vector<Integer> findBoundaries(String[] keyword, boolean singleLine, int start, int end, String text) {

		Pattern pattern;
		Matcher matcher = null;
		Vector<Integer> bound = new Vector<Integer>();
		
		for(int i = 0 ; i < keyword.length ; i++)	{
			pattern = Pattern.compile(keyword[i], Pattern.DOTALL);
			try {
				if (singleLine && text == null) {
					matcher = pattern.matcher(this.getText(start, end - start));
					while(matcher.find()){
						//System.err.println("Match Found : "+(matcher.start())+","+(matcher.end()/*-matcher.start()*/));
						bound.add(new Integer(matcher.start() + start));	
						bound.add(new Integer(matcher.end() + start));
					}

				} else {
					if (text != null) {
						matcher = pattern.matcher(text);
					} else {
						matcher = pattern.matcher(this.getText(0, this.getLength()));
					}
					
					while(matcher.find()){
						//System.err.println("Match Found : "+(matcher.start())+","+(matcher.end()/*-matcher.start()*/));
						bound.add(new Integer(matcher.start()));	
						bound.add(new Integer(matcher.end()));
					}
				}
			} catch (BadLocationException e) {
				e.printStackTrace();
			}
		}
		return bound;
	}

	/*
	 * When we have all boundaries for each type of keywords
	 * we filter 'bad' boundaries
	 * ex: if we have quotations into comments (and the opposite), keywords into quotations or comments 
	 */
	private Vector<Vector<Integer>> organizeBoundaries(Vector<Integer> boolsBoundaries, Vector<Integer> commandsBoundaries,
			Vector<Integer> commentsBoundaries, Vector<Integer> functionsBoundaries, 
			Vector<Integer> macrosBoundaries, Vector<Integer> operatorsBoundaries, 
			Vector<Integer> quotationsBoundaries) {

		Vector<Integer> tmp_comm_and_quot = new Vector<Integer>();
		Vector<Vector<Integer>> vector_list = new Vector<Vector<Integer>>();

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

	private Vector<Integer> strictlyNotIn(Vector<Integer> v1, Vector<Integer> v2) {
		int v1_start = 0;
		int v1_end = 0;
		int v2_start = 0;
		int v2_end = 0;

		Vector<Integer> vector_strictlyNotIn = new Vector<Integer>();

		// Remove interval from v1 which are include in interval of v2
		for(int i=0; i < v1.size(); i=i+2) {
			boolean dropMe = false;
			v1_start = v1.elementAt(i);
			v1_end = v1.elementAt(i+1);

			for(int j=0; j < v2.size(); j=j+2) {
				v2_start = v2.elementAt(j);
				v2_end = v2.elementAt(j+1);

				if(((v1_start >= v2_start) && (v1_start <= v2_end)) && ((v1_end >= v2_start) && (v1_end <= v2_end))) {
					dropMe = true;
				}
			}
			if (!dropMe) {
				vector_strictlyNotIn.addElement(v1.elementAt(i));
				vector_strictlyNotIn.addElement(v1.elementAt(i+1));
			}

			//System.out.println("vector_strictlyNotIn"+vector_strictlyNotIn);
		}
		return vector_strictlyNotIn;
	}

	private Vector<Integer> startNotIn(Vector<Integer> v1, Vector<Integer> v2) {
		int v1_start = 0;
		int v2_start = 0;
		int v2_end = 0;
		Vector<Integer> vector_startNotIn = new Vector<Integer>();

		for(int i=0; i < v1.size(); i=i+2) {
			boolean dropMe = false;
			v1_start = v1.elementAt(i);

			for(int j=0; j < v2.size(); j=j+2) {
				v2_start = v2.elementAt(j);
				v2_end = v2.elementAt(j+1);

				if(((v1_start >= v2_start) && (v1_start <= v2_end))) {
					dropMe = true;
				}
			}
			if (!dropMe) {
				vector_startNotIn.addElement(v1.elementAt(i));
				vector_startNotIn.addElement(v1.elementAt(i+1));
			}

			//System.out.println("vector_startNotIn"+vector_startNotIn);
		}
		return vector_startNotIn;
	}
	/**
	 * UTILITARIAN FUNCTIONS END
	 */

	private final void DEBUG(String msg) {
		System.err.println("[DEBUG] "+msg);
	}
	public void disableUpdaters() {
		updaterDisabled = true;
	}

	public String getFullDocument() {
		int startOffset;
		int endOffset;
		String textLine = "";
		String text = "";

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
			text += textLine;
		}
		return text;
	}

	public String getseletecDocumentLines(int start , int end ) {
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

	public void insertUpdate(DocumentEvent e) {
		//System.err.println("--- Calling insertUpdate");
		if (!updaterDisabled) {
			SwingUtilities.invokeLater(new Runnable() {
				public void run() {
					//resetStyle();
					if (autoIndent) {
						indent();
					}
					if (autoColorize) {
						//colorize();
						setSingleLine(true);
						ColorizeAction.getXpadEditor();
						editor = getEditor();
						
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
							colorizeSingleLine(editor, caretPosition, lineStartPosition, lineEndPosition);
						}
						setSingleLine(false);
					}
				}
			});  
		}
	}
	public void removeUpdate(DocumentEvent e) {
		//System.err.println("--- Calling ScilabStyleDocument.removeUpdate");
		if (!updaterDisabled) {
			SwingUtilities.invokeLater(new Runnable() {
				public void run() {
					//resetStyle();
					if (autoIndent) {
						indent();
					}
					if (autoColorize) {
						//colorize();
						setSingleLine(true);
						ColorizeAction.getXpadEditor();
						editor = getEditor();
						
						
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
							colorizeSingleLine(editor, caretPosition, lineStartPosition, lineEndPosition);
						}
						setSingleLine(false);
					}
				}
			});
		}
	}

	public void changedUpdate(DocumentEvent arg0) {
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

	

	public boolean isSingleLine() {
		return singleLine;
	}

	public void setSingleLine(boolean singleLine) {
		this.singleLine = singleLine;
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

	

	
}
