import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import sdl2mappercomponents 1.0

Window {
    id: rootWindow

    visible: true
    width: 540
    height: 600

    minimumWidth: 540
    minimumHeight: 600
    maximumWidth: 540
    maximumHeight: 600

    //flags: Qt.Dialog

    title: qsTr("SDL2 Gamepad Mapper")

    QtObject
    {
        id: msgDialogStrings

        property string copyGUIDText: qsTr("Copying GUID of \"%1\" to clipboard");
        property string copyGUIDTitle: qsTr("Copying GUID");
        property string copyMappingText: qsTr("Copying SDL Mapping string of \"%1\" to clipboard");
        property string copyMappingTitle: qsTr("Copying SDL Mapping string");
    }

    Component
    {
        id: msgDialogComp

        MessageDialog
        {
            id: msgDialog
            text: msgDialogStrings.copyGUIDText
            title: msgDialogStrings.copyGUIDTitle
            standardButtons: StandardButton.Close
            icon: StandardIcon.Information
        }
    }

    Component
    {
        id: mappingConfirmDialogComp

        Dialog
        {
            id: saveMapDialog

            width: 600
            height: 180

            title: qsTr("Save Mapping?")
            contentItem: Item
            {
                ColumnLayout
                {
                    anchors.fill: parent
                    anchors.margins:
                    {
                        leftMargin: 4
                        rightMargin: 4
                        topMargin: 8
                        bottomMargin: 8
                    }

                    Text
                    {
                        Layout.fillHeight: false
                        Layout.preferredHeight: 30
                        Layout.alignment: Qt.AlignCenter
                        text: qsTr("Save Mapping with provided name?")
                    }

                    ColumnLayout
                    {
                        Layout.fillHeight: false

                        Label
                        {
                            text: qsTr("Mapping Name:")
                        }

                        TextField
                        {
                            id: mappingNameTxt
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            selectByMouse: true
                            text: {
                                var result = "";
                                var index = deviceCombo.currentIndex;
                                var joypad = backend.reader.joypadContainer.getJoystick(index);
                                if (joypad !== undefined)
                                {
                                    result = joypad.gameContDevName;
                                }

                                return result;
                            }

                            maximumLength: 200
                        }
                    }

                    DialogButtonBox
                    {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignRight
                        spacing: 20
                        standardButtons: StandardButton.Yes | StandardButton.Cancel
                        onAccepted: {
                            var index = deviceCombo.currentIndex;
                            var joypad = backend.reader.joypadContainer.getJoystick(index);
                            if (joypad !== undefined)
                            {
                                joypad.gameMappedName = mappingNameTxt.text
                                var tempString = mappingDisplayItem.getGCMapping();
                                var result = joypad.setMappingString(tempString);
                                joypad.refreshGameController();
                                var logMessage = qsTr("Mapping string: %1").arg(tempString);
                                viewBackend.writeMappingString(tempString);
                                logger.log(logMessage);
                                viewBackend.joyComboModel.refreshItem(index);
                            }

                            saveMapDialog.close();
                            displayMappedItem.enableDisplay();
                        }

                        onRejected: {
                            saveMapDialog.close();
                            displayMappedItem.enableDisplay();
                        }
                    }

                }
            }

        }
    }

    MessageDialog
    {
        id: createMappingDialog
        text: "Create Game Controller Mapping?"
        title: "Create Game Controller Mapping?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            actionButtonLayout.currentIndex = mappingActionRow.elementIndex;
            displayMappedItem.disableDisplay();
            //highlightImgLayout.currentIndex = mappingDisplayItem.elementIndex;
            mappingDisplayItem.resetHighlight();
            mappingDisplayItem.startMapping();
        }
    }

    SDL2MapperBackend
    {
        id: backend
    }

    AppLogger
    {
        id: logger

        onLogMessage: {
            //console.log("LOGGER MESSAGE: " + message);
            //logTxtArea.text = logTxtArea.text.concat("\n" + message);
            logTxtArea.append(message);
        }
    }

    Item
    {
        id: privateItem

        MainViewBackend
        {
            id: viewBackend
            mapperBack: backend
        }
    }

    ColumnLayout
    {
        id: mainColLayout

        spacing: 20
        anchors.fill: parent
        anchors.margins:
        {
            leftMargin: 8;
            rightMargin: 8;
            topMargin: 14;
            bottomMargin: 14;
        }

        ComboBox
        {
            id: deviceCombo
            Layout.fillWidth: true;
            textRole: "details"
            currentIndex: 0
            model: ListModel
            {
                ListElement { details: "No devices" }
            }

            function changeActiveDevice() {
                displayMappedItem.disableDisplay();

                var index = currentIndex;
                if (index >= 0)
                {
                    var joypad = backend.reader.joypadContainer.getJoystick(index);
                    if (joypad !== undefined)
                    {
                        mappingDisplayItem.setJoyDevice(joypad);
                        displayMappedItem.setJoyDevice(joypad);
                        if (joypad.isGameController)
                        {
                            displayMappedItem.enableDisplay();
                        }
                    }
                }
            }
        }

        StackLayout
        {
            id: highlightImgLayout
            Layout.fillHeight: false
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignHCenter

            currentIndex: mappingDisplayItem.mappingActive ? mappingDisplayItem.elementIndex : displayMappedItem.elementIndex

            DisplayMappedJoystickItem
            {
                id: displayMappedItem

                property int elementIndex: 0
                logger: logger

                Layout.alignment: Qt.AlignHCenter
                //color: "#FF00FF"
            }

            MappingGCItem
            {
                id: mappingDisplayItem
                property int elementIndex: 1
                logger: logger

                //Layout.fillWidth: true;
                //Layout.fillHeight: false;
                //Layout.maximumHeight: 50;
                //height: 200
                //width: 300
                //Layout.preferredWidth: 50
                //Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignHCenter
                //color: "#FF00FF"
                //scale: 0.5
                //transformOrigin: Item.TopLeft
                /*transform: Scale {
                    xScale: 0.5; yScale: 0.5;
                }
                */

                //Layout.maximumHeight: 500;
                onMappingFinished: {
                    actionButtonLayout.currentIndex = defaultActionRow.elementIndex;
                    mappingDisplayItem.endMapping();
                    var msgDialog = mappingConfirmDialogComp.createObject(rootWindow);
                    msgDialog.open();
                }
            }
        }



        /*AnimatedImage
        {
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            Layout.preferredHeight: 100;
            Layout.maximumHeight: 500;
            source: "file:///C:/Users/ryoch/Pictures/1541109515196-1.gif"
            fillMode: Image.PreserveAspectFit
        }
        */

        StackLayout
        {
            id: actionButtonLayout
            Layout.fillHeight: false
            //Layout.preferredHeight: 60

            RowLayout
            {
                id: defaultActionRow
                spacing: 20
                property int elementIndex: 0
                enabled: deviceCombo.currentIndex >= 0

                Button
                {
                    Layout.fillWidth: true
                    text: qsTr("Copy Gamepad GUID")
                    onClicked: {
                        var index = deviceCombo.currentIndex;
                        var joypad = backend.reader.joypadContainer.getJoystick(index);
                        var guid = backend.reader.joypadContainer.getJoystick(index).GUID;
                        var msgDialog = msgDialogComp.createObject(rootWindow);
                        msgDialog.text = msgDialogStrings.copyGUIDText.arg(joypad.gameContDevName);
                        msgDialog.open();
                        viewBackend.copyGUIDToClipboard(guid);
                        //logger.log(msgDialogStrings.copyGUIDText.arg(joypad.gameContDevName));
                        var logString = qsTr("The GUID for \"%1\" is \"%2\"").arg(joypad.gameMappedName).arg(joypad.GUID);
                        logger.log(logString);
                        //logger.log(viewBackend.generateGUIDStringDetails(joypad));
                    }
                }

                Button
                {
                    Layout.fillWidth: true
                    text: qsTr("Create A New Mapping")
                    onClicked: {
                        createMappingDialog.open();
                    }
                }

                Button
                {
                    Layout.fillWidth: true
                    text: qsTr("Copy Mapping String")
                    onClicked: {
                        var index = deviceCombo.currentIndex;
                        var joypad = backend.reader.joypadContainer.getJoystick(index);
                        var temp = index >= 0 ?
                                    backend.reader.joypadContainer.getJoystick(index).gameContMapping : ""

                        if (temp !== "")
                        {
                            var msgDialog = msgDialogComp.createObject(rootWindow);
                            msgDialog.text = msgDialogStrings.copyMappingText.arg(joypad.gameContDevName);
                            msgDialog.title = msgDialogStrings.copyMappingTitle;
                            msgDialog.open();
                            viewBackend.copyMappingString(temp);
                            var logString = qsTr("Mapping string for \"%1\" is \"%2\"").arg(joypad.gameContDevName).arg(temp);
                            logger.log(logString);
                            //logger.log(msgDialogStrings.copyMappingText.arg(joypad.gameContDevName));
                        }
                    }
                }
            }

            RowLayout
            {
                id: mappingActionRow
                spacing: 20
                property int elementIndex: 1

                Button
                {
                    Layout.fillWidth: true
                    text: qsTr("Skip")
                    //enabled: !mappingDisplayItem.atListEnd
                    onClicked: {
                        mappingDisplayItem.activeNextHighlightButton();
                    }
                }

                Button
                {
                    Layout.fillWidth: true
                    text: qsTr("Previous")
                    enabled: !mappingDisplayItem.atListStart
                    onClicked: {
                        mappingDisplayItem.activatePrevHighlightButton();
                    }
                }

                Button
                {
                    Layout.fillWidth: true
                    text: qsTr("Cancel")
                    onClicked: {
                        actionButtonLayout.currentIndex = defaultActionRow.elementIndex;
                        //highlightImgLayout.currentIndex = displayMappedItem.elementIndex;
                        mappingDisplayItem.endMapping();
                        displayMappedItem.enableDisplay();
                    }
                }
            }
        }

        RowLayout
        {
            id: envVarActionRow
            Layout.fillHeight: false
            //Layout.preferredHeight: 60
            spacing: 20
            enabled: Qt.platform.os === "windows" && actionButtonLayout.currentIndex == defaultActionRow.elementIndex
            //Layout.topMargin: 10
            //visible: Qt.platform.os === "windows"

            Button
            {
                Layout.fillWidth: true
                text: qsTr("Set Mapping As Environmental Variable")
                onClicked: {
                    var index = deviceCombo.currentIndex;
                    var joypad = backend.reader.joypadContainer.getJoystick(index);
                    if (joypad !== undefined && joypad.isGameController)
                    {
                        viewBackend.createMappingEnvvar(joypad.gameContMapping);
                        var logMessage = qsTr("Setting environmental variable \"SDL_GAMECONTROLLERCONFIG\" to \"%1\"")
                            .arg(joypad.gameContMapping);
                        logger.log(logMessage);
                    }
                }
            }

            Button
            {
                Layout.fillWidth: true
                text: qsTr("Reset Environmental Variable")
                enabled: viewBackend.hasSDLEnvVar;
                onClicked:
                {
                    if (viewBackend.hasSDLEnvVar)
                    {
                        logger.log(qsTr("Reseting environmental variable \"SDL_GAMECONTROLLERCONFIG\""));
                        viewBackend.removeMappingEnvvar();
                    }
                }
            }
        }

        ScrollView
        {
            Layout.fillWidth: true;
            Layout.fillHeight: false;
            Layout.maximumHeight: 100
            Layout.preferredHeight: 100
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            TextArea
            {
                id: logTxtArea
                text: "SDL2 Gamepad Mapper\n"
                selectByMouse: true
                readOnly: true
                wrapMode: TextEdit.WrapAnywhere

                MouseArea
                {
                    anchors.fill: logTxtArea
                    enabled: false
                    cursorShape: Qt.IBeamCursor
                }
            }
        }

        Text
        {
            text: qsTr("Found %0 gamepad(s)").arg(backend.reader.joypadContainer.numPads);
        }
    }

    function prepareSession()
    {
        logger.log(qsTr("Scan for controllers"));

        backend.start();
        viewBackend.createJoyComboModel();
        var numPads = backend.reader.joypadContainer.numPads;
        var logMessage = qsTr("Found %0 gamepad(s)").arg(numPads);
        logger.log(logMessage);

        if (numPads > 0)
        {
            deviceCombo.model = Qt.binding(function()
            {
                return viewBackend.joyComboModel;
            });

            deviceCombo.currentIndex = -1;
            deviceCombo.currentIndexChanged.connect(deviceCombo.changeActiveDevice);
            deviceCombo.currentIndex = 0;

            for (var i = 0; i < numPads; i++)
            {
                var joypad = backend.reader.joypadContainer.getJoystick(i);
                if (joypad !== undefined)
                {
                    logMessage = String("\"%1\", %2 %3").arg(joypad.gameMappedName)
                        .arg(joypad.GUID).arg(joypad.isGameController ? qsTr("(mapping available)") : "");
                    logger.log(logMessage);
                }
            }
        }

        deviceCombo.enabled = Qt.binding(function()
        {
            return !mappingDisplayItem.mappingActive && backend.reader.joypadContainer.numPads > 0;
        });

        if (viewBackend.hasSDLEnvVar)
        {
            logger.log(qsTr("Environmental variable \"SDL_GAMECONTROLLERCONFIG\" was found"));
        }
        else
        {
            logger.log(qsTr("Environmental variable \"SDL_GAMECONTROLLERCONFIG\" was not found"));
        }

        //mappingDisplayItem.mappingActive = true;
        //mappingDisplayItem.setHighlightButton(0);

        viewBackend.upstreamMappingCheckFinished.disconnect(prepareSession);
        mainColLayout.enabled = true;
    }

    Component.onCompleted:
    {
        logTxtArea.text = "";
        var logMessage = String("SDL2 Gamepad Mapper %1 (%2)").arg(viewBackend.progVersion)
            .arg(viewBackend.generateSDLVersionText());
        logger.log(logMessage);
        logger.log("");

        var fetchRemote = viewBackend.checkLatestMappingFile();
        if (fetchRemote)
        {
            logger.log("Checking for latest GameController mapping file");

            mainColLayout.enabled = false;
            viewBackend.upstreamMappingCheckFinished.connect(prepareSession);
            viewBackend.requestLatestMappingInfo();
            //viewBackend.requestLatestMappingFile();
        }
        else
        {
            prepareSession();
        }
    }

    Component.onDestruction:
    {
        backend.stop();
    }
}
