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
package org.scilab.modules.xcos.block.generic;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;

public class GenericBlock extends BasicBlock {

    public GenericBlock() {
	super("Generic", "Generic");
	addPort(new CommandPort());
	addPort(new ControlPort());
	addPort(new ImplicitInputPort());
	addPort(new ImplicitInputPort());
	addPort(new ExplicitInputPort());
	addPort(new ImplicitOutputPort());
	addPort(new ImplicitOutputPort());
	addPort(new ExplicitOutputPort());
	addPort(new ImplicitOutputPort());
    }
}
