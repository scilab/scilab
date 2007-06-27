/*------------------------------------------------------------------------*/
/* file: FigureMapper.java                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class dedicated in destroying ObjectJoGL objects                */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer;

import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;




/**
 * Class dedicated in destroying ObjectJoGL objects.
 * Actually destroy method of ObjectJoGL can only be destroyed from 
 * the OpenGL context. So we store in this class every object which need to
 * be destroyed. Someone must destroy them from within the display function.
 * @author Jean-Baptiste Silvy
 */
public class ObjectJoGLCleaner {

	/** list of objects to be destroyed */
	private List<ObjectJoGL> destroyedObjects;

	/**
	 * Default constructor
	 */
	public ObjectJoGLCleaner() {
		destroyedObjects = new LinkedList<ObjectJoGL>();
	}
	
	/**
	 * Add an obkect in the list of objects which will be destroyed
	 * @param obj object to add.
	 */
	public void addObjectToDestroy(ObjectJoGL obj) {
		destroyedObjects.add(obj);
	}
	
	/**
	 * Destroy all object inserted in the list
	 * Should be called within OpenGL context
	 * @param parentFigureIndex index of the figure containing this objects
	 */
	public void destroyAll(int parentFigureIndex) {
		ListIterator<ObjectJoGL> it = destroyedObjects.listIterator();
		
		while (it.hasNext()) {
			it.next().clean(parentFigureIndex);
		}
		
		// everything has been destroyed
		destroyedObjects.clear();
	}
	
}
