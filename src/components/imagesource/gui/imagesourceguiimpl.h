#pragma once

#include "imagesourceguiint.h"
#include "ui_canrawfilter.h"
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <log.h>

struct ImageSourceGuiImpl : public ImageSourceGuiInt {
public :
    ImageSourceGuiImpl()
        : _ui(new Ui::CanRawFilterPrivate)
        , _widget(new QWidget)
    {
        _ui->setupUi(_widget);
        QObject::connect(_ui->pbRxAdd, &QPushButton::pressed, std::bind(&ImageSourceGuiImpl::handleListAdd, this));
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
        // 打开文件对话框，选择图片文件
        QString fileName = QFileDialog::getOpenFileName(nullptr, // 父窗口
            "select image", // 对话框标题
            QDir::homePath(), // 默认目录（用户主目录）
            "image file (*.png *.jpg *.jpeg *.bmp *.gif *.tiff *.webp);;All Files (*.*)" // 文件过滤器
        );
        if (!fileName.isEmpty()) {
            // 处理选中的图片文件
            qDebug() << "choosen image :" << fileName;

            QImage image(fileName);
            // 加载图片
            QPixmap pixmap(fileName);
            if (!pixmap.isNull()) {
                // 图片加载成功
                qDebug() << "image size :" << pixmap.size();
            } else {
                QMessageBox::warning(nullptr, "warning", "unable to load image！");
            }
            _ui->label_3->setPixmap(pixmap);
            _list.push_back(image);
            _cb(_list);
        } else {
            qDebug() << "user canceled!";
        }
    }

private:
    Ui::CanRawFilterPrivate* _ui;
    QWidget* _widget;
    AcceptList_t _list;
    ListUpdated_t _cb;
};