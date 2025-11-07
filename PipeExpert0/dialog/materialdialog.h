#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <QDialog>

class QTableWidget;
class QLineEdit;
class QPushButton;
class QGridLayout;
class MaterialManager;

class MaterialDialog : public QDialog
{
    Q_OBJECT

public:
    MaterialDialog(const QString& type, QWidget *parent = nullptr);
    ~MaterialDialog();

signals:
    /**
     * @brief 材料数据改变信号
     */
    void materialDataChanged();

private:
    QString materialType;
    MaterialManager *materialManager;
    QTableWidget *materialTable;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *closeButton;

    /**
     * @brief 初始化界面
     */
    void setupUi();

    /**
     * @brief 创建材料列表表格
     */
    void createMaterialTable();

    /**
     * @brief 获取对话框标题
     * @return 标题字符串
     */
    QString getTitle() const;

    /**
     * @brief 获取表格列定义
     * @return 列标题列表
     */
    QStringList getColumns() const;
    void refreshTable();

private slots:
    /**
     * @brief 添加新材料
     */
    void addMaterial();

    /**
     * @brief 编辑选中的材料
     */
    void editMaterial();

    /**
     * @brief 删除选中的材料
     */
    void deleteMaterial();
};


class MaterialEditDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief 材料编辑对话框构造函数
     * @param parent 父窗口
     * @param materialManager 材料管理器
     * @param materialType 材料类型
     * @param materialName 材料名称（编辑时传入，添加时为空）
     */
    explicit MaterialEditDialog(QWidget *parent,
                                MaterialManager *materialManager,
                                const QString &materialType,
                                const QString &materialName = QString());
    ~MaterialEditDialog();

private slots:
    /**
     * @brief 保存材料
     */
    void saveMaterial();

private:
    /**
     * @brief 初始化界面
     */
    void setupUI();

    /**
     * @brief 创建保温材料编辑字段
     * @param layout 父布局
     */
    void createInsulationFields(QGridLayout *layout);

    /**
     * @brief 创建外保护层编辑字段
     * @param layout 父布局
     */
    void createCladFields(QGridLayout *layout);

    /**
     * @brief 创建管道元件编辑字段
     * @param layout 父布局
     */
    void createFittingsFields(QGridLayout *layout);

    /**
     * @brief 创建管道类型编辑字段
     * @param layout 父布局
     */
    void createPipeTypeFields(QGridLayout *layout);

    /**
     * @brief 加载材料数据到表单
     */
    void loadMaterialData();

    /**
     * @brief 验证输入数据
     * @return 验证通过返回true，否则返回false
     */
    bool validateInput();

    // UI组件 - 公共字段
    QLineEdit *nameEdit;
    QLineEdit *descriptionEdit;

    // UI组件 - 保温材料字段
    QLineEdit *conductivityEq1Edit;
    QLineEdit *conductivityEq2Edit;
    QLineEdit *conductivityEq3Edit;
    QLineEdit *range1Edit;
    QLineEdit *range2Edit;
    QLineEdit *range3Edit;
    QLineEdit *densityEdit;

    // UI组件 - 外保护层字段
    QLineEdit *emissivityEdit;

    // UI组件 - 管道元件字段
    QLineEdit *resistanceEdit;

    // UI组件 - 管道类型字段
    QLineEdit *roughnessEdit;

    // 按钮
    QPushButton *saveButton;
    QPushButton *cancelButton;

    // 布局
    //QVBoxLayout *mainLayout;
    //QGridLayout *fieldsLayout;
    //QHBoxLayout *buttonLayout;

    // 数据
    MaterialManager *materialManager;
    QString materialType;
    QString materialName;
    bool isEditMode;
};
#endif // MATERIALDIALOG_H
