#ifndef TREEITEMDELEGATE_H
#define TREEITEMDELEGATE_H

#include <QItemDelegate>

class TreeItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TreeItemDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    virtual void updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

signals:
    void sigEditArr(const QString& name,const QVariantMap& disc, QVariant* data);

};

#endif // TREEITEMDELEGATE_H
