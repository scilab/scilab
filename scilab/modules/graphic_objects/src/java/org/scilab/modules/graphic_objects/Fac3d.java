package org.scilab.modules.graphic_objects;

/**
 * Fac3d class
 * @author juliachs
 */
public class Fac3d extends Surface {
	/** Data mapping type */
	private enum DataMapping { SCALED, DIRECT };

	/** Specifies how colors are mapped to scalar values */
	private DataMapping dataMapping;

	/** Constructor */
	public Fac3d() {
		super();
		dataMapping = DataMapping.DIRECT;
	}
	
	
	/**
	 * @return the dataMapping
	 */
	public DataMapping getDataMapping() {
		return dataMapping;
	}

	/**
	 * @param dataMapping the dataMapping to set
	 */
	public void setDataMapping(DataMapping dataMapping) {
		this.dataMapping = dataMapping;
	}

	// Methods: to be done
}
