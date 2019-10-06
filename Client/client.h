#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "symbol.h"

class QHostAddress;
class QJsonDocument;

class Client : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Client)

public:
    explicit Client(QObject *parent = nullptr);
    QString getNickname();
    QString getUsername();
    void setNickname(const QString& nickname);
    QPixmap* getProfile();
    void setProfileImage(const QString& filename);
    void sendProfileImage();
    void sendProfileImage(const QString& name,QPixmap* pixmap);
    void overrideProfileImage(const QPixmap& pixmap);

    void getFiles();
    QMap<QString,QString> getActiveFiles();

    void sendJson(const QJsonObject& message);
    void createNewFile(QString filename);
    void closeFile();



public slots:
    void connectToServer(const QHostAddress &address, quint16 port);
    void login(const QString &username, const QString &password);
    void signup(const QString &username, const QString &password);
    void updateNickname(const QString &nickname);
    void updatePassword(const QString &oldpassword,const QString &newpassword);
    void checkOldPassword(const QString &old_password);
    void disconnectFromHost();
    void openFile(const QString& filename);


private slots:
    void onReadyRead();

signals:
    void connected();
    void loggedIn();
    void signedUp();
    void signupError(const QString &reason);
    void loginError(const QString &reason);
    void disconnected();
    void messageReceived(const QString &sender, const QString &text);
    void error(QAbstractSocket::SocketError socketError);
    void userJoined(const QString &username);
    void userLeft(const QString &username);
    void wrongOldPassword();
    void correctOldPassword();

    void filesReceived();
    void openFilesError(const QString& reason);

    void remoteInsert(Symbol s);
    void remoteErase(Symbol s);
    void correctNewFIle();
    void correctOpenedFile();
    void wrongNewFIle(const QString& reason);
    void wrongOpenedFile(const QString& reason);
    void usersConnectedReceived(QList<QPair<QString,QString>>);
    void contentReceived(const QString text);
     void userDisconnected(const QString& username);


private:
    QTcpSocket *m_clientSocket;
    bool m_loggedIn;
    QString username;   //valutare se puntatore o no!
    QString nickname;
    QPixmap* profile;
    QMap<QString,QString> files;
    QString openfile;

    void jsonReceived(const QJsonObject &doc);
};

#endif // CLIENT_H
