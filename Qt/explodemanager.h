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

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
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
    bool isSelectionEnabled() const;
    bool isCreateGroupGeometry() const;
    bool onSelectItem(const SceneSegment* pSegm);
    QComboBox* m_quantityCombobox = nullptr;
    QComboBox* m_comboConfigure = nullptr;
    QPushButton* m_reconfigureButton = nullptr;

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
       //            1             2   3  4    5   6     7     8    9   10    11    12    13  14    15   16
       {u8"ТТОР 89/133-1,6/1,6",  89,  5, 133, 5, 4500, 5410, 600, 80,  860,  425, 3000, 500, 730,  400, 255},
       {u8"ТТОР 89/133-4,0/1,6",  89,  5, 133, 5, 4500, 5420, 600, 80,  860,  425, 3000, 550, 730,  400, 255},
       {u8"ТТОР 89/133-4,0/4,0",  89,  5, 133, 6, 4500, 5420, 600, 80,  860,  425, 3000, 560, 730,  400, 255},
       {u8"ТТОР 89/159-1,6/1,6",  89,  5, 159, 5, 4500, 5470, 600, 100, 860,  425, 3000, 560, 730,  400, 255},
       {u8"ТТОР 89/159-4,0/1,6",  89,  5, 159, 5, 4500, 5480, 600, 100, 860,  425, 3000, 620, 730,  400, 255},
       {u8"ТТОР 89/159-4,0/4,0",  89,  5, 159, 6, 4500, 5480, 600, 100, 860,  425, 3000, 620, 730,  400, 255},
       {u8"ТТОР 108/159-1,6/1,6", 108, 5, 159, 5, 4500, 5480, 600, 100, 860,  425, 3000, 600, 730,  400, 255},
       {u8"ТТОР 108/159-4,0/1,6", 108, 6, 159, 5, 4500, 5515, 600, 100, 860,  425, 3000, 640, 730,  400, 255},
       {u8"ТТОР 108/159-4,0/4,0", 108, 6, 159, 6, 4500, 5515, 600, 100, 860,  425, 3000, 660, 730,  400, 255},
       //////////////////////////////////////////////////////////////////////////////////////////////////////
       {u8"ТТОР 133/219-1,6/1,6", 133, 5, 219, 7, 6000, 7250, 800, 150, 1080, 535, 3000, 680, 1550, 550, 320},
       {u8"ТТОР 133/219-4,0/1,6", 133, 6, 219, 7, 6000, 7260, 800, 150, 1080, 535, 3000, 740, 1490, 550, 320},
       {u8"ТТОР 133/219-4,0/4,0", 133, 6, 219, 7, 6000, 7260, 800, 150, 1080, 535, 3000, 760, 1475, 550, 320},
       {u8"ТТОР 159/219-1,6/1,6", 159, 5, 219, 7, 6000, 7250, 800, 150, 1080, 535, 3000, 690, 1550, 550, 320},
       {u8"ТТОР 159/219-4,0/1,6", 159, 6, 219, 7, 6000, 7280, 800, 150, 1080, 535, 3000, 760, 1490, 550, 320},
       {u8"ТТОР 159/219-4,0/4,0", 159, 6, 219, 7, 6000, 7280, 800, 150, 1080, 535, 3000, 780, 1475, 550, 320},
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
public:
    QGroupBox* getFiltersGroupBox() const { return gr_Wfilters; }
    QGroupBox* getSelectionsGroupBox() const { return gr_WSelections; }
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
    QFormLayout* createWarmForm(QVBoxLayout* layout);
    void calculateThickness(QLineEdit* innerTubesLineEdit, QLineEdit* outerTubesLineEdit, QLineEdit* gridsLineEdit, QDoubleSpinBox* lengthSpinBox);
    void onReconfigureButtonClicked();
    QAction* createActionButton(const QString& fileName, QGroupBox* groupFilter, QHBoxLayout* fGroupLayout);
    QGroupBox* createFilterGroupBox();
    QGroupBox* createSelectionGroupBox();
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

    QGroupBox* gr_Wfilters = nullptr;
    QGroupBox* gr_WSelections = nullptr;
    std::string m_mainTabName            = "";
    const std::string m_tabNameSeparator = "_";
    bool m_isSelectionDisabled = true;
    int m_heightButton = 0;
};

#endif // __VSN_EXPLODEGROUP_H
