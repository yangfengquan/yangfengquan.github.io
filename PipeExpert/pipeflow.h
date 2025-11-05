#ifndef PIPEFLOW_H
#define PIPEFLOW_H

#include <QWidget>
#include <QPair>
#include <QList>
class QWidget;
class QComboBox;
class QLineEdit;
class QTableWidget;
class QPushButton;
class QString;
class MaterialManager;

class PipeFlow : public QWidget
{
public:
    PipeFlow(QWidget *parent = nullptr);
    ~PipeFlow();
    void setupUi(QWidget *parent);
    void open(QVariantMap data);
    QVariantMap save();
    QString run();

private:
    QComboBox *fluidCombo;
    QComboBox *flowRateCombo;
    QLineEdit *flowRateEdit;
    QLineEdit *inletPressureEdit;
    QComboBox *inletArg2Combo;
    QLineEdit *inletArg2Edit;
    QComboBox *pipeTypeCombo;
    QLineEdit *lengthEdit;
    QLineEdit *pipeOdEdit;
    QLineEdit *pipeWallThicknessEdit;
    QLineEdit *segmentLengthEdit;
    QTableWidget *fittingsTable;
    QComboBox *insulationMaterialCombo;
    QLineEdit *insulationThicknessEdit;
    QComboBox *cladMaterialCombo;
    QLineEdit *ambientTempEdit;
    QLineEdit *windSpeedEdit;

    QList<QPair<QString, int>> fittingsData;

    QPushButton *addFittingButton;
    QPushButton *removeFittingButton;
    MaterialManager *materialManager;

    void loadFittingsToTable();
    void loadFluidsToCombobox();
    bool validateInputs(QString& errorMessage);
    bool validateNumber(const QString& value, const QString& fieldName, double min, double max, bool required, QString& errorMessage);
    bool validateFittingsData(QString& errorMessage);

