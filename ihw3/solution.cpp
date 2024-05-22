#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout> // Добавлен заголовок

class ClientApp : public QWidget
{
    Q_OBJECT

public:
    ClientApp(QWidget *parent = nullptr) : QWidget(parent)
    {
        tcpSocket = new QTcpSocket(this);

        connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientApp::readData);

        connectButton = new QPushButton("Connect", this);
        connect(connectButton, &QPushButton::clicked, this, &ClientApp::connectToServer);

        sendButton = new QPushButton("Send Request", this);
        connect(sendButton, &QPushButton::clicked, this, &ClientApp::sendRequest);

        textEdit = new QTextEdit(this);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(connectButton);
        layout->addWidget(sendButton);
        layout->addWidget(textEdit);

        setLayout(layout);
    }

private slots:
    void connectToServer()
    {
        tcpSocket->connectToHost("127.0.0.1", 8080);
    }

    void sendRequest()
    {
        QString request = "Дикарь: Я хочу съесть миссионера\n"; // Добавлен символ новой строки
        tcpSocket->write(request.toUtf8());
    }

    void readData()
    {
        QByteArray data = tcpSocket->readAll();
        QString dataStr = QString::fromUtf8(data); // Преобразование данных в строку
        textEdit->append(dataStr);
    }

private:
    QTcpSocket *tcpSocket;
    QPushButton *connectButton;
    QPushButton *sendButton;
    QTextEdit *textEdit;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ClientApp clientApp;
    clientApp.show();

    return app.exec();
}

#include "main.moc"
