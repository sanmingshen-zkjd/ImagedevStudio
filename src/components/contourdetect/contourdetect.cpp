#include "contourdetect.h"
#include "contourdetect_p.h"
#include <QVariant>
#include <QtCore/QQueue>
#include <iostream>

ContourDetect::ContourDetect() : d_ptr(new ContourDetectPrivate(this))
{
}

ContourDetect::~ContourDetect() {}

bool ContourDetect::init()
{
    Q_D(ContourDetect);

    //const auto& props = d->_props;
    //const auto& backend = d->_backendProperty;
    //const auto& iface = d->_interfaceProperty;

    // check if required properties are set. They always exists as are initialized in constructor
    //if ((props.at(backend).toString().length() == 0) || (props.at(iface).toString().length() == 0)) {
    //    return d->_initialized;
    //}

    //QString errorString;

    d->_initialized = true;

    //QString b = props.at(backend).toString();
    //QString i = props.at(iface).toString();
    return d->_initialized;
}

void ContourDetect::sendFrame(const QImage& frame)
{
    Q_D(ContourDetect);

    if (!d->_initialized) {
        return;
    }
    d->pushImage(frame);
    // Success will be reported in framesWritten signal.
    // Sending may be buffered. Keep correlation between sending results and frame/context
  //  d->_sendQueue.push_back(frame);
}

ComponentInterface::ComponentProperties ContourDetect::getSupportedProperties() const
{
    return {};
}

void ContourDetect::framesReceived()
{
    Q_D(ContourDetect);

    if (!d->_initialized) {
        return;
    }

    //while (static_cast<bool>(d->_canDevice.framesAvailable())) {
    //    const QCanBusFrame frame = d->_canDevice.readFrame();
    //    emit frameReceived(frame);
    //}
}

void ContourDetect::framesWritten(qint64 cnt)
{
    Q_D(ContourDetect);

    //while (cnt--) {
    //    if (!d->_sendQueue.isEmpty()) {
    //        auto sendItem = d->_sendQueue.takeFirst();
    //        emit frameSent(true, sendItem);
    //    } else {
    //        //cds_warn("Send queue is empty!");
    //    }
    //}
}

void ContourDetect::errorOccurred(int error)
{
    Q_D(ContourDetect);

    //cds_warn("Error occurred. Send queue size {}", d->_sendQueue.count());

    //if (error == QCanBusDevice::WriteError && !d->_sendQueue.isEmpty()) {
    //    auto sendItem = d->_sendQueue.takeFirst();
    //    emit frameSent(false, sendItem);
    //}
}

void ContourDetect::setConfig(const QJsonObject& json)
{
    //assert(d_ptr != nullptr);
    //d_ptr->restoreConfiguration(json);
}

QJsonObject ContourDetect::getConfig() const
{
    //QJsonObject config;

    //d_ptr->saveSettings(config);

    return {};
}

void ContourDetect::setConfig(const QWidget& qobject)
{
    //Q_D(CanDevice);

    //configHelpers::setQConfig(qobject, getSupportedProperties(), d->_props);
}

std::shared_ptr<QWidget> ContourDetect::getQConfig() const
{
    const Q_D(ContourDetect);

    return nullptr;// configHelpers::getQConfig(getSupportedProperties(), d->_props);
}

void ContourDetect::startSimulation()
{
    Q_D(ContourDetect);

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

void ContourDetect::stopSimulation()
{
    Q_D(ContourDetect);

    d->_simStarted = false;

    //if (!d->_initialized) {
    //    cds_info("CanDevice not initialized");
    //    return;
    //}

    //d->_canDevice.disconnectDevice();
}

QWidget* ContourDetect::mainWidget()
{
    Q_D(ContourDetect);
    // Component does not have main widget
    return d->_ui.mainWidget();
}

bool ContourDetect::mainWidgetDocked() const
{
    // Widget does not exist. Return always true
    return false;
}

void ContourDetect::configChanged()
{
    Q_D(ContourDetect);

    init();

    if (d->_simStarted) {
        startSimulation();
    }
}

void ContourDetect::simBcastRcv(const QJsonObject& msg, const QVariant& param)
{
    Q_UNUSED(msg);
    Q_UNUSED(param);
}
