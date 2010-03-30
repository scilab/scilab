package org.scilab.modules.xcos.port;

import java.util.NoSuchElementException;

import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.output.OutputPort;

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
	 * Get the orientation angle linked to its parent block.
	 * 
	 * @param blockAngle
	 *            The value of the block angle
	 * @param klass
	 *            The kind of port
	 * @param flipped
	 *            The block flip state
	 * @param mirrored
	 *            The block mirror state
	 * @return The value of the angle.
	 */
	public final int getRelativeAngle(int blockAngle, Class<? extends BasicPort> klass, boolean flipped, boolean mirrored) {
		final int orientation = getOrientationAngle();
		final int base = getBaseAngle(klass, orientation);
		
		return getFlippedAndMirroredAngle(base + blockAngle, flipped, mirrored);
	}
	
	/**
	 * @param klass the kind of port
	 * @param flipped the flip status
	 * @param mirrored the mirror status
	 * @return the real angle
	 */
	public final int getAbsoluteAngle(Class<? extends BasicPort> klass, boolean flipped, boolean mirrored) {
		final int orientation = getOrientationAngle();
		final int base = getBaseAngle(klass, orientation);
		
		return getFlippedAndMirroredAngle(base, flipped, mirrored);
	}
	
	/**
	 * Update the base angle according to the flipped and mirrored flag.
	 * @param base the previous angle
	 * @param flipped the flip status
	 * @param mirrored the mirror status
	 * @return the updated angle.
	 */
	private final int getFlippedAndMirroredAngle(int base, boolean flipped, boolean mirrored) {
		int angle = base;

		switch (this) {
		case WEST:
		case EAST:
			if (flipped) {
				angle = angle + (MAX_ROTATION / 2);
			}
			break;

		case NORTH:
		case SOUTH:
			if (mirrored) {
				angle = angle + (MAX_ROTATION / 2);
			}
			break;

		default:
			throw new NoSuchElementException();
		}

		/* Calculate angle */
		return angle % MAX_ROTATION;
	}
	
	/**
	 * @return the angle associated with this orientation.
	 */
	private final int getOrientationAngle() {
		return this.ordinal() * PERPENDICULAR_ROTATION;
	}

	/**
	 * As the orientation angle is calculated as an input (default jgraphx
	 * triangle direction), we have to update it for output blocks.
	 * 
	 * @param klass kind of block
	 * @param orientationAngle calculated orientation angle
	 * @return updated angle
	 */
	private final int getBaseAngle(Class<? extends BasicPort> klass, int orientationAngle) {
		final boolean isOutput = OutputPort.class.isAssignableFrom(klass)
				|| CommandPort.class.isAssignableFrom(klass);
		
		if (isOutput) {
			return orientationAngle + (MAX_ROTATION / 2);
		}
		
		return orientationAngle;
	}
}
