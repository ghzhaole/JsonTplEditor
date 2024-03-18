#ifndef JSONEDITOR_H
#define JSONEDITOR_H

#include <QWidget>
#include <QVariant>

QT_BEGIN_NAMESPACE
namespace Ui { class JsonEditor; }
QT_END_NAMESPACE

class JsonEditor : public QWidget
{
    Q_OBJECT

public:
    JsonEditor(QWidget *parent = nullptr);
    ~JsonEditor();

private:
    Ui::JsonEditor *ui;

    QVariant m_;
};
#endif // JSONEDITOR_H
