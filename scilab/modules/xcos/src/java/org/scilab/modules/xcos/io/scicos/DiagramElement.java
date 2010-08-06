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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.types.scilabTypes.ScilabBoolean;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabMList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabTList;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.link.BasicLink;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;

/**
 * Perform a diagram transformation between Scicos and Xcos.
 */
//CSOFF: ClassDataAbstractionCoupling
//CSOFF: ClassFanOutComplexity
public class DiagramElement extends AbstractElement<XcosDiagram> {
	private static final List<String> BASE_FIELD_NAMES = asList(
			"diagram", "props", "objs");
	private static final String VERSION = "scicos4.3";
	
	private static final int OBJS_INDEX = 2;
	private static final int VERSION_INDEX = 3;
	
	private static final double H_MARGIN = 40.0;
	private static final double V_MARGIN = 40.0;

	/** Diagram properties MList header (scs_m.props) */
	private static final String[] PROPS_FIELDS = {"params", "wpar", "title", "tol", "tf", "context", "void1", "options", "void2", "void3", "doc"};
	
	/** Index of the title in the props field */
	private static final int TITLE_INDEX = 2;
	
	/** Diagram options MList header (scs_m.props.options) */
	private static final String[] OPTS_FIELDS = {"scsopt", "3D", "Background", "Link", "ID", "Cmap"};
	/**
	 * Window properties (scs_m.props.wpar).
	 * 
	 * This property has no impact among simulation
	 */
	private static final double[][] WPAR = {{600, 450, 0, 0, 600, 450}};
	
	// The window parameters and diagram options are not used in the simulation
	// thus we set it to default values.
	// As the values are scicos dependent we avoid using constant references.
	// CSOFF: MagicNumber
	private static final ScilabTList DIAGRAM_OPTIONS = new ScilabTList(
			OPTS_FIELDS,
		Arrays.asList(
			new ScilabList(// 3D
				Arrays.asList(
					new ScilabBoolean(true),
					new ScilabDouble(33)
				)
			),
			new ScilabDouble(new double[][] {{8, 1}}), // Background
			new ScilabDouble(new double[][] {{1, 5}}), // Link
			new ScilabList(// ID
				Arrays.asList(
					new ScilabDouble(new double[][] {{5, 1}}),
					new ScilabDouble(new double[][] {{4, 1}})
				)
			),
			new ScilabDouble(new double[][] {{0.8, 0.8, 0.8}}) // Cmap
		)
	);
	// CSON: MagicNumber
	
	private ScilabMList base;

	/** Map from index to blocks */
	private final Map<Integer, BasicBlock> blocks;
	/**
	 * Default constructor
	 */
	public DiagramElement() {
		blocks = new HashMap<Integer, BasicBlock>();
	}

	/**
	 * Decode the diagram with version validation.
	 * 
	 * @param element
	 *            the diagram Scicos element
	 * @param into
	 *            the Xcos instance, if null, a new instance is returned.
	 * @return the modified into parameters
	 * @throws ScicosFormatException when a decoding error occurs
	 * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.scilabTypes.ScilabType,
	 *      java.lang.Object)
	 */
	@Override
	public XcosDiagram decode(ScilabType element, XcosDiagram into)
			throws ScicosFormatException {
		return decode(element, into, true);
	}
	
