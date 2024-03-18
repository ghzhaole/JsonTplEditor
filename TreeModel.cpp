#include "TreeModel.h"
#include "TreeItem.h"

#include <QtWidgets>

//! [0]
TreeModel::TreeModel(QObject *parent): QAbstractItemModel(parent)
{

}

TreeModel::TreeModel(const QVariantMap& discData, const QVariant &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem("root", discData, data);
}
//! [0]

//! [1]
TreeModel::~TreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}
//! [2]

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

//! [3]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    TreeItem *item = getItem(index);
    if (item->isRoot())
        return Qt::ItemIsEnabled;
    else if (item->isReadOnly())
        return  Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
//! [3]

//! [4]
TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}
//! [4]

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section) {
        case 0:
            return "name";
            break;
        case 1:
            return "type";
            break;
        case 2:
            return "value";
            break;
        default:
            break;
        }
    }

    return QVariant();
}

//! [5]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
    //! [5]

    //! [6]
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}
//! [6]

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool TreeModel::insertRows(int position, int rows,
                           const QModelIndex &parent){
    QModelIndex index = this->index(parent.row(), 0 ,parent.parent());
    TreeItem *parentItem = getItem(parent);
    if (parentItem->type() != ItemType::kTypeList){
        if(parentItem->parent()){
            index = parent.parent();
            position = parentItem->childNumber()+1;
            parentItem = parentItem->parent();
        }else{
            return false;
        }
    }else{
        position = parentItem->childCount();
    }

    if(parentItem->type() != ItemType::kTypeList){
        return false;
    }
    beginInsertRows(index, position, position + rows - 1);
    auto parDisc = parentItem->disc();
    QVariantMap disc = parDisc.isEmpty()?QVariantMap():parDisc["value"].toMap();
    for(int i=0;i<rows;i++){
        parentItem->insertChildren(position, QString::number(parentItem->childCount()>0?(parentItem->child(parentItem->childCount()-1)->name().toInt()+1):0), disc, QVariant());
    }
    endInsertRows();
    emit dataChanged(this->index(position, 0, index), this->index(position+rows - 1, 2, index), {Qt::DisplayRole, Qt::EditRole});
    return true;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem || parentItem->type() != ItemType::kTypeList)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    emit dataChanged(createIndex(position, 0, parentItem), createIndex(position, 1, parentItem), {Qt::DisplayRole, Qt::EditRole});
    return success;
}

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    const TreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}
//! [8]

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

QModelIndex TreeModel::buddy(const QModelIndex &index) const {
    if (index.isValid() && index.column() <= 1)
        return createIndex(index.row(), 2, index.internalPointer());
    return index;
}

QVariant TreeModel::getData()
{
    return rootItem->getData();
}

void TreeModel::setData(const QVariantMap &disc,const QVariant &data)
{
    auto lastRoot = rootItem;
    rootItem = new TreeItem("root", disc, data);
}
