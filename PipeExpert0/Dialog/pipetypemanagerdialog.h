#ifndef PIPETYPEMANAGERDIALOG_H
#define PIPETYPEMANAGERDIALOG_H

#include <QDialog>

namespace Ui {
class PipeTypeManagerDialog;
}

class PipeTypeManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PipeTypeManagerDialog(QWidget *parent = nullptr);
    ~PipeTypeManagerDialog();

private:
    Ui::PipeTypeManagerDialog *ui;
};

#endif // PIPETYPEMANAGERDIALOG_H
