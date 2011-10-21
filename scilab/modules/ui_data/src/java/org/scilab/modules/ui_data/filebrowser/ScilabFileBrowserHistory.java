/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;

/**
 * The File Browser history
 * @author Calixte DENIZET
 */
public class ScilabFileBrowserHistory {

    private static final String PREVIOUSICON = System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/go-previous.png";
    private static final String NEXTICON = System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/go-next.png";

    private SwingScilabTreeTable stt;
    private List<String> history = new ArrayList<String>();
    private int position = -1;
    private PushButton previous;
    private PushButton next;
    private JPopupMenu popup;
    private Timer timer;

    /**
     * Default constructor
     * @param stt the treetable associated with this history
     */
    public ScilabFileBrowserHistory(SwingScilabTreeTable stt) {
        this.stt = stt;
        this.popup = new JPopupMenu();
        this.popup.setBorderPainted(true);

        previous = ScilabPushButton.createPushButton();
        previous.setIcon(PREVIOUSICON);

        final SwingScilabPushButton swingPrevious = (SwingScilabPushButton) previous.getAsSimplePushButton();
        swingPrevious.addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                if (timer == null) {
                    timer = new Timer();
                    timer.schedule(new TimerTask() {
                        public void run() {
                            if (!popup.isVisible() || popup.getInvoker() != next) {
                                showPopup(true);
                            }
                        }
                    }, 1000);
                }
            }

            public void mouseReleased(MouseEvent e) {
                if (timer != null) {
                    timer.cancel();
                    timer = null;
                }
            }

            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e) && previous.isEnabled() && (!popup.isVisible() || popup.getInvoker() != swingPrevious)) {
                    showPopup(true);
                } else if (SwingUtilities.isLeftMouseButton(e) && !popup.isVisible() && previous.isEnabled()) {
                    ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(position - 1), false);
                    setPositionInHistory(position - 1);
                }
            }
        });

        next = ScilabPushButton.createPushButton();
        next.setIcon(NEXTICON);

        final SwingScilabPushButton swingNext = (SwingScilabPushButton) next.getAsSimplePushButton();
        swingNext.addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                if (timer == null) {
                    timer = new Timer();
                    timer.schedule(new TimerTask() {
                        public void run() {
                            if (!popup.isVisible() || popup.getInvoker() != next) {
                                showPopup(false);
                            }
                        }
                    }, 1000);
                }
            }

            public void mouseReleased(MouseEvent e) {
                if (timer != null) {
                    timer.cancel();
                    timer = null;
                }
            }

            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e) && next.isEnabled() && (!popup.isVisible() || popup.getInvoker() != swingNext)) {
                    showPopup(false);
                } else if (SwingUtilities.isLeftMouseButton(e) && !popup.isVisible() && next.isEnabled()) {
                    ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(position + 1), false);
                    setPositionInHistory(position + 1);
                }
            }
        });

        updateButton(0);
    }

    /**
     * Show the popup under the button
     * @param prev if true, the popup is show under the previous button
     */
    private void showPopup(boolean prev) {
        popup.removeAll();
        if (prev) {
            for (int i = position - 1; i >= 0; i--) {
                JMenuItem item = new JMenuItem(history.get(i));
                final int j = i;
                item.addActionListener(new CommonCallBack(null) {
                    public void callBack() {
                        ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(j), false);
                        chDir(history.get(j));
                        setPositionInHistory(j);
                    }
                });
                popup.add(item);
            }
        } else {
            for (int i = position + 1; i < history.size(); i++) {
                JMenuItem item = new JMenuItem(history.get(i));
                final int j = i;
                item.addActionListener(new CommonCallBack(null) {
                    public void callBack() {
                        ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(j), false);
                        chDir(history.get(j));
                        setPositionInHistory(j);
                    }
                });
                popup.add(item);
            }
        }
        popup.pack();

        SwingScilabPushButton button;
        if (prev) {
            button = (SwingScilabPushButton) previous.getAsSimplePushButton();
        } else {
            button = (SwingScilabPushButton) next.getAsSimplePushButton();
        }

        popup.show(button, 0, button.getBounds(null).height);
    }

    private static final void chDir(String path) {
        File f = new File(path);
        if (f.exists() && f.isDirectory() && f.canRead()) {
            InterpreterManagement.requestScilabExec("chdir('" + path + "')");
        }
    }

    /**
     * Add a path in the history
     * @param path the path to add
     */
    public void addPathInHistory(String path) {
        history.add(path);
        setPositionInHistory(position + 1);
    }

    /**
     * @return the previous button
     */
    public PushButton getPreviousButton() {
        return previous;
    }

    /**
     * @return the next button
     */
    public PushButton getNextButton() {
        return next;
    }

    /**
     * Set the current position in the history
     * @param pos the new position
     */
    private void setPositionInHistory(int pos) {
        position = pos;
        updateButton(pos);
    }

    /**
     * Update the buttons state according to the position
     * @param pos the position
     */
    private void updateButton(int pos) {
        previous.setEnabled(history.size() >= 2 && pos != 0);
        next.setEnabled(history.size() >= 2 && pos != history.size() - 1);
    }
}
