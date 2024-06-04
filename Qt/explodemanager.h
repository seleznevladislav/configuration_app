#ifndef __VSN_EXPLODEGROUP_H
#define __VSN_EXPLODEGROUP_H

#include "vsn_explodedispatcher.h"

#include "explodetree.h"
#include <QObject>
#include <QGroupBox>
#include "BuildParams.h"
#include "QComboBox"
#include "QFormLayout"
#include <QLineEdit>
#include <QDoubleSpinBox>
#include "qtoolbutton.h"
#include "QWidget"
#include <QMessageBox>
#include <QCheckBox>

#include <array>
#include <memory>
#include <vector>

using namespace BuildMathModel;

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QComboBox;
class QCheckBox;
class QRadioButton;
class QGroupBox;
class QLabel;
class ExplodeWidget;

VSN_USE_NAMESPACE

class ExplodeManager : public QObject
{
    Q_OBJECT
private:
    typedef ExplodeDispatcher ED;
public:
    explicit ExplodeManager(ExplodeWidget* pOpenScene);
    virtual ~ExplodeManager();
public:
    void init(SceneSegment* pSegmModel, ExplodeTreeView* pTreeWidget);
    QGroupBox* createGroupExplode(QWidget& widget, const int heightButton, const std::string& mainTabName);
    void createCalculationTab(const int numberOfHeatExchanger);
    void createParametrizationTab(const int numberOfHeatExchanger);
    void iterateHeatExchanger(double hotOutletTemp, double coldOutletTemp);
    QFormLayout* createParametrizationForm(const int numberOfHeatExchanger, QVBoxLayout* layout);
    bool isSelectionEnabled() const;
    bool isCreateGroupGeometry() const;
    bool onSelectItem(const SceneSegment* pSegm);
    QComboBox* m_quantityCombobox = nullptr;
    QComboBox* m_comboConfigure = nullptr;
    QPushButton* m_reconfigureButton = nullptr;
    QVBoxLayout* m_vLayoutWarmParams = nullptr;
    bool isCheckedManualType = false;
    ConfigParams manualTTRMParams;
    BuildParamsForHeatExchangerTTOR manualTTORParams;

