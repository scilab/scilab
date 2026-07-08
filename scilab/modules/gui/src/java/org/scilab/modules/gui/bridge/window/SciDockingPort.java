/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.window;

import java.awt.AWTException;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.image.BufferedImage;
import java.util.Arrays;

import javax.swing.JPanel;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.defaults.DefaultDockingPort;
import org.flexdock.view.View;
import org.scilab.modules.commons.gui.ScilabCefPanel;

public class SciDockingPort extends DefaultDockingPort {

    public static void initStatic() {
        DockingManager.setDockingStrategy(SciDockingPort.class, new SciDockingStrategy());
    }

    // disable ScilabCefPanel#removeNotify() on dock()
    @Override
    public boolean dock(Dockable dockable, String region) {
        ScilabCefPanel.setDockInProgress(true);
        boolean d = super.dock(dockable, region);
        ScilabCefPanel.setDockInProgress(false);
        return d;
    }

    // disable ScilabCefPanel#removeNotify() on undock()
    @Override
    public boolean undock(Component comp) {
        ScilabCefPanel.setDockInProgress(true);
        boolean dedock = super.undock(comp);
        ScilabCefPanel.setDockInProgress(false);
        return dedock;
    }

    /**
     * @return true if the container has a ScilabCefPanel child
     */
    private boolean hasScilabCefPanel(Container container) {
        // check all children
        return Arrays.stream(container.getComponents())
                .anyMatch(component -> 
                    (component instanceof ScilabCefPanel) ||
                    (component instanceof Container && hasScilabCefPanel((Container) component))
                );
    }

    /**
     * Helper Panel, will be painted as content pane during docking
     */
    private static class CapturedPanel extends JPanel {

        private Image image;

        public CapturedPanel() {
            setBackground(Color.WHITE);
        }

        public void setImage(BufferedImage image) {
            this.image = image;
        }

        public void captureScreen(Component component) {
            if (component == null || !component.isShowing()) {
                return;
            }
            
            try {
                // Get the component's location and size on screen
                Point locationOnScreen = component.getLocationOnScreen();
                Rectangle bounds = component.getBounds();
                bounds.x = locationOnScreen.x;
                bounds.y = locationOnScreen.y;
                
                // Create a Robot to capture the screen
                Robot robot = new Robot();
                BufferedImage screenCapture = robot.createScreenCapture(bounds);
                
                // Set the captured image to our panel
                setImage(screenCapture);
                setSize(component.getSize());
                
            } catch (AWTException e) {
                // Handle AWT exception (Robot creation failed)
                e.printStackTrace();
            } catch (Exception e) {
                // Handle other exceptions (component not on screen, etc.)
                e.printStackTrace();
            }
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            g.drawImage(image, 0, 0, null);
        }
    };

    private CapturedPanel capturedPanel = new CapturedPanel();
    private Container contentPane = null;
    private View dockedComponent = null;

    @Override
    public void setDragInProgress(boolean inProgress) {
        Component docked = getDockedComponent();
        if (docked instanceof View)
        {
            dockedComponent = (View) docked;
        }

        if (inProgress && dockedComponent != null) {
            contentPane = dockedComponent.getContentPane();
        
            if (hasScilabCefPanel(contentPane)) {
                // Perform AWT screen capture for heavyweight components
                capturedPanel.captureScreen(contentPane);
                
                ScilabCefPanel.setDockInProgress(true);
                dockedComponent.setContentPane(capturedPanel);
                ScilabCefPanel.setDockInProgress(false);
            }
        } else if (contentPane != null && dockedComponent != null) {
            ScilabCefPanel.setDockInProgress(true);
            dockedComponent.setContentPane(contentPane);
            ScilabCefPanel.setDockInProgress(false);
            contentPane = null;

        }
        
        super.setDragInProgress(inProgress);
   }
};

