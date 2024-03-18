#include "jsontplloader.h"
#include <QJsonDocument>
#include <QFile>

JsonTplLoader::JsonTplLoader(QObject *parent)
    : QObject{parent}
{

}

QJsonObject JsonTplLoader::loadFromFile(const QString &fileName)
{
    QFile loadFile(fileName);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open projects json";
        return QJsonObject();
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));

    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        return QJsonObject();
    }

    QJsonObject rootObj = jsonDoc.object();
    return rootObj;
}
