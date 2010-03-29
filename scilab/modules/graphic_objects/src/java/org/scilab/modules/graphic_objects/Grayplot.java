package org.scilab.modules.graphic_objects;

/**
 * Grayplot class
 * @author juliachs
 */
public class Grayplot extends Imageplot {
	/** Data mapping */
	private enum DataMapping { SCALED, DIRECT };

	/** Specifies how colors are mapped to values */
	private DataMapping dataMapping;

	/** Constructor */
	public Grayplot() {
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

	//	 Methods: to be done
}
