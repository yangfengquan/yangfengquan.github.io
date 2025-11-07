#include "tymodule.h"

TYModule::TYModule(QWidget *parent)
    : QWidget{parent}
    , m_mainWidget(new QWidget(parent))
{
    //m_mainWidget->setGeometry(QRect(10, 10, 930, 380));
}

TYModule::~TYModule()
{}
