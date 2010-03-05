/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xpad.style;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.event.DocumentEvent;
import javax.swing.text.BadLocationException;
import javax.swing.text.Position;
import javax.swing.text.Style;

/**
 * This class manages the Colorization aspect
 */
public class ColorizationManager {
	private volatile boolean colorizeInprogress;
	private boolean singleLine = false;
	private int lineStartPosition;
	private int lineEndPosition;
	private int currentLine;
	private final int VARIABLES = 0;
	private final int COMMANDS = 1;
	private final int COMMENTS = 2;
	private final int FUNCTIONS = 3;
	private final int MACROS = 4;
	private final int OPERATORS = 5;
	private final int QUOTATIONS = 6;

	private long millis = 0;

	private Pattern quotationsPattern, commentsPattern, boolsPattern, commandsPattern, functionsPattern, macrosPattern, operatorsPattern;

	/**
	 * Compile Pattern
	 * @param words word list
	 * @param useWordBoundaries boolean
	 * @return pattern
	 */
	private Pattern compilePattern(String[] words, boolean useWordBoundaries) {
		StringBuffer buffer = new StringBuffer();
		if (useWordBoundaries) {
			buffer.append("(?=%|\\b)("); // '%' is not a 'word' character so \b would not match place before '%'
		}
		for (int i = 0; i != words.length; ++i) {
			if (i != 0) {
				buffer.append('|');
			}
			buffer.append(words[i]);
		}
		if (useWordBoundaries) {
			buffer.append(")\\b");
		}
		return Pattern.compile(buffer.toString(), Pattern.DOTALL);
	}

	/**
	 * Colorization manager
	 */
	public ColorizationManager() {
		// Scilab keywords to be colored
		KeywordManager keywordManager = new KeywordManager();
		boolsPattern = compilePattern(KeywordManager.getBools(), true);
		quotationsPattern = compilePattern(KeywordManager.getQuotations(), false);
		commentsPattern = compilePattern(KeywordManager.getComments(), false);
		operatorsPattern = compilePattern(KeywordManager.getOperators(), false);

		Hashtable<String, String[]> keywords = keywordManager.getScilabKeywords();
		commandsPattern = compilePattern(keywords.get("command"), true);
		functionsPattern = compilePattern(keywords.get("function"), true);
		macrosPattern = compilePattern(keywords.get("macro"), true);

	}


	/**
	 * Getter line to color
	 * @return integer
	 */
	public int getLineToColor() {
		return currentLine;
	}

	/**
	 * Setter line to color
	 * @param lineToColor integer
	 */
	public void setLineToColor(int lineToColor) {
		this.currentLine = lineToColor;
	}

	/**
	 * Getter line end position
	 * @return integer
	 */
	public int getLineEndPosition() {
		return lineEndPosition;
	}

	/**
	 * Setter line end position
	 * @param lineEndPosition integer
	 */
	public void setLineEndPosition(int lineEndPosition) {
		this.lineEndPosition = lineEndPosition;
	}

	/**
	 * Getter line start position
	 * @return integer
	 */
	public int getLineStartPosition() {
		return lineStartPosition;
	}

	/**
	 * Setter line start position
	 * @param lineStartPosition integer
	 */
	public void setLineStartPosition(int lineStartPosition) {
		this.lineStartPosition = lineStartPosition;
	}

	/**
	 * DOCUMENT COLORISATION START
	 */
	public void loadingsForColorisation() {
	}

