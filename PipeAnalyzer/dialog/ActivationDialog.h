#ifndef ACTIVATIONDIALOG_H
#define ACTIVATIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextBrowser>
#include <QGroupBox>
#include <QMessageBox>
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

    /**
     * @brief 生成新的申请码
     */
    void generateNewRequestCode();

    /**
     * @brief 打开QQ群链接
     */
    void openQQGroup();

private:
    /**
     * @brief 初始化界面
     */
    void setupUI();

    /**
     * @brief 创建已激活状态界面
     */
    void createActivatedUI();

    /**
     * @brief 创建未激活状态界面
     */
    void createUnactivatedUI();

    /**
     * @brief 创建试用状态界面
     */
    void createTrialUI();

    /**
     * @brief 检查当前软件状态
     */
    void checkSoftwareStatus();

    /**
     * @brief 更新界面状态
     */
    void updateUI();

    // UI组件 - 公共
    QLabel *titleLabel;
    QLabel *statusLabel;
    QLabel *requestCodeLabel;
    QLineEdit *requestCodeEdit;
    QPushButton *copyButton;
    QPushButton *generateButton;
    QLabel *activationCodeLabel;
    QLineEdit *activationCodeEdit;
    QPushButton *activateButton;
    QPushButton *exitButton;
    QPushButton *okButton;
    QTextBrowser *infoText;

    // 布局
    QVBoxLayout *mainLayout;
    QGridLayout *formLayout;
    QHBoxLayout *buttonLayout;

    // 数据
    ActivationManager *activationManager;
    bool isActivated;
    int trialCount;
    QString requestCode;
};

#endif // ACTIVATIONDIALOG_H
