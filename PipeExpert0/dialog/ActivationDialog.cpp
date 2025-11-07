#include "ActivationDialog.h"
#include <QApplication>
#include <QPlainTextEdit>
#include <QClipboard>
//#include <QDesktopServices>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>

ActivationDialog::ActivationDialog(QWidget *parent, ActivationManager *activationManager)
    : QDialog(parent)
    , activationManager(activationManager)
{
    setWindowTitle("软件激活");
    setFixedSize(500, 350);
    setModal(true);

    // 设置界面
    setupUI();
}

ActivationDialog::~ActivationDialog()
{
}

void ActivationDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 生成申请码
    requestCode = activationManager->generateRequestCode();

    // 申请码区域
    QGroupBox *requestGroup = new QGroupBox("申请码");
    QVBoxLayout *requestLayout = new QVBoxLayout(requestGroup);

    requestLayout->addWidget(new QLabel("申请码:"));

    requestCodeEdit = new QLineEdit(requestCode);
    requestCodeEdit->setReadOnly(true);
    requestLayout->addWidget(requestCodeEdit);

    QHBoxLayout *copyLayout = new QHBoxLayout();
    copyButton = new QPushButton("复制申请码");
    copyLayout->addWidget(copyButton);
    copyLayout->addStretch();
    requestLayout->addLayout(copyLayout);

    // 激活码区域
    QGroupBox *activationGroup = new QGroupBox("激活码");
    QVBoxLayout *activationLayout = new QVBoxLayout(activationGroup);

    activationLayout->addWidget(new QLabel("请输入激活码:"));

    activationCodeEdit = new QLineEdit();
    activationLayout->addWidget(activationCodeEdit);

    // 按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    activateButton = new QPushButton("激活软件");
    exitButton = new QPushButton("退出");

    buttonLayout->addStretch();
    buttonLayout->addWidget(activateButton);
    buttonLayout->addWidget(exitButton);

    // 帮助信息
    QPlainTextEdit *infoText = new QPlainTextEdit(this);
    infoText->setReadOnly(true);
    infoText->setPlainText("激活说明\n添加QQ群：816103114\n将申请码发给群主");
    infoText->setFixedHeight(80);

    // 布局
    mainLayout->addWidget(requestGroup);
    mainLayout->addWidget(activationGroup);
    mainLayout->addWidget(infoText);
    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(copyButton, &QPushButton::clicked, this, &ActivationDialog::copyRequestCode);
    connect(activateButton, &QPushButton::clicked, this, &ActivationDialog::validateActivation);
    connect(exitButton, &QPushButton::clicked, this,  &ActivationDialog::close);
}

void ActivationDialog::validateActivation()
{
    QString activationCode = activationCodeEdit->text().trimmed();

    if (activationCode.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入激活码");
        activationCodeEdit->setFocus();
        return;
    }

    // 验证激活码
    if (activationManager->validateActivation(activationCode, requestCode)) {
        QMessageBox::information(this, "成功", "软件激活成功！");
        accept(); // 关闭对话框并返回Accepted
    } else {
        QMessageBox::critical(this, "错误", "激活码无效，请检查后重试");
        activationCodeEdit->selectAll();
        activationCodeEdit->setFocus();
    }
}

void ActivationDialog::copyRequestCode()
{
    QApplication::clipboard()->setText(requestCode);
    QMessageBox::information(this, "成功", "申请码已复制到剪贴板");
}
