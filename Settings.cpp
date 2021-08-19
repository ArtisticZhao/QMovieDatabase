#include "Settings.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

Settings::Settings() {
	openOrCreate();
}

void Settings::openOrCreate() {
	QFile file("settings.json");
	if (file.exists()) {
		// read settings value
		file.open(QIODevice::ReadOnly);
		QByteArray data = file.readAll();
		file.close();
		QJsonParseError parseError;
		QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
		if (parseError.error != QJsonParseError::NoError) {
			qDebug() << data << "parse error";
			return;
		}
		QJsonObject obj = doc.object();
		if (obj.contains("DBPath") && obj["DBPath"].isString()) {
			dbPath = obj["DBPath"].toString();
		}
		else {
			dbPath = "DB.sqlite";
		}

		if (obj.contains("PlayerPath") && obj["PlayerPath"].isString()) {
			playerPath = obj["PlayerPath"].toString();
		}
		else {
			playerPath = "";
		}
	}
	else {
		// Create settings.json with default value;
		QJsonObject obj;
		obj.insert("DBPath", QJsonValue("DB.sqlite"));
		obj.insert("PlayerPath", QJsonValue("potplayer.exe"));

		QJsonDocument doc(obj);
		QByteArray data = doc.toJson();
		bool ok = file.open(QIODevice::WriteOnly);
		if (ok) {
			file.write(data);
			file.close();
		}
		else {
			qDebug() << "write error!" << endl;
		}
	}
}
