package org.scilab.modules.graphic_objects;

/**
 * FormattedText class
 * @author juliachs
 */
public class FormattedText {
	/** Text */
	private String text;

	/** Font */
	private Font font;

	/** Constructor */
	public FormattedText() {
		text = null;
		font = null;
	}

	/**
	 * @return the font
	 */
	public Font getFont() {
		return font;
	}

	/**
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		this.font = font;
	}

	/**
	 * @return the text
	 */
	public String getText() {
		return text;
	}

	/**
	 * @param text the text to set
	 */
	public void setText(String text) {
		this.text = text;
	}

	//	 Methods: to be done
}
