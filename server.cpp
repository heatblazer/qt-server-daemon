#include "server.h"

#include "logger.h" // logging

#include <QDataStream>
namespace qdaemon {


MyServer::MyServer(QObject *parent)
    : QTcpServer(parent)
{

}

MyServer::~MyServer()
{

}

void MyServer::startServer()
{
    int port = 1234;
    if (!this->listen(QHostAddress::Any, port)) {
        logger::logMessage("Could not start server!\n");
        exit(1);
    } else {
        logger::logMessage(QString("Listening to port...%1\n").arg(port));
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    logger::logMessage(QString("%1 connecting... \n").arg(socketDescriptor));
    MyThread* pThread =
            new MyThread(socketDescriptor, this);

    connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
    pThread->start();
}

// thrad
MyThread::MyThread(int sockDescr, QObject *parent)
    : QThread(parent), m_socket_fd(sockDescr)
{

}

MyThread::~MyThread()
{

}

/** overriding the run form thread
 * @brief MyThread::run
 */
void MyThread::run()
{

    logger::logMessage(QString("Thread %1 id starterd...\n").
                       arg(*(int*)QThread::currentThreadId()));

    p_socket = new QTcpSocket();
    if (!p_socket->setSocketDescriptor(m_socket_fd)) {
        logger::logMessage(QString("Error: could not set sock descriptor\n"));
        emit error(p_socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    // This makes the slot to be invoked immediately, when the signal is emitted.
    connect(p_socket, SIGNAL(readyRead()),
            this, SLOT(readyRead()), Qt::DirectConnection);

    connect(p_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    logger::logMessage(QString("%1 Client connected\n").arg(m_socket_fd));

    exec(); // make this thread an event loop


}

void MyThread::hReadyRead()
{
//    if (p_socket->canReadLine())
    QByteArray data = p_socket->readAll();
    QString msg;
    msg.append("Writing data...\n");
    msg.append("%1").arg(m_socket_fd);
    msg.append(QString(" Data in: %1 \n")).arg(QString(data));
    logger::logMessage(msg);
    p_socket->write(data);
}

void MyThread::hDisconnected()
{
    logger::logMessage(QString("%1 Disconnected!\n").arg(m_socket_fd));
    p_socket->deleteLater();
    exit(0);
}

} // namespace qdaemon

