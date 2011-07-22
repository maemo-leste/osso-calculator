/*
    osso-calculator-frame.h - OSSO Calculator
    Copyright (C) 2011  Pali Rohár <pali.rohar@gmail.com>
    Copyright (C) 2011  Ivaylo Dimitrov <freemangordon@abv.bg>

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

#ifndef OSSO_CALCULATOR_FRAME_H
#define OSSO_CALCULATOR_FRAME_H

#include <QFrame>

class QWidget;
class QPaintEvent;

class OssoCalculatorFrame : public QFrame {

	Q_OBJECT

	public:
		explicit OssoCalculatorFrame(QWidget * parent = 0);

	protected:
		void paintEvent(QPaintEvent *);

};

#endif
