
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.container;

import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.dockable.SimpleDockable;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.frame.SimpleFrame;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.pushbutton.SimplePushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.textbox.SimpleTextBox;

/**
 * Interface for SimpleContainer the associated object to Scilab GUIs Container
 * @author Marouane BEN JELLOUL
 */
public interface SimpleContainer extends SimpleDockable {

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(SimpleCanvas member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(CheckBox member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(SimpleConsole member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(SimpleDockable member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(EditBox member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(SimpleFrame member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(Label member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(ListBox member);
		
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(SimplePushButton member);

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(RadioButton member);

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(Slider member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(SimpleTab member);
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(SimpleTextBox member);
}
