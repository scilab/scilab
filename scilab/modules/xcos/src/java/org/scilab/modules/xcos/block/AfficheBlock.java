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

/**
 * Implement the AFFICH_m block
 */
public final class AfficheBlock extends BasicBlock {

    private static final long serialVersionUID = 6874403612919831380L;
    
	private int hashCode;
	
	/** Default constructor */
	public AfficheBlock() {
		super();
	}

	/**
	 * Constructor with label
	 * 
	 * @param value
	 *            the default value.
	 */
	protected AfficheBlock(String value) {
		this();
		setValue(value);
	}

	/**
	 * @return the instance UID.
	 */
	public synchronized int getHashCode() {
		//generate UID if needed (initialization is 0)
		if (hashCode == 0) {
			hashCode = UUID.randomUUID().hashCode();			
		}
		return hashCode;
	}
	
	/**
	 * Set the instance UID.
	 * @param hashcode The new UID.
	 */
	@Deprecated
	public void setHashCode(int hashcode) {
		this.hashCode = hashcode;
	}

	/**
	 * @return The scilab formated object parameters
	 */
	@Override
	public ScilabType getObjectsParameters() {
		ScilabList list = new ScilabList();
		list.add(new ScilabDouble(getHashCode()));
		return list;
	}

}
