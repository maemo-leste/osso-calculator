/*
    main.cpp - OSSO Calculator
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

#include <QApplication>

#ifdef Q_WS_MAEMO_5
#include <QDBusConnection>
#include <QDBusMessage>
#endif

#include "osso-calculator.h"
#include "osso-intl.h"

int main(int argc, char * argv[]) {

	intl("osso-calculator-ui");

	QApplication app(argc, argv);
	app.setOrganizationName("osso-calculator");
	app.setApplicationName("osso-calculator-ui");

#ifdef Q_WS_MAEMO_5
	if ( ! QDBusConnection::sessionBus().registerService("com.nokia.osso_calculator") ) {
		QDBusConnection::sessionBus().send(QDBusMessage::createMethodCall("com.nokia.osso_calculator", "/com/nokia/osso_calculator", "com.nokia.osso_calculator", "top_application"));
		return 0;
	}
#endif

	OssoCalculator calculator(&app);

#ifdef Q_WS_MAEMO_5
	if ( ! QDBusConnection::sessionBus().registerObject("/com/nokia/osso_calculator", &calculator, QDBusConnection::ExportScriptableSlots) )
		return 1;
#endif

	calculator.top_application();

	return app.exec();

}
