package org.scilab.modules.xpad.style;

import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.text.BadLocationException;

public class SearchManager {
	/**
	 * FIND AND REPLACE START
	 */
	public ArrayList<Integer[]> findWord(ScilabStyleDocument scilabDocument, String word, boolean caseSensitive, boolean wholeWord, boolean useRegexp) {
		String fullText = scilabDocument.getText(); 
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
	
	public ArrayList<Integer[]> findWord(ScilabStyleDocument scilabDocument, String word,int currentSelectStart ,int currentSelectEnd, boolean caseSensitive , boolean wholeWord , boolean useRegexp ) {
		String fullText = getSelectedDocumentLines(scilabDocument, currentSelectStart, currentSelectEnd);
		
		int offset = scilabDocument.getParagraphElement(currentSelectStart).getStartOffset();
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
	 * @param currentPos the position where the search start
	 * @param caseSensitive whether the search is sensitive or not to case
	 * @param wholeWord whether the search will only look to separate word or not
	 * @param useRegexp whether the string to search should be interpreted as a regexp or not
	 * @return 
	 */
	public int[] findNextWord (ScilabStyleDocument scilabDocument, String word ,int currentPos, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		String fullText = scilabDocument.getText();
		int index = -1;
		int end = -1;



		if ((word != null) && (!word.equals(""))) {
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

	public int[] findNextWord (ScilabStyleDocument scilabDocument, String word ,int currentPos,int currentSelectStart ,int currentSelectEnd, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		
		String fullText = getSelectedDocumentLines(scilabDocument, currentSelectStart, currentSelectEnd);
		int offset = scilabDocument.getParagraphElement(currentSelectStart).getStartOffset();
		System.out.println(currentPos);
		currentPos -=  offset;
		
		int index = -1;
		int end = -1;


		if ((word != null) && (!word.equals(""))) {
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
	public int[] findPreviousWord (ScilabStyleDocument scilabDocument, String word , int currentPos, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		String fullText = scilabDocument.getText();
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
	
	public String getSelectedDocumentLines(ScilabStyleDocument scilabDocument, int start , int end ) {
		int startOffset;
		int endOffset;

		String text = "";

		startOffset = scilabDocument.getParagraphElement(start).getStartOffset();
		endOffset = scilabDocument.getParagraphElement(end).getEndOffset();
		//We read the document and put the document into the String text

		try {
			//Get the document line by line
			text = scilabDocument.getText(startOffset, endOffset - startOffset);
		} catch (BadLocationException ex) {
			ex.printStackTrace();
		}


		return text;
	}
	
	public int[] findPreviousWord (ScilabStyleDocument scilabDocument, String word , int currentPos,int currentSelectStart ,int currentSelectEnd, boolean caseSensitive , boolean wholeWord , boolean useRegexp ){
		String fullText = getSelectedDocumentLines(scilabDocument, currentSelectStart, currentSelectEnd);
		int offset = scilabDocument.getParagraphElement(currentSelectStart).getStartOffset();
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
				Matcher matcher = pattern.matcher(fullText.substring(0, currentPos));

				boolean found = false;
				while (matcher.find()) {
					index = matcher.start() + offset;
					end = matcher.end() + offset;
					found = true;
				}

				if (!found){
					index = -1;
					end = -1;
				}
		}


		/*if nothing index and end will both be equal to -1*/
		return new int [] {index , end };


	}
	

}