	/**
	 * Colorize function
	 * @param scilabDocument document
	 * @param startOffset start offset
	 * @param endOffset end offset
	 * @return boolean
	 */
	public boolean colorize(ScilabStyleDocument scilabDocument, int startOffset, int endOffset) {
		//System.err.println("--> Calling colorize("+startOffset+", "+endOffset+")");	
		//Timer timer = new Timer();
		//DEBUG("Colorize [before parse] : " + timer.top());
		singleLine = false;
		/*try {
		System.err.println("colorizing:|"+scilabDocument.getText(startOffset, endOffset-startOffset)+"|");
		} catch (BadLocationException e) {
			e.printStackTrace();
		}*/
		// We parse all words which are susceptible to be colored
		ArrayList<ArrayList<Integer>> boundaries_list = 
			this.parse(scilabDocument, startOffset, endOffset);
		//DEBUG("Colorize [after parse] : " + timer.top());
		if (!colorizeInprogress) {
			//colorizeInprogress = true;
			scilabDocument.disableUndoManager();
			resetStyle(scilabDocument, startOffset, endOffset);
			try {
				if (applyStyle(scilabDocument, boundaries_list.get(VARIABLES), scilabDocument.getStyle("Variable")) == false) { 
					return false; 
				}
				if (applyStyle(scilabDocument, boundaries_list.get(COMMANDS), scilabDocument.getStyle("Command")) == false) { 
					return false;
				}
				if (applyStyle(scilabDocument, boundaries_list.get(FUNCTIONS), scilabDocument.getStyle("Function")) == false) { 
					return false;
				}
				if (applyStyle(scilabDocument, boundaries_list.get(MACROS), scilabDocument.getStyle("Macro")) == false) { 
					return false;
				}
				if (applyStyle(scilabDocument, boundaries_list.get(OPERATORS), scilabDocument.getStyle("Operator")) == false) { 
					return false;
				}
				if (applyStyle(scilabDocument, boundaries_list.get(QUOTATIONS), scilabDocument.getStyle("String")) == false) { 
					return false;
				}
				if (applyStyle(scilabDocument, boundaries_list.get(COMMENTS), scilabDocument.getStyle("Comment")) == false) { 
					return false;
				}
			} catch (BadLocationException e) {
				e.printStackTrace();
			} finally {
				scilabDocument.enableUndoManager();
				//colorizeInprogress = false;
			}
		}
		//		DEBUG("Colorize [after all applyStyle] : " + timer.top());

		return true;
	}

	/**
	 * Reset document style
	 * @param scilabDocument document
	 * @param start_offset start offset
	 * @param end_offset end offset
	 */
	private void resetStyle(ScilabStyleDocument scilabDocument, int start_offset, int end_offset) {
		// Reset Color
		scilabDocument.setCharacterAttributes(start_offset, end_offset-start_offset, scilabDocument.getStyle("Default"), false);
	}

	/**
	 * Reset document style
	 * @param scilabDocument document
	 */
	public void resetStyle(ScilabStyleDocument scilabDocument) {
		resetStyle(scilabDocument, 0, scilabDocument.getLength());
	}

	/**
	 * Apply a document style
	 * @param scilabDocument document
	 * @param boundaries word boundaries
	 * @param style style to apply
	 * @return boolean
	 * @throws BadLocationException exception
	 */
	private boolean  applyStyle(ScilabStyleDocument scilabDocument, ArrayList<Integer> boundaries, Style style) throws BadLocationException {
		if (style != null) {
			for (int i = 0; i < boundaries.size(); i = i + 2) {
				scilabDocument.setCharacterAttributes(boundaries.get(i), boundaries.get(i + 1) - boundaries.get(i), style, false);
			}
			return true;
		}
		return false;
	}

	/**
	 * This Class updates the color
	 * @author Sylvestre Koumar
	 *
	 */
	public class ColorUpdater implements Runnable {
		private ScilabStyleDocument scilabDocument;
		private Position startPosition;
		private Position endPosition;
		int endOffset = -1; // javax.text API treats end of doc as a special case so we need to handle it as a special case :(
		
		/**
		 * Default constructor
		 * @param scilabDocument document
		 * @param event document event
		 */
		public ColorUpdater(ScilabStyleDocument scilabDocument, DocumentEvent event) {
			super();
			this.scilabDocument = scilabDocument;
			if (event != null && event.getType() != DocumentEvent.EventType.CHANGE) {
				try {

					this.startPosition = scilabDocument.createPosition(scilabDocument.getParagraphElement(
							event.getOffset()).getStartOffset());
					// when inserting we must colorize until the end of the last line
					// when removing edit length is not considered there is only one line in the end
					int endOffset = scilabDocument.getParagraphElement(
							event.getOffset() + ((event.getType() 
									==  DocumentEvent.EventType.INSERT) ? event.getLength() : 0)).getEndOffset();
					// unfortunately, when appending text, the java API considers that we are inserting before the virtual
					// end-of-document position (getEndOffset() of the last Element is > getLength() !)
					// so we need to special case this and use offset and not Position is this case.
					if (endOffset > scilabDocument.getLength()) {
						this.endOffset = endOffset;
						this.endPosition = null;
					} else {
						this.endOffset = -1;
						this.endPosition = scilabDocument.createPosition(endOffset);
					}
				} catch (BadLocationException e) {
					e.printStackTrace();
					this.startPosition = this.endPosition = null;
				} 
			} else {	    	
				this.startPosition = this.endPosition = null;
			}
		}
		/**
		 * Constructor
		 * @param event document event
		 */
		public ColorUpdater(DocumentEvent event) {
			this((ScilabStyleDocument) event.getDocument(), event);
		}
		
