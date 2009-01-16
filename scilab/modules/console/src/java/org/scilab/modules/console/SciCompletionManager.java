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

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.completion.Completion;
import org.scilab.modules.localization.Messages;

import com.artenum.rosetta.core.CompletionItemImpl;
import com.artenum.rosetta.interfaces.core.CompletionItem;
import com.artenum.rosetta.interfaces.core.CompletionManager;
import com.artenum.rosetta.interfaces.core.GenericInterpreter;
import com.artenum.rosetta.interfaces.core.InputParsingManager;


/**
 * Class used completion management in Scilab Java Console
 * @author Vincent COUVERT
 */
public class SciCompletionManager implements CompletionManager {
	private List<CompletionItem> dictionnary;
	private InputParsingManager inputParsingManager;

	/**
	 * Create a fake database of completion informations
	 */
	public SciCompletionManager() {
	}

	/**
	 * Get all completion items matching currently edited line
	 * @return array list of matching items
	 * @see com.artenum.rosetta.interfaces.core.CompletionManager#getCompletionItems()
	 */
	public List<CompletionItem> getCompletionItems() {
		int compLevel = inputParsingManager.getCompletionLevel();

		// Build dictionnary
		dictionnary	= new ArrayList<CompletionItem>();

		// Get the completion part used to filter the paths/files dictionary
		
		String fileSearchedPattern = ((SciInputParsingManager) inputParsingManager).getFilePartLevel(compLevel);

		String[] scilabFilesDictionnary = Completion.searchFilesDictionary(fileSearchedPattern);
		addItemsToDictionnary(Messages.gettext("File or Directory"), scilabFilesDictionnary);

		if (scilabFilesDictionnary == null) {
			// Get the completion part used to filter the dictionary
			String searchedPattern = inputParsingManager.getPartLevel(compLevel);

			String[] scilabCommandsDictionnary = Completion.searchCommandsDictionary(searchedPattern);
			addItemsToDictionnary(Messages.gettext("Scilab Command"), scilabCommandsDictionnary);

			String[] scilabFunctionsDictionnary = Completion.searchFunctionsDictionary(searchedPattern);
			addItemsToDictionnary(Messages.gettext("Scilab Function"), scilabFunctionsDictionnary);

			String[] scilabHandlesDictionnary = Completion.searchHandleGraphicsPropertiesDictionary(searchedPattern);
			addItemsToDictionnary(Messages.gettext("Graphics handle field"), scilabHandlesDictionnary);

			String[] scilabMacrosDictionnary = Completion.searchMacrosDictionary(searchedPattern);
			addItemsToDictionnary(Messages.gettext("Scilab Macro"), scilabMacrosDictionnary);

			String[] scilabVariablesDictionnary = Completion.searchVariablesDictionary(searchedPattern);
			addItemsToDictionnary(Messages.gettext("Scilab Variable"), scilabVariablesDictionnary);
		}
		return dictionnary;
	}

	/**
	 * Associate a parsing tool to this completion manager
	 * @param inputParsingManager a parsing tool
	 * @see com.artenum.rosetta.interfaces.core.CompletionManager#setInputParsingManager(com.artenum.rosetta.interfaces.core.InputParsingManager)
	 */
	public void setInputParsingManager(InputParsingManager inputParsingManager) {
		this.inputParsingManager = inputParsingManager;
	}

	/**
	 * Associate an interpreting tool to this completion manager
	 * @param  interpretor an interpreting tool
	 * @see com.artenum.rosetta.interfaces.core.CompletionManager#setInputParsingManager(com.artenum.rosetta.interfaces.core.InputParsingManager)
	 */
	public void setInterpretor(GenericInterpreter interpretor) {
		// No need for Scilab implementation
	}

	/**
	 * Add items to current completion dictionnary
	 * @param type type of the items to add
	 * @param items all items to add
	 */
	public void addItemsToDictionnary(String type, String[] items) {
		if (items != null) {
			for (int i = 0;	i < items.length; i++) {
				dictionnary.add(new CompletionItemImpl(type, items[i] + " (" + type + ")", items[i], Messages.gettext("No help")));
			}
		}

	}

}
