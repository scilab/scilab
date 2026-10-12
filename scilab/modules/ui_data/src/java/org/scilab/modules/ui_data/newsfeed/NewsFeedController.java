/*
* Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
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

package org.scilab.modules.ui_data.newsfeed;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Timer;

import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;

/**
 * News feed controller
 * Updates news periodically & manages news rolling
 * Communicates through NewsFeedEvent event class
 * URL and other settings loaded from a settings file
 */
public final class NewsFeedController implements ActionListener {

    private List<News> news;
    private News currentNews = new News();

    private static final int NO_TIME_INTERVAL = -1;
    private static final int ONE_SECOND = 1000;

    // by default do not change news automatically
    private Timer newsChangeTimer;
    private int newsChangeTimeInterval = NO_TIME_INTERVAL;

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

        if (newsChangeTimeInterval != NO_TIME_INTERVAL) {
            newsChangeTimer = new Timer(newsChangeTimeInterval, this);
            newsChangeTimer.setRepeats(true);
        } else {
            newsChangeTimer = new Timer(ONE_SECOND, this);
            newsChangeTimer.setRepeats(false);
            newsChangeTimer.setInitialDelay(ONE_SECOND);
        }

        if (feedUpdateTimeInterval != NO_TIME_INTERVAL) {
            feedUpdateTimer = new Timer(feedUpdateTimeInterval, this);
            feedUpdateTimer.setInitialDelay(0);
            feedUpdateTimer.setRepeats(true);
        } else {
            feedUpdateTimer = new Timer(0, this);
            feedUpdateTimer.setRepeats(false);
        }
    }

    public void start() {
        newsChangeTimer.start();
        feedUpdateTimer.start();
    }

    public void stop() {
        newsChangeTimer.stop();
        feedUpdateTimer.stop();
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

    public NewsFetcher getNewsFetcher() {
        return newsFetcher;
    }

    public News getCurrentNews() {
        return currentNews;
    }

    public void resetCurrentNews() {
        if (news.isEmpty()) {
            currentNews = new News();
        } else {
            currentNews = news.get(0);
        }
    }

    public void addNewsFeedEventListener(NewsFeedEventListener listener) {
        listeners.add(listener);
    }

    public void removeNewsFeedEventListener(NewsFeedEventListener listener) {
        listeners.remove(listener);
    }

    public void previousNews() {
        // defensive programming
        if (news.isEmpty()) {
            currentNews = new News();
            fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
            return;
        }

        int index = news.indexOf(currentNews);
        index--;

        if (index >= 0) {
            currentNews = news.get(index);
        } else {
            currentNews = news.get(news.size() - 1);
        }

        fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
    }

    public void nextNews() {
        // defensive programming
        if (news.isEmpty()) {
            currentNews = new News();
            fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
            return;
        }

        int index = news.indexOf(currentNews);
        index++;

        if (index < news.size()) {
            currentNews = news.get(index);
        } else {
            currentNews = news.get(0);
        }

        fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
    }

    private boolean isOK() {
        return (news != null && !news.isEmpty());
    }

    public void updateNewsFeed() {
        try {
            newsFetcher.readSettings();

            if (!newsFetcher.isOK()) {
                return;
            }

            newsFetcher.fetchNews(this, new ArrayList<>());
        } catch (Exception e) {
            System.err.println(e);
            fireNewsFeedErrorEvent(NewsFeedUIMessages.NEWS_FEED_UNAVAILABLE_ERROR);
        }
    }

    public void updateToFetchedNews(List<News> fetched) {
        // on network issue, do not update the feed
        if (fetched.isEmpty()) {
            return;
        }

        // on newly fetched news, force update
        if (news.isEmpty()) {
            currentNews = fetched.get(0);
            news = fetched;
            fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
            return;
        }

        // update the feed if there is more recent news
        News recentOld = Collections.max(news, Comparator.comparing(News::getDate));
        News recentFetched = Collections.max(fetched, Comparator.comparing(News::getDate));
        
        if (recentFetched.getDate().after(recentOld.getDate()))
        {
            news = fetched;
            currentNews = fetched.get(0);
            
            fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
        }
    }

    private void readSettings() {
        try {
            newsFetcher.readSettings();
            newsChangeTimeInterval = NewsFeedSettings.getIntSetting("newsChangeTimeInterval");
            if (newsChangeTimeInterval != NO_TIME_INTERVAL) {
                newsChangeTimeInterval *= ONE_SECOND;
            }
            feedUpdateTimeInterval = NewsFeedSettings.getIntSetting("feedUpdateTimeInterval");
            if (feedUpdateTimeInterval != NO_TIME_INTERVAL) {
                feedUpdateTimeInterval *= ONE_SECOND;
            }
        } catch (Exception e) {
            System.err.println(e);
            fireNewsFeedErrorEvent(NewsFeedUIMessages.NEWS_FEED_CONFIGURATION_ERROR);
        }
    }

    public void fireNewsFeedEvent(int eventType) {
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
