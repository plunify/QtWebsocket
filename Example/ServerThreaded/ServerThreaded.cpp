/*
Copyright 2013 Antoine Lafarge qtwebsocket@gmail.com

This file is part of QtWebsocket.

QtWebsocket is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

QtWebsocket is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with QtWebsocket.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ServerThreaded.h"
#include <iostream>

ServerThreaded::ServerThreaded()
{
	int port = 80;
	server = new QtWebsocket::QWsServer(this);
	if (! server->listen(QHostAddress::Any, port))
	{
		std::cout << QObject::tr("Error: Can't launch server").toStdString() << std::endl;
		std::cout << QObject::tr("QWsServer error : %1").arg(server->errorString()).toStdString() << std::endl;
	}
	else
	{
		std::cout << QObject::tr("Server is listening port %1").arg(port).toStdString() << std::endl;
	}
	QObject::connect(server, SIGNAL(newConnection()), this, SLOT(processNewConnection()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(broadcastHello()));
    timer->start(1000);
}

ServerThreaded::~ServerThreaded()
{
}

void ServerThreaded::processNewConnection()
{
    std::cout << QObject::tr("Client connected.").toStdString() << "\n";


	// Get the connecting socket
	QtWebsocket::QWsSocket* socket = server->nextPendingConnection();

    QString peer = socket->peerAddress().toString() + ":" + QString::number(socket->peerPort());
    addPeer(peer);
    //std::cout << "List of clients:\n" << peers.join("\n").toStdString() << "\n";

	// Create a new thread and giving to him the socket
	SocketThread* thread = new SocketThread(socket);
	
	// connect for message broadcast
	QObject::connect(socket, SIGNAL(frameReceived(QString)), this, SIGNAL(broadcastMessage(QString)));
    QObject::connect(thread, SIGNAL(disconnected(QString)), this, SLOT(removePeer(QString)));
	QObject::connect(this, SIGNAL(broadcastMessage(QString)), thread, SLOT(sendMessage(QString)));

	// Starting the thread
	thread->start();
}

void ServerThreaded::broadcastHello()
{
    QString message;
    message += QDateTime::currentDateTime().toString() + "\n";
    message += "List of clients:\n" + peers.join("\n") + "\n";
    emit broadcastMessage(message);
}

void ServerThreaded::addPeer(QString name)
{
    peers << name;
}

void ServerThreaded::removePeer(QString name)
{
    peers.removeOne(name);
}
