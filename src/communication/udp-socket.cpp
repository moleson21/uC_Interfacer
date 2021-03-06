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

#include "udp-socket.hpp"
#include <QNetworkDatagram>

UDP_SOCKET::UDP_SOCKET(QString client_ip, int client_port, int server_port, QObject *parent) :
    COMMS_BASE(parent)
{
    // Create new client and server
    client = new QUdpSocket(this);
    server = new QUdpSocket(this);
    initSuccess = (initSuccess && client && server);
    if (!initSuccess) return;

    // Set values
    udp_client_ip = QHostAddress(client_ip);
    udp_client_port = client_port;
    udp_server_port = server_port;

    connect(server, SIGNAL(readyRead()),
            this, SLOT(read()),
            Qt::DirectConnection);
    connect(server, SIGNAL(disconnected()),
            this, SLOT(disconnectClient()),
            Qt::DirectConnection);
}

UDP_SOCKET::~UDP_SOCKET()
{
    if (isConnected()) close();

    delete client;
    delete server;
}

void UDP_SOCKET::open()
{
    // Attempt to bind port
    bool connected = server->bind(udp_server_port);
    if (!connected && !server->waitForConnected(1000))
        emit deviceDisconnected();
    else
        emit deviceConnected();
}

bool UDP_SOCKET::isConnected()
{
    return (server && (server->state() == QUdpSocket::BoundState));
}

void UDP_SOCKET::close()
{
    // Remove close slot to prevent infinite loop
    disconnect(server, SIGNAL(disconnected()),
            this, SLOT(disconnectClient()));

    // Disconnect
    server->disconnectFromHost();
}

void UDP_SOCKET::disconnectClient()
{
    emit deviceDisconnected();
}

void UDP_SOCKET::write(QByteArray writeData)
{
    // Acquire Lock
    writeLock->lock();

    // Write data (try to force start)
    client->writeDatagram((const QByteArray) writeData,
                          udp_client_ip, udp_client_port);
    client->flush();

    // Unlock lock
    writeLock->unlock();
}

void UDP_SOCKET::read()
{
    // Acquire Lock
    readLock->lock();

    // Read data
    QByteArray recvData;
    while (server->hasPendingDatagrams())
    {
        recvData += server->receiveDatagram().data();
    }
    // Emit signal
    emit readyRead(recvData);

    // Unlock lock
    readLock->unlock();
}
