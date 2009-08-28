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

public class ScilabStyleDocument extends DefaultStyledDocument implements DocumentListener {
    String sb = "";
    int startPoint = 0;

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
    
    private final String[] operators = {"==", "<", ">", "<=", ">=", "\\+", "-", "\\*", "/", "\\\\", 
    									"=", "\\+=", "-=", "\\*=", "/=", "\\++", "--", "!=", "~=", 
    									"\\||", "&&", "!", "&", "\\|", "\\^", "<<", ">>", ">>>>"};
    
    //'controls' is now under the name of 'commands'
    //private String[] controls = {"if ", "while ", "for ", " then", "else", "do", "case ", "end", "function", "endfunction"};
    private String[] commands;
    private String[] functions;
    private String[] macros;
    private final String[] strings = {"(\"|')[^{\n}]*?(\"|')"};
    private final String[] bools = {"%T", "%F", "%t", "%f"};
    private final String[] comments = {"//[^{\n}]*\n", "/\\*.*?\\*/"};

    //Warning operators should be define in this order!
    private final String inInstruction = "elseif|else|if|while|for|do|function";
    private final String outInstruction = "endfunction|end";

    private final String IN = "IN";
    private final String OUT = "OUT";
    private final String TABULATION = "  ";
    
    private Style defaultStyle;
    private Style operatorStyle;
    private Style commandStyle;
    private Style stringStyle;
    private Style boolStyle;
    private Style commentStyle;
    private Style functionStyle;
    private Style macroStyle;


    private final void DEBUG(String msg) {
	System.err.println("[DEBUG] "+msg);
    }

    public ScilabStyleDocument() {
	super();
	addDocumentListener(this);
	addUndoableEditListener(undo);
	/*Style */defaultStyle = this.addStyle("Default", null);
	StyleConstants.setBold(defaultStyle, false);
	StyleConstants.setForeground(defaultStyle, Color.BLACK);
	StyleConstants.setFontSize(defaultStyle, 16);
	StyleConstants.setLeftIndent(defaultStyle, 0);

	/*Style */operatorStyle = this.addStyle("Operator", defaultStyle);
	StyleConstants.setBold(operatorStyle, true);
	StyleConstants.setForeground(operatorStyle, Color.RED);

	/*Style */commandStyle = this.addStyle("Command", defaultStyle);
	StyleConstants.setBold(commandStyle, true);
	StyleConstants.setForeground(commandStyle, Color.CYAN);

	/*Style */stringStyle = this.addStyle("String", defaultStyle);
	StyleConstants.setBold(stringStyle, true);
	StyleConstants.setForeground(stringStyle, Color.BLUE);

	/*Style */boolStyle = this.addStyle("Bool", defaultStyle);
	StyleConstants.setBold(boolStyle, true);
	StyleConstants.setForeground(boolStyle, Color.YELLOW);

	/*Style */commentStyle = this.addStyle("Comment", defaultStyle);
	StyleConstants.setBold(commentStyle, true);
	StyleConstants.setForeground(commentStyle, Color.GREEN);
	
	/*Style */functionStyle = this.addStyle("Function", defaultStyle);
	StyleConstants.setBold(functionStyle, true);
	StyleConstants.setForeground(functionStyle, Color.PINK);
	
	/*Style */macroStyle = this.addStyle("Macro", defaultStyle);
	StyleConstants.setBold(macroStyle, true);
	StyleConstants.setForeground(macroStyle, Color.ORANGE);
	
    }

    public void indent(int start, int end) {
	if (!indentInprogress) {
	    indentInprogress = true;
	    resetStyle();
	    try {
		//Get the correct starting line to indent
		int[] interval = new int[2];
		interval = readText(start, end);
		//Here start the text indentation
		applyIndent(interval[0], interval[1]);
	    } catch (BadLocationException e) {
		e.printStackTrace();
	    }
	}
	indentInprogress = false;
    }

