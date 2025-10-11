#include "ActivationDialog.h"
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>
#include <QFont>
#include <QPalette>
#include <QStyle>
#include <QDebug>

ActivationDialog::ActivationDialog(QWidget *parent, ActivationManager *activationManager)
    : QDialog(parent)
    , activationManager(activationManager)
    , isActivated(false)
    , trialCount(0)
{
    setWindowTitle("软件激活");
    setFixedSize(500, 400);
    setModal(true);

    // 检查软件状态
    checkSoftwareStatus();
    // 设置界面
    setupUI();
}

ActivationDialog::~ActivationDialog()
{
}

void ActivationDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 根据状态创建不同的界面
    if (isActivated) {
        createActivatedUI();
    } else if (trialCount >= 5) {
        createUnactivatedUI();
    } else {
        createTrialUI();
    }
}

void ActivationDialog::createActivatedUI()
{
    // 标题
    titleLabel = new QLabel("软件激活状态");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2E8B57;");

    // 状态信息
    statusLabel = new QLabel("✅ 软件已激活");
    statusLabel->setAlignment(Qt::AlignCenter);
    QFont statusFont = statusLabel->font();
    statusFont.setPointSize(14);
    statusLabel->setFont(statusFont);

    // 信息文本
    infoText = new QTextBrowser();
    infoText->setReadOnly(true);
    infoText->setHtml(
        "<p><b>激活状态：</b>已激活</p>"
        "<p><b>激活类型：</b>永久授权</p>"
        "<p><b>软件版本：</b>v1.0</p>"
        "<p><b>技术支持：</b>QQ群 816103114</p>"
        "<br>"
        "<p>感谢您使用管道分析软件！</p>"
        "<p>软件已成功激活，您可以无限制使用所有功能。</p>"
        );
    infoText->setFixedHeight(180);

    // 确定按钮
    okButton = new QPushButton("确定");
    okButton->setFixedSize(100, 35);

    // 布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(infoText);
    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
}

void ActivationDialog::createUnactivatedUI()
{
    // 生成申请码
    requestCode = activationManager->generateRequestCode();

    // 标题
    titleLabel = new QLabel("软件激活");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // 状态信息
    statusLabel = new QLabel("❌ 试用次数已用完，请输入激活码");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: #FF4500; font-weight: bold;");

    // 申请码区域
    QGroupBox *requestGroup = new QGroupBox("申请码");
    QVBoxLayout *requestLayout = new QVBoxLayout(requestGroup);

    requestCodeLabel = new QLabel("您的申请码:");
    requestCodeEdit = new QLineEdit(requestCode);
    requestCodeEdit->setReadOnly(true);
    requestCodeEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");

    QHBoxLayout *copyLayout = new QHBoxLayout();
    copyButton = new QPushButton("复制申请码");
    generateButton = new QPushButton("重新生成");
    copyLayout->addWidget(copyButton);
    copyLayout->addWidget(generateButton);
    copyLayout->addStretch();

    requestLayout->addWidget(requestCodeLabel);
    requestLayout->addWidget(requestCodeEdit);
    requestLayout->addLayout(copyLayout);

    // 激活码区域
    QGroupBox *activationGroup = new QGroupBox("激活码");
    QVBoxLayout *activationLayout = new QVBoxLayout(activationGroup);

    activationCodeLabel = new QLabel("请输入激活码:");
    activationCodeEdit = new QLineEdit();
    activationCodeEdit->setPlaceholderText("请输入从QQ群获取的激活码...");

    activationLayout->addWidget(activationCodeLabel);
    activationLayout->addWidget(activationCodeEdit);

    // 按钮区域
    buttonLayout = new QHBoxLayout();
    activateButton = new QPushButton("激活软件");
    exitButton = new QPushButton("退出");

    activateButton->setFixedSize(100, 35);
    exitButton->setFixedSize(100, 35);

    buttonLayout->addStretch();
    buttonLayout->addWidget(activateButton);
    buttonLayout->addWidget(exitButton);

    // 帮助信息
    infoText = new QTextBrowser(this);
    infoText->setReadOnly(true);
    infoText->setHtml(
        "<p><b>激活说明：</b></p>"
        "<p>1. 复制上方的申请码</p>"
        "<p>2. 添加QQ群：<a href=\"https://jq.qq.com/?_wv=1027&k=5RWGf6a\">816103114</a></p>"
        "<p>3. 将申请码发送给群主获取激活码</p>"
        "<p>4. 在下方输入激活码完成激活</p>"
        "<br>"
        "<p><b>注意：</b>每个申请码只能使用一次，重新生成申请码后之前的激活码将失效。</p>"
        );
    infoText->setFixedHeight(150);
    infoText->setOpenExternalLinks(true);

    // 布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(requestGroup);
    mainLayout->addWidget(activationGroup);
    mainLayout->addWidget(infoText);
    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(copyButton, &QPushButton::clicked, this, &ActivationDialog::copyRequestCode);
    connect(generateButton, &QPushButton::clicked, this, &ActivationDialog::generateNewRequestCode);
    connect(activateButton, &QPushButton::clicked, this, &ActivationDialog::validateActivation);
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}

