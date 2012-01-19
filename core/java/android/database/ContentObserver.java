/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.database;

<<<<<<< HEAD
import android.net.Uri;
=======
>>>>>>> upstream/master
import android.os.Handler;

/**
 * Receives call backs for changes to content. Must be implemented by objects which are added
 * to a {@link ContentObservable}.
 */
public abstract class ContentObserver {

    private Transport mTransport;

    // Protects mTransport
    private Object lock = new Object();

    /* package */ Handler mHandler;

    private final class NotificationRunnable implements Runnable {

        private boolean mSelf;
<<<<<<< HEAD
	private Uri mUri = null;
=======
>>>>>>> upstream/master

        public NotificationRunnable(boolean self) {
            mSelf = self;
        }

<<<<<<< HEAD

        public NotificationRunnable(Uri uri, boolean self) {
            mSelf = self;
            mUri = uri;
        }

        public void run() { 
            if (mUri != null) {
                ContentObserver.this.onChangeUri(mUri, mSelf);
            } else {
                ContentObserver.this.onChange(mSelf);
            }
=======
        public void run() {
            ContentObserver.this.onChange(mSelf);
>>>>>>> upstream/master
        }
    }

    private static final class Transport extends IContentObserver.Stub {
        ContentObserver mContentObserver;

        public Transport(ContentObserver contentObserver) {
            mContentObserver = contentObserver;
        }

        public boolean deliverSelfNotifications() {
            ContentObserver contentObserver = mContentObserver;
            if (contentObserver != null) {
                return contentObserver.deliverSelfNotifications();
            }
            return false;
        }

<<<<<<< HEAD
        public void onChangeUri(Uri uri, boolean selfChange) {
            ContentObserver contentObserver = mContentObserver;
            if (contentObserver != null) {
                contentObserver.dispatchChange(uri, selfChange);
            }
        }

=======
>>>>>>> upstream/master
        public void onChange(boolean selfChange) {
            ContentObserver contentObserver = mContentObserver;
            if (contentObserver != null) {
                contentObserver.dispatchChange(selfChange);
            }
        }

        public void releaseContentObserver() {
            mContentObserver = null;
        }
    }

    /**
     * onChange() will happen on the provider Handler.
     *
     * @param handler The handler to run {@link #onChange} on.
     */
    public ContentObserver(Handler handler) {
        mHandler = handler;
    }

    /**
     * Gets access to the binder transport object. Not for public consumption.
     *
     * {@hide}
     */
    public IContentObserver getContentObserver() {
        synchronized(lock) {
            if (mTransport == null) {
                mTransport = new Transport(this);
            }
            return mTransport;
        }
    }

    /**
     * Gets access to the binder transport object, and unlinks the transport object
     * from the ContentObserver. Not for public consumption.
     *
     * {@hide}
     */
    public IContentObserver releaseContentObserver() {
        synchronized(lock) {
            Transport oldTransport = mTransport;
            if (oldTransport != null) {
                oldTransport.releaseContentObserver();
                mTransport = null;
            }
            return oldTransport;
        }
    }

    /**
     * Returns true if this observer is interested in notifications for changes
     * made through the cursor the observer is registered with.
     */
    public boolean deliverSelfNotifications() {
        return false;
    }

    /**
     * This method is called when a change occurs to the cursor that
     * is being observed.
     *  
     * @param selfChange true if the update was caused by a call to <code>commit</code> on the
     *  cursor that is being observed.
     */
    public void onChange(boolean selfChange) {}

<<<<<<< HEAD
    /** @hide */
    public void onChangeUri(Uri uri, boolean selfChange) {}

=======
>>>>>>> upstream/master
    public final void dispatchChange(boolean selfChange) {
        if (mHandler == null) {
            onChange(selfChange);
        } else {
            mHandler.post(new NotificationRunnable(selfChange));
        }
    }
<<<<<<< HEAD


    /** @hide */
    public final void dispatchChange(Uri uri, boolean selfChange) {
        if (mHandler == null) {
            onChangeUri(uri, selfChange);
        } else {
            mHandler.post(new NotificationRunnable(uri, selfChange));
        }
    }

=======
>>>>>>> upstream/master
}