	/**
	 * Decode the diagram
	 * 
	 * @param element
	 *            the diagram Scicos element
	 * @param into
	 *            the Xcos instance, if null, a new instance is returned.
	 * @param validate true, if the diagram version will be checked. false otherwise.
	 * @return the modified into parameters
	 * @throws ScicosFormatException when a decoding error occurs
	 * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.scilabTypes.ScilabType,
	 *      java.lang.Object)
	 */
	public XcosDiagram decode(ScilabType element, XcosDiagram into, boolean validate)
			throws ScicosFormatException {
		base = (ScilabMList) element;
		
		XcosDiagram diag = into;
		if (diag == null) {
			diag = new XcosDiagram();
		}
		
		diag = beforeDecode(element, diag);
		diag.getModel().beginUpdate();
		
		// Validate the base field
		String wrongVersion = null;
		try {
			validate(validate);
		} catch (VersionMismatchException e) {
			wrongVersion = e.getWrongVersion();
		}
		
		// Fill the diag
		decodeDiagram(diag);
		
		diag.getModel().endUpdate();
		diag = afterDecode(element, diag);
		
		// Rethrow the version exception after a decode. 
		if (wrongVersion != null) {
			throw new VersionMismatchException(wrongVersion);
		}
		
		return diag;
	}
	
	/**
	 * Reassociate the children with the current diagram.
	 * 
	 * @param element the encoded element
	 * @param into the target instance
	 * @return the modified target instance
	 * @see org.scilab.modules.xcos.io.scicos.AbstractElement#afterDecode(org.scilab.modules.types.scilabTypes.ScilabType, java.lang.Object)
	 */
	@Override
	public XcosDiagram afterDecode(ScilabType element, XcosDiagram into) {
		into.setChildrenParentDiagram();
		return super.afterDecode(element, into);
	}

	/**
	 * Decode the diagram
	 * @param diag the current diagram
	 * @throws ScicosFormatException on decoding error
	 */
	private void decodeDiagram(XcosDiagram diag) throws ScicosFormatException {
		// Fill the local parameters
		// NOTE: the title field is checked on the ScicosParametersElement
		final String title = ((ScilabString) ((ScilabTList) base.get(1)).get(2))
				.getData()[0][0];
		diag.setTitle(title);

		// Fill the diagram attributes
		ScicosParametersElement params = new ScicosParametersElement();
		params.decode(base.get(1), diag.getScicosParameters());

		// Decode the objs attributes
		decodeObjs(diag);
	}

	/**
	 * Decode the objs list into cells
	 * 
	 * @param diag the target instance
	 * @throws ScicosFormatException on error
	 */
	private void decodeObjs(XcosDiagram diag) throws ScicosFormatException {
		final int nbOfObjs = ((ScilabList) base.get(OBJS_INDEX)).size();
		final BlockElement blockElement = new BlockElement();
		final LinkElement linkElement = new LinkElement(blocks);
		final LabelElement labelElement = new LabelElement();
		
		double minimalYaxisValue = 0.0; 
		
		/*
		 * Decode blocks
		 */
		for (int i = 0; i < nbOfObjs; i++) {
			final ScilabMList data = (ScilabMList) ((ScilabList) base.get(OBJS_INDEX)).get(i);
			Object cell = null;
			
			if (blockElement.canDecode(data)) {
				BasicBlock block = blockElement.decode(data, null);
				blocks.put(i, block);
				cell = block;
				
				minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
			} else if (labelElement.canDecode(data)) {
				cell = labelElement.decode(data, null);
				
				minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
			}
			
			if (cell != null) {
				diag.addCell(cell);
			}
		}
		
		/*
		 * Decode links
		 */
		for (int i = 0; i < nbOfObjs; i++) {
			final ScilabMList data = (ScilabMList) ((ScilabList) base.get(OBJS_INDEX)).get(i);
			Object cell = null;
			
			if (linkElement.canDecode(data)) {
				BasicLink link = linkElement.decode(data, null);
				cell = link;
				
				minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
			}
			
			if (cell != null) {
				diag.addCell(cell);
			}
		}
		
		/*
		 * Perform post-calculus
		 */
		final mxICell defaultParent = ((mxICell) diag.getDefaultParent());
		
		// Translate the y axis
		final double minY = -minimalYaxisValue + V_MARGIN;
		defaultParent.setGeometry(new mxGeometry(H_MARGIN, minY, 0, 0));
	}
	
