package org.scilab.modules.xpad.style;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.event.DocumentEvent;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

public class IndentManager {
	private volatile boolean autoIndent         = true;
	private volatile boolean indentInprogress   = false;
	private String currentStringIndent = "";
	private int currentLevelIdent = 0;
	private Pattern patternSpace;
	private Pattern patternIn;
	private Pattern patternInOut;
	private Pattern patternOut;
	private Pattern patternComment;
	private Pattern patternQuote;

public IndentManager() {
	patternIn = Pattern.compile("(\\b(if|while|for|select|function|try)\\b)"); // do should change nothing to indent
	patternInOut = Pattern.compile("\\b(else|elseif|case|catch)\\b");
	patternOut = Pattern.compile("\\b(end|endfunction)\\b");
	patternComment = Pattern.compile("(.*?)//");
	patternQuote = Pattern.compile("(\"|')[^\\n]*?(\"|')");
	
	patternSpace = Pattern.compile("\\s*");
}
	

	public boolean isIndentInprogress() {
		return indentInprogress;
	}

	public void setIndentInprogress(boolean indentInprogress) {
		this.indentInprogress = indentInprogress;
	}


	/**
	 * DOCUMENT INDENTATION START
	 */
	public void indent(ScilabStyleDocument scilabDocument, int startPosition, int endPosition) {
		if (!indentInprogress) {
			scilabDocument.disableUndoManager();
			
			indentInprogress = true;
			//resetStyle(startPosition, startPosition);
			try {
				applyIdent_trueone(scilabDocument, startPosition, endPosition);
			} catch (BadLocationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//applySelectionIndent();
			
			indentInprogress = false;
			scilabDocument.enableUndoManager();
		}
	}

	public String indentLine (String lineToIndent, String baseSpaces) {
		
		 Matcher matcherSpace = patternSpace.matcher(lineToIndent);
		 if (matcherSpace.find()) {
			 lineToIndent = lineToIndent.replaceFirst(matcherSpace.group(), "");
		 }
		 
		 
		Matcher matcherComment = patternComment.matcher(lineToIndent);
		String lineWithoutComment = lineToIndent;
		
		if (matcherComment.find()) {
			lineWithoutComment = matcherComment.group(1);
		}
		Matcher matcherQuote = patternQuote.matcher(lineWithoutComment);
		
		while (matcherQuote.find()) {
			lineWithoutComment = matcherQuote.replaceAll("");
		}

		Matcher matcherIn = patternIn.matcher(lineWithoutComment);
		Matcher matcherInOut = patternInOut.matcher(lineWithoutComment);	
		Matcher matcherOut = patternOut.matcher(lineWithoutComment);
		
		// if it's a middle keyword we remove a tab only for this line and then return to normal indent
		if ( matcherInOut.find(0)){
			currentStringIndent = currentStringIndent.replaceFirst(TabManager.getTabulation(), "");
			lineToIndent = baseSpaces + currentStringIndent +lineToIndent;
			currentStringIndent += TabManager.getTabulation();
			
		// if close keyword we remove a tab 
		}else if (matcherOut.find(0)) {
			currentStringIndent = currentStringIndent.replaceFirst(TabManager.getTabulation(), "");
			lineToIndent = baseSpaces + currentStringIndent +lineToIndent;
		}else {
			lineToIndent = baseSpaces + currentStringIndent +lineToIndent;
		}

		// we make the difference of open/close keywords of the line we've just indent to know
		// how to indent the next one 
		while (matcherIn.find()){
			currentStringIndent += TabManager.getTabulation();
		}
		while (matcherOut.find()){
			currentStringIndent = currentStringIndent.replaceFirst(TabManager.getTabulation(), "");
		}
		//System.out.println(lineToIndent);
		//System.out.println(indentLevel);
		
		 //lineToIndent + System.getProperty("line.separator");
		
		
		return lineToIndent;
	}
	
	public synchronized void applyIdent_trueone (ScilabStyleDocument scilabDocument, int startPosition, int endPosition) throws BadLocationException {
		
		//System.out.println(startPosition);
		//System.out.println(endPosition);
		String previousSpace = "";
		String currentSpace ="";
		String previousLineContent = "";
		
		int currentLineStart = scilabDocument.getParagraphElement(startPosition).getStartOffset();
		int currentLineLength =  scilabDocument.getParagraphElement(startPosition).getEndOffset() - currentLineStart - 1;
		String currentLineContent = scilabDocument.getText(currentLineStart, currentLineLength);
		
		/* compute number of space characters in the previous line */
		if (startPosition > 1){
			int previousLineStart = scilabDocument.getParagraphElement(startPosition-currentLineLength -1).getStartOffset();
			int previousLineLength =  scilabDocument.getParagraphElement(startPosition-currentLineLength - 1).getEndOffset()-previousLineStart-1;
			
			previousLineContent = scilabDocument.getText(previousLineStart, previousLineLength);
			
			Pattern patternSpace = Pattern.compile("\\s*"); 
			Matcher matcherSpace = patternSpace.matcher(previousLineContent);
			
			if (matcherSpace.find()){
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
			int start = scilabDocument.getParagraphElement(endPosition).getStartOffset();
			int length = scilabDocument.getParagraphElement(endPosition).getEndOffset() - start;
			nextLineContent = scilabDocument.getText(start, length-1);
		}
		Pattern patternIn = Pattern.compile("(\\b(if|while|for|select|function|try)\\b)"); // do should change nothing to indent	
		Pattern patternInOut = Pattern.compile("\\b(else|elseif|case|catch)\\b");
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
			
				scilabDocument.replace(currentLineStart, currentSpace.length(),previousSpace.replaceFirst(TabManager.getTabulation(), "") , null);
				endPosition -= (currentSpace.length() - previousSpace.replaceFirst(TabManager.getTabulation(), "").length() );
				currentSpace += TabManager.getTabulation();
			} else {
				if (currentSpace.replaceFirst(TabManager.getTabulation(), "").length() ==   previousSpace.length())  {
					scilabDocument.replace(currentLineStart, currentSpace.length(),previousSpace , null);
					endPosition -= (currentSpace.length() - previousSpace.length() );
					
				}
					previousSpace += TabManager.getTabulation();
			}
			
			
			/* strcuture keyword which are supposed to close a structure (end, endfunction ...  ) */
		}else if (  matcherOut.find(0) ){
			scilabDocument.replace(currentLineStart, currentSpace.length(),previousSpace.replaceFirst(TabManager.getTabulation(), "") , null);
			endPosition -= (currentSpace.length() - previousSpace.replaceFirst(TabManager.getTabulation(), "").length() );
			currentLevelIdent --;
			
		}else {

			//this.insertString(endPosition, currentSpace , null);
		}
		

		
		// update next line
		// more open keywords than close ones
		if ( i > 0 ){
			for (int j =0; j < i; j++){
				tabToAdd += TabManager.getTabulation();
			}
			scilabDocument.insertString(endPosition, tabToAdd + currentSpace , null);
		// less open keywords than close ones	
		}else if (i < 0 ) {
			for (int j =  0; j > i; j--){
				previousSpace =  previousSpace.replaceFirst(TabManager.getTabulation(), "");
			}
			scilabDocument.insertString(endPosition, previousSpace , null);
		// no open/close keyword
		}else {
			if (!matcherInOut.find(0)){
				scilabDocument.insertString(endPosition, currentSpace , null);
			}else{
				scilabDocument.insertString(endPosition, previousSpace , null);
			}
		
		}
	}
	
