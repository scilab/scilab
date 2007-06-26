
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.Component;
import java.awt.FontMetrics;
import java.awt.Point;

import javax.swing.text.BadLocationException;

import com.artenum.console.interfaces.ui.InputCommandView;
import com.artenum.console.ui.ConsoleTextPane;

/**
 * Scilab UI that contains the line edited by th euser
 * @author Vincent COUVERT
 */
public class SciInputCommandView extends ConsoleTextPane implements InputCommandView {
	
	private static final long serialVersionUID = 1L;
	private static final String END_LINE = "\n";
	private static final Point ERROR_POINT = new Point(0, 0);

	/**
	 * Constructor
	 */
	public SciInputCommandView() {
		super();
	}

	/**
	 * Gets the location of the caret in the UI
	 * @return the location as a Point object
	 * @see com.artenum.console.interfaces.ui.InputCommandView#getCaretLocation()
	 */
	public Point getCaretLocation() {
		FontMetrics fontMetric = getFontMetrics(getFont());
		String[] lines = null;
		try {
			lines = getStyledDocument().getText(0, getCaretPosition()).split(END_LINE);
		} catch (BadLocationException e1) {
			e1.printStackTrace();
			return ERROR_POINT;
		}

		Point result = new Point(fontMetric.stringWidth(lines[lines.length - 1]), (lines.length * fontMetric.getHeight()));

		// Translate for absolute coordinates
		Component currentComponent = this;
		while (currentComponent != null) {
			result.translate(currentComponent.getLocation().x, currentComponent.getLocation().y);
			currentComponent = currentComponent.getParent();
		}
		return result;
	}
}
