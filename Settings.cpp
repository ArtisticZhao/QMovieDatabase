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

		if (obj.contains("fileFilter") && obj["fileFilter"].isString()) {
			fileFilter = obj["fileFilter"].toString();
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
		obj.insert("fileFilter", QJsonValue("avi wmv wmp wm asf mpg mpeg mpe m1v m2v mpv2 mp2v ts tp tpr trp vob ifo ogm ogv mp4 m4v m4p m4b 3gp 3gpp 3g2 3gp2 mkv rm ram rmvb rpm flv mov qt nsv dpg m2ts m2t mts dvr-ms k3g skm evo nsr amv divx webm wtv f4v mxf"));

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
