//#include <QDebug>
#include <QStringList>
#include <QListIterator>
#include <SDL2/SDL_gamecontroller.h>

#include "mappinggcviewbackend.h"

const QString MappingGCViewBackend::HIGH_CUCKERY = QString("button.png");

MappingGCViewBackend::MappingGCViewBackend(QObject *parent) : QObject(parent)
{
    joyDevice = nullptr;
    createBindingData();

    connect(this, &MappingGCViewBackend::bindingAssignned, this, &MappingGCViewBackend::emptyCurrentTracked);
}

void MappingGCViewBackend::createBindingData()
{
    mappedBindings.append(new GCBinding(0, "a", InputBindType::None, SDL_CONTROLLER_BUTTON_A));
    mappedBindings.append(new GCBinding(1, "b", InputBindType::None, SDL_CONTROLLER_BUTTON_B));
    mappedBindings.append(new GCBinding(2, "x", InputBindType::None, SDL_CONTROLLER_BUTTON_X));
    mappedBindings.append(new GCBinding(3, "y", InputBindType::None, SDL_CONTROLLER_BUTTON_Y));
    mappedBindings.append(new GCBinding(4, "back", InputBindType::None, SDL_CONTROLLER_BUTTON_BACK));
    mappedBindings.append(new GCBinding(5, "guide", InputBindType::None, SDL_CONTROLLER_BUTTON_GUIDE));
    mappedBindings.append(new GCBinding(6, "start", InputBindType::None, SDL_CONTROLLER_BUTTON_START));
    mappedBindings.append(new GCBinding(7, "leftshoulder", InputBindType::None, SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
    mappedBindings.append(new GCBinding(8, "rightshoulder", InputBindType::None, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
    mappedBindings.append(new GCBinding(9, "leftstick", InputBindType::None, SDL_CONTROLLER_BUTTON_LEFTSTICK));
    mappedBindings.append(new GCBinding(10, "rightstick", InputBindType::None, SDL_CONTROLLER_BUTTON_RIGHTSTICK));

    mappedBindings.append(new GCBinding(11, "dpup", InputBindType::None, SDL_CONTROLLER_BUTTON_DPAD_UP));
    mappedBindings.append(new GCBinding(12, "dpleft", InputBindType::None, SDL_CONTROLLER_BUTTON_DPAD_LEFT));
    mappedBindings.append(new GCBinding(13, "dpdown", InputBindType::None, SDL_CONTROLLER_BUTTON_DPAD_DOWN));
    mappedBindings.append(new GCBinding(14, "dpright", InputBindType::None, SDL_CONTROLLER_BUTTON_DPAD_RIGHT));

    mappedBindings.append(new GCBinding(15, "leftx", InputBindType::None, SDL_CONTROLLER_BIND_AXIS_LEFTX_NEGATIVE));
    mappedBindings.append(new GCBinding(16, "leftx", InputBindType::None, SDL_CONTROLLER_BIND_AXIS_LEFTX_POSITIVE));

    mappedBindings.append(new GCBinding(17, "lefty", InputBindType::None, SDL_CONTROLLER_BIND_AXIS_LEFTY_NEGATIVE));
    mappedBindings.append(new GCBinding(18, "lefty", InputBindType::None, SDL_CONTROLLER_BIND_AXIS_LEFTY_POSITIVE));

    mappedBindings.append(new GCBinding(19, "rightx", InputBindType::None, SDL_CONTROLLER_BIND_AXIS_RIGHTX_NEGATIVE));
    mappedBindings.append(new GCBinding(20, "rightx", InputBindType::None, SDL_CONTROLLER_BIND_AXIS_RIGHTX_POSITIVE));

    mappedBindings.append(new GCBinding(21, "righty", InputBindType::None, SDL_CONTROLLER_BIND_AXIS_RIGHTY_NEGATIVE));
    mappedBindings.append(new GCBinding(22, "righty", InputBindType::None, SDL_CONTROLLER_BIND_AXIS_RIGHTY_POSITIVE));


    mappedBindings.append(new GCBinding(23, "lefttrigger", InputBindType::None, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
    mappedBindings.append(new GCBinding(24, "righttrigger", InputBindType::None, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

}

void MappingGCViewBackend::setActiveDevice(JoystickSDL* device)
{
    if (joyDevice)
    {
        disconnectConnections();
    }

    this->joyDevice = device;
}

bool MappingGCViewBackend::beginTracking()
{
    refreshTrackedAxes();
    return establishConnections();
}

bool MappingGCViewBackend::endTracking()
{
    return disconnectConnections();
}

bool MappingGCViewBackend::establishConnections()
{
    bool result = false;

    if (joyDevice)
    {
        connect(joyDevice, &JoystickSDL::buttonEvent, this, &MappingGCViewBackend::handleButtonEvent);
        connect(joyDevice, &JoystickSDL::axisEvent, this, &MappingGCViewBackend::handleAxisEvent);
        connect(joyDevice, &JoystickSDL::hatEvent, this, &MappingGCViewBackend::handleHatEvent);

        result = true;
    }

    return result;
}

bool MappingGCViewBackend::disconnectConnections()
{
    bool result = false;

    if (joyDevice)
    {
        disconnect(joyDevice, &JoystickSDL::buttonEvent, this, nullptr);
        disconnect(joyDevice, &JoystickSDL::axisEvent, this, nullptr);
        disconnect(joyDevice, &JoystickSDL::hatEvent, this, nullptr);

        result = true;
    }

    return result;
}


void MappingGCViewBackend::handleButtonEvent(int index, bool state)
{
    if (trackingButton == EMPTY_TRACKING_NUM && state)
    {
        trackingButton = index;
    }
    else if (trackingButton == index && !state)
    {
        GCBinding* tempBind = mappedBindings.at(mappingIndex);
        tempBind->bindType = InputBindType::Button;
        tempBind->bindData.button = index;

        trackingButton = EMPTY_TRACKING_NUM;

        emit bindingAssignned();
    }
}

void MappingGCViewBackend::handleAxisEvent(int index, int value)
{
    /*if (value >= TRACK_AXIS_THRESHOLD)
    {
        qDebug() << "WHO THAT IS " << value;
    }

    if (trackingAxis != EMPTY_TRACKING_NUM && trackingAxis == index)
    {
        qDebug() << "INDEX: " << index << " AXIS VALUE: " << qAbs(value) << " OG: " << value;
    }
    */

    bool isTrigger = joyDevice->isAxisTrigger(index);
    InputAxisInfo* tempInfo = trackedAxesStates.at(index);
    tempInfo->axisValue = value;

    if (!isTrigger)
    {
        if (trackingAxis == EMPTY_TRACKING_NUM && qAbs(value) >= TRACK_AXIS_THRESHOLD)
        {
            trackingAxis = index;
            trackedAxisvalue = value;
            trackingTrigger = isTrigger;

            tempInfo->isActive = true;
            tempInfo->highValue = value;

            //qDebug() << "TRACKING AXIS: " << index;
            //qDebug() << "CURRENT VALUE: " << value;
            //qDebug();
        }
        //else if (trackingAxis == index && qAbs(value) <= TRACK_AXIS_DEAD &&
        //         ((trackedAxisvalue >= 0) == (value >= 0)))
        else if (trackingAxis == index &&
                 qAbs(value - tempInfo->highValue) >= TRACK_AXIS_DIFF_THRESHOLD)
        {
            GCBinding* tempBind = mappedBindings.at(mappingIndex);
            tempBind->bindType = InputBindType::AxisDir;
            tempBind->bindData.axis.axis = index;
            tempBind->bindData.axis.dir = trackedAxisvalue >= 0 ? AxisPos : AxisNeg;
            tempBind->bindData.axis.base_value = rangeValue(value);
            tempBind->bindData.axis.max_value = rangeValue(trackedAxisvalue);

            trackingAxis = EMPTY_TRACKING_NUM;
            trackedAxisvalue = 0;
            trackingTrigger = false;

            tempInfo->isActive = false;
            tempInfo->highValue = 0;

            //qDebug() << "PASSED DEAD " << index;
            //qDebug() << "CURRENT VALUE: " << value;
            //qDebug();
            emit bindingAssignned();
        }
    }
    else
    {
        if (trackingAxis == EMPTY_TRACKING_NUM && value >= TRACK_AXIS_THRESHOLD)
        {
            trackingAxis = index;
            trackedAxisvalue = value;
            trackingTrigger = isTrigger;

            tempInfo->isActive = true;
            tempInfo->highValue = value;

            //qDebug() << "TRACKING AXIS: " << index;
            //qDebug() << "CURRENT VALUE: " << value;
            //qDebug();
        }
        else if (trackingAxis == index && value <= TRACK_AXIS_THRESHOLD)
        {
            GCBinding* tempBind = mappedBindings.at(mappingIndex);
            tempBind->bindType = InputBindType::Axis;
            tempBind->bindData.axis.axis = index;
            tempBind->bindData.axis.dir = AxisFull;
            tempBind->bindData.axis.base_value = rangeValue(value);
            tempBind->bindData.axis.max_value = rangeValue(trackedAxisvalue);

            trackingAxis = EMPTY_TRACKING_NUM;
            trackedAxisvalue = 0;
            trackingTrigger = false;

            tempInfo->isActive = false;
            tempInfo->highValue = 0;

            //qDebug() << "PASSED DEAD " << index;
            //qDebug() << "CURRENT VALUE: " << value;
            //qDebug();
            emit bindingAssignned();
        }
    }
}

void MappingGCViewBackend::handleHatEvent(int index, int value)
{
    if (trackingHat == EMPTY_TRACKING_NUM && value >= 0 && value % 3 != 0)
    {
        trackingHat = index;
        trackingHatValue = value;
    }
    else if (trackingHat == index && value == 0)
    {
        GCBinding* tempBind = mappedBindings.at(mappingIndex);
        tempBind->bindType = InputBindType::Hat;
        tempBind->bindData.dpad.hat = index;
        tempBind->bindData.dpad.hat_mask = trackingHatValue;

        trackingHat = EMPTY_TRACKING_NUM;
        trackingHatValue = 0;

        emit bindingAssignned();
    }
}

void MappingGCViewBackend::emptyCurrentTracked()
{
    trackingButton = EMPTY_TRACKING_NUM;
    trackingAxis = EMPTY_TRACKING_NUM;
    trackingHat = EMPTY_TRACKING_NUM;
    trackingHatValue = 0;
}

void MappingGCViewBackend::resetMappings()
{
    QListIterator<GCBinding*> iter(mappedBindings);
    while (iter.hasNext())
    {
        GCBinding* temp = iter.next();
        temp->bindType = InputBindType::None;
        temp->bindData = {0};
    }
}

void MappingGCViewBackend::resetBinding(int index)
{
    if (index >= 0 && index < mappedBindings.count())
    {
        GCBinding *tempBind = mappedBindings.at(index);
        tempBind->resetBind();
    }
}

QString MappingGCViewBackend::generateSDLBindings()
{
    QString result;
    QStringList tempList;

    tempList.append(joyDevice->getDeviceGUID());
    tempList.append(joyDevice->getMappedName());

    QListIterator<GCBinding*> iter(mappedBindings);
    while (iter.hasNext())
    {
        QString mapString;

        GCBinding* temp = iter.next();
        QString bindName = temp->bindName;

        if (temp->outputBind >= SDL_CONTROLLER_BUTTON_MAX)
        {
            switch (temp->outputBind) {
                case SDL_CONTROLLER_BIND_AXIS_LEFTX_NEGATIVE:
                case SDL_CONTROLLER_BIND_AXIS_LEFTY_NEGATIVE:
                case SDL_CONTROLLER_BIND_AXIS_RIGHTX_NEGATIVE:
                case SDL_CONTROLLER_BIND_AXIS_RIGHTY_NEGATIVE:
                {
                    bool skipNext = skipAdjacentAxisMapping(temp->index);
                    if (skipNext)
                    {
                         // Move iter to next binding here. Next run of while loop will skip it
                        iter.next();
                    }
                    else
                    {
                        bindName.prepend("-");
                    }

                    break;
                }

                case SDL_CONTROLLER_BIND_AXIS_LEFTX_POSITIVE:
                case SDL_CONTROLLER_BIND_AXIS_LEFTY_POSITIVE:
                case SDL_CONTROLLER_BIND_AXIS_RIGHTX_POSITIVE:
                case SDL_CONTROLLER_BIND_AXIS_RIGHTY_POSITIVE:
                    bindName.prepend("+");
                    break;

                default:
                    break;
            }
        }

        if (temp->bindType == InputBindType::Button)
        {
            mapString.append("b");
            mapString.append(QString::number(temp->bindData.button));
        }
        else if (temp->bindType == InputBindType::Axis)
        {
            mapString.append("a");
            mapString.append(QString::number(temp->bindData.axis.axis));

            // Check for invert here
            if (temp->bindData.axis.invert)
            {
                mapString.append("~");
            }
        }
        else if (temp->bindType == InputBindType::AxisDir)
        {
            // Place direction sign
            if (temp->bindData.axis.dir == AxisNeg)
            {
                mapString.append("-");
            }
            else if (temp->bindData.axis.dir == AxisPos)
            {
                mapString.append("+");
            }

            mapString.append("a");
            mapString.append(QString::number(temp->bindData.axis.axis));
        }
        else if (temp->bindType == InputBindType::Hat)
        {
            mapString.append("h");
            mapString.append(QString::number(temp->bindData.dpad.hat));
            mapString.append(".").append(QString::number(temp->bindData.dpad.hat_mask));
        }

        if (!mapString.isEmpty())
        {
            QString outputMapLabel = bindName;
            QString temp = QString("%1:%2").arg(outputMapLabel).arg(mapString);
            tempList.append(temp);
        }
    }

    tempList.append(QString("platform:").append(joyDevice->getPlatformName()));

    result = tempList.join(",").append(",");
    return result;
}

bool MappingGCViewBackend::skipAdjacentAxisMapping(int index)
{
    bool result = false;

    if (index >= SDL_CONTROLLER_BIND_AXIS_LEFTX_NEGATIVE &&
        index <= SDL_CONTROLLER_BIND_AXIS_RIGHTY_NEGATIVE)
    {
        GCBinding *current = mappedBindings.at(index);
        GCBinding *next = mappedBindings.at(index+1);
        if (current->bindType == InputBindType::AxisDir &&
            next->bindType == InputBindType::AxisDir &&
            current->bindData.axis.axis == next->bindData.axis.axis)
        {
            if (current->bindData.axis.dir != AxisFull && next->bindData.axis.dir != AxisFull &&
                current->bindData.axis.dir != next->bindData.axis.dir)
            {
                // Use full axis range in mapping
                current->bindType = InputBindType::Axis;
                next->bindType = InputBindType::Axis;

                // Find if axis dirs are inverted
                current->bindData.axis.base_value = current->bindData.axis.max_value;
                current->bindData.axis.max_value = next->bindData.axis.max_value;
                current->bindData.axis.invert = current->bindData.axis.base_value > current->bindData.axis.max_value;
                result = true;
            }
        }
    }

    return result;
}

int MappingGCViewBackend::rangeValue(int current)
{
    int value = 0;
    if (current > SDL_JOYSTICK_AXIS_MAX / 2)
    {
        value = SDL_JOYSTICK_AXIS_MAX;
    }
    else if (current < SDL_JOYSTICK_AXIS_MIN / 2)
    {
        value = SDL_JOYSTICK_AXIS_MIN;
    }

    return value;
}

void MappingGCViewBackend::refreshTrackedAxes()
{
    deleteTrackedAxes();
    for (int i = 0; i < joyDevice->getJoyNumAxes(); i++)
    {
        trackedAxesStates.append(new InputAxisInfo(i));
    }
}

void MappingGCViewBackend::deleteTrackedAxes()
{
    QListIterator<InputAxisInfo*> iter(trackedAxesStates);
    while (iter.hasNext())
    {
        InputAxisInfo* tempInfo = iter.next();
        delete tempInfo;
        tempInfo = nullptr;
    }

    trackedAxesStates.clear();
}

MappingGCViewBackend::~MappingGCViewBackend()
{
    QListIterator<GCBinding*> iter(mappedBindings);
    while (iter.hasNext())
    {
        GCBinding* temp = iter.next();
        delete temp;
        temp = nullptr;
    }

    mappedBindings.clear();

    deleteTrackedAxes();
}
