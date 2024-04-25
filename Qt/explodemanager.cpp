#include "explodemanager.h"

#include "explodewidget.h"
#include <QSignalBlocker>
#include <QSlider>
#include <QCheckBox>
#include <QRadioButton>
#include <QToolTip>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QString>
#include <QStringList>


#include <last.h>

/* ExplodeManager */
ExplodeManager::ExplodeManager(ExplodeWidget* pExplodeWidget)
    : QObject(nullptr)
    , m_pExplodeWidget(pExplodeWidget)
{
}

ExplodeManager::~ExplodeManager()
{
}

void ExplodeManager::init(SceneSegment* pSegmModel, ExplodeTreeView* pTreeWidget)
{
    m_widgetsMap.clear();
    for (QWidget* obj : m_pWidget->findChildren<QWidget*>())
    {
        auto v = obj->property("ControlParameterType");
        if (v.isValid() && (GetParameterType(obj) != ED::cpt_Undefined))
            m_widgetsMap[GetParameterType(obj)] = obj;
    }
    m_pSegmModel = pSegmModel;
    m_pTreeWidget = pTreeWidget;
    m_explodeDispatcher.Init(m_mainTabName, m_tabNameSeparator, m_pSegmModel);
    initWidgets();
    applyParameter(ED::cpt_RebuildExplodeView, Variant(), { true, true, true }); // ED::cpt_ResetExplodeView // m_explodeDispatcher.Recalulate()
}

bool ExplodeManager::isSelectionEnabled() const
{
    return !m_isSelectionDisabled;
}

bool ExplodeManager::isCreateGroupGeometry() const
{
    return false; // m_params.m_createGroupGeometry;
}

bool ExplodeManager::onSelectItem(const SceneSegment* pSegm)
{
    m_explodeDispatcher.SetSelectedSegment(pSegm);
    const SceneSegment* pSegmSelected = m_explodeDispatcher.GetSelectedSegment();
    ExplodeParameterState paramState;
    if (pSegmSelected && m_explodeDispatcher.GetParameterState(ED::cpt_AutoRebuild, paramState) && paramState.GetChecked())
        m_explodeDispatcher.SetParameter(ED::cpt_ExplodeFromItem, Variant());
    update(true, false, false);
    return pSegmSelected;
}

void ExplodeManager::initWidgets()
{
    m_isSelectionDisabled = false;
    m_groupExpl->setEnabled(true);
    m_groupAdditionalOptions->setEnabled(true);
    m_tabWidget->setTabsClosable(false);

    /*for (int idx = m_tabWidget->count() - 1; idx > 0; --idx)
        tabWidgetCloseRequested(idx);*/

    QTabBar* tabBar = m_tabWidget->tabBar();
    QTabBar::ButtonPosition closeSide = (QTabBar::ButtonPosition)tabBar->style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition, 0/*, this*/);
    tabBar->setTabButton(0, closeSide, 0);
    m_parametersForUpdateGUI = m_explodeDispatcher.GetControlParameters();
}

void ExplodeManager::applyParameter(const ExplodeDispatcher::ControlParameterType paramType, const Variant& data, std::array<bool, 3> params)
{
    m_explodeDispatcher.SetParameter(paramType, data);
    update(params[0], params[1], params[2]);
}

void ExplodeManager::update(const bool doUpdateWidgets, const bool doUpdateTree, const bool doUpdateScene)
{
    if (doUpdateWidgets)
    {
        for (const auto param : m_parametersForUpdateGUI)
        {
            auto iter = m_widgetsMap.find(param);
            if (iter == m_widgetsMap.end())
                continue;
            QWidget* widget = iter->second;
            ExplodeParameterState paramState;
            ExplodeParameterValues paramValues;
            if (m_explodeDispatcher.GetParameterState(param, paramState) == ExplodeDispatcher::rs_Success)
                updateWidgetState(widget, paramState);
            if (m_explodeDispatcher.GetParameterValues(param, paramValues) == ExplodeDispatcher::rs_Success)
                updateWidgetValues(widget, paramValues);
        }
        updateLabelText(m_labelLevel, ED::cpt_TreeLevel);
        updateLabelText(m_labelSelectAssembly, ED::cpt_SelectAssembly);
    }
    if (doUpdateTree)
    {
        const SceneSegment* pSegm = m_explodeDispatcher.GetSelectedSegment();
        m_pTreeWidget->setCurrentItem(pSegm ? m_pTreeWidget->findItemByObject(pSegm) : nullptr);
    }
    if (doUpdateScene)
        m_pExplodeWidget->update();
}

