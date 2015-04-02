TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR += bin/
OBJECTS_DIR += obj/

QMAKE_CXXFLAGS +=  -std=c++11
INCLUDEPATH += src/
INCLUDEPATH += src/AControllers/
INCLUDEPATH += src/CombatModels/
INCLUDEPATH += src/Character/
INCLUDEPATH += src/Common/
INCLUDEPATH += src/Actions/
INCLUDEPATH += src/SFML/
INCLUDEPATH += src/LogicObjects/

LIBS += -lGLEW  -lBox2D -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lboost_system \
 -lboost_thread \
 -lpthread -ltgui -lGL -lX11 -lconfig++ -ldl -lz


SOURCES += \
    src/App.cpp \
    src/SFML/ATextureContainer.cpp \
    src/SFML/ASpriteData.cpp \
    src/SFML/ASpriteContainer.cpp \
    src/SFML/ASprite.cpp \
    src/APhysicObjectData.cpp \
    src/AGroundCell.cpp \
    src/ALocationBase.cpp \
    src/SFML/ALocationSFML.cpp \
    src/APhysicObjectBase.cpp \
    src/SFML/APhysicObjectSFML.cpp \
    src/SFML/AWorldSFML.cpp \
    src/AWorldBase.cpp \
    src/AControllerBase.cpp \
    src/AVector2.cpp \
    src/AControllers/AKeyboardControllerSFML.cpp \
    src/SFML/ACharacterSFML.cpp \
    src/AContactListener.cpp \
    src/ACombatModelBase.cpp \
    src/CombatModels/AHumanCombatModel.cpp \
    src/Actions/CombatActions.cpp \
    src/Character/Race.cpp \
    src/MainMenu.cpp \
    src/SinglePlayerMode.cpp \
    src/EditorMode.cpp \
    src/Mode.cpp \
    src/Action.cpp \
    src/Common/Common.cpp \
    src/MultiplayerMode.cpp \
    src/ADebugOutput.cpp \
    src/GUI/GInventory.cpp \
    src/GUI/GContainerSystem.cpp \
    src/GUI/GContainer.cpp \
    src/GUI/GEditor.cpp \
    src/ATranslationReader.cpp \
    src/Common/Global.cpp \
    src/GUI/GCommon.cpp \
    src/LogicObjects/ACharacterData.cpp \
    src/GUI/GInGame.cpp \
    src/Character/Effects.cpp \
    src/GUI/GPicture.cpp \
    src/GUI/GObjectTemplateEditor.cpp \
    src/SFML/AAnimation.cpp \
    src/SFML/AAnimationScheme.cpp \
    src/AAlchemySystem.cpp \
    src/AScriptWrapper.cpp \
    src/AObjectFilter.cpp \
    src/AScriptArea.cpp \
    src/GUI/GScriptEditor.cpp \
    src/AControllers/ASimpleAIController.cpp \
    src/AGarbageCollector.cpp \
    src/SFML/AObjectDrawerSFML.cpp \
    src/LogicObjects/AObjectArmor.cpp \
    src/LogicObjects/AObjectPotion.cpp \
    src/LogicObjects/AObjectWeapon.cpp \
    src/LogicObjects/ANonCharacterLogicObject.cpp \
    src/LogicObjects/ACharacter.cpp \
    src/AObjectDrawer.cpp \
    src/SFML/ACharacterDrawerSFML.cpp

OTHER_FILES += \
    resources/characters.xml \
    resources/mainSettings.cfg \
    resources/locations/testLocation.cfg \
    resources/locations/testLocSave.cfg \
    resources/objects.cfg \
    resources/groundCells.cfg \
    resources/logicMaterials.cfg \
    resources/sprites.cfg \
    resources/races.cfg \
    resources/characters.cfg \
    resources/SPWorldSettings.cfg


HEADERS += \
    src/SFML/ATextureContainer.h \
    src/SFML/ASpriteContainer.h \
    src/SFML/ASprite.h \
    src/AData.h \
    src/SFML/ASpriteData.h \
    src/APhysicObjectData.h \
    src/AObject.h \
    src/AGroundCell.h \
    src/SFML/ALocationSFML.h \
    src/ALocationBase.h \
    src/APhysicObjectBase.h \
    src/SFML/APhysicObjectSFML.h \
    src/SFML/AWorldSFML.h \
    src/AWorldBase.h \
    src/AIDManager.h \
    src/AControllerBase.h \
    src/AVector2.h \
    src/SFML/AControllerSFML.h \
    src/icon.h \
    src/AControllers/AKeyboardControllerSFML.h \
    src/SFML/ACharacterSFML.h \
    src/AContactListener.h \
    src/ACombatModelBase.h \
    src/CombatModels/AHumanCombatModel.h \
    src/Action.h \
    src/Actions/CombatActions.h \
    src/Character/Race.h \
    src/Common/AdvRandom.h \
    src/Actions/MovementActions.h \
    src/Mode.h \
    src/const.h \
    src/Common/Common.h \
    src/ADebugOutput.h \
    src/GUI/GInventory.h \
    src/GUI/GContainerSystem.h \
    src/GUI/GContainer.h \
    src/GUI/GEditor.h \
    src/Common/Global.h \
    src/EditorMode.h \
    src/ATranslationReader.h \
    src/GUI/GCommon.h \
    src/AObjectTemplatesContainer.h \
    src/LogicObjects/ACharacterData.hpp \
    src/GUI/GInGame.hpp \
    src/Character/Effects.hpp \
    src/SinglePlayerMode.hpp \
    src/GUI/GPicture.hpp \
    src/GUI/GObjectTemplateEditor.hpp \
    src/SFML/AAnimation.hpp \
    src/SFML/AAnimationScheme.hpp \
    src/AAlchemySystem.hpp \
    src/AScriptWrapper.hpp \
    src/AScriptArea.hpp \
    src/AObjectFilter.hpp \
    src/GUI/GScriptEditor.hpp \
    src/AControllers/ASimpleAIController.hpp \
    src/AGarbageCollector.hpp \
    src/LogicObjects/ANonCharacterLogicObject.hpp \
    src/AObjectDrawer.hpp \
    src/SFML/AObjectDrawerSFML.hpp \
    src/LogicObjects/AObjectPotion.hpp \
    src/LogicObjects/ACharacter.h \
    src/LogicObjects/AObjectWeapon.hpp \
    src/LogicObjects/AObjectArmor.hpp \
    src/SFML/ACharacterDrawerSFML.hpp

