#include "contourdetectmodel.h"
#include <assert.h>
#include <datamodeltypes/canrawdata.h>
#include <log.h>
#include "contourdetectplugin.h"

namespace {

const std::map<PortType, std::vector<NodeDataType>> portMappings = { 
    { PortType::In,
        { 
            { CanRawData{}.type() } 
        }
    },
    { PortType::Out, 
      { 
          { CanRawData{}.type() } 
      } 
     } 
};
} // namespace

ContourDetectModel::ContourDetectModel()
    : ComponentModel("ContourDetect")
    , _status(false)
    , _direction(Direction::Uninitialized)
    , _painter(std::make_unique<NodePainter>(ImageContourDetectPlugin::PluginType::sectionColor()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(&_component, &ContourDetect::frameSent, this, &ContourDetectModel::frameSent);
    connect(&_component, &ContourDetect::frameReceived, this, &ContourDetectModel::frameReceived);
    connect(this, &ContourDetectModel::sendFrame, &_component, &ContourDetect::sendFrame);
}

QtNodes::NodePainterDelegate* ContourDetectModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int ContourDetectModel::nPorts(PortType portType) const
{
    //assert((PortType::In == portType) || (PortType::Out == portType) || (PortType::None == portType)); // range check

    //return (PortType::None != portType) ? 1 : 0;
    return portMappings.at(portType).size();
}

void ContourDetectModel::frameReceived(const QImage& frame)
{
    bool ret = _rxQueue.try_enqueue(std::make_shared<CanRawData>(frame, Direction::RX));

    if(ret) {
        emit dataUpdated(0); // Data ready on port 0
    } else {
        cds_warn("Queue full. Frame dropped");
    } 
}

void ContourDetectModel::frameSent(bool status, const QImage& frame)
{
    bool ret = _rxQueue.try_enqueue(std::make_shared<CanRawData>(frame, Direction::TX, status));

    if(ret) {
        emit dataUpdated(0); // Data ready on port 0
    } else {
        cds_warn("Queue full. Frame dropped");
    } 
}

NodeDataType ContourDetectModel::dataType(PortType portType, PortIndex) const
{
    assert((PortType::In == portType) || (PortType::Out == portType)); // allowed input

    return (PortType::Out == portType) ? CanRawData{}.type() : CanRawData{}.type();
}

std::shared_ptr<NodeData> ContourDetectModel::outData(PortIndex)
{
    std::shared_ptr<NodeData> ret;
    bool status = _rxQueue.try_dequeue(ret);

    if (!status) {
        cds_error("No data available on rx queue");
        return {};
    }

    return ret;
}

void ContourDetectModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex)
{
    if (nodeData) {
        auto d = std::dynamic_pointer_cast<CanRawData>(nodeData);
        assert(nullptr != d);
        _label->setPixmap(QPixmap::fromImage(d->frame()));
        emit sendFrame(d->frame());
    } else {
        cds_warn("Incorrect nodeData");
    }
}
