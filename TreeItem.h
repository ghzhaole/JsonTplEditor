#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QVector>

enum class ItemType{
    kTypeNull = 0,
    kTypeFundamental,
    kTypeList,
    kTypeMap
};

//! [0]
class TreeItem
{
public:
    explicit TreeItem(const QString& name, const QVariant &data, TreeItem *parent = nullptr);
    explicit TreeItem(const QString& name, const QVariantMap &discData,const QVariant &data, TreeItem *parent = nullptr);
    ~TreeItem();

    TreeItem *child(int number);
    int childCount() const;
    QVariant data(int column) const;
    QString name() const {return itemName;}
    ItemType type() const {return itemType; };
    QVariantMap disc() const {return itemDisc; };
    bool isRoot() const {return !parentItem; };
    bool isReadOnly() const { return itemType!=ItemType::kTypeFundamental;};
    TreeItem * insertChildren(int position, TreeItem* childItem);
    TreeItem * insertChildren(int position, const QString& name, const QVariantMap &discData, const QVariant& value);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    bool setData(const QVariant &value);
    QVariant getData();

private:
    QVector<TreeItem*> childItems;
    QVariant itemData;
    QVariantMap itemDisc;
    TreeItem *parentItem{nullptr};
    ItemType itemType;
    QString itemName;
};
//! [0]

#endif // TREEITEM_H
