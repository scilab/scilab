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

	/** Text box (2-element array) */
	private double [] textBox;

	/** Text box mode */
	private TextBoxMode textBoxMode;

	/** Auto-dimensioning */
	private boolean autoDimensioning;
	
	//	 Methods: to be done
}
