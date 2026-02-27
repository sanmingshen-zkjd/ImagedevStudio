#include "sequencediagram.h"
#include "sequencediagram_p.h"
#include <QVariant>
#include <QtCore/QQueue>
#include <iostream>

SequenceDiagram::SequenceDiagram() : d_ptr(new SequenceDiagramPrivate(this))
{
}

SequenceDiagram::~SequenceDiagram() {}

bool SequenceDiagram::init()
{
    Q_D(SequenceDiagram);

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

void SequenceDiagram::sendFrame(const QImage& frame)
{
    Q_D(SequenceDiagram);

    if (!d->_initialized) {
        return;
    }

    // Success will be reported in framesWritten signal.
    // Sending may be buffered. Keep correlation between sending results and frame/context
  //  d->_sendQueue.push_back(frame);
}

ComponentInterface::ComponentProperties SequenceDiagram::getSupportedProperties() const
{
    return {};
}

void SequenceDiagram::framesReceived()
{
    Q_D(SequenceDiagram);

    if (!d->_initialized) {
        return;
    }

    //while (static_cast<bool>(d->_canDevice.framesAvailable())) {
    //    const QCanBusFrame frame = d->_canDevice.readFrame();
    //    emit frameReceived(frame);
    //}
}

void SequenceDiagram::framesWritten(qint64 cnt)
{
    Q_D(SequenceDiagram);

    //while (cnt--) {
    //    if (!d->_sendQueue.isEmpty()) {
    //        auto sendItem = d->_sendQueue.takeFirst();
    //        emit frameSent(true, sendItem);
    //    } else {
    //        //cds_warn("Send queue is empty!");
    //    }
    //}
}

void SequenceDiagram::errorOccurred(int error)
{
    Q_D(SequenceDiagram);

    //cds_warn("Error occurred. Send queue size {}", d->_sendQueue.count());

    //if (error == QCanBusDevice::WriteError && !d->_sendQueue.isEmpty()) {
    //    auto sendItem = d->_sendQueue.takeFirst();
    //    emit frameSent(false, sendItem);
    //}
}

void SequenceDiagram::setConfig(const QJsonObject& json)
{
    //assert(d_ptr != nullptr);
    //d_ptr->restoreConfiguration(json);
}

QJsonObject SequenceDiagram::getConfig() const
{
    //QJsonObject config;

    //d_ptr->saveSettings(config);

    return {};
}

void SequenceDiagram::setConfig(const QWidget& qobject)
{
    //Q_D(CanDevice);

    //configHelpers::setQConfig(qobject, getSupportedProperties(), d->_props);
}

std::shared_ptr<QWidget> SequenceDiagram::getQConfig() const
{
    const Q_D(SequenceDiagram);

    return nullptr;// configHelpers::getQConfig(getSupportedProperties(), d->_props);
}

void SequenceDiagram::startSimulation()
{
    Q_D(SequenceDiagram);

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

void SequenceDiagram::stopSimulation()
{
    Q_D(SequenceDiagram);

    d->_simStarted = false;

    //if (!d->_initialized) {
    //    cds_info("CanDevice not initialized");
    //    return;
    //}

    //d->_canDevice.disconnectDevice();
}

QWidget* SequenceDiagram::mainWidget()
{
    Q_D(SequenceDiagram);
    // Component does not have main widget
    return d->_ui.mainWidget();
}

bool SequenceDiagram::mainWidgetDocked() const
{
    // Widget does not exist. Return always true
    return false;
}

void SequenceDiagram::configChanged()
{
    Q_D(SequenceDiagram);

    init();

    if (d->_simStarted) {
        startSimulation();
    }
}

void SequenceDiagram::simBcastRcv(const QJsonObject& msg, const QVariant& param)
{
    Q_UNUSED(msg);
    Q_UNUSED(param);
}