    QMap<QString, QString> fluidMap = {
        // 基础气体
        {"Air", "空气"},
        {"Nitrogen", "氮气"},
        {"Oxygen", "氧气"},
        {"Hydrogen", "氢气"},
        {"Helium", "氦气"},
        {"Argon", "氩气"},
        {"Neon", "氖气"},
        {"Krypton", "氪气"},
        {"Xenon", "氙气"},
        {"CarbonMonoxide", "一氧化碳"},
        {"CarbonDioxide", "二氧化碳"},
        {"HydrogenSulfide", "硫化氢"},
        {"SulfurDioxide", "二氧化硫"},
        {"Ammonia", "氨"},
        {"NitrousOxide", "一氧化二氮"},

        // 水和水蒸气
        {"Water", "水"},
        {"HeavyWater", "重水"},
        {"D2O", "重水(D2O)"},

        // 碳氢化合物 - 烷烃
        {"Methane", "甲烷"},
        {"Ethane", "乙烷"},
        {"Propane", "丙烷"},
        {"n-Butane", "正丁烷"},
        {"IsoButane", "异丁烷"},
        {"n-Pentane", "正戊烷"},
        {"Isopentane", "异戊烷"},
        {"n-Hexane", "正己烷"},
        {"n-Heptane", "正庚烷"},
        {"n-Octane", "正辛烷"},
        {"n-Nonane", "正壬烷"},
        {"n-Decane", "正癸烷"},
        {"Cyclopentane", "环戊烷"},
        {"Cyclohexane", "环己烷"},
        {"Methylcyclohexane", "甲基环己烷"},

        // 碳氢化合物 - 烯烃
        {"Ethylene", "乙烯"},
        {"Propylene", "丙烯"},
        {"Isobutene", "异丁烯"},
        {"1-Butene", "1-丁烯"},
        {"cis-2-Butene", "顺-2-丁烯"},
        {"trans-2-Butene", "反-2-丁烯"},

        // 碳氢化合物 - 其他
        {"Acetylene", "乙炔"},
        {"Benzene", "苯"},
        {"Toluene", "甲苯"},
        {"m-Xylene", "间二甲苯"},
        {"o-Xylene", "邻二甲苯"},
        {"p-Xylene", "对二甲苯"},
        {"Styrene", "苯乙烯"},
        {"Methanol", "甲醇"},
        {"Ethanol", "乙醇"},
        {"n-Propanol", "正丙醇"},
        {"Isopropanol", "异丙醇"},
        {"n-Butanol", "正丁醇"},
        {"Isobutanol", "异丁醇"},

        // 制冷剂 - CFCs
        {"R11", "R11制冷剂"},
        {"R12", "R12制冷剂"},
        {"R13", "R13制冷剂"},
        {"R113", "R113制冷剂"},
        {"R114", "R114制冷剂"},
        {"R115", "R115制冷剂"},

        // 制冷剂 - HCFCs
        {"R22", "R22制冷剂"},
        {"R123", "R123制冷剂"},
        {"R124", "R124制冷剂"},
        {"R141b", "R141b制冷剂"},
        {"R142b", "R142b制冷剂"},

        // 制冷剂 - HFCs
        {"R32", "R32制冷剂"},
        {"R125", "R125制冷剂"},
        {"R134a", "R134a制冷剂"},
        {"R143a", "R143a制冷剂"},
        {"R152a", "R152a制冷剂"},
        {"R227ea", "R227ea制冷剂"},
        {"R236fa", "R236fa制冷剂"},
        {"R245fa", "R245fa制冷剂"},

        // 制冷剂 - 混合物
        {"R404A", "R404A制冷剂"},
        {"R407C", "R407C制冷剂"},
        {"R410A", "R410A制冷剂"},
        {"R507A", "R507A制冷剂"},
        {"R508B", "R508B制冷剂"},

        // 制冷剂 - HFOs
        {"R1234yf", "R1234yf制冷剂"},
        {"R1234ze", "R1234ze制冷剂"},

        // 制冷剂 - 天然制冷剂
        {"R290", "R290制冷剂(丙烷)"},
        {"R600", "R600制冷剂(丁烷)"},
        {"R600a", "R600a制冷剂(异丁烷)"},
        {"R717", "R717制冷剂(氨)"},
        {"R744", "R744制冷剂(二氧化碳)"},
        {"R718", "R718制冷剂(水)"},

        // 硅油和润滑剂
        {"MD2M", "八甲基三硅氧烷"},
        {"MD3M", "十甲基四硅氧烷"},
        {"MD4M", "十二甲基五硅氧烷"},
        {"MM", "六甲基二硅氧烷"},
        {"D4", "八甲基环四硅氧烷"},
        {"D5", "十甲基环五硅氧烷"},
        {"D6", "十二甲基环六硅氧烷"},

        // 离子液体
        {"[EMIM][Tf2N]", "[EMIM][Tf2N]离子液体"},
        {"[HMIM][Tf2N]", "[HMIM][Tf2N]离子液体"},
        {"[OMIM][Tf2N]", "[OMIM][Tf2N]离子液体"},

        // 生物燃料和替代燃料
        {"Biodiesel", "生物柴油"},
        {"DiethylEther", "乙醚"},
        {"MTBE", "甲基叔丁基醚"},
        {"DimethylEther", "二甲醚"},

        // 特殊流体
        {"Deuterium", "氘"},
        {"Tritium", "氚"},
        {"ParaHydrogen", "仲氢"},
        {"NormalHydrogen", "正氢"},
        {"OrthoHydrogen", "正氢"},
        {"RP1", "RP-1火箭燃料"},
        {"SES36", "SES36"},
        {"FluorinertFC72", "Fluorinert FC72"},
        {"FluorinertFC77", "Fluorinert FC77"},
        {"FluorinertFC84", "Fluorinert FC84"},

        // 盐水溶液
        {"NaCl", "氯化钠溶液"},
        {"CaCl2", "氯化钙溶液"},
        {"KCl", "氯化钾溶液"},
        {"LiBr", "溴化锂溶液"},

        // 其他有机化合物
        {"Acetone", "丙酮"},
        {"DiethylEther", "乙醚"},
        {"DimethylCarbonate", "碳酸二甲酯"},
        {"EthylAcetate", "乙酸乙酯"},
        {"MethylAcetate", "乙酸甲酯"},
        {"MethylEthylKetone", "甲乙酮"},
        {"Tetrahydrofuran", "四氢呋喃"},
        {"AceticAcid", "乙酸"},
        {"FormicAcid", "甲酸"},
        {"Aniline", "苯胺"},
        {"Chloroform", "氯仿"},
        {"Dichloromethane", "二氯甲烷"},
        {"Tetrachloroethylene", "四氯乙烯"},
        {"Trichloroethylene", "三氯乙烯"},
        {"Perfluorohexane", "全氟己烷"},
        {"Perfluoropentane", "全氟戊烷"},

        // 地质流体
        {"Brine", "盐水"},

        // 润滑油
        {"POE", "聚酯润滑油"},
        {"PAG", "聚亚烷基二醇润滑油"},

        // 其他
        {"Solkane365mfc", "Solkane365mfc"},
        {"R365mfc", "R365mfc制冷剂"},
        {"Solkane365mfc", "Solkane365mfc"},
        {"SolsticeL40", "Solstice L40"},
        {"HFE7100", "HFE-7100"},
        {"HFE7200", "HFE-7200"},
        {"Novec649", "Novec 649"},
        {"Novec1230", "Novec 1230"},
        {"CarbonylSulfide", "羰基硫"},
        {"CycloPropane", "环丙烷"},
        {"Dichloroethane", "二氯乙烷"},
        {"EthylBenzene", "乙苯"},
        {"EthyleneOxide", "环氧乙烷"},
        {"Fluorine", "氟"},
        {"HFE143m", "HFE143m"},
        {"HydrogenChloride", "氯化氢"},
        {"IsoButane", "异丁烷"},
        {"IsoButene", "异丁烯"},
        {"MDM", "八甲基三硅氧烷(MDM)"},
        {"MethylLinoleate", "亚油酸甲酯"},
        {"MethylLinolenate", "亚麻酸甲酯"},
        {"MethylOleate", "油酸甲酯"},
        {"MethylPalmitate", "棕榈酸甲酯"},
        {"MethylStearate", "硬脂酸甲酯"},
        {"n-Dodecane", "正十二烷"},
        {"n-Propane", "正丙烷"},
        {"n-Undecane", "正十一烷"},
        {"Neopentane", "新戊烷"},
        {"Novec649", "Novec 649"},
        {"ParaDeuterium", "仲氘"},
        {"Propyne", "丙炔"},
        {"SulfurHexafluoride", "六氟化硫"},
        {"CycloHexane", "环己烷"},
        {"Isohexane", "异己烷"},
        {"OrthoDeuterium", "正氘"},
        {"R116", "R116制冷剂"},
        {"R1233zd(E)", "R1233zd(E)制冷剂"},
        {"R1234ze(E)", "R1234ze(E)制冷剂"},
        {"R1234ze(Z)", "R1234ze(Z)制冷剂"},
        {"R1243zf", "R1243zf制冷剂"},
        {"R1336mzz(E)", "R1336mzz(E)制冷剂"},
        {"R13I1", "R13I1制冷剂"},
        {"R14", "R14制冷剂"},
        {"R152A", "R152A制冷剂"},
        {"R161", "R161制冷剂"},
        {"R21", "R21制冷剂"},
        {"R218", "R218制冷剂"},
        {"R227EA", "R227EA制冷剂"},
        {"R23", "R23制冷剂"},
        {"R236EA", "R236EA制冷剂"},
        {"R236FA", "R236FA制冷剂"},
        {"R245ca", "R245ca制冷剂"},
        {"R440", "R440制冷剂"},
        {"R41", "R41制冷剂"},
        {"RC318", "RC318制冷剂"},
        {"R365MFC", "R365MFC制冷剂"},
        {"R40", "R40制冷剂（氯甲烷）"}
    };



public slots:
    void refreshMaterialCombos();

private slots:

    void addFitting();
    void removeFitting();
};

#endif // PIPEFLOW_H
