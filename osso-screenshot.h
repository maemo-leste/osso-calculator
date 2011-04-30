/*
    osso-screenshot.h - OSSO screenshot
    Copyright (C) 2011  Pali Rohár <pali.rohar@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef OSSO_SCREENSHOT
#define OSSO_SCREENSHOT

#ifdef Q_WS_MAEMO_5

#include <QWidget>
#include <QX11Info>

#include <X11/Xlib.h>

static inline void windowTakeScreenshot(WId winId, bool take) {

	XEvent event = { 0 };

	event.xclient.type = ClientMessage;
	event.xclient.serial = 0;
	event.xclient.send_event = true;
	event.xclient.display = QX11Info::display();
	event.xclient.window = QX11Info::appRootWindow();
	event.xclient.message_type = XInternAtom(QX11Info::display(), "_HILDON_LOADING_SCREENSHOT", false);
	event.xclient.format = 32;
	event.xclient.data.l[0] = ! take;
	event.xclient.data.l[1] = winId;

	XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), false, SubstructureRedirectMask | SubstructureNotifyMask, &event);
	XFlush(QX11Info::display());
	XSync(QX11Info::display(), false);

}

#endif

#endif
