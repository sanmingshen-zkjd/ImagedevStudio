#pragma once

#include "brightnesscontrast.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QtCore/QVector>
#include "gui/brightnesscontrastguiimpl.h"
//#include <propertyfields.h>

class BrightnessContrast;

class BrightnessContrastPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(BrightnessContrast)

public:
    BrightnessContrastPrivate(
        BrightnessContrast* q, BrightnessContrastCtx&& ctx = BrightnessContrastCtx(new BrightnessContrastGuiImpl))
        : _ctx(std::move(ctx))
        ,_ui(_ctx.get<BrightnessContrastGuiInt>()) 
        ,q_ptr(q)
    {

    }

    void saveSettings(QJsonObject& json)
    {
    }

    bool restoreConfiguration(const QJsonObject& json)
    {
        return true;
    }

    void pushImage(QImage image) {
        _sendQueue.push_back(image);
        _ui.setCurImage(image);
    }

    using devConfigPair = std::pair<int, QVariant>;
    std::vector<devConfigPair> getDevConfig()
    {
        std::vector<devConfigPair> ret;
        return ret;
    }

signals:
    void backendChanged(const QString& backend);

public:
  //  CanDeviceCtx _ctx;
  //  QVector<QImage> _sendQueue;
    //CanDeviceInterface& _canDevice;
    bool _initialized{ true };
    bool _simStarted{ false };

    BrightnessContrastCtx _ctx;
    BrightnessContrastGuiInt& _ui;

private:
    BrightnessContrast* q_ptr;
   QMetaObject::Connection _prevConn;
    QVector<QImage> _sendQueue;
};