package org.scilab.modules.xpad.actions;

import java.awt.Font;
import java.util.List;

import javax.swing.JScrollPane;

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabEditorPane;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class SetFontAction extends DefaultAction {

	private SetFontAction(Xpad editor) {
		super(XpadMessages.SET_FONT, editor);
	}

	public void doAction() {
		SwingScilabFontChooser _fontChooser = new SwingScilabFontChooser(ConfigXpadManager.getFont());
		_fontChooser.displayAndWait();

		Font newFont = _fontChooser.getSelectedFont();

		if (newFont != null) {

			List<String> listStylesName = ConfigXpadManager.getAllStyleName();

			getEditor().getTextPane().setFont(newFont);

			/*we need to loop on every style , if not after the second change, styles will not change anymore
    		  except default*/
			int numberOfTab = getEditor().getTabPane().getComponentCount();
			for (int j = 0; j < numberOfTab; j++) {
			    ScilabEditorPane textPane = (ScilabEditorPane) ((JScrollPane) getEditor().getTabPane().getComponentAt(j)).getViewport().getComponent(0) ;
			    textPane.resetFont(newFont);
			}
			getEditor().getTextPane().setFocusable(true);
			ConfigXpadManager.saveFont(newFont);
		}

	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.SET_FONT, null, new SetFontAction(editor), null);
	}

}
