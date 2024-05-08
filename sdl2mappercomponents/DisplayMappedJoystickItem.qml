import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import sdl2mappercomponents 1.0

/* Component used in main view to display a currently bound mapping. Displays background image and
  control image for detected input controls */
Rectangle {

    id: root

    property bool joydisplayActive: false
    property bool bindDisplayActive: false
    property var joystick
    property QtObject logger

    //color: "red"

    QtObject
    {
        id: privateData
        property var buttonHighlight: {"source": "/images/button.png", "width": 28, "height": 28}
        property var axisHighlight: {"source": "/images/axis.png", "width": 29, "height": 29}
        property var axisLeftHighlight: {"source": "/images/left-arrow-small.png", "width": 40, "height": 29}
        property var axisRightHighlight: {"source": "/images/right-arrow-small.png", "width": 40, "height": 29}
        property var axisUpHighlight: {"source": "/images/up-arrow-small.png", "width": 29, "height": 40}
        property var axisDownHighlight: {"source": "/images/down-arrow-small.png", "width": 29, "height": 40}
        property var highlightImgArray: []
        property var activeControlsSet: new Set()

        property int buttonType: 0;
        property int axisXType: 1;
        property int axisYType: 2;
    }

    DisplayMappedJoystickBackend
    {
        id: displayMappedJoyBackend

        onBindExecuted: function(bindIndex) {
            privateData.activeControlsSet.add(bindIndex)
            showHighlightImage(bindIndex);
        }

        onBindReleased: function(bindIndex) {
            var element = privateData.highlightImgArray[bindIndex];
            element.visible = false;
            element.x = 0;
            element.y = 0;
            privateData.activeControlsSet.delete(bindIndex);
        }

        function showHighlightImage(bindIndex)
        {
            var element = privateData.highlightImgArray[bindIndex];
            var origInfo = highlightInfo.get(bindIndex);
            var scaleX = (backgroundXboxImg.paintedWidth / backgroundXboxImg.origWidth);
            var scaleY = (backgroundXboxImg.paintedHeight / backgroundXboxImg.origHeight);
            element.x = origInfo.x * scaleX + ((backgroundXboxImg.width - backgroundXboxImg.paintedWidth) / 2.0);
            element.y = origInfo.y * scaleY + ((backgroundXboxImg.height - backgroundXboxImg.paintedHeight) / 2.0);
            element.width = origInfo.width * scaleX;
            element.height = origInfo.height * scaleY;
            element.visible = true;
        }
    }

    ListModel
    {
        id: highlightInfo

        /*Component.onCompleted:
        {
            console.log("LISTMODEL COMPLETED");
        }
        */

        function populateModel()
        {
            // SDL_CONTROLLER_BUTTON_A
            highlightInfo.append({x: 225, y: 96, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_B
            highlightInfo.append({x: 252, y: 77, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_X
            highlightInfo.append({x: 200, y: 77, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_Y
            highlightInfo.append({x: 227, y: 59, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_BACK
            highlightInfo.append({x: 102, y: 77, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_GUIDE
            highlightInfo.append({x: 137, y: 77, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_START
            highlightInfo.append({x: 169, y: 77, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_LEFTSTICK
            highlightInfo.append({x: 44, y: 90, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_RIGHTSTICK
            highlightInfo.append({x: 179, y: 135, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_LEFTSHOULDER
            highlightInfo.append({x: 45, y: 23, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
            highlightInfo.append({x: 232, y: 21, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});

            // SDL_CONTROLLER_BUTTON_DPAD_UP
            highlightInfo.append({x: 90, y: 110, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_DPAD_DOWN
            highlightInfo.append({x: 90, y: 146, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_DPAD_LEFT
            highlightInfo.append({x: 68, y: 127, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_DPAD_RIGHT
            highlightInfo.append({x: 109, y: 127, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});

            // Add empty entries for possible unknown buttons
            for (var i = highlightInfo.count; i < displayMappedJoyBackend.sdlMaxButtons; i++)
            {
                highlightInfo.append({x: 0, y: 0, width: 0, height: 0, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source})
            }

            // SDL_CONTROLLER_AXIS_LEFTX-
            highlightInfo.append({x: 15, y: 89, width: privateData.axisLeftHighlight.width, height: privateData.axisLeftHighlight.height, buttonType: privateData.axisXType, source: privateData.axisLeftHighlight.source});
            // SDL_CONTROLLER_AXIS_LEFTX+
            highlightInfo.append({x: 58, y: 89, width: privateData.axisRightHighlight.width, height: privateData.axisRightHighlight.height, buttonType: privateData.axisXType, source: privateData.axisRightHighlight.source});
            // SDL_CONTROLLER_AXIS_LEFTY-
            highlightInfo.append({x: 43, y: 64, width: privateData.axisUpHighlight.width, height: privateData.axisUpHighlight.height, buttonType: privateData.axisYType, source: privateData.axisUpHighlight.source});
            // SDL_CONTROLLER_AXIS_LEFTY+
            highlightInfo.append({x: 43, y: 104, width: privateData.axisDownHighlight.width, height: privateData.axisDownHighlight.height, buttonType: privateData.axisYType, source: privateData.axisDownHighlight.source});
            // SDL_CONTROLLER_AXIS_RIGHTX-
            highlightInfo.append({x: 154, y: 137, width: privateData.axisLeftHighlight.width, height: privateData.axisLeftHighlight.height, buttonType: privateData.axisXType, source: privateData.axisLeftHighlight.source});
            // SDL_CONTROLLER_AXIS_RIGHTX+
            highlightInfo.append({x: 194, y: 137, width: privateData.axisRightHighlight.width, height: privateData.axisRightHighlight.height, buttonType: privateData.axisXType, source: privateData.axisRightHighlight.source});
            // SDL_CONTROLLER_AXIS_RIGHTY-
            highlightInfo.append({x: 179, y: 110, width: privateData.axisUpHighlight.width, height: privateData.axisUpHighlight.height, buttonType: privateData.axisYType, source: privateData.axisUpHighlight.source});
            // SDL_CONTROLLER_AXIS_RIGHTY+
            highlightInfo.append({x: 179, y: 150, width: privateData.axisDownHighlight.width, height: privateData.axisDownHighlight.height, buttonType: privateData.axisYType, source: privateData.axisDownHighlight.source});

            // SDL_CONTROLLER_AXIS_TRIGGERLEFT
            highlightInfo.append({x: 53, y: 0, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_AXIS_TRIGGERRIGHT
            highlightInfo.append({x: 220, y: 0, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});

            // Add empty entries for possible unknown axes
            for (var j = highlightInfo.count; j < (displayMappedJoyBackend.sdlMaxAxes + displayMappedJoyBackend.sdlMaxButtons); j++)
            {
                highlightInfo.append({x: 0, y: 0, width: 0, height: 0, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source})
            }
        }
    }

    Component
    {
        id: highlightImgComp
        Image
        {
            visible: false
            fillMode: Image.PreserveAspectFit
        }
    }

    Image
    {
        id: backgroundXboxImg
        source: "/images/controllermap.png"
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit

        property int origWidth: 300;
        property int origHeight: 186;

        //width: 300
        //height: 186

        property int origWidth: 300;
        property int origHeight: 186;
    }

    /*Image
    {
        id: highlightImg
        //width: 50
        //height: 50
        x: 0
        y: 0
        width: 0
        height: 0
        //fillMode: Image.PreserveAspectFit
        visible: bindDisplayActive

        function establishLateBindings()
        {
            highlightImg.x = Qt.binding(function()
            {
               return highlightInfo.get(privateData.currentHighlight).x;
            });

            highlightImg.y = Qt.binding(function()
            {
               return highlightInfo.get(privateData.currentHighlight).y;
            });

            highlightImg.source = Qt.binding(function()
            {
                return highlightInfo.get(privateData.currentHighlight).source;
            });

            highlightImg.width = Qt.binding(function()
            {
                return highlightInfo.get(privateData.currentHighlight).width;
            });

            highlightImg.height = Qt.binding(function()
            {
                return highlightInfo.get(privateData.currentHighlight).height;
            });

            /*highlightImg.rotation = Qt.binding(function()
            {
                var result = 0;
                var highlightItem = highlightInfo.get(privateData.currentHighlight);
                if (highlightItem.buttonType === privateData.axisYType)
                {
                    result = 90;
                }

                return result;
            });

        }

        Component.onCompleted: {
            establishLateBindings();
        }
    }
    */

    /*Image
    {
        id: highlightImgA
        Component.onCompleted: {
            var item = highlightInfo.get(0)
            highlightImg.x = item.x;
            highlightImg.y = item.y;
            highlightImg.source = item.source;
            highlightImg.width = item.width;
            highlightImg.height = item.height;
        }
    }


    Image
    {
        id: highlightImgB
        Component.onCompleted: {
            var item = highlightInfo.get(1)
            highlightImg.x = item.x;
            highlightImg.y = item.y;
            highlightImg.source = item.source;
            highlightImg.width = item.width;
            highlightImg.height = item.height;
        }
    }

    Image
    {
        id: highlightImgX
        Component.onCompleted: {
            var item = highlightInfo.get(2)
            highlightImg.x = item.x;
            highlightImg.y = item.y;
            highlightImg.source = item.source;
            highlightImg.width = item.width;
            highlightImg.height = item.height;
        }
    }

    Image
    {
        id: highlightImgY
        Component.onCompleted: {
            var item = highlightInfo.get(3)
            highlightImg.x = item.x;
            highlightImg.y = item.y;
            highlightImg.source = item.source;
            highlightImg.width = item.width;
            highlightImg.height = item.height;
        }
    }
    */

    function setHighlightButton(index)
    {
        privateData.currentHighlight = index;
    }

    function resetHighlight()
    {
        joydisplayActive = false;
        privateData.currentHighlight = 0;
    }

    function setJoyDevice(device)
    {
        displayMappedJoyBackend.setActiveDevice(device);
    }

    function enableDisplay()
    {
        joydisplayActive = true;
        displayMappedJoyBackend.establishConnections();
    }

    function disableDisplay()
    {
        joydisplayActive = false;
        displayMappedJoyBackend.disconnectConnections();
        for (var i = 0; i < privateData.highlightImgArray.length; i++)
        {
            var element = privateData.highlightImgArray[i];
            element.visible = false;
        }

        privateData.activeControlsSet.clear();
    }

    Component.onCompleted: {
        highlightInfo.populateModel();

        for (var i = 0; i < highlightInfo.count; i++)
        {
            var item = highlightInfo.get(i);
            //console.log(item);
            //console.log(item.x);
            var currentHighlightImg = highlightImgComp.createObject(root,
                {x: item.x, y: item.y, width: item.width, height: item.height, source: item.source});
            //console.log(currentHighlightImg);
            privateData.highlightImgArray.push(currentHighlightImg);
        }
    }

    onWidthChanged: function()
    {
        if (joydisplayActive)
        {
            for (const key of privateData.activeControlsSet.keys())
            {
                displayMappedJoyBackend.showHighlightImage(key);
            }
        }
    }

    onHeightChanged: function()
    {
        if (joydisplayActive)
        {
            for (const key of privateData.activeControlsSet.keys())
            {
                displayMappedJoyBackend.showHighlightImage(key);
            }
        }
    }
}
