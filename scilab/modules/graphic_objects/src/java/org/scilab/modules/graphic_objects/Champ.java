package org.scilab.modules.graphic_objects;

/**
 * Champ class
 * @author juliachs
 */
public class Champ extends VectField {
	/** Specifies whether vectors are colored or not */
	private boolean colored;

	/** Constructor */
	public Champ() {
		super();
		colored = false;
	}

	/**
	 * @return the colored
	 */
	public boolean isColored() {
		return colored;
	}

	/**
	 * @param colored the colored to set
	 */
	public void setColored(boolean colored) {
		this.colored = colored;
	}

	// Methods: to be done
}
