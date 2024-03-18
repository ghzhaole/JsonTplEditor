#include "JsonEditor.h"
#include "jsontplloader.h"

#include "ui_JsonEditor.h"

#include<QJsonDocument>
#include <QVariant>

JsonEditor::JsonEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JsonEditor)
{
    ui->setupUi(this);

    m_ =  QVariant(QVariantMap());

    QVariantMap jsonDiscMap = JsonTplLoader::loadFromFile(":/example.json").toVariantMap();
    ui->treeView->setData(jsonDiscMap, m_);

    connect(ui->treeView,&TreeWidget::sigDataChanged,[=]{
        QString s(QJsonDocument::fromVariant(QVariant(ui->treeView->getData())).toJson());
        ui->textEdit->setText(s);
    });
}

JsonEditor::~JsonEditor()
{
    delete ui;
}

