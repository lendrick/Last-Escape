#QT       += core gui script opengl xml declarative
QT       += core script
TEMPLATE = app

OTHER_FILES += \
    ../scripts/startup.js \
    ../scripts/enums.inc.js \
    ../animations/xeon.xml \
    ../animations/flyer.xml \
    ../animations/walker.xml \
    ../animations/centipede.xml \
    ../animations/centipede.xml \
    ../animations/crawler.xml \
    ../animations/crawler.xml \
    ../animations/spider.xml \
    ../animations/default_bullet.xml \
    ../scripts/testActor.js \
    ../scripts/flyer.js \
    ../scripts/baseEnemy.js \
    ../scripts/utils.inc.js \
    ../scripts/sounds.inc.js \
    ../scripts/walker.js \
    ../scripts/baseEnemyPatroller.js \
    ../scripts/crawler.js \
    ../scripts/baseCollectible.js \
    ../scripts/energyBall.js \
    ../animations/energyBall.xml \
    ../scripts/playerBullet.js \
    ../animations/playerBullet.xml \
    ../scripts/centipedeSpit.js \
    ../animations/centipedeSpit.xml \
    ../scripts/centipede.js

HEADERS += \
    src/Utils.h \
    src/Ui.h \
    src/Teleport.h \
    src/StartPoint.h \
    src/SpawnPoint.h \
    src/SoundCache.h \
    src/Sound.h \
    src/Sensor.h \
    src/PlayerBullet.h \
    src/Player.h \
    src/PhysicsCrate.h \
    src/Particles.h \
    src/Map.h \
    src/InventoryItem.h \
    src/Input.h \
    src/ImageCache.h \
    src/globals.h \
    src/ExitPoint.h \
    src/Entity.h \
    src/EnemyWalker.h \
    src/EnemyShooter.h \
    src/EnemyPatroller.h \
    src/EnemyFlyer.h \
    src/EnemyCrawler.h \
    src/EnemyCentipede.h \
    src/Enemy.h \
    src/Component.h \
    src/Collectible.h \
    src/Bumper.h \
    src/BossSpider.h \
    src/Animation.h \
    src/AnimatedActor.h \
    src/Actor.h \
    src/tinyxml/tinyxml.h \
    src/tinyxml/tinystr.h \
    src/ScriptedActor.h \
    src/PhysicsBodyScriptWrapper.h \
    src/PhysicsShapeScriptWrapper.h \
    src/PhysicsVectorScriptWrapper.h \
    src/PhysicsConstVectorScriptWrapper.h

SOURCES += \
    src/Utils.cpp \
    src/Ui.cpp \
    src/Teleport.cpp \
    src/StartPoint.cpp \
    src/SpawnPoint.cpp \
    src/SoundCache.cpp \
    src/Sound.cpp \
    src/Sensor.cpp \
    src/PlayerBullet.cpp \
    src/Player.cpp \
    src/PhysicsCrate.cpp \
    src/Particles.cpp \
    src/Map.cpp \
    src/main.cpp \
    src/InventoryItem.cpp \
    src/Input.cpp \
    src/ImageCache.cpp \
    src/ExitPoint.cpp \
    src/Entity.cpp \
    src/EnemyWalker.cpp \
    src/EnemyShooter.cpp \
    src/EnemyPatroller.cpp \
    src/EnemyFlyer.cpp \
    src/EnemyCrawler.cpp \
    src/EnemyCentipede.cpp \
    src/Enemy.cpp \
    src/Component.cpp \
    src/Collectible.cpp \
    src/Bumper.cpp \
    src/BossSpider.cpp \
    src/Animation.cpp \
    src/AnimatedActor.cpp \
    src/Actor.cpp \
    src/tinyxml/tinyxmlparser.cpp \
    src/tinyxml/tinyxmlerror.cpp \
    src/tinyxml/tinyxml.cpp \
    src/tinyxml/tinystr.cpp \
    src/ScriptedActor.cpp \
    src/PhysicsBodyScriptWrapper.cpp \
    src/PhysicsShapeScriptWrapper.cpp \
    src/PhysicsVectorScriptWrapper.cpp \
    src/PhysicsConstVectorScriptWrapper.cpp

INCLUDEPATH = ../chipmunk/include
LIBS += -lsfml-audio -lsfml-system -lsfml-graphics -lsfml-network -lsfml-window -L../chipmunk-build-desktop -lchipmunk
