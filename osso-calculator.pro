#
#    osso-calculator.pro - OSSO Calculator
#    Copyright (C) 2011  Pali Rohár <pali.rohar@gmail.com>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#

TEMPLATE = app
TARGET = osso_calculator

unix {
   INSTALLS += target desktop service
   target.path = $$PREFIX/bin
   desktop.path = $$PREFIX/share/applications/hildon
   desktop.files += osso_calculator.desktop
   service.path = $$PREFIX/share/dbus-1/services
   service.files += com.nokia.osso_calculator.service
}

maemo5 {
   QT += dbus maemo5
}

HEADERS += osso-calculator.h osso-calculator-ui.h osso-calculator-button.h osso-intl.h osso-screenshot.h
SOURCES += main.cpp osso-calculator.cpp osso-calculator-ui.cpp osso-calculator-button.cpp
