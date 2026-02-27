#include "imagesource.h"
#include "imagesource_p.h"
#include <confighelpers.h>
#include <log.h>

ImageSource::ImageSource() : d_ptr(new ImageSourcePrivate(this))
{
   // d_ptr
}

ImageSource::~ImageSource() {}

QWidget* ImageSource::mainWidget()
{
    Q_D(ImageSource);
    connect(d, &ImageSourcePrivate::broadcastImageList, this, &ImageSource::slotReadFrame);
    return d->_ui.mainWidget();
}

void ImageSource::setConfig(const QJsonObject& json)
{
    Q_D(ImageSource);

   // d->setSettings(json);
}

void ImageSource::setConfig(const QWidget& qobject)
{
    Q_D(ImageSource);

   // configHelpers::setQConfig(qobject, getSupportedProperties(), d->_props);
}

QJsonObject ImageSource::getConfig() const
{
    return {};
}

std::shared_ptr<QWidget> ImageSource::getQConfig() const
{
    const Q_D(ImageSource);

    return nullptr;
}

void ImageSource::configChanged() {
}
 
ComponentInterface::ComponentProperties ImageSource::getSupportedProperties() const
{
    std::vector<ComponentProperty> vec;
    return vec;

}

bool ImageSource::mainWidgetDocked() const
{
    return d_ptr->_docked;
}

void ImageSource::stopSimulation()
{
    Q_D(ImageSource);

    d->_simStarted = false;
}

void ImageSource::startSimulation()
{
    Q_D(ImageSource);

    d->_simStarted = true;
}

void ImageSource::txFrameIn(const QImage& frame)
{
    Q_D(ImageSource);
    
    if(d->acceptTxFrame(frame) && d->_simStarted) {
        emit txFrameOut(frame);
    }
}

void ImageSource::rxFrameIn(const QImage& frame)
{
    Q_D(ImageSource);

    //if(d->acceptRxFrame(frame) && d->_simStarted) {
    //    emit rxFrameOut(frame);
    //}
}

void ImageSource::simBcastRcv(const QJsonObject& msg, const QVariant& param)
{
    Q_UNUSED(msg);
    Q_UNUSED(param);
}


void ImageSource::slotReadFrame(const std::vector<QImage>& images)
{
    int imageSize = images.size();
    for (auto iter = images.begin();iter!=images.end();iter++) {
        emit txFrameOut(*iter);
    }
    //
}