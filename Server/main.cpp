#include "serverwindow.h"
#include <QApplication>
#include <QDebug>
#include <stdlib.h>
#include "mongo.h"

#define PORT 1500

int main(int argc, char *argv[])
{
	Mongo db;
//	qDebug() << db.signup("enrico@gmail.com", "Admin11.");

	QString nick;
//	qDebug() << db.login("enrico@gmail.com", "Admin11.", nick);
//	qDebug() << nick;

//	qDebug() << db.updateNickname("enrico@gmail.com", "enrico");
//	qDebug() << db.checkOldPassword("enrico@gmail.com", "enrico");
//	qDebug() << db.checkOldPassword("enrico@gmail.com", "Admin11.");

	QString link;
//	qDebug() << db.newFile("enrico@gmail.com", "testfile3", link);
//	qDebug() << link;

//	qDebug() << db.getSharedLink("enrico@gmail.com", "testfile", link);
//	qDebug() << link;
//	qDebug() << db.getSharedLink("enrico@gmail.com", "testfile2", link);
//	qDebug() << link;
//	qDebug() << db.getSharedLink("enrico@gmail.com", "testfile3", link);
//	qDebug() << link;

//	qDebug() << db.updatePassword("enrico@gmail.com", "aaa", "bb");
//	qDebug() << db.updatePassword("enrico@gmail.com", "Admin11.", "password");
//	qDebug() << db.login("enrico@gmail.com", "Admin11.", nick);
//	qDebug() << db.login("enrico@gmail.com", "password", nick);

//	qDebug() << db.updatePassword("enrico@gmail.com", "password", "Admin11.");

//	qDebug() << db.getFilenameFromSharedLink("enrico@gmail.com", nick, "user");
//	qDebug() << nick;
	qDebug() << db.getFilenameFromSharedLink("shared_editor://file/sb98zc76GYCXv34jMMpzgOZ0ehvRCo", nick, "enrico@gmail.com");
//	qDebug() << nick;

	QApplication a(argc, argv);
	quint16 port = PORT;

	QStringList args = QCoreApplication::arguments();
	if (args.length() != 1 && args.length() != 2) {
        //qDebug() << "Usage: ./Server <port_number>";
        //qDebug() << "If no argument provided port 1500 is used.";
		exit(-1);
	}

	if (args.length() == 2) {
		bool ok;
		unsigned int tmp = args.at(1).toUInt(&ok);
		if (ok) {
			port = tmp;
		}
	}

	ServerWindow w(nullptr, port);
	w.show();
	return a.exec();
}
