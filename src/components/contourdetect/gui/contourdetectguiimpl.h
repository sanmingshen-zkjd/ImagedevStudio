#pragma once

#include "contourdetectguiint.h"
#include "contourdetect.h"
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <log.h>
#include <QObject>
#include "ui_contourdetect.h"
#include "auto_thresholder.h"
#include "opencv2/opencv.hpp"

class ContourDetectGuiImpl : public QObject, public ContourDetectGuiInt {
    Q_OBJECT // 核心修正4：添加Q_OBJECT宏，必须位于类声明第一行（private区前）
public :
    ContourDetectGuiImpl()
        : _ui(new Ui::ContourDetectPrivate)
        , _widget(new QWidget)
    {
        _ui->setupUi(_widget);
        //QObject::connect(_ui->pbRxAdd, &QPushButton::pressed, std::bind(&CanDeviceGuiImpl::handleListAdd, this));
        //QObject::connect(_ui->comboBoxAlgorithm,&QComboBox::clicked)
        initAlgorithms();
        connect(_ui->comboBoxAlgorithm, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &ContourDetectGuiImpl::onThresholdAlgorithmIndexChanged);
    }

    virtual QWidget* mainWidget()
    {
        return _widget;
    }

    void initAlgorithms() 
    {
        m_functionMap["DefaultIsoData"] = AutoThresholder::DefaultIsoData;
        //   functionMap["IJIsoData"] = AutoThresholder::IJIsoData;
        m_functionMap["Huang"] = AutoThresholder::Huang;
        m_functionMap["Intermodes"] = AutoThresholder::Intermodes;
        m_functionMap["IsoData"] = AutoThresholder::IsoData;
        m_functionMap["Li"] = AutoThresholder::Li;
        m_functionMap["MaxEntropy"] = AutoThresholder::MaxEntropy;
        m_functionMap["Mean"] = AutoThresholder::Mean;
        m_functionMap["MinErrorI"] = AutoThresholder::MinErrorI;
        m_functionMap["Minimum"] = AutoThresholder::Minimum;
        m_functionMap["Moments"] = AutoThresholder::Moments;
        m_functionMap["Otsu"] = AutoThresholder::Otsu;
        m_functionMap["Percentile"] = AutoThresholder::Percentile;
        m_functionMap["RenyiEntropy"] = AutoThresholder::RenyiEntropy;
        m_functionMap["Shanbhag"] = AutoThresholder::Shanbhag;
        //  functionMap["Triangle"] = AutoThresholder::Triangle;
        m_functionMap["Yen"] = AutoThresholder::Yen;

        _ui->comboBoxAlgorithm->addItem("None Algorithm");
        _ui->comboBoxAlgorithm->addItem("All Algorithm");
        for (auto iter = m_functionMap.begin(); iter != m_functionMap.end(); iter++) {
            _ui->comboBoxAlgorithm->addItem(iter->first);
        }

        _ui->comboBoxLocalAlgorithm->addItem("None Algorithm");
        _ui->comboBoxLocalAlgorithm->addItem("All Algorithm");
        for (auto iter = m_functionLocalThresholdMap.begin(); iter != m_functionLocalThresholdMap.end(); iter++) {
            _ui->comboBoxLocalAlgorithm->addItem(iter->first);
        }
    }

    void setTxListCbk(const ListUpdated_t& cb) 
    {
        _cb = cb;
    }

