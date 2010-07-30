/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.listeners;

import java.awt.Adjustable;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;

import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JViewport;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;



public class ExpandListener implements AdjustmentListener {
	
	public void adjustmentValueChanged(AdjustmentEvent evt) {
		if (evt.getValueIsAdjusting()) {
			// The user is dragging the knob
		return;
		} 
		
		Adjustable source = evt.getAdjustable(); 
		
        // Determine which scrollbar fired the event
        int orient = source.getOrientation();
        //System.out.println(((JScrollBar)source).getComponentCount());
        JTable table = (JTable)((JViewport)((JScrollBar)source) .getParent().getComponent(0)).getComponent(0);
        if (orient == Adjustable.HORIZONTAL) {
        	//((SwingEditvarTableModel<Object>)(table.getModel())).test();
        } else {
            // Event from vertical scrollbar
        }
        
        

        // Determine the type of event
        int type = evt.getAdjustmentType();
        switch (type) {
          case AdjustmentEvent.UNIT_INCREMENT:
              // Scrollbar was increased by one unit
              break;
          case AdjustmentEvent.UNIT_DECREMENT:
              // Scrollbar was decreased by one unit
              break;
          case AdjustmentEvent.BLOCK_INCREMENT:
              // Scrollbar was increased by one block
              break;
          case AdjustmentEvent.BLOCK_DECREMENT:
              // Scrollbar was decreased by one block
              break;
          case AdjustmentEvent.TRACK:
              // The knob on the scrollbar was dragged
              break;
        }

        // Get current value
        //System.out.println(((JScrollBar)source).);
        //System.out.println( evt.getValue());
        if ( evt.getValue() ==  ((JScrollBar)source).getMaximum()) {
        	//((SwingEditvarTableModel<Object>)(table.getModel())).test();
        }

	
	}


}
