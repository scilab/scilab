/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.bridge.helpbrowser;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import javax.help.DefaultHelpHistoryModel;
import javax.help.JHelpContentViewer;
import javax.help.plaf.basic.BasicContentViewerUI;
import javax.swing.JComponent;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.event.HyperlinkEvent;
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.localization.Messages;

/**
 * This class inherits from BasicContentViewerUI from Javahelp.
 * Through this class, we are adding some features on the javahelp browser
 * We are adding a popup menu on the right click of the mouse
 * In this menu, we are providing:
 * 	- Execute in Scilab
 *  - Edit in the text editor
 *  - Copy
 *  - Select all
 *  - History
 */
public class SwingScilabHelpBrowserViewer extends BasicContentViewerUI {

	/**
	 *
	 */
	private static final long serialVersionUID = -2593697956426596790L;
	/* This field is a copy of BasicContentViewerUI which is privated.
	 * Therefor, I am changing the permission here to make it available
	 * to the methods of this object
	 */
	private javax.swing.JEditorPane accessibleHtml;

	public SwingScilabHelpBrowserViewer(JHelpContentViewer x) {
		super(x);
	}

	public static javax.swing.plaf.ComponentUI createUI(JComponent x) {
		return new SwingScilabHelpBrowserViewer((JHelpContentViewer) x);
	}

