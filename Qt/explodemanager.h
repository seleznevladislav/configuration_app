#ifndef __VSN_EXPLODEGROUP_H
#define __VSN_EXPLODEGROUP_H

#include "vsn_explodedispatcher.h"

#include "explodetree.h"
#include <QObject>
#include <QGroupBox>
#include "BuildParams.h"
#include "QComboBox"

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
    QComboBox* m_comboConfigure = nullptr;
    QComboBox* m_comboConfigureZarubin = nullptr;

    // TODO: 2) Прописываем здесь через такой же тип переменную, в которую прописываем значения.
    // Важно! чтобы количество переменных в элементе совпадало количеством описанных переменных в структуре struct 
    // Количество элементов может быть любое
    std::vector<ConfigParams> configuration = {
       {u8"ТТРМ 25/57-6,3/1,6", 25, 3, 57, 4, 219, 8, 1500, 750, 2150, 32, 80, 415, 250, 230 },
       {u8"ТТРМ 25/57-6,3/4,0", 25, 3, 57, 4, 219, 8, 3000, 1500, 3680, 32, 80, 875, 250, 230 },
    };
    std::vector<BuildParamsZarubin> configurationZarubin = {
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
    void sliderSelectAssemblyValueChanged(const int val);
    void slidersFilterValueChanged(const int val);
    void radiosTrajectryTypeToggled(bool checked);
    void radiosExplodeFromToggled(bool checked);
    void checkboxAutoRebuildToggled(bool checked);
    void checkboxRefineAxisSymmetryToggled(bool checked);
    void checkboxesToggled(bool checked);
    ExplodeDispatcher::ControlParameterType GetParameterType(const QObject* widg) const;
private:
    QGroupBox* createGroupBox(const char* title, const bool flat, const bool checkable, const bool checked);
    QVBoxLayout* createVBoxLayout(QGroupBox* group);
    QPushButton* createButton(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const char* tip);
    QComboBox* ExplodeManager::createCombobox(QVBoxLayout* vLayout);
    QComboBox* ExplodeManager::createComboboxZarubin(QVBoxLayout* vLayout);
    std::pair<QSlider*, QLabel*> createSliderWithLabel(const ExplodeDispatcher::ControlParameterType param, const char* labelName, QVBoxLayout* vLayout, const char* tip);
    QCheckBox* createCheckBox(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const bool checked, const char* tip);
    QRadioButton* createRadioButton(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const bool checked);
    QTabWidget* createTabWidget(QWidget& widget, const int heightButton, const std::string& mainTabName);
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
    QGroupBox* m_groupAdditionalOptions  = nullptr;
    QLabel* m_labelLevel                 = nullptr;
    QLabel* m_labelSelectAssembly        = nullptr;
    std::string m_mainTabName            = "";
    const std::string m_tabNameSeparator = "_";
    bool m_isSelectionDisabled = true;
    int m_heightButton = 0;
};

#endif // __VSN_EXPLODEGROUP_H