	public class IndentUpdater implements Runnable {
	    private DocumentEvent event = null;
	    private ScilabStyleDocument scilabDocument;
	    
	    public IndentUpdater(ScilabStyleDocument scilabDocument, DocumentEvent event) {
	    	super();
	    	this.event = event;
	    	this.scilabDocument = scilabDocument; 
	    }
	    
	    public void run() { 
		//IndentAction.getXpadEditor();
		//editor = getEditor();
		//int  caretPosition = editor.getTextPane().getCaretPosition();
		
		try {
			//if (caretPosition != 0) {
			    //System.err.println("Text inserted = {"+event.getDocument().getText(event.getOffset(), event.getLength())+"}");
				//if (editor.getTextPane().getText(caretPosition-1, 1).equals("\n")) {
			if (event == null) { // Called from SetFontAction: do nothing, change is done by ColorUpdater
				return;
			}
			if (event.getDocument().getText(event.getOffset(), event.getLength()).contains("\n")) {
			    indent(scilabDocument, event.getOffset(), event.getOffset() + event.getLength());
			}
			//}
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
		
		//indent();
	    }
	}
	/*
	 * dump document on stderr with line positions 
	 */
	private void dumpDoc( ScilabStyleDocument doc){
		try{
		Element root = doc.getDefaultRootElement();
		for(int i = 0; i!=root.getElementCount() ; ++i){
			Element e= root.getElement(i);
			int start = e.getStartOffset();
			int end = e.getEndOffset();
			System.err.println("line "+i+ " from: "+start +"to: "+end+ ":|"+doc.getText(start, end-start)+"|");
		}
		} catch (BadLocationException e) {
			System.err.println(e);
		}
	}
	/*
	 * update indentation level for current line and remaining lines
	 * keywords impacting indentation levels are in 3 kinds:
	 *  in : 'if' opening a new nesting block : no impact on current level but remaining lines are indented  
	 *  out: 'end' closing a nesting block current level and remaining lines are deindented
	 *  in/out : 'else' closing en nesting block for current line and opening another one for remaining lines
	 *  
	 *   For current line indenting, only the first keyword if relevent, for the remaining lines, we compute the algebraic sum
	 *   of indent level variations.
	 *   
	 *   Indentation levels for current line end remaining lines are stored respectively in indenLevels[0] and indentLevels[1].
	 */
	private void updateIndentLevels(String toIndent, int[]indentLevels){
		// remove space prefix
		 Matcher matcherSpace = patternSpace.matcher(toIndent);
		 if (matcherSpace.find()) {
			 toIndent = toIndent.substring(matcherSpace.group().length(), toIndent.length());
		 }
		 // extract uncommented part of the line
		Matcher matcherComment = patternComment.matcher(toIndent);
		String lineWithoutComment = matcherComment.find() ? matcherComment.group(1) : toIndent;
		// remove quoted strings
		Matcher matcherQuote = patternQuote.matcher(lineWithoutComment);
		lineWithoutComment = matcherQuote.replaceAll("");

		Matcher matcherIn = patternIn.matcher(lineWithoutComment);
		Matcher matcherInOut = patternInOut.matcher(lineWithoutComment);	
		Matcher matcherOut = patternOut.matcher(lineWithoutComment);

		// find the first of indenting keywords
		int  startInOut = matcherInOut.find(0) ? matcherInOut.start() : Integer.MAX_VALUE;
		int  startOut = matcherOut.find(0) ? matcherOut.start() : Integer.MAX_VALUE;
		if(startInOut < startOut && startInOut != Integer.MAX_VALUE){
			--indentLevels[0];
			indentLevels[1] = indentLevels[0]+1;
		}else if( startOut< startInOut && startOut != Integer.MAX_VALUE ){
			--indentLevels[0];
			--indentLevels[1];
		}
		while (matcherIn.find()){
			++indentLevels[1];
		}
		while (matcherOut.find()){
			--indentLevels[1];
		}
		
	}