        /**
         * {@inheritDoc}
         */
        public void hyperlinkUpdate(HyperlinkEvent event) {
                if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
                        if (event.getDescription().startsWith("http://")) {
                                WebBrowser.openUrl(event.getURL(), event.getDescription());
                        } else {
                                super.hyperlinkUpdate(event);
                        }
                }
       }

	/**
	 * Create the UI interface
	 * @see javax.help.plaf.basic.BasicContentViewerUI#installUI(javax.swing.JComponent)
	 * @param c The component
	 */
	public void installUI(JComponent c) {
		super.installUI(c);
		this.retrievePrivateFieldFromBasicContentViewerUI();
		this.createPopupMenu(c);
	}


	/**
	 * Retrieve the field "html" from BasicContentViewerUI and change
	 * permission (it is private by default)
	 */
	private void retrievePrivateFieldFromBasicContentViewerUI() {
		Field privateField = null;
		try {
			privateField = BasicContentViewerUI.class.getDeclaredField("html");
			privateField.setAccessible(true);
		} catch (SecurityException e) {
			System.err.println("Security error: Could not change the accessibility on the html component of the help browser.");
			System.err.println("Please submit a bug report: http://bugzilla.scilab.org");
			e.printStackTrace();
		} catch (NoSuchFieldException e) {
			System.err.println("Could not find the field of the html component of the help browser.");
			System.err.println("Please submit a bug report: http://bugzilla.scilab.org");
			e.printStackTrace();
		}

		try {
			this.accessibleHtml = (javax.swing.JEditorPane) privateField.get(this);
		} catch (IllegalArgumentException e) {
			System.err.println("Illegal argument in the retrieval of the html component of Javahelp");
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			System.err.println("Illegal access in the retrieval of the html component of Javahelp");
			e.printStackTrace();
		}
	}

	/**
	 * Create the popup menu on the help
	 * @param c The graphic component
	 */
	private void createPopupMenu(JComponent c) {
		final JPopupMenu popup = new JPopupMenu();

		JMenuItem menuItem = null;

		/* Execute into Scilab */
		ActionListener actionListenerExecuteIntoScilab = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				String selection = accessibleHtml.getSelectedText();
				if (selection == null) {
					ScilabHelpBrowser.getHelpBrowser().getInfoBar().setText(Messages.gettext("No text selected"));
				} else {
					ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(selection, true /* display */, true /* store in history */);
				}
			}
		};
		menuItem = new JMenuItem(Messages.gettext("Execute into Scilab"));
		menuItem.addActionListener(actionListenerExecuteIntoScilab);
		if (!ScilabConsole.isExistingConsole()) { /* Only available in STD mode */
			menuItem.setEnabled(false);
		}
		popup.add(menuItem);


		/* Edit in the Scilab Text Editor */
		ActionListener actionListenerLoadIntoTextEditor = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				String selection = accessibleHtml.getSelectedText();
				if (selection == null) {
					ScilabHelpBrowser.getHelpBrowser().getInfoBar().setText(Messages.gettext("No text selected"));
				} else {
					try {
						/* Dynamic load of the SciNotes class.
						 * This is done to avoid a cyclic dependency on gui <=> scinotes
						 */
						Class scinotesClass = Class.forName("org.scilab.modules.scinotes.SciNotes");
						Class[] arguments = new Class[] {String.class};
						Method method = scinotesClass.getMethod("scinotesWithText", arguments);
						method.invoke(scinotesClass, new Object[]{selection});

					} catch (ClassNotFoundException e) {
						System.err.println("Could not find SciNotes class");
						e.printStackTrace();
					} catch (SecurityException e) {
						System.err.println("Security error: Could not access to SciNotes class");
						e.printStackTrace();
					} catch (NoSuchMethodException e) {
						System.err.println("Could not access to scinotesWithText method from object SciNotes");
						e.printStackTrace();
					} catch (IllegalArgumentException e) {
						System.err.println("Wrong argument used with scinotesWithText method from object SciNotes");
						e.printStackTrace();
					} catch (IllegalAccessException e) {
						System.err.println("Illegal access with scinotesWithText method from object SciNotes");
						e.printStackTrace();
					} catch (InvocationTargetException e) {
						System.err.println("Error of invocation with scinotesWithText method from object SciNotes");
						e.printStackTrace();
					}
				}
			}
		};



		menuItem = new JMenuItem(Messages.gettext("Edit in the Scilab Text Editor"));
		try {
			Class scinotesClass = Class.forName("org.scilab.modules.scinotes.SciNotes");
		} catch (ClassNotFoundException e) {
			/* SciNotes not available */
			menuItem.setEnabled(false);
		}
		menuItem.addActionListener(actionListenerLoadIntoTextEditor);
		popup.add(menuItem);
		popup.addSeparator();

		/* Back in the history*/
		ActionListener actionListenerBackHistory = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				DefaultHelpHistoryModel history = SwingScilabHelpBrowser.getHelpHistory();
				/* Not at the first position */
				if (history.getIndex() > 0) {
					SwingScilabHelpBrowser.getHelpHistory().goBack();
				}
			}
		};

		menuItem = new JMenuItem(Messages.gettext("Back"));
		menuItem.addActionListener(actionListenerBackHistory);
		popup.add(menuItem);

		/* Forward in the history*/
		ActionListener actionListenerForwardHistory = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				DefaultHelpHistoryModel history = SwingScilabHelpBrowser.getHelpHistory();
				/* Not at the last position */
				if (history.getHistory().size() != (history.getIndex() + 1)) {
					SwingScilabHelpBrowser.getHelpHistory().goForward();
				}
			}
		};

		menuItem = new JMenuItem(Messages.gettext("Forward"));
		menuItem.addActionListener(actionListenerForwardHistory);
		popup.add(menuItem);
		popup.addSeparator();

		/* Copy */
		menuItem = new JMenuItem(new DefaultEditorKit.CopyAction());
		menuItem.setText(Messages.gettext("Copy"));
		popup.add(menuItem);
		popup.addSeparator();


		/* Select all */
		ActionListener actionListenerSelectAll = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				accessibleHtml.selectAll();
			}
		};
		menuItem = new JMenuItem(Messages.gettext("Select All"));
		menuItem.addActionListener(actionListenerSelectAll);
		popup.add(menuItem);

		/* Edit in the Scilab Text Editor */
		final JMenuItem helpMenuItem = new JMenuItem("Help on the selected text");

		ActionListener actionListenerHelpOnKeyword= new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				String selection = accessibleHtml.getSelectedText();
				if (selection == null) {
					ScilabHelpBrowser.getHelpBrowser().getInfoBar().setText(Messages.gettext("No text selected"));
				} else {
					ScilabHelpBrowser.getHelpBrowser().searchKeywork(selection);
				}
			}
		};
		PropertyChangeListener listenerTextItem = new PropertyChangeListener() {
			public void propertyChange(PropertyChangeEvent arg0) {
				String keyword = accessibleHtml.getSelectedText();
				if (keyword == null) {
					helpMenuItem.setText(Messages.gettext("Help about a selected text"));
				} else {
					int nbOfDisplayedOnlyXChar=10;
					if (keyword.length() > nbOfDisplayedOnlyXChar) {
						keyword = keyword.substring(0, nbOfDisplayedOnlyXChar) + "...";
					}
					helpMenuItem.setText(Messages.gettext("Help about '") +keyword+"'");
				}
			}
		};
		helpMenuItem.addPropertyChangeListener(listenerTextItem);
		helpMenuItem.addActionListener(actionListenerHelpOnKeyword);
		popup.add(helpMenuItem);


		/* Creates the Popupmenu on the component */
		accessibleHtml.setComponentPopupMenu(popup);
	}
}
