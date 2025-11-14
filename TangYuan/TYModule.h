#ifndef TYMODULE_H
#define TYMODULE_H

#include <QWidget>
#include <any>

class TYModule : public QWidget
{
    Q_OBJECT
public:
    explicit TYModule(QWidget *parent = nullptr)
        : QWidget{parent}
        , m_mainWidget(new QWidget(parent))
    {}
    ~TYModule(){};

    virtual void open(const std::map<std::string, std::any>& data) = 0;
    virtual std::map<std::string, std::any> data() = 0;
    virtual QString calculate() = 0;

protected:
    QWidget *m_mainWidget;
    virtual void setupUi() = 0;
};

#endif // TYMODULE_H
