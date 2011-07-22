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

#ifdef Q_WS_MAEMO_5
#include <QPainter>
#include <QPalette>
#endif

#include "osso-calculator-button.h"
#include "osso-intl.h"

OssoCalculatorButton::OssoCalculatorButton(const QString &name, QWidget * parent) : QPushButton(parent) {

	setText(_(name));
	setObjectName(name);
	setMinimumWidth(85);
	setMinimumHeight(60);

	connect ( this, SIGNAL( clicked(bool) ), this, SLOT( clicked() ) );

}

void OssoCalculatorButton::clicked() {

	emit clicked(objectName());

}

#ifdef Q_WS_MAEMO_5
void OssoCalculatorButton::paintEvent(QPaintEvent *) {

	QPainter paint(this);

	if ( isDown() )
		paint.setBrush(palette().color(QPalette::Highlight));
	else
		paint.setBrush(palette().color(isEnabled() ? QPalette::Normal : QPalette::Disabled, QPalette::Button));

	paint.setPen(palette().color(QPalette::Disabled, QPalette::ButtonText));
	paint.drawRoundedRect(0, 0, width()-1, height()-1, 8, 8);

	if( icon().isNull() ) {

		paint.setPen(palette().color(isEnabled() ? QPalette::Normal : QPalette::Disabled, QPalette::ButtonText));
		paint.drawText(rect(), Qt::AlignCenter, text());

	} else {

		icon().paint(&paint, rect());

	}

}
#endif
