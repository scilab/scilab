/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
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
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.text.BadLocationException;

/**
 * Class SearchManager
 * @author Sylvestre Koumar
 *
 */
public class SearchManager {

	/**
	 * FIND AND REPLACE START
	 * @param scilabDocument document
	 * @param word string
	 * @param currentSelectStart int 
	 * @param currentSelectEnd int
	 * @param caseSensitive boolean
	 * @param wholeWord boolean
	 * @param useRegexp boolean
	 * @return ArrayList
	 */
	public ArrayList<Integer[]> findWord(ScilabStyleDocument scilabDocument, String word, 
										 int currentSelectStart, int currentSelectEnd, 
										 boolean caseSensitive, boolean wholeWord, boolean useRegexp) {
		String fullText = getSelectedDocumentLines(scilabDocument, currentSelectStart, currentSelectEnd);

		int offset = scilabDocument.getParagraphElement(currentSelectStart).getStartOffset();
		int lastIndex = 0;
		int wordSize = word.length();
		ArrayList<Integer[]> offsetList = new ArrayList<Integer[]>();

		//If we don't give any word to find
		if ((word != null) && !(word.equals(""))) {
			// prepare word for each kind of search
			if (wholeWord) {
				word = "\\b" + word + "\\b";
			}
			if (!caseSensitive) {
				if (useRegexp || wholeWord) {
					word = "(?i)" + word;
				} else {
					fullText = fullText.toLowerCase();
					word = word.toLowerCase();
				}
			}		

			//We find matching words ...
			// ... for regexp or whole words
			if (useRegexp || wholeWord) {
				word = "(?m)" + word;
				Pattern pattern = Pattern.compile(word);
				Matcher matcher = pattern.matcher(fullText);

				while (matcher.find()) {
					offsetList.add(new Integer[] {matcher.start() + offset, matcher.end() + offset});
				}
				// ... for other case
			} else {
				while ((lastIndex = fullText.indexOf(word, lastIndex)) != -1) {
					int endIndex = lastIndex + wordSize;
					offsetList.add(new Integer[] {lastIndex + offset, endIndex + offset});
					lastIndex = endIndex;
				}
			}
		}
		return offsetList;
	}

	/**
	 * Get lines selected in a document
	 * @param scilabDocument document
	 * @param start start offset
	 * @param end end offset
	 * @return the selected text
	 */
	public String getSelectedDocumentLines(ScilabStyleDocument scilabDocument, int start , int end) {
		int startOffset;
		int endOffset;

		startOffset = scilabDocument.getParagraphElement(start).getStartOffset();
		endOffset = scilabDocument.getParagraphElement(end).getEndOffset();
		//We read the document and put the document into the String text

		try {
			//Get the document line by line
			return scilabDocument.getText(startOffset, endOffset - startOffset);
		} catch (BadLocationException ex) {
			ex.printStackTrace();
		}

		return "";
	}

}