    std::vector<ConfigParams> dataTTRM = {
       {u8"ТТРМ 25/57-6,3/1,6", 25, 3, 57, 4, 219, 8, 1500, 750, 2150, 32, 80, 415, 250, 230 },
       {u8"ТТРМ 25/57-6,3/4,0", 25, 3, 57, 4, 219, 8, 3000, 1500, 3680, 32, 80, 875, 280, 230 },
       {u8"ТТРМ 25/57-10,0/6,3", 25, 4, 57, 5, 219, 8, 1500, 750, 2240, 32, 80, 515, 340, 230 },
       {u8"ТТРМ 25/57-16,0/10,0", 25, 4, 57, 5, 219, 8, 3000, 1500, 3770, 32, 80, 890, 360, 240 },

       {u8"ТТРМ 38/57-10,0/1,6", 38, 4, 57, 4, 219, 8, 1500, 750, 2150, 32, 80, 345, 230, 260 },
       {u8"ТТРМ 38/57-10,0/4,0", 38, 4, 57, 4, 219, 8, 3000, 1500, 3680, 32, 80, 805, 260, 260 },
       {u8"ТТРМ 38/76-10,0/1,6", 38, 4, 76, 4, 273, 8, 1500, 750, 2270, 50, 110, 450, 310, 260 },
       {u8"ТТРМ 38/76-10,0/4,0", 38, 4, 76, 4, 273, 8, 3000, 1500, 3820, 50, 110, 865, 360, 260 },

       {u8"ТТРМ 38/76-16,0/10,0", 38, 5, 76, 6, 273, 8, 3000, 1500, 3890, 50, 110, 890, 420, 270 },

       {u8"ТТРМ 38/89-6,3/1,6", 38, 4, 89, 5, 273, 8, 3000, 1500, 3770, 50, 110, 500, 280, 240 },
       {u8"ТТРМ 38/89-6,3/4,0", 38, 4, 89, 5, 273, 8, 4500, 3000, 5320, 50, 110, 855, 340, 240 },

       {u8"ТТРМ 48/76-6,3/1,6", 48, 4, 76, 4, 273, 8, 3000, 1500, 3800, 50, 110, 830, 280, 285 },
       {u8"ТТРМ 48/76-6,3/4,0", 48, 4, 76, 4, 273, 8, 4500, 3000, 5350, 50, 110, 835, 330, 285 },
       {u8"ТТРМ 48/89-6,3/1,6", 48, 4, 89, 5, 273, 8, 3000, 1500, 3840, 50, 110, 830, 310, 285 },
       {u8"ТТРМ 48/89-6,3/4,0", 48, 4, 89, 5, 273, 8, 6000, 3000, 6890, 50, 110, 1525, 360, 285 },

       {u8"ТТРМ 48/89-10,0/6,3", 48, 5, 89, 5, 273, 8, 4500, 3000, 5440, 50, 110, 845, 410, 285 },

       {u8"ТТРМ 48/108-6,3/1,6", 48, 4, 108, 5, 325, 8, 4500, 3000, 5380, 80, 130, 795, 340, 295 },
       {u8"ТТРМ 48/108-6,3/4,0", 48, 4, 108, 5, 325, 8, 6000, 3000, 6960, 80, 130, 1530, 420, 295 },

       {u8"ТТРМ 57/89-10,0/1,6", 57, 5, 89, 5, 325, 8, 4500, 3000, 5420, 50, 130, 805, 320, 315 },
       {u8"ТТРМ 57/89-10,0/4,0", 57, 5, 89, 5, 325, 8, 6000, 3000, 7000, 50, 130, 1570, 400, 315 },
       {u8"ТТРМ 57/108-10,0/1,6", 57, 5, 108, 5, 325, 8, 4500, 3000, 5440, 80, 130, 815, 340, 315 },
       {u8"ТТРМ 57/108-10,0/4,0", 57, 5, 108, 5, 325, 8, 4500, 3000, 5520, 80, 130, 860, 420, 315 },
       {u8"ТТРМ 57/108-10,0/6,3", 57, 5, 108, 6, 325, 8, 6000, 3000, 7040, 80, 130, 1595, 440, 315 },

    };
    std::vector<BuildParamsForHeatExchangerTTOR> dataTTOR = {
       //            1             2   3  4    5   6     7     8    9   10    11    12    13  14    15   16   17    18     19     20
       {u8"ТТОР 89/133-1,6/1,6",  89,  5, 133, 5, 4500, 5410, 600, 80,  860,  425, 3000, 500, 730,  400, 255, 4825, false, false, 750},
       {u8"ТТОР 89/133-4,0/1,6",  89,  5, 133, 5, 4500, 5420, 600, 80,  860,  425, 3000, 550, 730,  400, 255, 4835, false, false, 750},
       {u8"ТТОР 89/133-4,0/4,0",  89,  5, 133, 6, 4500, 5420, 600, 80,  860,  425, 3000, 560, 730,  400, 255, 4835, false, false, 750},
       {u8"ТТОР 89/159-1,6/1,6",  89,  5, 159, 5, 4500, 5470, 600,  100,  860,  425, 3000, 560, 730,  400, 255, 4885, false, false, 750},
       {u8"ТТОР 89/159-4,0/1,6",  89,  5, 159, 5, 4500, 5480, 600,  100,  860,  425, 3000, 620, 730,  400, 255, 4895, false, false, 750},
       {u8"ТТОР 89/159-4,0/4,0",  89,  5, 159, 6, 4500, 5480, 600,  100,  860,  425, 3000, 620, 730,  400, 255, 4895, false, false, 750},
       {u8"ТТОР 108/159-1,6/1,6", 108,  5, 159, 5, 4500, 5480, 600, 100,  860,  425, 3000, 600, 730,  400, 255, 4895, false, false, 750},
       {u8"ТТОР 108/159-4,0/1,6", 108,  5, 159, 5, 4500, 5515, 600, 100,  860,  425, 3000, 640, 730,  400, 255, 4930, false, false, 750},
       {u8"ТТОР 108/159-4,0/4,0", 108,  5, 159, 6, 4500, 5515, 600, 100,  860,  425, 3000, 660, 730,  400, 255, 4930, false, false, 750},
       /////////////////////////////////////////////////////////////////////////////////////////////////////, //, false, false/ , 750
       {u8"ТТОР 133/219-1,6/1,6", 133, 5, 219, 7, 6000, 7250, 800, 125, 1080, 535, 3000, 680, 1550, 550, 320, 6420, false, false, 976},
       {u8"ТТОР 133/219-4,0/1,6", 133, 6, 219, 7, 6000, 7260, 800, 125, 1080, 535, 3000, 740, 1490, 550, 320, 6430, false, false, 976},
       {u8"ТТОР 133/219-4,0/4,0", 133, 6, 219, 7, 6000, 7260, 800, 125, 1080, 535, 3000, 760, 1475, 550, 320, 6430, false, false, 976},
       {u8"ТТОР 159/219-1,6/1,6", 159, 5, 219, 7, 6000, 7250, 800, 150, 1080, 535, 3000, 690, 1550, 550, 320, 6420, false, false, 976},
       {u8"ТТОР 159/219-4,0/1,6", 159, 6, 219, 7, 6000, 7280, 800, 150, 1080, 535, 3000, 760, 1490, 550, 320, 6450, false, false, 976},
       {u8"ТТОР 159/219-4,0/4,0", 159, 6, 219, 7, 6000, 7280, 800, 150, 1080, 535, 3000, 780, 1475, 550, 320, 6450, false, false, 976},
    };
    std::vector<ConfigParams_IP> dataIP = {
       {u8"800ИП-1,6-2,5-М1/25-6-2 ", 840, 7120, 20, 1.6, 500},
       {u8"800ИП-2,5-4,0-М1/25-6-2 ", 840, 7120, 20, 2.5, 500},
       {u8"1000ИП-1,6-2,5-М1/25-6-2", 1040, 7205, 20, 1.6, 600},
       {u8"1200ИП-1,6-2,5-М1/25-6-2", 1240, 7335, 20, 1.6, 700},
       {u8"1400ИП-1,6-2,5-М1/25-6-2", 1440, 7690, 20, 1.6, 800},
       {u8"1600ИП-1,6-2,5-М1/25-6-2", 1640, 7735, 20, 1.6, 900},
       {u8"1800ИП-1,6-2,5-М1/25-6-2", 1840, 7950, 20, 1.6, 1000},
       {u8"1800ИП-2,5-4,0-М1/25-6-2", 1840, 7950, 20, 1.6, 1100},
       {u8"2000ИП-1,6-2,5-М1/25-6-2", 2040, 8210, 20, 1.6, 1200},
    };
    std::vector<ConfigParams_IU> dataIU = {
        {u8"800ИУ-2,5-2,5-М4/20-6-2-Т", 840, 7120, 20, 1.6, 800, 540, 500, 562, 1.6},
        {u8"1000ИУ-2,5-2,5-М4/20-6-2-Т", 1040, 7205, 20, 1.6, 1000, 640, 600, 562, 1.6},
        {u8"1200ИУ-2,5-2,5-М4/20-6-2-Т", 1240, 7335., 20, 1.6, 1200, 740, 700, 562, 1.6},
        {u8"1400ИУ-2,5-2,5-М4/20-6-2-Т", 1440, 7690., 20, 1.6, 1400, 840, 800, 562, 1.6},
        {u8"1600ИУ-2,5-2,5-М4/20-6-2-Т", 1640, 7735, 20, 1.6, 1600, 940, 900, 562, 1.6},
        {u8"1800У-2,5-2,5-М4/20-6-2-Т", 1840, 7950, 20, 1.6, 1800, 1040, 1000, 562, 1.6},
        {u8"1800У-2,5-2,5-М4/20-6-2-Т", 1840, 7950, 20, 1.6, 1800, 1140, 1100, 562, 1.6},
        {u8"2000У-2,5-2,5-М4/20-6-2-Т", 2040, 8210, 20, 1.6, 2000, 1240, 1200, 562, 1.6},
    };

