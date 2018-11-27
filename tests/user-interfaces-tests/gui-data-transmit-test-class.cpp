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

GUI_DATA_TRANSMIT_TEST_CLASS::GUI_DATA_TRANSMIT_TEST_CLASS(QWidget *parent) :
    GUI_DATA_TRANSMIT(parent)
{
    /* DO NOTHING */
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

void GUI_DATA_TRANSMIT_TEST_CLASS::rcvd_formatted_clear_test()
{
    rcvd_formatted_clear();
}

void GUI_DATA_TRANSMIT_TEST_CLASS::set_expected_recv_length_test(uint32_t expected_length)
{
    set_expected_recv_length(expected_length);
}

void GUI_DATA_TRANSMIT_TEST_CLASS::update_current_recv_length_test(uint32_t recv_len)
{
    update_current_recv_length(recv_len);
}