	/**
	 * Check that the current ScilabType is a valid Diagram.
	 * 
	 * This method doesn't pass the metrics because it perform many test.
	 * Therefore all these tests are trivial and the conditioned action only
	 * throw an exception.
	 * 
	 * @param checkVersion
	 *            true, when the check validate the version
	 * @throws ScicosFormatException
	 *             When the diagram is not valid
	 */
	// CSOFF: CyclomaticComplexity
	// CSOFF: NPathComplexity
	private void validate(boolean checkVersion) throws ScicosFormatException {
		
		// Have we enough fields ?
		if (base.size() < BASE_FIELD_NAMES.size()) {
			throw new WrongStructureException(BASE_FIELD_NAMES);
		}
		
		int field = 0;
		
		/*
		 * Checking the MList header
		 */

		// Check the first field
		if (!(base.get(field) instanceof ScilabString)) {
			throw new WrongTypeException(BASE_FIELD_NAMES, field);
		}
		String[] header = ((ScilabString) base.get(field)).getData()[0];

		// Check the number of fields
		if (header.length < BASE_FIELD_NAMES.size()) {
			throw new WrongStructureException(BASE_FIELD_NAMES);
		}

		// Check the first fields values
		for (int i = 0; i < BASE_FIELD_NAMES.size(); i++) {
			if (!header[i].equals(BASE_FIELD_NAMES.get(i))) {
				throw new WrongStructureException(BASE_FIELD_NAMES);
			}
		}

		/*
		 * Checking the data types
		 */

		// the second field must contain list of props
		field++;
		if (!(base.get(field) instanceof ScilabTList)) {
			throw new WrongTypeException(BASE_FIELD_NAMES, field);
		}

		// the third field must contains lists of blocks and links
		field++;
		if (!(base.get(field) instanceof ScilabList)) {
			throw new WrongTypeException(BASE_FIELD_NAMES, field);
		}

		// the last field must contain the scicos version used
		field++;
		
		// doesn't check version if not present (optional field)
		if (base.size() <= field) {
			return;
		}
		
		if (!(base.get(field) instanceof ScilabString)) {
			throw new WrongTypeException(BASE_FIELD_NAMES, field);
		}

		/*
		 * Check the version if applicable
		 */
		if (checkVersion) {
			String scicosVersion = ((ScilabString) base.get(field)).getData()[0][0];
			if (!scicosVersion.equals(VERSION)) {
				throw new VersionMismatchException(scicosVersion);
			}
		}
	}
	// CSON: CyclomaticComplexity
	// CSON: NPathComplexity
	
	/**
	 * @param element the base element
	 * @return true if the header is valid, false otherwise
	 * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.scilabTypes.ScilabType)
	 */
	@Override
	public boolean canDecode(ScilabType element) {
		base = (ScilabMList) element;
		
		/*
		 * Checking header
		 */
		final String type = ((ScilabString) base.get(0)).getData()[0][0];
		final boolean typeIsValid = type.equals(BASE_FIELD_NAMES.get(0));
		
		/*
		 * Check the version if applicable
		 */
		final String scicosVersion = ((ScilabString) base.get(VERSION_INDEX)).getData()[0][0];
		final boolean versionIsValid = !scicosVersion.equals(VERSION);
		return typeIsValid && versionIsValid;
	}
	
	/**
	 * Encode the instance into the element
	 * 
	 * @param from the source instance
	 * @param element the previously allocated element.
	 * @return the element parameter
	 * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object, org.scilab.modules.types.scilabTypes.ScilabType)
	 */
	@Override
	public ScilabType encode(XcosDiagram from, ScilabType element) {
		base = (ScilabMList) element;
		
		int field = 0;
		
		if (base == null) {
			base = allocateElement();
		}
		
		base = (ScilabMList) beforeEncode(from, base);
		
		field++;
		fillParams(from, field);
		
		field++;
		fillObjs(from, field);

		base = (ScilabMList) afterEncode(from, base);
		
		return base;
	}