	/*
	 * Adjust a baseSpaces prefix according to an indentation level and a tabulation string.
	 * for each level > 0, we append tabulation on baseSpace, for each level < 0, we remove 
	 * tabulation.length() from baseSpace.
	 * TODO: should assert a canonical representation wrt \t vs ' ' 
	 */
	private String adjustBaseSpaces(String baseSpaces, int level, String tabulation){
		String res;
		if(level < 0){
			res = baseSpaces = baseSpaces.substring(0, Math.max(0, baseSpaces.length()+tabulation.length()*level));
		} else {
			StringBuffer tmp = new StringBuffer(baseSpaces);
			while(level-- >0){
				tmp.append(tabulation);
			}
			res = tmp.toString();
		}
		return res;
	}
	
	/*
	 * adjust (line) indentation and (line+1) indenting prefix using the context of (line-1)
	 * 
	 */
	
	public void beautifyLine(ScilabStyleDocument doc, int line){
		String baseSpaces = "";
		String tabulation = TabManager.getTabulation();
		//dumpDoc(doc);
		try {
		Element root = doc.getDefaultRootElement();
		int[] indentLevels = {0,0};
		if(line > 0) {
			Element previousLine = root.getElement(line - 1);
			int startOfPreviousLine = previousLine.getStartOffset();
			String previousLineText = doc.getText(startOfPreviousLine,previousLine.getEndOffset() - startOfPreviousLine-1);// -1 to remove last \n
			// getting available context
			if(line >1) {
				Element contextLine = root.getElement(line - 2);
				int startOfContextLine = contextLine.getStartOffset();
				String contextLineText = doc.getText(startOfContextLine, contextLine.getEndOffset() - startOfContextLine-1);// -1 to remove last \n
				Matcher matcherSpace = patternSpace.matcher( contextLineText );
				baseSpaces = matcherSpace.find() ? matcherSpace.group() :"";
			
			}
			updateIndentLevels(previousLineText, indentLevels);
			if(indentLevels[0]<0){ // limited context can lead to indent levels<0 that need to be adjusted
				indentLevels[1] -= indentLevels[0];
				indentLevels[0] = 0;
			}
			indentLevels[0] = Math.max(0, indentLevels[0]);
			indentLevels[1] = Math.max(0, indentLevels[1]);
			
			Matcher matcherSpace = patternSpace.matcher( previousLineText );
			baseSpaces = matcherSpace.find() ? matcherSpace.group() :"";
		}
		Element lineElt = root.getElement(line);
		int startOfLine =  lineElt.getStartOffset();
		String lineText = doc.getText(startOfLine, lineElt.getEndOffset()-startOfLine-1);
		Matcher matcherSpace = patternSpace.matcher( lineText );
	
		indentLevels[0]=indentLevels[1];
		updateIndentLevels(lineText, indentLevels);
		doc.replace(startOfLine, matcherSpace.find() ? matcherSpace.group().length() : 0, adjustBaseSpaces(baseSpaces,indentLevels[0], tabulation), null);
		doc.insertString(java.lang.Math.min(lineElt.getEndOffset(),doc.getLength()), adjustBaseSpaces(baseSpaces,indentLevels[1], tabulation),null);
		} catch (BadLocationException e) {
			System.err.println(e);
		}
	}
	
