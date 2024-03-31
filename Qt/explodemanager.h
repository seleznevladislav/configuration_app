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
    // TODO: 2) Прописываем здесь через такой же тип переменную, в которую прописываем значения.
    // Важно! чтобы количество переменных в элементе совпадало количеством описанных переменных в структуре struct 
    // Количество элементов может быть любое
    std::vector<ConfigParams> configuration = {
       {u8"ТТРМ 25/57-6,3/1,6", 1500, 750, 2150, 415},
       {u8"ТТРМ 38/89-6,3/4,0", 3000, 1500, 3820, 745},
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
