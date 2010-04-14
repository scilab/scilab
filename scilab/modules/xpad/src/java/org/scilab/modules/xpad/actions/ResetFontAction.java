package org.scilab.modules.xpad.actions;

import java.awt.Font;
import java.util.ArrayList;

import javax.swing.JScrollPane;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabDocument;
import javax.swing.JEditorPane;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class ResetFontAction extends DefaultAction {

	private ResetFontAction(Xpad editor) {
		super(XpadMessages.RESET_DEFAULT_FONT, editor);
	}

	public void doAction() {


		Font oldFont = ConfigXpadManager.getDefaultFont();


		ArrayList<String> listStylesName = ConfigXpadManager.getAllStyleName();

		getEditor().getTextPane().setFont(oldFont);

		/*we need to loop on every style , if not after the second change, styles will not change anymore
    		  except default*/
		int numberOfTab = getEditor().getTabPane().getComponentCount();
		for (int j = 0; j < numberOfTab; j++) {

			JEditorPane textPane = (JEditorPane) ((JScrollPane) getEditor().getTabPane().getComponentAt(j)).getViewport().getComponent(0) ;
			ScilabDocument styleDocument = (ScilabDocument)textPane.getDocument();
			for (int i = 0; i < listStylesName.size(); i++) {
			    //Style tempStyle =  styleDocument.getStyle(listStylesName.get(i));

			    //StyleConstants.setFontFamily(tempStyle, oldFont.getFamily());
			    //StyleConstants.setFontSize(tempStyle, oldFont.getSize());
			    //StyleConstants.setBold(tempStyle, oldFont.isBold());
				//StyleConstants.setItalic(tempStyle, newFont.isItalic());  			
			}
			//new ColorizationManager().colorize(styleDocument, 0, styleDocument.getLength());
		}



		getEditor().getTextPane().setFocusable(true);

		ConfigXpadManager.saveFont(oldFont);
	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.RESET_DEFAULT_FONT, null, new ResetFontAction(editor), null);
	}

}
