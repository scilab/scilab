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
package org.scilab.modules.xcos.block;

import java.util.UUID;

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.Xcos;

public class AfficheBlock extends BasicBlock {

	private int hashCode = 0;
	
	public AfficheBlock() {
		super();
		setVertex(false);
		setVisible(false);
	}
	
	protected AfficheBlock(String label) {
		super(label);
		setStyle("Affiche");
		setInterfaceFunctionName("AFFICH_m");
		setValue("0.00");
		//generate UID
		hashCode = UUID.randomUUID().hashCode();
		Xcos.getAfficheBlocks().put(hashCode, this);
	}

	public ScilabType getObjectsParameters() {
		ScilabList list = new ScilabList();
		if(this.getId() != null){
			System.err.println(this.getId());
			list.add(new ScilabDouble(hashCode));
		}
		return list;
	}

}
