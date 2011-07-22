/*
    osso-calculator-ui.h - OSSO Calculator
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

#ifndef OSSO_CALCULATOR_UI
#define OSSO_CALCULATOR_UI

#include <QWidget>
#include <QHash>

class QString;
class QMainWindow;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QLineEdit;
class QTextBrowser;
class QShowEvent;

class OssoCalculator;
class OssoCalculatorButton;
class OssoCalculatorFrame;

class OssoCalculatorUI : public QWidget {

	Q_OBJECT

	public:
		OssoCalculatorUI(QMainWindow * window);
		~OssoCalculatorUI();

		void displayAppend(const QString &num);
		void displayRemoveLast();
		QString displayText(bool getEmpty = true);
		void displaySetText(const QString &text);
		void displayErase();
		bool displayIsFull();

		void numericDisable();
		void numericEnable();

		void memoryEnable();
		void memoryDisable();

		void historyAppendExpression(const QString &expr);
		void historyAppendResult(const QString &res);
		void historyRemoveLast();
		void historyRedraw();

	public slots:
#ifdef Q_WS_MAEMO_5
		void takeScreenshot(bool take = true);
#endif
		void historyErase();

	private slots:
		void orientationChanged();
		void modeChanged(QAction * action);

	private:
		QBoxLayout * centralLayout;

		QVBoxLayout * displayLayout;
		QLineEdit * display;
		QStringList historyList;

		QVBoxLayout * historyLayout;
		OssoCalculatorFrame * historyFrame;
		QTextBrowser * history;

		QGridLayout * buttonsLayout;
		QHash <QString, OssoCalculatorButton *> buttons;

		bool basic;
		bool portrait;
		bool numericDisabled;
		bool displayEmpty;

		void deleteLayout();
		void createLayout();
		void redraw();

	signals:
		void clickedButton(const QString &name);

};

#endif