	public int beautifier(ScilabStyleDocument scilabDocument, int startPosition, int endPosition) throws BadLocationException{
		
		int currentStartOffset = scilabDocument.getParagraphElement(startPosition).getStartOffset();
		
		int endOfFirstLine = scilabDocument.getParagraphElement(startPosition).getEndOffset();
		/*if (   endOfFirstLine - currentStartOffset <= 1){// prevented intending regions starting with an empty line.
			return;
		}*/
		String firtstLine =  scilabDocument.getText(currentStartOffset, endOfFirstLine - currentStartOffset - 1);
		Matcher matcherSpace = patternSpace.matcher(firtstLine);
		// set the reference for indent
		String baseSpaces = matcherSpace.find() ? matcherSpace.group() :"";
		Element currentElement = null;
		Element lastElement = scilabDocument.getParagraphElement(endPosition);
		
		String toIndent = "";
		
		do {
			currentElement = scilabDocument.getParagraphElement(currentStartOffset);
			toIndent= scilabDocument.getText(currentStartOffset, currentElement.getEndOffset()- currentStartOffset - 1);//-1 to remove \n			
			String indented = indentLine(toIndent, baseSpaces);
			scilabDocument.replace(currentStartOffset, toIndent.length(), indented, null);
			currentStartOffset += indented.length() + 1;
		} while(currentElement != lastElement);
		currentStringIndent = "";
		currentLevelIdent = 0;
		
		return currentStartOffset;
	}
}
