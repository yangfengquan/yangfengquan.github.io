#ifndef ACTIVATIONDIALOG_H
#define ACTIVATIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "ActivationManager.h"

class ActivationDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief 激活对话框构造函数
     * @param parent 父窗口
     * @param activationManager 激活管理器
     */
    explicit ActivationDialog(QWidget *parent, ActivationManager *activationManager);
    ~ActivationDialog();

private slots:
    /**
     * @brief 验证激活码
     */
    void validateActivation();

    /**
     * @brief 复制申请码到剪贴板
     */
    void copyRequestCode();

private:
    /**
     * @brief 初始化界面
     */
    void setupUI();

    QLineEdit *requestCodeEdit;
    QPushButton *copyButton;
    QLineEdit *activationCodeEdit;
    QPushButton *activateButton;
    QPushButton *exitButton;

    // 数据
    ActivationManager *activationManager;
    QString requestCode;
};

#endif // ACTIVATIONDIALOG_H
