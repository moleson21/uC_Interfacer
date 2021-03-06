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

#ifndef GUI_BASE_H
#define GUI_BASE_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QTemporaryFile>

#include "gui-base-major-keys.h"
#include "../gui-helpers/gui-generic-helper.hpp"

class GUI_BASE : public QWidget
{
    Q_OBJECT

public:
    GUI_BASE(QWidget *parent = 0);
    ~GUI_BASE();

    virtual bool isClosable();
    virtual void setClosable(bool new_close);

    virtual uint8_t get_gui_key();
    virtual QString get_gui_name();
    virtual QVariant get_gui_map_value(QString key);

    virtual QString get_gui_tab_name();
    virtual void set_gui_tab_name(QString new_name);

    virtual QString get_gui_config();

    virtual bool acceptAllCMDs();

    virtual void parseConfigMap(QMap<QString, QVariant> *configMap);

    virtual bool waitForDevice(uint8_t minorKey);

signals:
    // Read updates
    void readyRead(QByteArray data);

    // File transmit
    void transmit_file(quint8 major_key, quint8 minor_key,
                       QString filePath, quint8 base = 0,
                       QString encoding = "^(.*)");
    void transmit_file_pack(quint8 major_key, quint8 minor_key,
                            QString filePath, quint8 base = 0,
                            QString encoding = "^(.*)");

    // Chunk transmit
    void transmit_chunk(quint8 major_key, quint8 minor_key,
                        QByteArray chunk = QByteArray(), quint8 base = 0,
                        QString encoding = "^(.*)");
    void transmit_chunk_pack(quint8 major_key, quint8 minor_key,
                             QByteArray chunk = QByteArray(), quint8 base = 0,
                             QString encoding = "^(.*)");

    // Send progress bar updates
    void progress_update_recv(int progress, QString label);
    void progress_update_send(int progress, QString label);

public slots:
    // Resets the GUI (must not call reset_remote())
    virtual void reset_gui();

protected slots:
    // Receive and parse data
    virtual void receive_gui(QByteArray recvData);

    // Reset the gui and connected device
    virtual void on_ResetGUI_Button_clicked();

    // Set progress bar updates
    virtual void set_progress_update_recv(int progress, QString label);
    virtual void set_progress_update_send(int progress, QString label);

protected:
    // Set gui values
    virtual void set_gui_key(uint8_t new_key);
    virtual void set_gui_name(QString new_name);
    virtual void set_gui_map_value(QString key, QVariant value);

    // Save & clear rcvd file
    void rcvd_formatted_append(QByteArray data);
    void rcvd_formatted_save(QString fileName = "");
    QByteArray rcvd_formatted_readAll();
    uint32_t rcvd_formatted_size();
    void rcvd_formatted_clear();

    // Set recv lengths
    void set_expected_recv_length(uint32_t expected_length);
    void update_current_recv_length(uint32_t recv_len);

private:
    // GUI Variables
    uint8_t gui_key;
    QString gui_name;

    // Holder for complete config map
    CONFIG_MAP *gui_config;
    QMap<QString, QVariant> *gui_map;

    // Receive arrays & variables
    QTemporaryFile rcvd_formatted;
    uint32_t current_recv_length;
    uint32_t expected_recv_length;
    QString expected_recv_length_str;

    // Other functions
    bool init_maps();
    bool rcvd_formatted_isOpen();
};

#endif // GUI_BASE_H
