package org.scilab.modules.graphic_objects;

/**
 * Text class
 * @author juliachs
 */
public class Text extends ClippableTextObject {
	/** Alignement */
	private enum Alignment { LEFT, CENTER, RIGHT };

	/** Text box mode */
	private enum TextBoxMode { OFF, CENTERED, FILLED };

	/** Text angle */
	private double fontAngle;

	/** 3D coordinates position (3-element array)*/
	private double [] position;

	/** Text alignment */
	private Alignment alignment;

	/** Specifies whether a box is drawn */
	private boolean box;

	/** Text box dimensions (2-element array) */
	private double [] textBox;

	/** Text box mode */
	private TextBoxMode textBoxMode;

	/** Auto-dimensioning */
	private boolean autoDimensioning;

	/** Constructor */
	public Text() {
		super();
		fontAngle = 0.0;
		position = null;
		alignment = null;
		box = false;
		textBox = null;
		textBoxMode = null;
		autoDimensioning = false;
	}

	/**
	 * @return the alignment
	 */
	public Alignment getAlignment() {
		return alignment;
	}

	/**
	 * @param alignment the alignment to set
	 */
	public void setAlignment(Alignment alignment) {
		this.alignment = alignment;
	}

	/**
	 * @return the autoDimensioning
	 */
	public boolean isAutoDimensioning() {
		return autoDimensioning;
	}

	/**
	 * @param autoDimensioning the autoDimensioning to set
	 */
	public void setAutoDimensioning(boolean autoDimensioning) {
		this.autoDimensioning = autoDimensioning;
	}

	/**
	 * @return the box
	 */
	public boolean isBox() {
		return box;
	}

	/**
	 * @param box the box to set
	 */
	public void setBox(boolean box) {
		this.box = box;
	}

	/**
	 * @return the fontAngle
	 */
	public double getFontAngle() {
		return fontAngle;
	}

	/**
	 * @param fontAngle the fontAngle to set
	 */
	public void setFontAngle(double fontAngle) {
		this.fontAngle = fontAngle;
	}

	/**
	 * @return the position
	 */
	public double[] getPosition() {
		return position;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(double[] position) {
		this.position = position;
	}

	/**
	 * @return the textBox
	 */
	public double[] getTextBox() {
		return textBox;
	}

	/**
	 * @param textBox the textBox to set
	 */
	public void setTextBox(double[] textBox) {
		this.textBox = textBox;
	}

	/**
	 * @return the textBoxMode
	 */
	public TextBoxMode getTextBoxMode() {
		return textBoxMode;
	}

	/**
	 * @param textBoxMode the textBoxMode to set
	 */
	public void setTextBoxMode(TextBoxMode textBoxMode) {
		this.textBoxMode = textBoxMode;
	}
	
	//	 Methods: to be done
}
