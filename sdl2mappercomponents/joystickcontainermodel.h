#ifndef JOYSTICKCONTAINERMODEL_H
#define JOYSTICKCONTAINERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QHash>

#include "joystickcontainer.h"

class JoystickContainerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum JoystickRoles
    {
        NameRole = Qt::UserRole + 1,
        ExpandedRole,
    };

    JoystickContainerModel(JoystickContainer *joyCont, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE void refreshItem(int index);

private:
    JoystickContainer *joyContainer;

private slots:
    void nameChanged();
};



#endif // JOYSTICKCONTAINERMODEL_H
