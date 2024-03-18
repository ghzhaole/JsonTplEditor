#include "TreeItemDelegate.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
TreeItemDelegate::TreeItemDelegate(QObject *parent)
    : QItemDelegate{parent}
{

}

QWidget *TreeItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget *editor = 0;
    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
    if (item->type() == ItemType::kTypeFundamental) {
        switch (item->getData().type()) {
        case QVariant::ULongLong:
            editor = new QSpinBox(parent);
            ((QSpinBox*)editor)->setMinimum(0);
            ((QSpinBox*)editor)->setMaximum(INT_MAX);
            break;
        case QVariant::LongLong:
            editor = new QSpinBox(parent);
            ((QSpinBox*)editor)->setMinimum(INT_MIN);
            ((QSpinBox*)editor)->setMaximum(INT_MAX);
            break;
        case QMetaType::Float:
            editor = new QDoubleSpinBox(parent);
            ((QDoubleSpinBox*)editor)->setMinimum(-3.40e+38);
            ((QDoubleSpinBox*)editor)->setMaximum(3.40e+38);
            ((QDoubleSpinBox*)editor)->setDecimals(6);
            break;
        case QVariant::Double:
            editor = new QDoubleSpinBox(parent);
            ((QDoubleSpinBox*)editor)->setMinimum(-1.79e+308);
            ((QDoubleSpinBox*)editor)->setMaximum(1.79e+308);
            ((QDoubleSpinBox*)editor)->setDecimals(9);
            break;
        default:
            editor = QItemDelegate::createEditor(parent, option, index);
            break;
        }
    }
    return editor;

}

void TreeItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
    switch (item->getData().type()) {
    case QVariant::LongLong:{
        static_cast<QSpinBox *>(editor)->setValue(item->getData().toLongLong());
        break;
    }
    case QVariant::ULongLong:{
        static_cast<QSpinBox *>(editor)->setValue(item->getData().toULongLong());
        break;
    }
    case QMetaType::Float:{
        static_cast<QDoubleSpinBox *>(editor)->setValue(item->getData().toFloat());
        break;
    }
    case QVariant::Double:{
        static_cast<QDoubleSpinBox *>(editor)->setValue(item->getData().toDouble());
        break;
    }
    default:
        QItemDelegate::setEditorData(editor, index);
        break;
    }
}

void TreeItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
    switch (item->getData().type()) {
    case QVariant::LongLong:
    case QVariant::ULongLong: {
        model->setData(index, static_cast<QSpinBox *>(editor)->value(), Qt::EditRole);
        break;
    }
    case QMetaType::Float:
    case QVariant::Double: {
        model->setData(index, static_cast<QDoubleSpinBox *>(editor)->value(), Qt::EditRole);
        break;
    }
    default:{
        QItemDelegate::setModelData(editor, model, index);
        break;
    }
    }
}

void TreeItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::updateEditorGeometry(editor, option, index);
}

QSize TreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QItemDelegate::sizeHint(option, index);
    return QSize(size.width(), 28);
}