void ExplodeManager::updateWidgetState(QWidget* wdg, const ExplodeParameterState& paramState)
{
    //const QSignalBlocker blockSignal(wdg);
    if (auto pb = dynamic_cast<QPushButton*>(wdg))
    {
        const QSignalBlocker blockSignal(pb);
        pb->setEnabled(paramState.GetEnabled());
    }
    else if (auto rb = dynamic_cast<QRadioButton*>(wdg))
    {
        const QSignalBlocker blockSignal(rb);
        rb->setChecked(paramState.GetChecked());
        rb->setEnabled(paramState.GetEnabled());
    }
    else if (auto cb = dynamic_cast<QCheckBox*>(wdg))
    {
        const QSignalBlocker blockSignal(cb);
        cb->setChecked(paramState.GetChecked());
        cb->setEnabled(paramState.GetEnabled());
    }
    else if (auto sl = dynamic_cast<QSlider*>(wdg))
    {
        const QSignalBlocker blockSignal(sl);
        sl->setEnabled(paramState.GetEnabled());
    }
}

void ExplodeManager::updateWidgetValues(QWidget* wdg, const ExplodeParameterValues& paramValues)
{
    if (auto sl = dynamic_cast<QSlider*>(wdg))
    {
        const QSignalBlocker blockSignal(sl);
        sl->setRange(paramValues.GetMinimumValue(), paramValues.GetMaximumValue());
        sl->setValue(paramValues.GetCurrentValue());
    }
}

void ExplodeManager::updateLabelText(QLabel* label, const ExplodeDispatcher::ControlParameterType& param)
{
    ExplodeParameterValues paramValues;
    if (m_explodeDispatcher.GetParameterValues(param, paramValues) == ExplodeDispatcher::rs_Success)
    {
        std::string s = "Level (%i0-%i1): %i2 ";
        s.replace(s.find("%i0"), 3, std::to_string(paramValues.GetMinimumValue()));
        s.replace(s.find("%i1"), 3, std::to_string(paramValues.GetMaximumValue()));
        s.replace(s.find("%i2"), 3, std::to_string(paramValues.GetCurrentValue()));
        label->setText(s.c_str());
    }
}

void ExplodeManager::saveCameraPosition()
{
    const Camera& camera = *m_pExplodeWidget->viewport()->GetCamera();
    m_explodeDispatcher.SetCameraOptions(camera.GetPosition(), camera.GetTargetPosition(), camera.GetUpVector());
}

void ExplodeManager::restoreCameraPosition()
{
    Point3DF position;
    Point3DF targetPosition;
    Vector3DF upVector;
    m_explodeDispatcher.GetCameraOptions(position, targetPosition, upVector);
    Camera& camera = *m_pExplodeWidget->viewport()->GetCamera();
    camera.SetPosition(position);
    camera.SetPositionTarget(targetPosition);
    camera.SetUpVector(upVector);
}

void ExplodeManager::tabWidgetCurrentChanged(int idx)
{
    saveCameraPosition();
    const std::string tabNameNew = m_tabWidget->tabText(idx).toStdString();
    if (!m_explodeDispatcher.SwitchActiveItem(tabNameNew))
        return;
    m_tabWidget->currentWidget()->setLayout(m_vLayoutTab);
    restoreCameraPosition();
    update(true, true, true);
}

