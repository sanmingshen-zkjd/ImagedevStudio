#pragma once

#include "sequencediagram.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QtCore/QVector>
#include "gui/sequencediagramguiimpl.h"
//#include <propertyfields.h>

class SequenceDiagram;

class SequenceDiagramPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(SequenceDiagram)

public:
    SequenceDiagramPrivate(
        SequenceDiagram* q, SequenceDiagramCtx&& ctx = SequenceDiagramCtx(new SequenceDiagramGuiImpl))
        : _ctx(std::move(ctx))
        ,_ui(_ctx.get<SequenceDiagramGuiInt>()) 
        ,q_ptr(q)
    {
       // initProps();
        //_ui.setTxListCbk([this](const CanRawFilterGuiInt::AcceptList_t& list) {
        //    _rxAcceptList = list;
        //    emit broadcastImageList(list);
        //});
    }

    void saveSettings(QJsonObject& json)
    {
        //QJsonArray viewModelsArray;

        //for (const auto& p : _props) {
        //    json[p.first] = QJsonValue::fromVariant(p.second);
        //}
    }

    bool restoreConfiguration(const QJsonObject& json)
    {
        //for (const auto& p : _supportedProps) {
        //    QString propName = ComponentInterface::propertyName(p);
        //    if (json.contains(propName))
        //        _props[propName] = json[propName].toVariant();
        //}
        return true;
    }

    using devConfigPair = std::pair<int, QVariant>;
    std::vector<devConfigPair> getDevConfig()
    {
        //QString c = _props.at(_configProperty).toString().simplified().replace(" ", "");
        std::vector<devConfigPair> ret;

        //if (c.length() == 0) {
        //    // Skip parsing for empty parameter
        //    return {};
        //}

        //auto&& propList = c.split(";");

        //for (auto& item : propList) {
        //    devConfigPair pair;
        //    bool res = getConfigPair(item, pair);

        //    if (res) {
        //        ret.push_back(pair);
        //    }
        //}

        return ret;
    }

signals:
    void backendChanged(const QString& backend);

public:
  //  CanDeviceCtx _ctx;
  //  QVector<QImage> _sendQueue;
    //CanDeviceInterface& _canDevice;
    bool _initialized{ false };
    bool _simStarted{ false };

    SequenceDiagramCtx _ctx;
    SequenceDiagramGuiInt& _ui;

    //const QString _nameProperty = "name";
    //const QString _backendProperty = "backend";
    //const QString _interfaceProperty = "interface";
    //const QString _configProperty = "configuration";

    // workaround for clang 3.5
    using cf = ComponentInterface::CustomEditFieldCbk;

    // clang-format off
    //ComponentInterface::ComponentProperties _supportedProps = {
    //        std::make_tuple(_nameProperty,  QVariant::String, true, cf(nullptr)),
    //        std::make_tuple(_backendProperty, QVariant::String, true, cf(nullptr)),
    //        std::make_tuple(_interfaceProperty, QVariant::String, true, cf(nullptr)),
    //        std::make_tuple(_configProperty, QVariant::String, true, cf(nullptr))
    //};
    // clang-format on

 //   std::map<QString, QVariant> _props;

private:
    //void initProps()
    //{
    //    for (const auto& p : _supportedProps) {
    //        _props[ComponentInterface::propertyName(p)];
    //    }
    //}

    //bool getConfigPair(const QString& in, devConfigPair& out)
    //{
    //    auto configStr = in.split("=");

    //    if ((configStr.length() != 2) || (configStr[1].length() == 0)) {
    //        return false;
    //    }

    //    auto keyStr = configStr[0];
    //    auto valStr = configStr[1];

    //    return true;
    //}

    SequenceDiagram* q_ptr;
    QMetaObject::Connection _prevConn;
};