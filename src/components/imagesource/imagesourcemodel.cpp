#include "imagesourcemodel.h"
#include <datamodeltypes/canrawdata.h>
#include <log.h>
#include "imagesourceplugin.h"

namespace {

const std::map<PortType, std::vector<NodeDataType>> portMappings = {
    { PortType::In, 
        {

        }
    },
    { PortType::Out, 
        {
            {CanRawData{}.type() }
        }
    }
};
} // namespace

ImageSourceModel::ImageSourceModel() : ComponentModel("ImageSource")
    , _painter(std::make_unique<NodePainter>(ImageSourcePlugin::PluginType::sectionColor()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &ImageSourceModel::filterTx, &_component, &ImageSource::txFrameIn);
    connect(this, &ImageSourceModel::filterRx, &_component, &ImageSource::rxFrameIn);
    connect(&_component, &ImageSource::txFrameOut, this, &ImageSourceModel::filteredTx);
    connect(&_component, &ImageSource::rxFrameOut, this, &ImageSourceModel::filteredRx);
}

QtNodes::NodePainterDelegate* ImageSourceModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int ImageSourceModel::nPorts(PortType portType) const
{
    return portMappings.at(portType).size();
}

NodeDataType ImageSourceModel::dataType(PortType portType, PortIndex ndx) const
{
    if (portMappings.at(portType).size() > static_cast<uint32_t>(ndx)) {
        return portMappings.at(portType)[ndx];
    }

    cds_error("No port mapping for ndx: {}", ndx);
    return {};
}

std::shared_ptr<NodeData> ImageSourceModel::outData(PortIndex)
{
    std::shared_ptr<NodeData> ret;

    if (_images.empty())
        return {};

    ret = _images.at(0);

    return ret;

    //bool status = _fwdQueue.try_dequeue(ret);
    //if (!status) 
    //{
    //    cds_error("No data available on fwd queue");
    //    return {};
    //}
    //return ret;
}

void ImageSourceModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex)
{
    if (nodeData)
    {
        auto d = std::dynamic_pointer_cast<CanRawData>(nodeData);
        assert(nullptr != d);
        if (d->direction() == Direction::TX)
        {
            if (d->status())
            {
                emit filterTx(d->frame());
            }
        }
        else if (d->direction() == Direction::RX)
        {
            emit filterRx(d->frame());
        }
        else
        {
            cds_warn("Incorrect direction");
        }
    } 
    else 
    {
        cds_warn("Incorrect nodeData");
    }
}

void ImageSourceModel::filteredTx(const QImage& frame)
{
    _images.push_back(std::make_shared<CanRawData>(frame, Direction::RX));
 //   bool ret = _fwdQueue.try_enqueue(std::make_shared<CanRawData>(frame, Direction::TX));

    //if (ret)
    //{
    //    emit dataUpdated(0); // Data ready on port 0
    //} 
    //else
    //{
    //    cds_warn("Queue full. Frame dropped");
    //}

   // Q_EMIT dataUpdated(0);
}

void ImageSourceModel::filteredRx(const QImage& frame)
{
    _images.push_back(std::make_shared<CanRawData>(frame, Direction::RX));
 //   bool ret = _fwdQueue.try_enqueue(std::make_shared<CanRawData>(frame, Direction::RX));

    //if (ret)
    //{
    //    emit dataUpdated(0); // Data ready on port 0
    //}
    //else
    //{
    //    cds_warn("Queue full. Frame dropped");
    //}

}
