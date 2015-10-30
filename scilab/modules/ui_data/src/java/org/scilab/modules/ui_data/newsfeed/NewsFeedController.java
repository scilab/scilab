/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

package org.scilab.modules.ui_data.newsfeed;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Timer;

import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.ListIterator;

/**
 * News feed controller
 * Updates news periodically & manages news rolling
 * Communicates through NewsFeedEvent event class
 * URL and other settings loaded from a settings file
 */
public class NewsFeedController implements ActionListener {

    private List<News> news;
    private ListIterator<News> newsIterator = null;
    private News currentNews;

    // by default do not change news automatically
    private Timer newsChangeTimer;
    private int newsChangeTimeInterval = -1;

    // by default update feed every 4 hours
    private Timer feedUpdateTimer;
    private int feedUpdateTimeInterval = 4 * 60 * 60 * 1000;

    private NewsFetcher newsFetcher;

    private ArrayList<NewsFeedEventListener> listeners;

    public NewsFeedController() {
        newsFetcher = new NewsFetcher();

        news = new ArrayList<News>();
        listeners = new ArrayList<NewsFeedEventListener>();

        readSettings();

        if (newsChangeTimeInterval != -1) {
            newsChangeTimer = new Timer(newsChangeTimeInterval, this);
            newsChangeTimer.setRepeats(true);
        } else {
            newsChangeTimer = new Timer(1000, this);
            newsChangeTimer.setRepeats(false);
        }
        newsChangeTimer.setInitialDelay(1000);

        if (feedUpdateTimeInterval != -1) {
            feedUpdateTimer = new Timer(feedUpdateTimeInterval, this);
            feedUpdateTimer.setInitialDelay(0);
            feedUpdateTimer.setRepeats(true);
        } else {
            feedUpdateTimer = new Timer(0, this);
            feedUpdateTimer.setRepeats(false);
        }
    }

    public void start() {
        currentNews = null;
        newsChangeTimer.start();
        feedUpdateTimer.start();
    }

    public void stop() {
        newsChangeTimer.stop();
        feedUpdateTimer.stop();
        currentNews = null;
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == newsChangeTimer) {
            if (isOK()) {
                nextNews();
            }
        } else if (e.getSource() == feedUpdateTimer) {
            updateNewsFeed();
        }
    }

    public News getCurrentNews() {
        return currentNews;
    }

    public void addNewsFeedEventListener(NewsFeedEventListener listener) {
        listeners.add(listener);
    }

    public void removeNewsFeedEventListener(NewsFeedEventListener listener) {
        listeners.remove(listener);
    }

    public void previousNews() {
        currentNews = getIterator().previous();
        fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
    }

    public void nextNews() {
        currentNews = getIterator().next();
        fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
    }

    public ListIterator<News> getIterator() {
        if ((newsIterator == null) || !newsIterator.hasNext()) {
            newsIterator = news.listIterator();
        }
        return newsIterator;
    }

    private boolean isOK() {
        return (news != null && !news.isEmpty());
    }

    public void updateNewsFeed() {
        news = null;

        try {
            newsFetcher.readSettings();

            if (!newsFetcher.isOK()) {
                return;
            }

            // TODO : update only if RSS feed has new news
            news = newsFetcher.fetchNews();
        } catch (Exception e) {
            System.err.println(e);
            e.printStackTrace();
            fireNewsFeedErrorEvent(NewsFeedUIMessages.NEWS_FEED_UNAVAILABLE_ERROR);
        }

        newsIterator = null;
    }

    private void readSettings() {
        try {
            newsFetcher.readSettings();
            newsChangeTimeInterval = NewsFeedSettings.getIntSetting("newsChangeTimeInterval");
            if (newsChangeTimeInterval != -1) {
                newsChangeTimeInterval *= 1000;
            }
            feedUpdateTimeInterval = NewsFeedSettings.getIntSetting("feedUpdateTimeInterval");
            if (feedUpdateTimeInterval != -1) {
                feedUpdateTimeInterval *= 1000;
            }
        } catch (Exception e) {
            System.err.println(e);
            e.printStackTrace();
            fireNewsFeedErrorEvent(NewsFeedUIMessages.NEWS_FEED_CONFIGURATION_ERROR);
        }
    }

    private void fireNewsFeedEvent(int eventType) {
        NewsFeedEvent event = new NewsFeedEvent(this, eventType);
        Iterator<NewsFeedEventListener> iterator = listeners.iterator();
        while (iterator.hasNext()) {
            iterator.next().newsFeedEventReceived(event);
        }
    }

    private void fireNewsFeedErrorEvent(String errorMessage) {
        NewsFeedErrorEvent errorEvent = new NewsFeedErrorEvent(this, errorMessage);
        Iterator<NewsFeedEventListener> iterator = listeners.iterator();
        while (iterator.hasNext()) {
            iterator.next().newsFeedEventReceived(errorEvent);
        }
    }

}
