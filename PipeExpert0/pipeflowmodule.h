#ifndef PIPEFLOWMODULE_H
#define PIPEFLOWMODULE_H

#include "tymodule.h"
#include <QWidget>

class PipeFlowModule : public TYModule
{
    Q_OBJECT
public:
    PipeFlowModule(QWidget *parent = nullptr);
};

#endif // PIPEFLOWMODULE_H
