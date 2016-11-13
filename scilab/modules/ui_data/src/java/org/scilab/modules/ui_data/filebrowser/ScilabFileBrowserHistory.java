/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * The File Browser history
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabFileBrowserHistory {

    private static final String PREVIOUSICON = FindIconHelper.findIcon("go-previous");
    private static final String NEXTICON = FindIconHelper.findIcon("go-next");

    private final SwingScilabTreeTable stt;
    private final List<String> history = new ArrayList<String>();
    private int position = -1;
    private final JButton previous;
    private final JButton next;
    private final JPopupMenu popup;
    private Timer timer;

    /**
     * Default constructor
     * @param stt the treetable associated with this history
     */
    public ScilabFileBrowserHistory(SwingScilabTreeTable stt) {
        this.stt = stt;
        this.popup = new JPopupMenu();
        this.popup.setBorderPainted(true);

        previous = new JButton();
        previous.setIcon(new ImageIcon(PREVIOUSICON));
        ScilabLAF.setDefaultProperties(previous);

        previous.setToolTipText(UiDataMessages.PREVIOUSDIR);
        previous.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                if (timer == null) {
                    timer = new Timer();
                    timer.schedule(new TimerTask() {
                        @Override
                        public void run() {
                            if (!popup.isVisible() || popup.getInvoker() != next) {
                                showPopup(true);
                            }
                        }
                    }, 1000);
                }
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                if (timer != null) {
                    timer.cancel();
                    timer = null;
                }
            }

            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e) && previous.isEnabled() && (!popup.isVisible() || popup.getInvoker() != previous)) {
                    showPopup(true);
                } else if (SwingUtilities.isLeftMouseButton(e) && !popup.isVisible() && previous.isEnabled()) {
                    ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(position - 1), false);
                    chDir(history.get(position - 1));
                    setPositionInHistory(position - 1);
                }
            }
        });

        next = new JButton();
        next.setIcon(new ImageIcon(NEXTICON));
        ScilabLAF.setDefaultProperties(next);

        next.setToolTipText(UiDataMessages.NEXTDIR);
        next.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                if (timer == null) {
                    timer = new Timer();
                    timer.schedule(new TimerTask() {
                        @Override
                        public void run() {
                            if (!popup.isVisible() || popup.getInvoker() != next) {
                                showPopup(false);
                            }
                        }
                    }, 1000);
                }
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                if (timer != null) {
                    timer.cancel();
                    timer = null;
                }
            }

            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e) && next.isEnabled() && (!popup.isVisible() || popup.getInvoker() != next)) {
                    showPopup(false);
                } else if (SwingUtilities.isLeftMouseButton(e) && !popup.isVisible() && next.isEnabled()) {
                    ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(position + 1), false);
                    chDir(history.get(position + 1));
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
                    @Override
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
                    @Override
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

        JButton button;
        if (prev) {
            button = previous;
        } else {
            button = next;
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
    public JButton getPreviousButton() {
        return previous;
    }

    /**
     * @return the next button
     */
    public JButton getNextButton() {
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