void ActivationDialog::createTrialUI()
{
    // 生成申请码
    requestCode = activationManager->generateRequestCode();

    // 标题
    titleLabel = new QLabel("软件试用");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // 状态信息
    int remainingTrials = 5 - trialCount;
    statusLabel = new QLabel(QString("您正在试用本软件，还剩 %1 次试用机会").arg(remainingTrials));
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: #FF8C00; font-weight: bold;");

    // 申请码区域
    QGroupBox *requestGroup = new QGroupBox("申请码（如需激活请复制此码）");
    QVBoxLayout *requestLayout = new QVBoxLayout(requestGroup);

    requestCodeLabel = new QLabel("您的申请码:");
    requestCodeEdit = new QLineEdit(requestCode);
    requestCodeEdit->setReadOnly(true);
    requestCodeEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");

    QHBoxLayout *copyLayout = new QHBoxLayout();
    copyButton = new QPushButton("复制申请码");
    generateButton = new QPushButton("重新生成");
    copyLayout->addWidget(copyButton);
    copyLayout->addWidget(generateButton);
    copyLayout->addStretch();

    requestLayout->addWidget(requestCodeLabel);
    requestLayout->addWidget(requestCodeEdit);
    requestLayout->addLayout(copyLayout);

    // 激活码区域
    QGroupBox *activationGroup = new QGroupBox("激活码（可选）");
    QVBoxLayout *activationLayout = new QVBoxLayout(activationGroup);

    activationCodeLabel = new QLabel("请输入激活码:");
    activationCodeEdit = new QLineEdit();
    activationCodeEdit->setPlaceholderText("如需激活请输入激活码...");

    activationLayout->addWidget(activationCodeLabel);
    activationLayout->addWidget(activationCodeEdit);

    // 按钮区域
    buttonLayout = new QHBoxLayout();
    activateButton = new QPushButton("激活软件");
    okButton = new QPushButton("继续试用");

    activateButton->setFixedSize(100, 35);
    okButton->setFixedSize(100, 35);

    buttonLayout->addStretch();
    buttonLayout->addWidget(activateButton);
    buttonLayout->addWidget(okButton);

    // 帮助信息
    infoText = new QTextBrowser();
    infoText->setReadOnly(true);
    infoText->setHtml(
        "<p><b>试用说明：</b></p>"
        "<p>• 您有 5 次试用机会，当前已使用 " + QString::number(trialCount) + " 次</p>"
                                        "<p>• 试用期间软件功能完整，无任何限制</p>"
                                        "<p>• 试用结束后需要激活才能继续使用</p>"
                                        "<br>"
                                        "<p><b>激活说明：</b></p>"
                                        "<p>1. 复制上方的申请码</p>"
                                        "<p>2. 添加QQ群：<a href=\"https://jq.qq.com/?_wv=1027&k=5RWGf6a\">816103114</a></p>"
                                        "<p>3. 将申请码发送给群主获取激活码</p>"
                                        "<p>4. 在下方输入激活码完成激活</p>"
        );
    infoText->setFixedHeight(180);
    infoText->setOpenExternalLinks(true);

    // 布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(requestGroup);
    mainLayout->addWidget(activationGroup);
    mainLayout->addWidget(infoText);
    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(copyButton, &QPushButton::clicked, this, &ActivationDialog::copyRequestCode);
    connect(generateButton, &QPushButton::clicked, this, &ActivationDialog::generateNewRequestCode);
    connect(activateButton, &QPushButton::clicked, this, &ActivationDialog::validateActivation);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
}

void ActivationDialog::checkSoftwareStatus()
{
    isActivated = activationManager->isActivated();
    trialCount = activationManager->getTrialCount();

    qDebug() << "软件状态检查 - 已激活:" << isActivated << "试用次数:" << trialCount;
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

void ActivationDialog::generateNewRequestCode()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认",
                                  "重新生成申请码将使之前的激活码失效，确定要继续吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        requestCode = activationManager->generateRequestCode();
        requestCodeEdit->setText(requestCode);
        activationCodeEdit->clear();
        QMessageBox::information(this, "成功", "新的申请码已生成");
    }
}

void ActivationDialog::openQQGroup()
{
    QDesktopServices::openUrl(QUrl("https://jq.qq.com/?_wv=1027&k=5RWGf6a"));
}