    //  std::string name;     // 0 - Наименование
    //  double p;             // 1 - Плотность
    //  double c;		      // 2 - Коэффициент теплопроводности (кДЖ/ кг*C)
    //  double laymbda;	      // 3 - Коэффициент линейного расширения вещества (Вт/ м*C)
    //  double u_viscocity;	  // 4 - Коэффициент динамический взякости (Есть не у всех, если что высчитывается, если 0 ) (Па/с)
    //  double viscocity;	  // 5 - Коэффициент кинематической взякости (Есть не у всех, если что высчитывается, если 0 ) (Па/с)
    //  double Pr;	          // 6 - Коэффициент кинематической взякости (Есть не у всех, если что высчитывается, если 0 ) (Па/с)

    std::vector<data_fluidProperties> fluidsProperties = {
        {u8"Ацетон",                813 , 2.114, 0.174, 0.395, 0.455, 2100, 4.3},
        {u8"Бензин",                900 , 1.800, 0.145, 0, 0.88, 1400, 4.4},
        {u8"Вода дистиллированная", 1000, 4.187, 0.550, 0, 0, 4200, 7},
        {u8"Керосин",               850 , 2.430, 0.121, 0, 0, 1880, 10},
        {u8"Нефтяное масло 5w40",   890 , 2.430, 0.104, 0, 0, 2100, 7 },
        {u8"Ртуть чистая",         13600, 0.138, 8.140, 0, 0.1, 130, 0.025},
        {u8"Спирт метиловый",       810,  2.470, 0.214, 0.817, 0.5, 2500, 6.7 },
        {u8"Спирт этиловый",        806,  2.303, 0.188, 1.780, 0.25, 2400, 16.2 },
    };