void ExplodeManager::tabWidgetCloseRequested(int idx) // Метод удаляет вкладку
{
    if (idx > 0)
    {
        std::string tabName = m_tabWidget->tabText(idx).toStdString();
        m_explodeDispatcher.RemoveItem(tabName);
        m_tabWidget->removeTab(idx);
        m_tabWidget->setCurrentIndex(0);
    }
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeManager::groupExplodeToggled(bool check)
{
    if (!m_pSegmModel)
        return;
    if (check)
        restoreCameraPosition();
    else
        saveCameraPosition();
    applyParameter(ED::cpt_ExplodeToggled, check, { false, false, true });
}

void ExplodeManager::buttonCreatePressed()
{
    if (const SceneSegment* pSegmSelected = m_explodeDispatcher.GetSelectedSegment())
    {
        saveCameraPosition();
        int maxTabNumber = m_explodeDispatcher.GetMaximumTabNumber();
        std::string tabName = m_mainTabName + m_tabNameSeparator + std::to_string(++maxTabNumber == 1 ? ++maxTabNumber: maxTabNumber);
        if (m_explodeDispatcher.CreateNewItem(tabName, pSegmSelected))
            m_tabWidget->setCurrentIndex(m_tabWidget->addTab(new QWidget(), tabName.c_str()));
    }
}

void ExplodeManager::buttonsPressed()
{
    applyParameter(GetParameterType(sender()), Variant(), { true, true, true });
}

void ExplodeManager::buttonZoomToFit()
{
    m_pExplodeWidget->viewport()->ZoomToFit(m_pExplodeWidget->sceneContent()->GetBoundingBox());
    update(false, false, true);
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeManager::slidersExplodeValueChanged(const int value)
{
    applyParameter(GetParameterType(sender()), value, { true, false, true });
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeManager::sliderSelectAssemblyValueChanged(const int value)
{
    m_isSelectionDisabled = true;
    applyParameter(GetParameterType(sender()), value, { true, true, false });
    m_isSelectionDisabled = false;
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeManager::slidersFilterValueChanged(const int val)
{
    const auto paramType = GetParameterType(sender());
    applyParameter(paramType, val, { false, false, true });
    ExplodeParameterValues paramValues;
    if (m_explodeDispatcher.GetParameterValues(paramType, paramValues))
        QToolTip::showText(QCursor::pos(), QString("%1").arg(paramValues.GetFinalValue()), nullptr);
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeManager::radiosTrajectryTypeToggled(bool checked)
{
    if (checked)
        applyParameter(GetParameterType(sender()), true, { false, false, true });
}

void ExplodeManager::radiosExplodeFromToggled(bool checked)
{
    if (checked)
    {
    	const auto paramType = GetParameterType(sender());
        const bool doUpdateTree = (paramType == ED::cpt_ExplodeFromCenter);
        applyParameter(paramType, true, { true, doUpdateTree, true });
    }
}

void ExplodeManager::checkboxAutoRebuildToggled(bool checked)
{
    applyParameter(ED::cpt_AutoRebuild, checked, { true, false, true });
}

void ExplodeManager::checkboxRefineAxisSymmetryToggled(bool checked)
{
    applyParameter(ED::cpt_RefineAxisSymmetry, checked, { false, false, true });
}

void ExplodeManager::checkboxesToggled(bool checked)
{
    applyParameter(GetParameterType(sender()), checked, { });
}

ExplodeDispatcher::ControlParameterType ExplodeManager::GetParameterType(const QObject* widg) const
{
    return static_cast<ExplodeDispatcher::ControlParameterType>(widg->property("ControlParameterType").toInt());
}

QGroupBox* ExplodeManager::createGroupBox(const char* title, const bool flat, const bool checkable, const bool checked)
{
    QGroupBox* groupbox = new QGroupBox(m_pWidget);
    groupbox->setFlat(flat);
    groupbox->setCheckable(checkable);
    groupbox->setChecked(checked);
    groupbox->setTitle(QObject::tr(title));
    return groupbox;
}

QVBoxLayout* ExplodeManager::createVBoxLayout(QGroupBox* group)
{
    QVBoxLayout* vLayout = new QVBoxLayout(group);
    vLayout->setContentsMargins(4, 2, 4, 2);
    vLayout->setSpacing(2);
    return vLayout;
}

QComboBox* ExplodeManager::createCombobox(QVBoxLayout* vLayout)
{   
    QComboBox* combo = new QComboBox(m_pWidget);

    QStringList values;
    // Здесь загоняются ваши элементы (варианты конфигураций) в комбобокс

    for (const auto& config : configuration) {
        values.append(QString::fromStdString(config.name));
    }

    combo->addItems(values);

    vLayout->addWidget(combo);
    return combo;
}

QComboBox* ExplodeManager::createComboboxZarubin(QVBoxLayout* vLayout)
{   
    QComboBox* comboZarubinParams = new QComboBox(m_pWidget);
    QStringList valuesZarubinParams;
    // TODO: Здесь загоняются ваши элементы (варианты конфигураций) в комбобокс
    // Здесь добавлять ничего не нужно.
    for (const auto& param : configurationZarubin) {
        valuesZarubinParams.append(QString::fromStdString(param.name));
    }

    comboZarubinParams->addItems(valuesZarubinParams);
    vLayout->addWidget(comboZarubinParams);

    return comboZarubinParams;
}

QPushButton* ExplodeManager::createButton(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const char* tip)
{
    QPushButton* button = new QPushButton(m_pWidget);
    button->setProperty("ControlParameterType", QVariant((int)param));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    button->setMinimumHeight(m_heightButton);
    button->setText(QObject::tr(text));
    button->setToolTip(QString(tip));
    hLayout->addWidget(button);
    return button;
}

std::pair<QSlider*, QLabel*> ExplodeManager::createSliderWithLabel(const ExplodeDispatcher::ControlParameterType param, const char* labelName, QVBoxLayout* vLayout, const char* tip)
{
    QHBoxLayout* hLayoutSlider = new QHBoxLayout();
    QLabel* label = new QLabel(labelName);
    QSlider* slider = new QSlider();
    slider->setProperty("ControlParameterType", QVariant((int)param));
    slider->setOrientation(Qt::Horizontal);
    hLayoutSlider->addWidget(label);
    hLayoutSlider->addWidget(slider);
    vLayout->addLayout(hLayoutSlider);
    return std::pair<QSlider*, QLabel*>(slider, label);
}

QCheckBox* ExplodeManager::createCheckBox(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const bool checked, const char* tip)
{
    QCheckBox* checkbox = new QCheckBox(m_pWidget);
    checkbox->setProperty("ControlParameterType", QVariant((int)param));
    checkbox->setText(QString(text));
    checkbox->setToolTip(QString(tip));
    checkbox->setChecked(checked);
    hLayout->addWidget(checkbox);
    return checkbox;
}

QRadioButton* ExplodeManager::createRadioButton(const ExplodeDispatcher::ControlParameterType param, QHBoxLayout* hLayout, const char* text, const bool checked)
{
    QRadioButton* radiobutton = new QRadioButton(m_pWidget);
    radiobutton->setProperty("ControlParameterType", QVariant((int)param));
    radiobutton->setText(QString(text));
    radiobutton->setChecked(checked);
    hLayout->addWidget(radiobutton);
    return radiobutton;
}

QGroupBox* ExplodeManager::createGroupExplode(QWidget& widget, const int heightButton, const std::string& mainTabName)
{
    m_mainTabName = mainTabName;

    m_groupExpl = createGroupBox("Explode views", false, true, true);
    m_groupExpl->setEnabled(false);
    QObject::connect(m_groupExpl, &QGroupBox::toggled, this, &ExplodeManager::groupExplodeToggled);
    QVBoxLayout* vLayoutGroupExpl = createVBoxLayout(m_groupExpl);

    m_tabWidget = createTabWidget(widget, heightButton, mainTabName);
    vLayoutGroupExpl->addWidget(m_tabWidget);

    // Group with synchronization options
    QGroupBox* groupSynchronization = createGroupBox("Synchronization options", true, true, true);
    QVBoxLayout* vLayoutSynchronizeModes = createVBoxLayout(groupSynchronization);
    QHBoxLayout* hLayoutSynchronize = new QHBoxLayout();
    vLayoutSynchronizeModes->addLayout(hLayoutSynchronize);

    QCheckBox* checkboxSynchronizeExplode = createCheckBox(ED::cpt_SynchronizeExplode, hLayoutSynchronize, "Explode", false, "");
    QObject::connect(checkboxSynchronizeExplode, &QRadioButton::toggled, this, &ExplodeManager::checkboxesToggled);

    QCheckBox* checkboxSynchronizeCamera = createCheckBox(ED::cpt_SynchronizeCamera, hLayoutSynchronize, "Camera", false, "");
    QObject::connect(checkboxSynchronizeCamera, &QRadioButton::toggled, this, &ExplodeManager::checkboxesToggled);
    hLayoutSynchronize->addWidget(checkboxSynchronizeCamera);

    QCheckBox* checkboxSynchronizeSelection = createCheckBox(ED::cpt_SynchronizeSelection, hLayoutSynchronize, "Selection", true, "");
    QObject::connect(checkboxSynchronizeSelection, &QRadioButton::toggled, this, &ExplodeManager::checkboxesToggled);
    hLayoutSynchronize->addWidget(new QLabel(""));

    vLayoutGroupExpl->addWidget(groupSynchronization);
    return m_groupExpl;
}

QTabWidget* ExplodeManager::createTabWidget(QWidget& widget, const int heightButton, const std::string& mainTabName)
{
    m_pWidget = &widget;
    m_heightButton = heightButton;
    QTabWidget* tabWidget = new QTabWidget();

    tabWidget->addTab(new QWidget(), m_mainTabName.c_str());
    tabWidget->addTab(new QWidget(), u8"Параметризация");

    m_vLayoutTab = new QVBoxLayout();
    m_vLayoutTab->setSpacing(2);

    m_vLayoutConfigureTab = new QVBoxLayout();
    m_vLayoutConfigureTab->setSpacing(2);

    m_vLayoutTab->setContentsMargins(4, 2, 4, 2);

    tabWidget->currentWidget()->setLayout(m_vLayoutTab);
    tabWidget->widget(1)->setLayout(m_vLayoutConfigureTab);

    m_vLayoutTab->setSizeConstraint(QLayout::SetMinimumSize);
    QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &ExplodeManager::tabWidgetCurrentChanged);
    QObject::connect(tabWidget, &QTabWidget::tabCloseRequested, this, &ExplodeManager::tabWidgetCloseRequested);

    m_comboConfigure = createCombobox(m_vLayoutConfigureTab);
    m_comboConfigureZarubin = createComboboxZarubin(m_vLayoutConfigureTab);
    //QObject::connect(configureCombobox, SIGNAL(activated()), this, SLOT(configureModel(int)));
    //QObject::connect(configureCombobox, SIGNAL(), SLOT(configureModel(int)));
    /*QObject::connect(configureCombobox, QOverload<int>::of(&QComboBox::activated), widget, &ExplodeWidget::configureModel);*/

    // Group for Explode options
    QGroupBox* groupExpl = createGroupBox("", true, false, true);
    m_vLayoutTab->addWidget(groupExpl);
    groupExpl->setObjectName("groupExpl");
    groupExpl->setStyleSheet("QGroupBox#groupExpl {border:0;}");
    QVBoxLayout* vLayoutExplode = createVBoxLayout(groupExpl);
    QHBoxLayout* hLayoutButtonsCreate = new QHBoxLayout();
    vLayoutExplode->addLayout(hLayoutButtonsCreate);

    QPushButton* buttonCreate = createButton(ED::cpt_CreateNewExplodeView, hLayoutButtonsCreate, "Create", "Open selected assembly in new mode");
    QObject::connect(buttonCreate, &QPushButton::pressed, this, &ExplodeManager::buttonCreatePressed);

    QPushButton* buttonStartReview = createButton(ED::cpt_StartReviewMode, hLayoutButtonsCreate, "Start review", "Open selected assembly in review mode");
    QObject::connect(buttonStartReview, &QPushButton::pressed, this, &ExplodeManager::buttonsPressed);

    QPushButton* buttonStopReview = createButton(ED::cpt_StopReviewMode, hLayoutButtonsCreate, "Stop review", "Close review mode and back to initial mode");
    QObject::connect(buttonStopReview, &QPushButton::pressed, this, &ExplodeManager::buttonsPressed);

    QHBoxLayout* hLayoutButtonsRebuild = new QHBoxLayout();
    vLayoutExplode->addLayout(hLayoutButtonsRebuild);

    QPushButton* buttonRebuild = createButton(ED::cpt_RebuildExplodeView, hLayoutButtonsRebuild, "Rebuild", "Rebuild explode model to set new center");
    QObject::connect(buttonRebuild, &QPushButton::pressed, this, &ExplodeManager::buttonsPressed);

    QPushButton* buttonZoomToFit = createButton(ED::cpt_Undefined, hLayoutButtonsRebuild, "Zoom to fit", "Zoom the object to fit the view screen");
    QObject::connect(buttonZoomToFit, &QPushButton::pressed, this, &ExplodeManager::buttonZoomToFit);

    QPushButton* buttonReset = createButton(ED::cpt_ResetExplodeView, hLayoutButtonsRebuild, "Reset", "Restore initial explode options for current mode");
    QObject::connect(buttonReset, &QPushButton::pressed, this, &ExplodeManager::buttonsPressed);

    QHBoxLayout* hLayoutCheckboxesRebuild = new QHBoxLayout();
    vLayoutExplode->addLayout(hLayoutCheckboxesRebuild);

    QCheckBox* checkboxAutoRebuild = createCheckBox(ED::cpt_AutoRebuild, hLayoutCheckboxesRebuild, "Auto rebuild", true,
        "Auto rebuild on selection to set new explosion center");
    QObject::connect(checkboxAutoRebuild, &QCheckBox::toggled, this, &ExplodeManager::checkboxAutoRebuildToggled);

    QCheckBox* checkboxCollapse = createCheckBox(ED::cpt_CollapsOnRebuild, hLayoutCheckboxesRebuild, "Collapse on rebuild", false, "Collapse model on Rebuild");
    QObject::connect(checkboxCollapse, &QCheckBox::toggled, this, &ExplodeManager::checkboxesToggled);

    QHBoxLayout* hLayoutRadiosExplodeFrom = new QHBoxLayout();
    vLayoutExplode->addLayout(hLayoutRadiosExplodeFrom);

    QRadioButton* radioExplodeFromItem = createRadioButton(ED::cpt_ExplodeFromItem, hLayoutRadiosExplodeFrom, "From selected item", false);
    QObject::connect(radioExplodeFromItem, &QRadioButton::toggled, this, &ExplodeManager::radiosExplodeFromToggled);
    
    QRadioButton* radioExplodeFromCenter = createRadioButton(ED::cpt_ExplodeFromCenter, hLayoutRadiosExplodeFrom, "From model center", true);
    QObject::connect(radioExplodeFromCenter, &QRadioButton::toggled, this, &ExplodeManager::radiosExplodeFromToggled);

    auto pairSliderLabelExplode = createSliderWithLabel(ED::cpt_Explode, "Explode:", vLayoutExplode, "");
    int labelExplodeWidth = pairSliderLabelExplode.second->sizeHint().width();
    QObject::connect(pairSliderLabelExplode.first, &QSlider::valueChanged, this, &ExplodeManager::slidersExplodeValueChanged);

    auto pairSliderLabelSpeed = createSliderWithLabel(ED::cpt_Speed, "Sensitivity:", vLayoutExplode, "");
    pairSliderLabelSpeed.second->setMinimumWidth(labelExplodeWidth);
    QObject::connect(pairSliderLabelSpeed.first, &QSlider::valueChanged, this, &ExplodeManager::slidersExplodeValueChanged);

    auto pairSliderLabelLevel = createSliderWithLabel(ED::cpt_TreeLevel, "Level (0-0): 0", vLayoutExplode, "");
    m_labelLevel = pairSliderLabelLevel.second;
    QObject::connect(pairSliderLabelLevel.first, &QSlider::valueChanged, this, &ExplodeManager::slidersExplodeValueChanged);

    // Group for Selection options
    QGroupBox* groupSelection = createGroupBox("Selection", true, false, true);
    m_vLayoutTab->addWidget(groupSelection);
    QVBoxLayout* vLayoutSelection = createVBoxLayout(groupSelection);
    QHBoxLayout* hLayoutButtonsSelect = new QHBoxLayout();
    QPushButton* buttonSelectAll = createButton(ED::cpt_SelectAll, hLayoutButtonsSelect, "Select all", "Selection all the  of model or assembly");
    QObject::connect(buttonSelectAll, &QPushButton::pressed, this, &ExplodeManager::buttonsPressed);
    vLayoutSelection->addLayout(hLayoutButtonsSelect);

    QPushButton* buttonSelectRootItem = createButton(ED::cpt_SelectRootItem, hLayoutButtonsSelect, "Select root item", "Root assembly selection of the selected object");
    QObject::connect(buttonSelectRootItem, &QPushButton::pressed, this, &ExplodeManager::buttonsPressed);

    auto pairSliderLabelSelectAssembly = createSliderWithLabel(ED::cpt_SelectAssembly, "Level (0-0): 0", vLayoutSelection,
        "Slider for scene graph's assembly selection that contained current assembly");
    m_labelSelectAssembly = pairSliderLabelSelectAssembly.second;
    m_labelSelectAssembly->setToolTip(QString("The level of selected assembly in the scene graph"));
    QObject::connect(pairSliderLabelSelectAssembly.first, &QSlider::valueChanged, this, &ExplodeManager::sliderSelectAssemblyValueChanged);

    // Group "Objects trajectory type"
    QGroupBox* groupTrajectory = createGroupBox("Objects trajectory type", true, false, true);
    m_vLayoutTab->addWidget(groupTrajectory);
    QVBoxLayout* vLayoutTrajectory = createVBoxLayout(groupTrajectory);
    QHBoxLayout* hLayoutTrajectoryType = new QHBoxLayout();
    vLayoutTrajectory->addLayout(hLayoutTrajectoryType);
    QRadioButton* radioRadial = createRadioButton(ED::cpt_RadialTrajectory, hLayoutTrajectoryType, "Radial", true);
    QObject::connect(radioRadial, &QRadioButton::toggled, this, &ExplodeManager::radiosTrajectryTypeToggled);

    QRadioButton* radioAxial = createRadioButton(ED::cpt_AxialTrajectory, hLayoutTrajectoryType,  "Axial", false);
    QObject::connect(radioAxial, &QRadioButton::toggled, this, &ExplodeManager::radiosTrajectryTypeToggled);

    QRadioButton* radioOrth = createRadioButton(ED::cpt_OrthToAxialTrajectory, hLayoutTrajectoryType, "Orth. to axial", false);
    QObject::connect(radioOrth, &QRadioButton::toggled, this, &ExplodeManager::radiosTrajectryTypeToggled);

    // Group "Trajectory quality improvement"
    QGroupBox* groupTestOptions = createGroupBox("Trajectory quality improvement", true, false, true);
    m_vLayoutTab->addWidget(groupTestOptions);
    QVBoxLayout* vLayoutTestOptions = createVBoxLayout(groupTestOptions);
    QHBoxLayout* hLayoutTestOptions = new QHBoxLayout();
    vLayoutTestOptions->addLayout(hLayoutTestOptions);

    QCheckBox* checkboxRefineAxisSymmetry = createCheckBox(ED::cpt_RefineAxisSymmetry, hLayoutTestOptions,
        "Refine assembly axis (demo option)", true, "");
    QObject::connect(checkboxRefineAxisSymmetry, &QRadioButton::toggled, this, &ExplodeManager::checkboxRefineAxisSymmetryToggled);

    // Group for Additional Options
    m_groupAdditionalOptions = createGroupBox("Hide small, big items, hide items by distance to center", true, true, true);
    m_groupAdditionalOptions->setDisabled(true);
    QVBoxLayout* vLayoutAdditionalOptions = createVBoxLayout(m_groupAdditionalOptions);
    
    auto pairSliderLabelHideSmallItems = createSliderWithLabel(ED::cpt_FilterHideSmallItems, "Min size:", vLayoutAdditionalOptions, "");
    QObject::connect(pairSliderLabelHideSmallItems.first, &QSlider::valueChanged, this, &ExplodeManager::slidersFilterValueChanged);
    
    auto pairSliderLabelHideBigItems = createSliderWithLabel(ED::cpt_FilterHideBigItems, "Max size:", vLayoutAdditionalOptions, "");
    QObject::connect(pairSliderLabelHideBigItems.first, &QSlider::valueChanged, this, &ExplodeManager::slidersFilterValueChanged);
    
    auto pairSliderLabelHideByDistance = createSliderWithLabel(ED::cpt_FilterHideByDistance, "Distance:", vLayoutAdditionalOptions, "");
    QObject::connect(pairSliderLabelHideByDistance.first, &QSlider::valueChanged, this, &ExplodeManager::slidersFilterValueChanged);

    m_vLayoutTab->addWidget(m_groupAdditionalOptions);
    return tabWidget;
}