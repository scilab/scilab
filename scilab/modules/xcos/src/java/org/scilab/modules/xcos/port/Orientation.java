package org.scilab.modules.xcos.port;

/**
 * Represent a port orientation related to the associated block. These
 * orientation semantics are valid when there is no rotation/mirror/flip
 * applied to the block.
 */
public enum Orientation {
	/** The port is on the left (west) side of the block */
	WEST,
	/** The port is on the top (north) side of the block */
	NORTH,
	/** The port is on the right (east) side of the block */
	EAST,
	/** The port is on the bottom (south) side of the block */
	SOUTH;
	
	private static final int MAX_ROTATION = 360;
	private static final int PERPENDICULAR_ROTATION = 90;
	
	/**
	 * Get the orientation angle where the associated block angle is
	 * blockAngle.
	 * 
	 * @param blockAngle
	 *            The value of the block angle
	 * @param flipped
	 *            The block flip state
	 * @param mirrored
	 *            The block mirror state
	 * @return The value of the angle.
	 */
	public int getAngle(int blockAngle, boolean flipped, boolean mirrored) {
		int angle;

		/* Specific settings */
		switch (this) {
		case WEST:
		case EAST:
			angle = 0;
			if (flipped) {
				angle = angle + (MAX_ROTATION / 2);
			}
			break;

		case NORTH:
		case SOUTH:
			angle = PERPENDICULAR_ROTATION;
			if (mirrored) {
				angle = angle + (MAX_ROTATION / 2);
			}
			break;

		default:
			angle = 0;
			break;
		}

		/* Calculate angle */
		return (angle + blockAngle) % MAX_ROTATION;
	}
	
	/**
	 * Check the style values with this position
	 * @param rotationValue Rotation value to check
	 * @param flipped Flip informations
	 * @param mirrored Mirror informations
	 * @return true is the rotation is correct, false otherwise.
	 */
	public boolean isDefaultRotation(int rotationValue, boolean flipped, boolean mirrored) {
		int angle = getBlockRotationValue(rotationValue, flipped, mirrored);
		return angle == 0;
	}

	/**
	 * Get the block rotation value from the style of the port.
	 * @param angle Rotation value of the port.
	 * @param flipped Flip state of the port
	 * @param mirrored Mirror state of the port.
	 * @return The parent block angle value (calculated).
	 */
	public int getBlockRotationValue(int angle, boolean flipped, boolean mirrored) {
		int rotation = angle;
		
		switch (this) {
		case WEST:
		case EAST:
			rotation -= 0;
			if (flipped) {
				rotation -= (MAX_ROTATION / 2);
			}
			break;

		case NORTH:
		case SOUTH:
			rotation -= PERPENDICULAR_ROTATION;
			if (mirrored) {
				rotation -= (MAX_ROTATION / 2);
			}
			break;

		default:
			break;
		}
		
		rotation = (rotation + MAX_ROTATION) % MAX_ROTATION;
		return rotation;
	}
}
