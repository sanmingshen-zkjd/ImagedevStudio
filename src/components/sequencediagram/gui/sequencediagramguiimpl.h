#pragma once

#include "sequencediagramguiint.h"
#include "sequencediagram.h"
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <log.h>
#include "ui_sequencediagram.h"

struct SequenceDiagramGuiImpl : public SequenceDiagramGuiInt {
public :
    SequenceDiagramGuiImpl()
        : _ui(new Ui::SequenceDiagramPrivate)
        , _widget(new QWidget)
    {
        _ui->setupUi(_widget);
        //QObject::connect(_ui->pbRxAdd, &QPushButton::pressed, std::bind(&CanDeviceGuiImpl::handleListAdd, this));
    }

    virtual QWidget* mainWidget()
    {
        return _widget;
    }

    void setTxListCbk(const ListUpdated_t& cb) 
    {
        _cb = cb;
    }
    //void setRxListCbk(const ListUpdated_t& cb) {}
    //void setListRx(const AcceptList_t& list) {}
    //void setListTx(const AcceptList_t& list) {}

private slots:
    void handleListAdd() 
    {
        QString fileName = QFileDialog::getOpenFileName(nullptr,"select image",QDir::homePath(),"image file (*.png *.jpg *.jpeg *.bmp *.gif *.tiff *.webp);;All Files (*.*)");
        if (!fileName.isEmpty()) {
            qDebug() << "choosen image :" << fileName;
            QImage image(fileName);
            QPixmap pixmap(fileName);
            if (!pixmap.isNull()) {
                qDebug() << "image size :" << pixmap.size();
            } else {
                QMessageBox::warning(nullptr, "warning", "unable to load image��");
            }
            _ui->label_3->setPixmap(pixmap);
            _list.push_back(image);
            _cb(_list);
        } else {
            qDebug() << "user canceled!";
        }
    }

private:
    Ui::SequenceDiagramPrivate* _ui;
    QWidget* _widget;
    AcceptList_t _list;
    ListUpdated_t _cb;
};
