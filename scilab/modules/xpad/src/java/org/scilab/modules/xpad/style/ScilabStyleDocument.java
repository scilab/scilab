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

	public void disableUpdaters() {
		updaterDisabled = true;
	}

	public void enableUpdaters() {
		updaterDisabled = false;
	}

	/*if you want to add a new style just add it in the xml*/
	private ArrayList<String> listStylesName ;
	//private final String[] allStyles = {"Operator", "Command","String","Bool" ,"Comment"} ;


	private final String[] quotations = {"(\"|')[^{\n}]*?(\"|')"};
	private final String[] bools = {"%T", "%F", "%t", "%f"};
	private final String[] comments = {"//[^{\n}]*", "/\\*.*?\\*/"};
	private final String[] operators = {"=", "\\+", "-", "\\*", "/", "\\\\", "\\^", 
			"\\./", "\\.\\\\", "\\.\\^", 
			"\\.\\*\\.", "\\./\\.", "\\.\\\\\\.",
			"==", "<", ">", "<=", ">=", "~=", "@=",
			"&", "\\|", "@", "~",
	"\\.\\.[\\.]*"};

	//Warning operators should be define in this order!
	private final String inInstruction = "elseif|else|if|while|for|do|function";
	private final String outInstruction = "endfunction|end";

	private final String IN = "IN";
	private final String OUT = "OUT";
	private final String TABULATION = "  ";

	private Style defaultStyle;

	private final void DEBUG(String msg) {
		System.err.println("[DEBUG] "+msg);
	}

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
	}

	/**
	 * DOCUMENT COLORISATION START
	 */
	public void colorize() {
		// Scilab keywords to be colored
		Hashtable<String, String[]> keywords = getScilabKeywords();    	
		String[] commands = (String[])keywords.get("command");
		String[] functions = (String[])keywords.get("function");
		String[] macros = (String[])keywords.get("macro");

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

		// We parse all words which are susceptible to be colored
		Vector<Vector<Integer>> boundaries_list = new Vector<Vector<Integer>>();
		boundaries_list = parse(bools, commands, comments, functions, macros, operators, quotations);

		if (!colorizeInprogress) {
			colorizeInprogress = true;
			this.removeUndoableEditListener(undo);
			this.addUndoableEditListener(null);
			resetStyle();
			try {
				applyStyle(boundaries_list.elementAt(0), getStyle("Bool"));
				applyStyle(boundaries_list.get(1), getStyle("Command"));
				applyStyle(boundaries_list.get(2), getStyle("Function"));
				applyStyle(boundaries_list.get(3), getStyle("Macro"));
				applyStyle(boundaries_list.get(4), getStyle("Operator"));
				applyStyle(boundaries_list.get(5), getStyle("String"));
				applyStyle(boundaries_list.get(6), getStyle("Comment"));

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

	private void applyStyle(Vector<Integer> boundaries, Style style) throws BadLocationException {
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
				//Get the correct starting line to indent
				//int[] interval = new int[2];
				//interval = readText(0, this.getLength());

				//Here start the text indentation
				//applyIndent2(interval[0], interval[1]);

				//applyIndent2();
				applyIndent3();
			} catch (BadLocationException e) {
				e.printStackTrace();
			}
			indentInprogress = false;
			enableUpdaters();
		}
	}

	public void applyIndent3() throws BadLocationException {

		String indentedText = "";
		String tab = "";
		boolean need_indentation = false;
		boolean bool_case = false;

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
		int select_line = -1;

		for (int i = 0; i < all_lines_without_spaces.size(); i++) {
			// Get commands for each lines
			command_list = getOperatorList2(all_lines_without_spaces.elementAt(i));

			// Check if in one line all operators are matching,
			// so we can know if the next line needs indentation or not
			// ex: if %T then function foo(1) endfunction end => doesn't need indentation
			// Warning: command_list looks like [IN, if, IN function, OUT, endfunction, OUT, end]
			Vector<String> vector_match;			
			vector_match = matchingOperators(command_list);			
			if (vector_match.isEmpty()) {
				need_indentation = false;
			} else {
				need_indentation = true;
			}
			//System.out.println("need_indentation= "+need_indentation);
			
			
			// Operator found in the given line
			if (command_list.size() > 0) {
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
						System.out.println("NOUS AVONS UN SELECT");
						tab += TABULATION;
						got_select = true;
						
						// If we have 'case' command
					} else if ((command_list.elementAt(1).toLowerCase().equals("case")) && 
							   (bool_case == false) &&
							   (got_select == true) ) {
						bool_case = true;
						tab += TABULATION;
						
					} else {
						if (bool_case == false) {
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
						if (bool_case == true) {
							bool_case = false;
						}
						if (got_select == true && command_list.elementAt(1).toLowerCase().equals("end")) {
							System.out.println("ON SUPP LA TAB");
							tab = tab.substring(0, tab.length()-2);
							got_select = false;
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


		System.out.println("indentedText = "+"{"+indentedText+"}");


		// Display the indentation
		this.replace(0, this.getLength(), indentedText, null);

	}






















	public void applyIndent2() throws BadLocationException {

		int document_start = 0;
		int document_end = this.getLength();

		System.out.println("+++ Calling applyIndent from "+document_start+" to "+document_end);

		int startOffset;
		int endOffset;
		String textLine = "";
		Vector<String> opList;
		String indentedText = "";
		String tab = "";
		boolean opMatch = false;
		boolean bool_case = false;

		// We read the document
		for (int i = document_start; i < document_end;) {
			// Get start & end position for a line
			startOffset = this.getParagraphElement(i).getStartOffset(); 
			endOffset = this.getParagraphElement(i).getEndOffset();
			//endOffset = this.getParagraphElement(i).getEndOffset() - 1;

			System.out.println("startOffset: "+startOffset);
			System.out.println("endOffset: "+endOffset);

			try {
				// Get the document line by line (start position, line size)
				textLine = this.getText(startOffset, endOffset - startOffset);
				//textLine = this.getText(startOffset, endOffset);
			} catch (BadLocationException e) {
				e.printStackTrace();
			}

			/*			System.out.println("TEXTLINE: {" + textLine + "}");
			//String is_return = textLine.substring(textLine.length()-2, textLine.length());
			if (textLine.equals("\n") || textLine.equals("")) {
				System.out.println("-------- SEE EOL ---- i = "+i+" --- document_end = "+document_end+" ------");
				endOffset = endOffset+1;
			} else {
				String is_return = textLine.substring(textLine.length()-1, textLine.length());
				System.out.println("*****is_return: {" + is_return + "}");
				if (is_return.equals("\n")) {
					endOffset = endOffset+1;
				}
			}*/


			// Step to next line
			//i = endOffset;

			// Remove space(s) located at the beginning of the given line
			textLine = removeFirstSpaces(textLine);
			textLine = textLine + "\n";

			System.out.println("textLine1111 = "+"{"+textLine+"}");

			// Get operators for a given line
			opList = getOperatorList2(textLine);

			/*			for (int j = 0; j < opList.size(); j++) {
				System.out.println("COMMANDS: " + opList.elementAt(j));
			}*/

			// Check if in one line all operators are matching,
			// so we can know if the next line needs indentation or not
			// ex: if %T then function foo(1) endfunction end => doesn't need indentation
			//opMatch = matchingOperators(opList);
			System.out.println("####################################OPMATCH "+ opMatch);
//			if (opMatch) {

//			}

			// Operator found in the given line
			if (opList.size() > 0  || opMatch == false) {
				// If we have 'IN' command
				if (opList.elementAt(0).equals(IN) && opMatch == false) {
					// No indentation in case of 'else' or 'elseif'
					if ((opList.elementAt(1).toLowerCase().equals("else")) || 
							(opList.elementAt(1).toLowerCase().equals("elseif"))) {
						if (indentedText.length() >= 2) {
							indentedText = indentedText.substring(0, indentedText.length()-2);
							System.out.println("indentedText1111 = "+"{"+indentedText+"}");
						}
						// 	If we have 'case' command
					} else if ((opList.elementAt(1).toLowerCase().equals("case")) && bool_case == false) {
						bool_case = true;
						tab += TABULATION;
					}else {
						if (bool_case == false) {
							tab += TABULATION;
						} else if (indentedText.length() >= 2) {
							indentedText = indentedText.substring(0, indentedText.length()-2);
							System.out.println("indentedText22222 = "+"{"+indentedText+"}");
						}
					}
					indentedText += textLine;
					System.out.println("textLine22222 = "+"{"+textLine+"}");
					System.out.println("indentedText3333 = "+"{"+indentedText+"}");

					// If we have "OUT' operator
				} else if (opList.elementAt(0).equals(OUT)) {
					if (indentedText.length() >= 2 && tab.length() >= 2) {	
						indentedText = indentedText.substring(0, indentedText.length()-2);
						System.out.println("indentedText4444 = "+"{"+indentedText+"}");
						tab = tab.substring(0, tab.length()-2);
						if (bool_case == true) {
							bool_case = false;
						}
					}
					indentedText += textLine;
					System.out.println("textLine3333 = "+"{"+textLine+"}");
					System.out.println("indentedText5555 = "+"{"+indentedText+"}");

					// Line got operators and they match, so no need of indentation
				} else {
					indentedText += textLine;
					System.out.println("textLine4444 = "+"{"+textLine+"}");
					System.out.println("indentedText6666 = "+"{"+indentedText+"}");
				}
				// Line without operator
			} else {
				indentedText += textLine;
				System.out.println("textLine55555 = "+"{"+textLine+"}");
				System.out.println("indentedText77777 = "+"{"+indentedText+"}");
			}

			// Add the indentation
			indentedText += tab;
			System.out.println("indentedText8888 = "+"{"+indentedText+"}");

			opList.clear();
			//textLine = "";
			opMatch = false;

			// Step to next line
			i = endOffset;
		}

		System.out.println("START= "+document_start);
		System.out.println("indentedText = "+"{"+indentedText+"}");
		System.out.println("END-START= "+ (document_end));

		// Remove the last line
		indentedText = indentedText.trim(); // si on selection tout le doc, sinon pas de trim (voir pk on rajoute saut de ligne a la fin)
		//indentedText = indentedText.replace("\n", "");
		// Display the indentation
		this.replace(document_start, document_end, indentedText, null);

		//System.out.println(indentedText);

		//ATTENTION : - reucperation des operateurs sur une meme ligne
		//            - si une ligne ne commence pas avec un operateur mais qu'elle en contient un 
		//            - gestion des commentaire /* */
		//			  - erreur d'indentation si ",end", ne reconnait pas le "end"
		//			  - erreur d'indentation, ne prend pas en compte l'indentation precedente
		//			  - erreur d'indentation si on a des lignes vides entre 2 lignes de texte
		//			  - 'case' non traite
		//			  - si on retouve un mot cle command qui n'est pas utiliser en tant que tel 
		//			    (ex: error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"gmres",4));)
		//				ici le 'for' est interpreter comme une commande, donc il y a indentation
		//			  - analyser la ligne du document qui est modifie et non plus le document en entier pour la colorisation
		//				dans le cas d'une quotation ou commentaire(/* */) analyser le document en entier
		//			  - 'if a==b then return; end' + saut de ligne => indente la ligne suivante
	}
	/**
	 * DOCUMENT INDENTATION END
	 */


	/**
	 * FIND AND REPLACE START
	 */
	public Vector<Integer> findWord(String word) {

		String fullText = getFullDocument();
		int lastIndex = 0;
		int wordSize = word.length();

		Vector<Integer> offsetList = new Vector<Integer>();

		//If we don't give any word to find
		if ( (word == null) || (word.equals("")) ) {
			System.out.println("Word is null or empty");;
		} else {
			//We find matching words
			while ((lastIndex = fullText.indexOf(word, lastIndex)) != -1) {
				int endIndex = lastIndex + wordSize;

				offsetList.add(lastIndex);		    	

				lastIndex = endIndex;
			}
		}
		return offsetList;
	}
	/**
	 * FIND AND REPLACE END
	 */


	/**
	 * UTILITARIAN FUNCTIONS START
	 */

	/**
	 * UTILITARIAN FUNCTIONS END
	 */


	public void changedUpdate(DocumentEvent e) {
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
						colorize();
					}
				}
			});  
		}
	}

	public void removeUpdate(DocumentEvent e) {
		//System.err.println("--- Calling removeUpdate");
		if (!updaterDisabled) {
			SwingUtilities.invokeLater(new Runnable() {
				public void run() {
					//resetStyle();
					if (autoIndent) {
						indent();
					}
					if (autoColorize) {
						colorize(); 
					}
				}
			});
		}
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


	public Vector<String> getOperatorList2(String text) {
		Vector<String> operator_list = new Vector<String>();
		Vector<String> op = new Vector<String>();

		Vector<Integer> comments_boundaries = new Vector<Integer>();
		Vector<Integer> quotations_boundaries = new Vector<Integer>();
		Vector<Integer> tmp_comm_and_quot = new Vector<Integer>();

		Vector<Integer> commands_boundaries = new Vector<Integer>();

		String[] commands_in = {"if", "else", "elseif", "while", "for", "do", "select", "case", "function"};
		String[] commands_out = {"end", "endfunction"};

		String[] commands = {"if", "else", "elseif", "while", "for", "do", "select", "case", "function", "end", "endfunction"};

		// Regexp for Scilab commands
		for (int i = 0; i < commands.length; i++) {
			commands[i] = "\\b" + commands[i] + "\\b"; 
		}

		// Find command boundaries in the given text
		commands_boundaries = findBoundaries(commands, text);

		comments_boundaries = findBoundaries(comments, null);
		quotations_boundaries = findBoundaries(quotations, null);			
		// Remove comments which are into quotations
		comments_boundaries = startNotIn(comments_boundaries, quotations_boundaries);
		// Remove quotations which are into comments
		quotations_boundaries = startNotIn(quotations_boundaries, comments_boundaries);

		// Union of comments & quotations to remove keywords
		tmp_comm_and_quot.addAll(comments_boundaries);
		tmp_comm_and_quot.addAll(quotations_boundaries);

		// Remove commads which are into quotations or comments
		commands_boundaries = strictlyNotIn(commands_boundaries, tmp_comm_and_quot);

		// Sort commands_boudaries
		Collections.sort(commands_boundaries);

		// The function applyIndent needs a vector in this format, ex: IN,IF,OUT,END
		for (int i = 0; i < commands_boundaries.size(); i=i+2) {
			op.add(text.substring(commands_boundaries.elementAt(i), commands_boundaries.elementAt(i+1)));
		}		
		for (int i = 0; i < op.size(); i++) {
			for (int j = 0; j < commands_in.length; j++) {
				if (op.elementAt(i).equals(commands_in[j])) {
					operator_list.add(IN);
					operator_list.add(op.elementAt(i));
				}
			}
			for (int j = 0; j < commands_out.length; j++) {
				if (op.elementAt(i).equals(commands_out[j])) {
					operator_list.add(OUT);
					operator_list.add(op.elementAt(i));
				}
			}
		}

		return operator_list;
	}


	public Vector<String> getOperatorList(String text) {

		//If the text is a comment we don't indent
		text = manageTextWithComment(text);

		Vector<String> v = new Vector<String>();
		Vector<String> op = new Vector<String>();
		Vector<String> inout = new Vector<String>();

		//Remove line separator for each line
		String sep = System.getProperty("line.separator"); 
		text = text.replaceAll(sep," ");

		//Getting word one by one for each line
		String[] word = text.split(" ");
		int[] wordSize = new int[word.length];

		//Find regular expressions
		Pattern beginPattern = Pattern.compile(inInstruction, Pattern.DOTALL);
		Pattern endPattern = Pattern.compile(outInstruction, Pattern.DOTALL);
		Matcher beginMatcher;
		Matcher endMatcher;

		for(int j=0; j<word.length; j++) {
			//Get word size
			wordSize[j] = word[j].length();

			//Looking for begin patterns
			beginMatcher = beginPattern.matcher(word[j]);
			endMatcher = endPattern.matcher(word[j]);

			while (beginMatcher.find()) {
				if (wordSize[j] == (beginMatcher.end()-beginMatcher.start())) {
					inout.addElement(IN);
					op.addElement(word[j]);
				}
			}
			//Looking for end patterns
			while (endMatcher.find()) {
				if (wordSize[j] == (endMatcher.end()-endMatcher.start())) {
					inout.addElement(OUT);
					op.addElement(word[j]);
				}

				//Manage 'end;' & 'endfunction;'
				if (word[j].startsWith("end;") || word[j].startsWith("endfunction;")) {
					inout.addElement(OUT);
					op.addElement(word[j]);
				}
			}
		}
		v.addAll(inout);
		v.addAll(op);

		for (int i = 0; i < v.size(); i++) {
			System.out.println("v = "+v.elementAt(i));
		}
		System.out.println("----------------------------------------");

		return v;
	}

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
		//line = line.trim();
		//spaces = 0;

		return line;
	}


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


	public String manageTextWithComment(String text) {

		String noCommentText = null;
		int pos = 0;

		if (text.contains("//")) {
			pos = text.indexOf("//");
			noCommentText = text.substring(0, pos);
		} else {
			noCommentText = text;
		}
		//TODO Later(Scilab 6), we will manage /* */ comments
		return noCommentText;
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
				textLine = this.getText(startOffset, endOffset - startOffset).toLowerCase();
			} catch (BadLocationException ex) {
				ex.printStackTrace();
			}
			i = endOffset;
			text += textLine;
		}
		return text;
	}

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

	private Vector<Vector<Integer>> parse(String[] bools, String[] commands, String[] comments, 
			String[] functions, String[] macros, String[] operators, String[] quotations) {

		Vector<Integer> boolsBoundaries, commandsBoundaries, 
		commentsBoundaries, functionsBoundaries, 
		macrosBoundaries, operatorsBoundaries, 
		quotationsBoundaries;

		Vector<Vector<Integer>> boundaries_list = new Vector<Vector<Integer>>();

		boolsBoundaries = findBoundaries(bools, null);
		commandsBoundaries = findBoundaries(commands, null);
		commentsBoundaries = findBoundaries(comments, null);
		functionsBoundaries = findBoundaries(functions, null);
		macrosBoundaries = findBoundaries(macros, null);
		operatorsBoundaries = findBoundaries(operators, null);
		quotationsBoundaries = findBoundaries(quotations, null);		


		boundaries_list = organizeBoundaries(boolsBoundaries, commandsBoundaries, commentsBoundaries, functionsBoundaries, 
				macrosBoundaries, operatorsBoundaries, quotationsBoundaries);

		return boundaries_list;
	}

	private Vector<Integer> findBoundaries(String[] keyword, String text) {

		Pattern pattern;
		Matcher matcher = null;
		Vector<Integer> bound = new Vector<Integer>();

		for(int i = 0 ; i < keyword.length ; i++)	{
			pattern = Pattern.compile(keyword[i], Pattern.DOTALL);
			try {
				if (text == null) {
					matcher = pattern.matcher(this.getText(0, this.getLength()));
				} else {
					matcher = pattern.matcher(text);
				}
				while(matcher.find()){
					//System.err.println("Match Found : "+(matcher.start())+","+(matcher.end()/*-matcher.start()*/));
					bound.add(new Integer(matcher.start()));	
					bound.add(new Integer(matcher.end()));
				}
			} catch (BadLocationException e) {
				e.printStackTrace();
			}
		}
		return bound;
	}

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
		vector_list.add(functionsBoundaries);
		vector_list.add(macrosBoundaries);
		vector_list.add(operatorsBoundaries);
		vector_list.add(quotationsBoundaries);
		vector_list.add(commentsBoundaries);

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
		}
		return vector_startNotIn;
	}
}