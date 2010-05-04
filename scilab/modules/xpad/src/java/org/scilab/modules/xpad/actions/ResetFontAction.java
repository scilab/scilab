package org.scilab.modules.xpad.actions;

import java.awt.Font;
import java.util.List;

import javax.swing.JScrollPane;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabEditorPane;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class ResetFontAction extends DefaultAction {

    private ResetFontAction(Xpad editor) {
	super(XpadMessages.RESET_DEFAULT_FONT, editor);
    }

    public void doAction() {
	Font oldFont = ConfigXpadManager.getDefaultFont();
	List<String> listStylesName = ConfigXpadManager.getAllStyleName();
	
	getEditor().getTextPane().resetFont(oldFont);
	
	/*we need to loop on every style , if not after the second change, styles will not change anymore
	  except default*/
	int numberOfTab = getEditor().getTabPane().getComponentCount();
	for (int j = 0; j < numberOfTab; j++) {
	    
	    ScilabEditorPane textPane = (ScilabEditorPane) ((JScrollPane) getEditor().getTabPane().getComponentAt(j)).getViewport().getComponent(0) ;
	    textPane.resetFont(oldFont);
	}
	getEditor().getTextPane().setFocusable(true);
	
	ConfigXpadManager.saveFont(oldFont);
    }
    
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.RESET_DEFAULT_FONT, null, new ResetFontAction(editor), null);
	}

}
