#include "sequencediagrammodel.h"
#include <assert.h>
#include <datamodeltypes/canrawdata.h>
#include <log.h>
#include "sequencediagramplugin.h"

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

SequenceDiagramModel::SequenceDiagramModel(): ComponentModel("SequenceDiagram")
    , _status(false)
    , _direction(Direction::Uninitialized)
    , _painter(std::make_unique<NodePainter>(ImageSequenceDiagramPlugin::PluginType::sectionColor()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(&_component, &SequenceDiagram::frameSent, this, &SequenceDiagramModel::frameSent);
    connect(&_component, &SequenceDiagram::frameReceived, this, &SequenceDiagramModel::frameReceived);
    connect(this, &SequenceDiagramModel::sendFrame, &_component, &SequenceDiagram::sendFrame);
}

QtNodes::NodePainterDelegate* SequenceDiagramModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int SequenceDiagramModel::nPorts(PortType portType) const
{
    //assert((PortType::In == portType) || (PortType::Out == portType) || (PortType::None == portType)); // range check

    //return (PortType::None != portType) ? 1 : 0;
    return portMappings.at(portType).size();
}

void SequenceDiagramModel::frameReceived(const QImage& frame)
{
    bool ret = _rxQueue.try_enqueue(std::make_shared<CanRawData>(frame, Direction::RX));

    if(ret) {
        emit dataUpdated(0); // Data ready on port 0
    } else {
        cds_warn("Queue full. Frame dropped");
    } 
}

void SequenceDiagramModel::frameSent(bool status, const QImage& frame)
{
    bool ret = _rxQueue.try_enqueue(std::make_shared<CanRawData>(frame, Direction::TX, status));

    if(ret) {
        emit dataUpdated(0); // Data ready on port 0
    } else {
        cds_warn("Queue full. Frame dropped");
    } 
}

NodeDataType SequenceDiagramModel::dataType(PortType portType, PortIndex) const
{
    assert((PortType::In == portType) || (PortType::Out == portType)); // allowed input

    return (PortType::Out == portType) ? CanRawData{}.type() : CanRawData{}.type();
}

std::shared_ptr<NodeData> SequenceDiagramModel::outData(PortIndex)
{
    std::shared_ptr<NodeData> ret;
    bool status = _rxQueue.try_dequeue(ret);

    if (!status) {
        cds_error("No data available on rx queue");
        return {};
    }

    return ret;
}

void SequenceDiagramModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex)
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
