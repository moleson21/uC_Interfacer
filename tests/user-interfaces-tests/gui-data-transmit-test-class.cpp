/*
 * uC Interface - A GUI for Programming & Interfacing with Microcontrollers
 * Copyright (C) 2018  Mitchell Oleson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "gui-data-transmit-test-class.hpp"
#include "ui_gui-data-transmit.h"

#include <QTest>
#include <QSignalSpy>

GUI_DATA_TRANSMIT_TEST_CLASS::GUI_DATA_TRANSMIT_TEST_CLASS(QWidget *parent) :
    GUI_DATA_TRANSMIT(parent)
{
    // Retrieve ui object
    ui_ptr = get_ui();
}

GUI_DATA_TRANSMIT_TEST_CLASS::~GUI_DATA_TRANSMIT_TEST_CLASS()
{
    /* DO NOTHING */
}

QByteArray GUI_DATA_TRANSMIT_TEST_CLASS::rcvd_formatted_readAll_test()
{
    return rcvd_formatted_readAll();
}

qint64 GUI_DATA_TRANSMIT_TEST_CLASS::rcvd_formatted_size_test()
{
    return rcvd_formatted_size();
}

void GUI_DATA_TRANSMIT_TEST_CLASS::set_expected_recv_length_test(uint32_t expected_length)
{
    set_expected_recv_length(expected_length);
}

void GUI_DATA_TRANSMIT_TEST_CLASS::update_current_recv_length_test(uint32_t recv_len)
{
    update_current_recv_length(recv_len);
}

void GUI_DATA_TRANSMIT_TEST_CLASS::receive_gui_test(QByteArray data)
{
    receive_gui(data);
}

void GUI_DATA_TRANSMIT_TEST_CLASS::reset_clicked_test()
{
    // Setup spy to catch tranmit signal
    QList<QVariant> spy_args;
    QSignalSpy transmit_chunk_spy(this, transmit_chunk);
    QVERIFY(transmit_chunk_spy.isValid());

    // Click the reset button
    QTest::mouseClick(ui_ptr->ResetGUI_Button, Qt::LeftButton);
    qApp->processEvents();

    // Verify that reset signal emitted
    QCOMPARE(transmit_chunk_spy.count(), 1);
    spy_args = transmit_chunk_spy.takeFirst();
    QCOMPARE(spy_args.at(0).toInt(), (int) MAJOR_KEY_RESET);
    QCOMPARE(spy_args.at(1).toInt(), (int) 0);
}

void GUI_DATA_TRANSMIT_TEST_CLASS::set_checked_click_test(QCheckBox *check, bool b)
{
    if (b && !check->isChecked())
        QTest::mouseClick(check, Qt::LeftButton);
    else if (!b && check->isChecked())
        QTest::mouseClick(check, Qt::LeftButton);

    qApp->processEvents();
}
