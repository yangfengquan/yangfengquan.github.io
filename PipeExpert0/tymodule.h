#ifndef TYMODULE_H
#define TYMODULE_H

#include <QWidget>

class TYModule : public QWidget
{
    Q_OBJECT
public:
    explicit TYModule(QWidget *parent = nullptr);
    ~TYModule();
    virtual void setupUi() = 0;
    virtual void open(QVariantMap data) = 0;
    virtual QVariantMap save() = 0;
    virtual QString calculate() = 0;

    QWidget *m_mainWidget;
};

#endif // TYMODULE_H
