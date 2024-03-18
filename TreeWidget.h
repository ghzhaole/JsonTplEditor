#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeView>
#include "TreeModel.h"

class TreeWidget : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeWidget(QWidget *parent = nullptr);

    void setData(const QVariantMap &disc, const QVariant &data);

    QVariant getData();

private:
    TreeModel* model_{nullptr};

public slots:
//    void sltEditArr(const QString& name,const QVariantMap& disc,const QVariant& data);

    void insertRow();
    void onTreeViewMenuRequested(const QPoint &pos);
    void removeRow();
signals:
    void sigDataChanged();

};

#endif // TREEWIDGET_H
