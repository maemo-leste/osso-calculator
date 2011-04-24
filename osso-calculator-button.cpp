/*
    osso-calculator-button.cpp - OSSO Calculator
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

#include <QString>

#include <QWidget>
#include <QPushButton>

#include "osso-calculator-button.h"
#include "osso-intl.h"

OssoCalculatorButton::OssoCalculatorButton(const QString &name, QWidget * parent) : QPushButton(parent) {

	setText(_(name));
	setObjectName(name);
	setMinimumWidth(90);

	connect ( this, SIGNAL( clicked(bool) ), this, SLOT( clicked() ) );

}

void OssoCalculatorButton::clicked() {

	emit clicked(objectName());

}
