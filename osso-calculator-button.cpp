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
#include <QPalette>

#ifdef Q_WS_MAEMO_5
#include <QMaemo5Style>
#endif

#include "osso-calculator-button.h"
#include "osso-intl.h"

OssoCalculatorButton::OssoCalculatorButton(const QString &name, QWidget * parent) : QPushButton(parent) {

	setText(_(name));
	setMinimumWidth(86);
	setMinimumHeight(55);

	buttonName = name;

	if ( name == "calc_bv_desk_ac" || name == "calc_bv_desk_c" )
		textColor = "AttentionColor";
	else if ( name == "calc_bv_tr_min" || name == "calc_bv_tr_mplus" || name == "calc_bv_tr_mminus" || name == "calc_bv_tr_mr" )
		textColor = "AccentColor1";

#ifdef Q_WS_MAEMO_5
	if ( name.at(0).isDigit() || name == "calc_bv_tr_decimal" )
		setObjectName("HILDON-WIDGET-hildon-accent-button");
	else
		setObjectName("HILDON-WIDGET-hildon-button-stylus");
#endif

	connect ( this, SIGNAL( clicked(bool) ), this, SLOT( clicked() ) );

}

void OssoCalculatorButton::clicked() {

	emit clicked(buttonName);

}

void OssoCalculatorButton::paintEvent(QPaintEvent * event) {

	if ( ! textColor.isEmpty() ) {

		QPalette palette = this->palette();

#ifdef Q_WS_MAEMO_5
		palette.setColor(QPalette::ButtonText, QMaemo5Style::standardColor(textColor));
#else
		if ( textColor == "AttentionColor" )
			palette.setColor(QPalette::ButtonText, QColor(227, 0, 42));
		else if ( textColor == "AccentColor1" )
			palette.setColor(QPalette::ButtonText, QColor(134, 213, 252));
#endif

		setPalette(palette);

	}

	QPushButton::paintEvent(event);

}

const QString & OssoCalculatorButton::name() {

	return buttonName;

}
