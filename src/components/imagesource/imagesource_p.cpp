#include "imagesource_p.h"
#include <QJsonArray>
#include <QRegularExpression>
#include <log.h>

ImageSourcePrivate::ImageSourcePrivate(ImageSource* q, ImageSourceCtx&& ctx)
    : _ctx(std::move(ctx))
    , _ui(_ctx.get<ImageSourceGuiInt>())
    , q_ptr(q)
{
    _ui.setTxListCbk([this](const ImageSourceGuiInt::AcceptList_t& list) {
        _rxAcceptList = list;
        emit broadcastImageList(list);
    });
}

bool ImageSourcePrivate::acceptTxFrame(const QImage& frame)
{
    return true;
    //return acceptFrame(_txAcceptList, frame);
}
