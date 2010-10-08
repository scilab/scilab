/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import static java.util.Arrays.asList;

import java.util.List;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.model.mxGeometry;

/**
 * Protected class which decode graphic fields of a block.
 * 
 * This class is intentionally package-protected to prevent external use.
 */
//CSOFF: ClassDataAbstractionCoupling
class BlockGraphicElement extends BlockPartsElement {
	private static final List<String> DATA_FIELD_NAMES = asList(
			"graphics", "orig", "sz", "flip", "theta", "exprs", "pin", "pout",
			"pein", "peout", "gr_i", "id", "in_implicit", "out_implicit");
	
	private static final int ORIGIN_INDEX = 1;
	private static final int DIMS_INDEX = 2;
	private static final int FLIP_INDEX = 3;
	private static final int EXPRS_INDEX = 5;
	
	private static final int GRAPHICS_INSTRUCTION_SIZE = 8;
	
	/** Size factor use to scale Xcos-Scicos dimensions */
	private static final double SIZE_FACTOR = 1.0;
	/** Minimal side length use for any Xcos block */
	private static final double MINIMAL_SIZE = 8.0;
	
	/** Mutable field to easily get the data through methods */
	private ScilabMList data;
	
	/**
	 * Default constructor
	 */
	public BlockGraphicElement() { }

	/**
	 * Decode Scicos element into the block.
	 * 
	 * This decode method doesn't coverage Port management because we need
	 * model informations to handle it.
	 * 
	 * @param element
	 *            the scicos element
	 * @param into
	 *            the previously instantiated block.
	 * @return the modified into block.
	 * @throws ScicosFormatException
	 *             on error.
	 * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
	 *      java.lang.Object)
	 */
	@Override
	public BasicBlock decode(ScilabType element, BasicBlock into)
			throws ScicosFormatException {
		
		if (into == null) {
			throw new IllegalArgumentException();
		}
		BasicBlock block = into;
		
		data = (ScilabMList) element;

		validate();
		
		block = beforeDecode(element, block);
		
		/*
		 * fill the data
		 */
		fillDimension(block);
		fillOrigin(block);
		fillFlipAndRotation(block);
		
		block.setExprs(data.get(EXPRS_INDEX));
		
		
		block = afterDecode(element, block);
		
		return block;
	}

	/**
	 * Validate the current data.
	 * 
	 * This method doesn't pass the metrics because it perform many test.
	 * Therefore all these tests are trivial and the conditioned action only
	 * throw an exception.
	 * 
	 * @throws ScicosFormatException
	 *             when there is a validation error.
	 */
	// CSOFF: CyclomaticComplexity
	// CSOFF: NPathComplexity
	// CSOFF: JavaNCSS
	private void validate() throws ScicosFormatException {
		if (!canDecode(data)) {
			throw new WrongElementException();
		}

		int field = 0;

		// we test if the structure as enough field
		if (data.size() != DATA_FIELD_NAMES.size()) {
			throw new WrongStructureException(DATA_FIELD_NAMES);
		}

		/*
		 * Checking the MList header
		 */

		// Check the first field
		if (!(data.get(field) instanceof ScilabString)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}
		final String[] header = ((ScilabString) data.get(field)).getData()[0];

		// Checking for the field names
		if (header.length != DATA_FIELD_NAMES.size()) {
			throw new WrongStructureException(DATA_FIELD_NAMES);
		}
		for (int i = 0; i < header.length; i++) {
			if (!header[i].equals(DATA_FIELD_NAMES.get(i))) {
				throw new WrongStructureException(DATA_FIELD_NAMES);
			}
		}

		/*
		 * Checking the data
		 */

		// orig : must contain the coord of the block
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// sz : must contains the size of the block
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// flip
		field++;
		if (!(data.get(field) instanceof ScilabBoolean)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// theta
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// exprs
		field++;
		if (!(data.get(field) instanceof ScilabString)
				&& !(data.get(field) instanceof ScilabList)
				&& !(data.get(field) instanceof ScilabTList)
				&& !isEmptyField(data.get(field))) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// pin
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// pout
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// pein
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// peout
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// gr_i
		// !! WARNING !! we do not care about gr_i because there are only
		// block look related.
		field++;

		// id
		field++;
		if (!(data.get(field) instanceof ScilabString)) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// in_implicit
		field++;
		if (!(data.get(field) instanceof ScilabString)
				&& !isEmptyField(data.get(field))) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}

		// out_implicit
		field++;
		if (!(data.get(field) instanceof ScilabString)
				&& !isEmptyField(data.get(field))) {
			throw new WrongTypeException(DATA_FIELD_NAMES, field);
		}
	}
	// CSON: CyclomaticComplexity
	// CSON: NPathComplexity
	// CSON: JavaNCSS
	
	/**
	 * Fill the block with the origin parameters
	 * 
	 * @param into
	 *            the target instance
	 */
	private void fillOrigin(BasicBlock into) {
		/*
		 * Getting the values
		 */
		double x;
		double y;
		
		final double[][] real = ((ScilabDouble) data.get(ORIGIN_INDEX)).getRealPart();
		x = real[0][0];
		final double[] vector = real[real.length - 1];
		y = vector[vector.length - 1];
		
		/*
		 * Apply compatibility patterns
		 */
		x *= SIZE_FACTOR;
		y *= SIZE_FACTOR;
		
		/*
		 * Invert y-axis and translate it.
		 */
		y = -y - into.getGeometry().getWidth();
		
		/*
		 * fill parameter
		 */
		into.getGeometry().setX(x);
		into.getGeometry().setY(y);
	}
	