    public void applyIndent(int start, int end) throws BadLocationException {

	int startOffset;
	int endOffset;
	String textLine = "";
	Vector<String> opList;
	String indentedText = "";
	String tab = "";
	boolean opMatch = false;

	//We read the document
	for (int i = start; i < end;) {
	    //Get start & end position for a line
	    startOffset = this.getParagraphElement(i).getStartOffset();
	    endOffset = this.getParagraphElement(i).getEndOffset();

	    try {
		//Get the document line by line (start position, line size)
		textLine = this.getText(startOffset, endOffset - startOffset);
	    } catch (BadLocationException e) {
		e.printStackTrace();
	    }
	    //Step to next line
	    i = endOffset;

	    //Remove space(s) located at the beginning of the given line
	    textLine = removeFirstSpace(textLine);

	    //Get operators for a given line
	    opList = getOperatorList(textLine);

	    //Check if in one line all operators are matching,
	    //so we can know if the next line needs indentation or not
	    //ex: if %T then function foo(1) endfunction end => doesn't need indentation
	    opMatch = matchingOperators(opList);

	    //Operator found in the given line
	    if (opList.size() > 0) {
		//If we have 'IN' operator
		if (opList.elementAt(0).equals(IN) && opMatch == false) {
		    //No indentation in case of 'else' or 'elseif'
		    if ((opList.elementAt(1).toLowerCase().equals("else")) || (opList.elementAt(1).toLowerCase().equals("elseif"))) {
			if (indentedText.length() >= 2) {
			    indentedText = indentedText.substring(0, indentedText.length()-2);
			}
		    } else {
			tab += TABULATION;
		    }
		    indentedText += textLine;

		    //If we have "OUT' operator
		} else if (opList.elementAt(0).equals(OUT)) {
		    if (indentedText.length() >= 2 && tab.length() >= 2) {
			indentedText = indentedText.substring(0, indentedText.length()-2);
			tab = tab.substring(0, tab.length()-2);
		    }
		    indentedText += textLine;

		    //Line got operators and they match, so no need of indentation
		} else {
		    indentedText += textLine;
		}
		//Line without operator
	    } else {
		indentedText += textLine;
	    }

	    //Add the indentation
	    indentedText += tab;

	    opList.clear();
	    opMatch = false;
	}

	//Remove the last line
	indentedText = indentedText.trim();
	//Display the indentation
	this.replace(start, end-start, indentedText, null);

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
    }
    
    public void colorize() {
    	//Get all Scilab keywords to be colorized
    	Hashtable<String, String[]>keywords = getScilabKeywords();
    	
    	commands = (String[])keywords.get("command");
    	functions = (String[])keywords.get("function");
    	macros = (String[])keywords.get("macro");
    	
    	//We mark the boundary of each regexp (for commands, functions & macros) 
    	for (int i = 0; i < commands.length; i++) {
    		commands[i] = "\\b" + commands[i] + "\\b"; 
		}
    	
    	for (int i = 0; i < functions.length; i++) {
    		functions[i] = "\\b" + functions[i] + "\\b"; 
		}
    	
    	for (int i = 0; i < macros.length; i++) {
    		macros[i] = "\\b" + macros[i] + "\\b"; 
		}
    	
	if (!colorizeInprogress) {
	    colorizeInprogress = true;
	    this.removeUndoableEditListener(undo);
	    this.addUndoableEditListener(null);
	    resetStyle();
	    try {
	    applyStyle(macros, getStyle("Macro"));	
	    applyStyle(functions, getStyle("Function"));
		applyStyle(operators, getStyle("Operator"));
		applyStyle(commands, getStyle("Command"));		
		applyStyle(strings, getStyle("String"));
		applyStyle(bools, getStyle("Bool"));
		applyStyle(comments, getStyle("Comment"));
	    } catch (BadLocationException e) {
		// TODO Auto-generated catch block
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
    
    /*
     * Colorize <pattern> with <style>
     */
    private void applyStyle(String[] words, Style style) throws BadLocationException {
    	for(int i = 0 ; i < words.length ; i++)
    	{
    		Pattern pattern = Pattern.compile(words[i], Pattern.MULTILINE);
    		Matcher matcher = pattern.matcher(this.getText(0, this.getLength()));

    		while(matcher.find()) {
    			//DEBUG("Apply Style : "+style.getName());
    			//DEBUG("Match Found : "+(matcher.start())+","+( matcher.end()-matcher.start()));
    			//DEBUG("Text : "+this.getText(matcher.start(), matcher.end()-matcher.start()));
    			this.setCharacterAttributes(matcher.start(), matcher.end()-matcher.start(), style, false);
    			this.setParagraphAttributes(matcher.start(), matcher.end()-matcher.start(), style, false);
    		}
    	}
    }

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
    public void changedUpdate(DocumentEvent e) {
	// TODO Auto-generated method stub
//	System.err.println("Calling changedUpdate "+e.toString());
//	SwingUtilities.invokeLater(new Runnable() {
//	    public void run() {
//		//colorize(); 
//	    }
//	});
    }

    public void insertUpdate(DocumentEvent e) {
	// TODO Auto-generated method stub
	//System.err.println("Calling insertUpdate");
	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		//resetStyle();
		if (autoIndent) {
		    //indent();
		}
		if (autoColorize) {
		    colorize();
		}
	    }
	});  
    }

