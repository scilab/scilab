/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xpad.utils;

import org.scilab.modules.localization.Messages;

/**
 * All messages used in Xpad menus, dialogs, ...
 * @author Bruno JOFRET
 */
public class XpadMessages {

    public static final String DOTS = "...";
	
    public static final String SCILAB_EDITOR = Messages.gettext("Scilab text editor");
    /** FILE MENU */
    public static final String FILE = Messages.gettext("File");
    public static final String NEW = Messages.gettext("New") + DOTS;
    public static final String OPEN = Messages.gettext("Open") + DOTS;
    public static final String RECENT_FILES = Messages.gettext("Recent Files");
    public static final String SAVE = Messages.gettext("Save");
    public static final String SAVE_AS = Messages.gettext("Save as") + DOTS;
    public static final String SAVE_ALL = Messages.gettext("Save All");
    public static final String PAGE_SETUP = Messages.gettext("Page Setup") + DOTS;
    public static final String PRINT_PREVIEW = Messages.gettext("Print Preview") + DOTS;
    public static final String PRINT = Messages.gettext("Print") + DOTS;
    public static final String CLOSE = Messages.gettext("Close");
    public static final String CLOSEALL = Messages.gettext("Close All");
    public static final String CLOSEALLBUTTHIS = Messages.gettext("Close All But This");
    public static final String EXIT = Messages.gettext("Exit");
    public static final String QUESTION_MARK = Messages.gettext("?");
    
    /** EDIT MENU */
    public static final String EDIT = Messages.gettext("Edit");
    public static final String UNDO = Messages.gettext("Undo");
    public static final String REDO = Messages.gettext("Redo");
    public static final String CUT = Messages.gettext("Cut");
    public static final String COPY = Messages.gettext("Copy");
    public static final String PASTE = Messages.gettext("Paste");
    public static final String SELECT_ALL = Messages.gettext("Select All");
    public static final String DELETE = Messages.gettext("Delete");
    public static final String COMMENT_SELECTION = Messages.gettext("Comment Selection");
    public static final String UNCOMMENT_SELECTION = Messages.gettext("Uncomment Selection");
    public static final String TABIFY_SELECTION = Messages.gettext("Shift Right");
    public static final String UNTABIFY_SELECTION = Messages.gettext("Shift Left");
    public static final String INDENT = Messages.gettext("Correct Indentation");
    
    /** SEARCH MENU */
    public static final String SEARCH = Messages.gettext("Search");
    public static final String FIND_REPLACE = Messages.gettext("Find/Replace");
    public static final String GOTO_LINE = Messages .gettext("Goto line");
    
    /** VIEW MENU */
    public static final String VIEW = Messages.gettext("View");
    public static final String SHOW_HIDE_TOOLBAR = Messages.gettext("Show/Hide ToolBar");
    public static final String HIGHLIGHT_CURRENT_LINE = Messages.gettext("Highlight current line");
    public static final String WORD_WRAP = Messages.gettext("Word Wrap");
    public static final String LINE_NUMBERS = Messages.gettext("Line Numbers");
    public static final String SET_COLORS = Messages.gettext("Set Colors") + DOTS;
    public static final String SET_FONT = Messages.gettext("Set Font") + DOTS;
    public static final String RESET_DEFAULT_FONT = Messages.gettext("Reset default font");
    
    /** DOCUMENT MENU */
    public static final String DOCUMENT = Messages.gettext("Document");
    public static final String SYNTAX_TYPE = Messages.gettext("Syntax Type");
    public static final String SCILAB_STYLE = Messages.gettext("Scilab");
    public static final String PLAIN_TEXT_STYLE = "Plain Text";
    public static final String XML_STYLE = "XML";
    public static final String ENCODING_TYPE = Messages.gettext("Encoding");
    public static final String COLORIZE = Messages.gettext("Colorize");
    public static final String AUTO_INDENT = Messages.gettext("Enable Auto Reformat");
    public static final String EOL_TYPE = Messages.gettext("Line Endings");
    public static final String EOL_AUT0 = Messages.gettext("Auto (Default)");
    public static final String EOL_WINDOWS = Messages.gettext("Windows/DOS (CR + LF)");
    public static final String EOL_MACOS = Messages.gettext("Mac Classic (CR)");
    public static final String EOL_LINUX = Messages.gettext("Unix (LF)");
    
    /** EXECUTE MENU */
    public static final String EXECUTE = Messages.gettext("Execute");
    public static final String LOAD_INTO_SCILAB = Messages.gettext("Load Into Scilab");
    public static final String EVALUATE_SELECTION = Messages.gettext("Evaluate Selection");
    public static final String EXECUTE_FILE_INTO_SCILAB = Messages.gettext("Execute File Into Scilab");
    
    /** HELP MENU */
    public static final String HELP = Messages.gettext("Scilab text editor help") + DOTS;
    public static final String ABOUT = Messages.gettext("About") + DOTS;
    public static final String XPAD_VERSION = Messages.gettext("Scilab text editor");
    
