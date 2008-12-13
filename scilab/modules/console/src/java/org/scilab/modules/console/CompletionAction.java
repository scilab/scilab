/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console;

import java.awt.Point;
import java.awt.event.ActionEvent;
import java.util.List;
import java.util.Iterator;

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import com.artenum.rosetta.interfaces.core.CompletionItem;

import org.scilab.modules.completion.Completion;

/**
 * Class used when Scilab user asks for completion on the current edited line
 * @author Vincent COUVERT
 * @author Allan CORNET 
 */
public class CompletionAction extends AbstractConsoleAction {
	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public CompletionAction() {
		super();
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		Point location = configuration.getInputParsingManager().getWindowCompletionLocation();
		List<CompletionItem> completionItems = configuration.getCompletionManager().getCompletionItems();

		if (completionItems != null && completionItems.size() == 1) {
			/* Only one item returned, autoselected and appended to command line */
			configuration.getCompletionWindow().show(completionItems, location);
			configuration.getInputParsingManager().writeCompletionPart(configuration.getCompletionWindow().getCompletionResult());
			((SciCompletionWindow) configuration.getCompletionWindow()).setVisible(false);
		} else if (completionItems != null && completionItems.size() != 0) {
			String [] completionArray = new String [completionItems.size()];
			
			int i = 0;
			Iterator < CompletionItem >  it = completionItems.iterator(); 
			while  (it.hasNext()) {  
				CompletionItem currentItem = it.next();  
				completionArray[i] = currentItem.getReturnValue();
				i++;
			}
	    
			java.util.Arrays.sort(completionArray);
			String commonPartOfWord = Completion.getCommonPart(completionArray, completionItems.size() );

			if (commonPartOfWord.length()!= 0) {
				if (configuration.getInputParsingManager().getPartLevel(0).length() != 0) {
					configuration.getInputParsingManager().append(commonPartOfWord.substring(configuration.getInputParsingManager().getPartLevel(0).length()) );
				}
			}
			configuration.getCompletionWindow().show(completionItems, location);
			
		}

	}
	
}