    public void removeUpdate(DocumentEvent e) {
	//System.err.println("Calling removeUpdate");
	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		//resetStyle();
		if (autoIndent) {
		    //indent();
		}
		if (autoColorize) {
		    colorize(); 
		}
	    }
	});      
    }

    public void setAutoIndent(boolean b) {
	//DEBUG("setAutoIndent("+b+")");
	autoIndent = b;
    }
  
    public boolean getAutoIndent() {
	//DEBUG("getAutoIndent("+autoIndent+")");
	return autoIndent;
    }
    
    public void setColorize(boolean b) {
	//DEBUG("setColorize("+b+")");
	autoColorize = b;
    }
    
    public boolean getColorize() {
	//DEBUG("setColorize("+autoColorize+")");
	return autoColorize;
    }

    public UndoManager getUndoManager() {
	return undo;
    }

    public Vector<String> getOperatorList(String text) {

	//If the text is a comment we don't indent
	text = manageTextWithComment(text);
	
	String toto = manageQuotationText(text);

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

	return v;
    }

    public String removeFirstSpace(String line) {
	int del = 0;

	for (int j = 0; j < line.length(); j++) {
	    if ((line.charAt(j) == '\t') || (line.charAt(j) == ' ')) {
		del++;
	    } else {
		break;
	    }				
	}			
	line = line.substring(del, line.length());			
	del = 0;

	return line;
    }

    public boolean matchingOperators(Vector<String> opList) {

	Vector<String> inOP = new Vector<String>();
	Vector<String> outOP = new Vector<String>();
	boolean opMatch = false;

	for (int j = 0; j < opList.size()/2; j++) {
	    if (opList.elementAt(j).equals(IN)) {
		inOP.add(opList.elementAt(opList.size()/2 + j));
	    }
	    if (opList.elementAt(j).equals(OUT)) {
		outOP.add(opList.elementAt(opList.size()/2 + j));
	    }
	}

	Collections.reverse(outOP);

	if (inOP.size() == outOP.size()) {
	    for (int j = 0; j < inOP.size(); j++) {

		if (inOP.elementAt(j).toLowerCase().equals("function")) {
		    if (outOP.elementAt(j).toLowerCase().equals("endfunction")) {
			opMatch = true;
		    } else {
			opMatch = false;
		    }
		} else {
		    if (outOP.elementAt(j).toLowerCase().equals("end")) {
			opMatch = true;							
		    } else {
			opMatch = false;
		    }
		}
	    }
	}
	return opMatch;
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
    
    /*******************************************************************************************************************/
    
    private String cleanText;
    private int[] quotationsBoundaries, commentsBoundaries, keywordsBoundaries;
    
    public String manageQuotationText(String text) {
    	cleanText = text;
    	
    	String noQuotationText = null;
    	
    	Pattern[] quotationsPatterns= new Pattern[1];
    	Pattern[] commentsPatterns= new Pattern[1];
    	Pattern[] keywordsPatterns= new Pattern[1];
    	quotationsPatterns[0]= Pattern.compile("[\'\"][^\'\"]*[\'\"]", Pattern.DOTALL);
    	commentsPatterns[0]= Pattern.compile("//[^\n]*", Pattern.DOTALL);
    	keywordsPatterns[0]= Pattern.compile("\\bif\\b", Pattern.DOTALL);
    	parse(quotationsPatterns, commentsPatterns, keywordsPatterns);
    	
    	return noQuotationText;
    }
    
    public void parse(Pattern[] quotations, Pattern[] comments, Pattern[] keywords){
    	System.err.println("=================================== quotations: ===========================");
    	quotationsBoundaries= findNotIn(quotations, null);
    	System.err.println("=================================== comments: ===========================");
    	commentsBoundaries= findNotIn(comments, quotationsBoundaries);
    	System.err.println("=================================== keywords: ===========================");
    	keywordsBoundaries= findNotIn(keywords, concatenateArrays(quotationsBoundaries, commentsBoundaries));
    	//System.arraycopy(quotationsBoundaries, 0, quotationsBoundaries, 0, 0);
    }
    
     private void OrganizeBoundaries(Vector<Integer> quotationsBoundaries, Vector<Integer> commentsBoundaries, Vector<Integer>keywordsBoundaries) {

    	//Quotation into comment
    	for(int i=0; i < commentsBoundaries.size(); i=i+2){

    		for(int j=0; j < quotationsBoundaries.size(); j=j+2){
    			
    			if((quotationsBoundaries.elementAt(j) > commentsBoundaries.elementAt(i)) && (quotationsBoundaries.elementAt(j) < commentsBoundaries.elementAt(i+1))) {
    				if(quotationsBoundaries.elementAt(j+1) > commentsBoundaries.elementAt(i) && quotationsBoundaries.elementAt(j+1) < commentsBoundaries.elementAt(i+1)) {
    					quotationsBoundaries.removeElementAt(j);
    					quotationsBoundaries.removeElementAt(j+1);
    				}
    			}
    		}
    	}

    	System.err.println("----- QUOTATIONS: ----- ");	
    	for(int i=0; i < quotationsBoundaries.size(); ++i){
    		System.err.println("quotation: " + quotationsBoundaries.elementAt(i));
    	}

    	System.err.println("----- COMMENTS: ----- ");	
    	for(int i=0; i < commentsBoundaries.size(); ++i){
    		System.err.println("comments: " + commentsBoundaries.elementAt(i));
    	}

    	System.err.println("----- KEYWORDS: ----- ");	
    	for(int i=0; i < keywordsBoundaries.size(); ++i){
    		System.err.println("keywords: " + keywordsBoundaries.elementAt(i));
    	}

    }
    
    
    
    
    
    public int[] concatenateArrays(int[]a, int[]b){
    	int[]res= new int[a.length+b.length];
    	int i=0;
    	for(int j=0; j!= a.length; ++j, ++i){
    		res[i]=a[j];
    	}
    	for(int j=0; j!= b.length; ++j, ++i){
    		res[i]=b[j];
    	}
    	return res;
    }

    private int[] findNotIn(Pattern[] targets, int[] exclusionBoundaries){
    	Vector tmpB= new Vector();
    	for(int i=0; i!= targets.length; ++i){
    		Matcher matcher = targets[i].matcher(cleanText);
    		while(matcher.find()){
    			System.err.println("Match Found : "+(matcher.start())+","+( matcher.end()-matcher.start())+" : |" +
    								cleanText.substring(matcher.start(),matcher.end())+"|");
    			
    			if(exclusionBoundaries.length > 1){
    				for (int j = 0; j < exclusionBoundaries.length; j++) {
    					System.out.println("^^^^^exclusionBoundaries: " + exclusionBoundaries[j]);
    				}
    			}
    			
    			if(notIn(matcher.start(), exclusionBoundaries) && notIn(matcher.end(), exclusionBoundaries)){
    				tmpB.add(new Integer(matcher.start()));	
    				tmpB.add(new Integer(matcher.end()));
    			}
    		}
    	}
    	return arrayFromVector(tmpB);
    }
    // to be more efficient, we should keep boundaries sorted after concatenete and use dichotomy to find gretest begin < pos
    // notIn iff corresponding end <  pos
    private boolean notIn(int pos, int[] boundaries){
    	if(boundaries == null) return true;
    	for(int i=0; i!= boundaries.length; i+=2){
    		if ((pos > boundaries[i]) && (pos< boundaries[i+1])){
    			return false;
    		}
    	}
    	return true;
    }

    private int[] arrayFromVector(Vector v){
    	int[] res= new int [v.size()];
    	for(int i=0; i!= res.length; ++i){
    		res[0]= ((Integer)v.elementAt(i)).intValue();
    	}
    	return res;
    }

    
    /***************************************************************************************************************************/
    
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

}
