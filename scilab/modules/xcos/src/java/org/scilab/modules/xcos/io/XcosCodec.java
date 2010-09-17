/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io;

import org.scilab.modules.graph.io.ScilabObjectCodec;
import org.scilab.modules.xcos.io.codec.BasicBlockCodec;
import org.scilab.modules.xcos.io.codec.BasicPortCodec;
import org.scilab.modules.xcos.io.codec.OrientationCodec;
import org.scilab.modules.xcos.io.codec.XcosDiagramCodec;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Root codec for Xcos diagram instance.
 * 
 * This class register all packages used by Xcos for
 * serialization/deserialization.
 */
public class XcosCodec extends mxCodec {
	/**
	 * Register packages for encoding/decoding diagrams
	 */
	static {
		// Add all xcos packages
		mxCodecRegistry.addPackage("org.scilab.modules.graph");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.graph");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.block");
		mxCodecRegistry
				.addPackage("org.scilab.modules.xcos.block.positionning");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.link");
		mxCodecRegistry
				.addPackage("org.scilab.modules.xcos.link.commandcontrol");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.explicit");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.implicit");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.port");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.command");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.control");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.input");
		mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.output");
		// Add 'types' package to have all scilab types known
		mxCodecRegistry.addPackage("org.scilab.modules.types");
	}

	/**
	 * Install codecs for serializable instance
	 */
	static {

		// Types
		ScilabObjectCodec.register();

		// Diagram
		XcosDiagramCodec.register();

		// Blocks
		BasicBlockCodec.register();

		// Link
		XcosObjectCodec.registerLinks();

		// Ports
		BasicPortCodec.register();
		
		// Orientation
		OrientationCodec.register();
	}

	/**
	 * Default constructor
	 */
	public XcosCodec() {
		super();
	}

	/**
	 * Construct a new codec associated with the document.
	 * 
	 * @param document
	 *            the document containing all the data.
	 */
	public XcosCodec(Document document) {
		super(document);
	}

}
