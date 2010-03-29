package org.scilab.modules.graphic_objects;

/**
 * TextObject class
 * @author juliachs
 */
public class TextObject extends ContouredObject {
	/** Formatted text object */
	private FormattedText text;

	/** Constructor */
	public TextObject() {
		super();
		text = null;
	}

	/**
	 * @return the text
	 */
	public FormattedText getText() {
		return text;
	}

	/**
	 * @param text the text to set
	 */
	public void setText(FormattedText text) {
		this.text = text;
	}
	
	//	 Methods: to be done
}
