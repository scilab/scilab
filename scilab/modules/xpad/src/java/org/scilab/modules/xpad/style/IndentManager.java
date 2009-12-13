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
		int  startIn = matcherIn.find(0) ? matcherIn.start() : Integer.MAX_VALUE;
		int  startInOut = matcherInOut.find(0) ? matcherInOut.start() : Integer.MAX_VALUE;
		int  startOut = matcherOut.find(0) ? matcherOut.start() : Integer.MAX_VALUE;
		// beginning with an 'in' keyword does not change current indenting but inhibits indenting from other keywords
		indentLevels[1] = indentLevels[0];
		if(startIn >Math.min(startInOut, startOut)) {
			if(startInOut < startOut && startInOut != Integer.MAX_VALUE){
				--indentLevels[0];
				indentLevels[1] = indentLevels[0]+1;
			}else if( startOut< startInOut && startOut != Integer.MAX_VALUE ){
				--indentLevels[0];
			}
		}
		matcherIn.reset();
		matcherInOut.reset();
		matcherOut.reset();
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
	 * if preIndentNextLine is true, we also insert space prefix at the start of the next line.
	 */
	
	public void beautifyLine(ScilabStyleDocument doc, int line, boolean preIndentNextLine){
		String baseSpaces = "";
		String tabulation = TabManager.getTabulation();
		//doc.dump();
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
		if(preIndentNextLine) {
			doc.insertString(java.lang.Math.min(lineElt.getEndOffset(),doc.getLength()), adjustBaseSpaces(baseSpaces,indentLevels[1], tabulation),null);
		}
		} catch (BadLocationException e) {
			System.err.println(e);
		}
	}
	/*
	 * returns the ^\s* prefix of a String. 
	 */
	private String prefixSpace(String str){
		Matcher m = patternSpace.matcher(str);
		return m.find() ? m.group() : "";
	}
	
	public int beautifier(ScilabStyleDocument doc, int startPosition, int endPosition) throws BadLocationException{
		Element root = doc.getDefaultRootElement();
		int startLine = root.getElementIndex(startPosition);
		int endLine = root.getElementIndex(endPosition);
		boolean merge = doc.getShouldMergeEdits();
		doc.setShouldMergeEdits(true);
		for( int i = startLine; i<=endLine; ++i){
			beautifyLine(doc, i, false);
		}
		doc.setShouldMergeEdits(merge);
		return root.getElement(endLine).getEndOffset();
	}
}
