package org.scilab.modules.xpad.style;

import javax.swing.text.BadLocationException;


public class TabManager {
	private static String tabulation = "  ";
	
	public static String getTabulation() { 
		return tabulation;
		}
	
	/*
	 * Insert a tab just after the caret position
	 */
	public synchronized void insertTab(ScilabStyleDocument scilabDocument, int position) {
		try {
			scilabDocument.insertString(position, getTabulation(), null);
		}
		catch (BadLocationException e) 		{
			e.printStackTrace();
		}
	}
	
	/*
	 * Tabify a line
	 */
	public synchronized int tabifyLine(ScilabStyleDocument scilabDocument, int line) {
		insertTab(scilabDocument, scilabDocument.getDefaultRootElement().getElement(line).getStartOffset());	
		return getTabulation().length();
	}
	
	/*
	 * Tabify several lines
	 */
	public synchronized int tabifyLines(ScilabStyleDocument scilabDocument, int line_start, int line_end) {
	
		boolean indentMode = scilabDocument.getAutoIndent();
		boolean colorizeMode = scilabDocument.getAutoColorize();
		boolean mergeEditsMode = scilabDocument.getShouldMergeEdits();
		
		scilabDocument.setAutoIndent(false);
		scilabDocument.setAutoColorize(false);
		
		scilabDocument.setShouldMergeEdits(true);
		
		for (int currentLine = line_start; currentLine <= line_end; ++currentLine) { // tabifying should not insert/remove lines
			tabifyLine(scilabDocument, currentLine);
		}
		scilabDocument.setAutoIndent(indentMode);
		scilabDocument.setAutoColorize(colorizeMode);
		scilabDocument.setShouldMergeEdits(mergeEditsMode);
		return getTabulation().length();
	}
		
	/**
	 * DOCUMENT UNTABIFY ACTION
	 */
	
	/*
	 * Delete a tab just after position if possible.
	 * @return the nb of deleted characters.
	 */
	public synchronized int deleteTab(ScilabStyleDocument scilabDocument, int position)	{
		String tab = getTabulation(); 
		int res = 0, tabLength = tab.length();
		try {
			String nextChars = scilabDocument.getText(position, java.lang.Math.min(tabLength, scilabDocument.getLength()-position));
			// remove as much chararcters as possible if they are part of the tab string cf. #5225
			while((res!= Math.min(nextChars.length(), tabLength)) && (nextChars.charAt(res)==tab.charAt(res))) {
				scilabDocument.remove(position, 1);
				++res;
			}
		}
		catch (BadLocationException e)
		{
			e.printStackTrace();
		}
		
		return res;
	}
	
	/*
	 * Delete a tab at the beginning of the line "line" if there was one.
	 * returns the nb of deleted characters. 
	 */
	
	public synchronized int untabifyLine(ScilabStyleDocument scilabDocument, int line)
	{
		return this.deleteTab(scilabDocument, scilabDocument.getDefaultRootElement().getElement(line).getStartOffset());
	}
	
	/*
	 * Delete tabs at the beginning of several lines
	 */
	public boolean canUntabifyLines(ScilabStyleDocument scilabDocument, int line_start, int line_end)
	{
		boolean result = true;
		String tab = getTabulation();
		int tabLength = tab.length();
		try {
			for (int i = line_start; result && (i <= line_end); i++)
			{
				result = result && (tab.equals(scilabDocument.getText(scilabDocument.getDefaultRootElement().getElement(i).getStartOffset(), tabLength)));
			}
		}
		catch( javax.swing.text.BadLocationException e){
			System.err.println("untabifying lines "+line_start+" to "+line_end+" "+e);
		}
		return result;
	}
	/*
	 * remove one tabulation step from every line starting with tabulation in [line_start, line_end]
	 * returns an array of int containing the position delta for line start and line end.
	 * the first value is getTabulation().length() if the first line was starting with a tabulation, 0 otherwise
	 * the second value is the total nb of character removed in the region.   
	 */
	public synchronized int[] untabifyLines(ScilabStyleDocument scilabDocument, int line_start, int line_end)
	{	
		int []res={0,0};
      
		if(true || canUntabifyLines(scilabDocument, line_start, line_end)) // always untabify as much lines as possible from a selection
		{
			//boolean indentMode= getAutoIndent(), colorizeMode= getColorize(), mergeEditsMode= getShouldMergeEdits();
            boolean indentMode = scilabDocument.getAutoIndent();
            boolean colorizeMode = scilabDocument.getAutoColorize();
            boolean mergeEditsMode = scilabDocument.getShouldMergeEdits();

			scilabDocument.setAutoIndent(false);
			scilabDocument.setAutoColorize(false);
			scilabDocument.setShouldMergeEdits(true);
			for (int currentLine = line_start; currentLine <= line_end; ++currentLine) { // tabifying should not insert/remove lines
				res[1] += untabifyLine(scilabDocument, currentLine); // accumulate nb of deleted characters
				if (currentLine == line_start) {
					res[0] = res[1]; // nb of deleted characters in the first line
				}
			}
			
            scilabDocument.setAutoIndent(indentMode);
            scilabDocument.setAutoColorize(colorizeMode);
            scilabDocument.setShouldMergeEdits(mergeEditsMode);

		}
		return res;
	}
	
}
