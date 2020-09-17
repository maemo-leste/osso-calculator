/*
    osso-calculator-ui.cpp - OSSO Calculator
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
#include <QByteArray>
#include <QHash>
#include <QProcess>
#include <QVariant>
#include <QSettings>

#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextBrowser>
#include <QLineEdit>
#include <QScrollBar>
#include <QShowEvent>

#include "osso-calculator.h"
#include "osso-calculator-ui.h"
#include "osso-calculator-button.h"
#include "osso-calculator-frame.h"
#include "osso-intl.h"

#ifdef Q_WS_MAEMO_5
#include "osso-screenshot.h"
#endif

struct buttonLayout {

	int fromRow;
	int fromColumn;
	int rowSpan;
	int columnSpan;

};

struct buttonDef {

	QByteArray name;
	int shortcut;
	buttonLayout basic;
	buttonLayout basicPortrait;
	buttonLayout scientific;
	buttonLayout scientificPortrait;

};

static const struct buttonDef buttonsDef[] = {

	{ "calc_bv_desk_ac",         -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 0, 0, 1, 1 }, { 0, 0, 1, 1 } },
	{ "calc_bv_desk_c",          Qt::Key_C,         { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 0, 1, 1, 1 }, { 0, 1, 1, 1 } },
	{ "calc_bv_tr_back",         Qt::Key_Backspace, {  0,  0, 1, 1 }, {  4,  2, 1, 1 }, { 5, 0, 1, 1 }, { 5, 0, 1, 1 } },

	{ "calc_bv_tr_openbracket",  Qt::Key_H,         { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 0, 2, 1, 1 }, { 0, 2, 1, 1 } },
	{ "calc_bv_tr_closebracket", Qt::Key_J,         { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 0, 3, 1, 1 }, { 0, 3, 1, 1 } },

	{ "calc_bv_tr_min",          -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 1, 0, 1, 1 }, { 1, 0, 1, 1 } },
	{ "calc_bv_tr_mplus",        -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 2, 0, 1, 1 }, { 2, 0, 1, 1 } },
	{ "calc_bv_tr_mminus",       -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 3, 0, 1, 1 }, { 3, 0, 1, 1 } },
	{ "calc_bv_tr_mr",           -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 4, 0, 1, 1 }, { 4, 0, 1, 1 } },

	{ "0",                       Qt::Key_P,         {  4,  1, 1, 2 }, {  4,  0, 1, 1 }, { 5, 1, 1, 1 }, { 5, 1, 1, 1 } },
	{ "1",                       Qt::Key_Q,         {  3,  1, 1, 1 }, {  3,  0, 1, 1 }, { 4, 1, 1, 1 }, { 4, 1, 1, 1 } },
	{ "2",                       Qt::Key_W,         {  3,  2, 1, 1 }, {  3,  1, 1, 1 }, { 4, 2, 1, 1 }, { 4, 2, 1, 1 } },
	{ "3",                       Qt::Key_E,         {  3,  3, 1, 1 }, {  3,  2, 1, 1 }, { 4, 3, 1, 1 }, { 4, 3, 1, 1 } },
	{ "4",                       Qt::Key_R,         {  2,  1, 1, 1 }, {  2,  0, 1, 1 }, { 3, 1, 1, 1 }, { 3, 1, 1, 1 } },
	{ "5",                       Qt::Key_T,         {  2,  2, 1, 1 }, {  2,  1, 1, 1 }, { 3, 2, 1, 1 }, { 3, 2, 1, 1 } },
	{ "6",                       Qt::Key_Y,         {  2,  3, 1, 1 }, {  2,  2, 1, 1 }, { 3, 3, 1, 1 }, { 3, 3, 1, 1 } },
	{ "7",                       Qt::Key_U,         {  1,  1, 1, 1 }, {  1,  0, 1, 1 }, { 2, 1, 1, 1 }, { 2, 1, 1, 1 } },
	{ "8",                       Qt::Key_I,         {  1,  2, 1, 1 }, {  1,  1, 1, 1 }, { 2, 2, 1, 1 }, { 2, 2, 1, 1 } },
	{ "9",                       Qt::Key_O,         {  1,  3, 1, 1 }, {  1,  2, 1, 1 }, { 2, 3, 1, 1 }, { 2, 3, 1, 1 } },
	{ "calc_bv_tr_decimal",      Qt::Key_Period,    {  4,  3, 1, 1 }, {  4,  1, 1, 1 }, { 5, 2, 1, 1 }, { 5, 2, 1, 1 } },
	{ "calc_bv_tr_unaryminus",   -1,                {  0,  1, 1, 1 }, {  0,  0, 1, 1 }, { 5, 3, 1, 1 }, { 5, 3, 1, 1 } },

	{ "calc_bv_tr_reciprocal",   -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 0, 4, 1, 1 }, { 0, 4, 1, 1 } },
	{ "calc_bv_tr_powerof",      -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } },
	{ "calc_bv_tr_sqroot",       -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 1, 2, 1, 1 }, { 1, 2, 1, 1 } },
	{ "calc_bv_tr_percent",      -1,                { -1, -1, 1, 1 }, { -1, -1, 1, 1 }, { 1, 3, 1, 1 }, { 1, 3, 1, 1 } },

	{ "calc_bv_tr_divide",       Qt::Key_V,         {  0,  2, 1, 1 }, {  0,  1, 1, 1 }, { 1, 4, 1, 1 }, { 1, 4, 1, 1 } },
	{ "calc_bv_tr_multiply",     Qt::Key_A,         {  0,  3, 1, 1 }, {  0,  2, 1, 1 }, { 2, 4, 1, 1 }, { 2, 4, 1, 1 } },
	{ "calc_bv_tr_subtract",     Qt::Key_F,         {  0,  4, 1, 1 }, {  0,  3, 1, 1 }, { 3, 4, 1, 1 }, { 3, 4, 1, 1 } },
	{ "calc_bv_tr_add",          Qt::Key_S,         {  1,  4, 2, 1 }, {  1,  3, 2, 1 }, { 4, 4, 1, 1 }, { 4, 4, 1, 1 } },
	{ "calc_bv_tr_equals",       Qt::Key_Comma,     {  3,  4, 2, 1 }, {  3,  3, 2, 1 }, { 5, 4, 1, 1 }, { 5, 4, 1, 1 } },

	{ QByteArray(), 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }

};

OssoCalculatorUI::OssoCalculatorUI(QMainWindow * window) {

	display = new QLineEdit(this);
	historyFrame = new OssoCalculatorFrame(this);
	historyLayout = new QVBoxLayout(historyFrame);
	history = new QTextBrowser(historyFrame);

	displayEmpty = true;
	numericDisabled = false;

	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	portrait = screenGeometry.width() < screenGeometry.height();

	basic = QSettings(this).value("basicMode", true).toBool();

	QActionGroup * mode = new QActionGroup(window);
	QAction * basicMode = new QAction(_("calc_me_basic"), mode);
	QAction * scientificMode = new QAction(_("calc_me_scientific"), mode);
	QAction * clearHistory = new QAction(_("calc_me_clear_till_roll"), window);

	mode->setExclusive(true);
	basicMode->setCheckable(true);
	basicMode->setChecked(basic);
	basicMode->setObjectName("basicMode");
	scientificMode->setCheckable(true);
	scientificMode->setChecked(!basic);
	scientificMode->setObjectName("scientificMode");

	window->menuBar()->addActions(mode->actions());
	window->menuBar()->addAction(clearHistory);
	window->setWindowTitle(_("calc_ap_title_calculator"));

#ifdef Q_WS_MAEMO_5
    window->setProperty("X-Maemo-Orientation", 2); // XXX: 2 = auto orientation
#endif

	for ( int i = 0; ! buttonsDef[i].name.isEmpty(); ++i ) {

		OssoCalculatorButton * button = new OssoCalculatorButton(buttonsDef[i].name, this);
		button->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
		button->hide();

		if ( buttonsDef[i].shortcut != -1 )
			button->setShortcut(QKeySequence(buttonsDef[i].shortcut));

#ifdef Q_WS_MAEMO_5
		if ( buttonsDef[i].name == "calc_bv_tr_back" ) {

			QPixmap icon("/usr/share/icons/hicolor/32x32/hildon/general_backspace.png");

			if ( ! icon.isNull() ) {

				button->setIcon(icon);
				button->setText(QString());

			}

		}
#endif

		connect ( button, SIGNAL( clicked(const QString &) ), this, SIGNAL( clickedButton(const QString &) ) );

		buttons.insert(buttonsDef[i].name, button);

	}

	memoryDisable();

	display->setReadOnly(true);
	display->setAlignment(Qt::AlignRight);
	displaySetText("0");
	displayErase();

	historyLayout->setSpacing(0);
	historyLayout->setContentsMargins(4, 4, 4, 4);
	historyLayout->addWidget(history);

	QPalette palette = history->palette();
	palette.setColor(QPalette::Text, palette.color(QPalette::WindowText));
	palette.setColor(QPalette::Base, palette.color(QPalette::Window));
	history->setPalette(palette);
	history->setMinimumWidth(265);
	history->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

	historyList << QString();

	connect ( mode, SIGNAL( triggered(QAction *) ), this, SLOT( modeChanged(QAction *) ) );
	connect ( clearHistory, SIGNAL( triggered(bool) ), this, SLOT( historyErase() ) );
	connect ( QApplication::desktop(), SIGNAL( resized(int) ), this, SLOT( orientationChanged() ) );

	createLayout();

}

OssoCalculatorUI::~OssoCalculatorUI() {

	deleteLayout();

}

#ifdef Q_WS_MAEMO_5
void OssoCalculatorUI::takeScreenshot(bool take) {

	windowTakeScreenshot(parentWidget()->winId(), take);

}
#endif

void OssoCalculatorUI::orientationChanged() {

	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	portrait = screenGeometry.width() < screenGeometry.height();

	redraw();

}

void OssoCalculatorUI::modeChanged(QAction * action) {

	if ( ! action->isChecked() )
		return;

	basic = action->objectName() == "basicMode";
	QSettings(this).setValue("basicMode", basic);

#ifdef Q_WS_MAEMO_5
	takeScreenshot(false);
#endif

	redraw();

}

void OssoCalculatorUI::deleteLayout() {

	foreach ( OssoCalculatorButton * button, buttons )
		button->hide();

	delete buttonsLayout;
	delete displayLayout;
	delete centralLayout;

}

void OssoCalculatorUI::redraw() {

	hide();
	deleteLayout();
	createLayout();
	show();

}

void OssoCalculatorUI::createLayout() {

	displayLayout = new QVBoxLayout;
	buttonsLayout = new QGridLayout;

	displayLayout->setContentsMargins(0, 17, 0, 10);
	displayLayout->addWidget(display);
	displayLayout->addWidget(historyFrame);

	if ( ! portrait )
		buttonsLayout->setContentsMargins(10, 17, 0, 10);

	buttonsLayout->setSpacing(9);

	for ( int i = 0; ! buttonsDef[i].name.isEmpty(); ++i ) {

		buttonLayout layout;

		if ( basic && ! portrait )
			layout = buttonsDef[i].basic;
		else if ( basic && portrait )
			layout = buttonsDef[i].basicPortrait;
		else if ( ! basic && ! portrait )
			layout = buttonsDef[i].scientific;
		else
			layout = buttonsDef[i].scientificPortrait;

		if ( layout.fromRow == -1 || layout.fromColumn == -1 )
			continue;

		OssoCalculatorButton * button = buttons.value(buttonsDef[i].name);
		buttonsLayout->addWidget(button, layout.fromRow, layout.fromColumn, layout.rowSpan, layout.columnSpan);
		button->show();

	}

	if ( portrait )
		centralLayout = new QVBoxLayout(this);
	else
		centralLayout = new QHBoxLayout(this);

	centralLayout->addLayout(displayLayout);
	centralLayout->addLayout(buttonsLayout);

	adjustSize();

	history->verticalScrollBar()->setValue(history->verticalScrollBar()->maximum());

}

void OssoCalculatorUI::displayAppend(const QString &num) {

	if ( displayEmpty )
		displaySetText(num);
	else
		displaySetText(display->text() + num);

}

void OssoCalculatorUI::displayRemoveLast() {

	if ( displayEmpty || display->text().size() <= 1 || ( display->text().size() <= 2 && display->text().at(0) == '-' ) ) {

		displaySetText("0");
		displayErase();

	} else {

		int size = display->text().size() - 1;
		displaySetText(display->text().left(size));

	}

}

QString OssoCalculatorUI::displayText(bool getEmpty) {

	if ( displayEmpty && getEmpty )
		return QString();
	else
		return display->text();

}

void OssoCalculatorUI::displaySetText(const QString &text) {

	display->setText(text);
	displayEmpty = false;

	if ( displayIsFull() )
		numericDisable();
	else
		numericEnable();

}

void OssoCalculatorUI::displayErase() {

	displayEmpty = true;
	numericEnable();

}

bool OssoCalculatorUI::displayIsFull() {

	if ( displayEmpty )
		return false;

	if ( ( display->text().at(0) == '-' && display->text().size() >= 13 )
	|| ( display->text().at(0) != '-' && display->text().size() >= 12 ) )
		return true;
	else
		return false;

}

void OssoCalculatorUI::numericEnable() {

	if ( ! numericDisabled )
		return;

	foreach ( OssoCalculatorButton * button, buttons )
		if ( button->name().at(0).isDigit() || button->name() == "calc_bv_tr_decimal" )
			button->setEnabled(true);

	numericDisabled = false;

}

void OssoCalculatorUI::numericDisable() {

	if ( numericDisabled )
		return;

	foreach ( OssoCalculatorButton * button, buttons )
		if ( button->name().at(0).isDigit() || button->name() == "calc_bv_tr_decimal" )
			button->setEnabled(false);

	numericDisabled = true;

}

void OssoCalculatorUI::memoryEnable() {

	buttons.value("calc_bv_tr_mr")->setEnabled(true);

}

void OssoCalculatorUI::memoryDisable() {

	buttons.value("calc_bv_tr_mr")->setEnabled(false);

}

void OssoCalculatorUI::historyAppendExpression(const QString &expr) {

	historyList.last() += expr;
	historyRedraw();

}

void OssoCalculatorUI::historyAppendResult(const QString &res) {

	historyList << res << QString();
	historyRedraw();

}

void OssoCalculatorUI::historyRemoveLast() {

	historyList.last().clear();
	historyRedraw();

}

void OssoCalculatorUI::historyRedraw() {

	QString html;
	QStringList::Iterator it = historyList.begin();

	while ( true ) {

		html += "<p style=\"margin-top:0px; margin-bottom:0px;\">" + it->trimmed() + "</p>";

		if ( ++it == historyList.end() )
			break;

		html += "<p align=\"right\" style=\"margin-top:0px; margin-bottom:0px;\">" + it->trimmed() + "</p>";

		if ( ++it == historyList.end() )
			break;

	}

	history->setHtml(html);
	history->verticalScrollBar()->setValue(history->verticalScrollBar()->maximum());
	history->moveCursor(QTextCursor::End);

}

void OssoCalculatorUI::historyErase() {

	history->clear();
	historyList.clear();
	historyList << QString();

}
