package org.scilab.modules.graphic_objects;

/**
 * Surface class
 * @author juliachs
 */
public abstract class Surface extends ClippableContouredObject {
	// TBD Data -> Data Model

	/** Specifies whether the surface is drawn or not */
	private boolean surfaceMode;

	/** Specifies the front face color (when colorFlag == 0) */
	private int colorMode;

	/** Specifies how facet colors are computed */
	private int colorFlag;

	/** Constructor */	
	public Surface() {
		super();
		surfaceMode = false;
		colorMode = 0;
		colorFlag = 0;
	}

	/**
	 * @return the colorFlag
	 */
	public int getColorFlag() {
		return colorFlag;
	}

	/**
	 * @param colorFlag the colorFlag to set
	 */
	public void setColorFlag(int colorFlag) {
		this.colorFlag = colorFlag;
	}

	/**
	 * @return the colorMode
	 */
	public int getColorMode() {
		return colorMode;
	}

	/**
	 * @param colorMode the colorMode to set
	 */
	public void setColorMode(int colorMode) {
		this.colorMode = colorMode;
	}

	/**
	 * @return the surfaceMode
	 */
	public boolean isSurfaceMode() {
		return surfaceMode;
	}

	/**
	 * @param surfaceMode the surfaceMode to set
	 */
	public void setSurfaceMode(boolean surfaceMode) {
		this.surfaceMode = surfaceMode;
	}
	
	//	 Methods: to be done
}
