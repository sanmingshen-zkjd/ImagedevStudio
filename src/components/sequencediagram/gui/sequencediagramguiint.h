#pragma once

#include <Qt>
#include <functional>
#include <tuple>
#include <vector>

class QWidget;
class QImage;

struct SequenceDiagramGuiInt {
    typedef std::vector<QImage> AcceptList_t;
    typedef std::function<void(AcceptList_t& list)> ListUpdated_t;

    virtual ~SequenceDiagramGuiInt() {}

    virtual QWidget* mainWidget() = 0;
    virtual void setTxListCbk(const ListUpdated_t& cb) = 0;
    //virtual void setRxListCbk(const ListUpdated_t& cb) = 0;
    //virtual void setListRx(const AcceptList_t& list) = 0;
    //virtual void setListTx(const AcceptList_t& list) = 0;
};