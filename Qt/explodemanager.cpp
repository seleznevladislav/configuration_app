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

void ExplodeManager::radiosExplodeFromToggled(bool checked)
{
    if (checked)
    {
    	const auto paramType = GetParameterType(sender());
        const bool doUpdateTree = (paramType == ED::cpt_ExplodeFromCenter);
        applyParameter(paramType, true, { true, doUpdateTree, true });
    }
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

    vLayout->addWidget(combo);
    return combo;
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

    m_groupExpl = createGroupBox("", false, false, true);
    m_groupExpl->setEnabled(false);
    QObject::connect(m_groupExpl, &QGroupBox::toggled, this, &ExplodeManager::groupExplodeToggled);
    QVBoxLayout* vLayoutGroupExpl = createVBoxLayout(m_groupExpl);

    m_tabWidget = createTabWidget(widget, heightButton, mainTabName);
    vLayoutGroupExpl->addWidget(m_tabWidget);

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

    QPushButton* buttonCreate = createButton(ED::cpt_CreateNewExplodeView, hLayoutButtonsCreate, u8"Открыть", u8"Oткрыть выделенное в новом окне");
    QObject::connect(buttonCreate, &QPushButton::pressed, this, &ExplodeManager::buttonCreatePressed);

    QPushButton* buttonZoomToFit = createButton(ED::cpt_Undefined, hLayoutButtonsCreate, u8"Центрировать сборку", u8"Центрировать объект по размерам экрана");
    QObject::connect(buttonZoomToFit, &QPushButton::pressed, this, &ExplodeManager::buttonZoomToFit);

    QPushButton* buttonReset = createButton(ED::cpt_ResetExplodeView, hLayoutButtonsCreate, u8"Сбросить", u8"Сбросить настройки сцены");
    QObject::connect(buttonReset, &QPushButton::pressed, this, &ExplodeManager::buttonsPressed);

    QGroupBox* groupExplodeButtonGroup = createGroupBox(u8"Разлёт сборки", true, false, true);
    m_vLayoutTab->addWidget(groupExplodeButtonGroup);

    QVBoxLayout* vLayoutExplodeButtons = createVBoxLayout(groupExplodeButtonGroup);

    QHBoxLayout* hLayoutRadiosExplodeFrom = new QHBoxLayout();
    vLayoutExplodeButtons->addLayout(hLayoutRadiosExplodeFrom);

    QRadioButton* radioExplodeFromItem = createRadioButton(ED::cpt_ExplodeFromItem, hLayoutRadiosExplodeFrom, u8"От выбранной детали", false);
    QObject::connect(radioExplodeFromItem, &QRadioButton::toggled, this, &ExplodeManager::radiosExplodeFromToggled);
    
    QRadioButton* radioExplodeFromCenter = createRadioButton(ED::cpt_ExplodeFromCenter, hLayoutRadiosExplodeFrom, u8"От центра сборки", true);
    QObject::connect(radioExplodeFromCenter, &QRadioButton::toggled, this, &ExplodeManager::radiosExplodeFromToggled);

    auto pairSliderLabelExplode = createSliderWithLabel(ED::cpt_Explode, "", vLayoutExplodeButtons, "");
    int labelExplodeWidth = pairSliderLabelExplode.second->sizeHint().width();
    QObject::connect(pairSliderLabelExplode.first, &QSlider::valueChanged, this, &ExplodeManager::slidersExplodeValueChanged);

    auto pairSliderLabelSpeed = createSliderWithLabel(ED::cpt_Speed, u8"Чувствительность:", vLayoutExplodeButtons, "");
    pairSliderLabelSpeed.second->setMinimumWidth(labelExplodeWidth);
    QObject::connect(pairSliderLabelSpeed.first, &QSlider::valueChanged, this, &ExplodeManager::slidersExplodeValueChanged);

    return tabWidget;
}