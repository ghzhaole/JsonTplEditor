#ifndef JSONTPLLOADER_H
#define JSONTPLLOADER_H

#include <QObject>
#include <QJsonObject>

class JsonTplLoader : public QObject
{
    Q_OBJECT
public:
    explicit JsonTplLoader(QObject *parent = nullptr);
    static QJsonObject loadFromFile(const QString& fileName);

signals:

};

#endif // JSONTPLLOADER_H