		/**
		 * Constructor
		 * @param scilabDocument document
		 * @param startOffset start offset
		 * @param endOffset end offset
		 */
		public ColorUpdater(ScilabStyleDocument scilabDocument, int startOffset, int endOffset) {
			super();
			this.scilabDocument = scilabDocument;
			try {
				this.startPosition = scilabDocument.createPosition(scilabDocument.getParagraphElement(startOffset).getStartOffset());
				this.endPosition = scilabDocument.createPosition(scilabDocument.getParagraphElement(endOffset).getEndOffset()-1);
			} catch (BadLocationException e) {
				e.printStackTrace();
				this.startPosition = this.endPosition = null;
			} 
		}

		/**
		 * Function run
		 */
		public void run() {
			if (scilabDocument.getAutoColorize() && (startPosition != null) && (endPosition != null || this.endOffset != -1)) {
				int startOffset = startPosition.getOffset();
				int endOffset = this.endOffset != -1 ? this.endOffset : endPosition.getOffset();
				colorize(scilabDocument, startOffset, 
						 java.lang.Math.min(endOffset, scilabDocument.getLength()));				
			}
		}
	}
	
	/**
	 * Function chrono
	 * @param str string
	 */
	private void chrono(String str) {
		System.err.println(str + ":" + (System.currentTimeMillis() - millis));
		millis = System.currentTimeMillis();
	}

	/**
	 * Parse all Scilab keywords
	 * This function is used for the syntactic colorization
	 * @param scilabDocument document
	 * @param start document start
	 * @param end document end
	 * @return ArrayList
	 */
	private ArrayList<ArrayList<Integer>> parse(ScilabStyleDocument scilabDocument, int start, int end) {
		//System.err.println("parse start"+start+" end:"+end);
		//	    	Timer timer = new Timer();
		ArrayList<ArrayList<Integer>>  boundaries_list = null;
		ArrayList<Integer> boolsBoundaries, commandsBoundaries, 
		commentsBoundaries, functionsBoundaries, 
		macrosBoundaries, operatorsBoundaries, 
		quotationsBoundaries;
		try {
			millis = System.currentTimeMillis();
			String text = scilabDocument.getText(start, end - start);	
			boolsBoundaries = findBoundaries(boolsPattern, start, text);
			//chrono("findBoundaries bools:");
			commandsBoundaries = findBoundaries(commandsPattern, start, text);
			//chrono("findBoundaries commands:");
			commentsBoundaries = findBoundaries(commentsPattern, start, text);
			//chrono("findBoundaries comments:");	
			functionsBoundaries = findBoundaries(functionsPattern, start, text);
			//chrono("findBoundaries functions:");
			macrosBoundaries = findBoundaries(macrosPattern, start, text);
			//chrono("findBoundaries macros:");
			//macrosBoundaries = new ArrayList<Integer>();
			operatorsBoundaries = findBoundaries(operatorsPattern, start, text);
			//chrono("findBoundaries operators:");
			quotationsBoundaries = findBoundaries(quotationsPattern, start, text);	
			//chrono("findBoundaries quotations:");
			//DEBUG("parse -> findAllBoundaries "+timer.top());
			boundaries_list = organizeBoundaries(boolsBoundaries, commandsBoundaries, commentsBoundaries, functionsBoundaries, 
					macrosBoundaries, operatorsBoundaries, quotationsBoundaries);
			//chrono("organizeBoundaires:");
			//DEBUG("parse -> organizeBoundaries "+timer.top());
		} catch (Exception e) {
			e.printStackTrace(); 
		} finally {
			return boundaries_list;
		}
	}


	/**
	 * Get start & end position for each keywords
	 * String[] keyword is a type of keywords(ex: operators, commands, macros..)
	 * String text is where we make the search, 
	 * if text is null we apply the research to the entire document
	 * @param keyword keyword boundaries
	 * @param start start boundaries
	 * @param text string
	 * @return ArrayList
	 */
	private ArrayList<Integer> findBoundaries(String[] keyword, int start, String text) {
		//Timer timer = new Timer();
		//DEBUG("   findBoundaries start : " + timer.top());

		Pattern pattern;
		ArrayList<Integer> bound = new ArrayList<Integer>();

		for (int i = 0; i < keyword.length; i++) {
			pattern = Pattern.compile(keyword[i], Pattern.DOTALL);
			Matcher matcher = pattern.matcher(text);
			while (matcher.find()) {
				//System.err.println("Match Found : "+(matcher.start())+","+(matcher.end()/*-matcher.start()*/));
				bound.add(new Integer(matcher.start() + start));	
				bound.add(new Integer(matcher.end() + start));
			}
		}

		//DEBUG("   findBoundaries end : " + timer.top());
		return bound;
	}

