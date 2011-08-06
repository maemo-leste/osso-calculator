/*
    osso-calculator-button.h - OSSO Calculator
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

#ifndef OSSO_CALCULATOR_BUTTON_H
#define OSSO_CALCULATOR_BUTTON_H

#include <QPushButton>

class QString;
class QWidget;

class QPaintEvent;

class OssoCalculatorButton : public QPushButton {

	Q_OBJECT

	public:
		OssoCalculatorButton(const QString &name, QWidget * parent = NULL);
		const QString & name();

	private:
		QString buttonName;
		QString textColor;

	private slots:
		void clicked();

	protected:
		void paintEvent(QPaintEvent * event);

	signals:
		void clicked(const QString &button);

};

#endif