    struct data_materialProperties {
        std::string name;				// 0 - Наименование
        double tMaterial;               // 1 - При какой температуре коэффициентр теплопроводности
        double laymbdaMateral;		    // 2 - Коэффициент теплопроводности (Вт/ m*c)
    };

    std::vector<data_materialProperties> materialProperties = {
        {u8"Сталь углеродистая 20",         200, 48.5},
        {u8"Сталь углеродистая 40",         200, 48.1},
        {u8"Алюминий",                      200, 229},
        {u8"Алюминий сплав 92% Al, 8% Mg",  200, 148},
        {u8"Медь (99,9%)",                  200, 378},
        {u8"Железо (99,9%)",                200, 61.6},
        {u8"Цинк (99,993%)",                200, 104.7},
        {u8"Вольфрам",                      200, 140.7},
        {u8"Золото (99,999%)",              200, 308.2},
    };

    std::vector<std::vector<double>> fluidDensity = {
        { 884.5, 877.2, 870.1, 863.2, 856, 849.1, 841.4, 834.3, 827, 820, 813, 801.9, 790.5, 778.5, 767.4, 756.4, 744.6, 732.6, 720.5, 703.5, 690.4 },
        { 851.25, 842.25, 833.25, 824.25, 815.25, 806.25, 797.25, 788.25, 779.25, 770.25, 761.25, 752.25, 743.25, 734.25, 725.25, 716.25, 707.25, 698.25, 689.25, 680.25, 671.25 },
        { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 999.8, 999.7, 998.2, 995.7, 992.2, 988.0, 983.2, 977.8, 971.8, 965.3, 958.4 },
        { 1010.0, 1005.0, 1000.0, 995.0, 990.0, 985.0, 980.0, 975.0, 970.0, 965.0, 960.0, 950.0, 940.0,930.0, 920.0, 910.0, 900.0, 890.0, 880.0, 870.0, 860.0 },
        { 850.0, 855.0, 860.0, 865.0, 870.0, 875.0, 880.0, 885.0, 890.0, 895.0, 900.0, 905.0, 910.0, 915.0, 920.0, 925.0, 930.0, 935.0, 940.0, 945.0, 950.0 },
        { 13595.1, 13678.0, 13757.0, 13830.0, 13898.0, 13959.0, 14015.0, 14064.0, 14107.0, 14143.0, 14171.0, 14192.0, 14204.0, 14208.0, 14204.0, 14192.0, 14171.0, 14143.0, 14107.0, 14064.0, 14015.0 },
        { 791.0, 791.0, 790.9, 790.5, 789.2, 787.1, 784.3, 780.8, 776.5, 771.5, 765.5, 758.8, 751.4, 743.2, 734.3, 724.7, 714.4, 703.3, 691.4, 678.7, 665.3 },
        { 826.4, 826, 825, 824, 823, 822, 821, 817, 815, 806, 797, 789, 780, 772, 763, 754, 744, 735, 726, 716, 693 },
    };

