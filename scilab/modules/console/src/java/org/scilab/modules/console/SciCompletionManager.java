
/* Copyright INRIA */

package org.scilab.modules.console;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.completion.Completion;

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
		// Get the completion part used to filter the dictionary
		String searchedPattern = inputParsingManager.getPartLevel(inputParsingManager.getCompletionLevel());
		
		// Build dictionnary
		dictionnary	= new ArrayList<CompletionItem>();

		String[] scilabCommandsDictionnary = Completion.searchCommandsDictionary(searchedPattern);
		addItemsToDictionnary("Scilab Command", scilabCommandsDictionnary);
		
		String[] scilabFilesDictionnary = Completion.searchFilesDictionary(searchedPattern);
		addItemsToDictionnary("File or Directory", scilabFilesDictionnary);
		
		String[] scilabFunctionsDictionnary = Completion.searchFunctionsDictionary(searchedPattern);
		addItemsToDictionnary("Scilab Function", scilabFunctionsDictionnary);
		
		String[] scilabHandlesDictionnary = Completion.searchHandleGraphicsPropertiesDictionary(searchedPattern);
		addItemsToDictionnary("Graphics handle field", scilabHandlesDictionnary);
		
		String[] scilabMacrosDictionnary = Completion.searchMacrosDictionary(searchedPattern);
		addItemsToDictionnary("Scilab Macro", scilabMacrosDictionnary);
		
		String[] scilabVariablesDictionnary = Completion.searchVariablesDictionary(searchedPattern);
		addItemsToDictionnary("Scilab Variable", scilabVariablesDictionnary);

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
				dictionnary.add(new CompletionItemImpl(type, items[i] + " (" + type + ")", items[i], "No help"));
			}
		}

	}

}