	/**
	 * Fill the block with the dimension parameters
	 * 
	 * @param into
	 *            the target instance
	 */
	private void fillDimension(BasicBlock into) {
		/*
		 * Getting the values
		 */
		double w;
		double h;
		
		final double[][] real = ((ScilabDouble) data.get(DIMS_INDEX)).getRealPart();
		w = real[0][0];
		final double[] vector = real[real.length - 1];
		h = vector[vector.length - 1];
		
		/*
		 * Apply compatibility patterns
		 */
		h *= SIZE_FACTOR;
		w *= SIZE_FACTOR;
		
		h = Math.max(h, MINIMAL_SIZE);
		w = Math.max(w, MINIMAL_SIZE);
		
		/*
		 * fill parameter
		 */
		into.getGeometry().setWidth(w);
		into.getGeometry().setHeight(h);
	}
	
	/**
	 * Fill the block with the flip and theta parameters
	 * 
	 * @param into
	 *            the target instance
	 */
	private void fillFlipAndRotation(BasicBlock into) {
		/*
		 * Flip management
		 */
		if (!((ScilabBoolean) data.get(FLIP_INDEX)).getData()[0][0]) {
			into.setFlip(true);
		} else {
			into.setFlip(false);
		}
		
		/*
		 * Rotation management
		 */
		int theta = (int) ((ScilabDouble) data.get(FLIP_INDEX + 1))
				.getRealPart()[0][0];

		// convert to a valid value
		theta = BlockPositioning.roundAngle(theta);
		
		into.setAngle(theta);
	}
	
	/**
	 * Check if the element can be decoded.
	 * 
	 * @param element
	 *            the Scicos element
	 * @return true, if the Scicos types match.
	 * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
	 */
	@Override
	public boolean canDecode(ScilabType element) {
		data = (ScilabMList) element;

		final String type = ((ScilabString) data.get(0)).getData()[0][0];
		return type.equals(DATA_FIELD_NAMES.get(0));
	}
	
	/**
	 * Encode the instance into the element
	 * 
	 * @param from the source instance
	 * @param element must be null
	 * @return the element parameter
	 * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object, org.scilab.modules.types.ScilabType)
	 */
	@Override
	public ScilabType encode(BasicBlock from, ScilabType element) {
		data = (ScilabMList) element;
		int field = 0;
		
		if (data == null) {
			data = allocateElement();
		} else {
			throw new IllegalArgumentException("The element parameter must be null.");
		}
		
		data = (ScilabMList) beforeEncode(from, data);
		
		/*
		 * Fill the geometry data
		 */
		field = encodePosition(from, field);
		
		field++; // flip
		data.set(field, new ScilabBoolean(true));
		field++; // theta
		data.set(field, new ScilabDouble(0));
		
		field++; // exprs
		data.set(field, from.getExprs());
		
		/*
		 * Fields managed by specific elements.
		 * 
		 * see InputPortElement and OutputPortElement.
		 */
		field++; // pin
		field++; // pout
		
		field++; // pein
		List<ControlPort> ctrlPorts = BasicBlockInfo.getAllTypedPorts(from, false, ControlPort.class);
		data.set(field, BasicBlockInfo.getAllLinkId(ctrlPorts));
		field++; // peout
		List<CommandPort> cmdPorts = BasicBlockInfo.getAllTypedPorts(from, false, CommandPort.class);
		data.set(field, BasicBlockInfo.getAllLinkId(cmdPorts));
		
		field++; // gr_i
		ScilabList graphics = (ScilabList) data.get(field);
		ScilabString graphicsInstructions = new ScilabString(
				"xstringb(orig(1),orig(2),\""
				+ from.getInterfaceFunctionName()
				+ "\",sz(1),sz(2));");
		graphics.add(graphicsInstructions);
		graphics.add(new ScilabDouble(GRAPHICS_INSTRUCTION_SIZE));
		
		data.set(field, graphics);
		
		field++; // id
		
		/*
		 * Fields managed by specific elements.
		 * 
		 * see InputPortElement and OutputPortElement.
		 */
		field++; // in_implicit
		field++; // out_implicit
		
		data = (ScilabMList) afterEncode(from, data);
		
		return data;
	}

	/**
	 * Encode the position and dimensions
	 * @param from the instance
	 * @param field the current field index
	 * @return the new field index
	 */
	private int encodePosition(BasicBlock from, int field) {
		final mxGeometry geom = from.getGeometry();
		int internalField = field;
		
		internalField++; // orig
		final double[][] orig = {{geom.getX(), -geom.getY() - geom.getHeight()}};
		data.set(internalField, new ScilabDouble(orig));
		
		internalField++; // sz
		final double[][] sz = {{geom.getWidth(), geom.getHeight()}};
		data.set(internalField, new ScilabDouble(sz));
		
		return internalField;
	}
	
	/**
	 * Allocate a new element
	 * @return the new element
	 */
	private ScilabMList allocateElement() {
		ScilabMList element = new ScilabMList(DATA_FIELD_NAMES.toArray(new String[0]));
		element.add(new ScilabDouble()); // orig
		element.add(new ScilabDouble()); // sz
		element.add(new ScilabBoolean()); // flip
		element.add(new ScilabDouble()); // theta
		element.add(new ScilabString()); // exprs
		addSizedPortVector(element, ScilabDouble.class, getInSize());  // pin
		addSizedPortVector(element, ScilabDouble.class, getOutSize()); // pout
		addSizedPortVector(element, ScilabDouble.class, getEinSize()); // pein
		addSizedPortVector(element, ScilabDouble.class, getEoutSize()); // peout
		element.add(new ScilabList()); // gr_i
		element.add(new ScilabString("")); // id
		addSizedPortVector(element, ScilabString.class, getInSize()); // in_implicit
		addSizedPortVector(element, ScilabString.class, getOutSize()); // out_implicit
		return element;
	}
}
//CSON: ClassDataAbstractionCoupling
