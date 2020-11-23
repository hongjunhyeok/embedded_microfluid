/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pauseAndPlay;
    QVBoxLayout *verticalLayout_7;
    QGridLayout *gridLayout;
    QPushButton *prevStep;
    QPushButton *nextStep;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1040, 597);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        Widget->setMaximumSize(QSize(1040, 597));
        gridLayout_2 = new QGridLayout(Widget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));
        label->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(0, 0));
        label->setMaximumSize(QSize(16777215, 16777215));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);
        pushButton->setMinimumSize(QSize(194, 0));
        pushButton->setMaximumSize(QSize(194, 16777215));
        pushButton->setCheckable(true);
        pushButton->setChecked(false);

        verticalLayout_5->addWidget(pushButton);


        verticalLayout->addLayout(verticalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        pauseAndPlay = new QPushButton(Widget);
        pauseAndPlay->setObjectName(QStringLiteral("pauseAndPlay"));
        sizePolicy2.setHeightForWidth(pauseAndPlay->sizePolicy().hasHeightForWidth());
        pauseAndPlay->setSizePolicy(sizePolicy2);
        QFont font;
        font.setPointSize(18);
        pauseAndPlay->setFont(font);
        pauseAndPlay->setCheckable(true);
        pauseAndPlay->setChecked(false);

        verticalLayout_3->addWidget(pauseAndPlay);


        verticalLayout->addLayout(verticalLayout_3);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        prevStep = new QPushButton(Widget);
        prevStep->setObjectName(QStringLiteral("prevStep"));
        sizePolicy2.setHeightForWidth(prevStep->sizePolicy().hasHeightForWidth());
        prevStep->setSizePolicy(sizePolicy2);
        prevStep->setMinimumSize(QSize(93, 0));
        prevStep->setMaximumSize(QSize(93, 16777215));

        gridLayout->addWidget(prevStep, 0, 0, 1, 1);

        nextStep = new QPushButton(Widget);
        nextStep->setObjectName(QStringLiteral("nextStep"));
        sizePolicy2.setHeightForWidth(nextStep->sizePolicy().hasHeightForWidth());
        nextStep->setSizePolicy(sizePolicy2);
        nextStep->setMinimumSize(QSize(93, 0));
        nextStep->setMaximumSize(QSize(93, 16777215));

        gridLayout->addWidget(nextStep, 0, 1, 1, 1);


        verticalLayout_7->addLayout(gridLayout);


        verticalLayout->addLayout(verticalLayout_7);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        label->setText(QApplication::translate("Widget", "No Image", nullptr));
        pushButton->setText(QApplication::translate("Widget", "START!", nullptr));
        pauseAndPlay->setText(QApplication::translate("Widget", "Mode", nullptr));
        prevStep->setText(QApplication::translate("Widget", "Prev Step", nullptr));
        nextStep->setText(QApplication::translate("Widget", "Next Step", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
