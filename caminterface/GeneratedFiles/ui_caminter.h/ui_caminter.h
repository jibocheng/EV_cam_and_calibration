/********************************************************************************
** Form generated from reading UI file 'caminter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMINTER_H
#define UI_CAMINTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_camdisplay
{
public:
    QWidget *configuration;
    QListWidget *CamListWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *Cameradeviceslabel;
    QComboBox *Devices;
    QLabel *label;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *Cameradeviceslabel_2;
    QComboBox *Imgtype;
    QWidget *abovewidget;
    QPushButton *D2;
    QPushButton *D3;
    QFrame *line_2;
    QWidget *Start;
    QPushButton *run_all;
    QPushButton *export_2;
    QFrame *line;
    QPushButton *run;
    QFrame *line_3;
    QPushButton *detect;
    QFrame *line_4;
    QPushButton *stop;
    QPushButton *stop_all;
    QFrame *line_5;
    QWidget *gap;
    QLabel *display;

    void setupUi(QWidget *camdisplay)
    {
        if (camdisplay->objectName().isEmpty())
            camdisplay->setObjectName(QString::fromUtf8("camdisplay"));
        camdisplay->resize(1350, 897);
        camdisplay->setStyleSheet(QString::fromUtf8("outline: none;\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(62, 63, 63);"));
        configuration = new QWidget(camdisplay);
        configuration->setObjectName(QString::fromUtf8("configuration"));
        configuration->setGeometry(QRect(0, 40, 251, 871));
        configuration->setStyleSheet(QString::fromUtf8("outline: none;\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(42, 43, 43);"));
        CamListWidget = new QListWidget(configuration);
        CamListWidget->setObjectName(QString::fromUtf8("CamListWidget"));
        CamListWidget->setGeometry(QRect(0, 100, 251, 81));
        CamListWidget->setStyleSheet(QString::fromUtf8("min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);"));
        layoutWidget = new QWidget(configuration);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 10, 251, 28));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        Cameradeviceslabel = new QLabel(layoutWidget);
        Cameradeviceslabel->setObjectName(QString::fromUtf8("Cameradeviceslabel"));
        Cameradeviceslabel->setStyleSheet(QString::fromUtf8("color:white; font-size:15px"));

        horizontalLayout->addWidget(Cameradeviceslabel);

        Devices = new QComboBox(layoutWidget);
        Devices->addItem(QString());
        Devices->setObjectName(QString::fromUtf8("Devices"));
        Devices->setStyleSheet(QString::fromUtf8("min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);"));

        horizontalLayout->addWidget(Devices);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 3);
        label = new QLabel(configuration);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 70, 151, 31));
        label->setStyleSheet(QString::fromUtf8("color:white; font-size:15px"));
        layoutWidget_2 = new QWidget(configuration);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(0, 40, 251, 28));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        Cameradeviceslabel_2 = new QLabel(layoutWidget_2);
        Cameradeviceslabel_2->setObjectName(QString::fromUtf8("Cameradeviceslabel_2"));
        Cameradeviceslabel_2->setStyleSheet(QString::fromUtf8("color:white; font-size:15px"));

        horizontalLayout_2->addWidget(Cameradeviceslabel_2);

        Imgtype = new QComboBox(layoutWidget_2);
        Imgtype->addItem(QString());
        Imgtype->addItem(QString());
        Imgtype->addItem(QString());
        Imgtype->setObjectName(QString::fromUtf8("Imgtype"));
        Imgtype->setStyleSheet(QString::fromUtf8("min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);"));

        horizontalLayout_2->addWidget(Imgtype);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 3);
        abovewidget = new QWidget(camdisplay);
        abovewidget->setObjectName(QString::fromUtf8("abovewidget"));
        abovewidget->setGeometry(QRect(0, 0, 1351, 41));
        abovewidget->setStyleSheet(QString::fromUtf8("background-color: rgb(135, 155, 170);"));
        D2 = new QPushButton(abovewidget);
        D2->setObjectName(QString::fromUtf8("D2"));
        D2->setGeometry(QRect(1210, 0, 61, 41));
        D3 = new QPushButton(abovewidget);
        D3->setObjectName(QString::fromUtf8("D3"));
        D3->setGeometry(QRect(1290, 0, 61, 41));
        line_2 = new QFrame(abovewidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(1270, 0, 21, 41));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        Start = new QWidget(camdisplay);
        Start->setObjectName(QString::fromUtf8("Start"));
        Start->setGeometry(QRect(270, 40, 1081, 61));
        Start->setStyleSheet(QString::fromUtf8("outline: none;\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(42, 43, 43);"));
        run_all = new QPushButton(Start);
        run_all->setObjectName(QString::fromUtf8("run_all"));
        run_all->setGeometry(QRect(220, 0, 91, 61));
        run_all->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);\n"
"}\n"
"QPushButton:hover{\n"
"color: rgb(255, 255, 255);\n"
"border: 2px solid gray;\n"
"border-radius:4px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(70, 70, 83);\n"
"}\n"
"QPushButton:pressed{\n"
"border-radius:5px;border:4px solid gray;\n"
"background-color: rgb(90, 90, 103);\n"
"border-color: rgb(35, 143, 159);\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:disabled{\n"
"border-radius:5px;\n"
"border:2px solid gray;\n"
"background-color: rgb(20, 30, 13);\n"
"border-color: rgb(15, 103, 109);\n"
"color: rgb(205, 205, 205);\n"
"border-style: outset;\n"
"padding: 1px;\n"
"}"));
        export_2 = new QPushButton(Start);
        export_2->setObjectName(QString::fromUtf8("export_2"));
        export_2->setGeometry(QRect(990, 0, 91, 61));
        export_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);\n"
"}\n"
"QPushButton:hover{\n"
"color: rgb(255, 255, 255);\n"
"border: 2px solid gray;\n"
"border-radius:4px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(70, 70, 83);\n"
"}\n"
"QPushButton:pressed{\n"
"border-radius:5px;border:4px solid gray;\n"
"background-color: rgb(90, 90, 103);\n"
"border-color: rgb(35, 143, 159);\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:disabled{\n"
"border-radius:5px;\n"
"border:2px solid gray;\n"
"background-color: rgb(20, 30, 13);\n"
"border-color: rgb(15, 103, 109);\n"
"color: rgb(205, 205, 205);\n"
"border-style: outset;\n"
"padding: 1px;\n"
"}"));
        line = new QFrame(Start);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(970, 0, 21, 61));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        run = new QPushButton(Start);
        run->setObjectName(QString::fromUtf8("run"));
        run->setGeometry(QRect(120, 0, 91, 61));
        run->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);\n"
"}\n"
"QPushButton:hover{\n"
"color: rgb(255, 255, 255);\n"
"border: 2px solid gray;\n"
"border-radius:4px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(70, 70, 83);\n"
"}\n"
"QPushButton:pressed{\n"
"border-radius:5px;border:4px solid gray;\n"
"background-color: rgb(90, 90, 103);\n"
"border-color: rgb(35, 143, 159);\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:disabled{\n"
"border-radius:5px;\n"
"border:2px solid gray;\n"
"background-color: rgb(20, 30, 13);\n"
"border-color: rgb(15, 103, 109);\n"
"color: rgb(205, 205, 205);\n"
"border-style: outset;\n"
"padding: 1px;\n"
"}"));
        line_3 = new QFrame(Start);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(100, 0, 21, 61));
        line_3->setStyleSheet(QString::fromUtf8("color:rgb(255, 255, 255)"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        detect = new QPushButton(Start);
        detect->setObjectName(QString::fromUtf8("detect"));
        detect->setGeometry(QRect(10, 0, 91, 61));
        detect->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);\n"
"}\n"
"QPushButton:hover{\n"
"color: rgb(255, 255, 255);\n"
"border: 2px solid gray;\n"
"border-radius:4px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(70, 70, 83);\n"
"}\n"
"QPushButton:pressed{\n"
"border-radius:5px;border:4px solid gray;\n"
"background-color: rgb(90, 90, 103);\n"
"border-color: rgb(35, 143, 159);\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:disabled{\n"
"border-radius:5px;\n"
"border:2px solid gray;\n"
"background-color: rgb(20, 30, 13);\n"
"border-color: rgb(15, 103, 109);\n"
"color: rgb(205, 205, 205);\n"
"border-style: outset;\n"
"padding: 1px;\n"
"}"));
        line_4 = new QFrame(Start);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(320, 0, 21, 61));
        line_4->setStyleSheet(QString::fromUtf8("color:rgb(255, 255, 255)"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        stop = new QPushButton(Start);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setGeometry(QRect(340, 0, 91, 61));
        stop->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);\n"
"}\n"
"QPushButton:hover{\n"
"color: rgb(255, 255, 255);\n"
"border: 2px solid gray;\n"
"border-radius:4px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(70, 70, 83);\n"
"}\n"
"QPushButton:pressed{\n"
"border-radius:5px;border:4px solid gray;\n"
"background-color: rgb(90, 90, 103);\n"
"border-color: rgb(35, 143, 159);\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:disabled{\n"
"border-radius:5px;\n"
"border:2px solid gray;\n"
"background-color: rgb(20, 30, 13);\n"
"border-color: rgb(15, 103, 109);\n"
"color: rgb(205, 205, 205);\n"
"border-style: outset;\n"
"padding: 1px;\n"
"}"));
        stop_all = new QPushButton(Start);
        stop_all->setObjectName(QString::fromUtf8("stop_all"));
        stop_all->setGeometry(QRect(450, 0, 91, 61));
        stop_all->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"min-width: 60px; min-height: 22px;\n"
"text-align: center;\n"
"border: 2px solid gray;\n"
"color: rgb(255, 255, 255);\n"
"border-radius:5px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(50, 50, 63);\n"
"}\n"
"QPushButton:hover{\n"
"color: rgb(255, 255, 255);\n"
"border: 2px solid gray;\n"
"border-radius:4px;\n"
"border-color: rgb(35, 143, 159);\n"
"background-color: rgb(70, 70, 83);\n"
"}\n"
"QPushButton:pressed{\n"
"border-radius:5px;border:4px solid gray;\n"
"background-color: rgb(90, 90, 103);\n"
"border-color: rgb(35, 143, 159);\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:disabled{\n"
"border-radius:5px;\n"
"border:2px solid gray;\n"
"background-color: rgb(20, 30, 13);\n"
"border-color: rgb(15, 103, 109);\n"
"color: rgb(205, 205, 205);\n"
"border-style: outset;\n"
"padding: 1px;\n"
"}"));
        line_5 = new QFrame(Start);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(550, 0, 21, 61));
        line_5->setStyleSheet(QString::fromUtf8("color:rgb(255, 255, 255)"));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);
        gap = new QWidget(camdisplay);
        gap->setObjectName(QString::fromUtf8("gap"));
        gap->setGeometry(QRect(250, 40, 20, 871));
        gap->setStyleSheet(QString::fromUtf8("background-color: rgb(204, 204, 204);"));
        display = new QLabel(camdisplay);
        display->setObjectName(QString::fromUtf8("display"));
        display->setGeometry(QRect(270, 100, 1061, 811));

        retranslateUi(camdisplay);

        QMetaObject::connectSlotsByName(camdisplay);
    } // setupUi

    void retranslateUi(QWidget *camdisplay)
    {
        camdisplay->setWindowTitle(QCoreApplication::translate("camdisplay", "Form", nullptr));
        Cameradeviceslabel->setText(QCoreApplication::translate("camdisplay", "Devices:", nullptr));
        Devices->setItemText(0, QCoreApplication::translate("camdisplay", "InterRealSense", nullptr));

        label->setText(QCoreApplication::translate("camdisplay", "Devices Description:", nullptr));
        Cameradeviceslabel_2->setText(QCoreApplication::translate("camdisplay", "ImgType:", nullptr));
        Imgtype->setItemText(0, QCoreApplication::translate("camdisplay", "RGB", nullptr));
        Imgtype->setItemText(1, QCoreApplication::translate("camdisplay", "DEPTH", nullptr));
        Imgtype->setItemText(2, QCoreApplication::translate("camdisplay", "POINTCLOUD", nullptr));

        D2->setText(QCoreApplication::translate("camdisplay", "2D", nullptr));
        D3->setText(QCoreApplication::translate("camdisplay", "3D", nullptr));
        run_all->setText(QCoreApplication::translate("camdisplay", "Run all", nullptr));
        export_2->setText(QCoreApplication::translate("camdisplay", "Export", nullptr));
        run->setText(QCoreApplication::translate("camdisplay", "Run", nullptr));
        detect->setText(QCoreApplication::translate("camdisplay", "Detect", nullptr));
        stop->setText(QCoreApplication::translate("camdisplay", "Stop", nullptr));
        stop_all->setText(QCoreApplication::translate("camdisplay", "Stop all", nullptr));
        display->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class camdisplay: public Ui_camdisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMINTER_H
