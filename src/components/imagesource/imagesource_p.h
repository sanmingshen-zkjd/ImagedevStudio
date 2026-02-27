#pragma once

#include "imagesource.h"
#include "gui/imagesourceguiimpl.h"
#include <QtCore/QObject>
#include <memory>

class ImageSource;

class ImageSourcePrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(ImageSource)

public:
    ImageSourcePrivate(ImageSource* q, ImageSourceCtx&& ctx = ImageSourceCtx(new ImageSourceGuiImpl));
    bool acceptTxFrame(const QImage& frame);

public:
    bool _simStarted{ false };
    ImageSourceCtx _ctx;
    ImageSourceGuiInt& _ui;
    bool _docked{ false };


signals:
    void broadcastImageList(const std::vector<QImage>& images);

private:
    ImageSourceGuiInt::AcceptList_t _rxAcceptList;
    ImageSource* q_ptr;
};
