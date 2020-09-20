#include "joystickcontainermodel.h"

JoystickContainerModel::JoystickContainerModel(JoystickContainer *joyCont, QObject *parent) :
    QAbstractListModel(parent)
{
    this->joyContainer = joyCont;

    /*for (int i = 0; i < joyCont->getNumberJoysticks(); i++)
    {
        JoystickSDL* tempJoy = joyCont->getJoystick(i);
        connect(tempJoy, &JoystickSDL::mappedNameChanged, this, &JoystickContainerModel::nameChanged);
    }
    */
}

int JoystickContainerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return joyContainer->getNumberJoysticks();
}

QVariant JoystickContainerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= joyContainer->getNumberJoysticks())
    {
        return QVariant();
    }

    JoystickSDL* joystick = joyContainer->getJoystick(index.row());
    switch (role) {
        case Qt::DisplayRole:
        case NameRole:
        {
            QString temp = joystick->getMappedName();
            if (temp.isEmpty())
            {
                temp = joystick->getDeviceName();
            }

            return QVariant::fromValue(temp);
            break;
        }
        case ExpandedRole:
        {
            QStringList tempList;

            QString temp = joystick->getMappedName();
            if (temp.isEmpty())
            {
                temp = joystick->getDeviceName();
            }

            tempList.append(temp);
            if (joystick->isMappingAvailable())
            {
                temp = "(mapping available)";
                tempList.append(temp);
            }

            temp = tempList.join(" ");
            return QVariant::fromValue(temp);
        }
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> JoystickContainerModel::roleNames() const
{
    static QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    static bool rolesPopulated = false;
    if (!rolesPopulated)
    {
        roles[NameRole] = "name";
        roles[ExpandedRole] = "details";
        rolesPopulated = true;
    }

    return roles;
}

void JoystickContainerModel::refreshItem(int index)
{
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
}

void JoystickContainerModel::nameChanged()
{
    JoystickSDL* joy = (JoystickSDL*)this->sender();
    int index = joy->getJoyIndex();
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
}
