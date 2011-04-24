/*
    osso-calculator.cpp - OSSO Calculator
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

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

#include <QMainWindow>
#include <QMenuBar>

#ifdef Q_WS_MAEMO_5
#include <QMaemo5InformationBox>
#endif

#include "osso-calculator.h"
#include "osso-calculator-ui.h"
#include "osso-intl.h"

OssoCalculator::OssoCalculator(QObject * parent) : QObject(parent) {

	window = new QMainWindow;
	ui = new OssoCalculatorUI(window);

	brackets = 0;
	memory = "0";

	connect ( ui, SIGNAL( clickedButton(const QString &) ), this, SLOT( clickedButton(const QString &) ) );
	window->setCentralWidget(ui);

}

OssoCalculator::~OssoCalculator() {

	delete window;

}

void OssoCalculator::show() {

	window->show();

}

QString OssoCalculator::executeCommand(const QString &command) {

	QProcess process(this);
	process.start("osso_calculator_engine", QStringList() << "-dp" << command);

	if ( ! process.waitForStarted() )
		return QString();

	process.closeReadChannel(QProcess::StandardError);
	process.closeWriteChannel();

	if ( ! process.waitForFinished() )
		return QString();

	QString output = QString::fromUtf8(process.readAll()).trimmed();

	if ( output.contains("Error 10001") ) {

#ifdef Q_WS_MAEMO_5
		QMaemo5InformationBox::information(window, _("calc_ap_divide_byzero"));
#endif
		return QString();

	}

	if ( output.contains("Raising to very large power") ) {

#ifdef Q_WS_MAEMO_5
		QMaemo5InformationBox::information(window, _("calc_ap_large_power"));
#endif
		return QString();

	}

	if ( output.contains("Not allowed the sqrt of negative number") ) {

#ifdef Q_WS_MAEMO_5
		QMaemo5InformationBox::information(window, _("calc_ap_square_neg"));
#endif
		return QString();

	}

	if ( process.exitCode() != 0 || output.isEmpty() || output.contains("Error") )
		return QString();

	return output;

}

void OssoCalculator::clickedButton(const QString &name) {

	if ( name.size() == 1 && name.at(0).isDigit() ) {

		ui->displayAppend(name);

	} else if ( name == "calc_bv_desk_ac" || name == "calc_bv_desk_c" ) {

		ui->displaySetText("0");
		ui->displayErase();

		if ( name == "calc_bv_desk_ac" )
			expression.clear();

	} else if ( name == "calc_bv_tr_back" ) {

		ui->displayRemoveLast();

	} else if ( name == "calc_bv_tr_openbracket" ) {

		expression += "( ";
		ui->historyAppendExpression(_(name) + ' ');
		++brackets;

	} else if ( name == "calc_bv_tr_closebracket" ) {

		if ( brackets > 0 ) {

			if ( ! ui->displayText().isEmpty() ) {

				expression += ui->displayText() + ' ';
				ui->historyAppendExpression(ui->displayText() + ' ');
				ui->displayErase();

			}

			expression += ") ";
			ui->historyAppendExpression(_(name) + ' ');
			--brackets;

		}

	} else if ( name == "calc_bv_tr_min" ) {

		memory = ui->displayText(false);
		ui->displayErase();

		if ( memory.isEmpty() || memory == "0" )
			ui->memoryDisable();
		else
			ui->memoryEnable();

	} else if ( name == "calc_bv_tr_mplus" ) {

		memory = executeCommand(memory + " + (" + ui->displayText(false) + ')');
		ui->displayErase();

		if ( memory.isEmpty() || memory == "0" )
			ui->memoryDisable();
		else
			ui->memoryEnable();

	} else if ( name == "calc_bv_tr_mminus" ) {

		memory = executeCommand(memory + " - (" + ui->displayText(false) + ')');
		ui->displayErase();

		if ( memory.isEmpty() || memory == "0" )
			ui->memoryDisable();
		else
			ui->memoryEnable();

	} else if ( name == "calc_bv_tr_mr" ) {

		if ( ! memory.isEmpty() && memory != "0" )
			ui->displaySetText(memory);

	} else if ( name == "calc_bv_tr_decimal" ) {

		if ( ui->displayText().isEmpty() )
			ui->displaySetText("0.");
		else
			ui->displaySetText(ui->displayText() + '.');

	} else if ( name == "calc_bv_tr_unaryminus" && ui->displayText() != "0" ) {

		if ( ui->displayText().startsWith('-') ) {

			int size = ui->displayText().size() - 1;
			ui->displaySetText(ui->displayText().right(size));

		} else if ( ! ui->displayText().isEmpty() ) {

			ui->displaySetText('-' + ui->displayText());

		}

	} else if ( name == "calc_bv_tr_reciprocal" ) {

		expression += "1 / (" + ui->displayText(false) + ") ";
		ui->historyAppendExpression(ui->displayText(false) + ' ' + _(name) + ' ');
		ui->displayErase();

	} else if ( name == "calc_bv_tr_powerof" ) {

		expression += '(' + ui->displayText(false) + ") ^ ";
		ui->historyAppendExpression(ui->displayText(false) + " ^ ");
		ui->displayErase();

	} else if ( name == "calc_bv_tr_sqroot" ) {

		expression += "sqrt(" + ui->displayText(false) + ") ";
		ui->historyAppendExpression(_(name) + ui->displayText(false) + ' ');
		ui->displayErase();

	} else if ( name == "calc_bv_tr_percent" ) {

		expression += '(' + ui->displayText(false) + ") * 0.01 * ";
		ui->historyAppendExpression(ui->displayText(false) + ' ' + _(name) + ' ');
		ui->displayErase();

	} else if ( name == "calc_bv_tr_add" ) {

		expression += '(' + ui->displayText(false) + ") + ";
		ui->historyAppendExpression(ui->displayText(false) + ' ' + _(name) + ' ');
		ui->displayErase();

	} else if ( name == "calc_bv_tr_subtract" ) {

		expression += '(' + ui->displayText(false) + ") - ";
		ui->historyAppendExpression(ui->displayText(false) + ' ' + _(name) + ' ');
		ui->displayErase();

	} else if ( name == "calc_bv_tr_multiply" ) {

		expression += '(' + ui->displayText(false) + ") * ";
		ui->historyAppendExpression(ui->displayText(false) + ' ' + _(name) + ' ');
		ui->displayErase();

	} else if ( name == "calc_bv_tr_divide" ) {

		expression += '(' + ui->displayText(false) + ") / ";
		ui->historyAppendExpression(ui->displayText(false) + ' ' + _(name) + ' ');
		ui->displayErase();

	} else if ( name == "calc_bv_tr_equals" ) {

		if ( ! ui->displayText().isEmpty() ) {

			expression += ui->displayText() + ' ';
			ui->historyAppendExpression(ui->displayText() + ' ');
			ui->displayErase();

		}

		while ( brackets > 0 ) {

			expression += ") ";
			ui->historyAppendExpression(_("calc_bv_tr_closebracket") + ' ');
			--brackets;

		}

		QString result = executeCommand(expression);
		expression.clear();

		if ( result.isEmpty() ) {

			ui->historyAppendResult("<b>" + _("calc_gi_e") + "</b>");
			ui->displaySetText("0");

		} else {

			ui->historyAppendResult(_(name) + ' ' + result);
			ui->displaySetText(result);

		}

		ui->displayErase();

	}

}