    std::vector<double> waterViscocity = { 1.787, 1.519, 1.307, 1.004, 0.801, 0.658, 0.475, 0.413, 0.365, 0.326, 0.294 };
    std::vector<double> cerosinViscocity = { 5.9, 4.2, 3.6, 3, 2.4, 1.2, 1.1, 1, 0.9, 0.7, 0.6 };
    std::vector<double> oilViscocity = { 100, 95, 90, 85, 80, 75, 70, 55, 40, 25, 12 };
public:
    QGroupBox* getExplodingGroupBox() const { return gr_WExploding; }
    QGroupBox* getFiltersGroupBox() const { return gr_Wfilters; }
    QGroupBox* getSelectionsGroupBox() const { return gr_WSelections; }
    QGroupBox* getRenderingGroupBox() const { return gr_WRendering; }
    QGroupBox* getPlaneCuttingGroupBox() const { return gr_WCutting; }
    QGroupBox* getPlaneDimensionsGroupBox() const { return gr_WDimensionBox; }

    QDoubleSpinBox* getM_Poffset() const { return m_Poffset; }
    QDoubleSpinBox* getM_Pa1() const { return m_Pa1; }
    QDoubleSpinBox* getM_Pa2() const { return m_Pa2; }
    QRadioButton* getM_PfreeSection() const { return m_PfreeSection; }
    QRadioButton* getM_PoffsetSection() const { return m_PoffsetSection; }
private:
    void initWidgets();
    void applyParameter(const ExplodeDispatcher::ControlParameterType paramType, const Variant& data, std::array<bool, 3> params);
    void update(const bool doUpdateWidgets, const bool doUpdateTree, const bool doUpdateScene);
    void updateWidgetState(QWidget* wdg, const ExplodeParameterState& paramState);
    void updateWidgetValues(QWidget* wdg, const ExplodeParameterValues& paramValues);
    void updateLabelText(QLabel* label, const ExplodeDispatcher::ControlParameterType& param);
    void saveCameraPosition();
    void restoreCameraPosition();
private:
    void tabWidgetCurrentChanged(int idx);
    void tabWidgetCloseRequested(int idx);
    void groupExplodeToggled(bool check);
    void buttonCreatePressed();
    void buttonsPressed();
    void buttonZoomToFit();
    void slidersExplodeValueChanged(const int value);
    void radiosExplodeFromToggled(bool checked);
    void radiosTypeFromToggled(bool checked, int type);
    ExplodeDispatcher::ControlParameterType GetParameterType(const QObject* widg) const;
private:
    QGroupBox* createGroupBox(const char* title, const bool flat, const bool checkable, const bool checked);
    QVBoxLayout* createVBoxLayout(QGroupBox* group);
    QPushButton* createButton(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const char* tip);
    QComboBox* createCombobox(QVBoxLayout* vLayout);
    std::pair<QSlider*, QLabel*> createSliderWithLabel(const ExplodeDispatcher::ControlParameterType param, const char* labelName, QVBoxLayout* vLayout, const char* tip);
    QCheckBox* createCheckBox(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const bool checked, const char* tip);
    QRadioButton* createRadioButton(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const bool checked);
    QRadioButton* createTypeRadioButton(QHBoxLayout* hLayout, const char* text, const bool checked, int type);
    QTabWidget* createTabWidget(QWidget& widget, const int heightButton, const std::string& mainTabName);
    ConfigParams findClosestMatch(int dimCamera, int param, const std::string& structName);
    void calculateThickness(QFormLayout* form);
    void onReconfigureButtonClicked();
    void onDrawingShowButtonClicked();
    void onCalculationButtonClicked();
    QAction* createActionButton(const QString& fileName, QGroupBox* groupFilter, QHBoxLayout* fGroupLayout);
    QGroupBox* createExplodingGroupBox();
    QGroupBox* createFilterGroupBox();
    QGroupBox* createSelectionGroupBox();
    QGroupBox* createRenderingGroupBox();
    QGroupBox* createCuttingGroupBox();
    QGroupBox* createDimensionsGroupBox();
    QWidget* createPairWidget(QWidget* widget1, QWidget* widget2);
    QMessageBox* ExplodeManager::createWarning(QString* warningText);
private:
    ExplodeDispatcher m_explodeDispatcher;
    std::map<ExplodeDispatcher::ControlParameterType, QWidget*> m_widgetsMap;
    std::vector<ExplodeDispatcher::ControlParameterType> m_parametersForUpdateGUI;
    ExplodeWidget* m_pExplodeWidget      = nullptr;
    ExplodeTreeView* m_pTreeWidget       = nullptr;
    QWidget* m_pWidget                   = nullptr;
    SceneSegment* m_pSegmModel           = nullptr;
    QTabWidget* m_tabWidget              = nullptr;
    QVBoxLayout* m_vLayoutTab            = nullptr;
    QVBoxLayout* m_vLayoutConfigureTab   = nullptr;
    QGroupBox* m_groupExpl               = nullptr;
    QGroupBox* m_warmParams              = nullptr;
    QLabel* m_labelLevel                 = nullptr;
    QLabel* m_labelSelectAssembly        = nullptr;
    QDoubleSpinBox* m_lengthSpinBox      = nullptr;
    QToolButton* m_closestLengthButton   = nullptr;
    // <summary>
    /// vars for calculations TTOR
    /// </summary>
    data_tubeProperties dataOfCurrentTubeTepl;
    data_tubeProperties dataOfCurrentTubeKozhux;
    QComboBox* m_PhotFluidComboBox;
    QComboBox* m_PcoldFluidComboBox;
    QDoubleSpinBox* m_PhotVelocity;
    QDoubleSpinBox* m_PcoldVelocity;
    QDoubleSpinBox* m_PhotInletTemp;
    QDoubleSpinBox* m_PcoldInletTemp;
    QDoubleSpinBox* m_PrresultTemp1;
    QDoubleSpinBox* m_PrresultTemp2;
    QComboBox* m_PmaterialCombobox;
    double m_PoutletTemp1;
    double m_PoutletTemp2;
    QDialog* m_PmodalDialog;
    /// <summary>
    /// vars for parametrization Zarubin
    /// </summary>
    QCheckBox* turnOnZagryaznenieTTOR = nullptr;
    QDoubleSpinBox* m_PresultFieldZagryaznenieQ1 = nullptr;
    QDoubleSpinBox* m_PresultFieldZagryaznenieQ2 = nullptr;
    QDoubleSpinBox* m_PresultFieldZagryaznenieH1 = nullptr;
    QDoubleSpinBox* m_PresultFieldZagryaznenieH2 = nullptr;