    /** FIND/REPLACE DIALOG */
    public static final String FIND = Messages.gettext("Find:");
    public static final String FIND_BUTTON = Messages.gettext("Find");
    public static final String REPLACE_FIND = Messages.gettext("Replace/Find");
    public static final String REPLACE_WITH = Messages.gettext("Replace With :");
    public static final String DIRECTION = Messages.gettext("Direction");
    public static final String FORWARD = Messages.gettext("Forward");
    public static final String BACKWARD = Messages.gettext("Backward");
    public static final String SCOPE = Messages.gettext("Scope");
    public static final String ALL = Messages.gettext("All");
    public static final String SELECTED_LINES = Messages.gettext("Selected lines");
    public static final String OPTIONS = Messages.gettext("Options");
    public static final String CASE_SENSITIVE = Messages.gettext("Case sensitive");
    public static final String WRAP_SEARCH = Messages.gettext("Wrap search");
    public static final String WHOLE_WORD = Messages.gettext("Whole word");
    public static final String REGULAR_EXPRESSIONS = Messages.gettext("Regular expressions");
    public static final String REPLACE = Messages.gettext("Replace");
    public static final String REPLACE_ALL = Messages.gettext("Replace All");
    public static final String INVALID_REGEXP = Messages.gettext("Invalid regular expression : %s");
    public static final String PASSED_END_OF_DOCUMENT = Messages.gettext("You have passed the end of the document");
    public static final String PASSED_BEGIN_OF_DOCUMENT = Messages.gettext("You have passed the beginning of the document");
    public static final String END_OF_DOCUMENT = Messages.gettext("You have reached the end of the document");
    public static final String BEGIN_OF_DOCUMENT = Messages.gettext("You have reached the beginning of the document");
    public static final String STRING_NOT_FOUND = Messages.gettext("String %s not found");

    public static final String FILE_DOESNT_EXIST = Messages.gettext("The file %s doesn't exist\n Do you want to create it?");
    public static final String CLOSE_TAB_TIP = Messages.gettext("close this tab");
    public static final String THIS_FILE = Messages.gettext("This file");
    public static final String MODIFIED = Messages.gettext(" has been modified since last save\nsave it?");
    public static final String EXTERNAL_MODIFICATION = Messages.gettext("The file %s has been modified by another program !\n Overwrite it ?");
    public static final String REPLACE_FILE_TITLE = Messages.gettext("Replace File?");
    public static final String FILE_ALREADY_EXIST = Messages.gettext("File already exists");
    public static final String UNTITLED = Messages.gettext("Untitled ");
    public static final String OK = Messages.gettext("Ok");
    public static final String CANCEL = Messages.gettext("Cancel");
    public static final String DEFAULT = Messages.gettext("Default");
    public static final String NEXT_TAB = Messages.gettext("Next tab");
    public static final String PREVIOUS_TAB = Messages.gettext("Previous tab");
    public static final String EXECUTE_WARNING = Messages.gettext("You need to save your modifications before executing this file into Scilab.");
    


    public static final String ENTER_LINE_NUMBER = Messages .gettext("Enter line number");
    public static final String BINARY_FILE = Messages .gettext("This file is a binary, saving it will result in a corrupt file.");
    
    /* Status bar messages */
    public static final String LOADING = Messages .gettext("Loading") + DOTS;
    public static final String COLORIZATION_CANCELED = Messages .gettext("Colorization canceled");
    
    
    /* About box */
    public static final String VERSION = Messages.gettext("Editor") + " 1.0";
    public static final String DIGITEO = "The Scilab Consortium (DIGITEO)";
    public static final String COPYRIGHT = Messages.gettext("Copyright (c)") + " 2009 (INRIA)";
    
    public static final String XPAD_ERROR = Messages.gettext("Editor error");
    public static final String COULD_NOT_SAVE_FILE = Messages.gettext("The file could not be saved.");
    public static final String COULD_NOT_CONVERT_FILE = Messages.gettext("The file could not be converted.");
    public static final String CANNOT_LOAD_DIRECTORY = Messages.gettext("%s is a directory.");
    public static final String MODIFICATIONS_WILL_BE_LOST = Messages.gettext("Your modifications will be lost, change encoding anyway?");
    public static final String CONTINUE = Messages.gettext("Continue?");
    public static final String CANNOT_LOAD_COLORIZE = Messages.gettext("Cannot load \"xpadConfiguration.xml\"");
    
    public static final String COULD_NOT_FIND_CONSOLE = Messages.gettext("Could not find the console nor the InterpreterManagement.");
    public static final String COULD_NOT_FIND_TMPFILE = Messages.gettext("Could not find the temporary file.");
    
    public static final String CHANGE_COLORS =  Messages.gettext("Change Colors");
    public static final String CHANGE_COLOR =  Messages.gettext("Change Color");
    
    public static final String EXIT_CONFIRM = Messages.gettext("Are you sure that you want to close Text Editor ?");
}
