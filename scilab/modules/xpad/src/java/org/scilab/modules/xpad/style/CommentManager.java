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

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

/**
 * Class Component manager
 * @author Sylvestre Koumar
 *
 */
public class CommentManager {
	private final String line_comment = "//";

	/**
	 * Comment the current line when no selection has been done
	 * @param scilabDocument document
	 * @param line line number
	 * @return int
	 */
	public int commentLine(ScilabStyleDocument scilabDocument, int line) {
		// No selection : comment the current line

		String comment_str = "//";
		int offset         = comment_str.length();
		int start          = scilabDocument.getDefaultRootElement().getElement(line).getStartOffset();

		try {
			// Replacement
			scilabDocument.insertString(start, comment_str, null);
		} catch (BadLocationException e) {
			e.printStackTrace();
		}

		return offset;
	}

	/**
	 * Comment several lines
	 * @param scilabDocument document
	 * @param line_start line start
	 * @param line_end line end
	 */
	public synchronized void commentLines(ScilabStyleDocument scilabDocument, int line_start, int line_end) {
		boolean  mergeEditsMode = scilabDocument.getShouldMergeEdits();
		scilabDocument.setShouldMergeEdits(true);
		for (int line = line_start; line <= line_end; ++line) {
			commentLine(scilabDocument, line);
		}
		scilabDocument.setShouldMergeEdits(mergeEditsMode);

	}

	/**
	 * Comment a part of a line
	 * @param scilabDocument document
	 * @param line_start line start
	 * @return int
	 */
	public int commentText(ScilabStyleDocument scilabDocument, int position_start) {
		String comment_str = "//";
		int offset         = comment_str.length();
		try {
			// Replacement
			scilabDocument.insertString(position_start, comment_str, null);
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
		return offset;
	}

	/**
	 * DOCUMENT COMMENT ACTION END
	 */

	/**
	 * Document uncomment action
	 * @param scilabDocument document
	 * @param line line number
	 * @return int
	 */
	public int uncommentLine(ScilabStyleDocument scilabDocument, int line) {
		int start   = scilabDocument.getDefaultRootElement().getElement(line).getStartOffset();
		int end     = scilabDocument.getDefaultRootElement().getElement(line).getEndOffset();			
		int offset  = 0;

		try	{
			String text     = scilabDocument.getText(start, end - start);
			Pattern pattern = Pattern.compile("^(\\s)*//");
			Matcher matcher = pattern.matcher(text);

			if (matcher.find())	{
				scilabDocument.remove(start + matcher.end() - 2, 2);
				offset = 2;
			}
		} catch (BadLocationException e) {
			e.printStackTrace();
		}

		return offset;
	}

	/**
	 * Un-Comment several lines
	 * @param scilabDocument document
	 * @param line_start line start
	 * @param line_end line end
	 */
	public synchronized void uncommentLines(ScilabStyleDocument scilabDocument, int line_start, int line_end) {
		Pattern pattern = Pattern.compile("^(\\s)*//");
		boolean  mergeEditsMode = scilabDocument.getShouldMergeEdits();
		scilabDocument.setShouldMergeEdits(true);

		for (int i = line_start; i <= line_end; i++) {
			int start   = scilabDocument.getDefaultRootElement().getElement(i).getStartOffset();
			int end     = scilabDocument.getDefaultRootElement().getElement(i).getEndOffset();			

			try	{
				// Get the text line
				String text     = scilabDocument.getText(start, end - start);
				Matcher matcher = pattern.matcher(text);

				if (matcher.find()) {
					scilabDocument.remove(start + matcher.end() - 2, 2);
				}
			} catch (BadLocationException e) {
				e.printStackTrace();
			}
		}
		scilabDocument.setShouldMergeEdits(mergeEditsMode);
	}

	/**
	 * Un-Comment a part of a line
	 * @param scilabDocument document
	 * @param position_start position start
	 * @return int
	 */
	public int uncommentText(ScilabStyleDocument scilabDocument, int position_start) {
		int offset = 0;
		try {
			Element root = scilabDocument.getDefaultRootElement();
			int lineLength = root.getElement(root.getElementIndex(position_start)).getEndOffset() - position_start;
			if ((lineLength >= line_comment.length()) 
				&& line_comment.equals(scilabDocument.getText(position_start, line_comment.length()))) {
				scilabDocument.remove(position_start, 2);
				offset = line_comment.length();
			}
		} catch (BadLocationException e) {
			e.printStackTrace();
			offset = 0;
		}
		return offset;
	}
}
