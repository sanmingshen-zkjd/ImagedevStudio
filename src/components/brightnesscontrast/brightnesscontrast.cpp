#include "brightnesscontrast.h"
#include "brightnesscontrast_p.h"
#include <QVariant>
#include <QtCore/QQueue>
#include <iostream>

BrightnessContrast::BrightnessContrast() : d_ptr(new BrightnessContrastPrivate(this))
{
}

BrightnessContrast::~BrightnessContrast() {}

bool BrightnessContrast::init()
{
    Q_D(BrightnessContrast);
    d->_initialized = true;

    return d->_initialized;
}

void BrightnessContrast::sendFrame(const QImage& frame)
{
    Q_D(BrightnessContrast);

    if (!d->_initialized) {
        return;
    }
    d->pushImage(frame);
}

ComponentInterface::ComponentProperties BrightnessContrast::getSupportedProperties() const
{
    return {};
}

void BrightnessContrast::framesReceived()
{
    Q_D(BrightnessContrast);

    if (!d->_initialized) {
        return;
    }
}

void BrightnessContrast::framesWritten(qint64 cnt)
{
    Q_D(BrightnessContrast);
}

void BrightnessContrast::errorOccurred(int error)
{
    Q_D(BrightnessContrast);
}

void BrightnessContrast::setConfig(const QJsonObject& json)
{
    //assert(d_ptr != nullptr);
    //d_ptr->restoreConfiguration(json);
}

QJsonObject BrightnessContrast::getConfig() const
{
    //QJsonObject config;

    //d_ptr->saveSettings(config);

    return {};
}

void BrightnessContrast::setConfig(const QWidget& qobject)
{
    //Q_D(CanDevice);

    //configHelpers::setQConfig(qobject, getSupportedProperties(), d->_props);
}

std::shared_ptr<QWidget> BrightnessContrast::getQConfig() const
{
    const Q_D(BrightnessContrast);

    return nullptr;// configHelpers::getQConfig(getSupportedProperties(), d->_props);
}

void BrightnessContrast::startSimulation()
{
    Q_D(BrightnessContrast);

    d->_simStarted = true;

    //if (!d->_initialized) {
    //    cds_info("CanDevice not initialized");
    //    return;
    //}

    //if (!d->_canDevice.connectDevice()) {
    //    cds_error("Failed to connect device. Trying to init the device again...");

    //    // Cannelloni plugin fails to reconnect after initial disconnection.
    //    // This workaround reinit the device before reconnection
    //    // TODO: Findout why cannelloni plugin fails.
    //    if (init()) {
    //        cds_info("Re-init successful");

    //        if (!d->_canDevice.connectDevice()) {
    //            cds_error("Failed to re-connect device");
    //        } else {
    //            cds_info("Re-connection successful");
    //        }
    //    }
    //}

  //  d->_sendQueue.clear();
}

void BrightnessContrast::stopSimulation()
{
    Q_D(BrightnessContrast);

    d->_simStarted = false;

    //if (!d->_initialized) {
    //    cds_info("CanDevice not initialized");
    //    return;
    //}

    //d->_canDevice.disconnectDevice();
}

QWidget* BrightnessContrast::mainWidget()
{
    Q_D(BrightnessContrast);
    // Component does not have main widget
    return d->_ui.mainWidget();
}

bool BrightnessContrast::mainWidgetDocked() const
{
    // Widget does not exist. Return always true
    return false;
}

void BrightnessContrast::configChanged()
{
    Q_D(BrightnessContrast);

    init();

    if (d->_simStarted) {
        startSimulation();
    }
}

void BrightnessContrast::simBcastRcv(const QJsonObject& msg, const QVariant& param)
{
    Q_UNUSED(msg);
    Q_UNUSED(param);
}
