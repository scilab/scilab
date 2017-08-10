/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

    protected List<CompletionItem> dictionary;
    private InputParsingManager inputParsingManager;

    /**
     * Create a fake database of completion information
     */
    public SciCompletionManager() {
    }

    /**
     * Get all completion items matching currently edited line
     * @return array list of matching items
     * @see com.artenum.rosetta.interfaces.core.CompletionManager#getCompletionItems()
     */
    @Override
    public List<CompletionItem> getCompletionItems() {
        int compLevel = inputParsingManager.getCompletionLevel();

        // Build dictionary
        dictionary     = new ArrayList<CompletionItem>();

        // Get the completion part used to filter the paths/files dictionary

        String fileSearchedPattern = ((SciInputParsingManager) inputParsingManager).getFilePartLevel(compLevel);

        String[] scilabFilesDictionary = Completion.searchFilesDictionary(fileSearchedPattern);
        //addItemsToDictionary(Messages.gettext("File or Directory"), scilabFilesDictionary);

        if (scilabFilesDictionary != null) {
            ArrayList<String> files = new ArrayList<String>();
            ArrayList<String> directories = new ArrayList<String>();
            separateFilesDirectories(scilabFilesDictionary, files, directories);
            String[] filesDictionary = files.toArray(new String[files.size()]);
            String[] directoriesDictionary = directories.toArray(new String[directories.size()]);

            addItemsToDictionary(Messages.gettext("File"), filesDictionary);
            addItemsToDictionary(Messages.gettext("Directory"), directoriesDictionary);
        } else {
            // Get the completion part used to filter the dictionary
            String searchedPattern = inputParsingManager.getPartLevel(compLevel);
            String commandLine = inputParsingManager.getCommandLine();
            String[] scilabFieldsDictionary = Completion.searchFieldsDictionary(commandLine, searchedPattern);
            if (scilabFieldsDictionary != null) {
                addItemsToDictionary(Messages.gettext("Field"), scilabFieldsDictionary);
            } else {
                String[] scilabCommandsDictionary = Completion.searchCommandsDictionary(searchedPattern);
                addItemsToDictionary(Messages.gettext("Scilab Command"), scilabCommandsDictionary);

                String[] scilabFunctionsDictionary = Completion.searchFunctionsDictionary(searchedPattern);
                addItemsToDictionary(Messages.gettext("Scilab Function"), scilabFunctionsDictionary);

                String[] scilabHandlesDictionary = Completion.searchHandleGraphicsPropertiesDictionary(searchedPattern);
                addItemsToDictionary(Messages.gettext("Graphics handle field"), scilabHandlesDictionary);

                String[] scilabMacrosDictionary = Completion.searchMacrosDictionary(searchedPattern);
                addItemsToDictionary(Messages.gettext("Scilab Macro"), scilabMacrosDictionary);

                String[] scilabVariablesDictionary = Completion.searchVariablesDictionary(searchedPattern);
                addItemsToDictionary(Messages.gettext("Scilab Variable"), scilabVariablesDictionary);
            }
        }
        return dictionary;
    }

    /**
     * Associate a parsing tool to this completion manager
     * @param inputParsingManager a parsing tool
     * @see com.artenum.rosetta.interfaces.core.CompletionManager#setInputParsingManager(com.artenum.rosetta.interfaces.core.InputParsingManager)
     */
    @Override
    public void setInputParsingManager(InputParsingManager inputParsingManager) {
        this.inputParsingManager = inputParsingManager;
    }

    /**
     * Associate an interpreting tool to this completion manager
     * @param  interpreter an interpreting tool
     * @see com.artenum.rosetta.interfaces.core.CompletionManager#setInputParsingManager(com.artenum.rosetta.interfaces.core.InputParsingManager)
     */
    @Override
    public void setInterpretor(GenericInterpreter interpreter) {
        // No need for Scilab implementation
    }

    /**
     * Add items to current completion dictionary
     * @param type type of the items to add
     * @param items all items to add
     */
    public void addItemsToDictionary(String type, String[] items) {
        if (items != null) {
            for (int i = 0; i < items.length; i++) {
                dictionary.add(new CompletionItemImpl(type, items[i] + " (" + type + ")", items[i], Messages.gettext("No help")));
            }
        }
    }

    /**
     * Separate files from directories
     * @param scilabFilesDictionary the input containing both
     * @param filesDictionary output: only the files
     * @param directoriesDictionary output: only the directories
     */
    public void separateFilesDirectories(String[] scilabFilesDictionary, ArrayList<String> filesDictionary, ArrayList<String> directoriesDictionary) {
        String fileSep = System.getProperty("file.separator");
        for (String word : scilabFilesDictionary) {
            if (word.endsWith(fileSep)) {
                directoriesDictionary.add(new String(word));
            } else {
                filesDictionary.add(new String(word));
            }
        }
    }
}
