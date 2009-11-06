package org.scilab.modules.xpad.style;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.event.DocumentEvent;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;

import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.actions.ColorizeAction;

import com.sun.org.apache.xpath.internal.compiler.Keywords;

/**
 * This class manages the Colorization aspect
 */
public class ColorizationManager {
	private volatile boolean autoColorize       = true;
	private volatile boolean colorizeInprogress;
	private String[] commands;
	private String[] functions;
	private String[] macros;
	private boolean singleLine = false;
	private int lineStartPosition;
	private int lineEndPosition;
	private int currentLine;
	private final int BOOLS = 0;
	private final int COMMANDS = 1;
	private final int COMMENTS = 2;
	private final int FUNCTIONS = 3;
	private final int MACROS = 4;
	private final int OPERATORS = 5;
	private final int QUOTATIONS = 6;

	
		public ColorizationManager() {
			// Scilab keywords to be colored
			KeywordManager keywordManager = new KeywordManager();
			Hashtable<String, String[]> keywords = keywordManager.getScilabKeywords();    	
			commands = (String[]) keywords.get("command");
			functions = (String[]) keywords.get("function");
			macros = (String[]) keywords.get("macro");

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
		
		public boolean isAutoColorize() {
			return autoColorize;
		}

		public void setAutoColorize(boolean autoColorize) {
			this.autoColorize = autoColorize;
		}

	/**
	 * @return if we are colorizing or not
	 */
	public boolean getColorize() {
		return autoColorize;
	}

	public void setColorize(boolean b) {
		//DEBUG("setColorize("+b+")");
		autoColorize = b;
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
	
	/**
	 * DOCUMENT COLORISATION START
	 */
	public void loadingsForColorisation() {
	}
	
	

	public boolean colorize(ScilabStyleDocument scilabDocument, int lineStartPosition, int lineEndPosition) {
	    //DEBUG("--> Calling colorize("+lineStartPosition+", "+lineEndPosition+")");	
	    //Timer timer = new Timer();
		//DEBUG("Colorize [before parse] : " + timer.top());
		singleLine = false;
		
		// We parse all words which are susceptible to be colored
		ArrayList<ArrayList<Integer>> boundaries_list = 
			this.parse(scilabDocument, KeywordManager.getBools(), 
					commands,
					KeywordManager.getComments(), 
					functions, 
					macros, 
					KeywordManager.getOperators(), 
					KeywordManager.getQuotations(), 
					lineStartPosition, 
					lineEndPosition);
		//DEBUG("Colorize [after parse] : " + timer.top());
		if (!colorizeInprogress) {
			//colorizeInprogress = true;
			HistoryManager historyManager = new HistoryManager();
			scilabDocument.removeUndoableEditListener(historyManager.getUndo());
			scilabDocument.addUndoableEditListener(null);
			resetStyle(scilabDocument, lineStartPosition, lineEndPosition);
			try {
				applyStyle(scilabDocument, boundaries_list.get(BOOLS), scilabDocument.getStyle("Bool"));
				applyStyle(scilabDocument, boundaries_list.get(COMMANDS), scilabDocument.getStyle("Command"));
				applyStyle(scilabDocument, boundaries_list.get(FUNCTIONS), scilabDocument.getStyle("Function"));
				applyStyle(scilabDocument, boundaries_list.get(MACROS), scilabDocument.getStyle("Macro"));
				applyStyle(scilabDocument, boundaries_list.get(OPERATORS), scilabDocument.getStyle("Operator"));
				applyStyle(scilabDocument, boundaries_list.get(QUOTATIONS), scilabDocument.getStyle("String"));
				applyStyle(scilabDocument, boundaries_list.get(COMMENTS), scilabDocument.getStyle("Comment"));
			} catch (BadLocationException e) {
				e.printStackTrace();
			} finally {
				scilabDocument.addUndoableEditListener(scilabDocument.getUndoManager());
				//colorizeInprogress = false;
			}
		}
//		DEBUG("Colorize [after all applyStyle] : " + timer.top());
	
		return true;
	}
	
	private void resetStyle(ScilabStyleDocument scilabDocument, int line_start, int line_end) {
//    	DEBUG("resetStyle("+line_start+", "+line_end+")");
	// Reset Color
		scilabDocument.setCharacterAttributes(line_start, line_end-line_start, scilabDocument.getStyle("Default"), false);
	}
	
	private void applyStyle(ScilabStyleDocument scilabDocument, ArrayList<Integer> boundaries, Style style) throws BadLocationException {
		for (int i = 0; i < boundaries.size(); i = i + 2) {
			scilabDocument.setCharacterAttributes(boundaries.get(i), boundaries.get(i+1)-boundaries.get(i), style, false);
		}
	}

	public class ColorUpdater implements Runnable {
	    private DocumentEvent event;
	    private ScilabStyleDocument scilabDocument;
	    
	    public ColorUpdater(ScilabStyleDocument scilabDocument, DocumentEvent event) {
	    	super();
	    	this.event = event;
	    }
	    
	    public void run() {
			//colorize();
			ColorizeAction.getXpadEditor();
			/* @TODO CHECK THAT */
			Xpad editor = scilabDocument.getEditor();
			javax.swing.text.StyledDocument doc = editor.getTextPane().getStyledDocument();
			synchronized (doc) {
				if (event == null) { // Called from SetFontAction: apply change to the whole document
					lineStartPosition =  0;
					lineEndPosition = scilabDocument.getLength();
				} else {
					lineStartPosition =  doc.getParagraphElement(event.getOffset()).getStartOffset();
					lineEndPosition = doc.getParagraphElement(event.getOffset() + event.getLength()).getEndOffset() - 1;
				}
			
				if (lineStartPosition != lineEndPosition) {
					colorize(scilabDocument, lineStartPosition, lineEndPosition);
				}
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
	/*
	 * Parse all Scilab keywords
	 * This function is used for the syntactic colorization
	 */
	private ArrayList<ArrayList<Integer>> parse(ScilabStyleDocument scilabDocument, String[] bools, String[] commands, String[] comments, 
			String[] functions, String[] macros, String[] operators, String[] quotations, int start, int end) {

//	    	Timer timer = new Timer();
	    	ArrayList<ArrayList<Integer>>  boundaries_list = null;
	    	ArrayList<Integer> boolsBoundaries, commandsBoundaries, 
		commentsBoundaries, functionsBoundaries, 
		macrosBoundaries, operatorsBoundaries, 
		quotationsBoundaries;
		try {
		boolsBoundaries = findBoundaries(bools, start, scilabDocument.getText(start, end - start));
		commandsBoundaries = findBoundaries(commands, start, scilabDocument.getText(start, end - start));
		commentsBoundaries = findBoundaries(comments, start, scilabDocument.getText(start, end - start));
		functionsBoundaries = findBoundaries(functions, start, scilabDocument.getText(start, end - start));
		macrosBoundaries = findBoundaries(macros, start, scilabDocument.getText(start, end - start));
		//macrosBoundaries = new ArrayList<Integer>();
		operatorsBoundaries = findBoundaries(operators, start, scilabDocument.getText(start, end - start));
		quotationsBoundaries = findBoundaries(quotations, start, scilabDocument.getText(start, end - start));	

		//DEBUG("parse -> findAllBoundaries "+timer.top());
		boundaries_list = organizeBoundaries(boolsBoundaries, commandsBoundaries, commentsBoundaries, functionsBoundaries, 
				macrosBoundaries, operatorsBoundaries, quotationsBoundaries);
		//DEBUG("parse -> organizeBoundaries "+timer.top());
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
	    //Timer timer = new Timer();
	    //DEBUG("   findBoundaries start : " + timer.top());
	    
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
		
		//DEBUG("   findBoundaries end : " + timer.top());
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

		for (int i=0; i < v1.size(); i=i+2) {
			boolean dropMe = false;
			v1_start = v1.get(i);

			for (int j=0; j < v2.size(); j=j+2) {
				v2_start = v2.get(j);
				v2_end = v2.get(j+1);

				if (((v1_start >= v2_start) && (v1_start <= v2_end))) {
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
	
}