    void setCurImage(QImage image)
    {
        m_loadedImage = image;
        //calculateBrightnessDistribution();
        //plotHistogramAndDiagonal();
        _ui->widgetGreyImage->setImage(m_loadedImage);
    }
    //void setRxListCbk(const ListUpdated_t& cb) {}
    //void setListRx(const AcceptList_t& list) {}
    //void setListTx(const AcceptList_t& list) {}

private slots:
    void handleListAdd() 
    {
        //QString fileName = QFileDialog::getOpenFileName(nullptr,"select image",QDir::homePath(),"image file (*.png *.jpg *.jpeg *.bmp *.gif *.tiff *.webp);;All Files (*.*)");
        //if (!fileName.isEmpty()) {
        //    qDebug() << "choosen image :" << fileName;
        //    QImage image(fileName);
        //    QPixmap pixmap(fileName);
        //    if (!pixmap.isNull()) {
        //        qDebug() << "image size :" << pixmap.size();
        //    } else {
        //        QMessageBox::warning(nullptr, "warning", "unable to load image��");
        //    }
        //    _ui->label_3->setPixmap(pixmap);
        //    _list.push_back(image);
        //    _cb(_list);
        //} else {
        //    qDebug() << "user canceled!";
        //}
    }
    void onThresholdAlgorithmIndexChanged(int id)
    {
        if (id == 0)
            return;
        if (id == 1) {
            //on_checkBoxAllAlgorithm_clicked(true);
            return;
        }

        QImage img = m_loadedImage.convertToFormat(QImage::Format_RGB888);
        int width = img.width();
        int height = img.height();
        uchar* data = img.bits();
        cv::Mat mat(height, width, CV_8UC3, data, img.bytesPerLine());
        cv::Mat processedImage;
        cv::cvtColor(mat, processedImage, cv::COLOR_BGR2GRAY);

        std::vector<int> hist;
        AutoThresholder::CalcHist(processedImage, hist);

        int i = 0;
        for (auto iter = m_functionMap.begin(); iter != m_functionMap.end(); iter++, i++) {
            if (i == id - 1) {
                m_usedFunc = iter->second;
                cv::Mat binImg;
                auto name = iter->first;
                auto func = iter->second;
                int threshold = func(hist);
                cv::threshold(processedImage, binImg, threshold, 255, cv::THRESH_BINARY_INV);
                QImage qimg = matToQImage(binImg, false);
                _ui->widgetBinImage->setImage(qimg);
               // m_slider->setValue(threshold);
                // QImage qimg = CTools::matToQImage(binImg, false);  // 保持灰度格式
                // if (qimg.isNull()) {
                //    qDebug() << "图像转换失败";
                //    return;
                //}
                // emit signalUpdateWindow(qimg, qimg);
                break;
            }
        }
    }

    QImage matToQImage(const cv::Mat& mat, bool convertToRGB)
    {
        if (mat.empty()) {
            qWarning() << "OpenCV Mat is empty";
            return QImage();
        }

        cv::Mat convertedMat;

        // 根据输入格式进行转换
        switch (mat.type()) {
        case CV_8UC1: // 灰度图像
            if (convertToRGB) {
                cv::cvtColor(mat, convertedMat, cv::COLOR_GRAY2RGB);
                return QImage(convertedMat.data, convertedMat.cols, convertedMat.rows,
                    static_cast<int>(convertedMat.step), QImage::Format_RGB888)
                    .copy();
            } else {
                // 保持灰度格式
                return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8)
                    .copy();
            }
            break;

        case CV_8UC3: // BGR 图像
            if (convertToRGB) {
                cv::cvtColor(mat, convertedMat, cv::COLOR_BGR2RGB);
                return QImage(convertedMat.data, convertedMat.cols, convertedMat.rows,
                    static_cast<int>(convertedMat.step), QImage::Format_RGB888)
                    .copy();
            } else {
                // 直接使用 BGR（注意：Qt 显示可能颜色不对）
                return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888).copy();
            }
            break;

        case CV_8UC4: // BGRA 图像
            if (convertToRGB) {
                cv::cvtColor(mat, convertedMat, cv::COLOR_BGRA2RGBA);
                return QImage(convertedMat.data, convertedMat.cols, convertedMat.rows,
                    static_cast<int>(convertedMat.step), QImage::Format_RGBA8888)
                    .copy();
            } else {
                return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGBA8888).copy();
            }
            break;

        default:
            qWarning() << "Unsupported image format:" << mat.type();
            return QImage();
        }
    }

private:
    Ui::ContourDetectPrivate* _ui;
    QWidget* _widget;
    AcceptList_t _list;
    ListUpdated_t _cb;
    QImage m_loadedImage;
    std::function<int(const std::vector<int>&)> m_usedFunc;
    std::map<QString, std::function<int(const std::vector<int>&)>> m_functionMap;
    std::map<QString, std::function<int(const std::vector<int>&)>> m_functionLocalThresholdMap;
};