    QLineEdit* m_ForTrresultTemp1;
    QLineEdit* m_ForTrresultTemp2;

    QLineEdit* m_ForGrresultTemp1;
    QLineEdit* m_ForGrresultTemp2;

    QLineEdit* m_ForFrresultTemp1;
    QLineEdit* m_ForFrresultTemp2;

    QLineEdit* m_ForArresultTemp1;
    QLineEdit* m_ForArresultTemp2;

    QLineEdit* m_ForQrresultTemp;
    QLineEdit* m_ForKrresultTemp;
    QLineEdit* m_ForRZrresultTemp;
    ///
    QCheckBox* simpleModeStandartTTOR = nullptr;
    QCheckBox* turnOnStandartDetailsTTOR = nullptr;
    QDoubleSpinBox* diametrTubeTTOR = nullptr;
    QDoubleSpinBox* diametrKozhuxTTOR = nullptr;
    QDoubleSpinBox* thicknessTubeTTOR = nullptr;
    QDoubleSpinBox* thicknessKozhuxTTOR = nullptr;
    QDoubleSpinBox* distanceOporiTTOR = nullptr;
    QDoubleSpinBox* dlinaKameriTTOR = nullptr;
    QDoubleSpinBox* visotaOporyH1TTOR = nullptr;
    QDoubleSpinBox* visotaOporyH2TTOR = nullptr;
    QDoubleSpinBox* distanceConnectorTTOR = nullptr;
    QDoubleSpinBox* dUForFlanecTTOR = nullptr;

