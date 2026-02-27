#pragma once

// 1. 显式包含QObject头文件（必备，Qt元对象系统基础）
#include "brightnesscontrast.h"
#include "brightnesscontrastguiint.h"
#include "qcustomplot/qcustomplot.h"
#include "ui_brightnesscontrast.h"
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QStyledItemDelegate>
#include <QWidget>
#include <algorithm> // 用于std::max_element，你的代码中用到了需显式包含
#include <log.h>

#pragma comment(lib, "D:\\MyWork\\ImageProcessProduct\\ImageProcessApp\\thirdpart\\opencv440\\lib\\x64\\Debug\\opencv_world440d.lib")

// 核心修正1：继承QObject（必须放在最左侧，Qt多继承规范）
// 核心修正2：添加Q_OBJECT宏（Qt元对象系统开关，必须加）
// 核心修正3：struct改class（Qt开发规范，避免默认公有带来的权限问题）
class BrightnessContrastGuiImpl : public QObject, public BrightnessContrastGuiInt {
    Q_OBJECT // 核心修正4：添加Q_OBJECT宏，必须位于类声明第一行（private区前）

    public : BrightnessContrastGuiImpl()
        : _ui(new Ui::BrightnessContrastPrivate)
        , _widget(new QWidget)
    {
        _ui->setupUi(_widget);
        m_brightCount.resize(256);
        m_brightCount.fill(0);
        initCustomPlot();
        // 信号槽连接代码不变（std::bind写法正确，修复后可正常工作）
        QObject::connect(_ui->horizontalScrollBarMin, &QScrollBar::valueChanged,
            std::bind(&BrightnessContrastGuiImpl::updateMinLabel, this, std::placeholders::_1));
        QObject::connect(_ui->horizontalScrollBarMax, &QScrollBar::valueChanged,
            std::bind(&BrightnessContrastGuiImpl::updateMaxLabel, this, std::placeholders::_1));
    }

    virtual QWidget* mainWidget() override // 加override更规范，检测父类虚函数匹配
    {
        return _widget;
    }

    void setTxListCbk(const ListUpdated_t& cb) override
    {
        _cb = cb;
    }

    void setCurImage(QImage image) {
        m_loadedImage = image;
        calculateBrightnessDistribution();
        plotHistogramAndDiagonal();
        _ui->widget_2->setImage(m_loadedImage);
    }

    void initCustomPlot()
    {
        QCustomPlot* plot = _ui->widget;
        plot->xAxis->setLabel("像素亮度值（0-255）");
        plot->yAxis->setLabel("像素数量");
        plot->xAxis->setRange(0, 256);
        plot->yAxis->setRange(0, 1);
        plot->xAxis->grid()->setVisible(true);
        plot->yAxis->grid()->setVisible(true);

        m_brightBars = new QCPBars(plot->xAxis, plot->yAxis);
        m_brightBars->setPen(Qt::NoPen);
        m_brightBars->setBrush(QColor(180, 180, 180));
        m_brightBars->setWidth(1);
        m_brightBars->setParent(plot);

        plot->addGraph();
        plot->graph(0)->setPen(QPen(Qt::red, 2));
    }

private:
    void plotHistogramAndDiagonal()
    {
        QCustomPlot* plot = _ui->widget;
        if (m_loadedImage.isNull() || !m_brightBars)
            return;

        QVector<double> xBright(256), yCount(256);
        for (int i = 0; i < 256; i++) {
            xBright[i] = i;
            yCount[i] = m_brightCount[i];
        }

        m_brightBars->setData(xBright, yCount);

        QVector<double> xDiag(2), yDiag(2);
        xDiag[0] = m_minBright;
        yDiag[0] = 0;
        int maxPixelCount = *std::max_element(m_brightCount.begin(), m_brightCount.end());
        xDiag[1] = m_maxBright;
        yDiag[1] = maxPixelCount;
        plot->graph(0)->setData(xDiag, yDiag);

        plot->yAxis->setRange(0, maxPixelCount * 1.1);
        plot->replot();
        plot->update();
    }

    void calculateBrightnessDistribution()
    {
        std::fill(m_brightCount.begin(), m_brightCount.end(), 0);
        QImage grayImage = m_loadedImage.convertToFormat(QImage::Format_Grayscale8);
        int width = grayImage.width();
        int height = grayImage.height();

        for (int y = 0; y < height; y++) {
            uchar* row = grayImage.scanLine(y);
            for (int x = 0; x < width; x++) {
                int bright = row[x];
                m_brightCount[bright]++;
            }
        }
    }

private slots:
    // 修复后：sender()可正常调用，无编译错误
    void updateMaxLabel(int currentValue)
    {

        if (currentValue <= m_minBright) {
            m_maxBright = m_minBright + 1;
            _ui->horizontalScrollBarMax->setValue(m_maxBright);
        }else {
            m_maxBright = currentValue;
        }
        // 亮度范围变化后，重新绘制直方图和对角线
        plotHistogramAndDiagonal();
    }

    void updateMinLabel(int currentValue)
    {
        if (currentValue >= m_maxBright) {
            _ui->horizontalScrollBarMin->setValue(m_maxBright - 1);
            m_minBright = m_maxBright - 1;
        } else {
            m_minBright = currentValue;
        }
        plotHistogramAndDiagonal();
    }

private:
    Ui::BrightnessContrastPrivate* _ui;
    QWidget* _widget;
    AcceptList_t _list;
    ListUpdated_t _cb;

    //直方图绘制
    QImage m_loadedImage;
    QVector<int> m_brightCount;
    int m_minBright = 0;
    int m_maxBright = 255;
    QCPBars* m_brightBars = nullptr;
};