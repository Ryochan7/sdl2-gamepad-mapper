import QtQuick 2.0
import QtQuick.Controls 2.12

import sdl2mappercomponents 1.0

Rectangle
{
    //height: 200
    //width: 50
    //color: "#FF00FF"
    //height: 186
    //width: 300
    //clip: true

    signal mappingFinished()

    property bool mappingActive: false
    property bool atListStart: privateData.currentHighlight === 0
    property bool atListEnd: privateData.currentHighlight === highlightInfo.count-1;
    property QtObject logger

    QtObject
    {
        id: privateData
        property int currentHighlight: 0
        property var buttonHighlight: {"source": "/images/button.png", "width": 28, "height": 28}
        property var axisHighlight: {"source": "/images/axis.png", "width": 29, "height": 29}
        property var axisLeftHighlight: {"source": "/images/left-arrow-small.png", "width": 40, "height": 29}
        property var axisRightHighlight: {"source": "/images/right-arrow-small.png", "width": 40, "height": 29}
        property var axisUpHighlight: {"source": "/images/up-arrow-small.png", "width": 29, "height": 40}
        property var axisDownHighlight: {"source": "/images/down-arrow-small.png", "width": 29, "height": 40}
        property int buttonType: 0;
        property int axisXType: 1;
        property int axisYType: 2;
    }

    MappingGCViewBackend
    {
        id: mappingGCBackend

        onBindingAssignned: {
            if (atListEnd)
            {
                mappingFinished();
            }
            else
            {
                activeNextHighlightButton();
            }
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
            // SDL_CONTROLLER_BUTTON_LEFTSHOULDER
            highlightInfo.append({x: 45, y: 23, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
            highlightInfo.append({x: 232, y: 21, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_LEFTSTICK
            highlightInfo.append({x: 44, y: 90, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_RIGHTSTICK
            highlightInfo.append({x: 179, y: 135, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});

            // SDL_CONTROLLER_BUTTON_DPAD_UP
            highlightInfo.append({x: 90, y: 110, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_DPAD_LEFT
            highlightInfo.append({x: 68, y: 127, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_DPAD_DOWN
            highlightInfo.append({x: 90, y: 146, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});
            // SDL_CONTROLLER_BUTTON_DPAD_RIGHT
            highlightInfo.append({x: 109, y: 127, width: privateData.buttonHighlight.width, height: privateData.buttonHighlight.height, buttonType: privateData.buttonType, source: privateData.buttonHighlight.source});

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
    }

    Image
    {
        id: highlightImg
        //width: 50
        //height: 50
        x: 0
        y: 0
        width: 0
        height: 0
        //fillMode: Image.PreserveAspectFit
        fillMode: Image.PreserveAspectFit
        visible: mappingActive

        function establishLateBindings()
        {
            /*highlightImg.x = Qt.binding(function()
            {
               return highlightInfo.get(privateData.currentHighlight).x;
            });

            highlightImg.y = Qt.binding(function()
            {
               return highlightInfo.get(privateData.currentHighlight).y;
            });
            */

            highlightImg.source = Qt.binding(function()
            {
                return highlightInfo.get(privateData.currentHighlight).source;
            });

            /*highlightImg.width = Qt.binding(function()
            {
                return highlightInfo.get(privateData.currentHighlight).width;
            });

            highlightImg.height = Qt.binding(function()
            {
                return highlightInfo.get(privateData.currentHighlight).height;
            });
            */

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
            */
        }
    }

    /*Image
    {
        source: "/images/up-arrow-small.png"
        //rotation: -90
        //fillMode: Image.PreserveAspectCrop
        x: 90
        y: 100
        width: 29
        height: 40
    }
    */

    function setHighlightButton(index)
    {
        privateData.currentHighlight = index;
        var origInfo = highlightInfo.get(privateData.currentHighlight);
        var scaleX = (backgroundXboxImg.paintedWidth / backgroundXboxImg.origWidth);
        var scaleY = (backgroundXboxImg.paintedHeight / backgroundXboxImg.origHeight);

        highlightImg.x = origInfo.x * scaleX + ((backgroundXboxImg.width - backgroundXboxImg.paintedWidth) / 2.0);
        highlightImg.y = origInfo.y * scaleY + ((backgroundXboxImg.height - backgroundXboxImg.paintedHeight) / 2.0);
        highlightImg.width = origInfo.width * scaleX;
        highlightImg.height = origInfo.height * scaleY;
        mappingActive = true;
    }

    function activeNextHighlightButton()
    {
        var temp = privateData.currentHighlight
        if (temp >= 0 && temp < highlightInfo.count)
        {
            if (temp < highlightInfo.count-1)
            {
                temp++;
                mappingGCBackend.currentMapIndex = temp;
                setHighlightButton(temp);
            }
            else
            {
                mappingFinished();
            }
        }
    }

    function activatePrevHighlightButton()
    {
        var temp = privateData.currentHighlight
        if (temp > 0 && temp < highlightInfo.count)
        {
            temp--;
            mappingGCBackend.currentMapIndex = temp;
            mappingGCBackend.resetBinding(temp);
            setHighlightButton(temp);
        }
    }

    function resetHighlight()
    {
        mappingActive = false;
        privateData.currentHighlight = 0;
        mappingGCBackend.currentMapIndex = 0;
    }

    function setJoyDevice(device)
    {
        mappingGCBackend.setActiveDevice(device);
    }

    function startMapping()
    {
        mappingActive = true;
        privateData.currentHighlight = 0;
        mappingGCBackend.currentMapIndex = 0;
        mappingGCBackend.resetMappings();
        mappingGCBackend.beginTracking();

        setHighlightButton(privateData.currentHighlight);
    }

    function endMapping()
    {
        resetHighlight();
        mappingGCBackend.endTracking();
    }

    function getGCMapping()
    {
        return mappingGCBackend.generateSDLBindings();
    }

    /*function calcListPosition()
    {
        atListStart = privateData.currentHighlight === 0;
        atListEnd = privateData.currentHighlight === highlightInfo.count-1
    }
    */

    Component.onCompleted:
    {
        //console.log("COMPONENT COMPLETED");
        highlightInfo.populateModel();
        //console.log(highlightInfo.get(privateData.currentHighlight).source);

        highlightImg.establishLateBindings();
    }

    Connections
    {
        target: privateData
        function onCurrentHighlightChanged() {
            //calcListPosition();
        }
    }

    onMappingActiveChanged: {
        //calcListPosition();
        /*if (mappingActive)
        {
            var result = mappingGCBackend.beginTracking();
            if (!result)
            {
                mappingActive = false;
            }
        }
        */
    }

    onMappingFinished: {
        endMapping();
    }

    onWidthChanged: function()
    {
        if (mappingActive)
        {
            setHighlightButton(privateData.currentHighlight);
        }
    }

    onHeightChanged: function()
    {
        if (mappingActive)
        {
            setHighlightButton(privateData.currentHighlight);
        }
    }
}