	/**
	 * Allocate the a new element
	 * @return the new element
	 */
	private ScilabMList allocateElement() {
		ScilabMList data = new ScilabMList(BASE_FIELD_NAMES.toArray(new String[0]));
		data.add(allocatePropsField()); // props
		data.add(new ScilabList()); // objs
		data.add(new ScilabString(VERSION)); // version
		return data;
	}

	/**
	 * Allocate the props field
	 * @return the new props field
	 */
	private ScilabTList allocatePropsField() {
		ScilabTList data = new ScilabTList(PROPS_FIELDS);
		data.add(new ScilabDouble(WPAR)); // wpar
		data.add(new ScilabString("")); // title
		data.add(new ScilabDouble()); // tol
		data.add(new ScilabDouble()); // tf
		data.add(new ScilabString("")); // context
		data.add(new ScilabDouble()); // void1
		data.add(DIAGRAM_OPTIONS); // options
		data.add(new ScilabDouble()); // void2
		data.add(new ScilabDouble()); // void3
		data.add(new ScilabList()); // doc
		
		return data;
	}
	
	/**
	 * Fill the params field
	 * 
	 * @param from the source instance
	 * @param field the params field number
	 */
	private void fillParams(XcosDiagram from, int field) {
		ScilabType data;
		final ScicosParametersElement paramsElement = new ScicosParametersElement();
		data = base.get(field);
		data = paramsElement.encode(from.getScicosParameters(), data);
		
		// set the title as it is need for generating files
		((ScilabTList) data).set(TITLE_INDEX, new ScilabString(from.getTitle()));
		
		base.set(field, data);
	}
	
	/**
	 * Fill the objs field
	 * 
	 * @param from the source instance
	 * @param field the objs field number
	 */
	private void fillObjs(XcosDiagram from, int field) {
		final BlockElement blockElement = new BlockElement();
		final LinkElement linkElement = new LinkElement(null);
		final ScilabList data = (ScilabList) base.get(field);
		
		final List<BasicBlock> blockList = new ArrayList<BasicBlock>();
    	final List<BasicLink> linkList = new ArrayList<BasicLink>();
		
    	/*
    	 * Fill the block and link lists
    	 */
		final Object parent = from.getDefaultParent();
		final mxIGraphModel model = from.getModel();
		final int nbObjs = model.getChildCount(parent);
		for (int i = 0; i < nbObjs; i++) {
			Object current = model.getChildAt(parent, i);
			
			if (current instanceof BasicBlock && !(current instanceof TextBlock)) {
				BasicBlock block = (BasicBlock) current;
				blockList.add(block);
				
    			//
    			// Look inside a Block to see if there is no "AutoLink"
    			// Jgraphx will store this link as block's child  
    			//
    			for (int j = 0; j < block.getChildCount(); ++j) {
    				if (block.getChildAt(j) instanceof BasicLink) {
    					linkList.add((BasicLink) block.getChildAt(j));
    				}
    			}
			} else if (current instanceof BasicLink) {
				BasicLink link = (BasicLink) current;
				linkList.add(link);
			}
		}
		
		/*
		 * Reorder links
		 */
    	for (int i = 0; i < linkList.size(); ++i) {
    		linkList.get(i).setOrdering(i + blockList.size() + 1);
    	}
    	
    	/*
    	 * Reorder and encode blocks
    	 */
    	for (int i = 0; i < blockList.size(); ++i) {
    		blockList.get(i).setOrdering(i + 1);
    		
    		data.add(blockElement.encode(blockList.get(i), null));
    	}
    	
    	/*
    	 * Encode links
    	 */
    	for (int i = 0; i < linkList.size(); ++i) {
    		data.add(linkElement.encode(linkList.get(i), null));
    	}
	}
}
//CSON: ClassDataAbstractionCoupling
//CSON: ClassFanOutComplexity
