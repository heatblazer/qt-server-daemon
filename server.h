#ifndef SERVER_H
#define SERVER_H

// qt bases
#include <QObject>

// multithreading
#include <QThread>

// network tools
#include <QTcpSocket>
#include <QTcpServer>


namespace qdaemon {
// an extra interface to imeplement the unix 32 signals may be needed but for now
// I`ll let it be, I won`t handle signals
class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(int sockDescr, QObject* parent = 0);
    virtual ~MyThread();

    // override the thread runnable
    void run() Q_DECL_OVERRIDE;

signals:
    void error(QTcpSocket::SocketError socketError);

public slots:
    void hReadyRead(void);
    void hDisconnected(void);

private:
    int         m_socket_fd;
    QByteArray  m_data;
    QTcpSocket*  p_socket;
};


class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject* parent = 0);
    virtual ~MyServer();
    void startServer(void);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
    QString m_message;
};

} // namespace qdaemon

#endif // TASK_H