	/**
	 * Get start & end position for each keywords
	 * Pattern keyword is a type of keywords(ex: operators, commands, macros..)
	 * String text is where we make the search, 
	 * if text is null we apply the research to the entire document
	 * @param keywords keyword boundaries
	 * @param start start boundaries
	 * @param text string
	 * @return ArrayList
	 */
	private ArrayList<Integer> findBoundaries(Pattern keywords, int start, String text) {
		//Timer timer = new Timer();
		//DEBUG("   findBoundaries start : " + timer.top());
		ArrayList<Integer> bound = new ArrayList<Integer>();
		//System.err.println("findBoundaries:"+keywords);
		Matcher matcher = keywords.matcher(text);
		while (matcher.find()) {
			//System.err.println("Match Found : "+(matcher.start())+","+(matcher.end()/*-matcher.start()*/));
			bound.add(new Integer(matcher.start() + start));	
			bound.add(new Integer(matcher.end() + start));
		}


		//DEBUG("   findBoundaries end : " + timer.top());
		return bound;
	}

	/**
	 * When we have all boundaries for each type of keywords
	 * we filter 'bad' boundaries
	 * ex: if we have quotations into comments (and the opposite), keywords into quotations or comments 
	 * @param boolsBoundaries boolean keywords
	 * @param commandsBoundaries command keywords
	 * @param commentsBoundaries comment keywords
	 * @param functionsBoundaries function keywords
	 * @param macrosBoundaries macros keywords
	 * @param operatorsBoundaries operator keywords
	 * @param quotationsBoundaries quotation keywords
	 * @return ArrayList
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
	
	/**
	 * Compare if vector1 is in vector2
	 * @param v1 vector
	 * @param v2 vector
	 * @return ArrayList
	 */
	private ArrayList<Integer> strictlyNotIn(ArrayList<Integer> v1, ArrayList<Integer> v2) {
		int v1_start = 0;
		int v1_end = 0;
		int v2_start = 0;
		int v2_end = 0;

		ArrayList<Integer> vector_strictlyNotIn = new ArrayList<Integer>();

		// Remove interval from v1 which are include in interval of v2
		for (int i = 0; i < v1.size(); i = i + 2) {
			boolean dropMe = false;
			v1_start = v1.get(i);
			v1_end = v1.get(i + 1);

			for (int j = 0; j < v2.size(); j = j + 2) {
				v2_start = v2.get(j);
				v2_end = v2.get(j + 1);

				if (((v1_start >= v2_start) && (v1_start <= v2_end)) && ((v1_end >= v2_start) && (v1_end <= v2_end))) {
					dropMe = true;
				}
			}
			if (!dropMe) {
				vector_strictlyNotIn.add(v1.get(i));
				vector_strictlyNotIn.add(v1.get(i + 1));
			}

			//System.out.println("vector_strictlyNotIn"+vector_strictlyNotIn);
		}
		return vector_strictlyNotIn;
	}

	/**
	 * Compare if start occurences are not in
	 * @param v1 vector
	 * @param v2 vector
	 * @return ArrayList
	 */
	private ArrayList<Integer> startNotIn(ArrayList<Integer> v1, ArrayList<Integer> v2) {
		int v1_start = 0;
		int v2_start = 0;
		int v2_end = 0;
		ArrayList<Integer> vector_startNotIn = new ArrayList<Integer>();

		for (int i = 0; i < v1.size(); i = i + 2) {
			boolean dropMe = false;
			v1_start = v1.get(i);

			for (int j = 0; j < v2.size(); j = j + 2) {
				v2_start = v2.get(j);
				v2_end = v2.get(j + 1);

				if (((v1_start >= v2_start) && (v1_start <= v2_end))) {
					dropMe = true;
				}
			}
			if (!dropMe) {
				vector_startNotIn.add(v1.get(i));
				vector_startNotIn.add(v1.get(i + 1));
			}

			//System.out.println("vector_startNotIn"+vector_startNotIn);
		}
		return vector_startNotIn;
	}

}