    QDoubleSpinBox* LengthTeplTubeTTOR = nullptr;
    QDoubleSpinBox* LengthKozhuxTubeTTOR = nullptr;
    /// <summary>
    /// vars tab management
    /// </summary>
    QTabWidget* m_mainTabWidget;
    QWidget* m_calculationTab;
    QVBoxLayout* m_vLayoutCalculationTabTTOR = nullptr;
    QVBoxLayout* m_vLayoutCalculationTabTTRM = nullptr;
    QVBoxLayout* m_vLayoutCalculationTabIP = nullptr;
    QVBoxLayout* m_vLayoutCalculationTabIU = nullptr;
    QPushButton* m_CalculationButton = nullptr;
    /// <summary>
    /// vars for cutting plane
    /// </summary>
    QDoubleSpinBox* m_Poffset = nullptr;
    QDoubleSpinBox* m_Pa1 = nullptr;
    QDoubleSpinBox* m_Pa2 = nullptr;
    QRadioButton* m_PfreeSection = nullptr;
    QRadioButton* m_PoffsetSection = nullptr;
    /// <summary>
    /// GroupBoxes Scene Widget
    /// </summary>
    QGroupBox* gr_WExploding = nullptr;
    QGroupBox* gr_Wfilters = nullptr;
    QGroupBox* gr_WSelections = nullptr;
    QGroupBox* gr_WRendering = nullptr;
    QGroupBox* gr_WCutting = nullptr;
    QGroupBox* gr_WDimensionBox = nullptr;
    /// <summary>
    /// Vars for create tabs in tab panel
    /// </summary>
    std::string m_mainTabName            = "";
    const std::string m_tabNameSeparator = "_";
    bool m_isSelectionDisabled = true;
    int m_heightButton = 0;

    int mp_thicknessCameraResult;
    int mp_thicknessInnerResult;
    int mp_thicknessOuterResult;
    int mp_dimCamera;
    int mp_dimOuterTube;
};

#endif // __VSN_EXPLODEGROUP_H
