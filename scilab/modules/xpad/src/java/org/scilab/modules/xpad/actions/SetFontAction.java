package org.scilab.modules.xpad.actions;

import java.awt.Font;
import java.util.ArrayList;

import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
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

			ArrayList<String> listStylesName = ConfigXpadManager.getAllStyleName();

			getEditor().getTextPane().setFont(newFont);

			/*we need to loop on every style , if not after the second change, styles will not change anymore
    		  except default*/
			int numberOfTab = getEditor().getTabPane().getComponentCount();
			for (int j = 0; j < numberOfTab; j++) {

				JTextPane textPane = (JTextPane) ((JScrollPane) getEditor().getTabPane().getComponentAt(j)).getViewport().getComponent(0) ;

				for (int i = 0; i < listStylesName.size(); i++) {
					Style tempStyle = textPane.getStyledDocument().getStyle(listStylesName.get(i));

					StyleConstants.setFontFamily(tempStyle, newFont.getFamily());
					StyleConstants.setFontSize(tempStyle, newFont.getSize());
					StyleConstants.setBold(tempStyle, newFont.isBold());
					//StyleConstants.setItalic(tempStyle, newFont.isItalic());

				}
				/*insert update refresh the styles without needing to type text*/
				((ScilabStyleDocument) textPane.getStyledDocument()).insertUpdate(null);
			}



			getEditor().getTextPane().setFocusable(true);

			ConfigXpadManager.saveFont(newFont);
		}

	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.SET_FONT, null, new SetFontAction(editor), null);
	}

}
