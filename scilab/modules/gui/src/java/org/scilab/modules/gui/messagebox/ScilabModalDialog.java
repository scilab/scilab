package org.scilab.modules.gui.messagebox;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.localization.Messages;


public class ScilabModalDialog {

    /**
     * Message box button type
     */
    public enum ButtonType {
	OK,
	OK_CANCEL,
	YES_NO,
	YES_NO_CANCEL
    }

    /**
     * Message box button type
     */

    public enum IconType {
	ERROR_ICON,
	INFORMATION_ICON,
	PASSWORD_ICON,
	QUESTION_ICON,
	WARNING_ICON,
	SCILAB_ICON
    }

    public enum AnswerOption {
	OK_OPTION,
	CANCEL_OPTION,
	YES_OPTION,
	NO_OPTION
    }
    /**
     * private constructor, only static functions 
     */
    private ScilabModalDialog() {
    }


    /**
     * @param messages : messages to display
     * @param title : title of the message box
     * default icon "scilab"
     * default button "OK"
     * @return index of the selected button
     */
    static public AnswerOption show(String messages[]) {
	return show(messages, Messages.gettext("Scilab Message"), IconType.SCILAB_ICON, ButtonType.OK);
    }

    /**
     * @param message : message to display
     * @param title : title of the message box
     * default icon "scilab"
     * default button "OK"
     * @return index of the selected button
     */
    static public AnswerOption show(String message) {
	return show(new String[]{message}, Messages.gettext("Scilab Message"), IconType.SCILAB_ICON, ButtonType.OK);
    }

    /**
     * @param messages : messages to display
     * @param title : title of the message box
     * default icon "Warning"
     * default button "OK"
     * @return index of the selected button
     */
    static public AnswerOption show(String messages[], String title) {
	return show(messages, title, IconType.SCILAB_ICON, ButtonType.OK);
    }

    /**
     * @param message : message to display
     * @param title : title of the message box
     * default icon "Warning"
     * default button "OK"
     * @return index of the selected button
     */
    static public AnswerOption show(String message, String title) {
	return show(new String[]{message}, title, IconType.SCILAB_ICON, ButtonType.OK);
    }

    /**
     * @param messages : messages to display
     * @param title : title of the message box
     * @param iconType : message box icon ( see IconType )
     * @return index of the selected button
     */
    static public AnswerOption show(String messages[], String title, 
	    ScilabModalDialog.IconType iconType) {
	return show(messages, title, iconType, ButtonType.OK);
    }

    /**
     * @param message : message to display
     * @param title : title of the message box
     * @param iconType : message box icon ( see IconType )
     * @return index of the selected button
     */
    static public AnswerOption show(String message, String title, 
	    ScilabModalDialog.IconType iconType) {
	return show(new String[]{message}, title, iconType, ButtonType.OK);
    }

    /**
     * @param message : message to display
     * @param title : title of the message box
     * @param iconType : message box icon ( see IconType )
     * @param buttonType : message box type ( see ButtonType )
     * @return index of the selected button
     */
    static public AnswerOption show(String message, String title, 
	    ScilabModalDialog.IconType iconType, ScilabModalDialog.ButtonType buttonType) {
	return show(new String[]{message}, title, iconType, buttonType);
    }
    
    /**
     * @param messages : messages to display
     * @param title : title of the message box
     * @param iconType : message box icon ( see IconType )
     * @param buttonType : message box type ( see ButtonType )
     * @return index of the selected button
     */
    static public AnswerOption show(String messages[], String title, 
	    ScilabModalDialog.IconType iconType, ScilabModalDialog.ButtonType buttonType) {

	
	MessageBox messageBox = ScilabMessageBox.createMessageBox();
	messageBox.setTitle(title);
	messageBox.setMessage(messages);
	
	String labels[] = null;
	
	Console console = ScilabConsole.getConsole();

	switch(buttonType) {
	case OK :
	    labels = new String[]{Messages.gettext("OK")};
	    break;
	case OK_CANCEL :
	    labels = new String[]{Messages.gettext("OK"), Messages.gettext("Cancel")};
	    break;
	case YES_NO :
	    labels = new String[]{Messages.gettext("Yes"), Messages.gettext("No")};
	    break;
	case YES_NO_CANCEL :
	    labels = new String[]{Messages.gettext("Yes"), Messages.gettext("No"), Messages.gettext("Cancel")};
	    break;
	}

	messageBox.setButtonsLabels(labels);

	String iconName = null;
	switch(iconType) {
	case ERROR_ICON :
	    iconName = "error"; 
	    break;
	case INFORMATION_ICON :
	    iconName = "info"; 
	    break;
	case PASSWORD_ICON :
	    iconName = "passwd"; 
	    break;
	case QUESTION_ICON :
	    iconName = "question"; 
	    break;
	case WARNING_ICON :
	    iconName = "warning"; 
	    break;
	default :
	    iconName = "scilab"; 
	    break;
	}
	
	messageBox.setIcon(iconName);

	messageBox.displayAndWait();
	int choice = (messageBox.getSelectedButton() - 1); //zero indexed
	
	AnswerOption answer = AnswerOption.OK_OPTION;
	
	switch (buttonType) {
	case OK : //OK
	    answer = AnswerOption.OK_OPTION;		
	    break;
	case OK_CANCEL :
	    if (choice == 0) { //OK
		answer = AnswerOption.OK_OPTION;		
	    } else { //Cancel
		answer = AnswerOption.CANCEL_OPTION;
	    }
	    break;
	case YES_NO :
	    if (choice == 0) { //Yes
		answer = AnswerOption.YES_OPTION;		
	    } else { //No
		answer = AnswerOption.NO_OPTION;
	    }
	    break;
	case YES_NO_CANCEL :
	    if(choice == 0) { //Yes
		answer = AnswerOption.YES_OPTION;		
	    } else if (choice == 1) { //No
		answer = AnswerOption.NO_OPTION;
	    } else { //Cancel
		answer = AnswerOption.CANCEL_OPTION;
	    }
	    break;
	}
	return answer;
    }
}
