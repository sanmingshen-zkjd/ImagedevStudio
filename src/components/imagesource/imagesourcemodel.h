#pragma once

#include "imagesource.h"
#include "componentmodel.h"
#include "nodepainter.h"
#include <QtCore/QObject>
#include <QImage>
#include <readerwriterqueue.h>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

enum class Direction;

class ImageSourceModel : public ComponentModel<ImageSource, ImageSourceModel> {
    Q_OBJECT

public:
    ImageSourceModel();

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData> nodeData, PortIndex port) override;
    QtNodes::NodePainterDelegate* painterDelegate() const override;

public slots:
    void filteredTx(const QImage& frame);
    void filteredRx(const QImage& frame);

signals:
    void filterTx(const QImage& frame);
    void filterRx(const QImage& frame);
    void requestRedraw();

private:
    std::unique_ptr<NodePainter> _painter;
    // 127 to use 4 blocks, 512 bytes each
    std::vector<std::shared_ptr<NodeData>> _images;
 //   moodycamel::ReaderWriterQueue<std::shared_ptr<NodeData>> _fwdQueue{ 127 };
};
