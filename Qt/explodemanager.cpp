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

static QSize sizeIcons = QSize(32, 32);

/* ExplodeManager */
ExplodeManager::ExplodeManager(ExplodeWidget* pExplodeWidget)
    : QObject(nullptr)
    , m_pExplodeWidget(pExplodeWidget)
    , m_mainTabWidget(nullptr)
    , m_calculationTab(nullptr)
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
    m_tabWidget->setTabsClosable(true);

    const int amountOfMainTabs = 3;

    for (int idx = m_tabWidget->count() - amountOfMainTabs; idx > 0; --idx)
        tabWidgetCloseRequested(idx);

    QTabBar* tabBar = m_tabWidget->tabBar();
    QTabBar::ButtonPosition closeSide = (QTabBar::ButtonPosition)tabBar->style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition, 0/*, this*/);
    tabBar->setTabButton(0, closeSide, 0);
    tabBar->setTabButton(1, closeSide, 0);
    tabBar->setTabButton(2, closeSide, 0);
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

void ExplodeManager::radiosTypeFromToggled(bool checked, int type)
{
    if (checked)
    {
        bool isCheckedManualType = type == 2;
        
        m_comboConfigure->setDisabled(isCheckedManualType);
        m_warmParams->setDisabled(!isCheckedManualType);
        m_reconfigureButton->setDisabled(isCheckedManualType);
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

void ExplodeManager::calculateThickness(QLineEdit* innerTubesLineEdit, QLineEdit* outerTubesLineEdit, QLineEdit* gridsLineEdit, QDoubleSpinBox* lengthSpinBox) {
    // Здесь выполняется ваш расчет
    // Например, предположим, что вы вычислили значения толщины
    double thicknessInnerTubes = 5.0;
    double thicknessOuterTubes = 6.0;
    double thicknessGrids = 7.0;

    // Устанавливаем вычисленные значения в соответствующие QLineEdit
    innerTubesLineEdit->setText(QString::number(thicknessInnerTubes));
    outerTubesLineEdit->setText(QString::number(thicknessOuterTubes));
    gridsLineEdit->setText(QString::number(thicknessGrids));

    lengthSpinBox->setDisabled(false);
    m_reconfigureButton->setDisabled(false);
}

QFormLayout* ExplodeManager::createWarmForm(QVBoxLayout* layout)
{
    QFormLayout* formLayout = new QFormLayout;
    formLayout->setObjectName("warmForm");

    QLabel* coolantLabel = new QLabel(u8"Теплоноситель:");
    QLabel* pressureLabel = new QLabel(u8"Рабочие давление:");
    
    QComboBox* coolantComboBox = new QComboBox;
    coolantComboBox->addItem(u8"Вода");
    QComboBox* pressureComboBox = new QComboBox;
    pressureComboBox->addItem("6");

    formLayout->addRow(coolantLabel, coolantComboBox);
    formLayout->addRow(pressureLabel, pressureComboBox);

    QPushButton* calculateThicknessButton = new QPushButton;
    calculateThicknessButton->setText(u8"Посчитать толщину стенок");
    calculateThicknessButton->setIcon(QIcon(":/res/calculate.png"));
    calculateThicknessButton->setContentsMargins(0, 5, 0, 5);

    formLayout->addRow(calculateThicknessButton);

    QLineEdit* thicknessInnerTubesLineEdit = new QLineEdit;
    thicknessInnerTubesLineEdit->setReadOnly(true);
    QLabel* thicknessInnerTubesLineEditLabel = new QLabel(u8"Толщина теплообменных труб, мм:");

    QLineEdit* thicknessOuterTubesLineEdit = new QLineEdit;
    thicknessOuterTubesLineEdit->setReadOnly(true);
    QLabel* thicknessOuterTubesLineEditLabel = new QLabel(u8"Толщина кожуховых труб, мм:");

    QLineEdit* thicknessGridsLineEdit = new QLineEdit;
    thicknessGridsLineEdit->setReadOnly(true);
    QLabel* thicknessGridsLineEditLabel = new QLabel(u8"Толщина днища у камер, мм:");

    formLayout->addRow(thicknessInnerTubesLineEditLabel, thicknessInnerTubesLineEdit);
    formLayout->addRow(thicknessOuterTubesLineEditLabel, thicknessOuterTubesLineEdit);
    formLayout->addRow(thicknessGridsLineEditLabel, thicknessGridsLineEdit);

    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setContentsMargins(0, 10, 0, 10);
    formLayout->addRow(line);

    QDoubleSpinBox* lengthSpinBox = new QDoubleSpinBox;
    lengthSpinBox->setDisabled(true);
    lengthSpinBox->setRange(2050, 7040);
    lengthSpinBox->setSingleStep(50);
    lengthSpinBox->setValue(2050);

    QLabel* lengthSpinBoxLabel = new QLabel(u8"Длина L, мм:");

    formLayout->addRow(lengthSpinBoxLabel, lengthSpinBox);
    

    layout->addLayout(formLayout);

    connect(calculateThicknessButton, &QPushButton::clicked, [=]() {
        calculateThickness(thicknessInnerTubesLineEdit, thicknessOuterTubesLineEdit, thicknessGridsLineEdit, lengthSpinBox);
        });

    return formLayout;
}

QFormLayout* ExplodeManager::createParametrizationForm(QVBoxLayout* layout)
{
    QFormLayout* formLayout = new QFormLayout;
    formLayout->setObjectName("warmForm");

    QDoubleSpinBox* pressureSpinBox = new QDoubleSpinBox;
    //pressureSpinBox->setRange(2050, 7040); Можно задать диапозон вводимых значений
    //pressureSpinBox->setSingleStep(50); Можжно задать шаг увелечения/уменьшения
    //pressureSpinBox->setValue(2050); Инициализация числом

    QLabel* pressureSpinBoxLabel = new QLabel(u8"Давление P:");

    QDoubleSpinBox* innerThicknessSpinBox = new QDoubleSpinBox;
    QLabel* innerThicknessSpinBoxLabel = new QLabel(u8"Диаметр кожуха D, мм:");

    QDoubleSpinBox* cameraThicknessSpinBox = new QDoubleSpinBox;
    QLabel* cameraThicknessSpinBoxLabel = new QLabel(u8"Диаметр камеры D, мм:");

    QDoubleSpinBox* iSpinBox = new QDoubleSpinBox;
    QLabel* iSpinBoxLabel = new QLabel(u8"I0");

    QDoubleSpinBox* iSecondSpinBox = new QDoubleSpinBox;
    QLabel* iSecondSpinBoxLabel = new QLabel(u8"I2");

    QDoubleSpinBox* iThirdSpinBox = new QDoubleSpinBox;
    QLabel* iThirdSpinBoxLabel = new QLabel(u8"I3");

    formLayout->addRow(pressureSpinBoxLabel, pressureSpinBox);
    formLayout->addRow(innerThicknessSpinBoxLabel, innerThicknessSpinBox);
    formLayout->addRow(cameraThicknessSpinBoxLabel, cameraThicknessSpinBox);
    formLayout->addRow(iSpinBoxLabel, iSpinBox);
    formLayout->addRow(iSecondSpinBoxLabel, iSecondSpinBox);
    formLayout->addRow(iThirdSpinBoxLabel, iThirdSpinBox);

    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setContentsMargins(0, 10, 0, 10);
    formLayout->addRow(line);

    layout->addLayout(formLayout);

    return formLayout;
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

QRadioButton* ExplodeManager::createTypeRadioButton(QHBoxLayout* hLayout, const char* text, const bool checked, int type)
{
    QRadioButton* radiobutton = new QRadioButton(m_pWidget);

    radiobutton->setText(QString(text));
    radiobutton->setChecked(checked);
    hLayout->addWidget(radiobutton);

    connect(radiobutton, &QRadioButton::toggled, this, [this, type](bool checked) {
        radiosTypeFromToggled(checked, type);
        });

    return radiobutton;
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

enum class FluidType {
    Water,
    Oil,
    EthyleneGlycol,
    Air,
    R134a,
    Methanol,
    Ethanol,
    Propane,
    Custom // для пользовательских жидкостей
};

QStringList fluidTypeNames = {
    u8"Вода",
    u8"Нефть",
    u8"Этиленгликоль",
    u8"Воздух",
    u8"R134a",
    u8"Метанол",
    u8"Этанол",
    u8"Пропан",
    u8"Пользовательская"
};

void ExplodeManager::createCalculationTab(const int numberOfHeatExchanger)
{
    m_calculationTab = new QWidget();

    m_mainTabWidget->addTab(m_calculationTab, u8"Расчеты");

    switch (numberOfHeatExchanger)
    {
        case 1: //ExplodeWidget::TTOR
        {
            m_vLayoutCalculationTabTTOR = new QVBoxLayout();

            // Поля для ввода температур
            m_vLayoutCalculationTabTTOR->addWidget(new QLabel(u8"Температура горячей среды на входе (°C):"));
            QLineEdit* hotInletTemp = new QLineEdit();
            m_vLayoutCalculationTabTTOR->addWidget(hotInletTemp);

            m_vLayoutCalculationTabTTOR->addWidget(new QLabel(u8"Температура холодной среды на входе (°C):"));
            QLineEdit* coldInletTemp = new QLineEdit();
            m_vLayoutCalculationTabTTOR->addWidget(coldInletTemp);

            // Поля для ввода скоростей
            m_vLayoutCalculationTabTTOR->addWidget(new QLabel(u8"Скорость горячей среды (м/с):"));
            QLineEdit* hotVelocity = new QLineEdit();
            m_vLayoutCalculationTabTTOR->addWidget(hotVelocity);

            m_vLayoutCalculationTabTTOR->addWidget(new QLabel(u8"Скорость холодной среды (м/с):"));
            QLineEdit* coldVelocity = new QLineEdit();
            m_vLayoutCalculationTabTTOR->addWidget(coldVelocity);

            // Комбобоксы для выбора теплоносителей
            m_vLayoutCalculationTabTTOR->addWidget(new QLabel(u8"Горячий теплоноситель:"));
            QComboBox* hotFluidComboBox = new QComboBox();
            hotFluidComboBox->addItems(fluidTypeNames);
            m_vLayoutCalculationTabTTOR->addWidget(hotFluidComboBox);

            m_vLayoutCalculationTabTTOR->addWidget(new QLabel(u8"Холодный теплоноситель:"));
            QComboBox* coldFluidComboBox = new QComboBox();
            coldFluidComboBox->addItems(fluidTypeNames);
            m_vLayoutCalculationTabTTOR->addWidget(coldFluidComboBox);

            // Поля для ввода теплопередающего коэффициента и площади теплообменника
            m_vLayoutCalculationTabTTOR->addWidget(new QLabel(u8"Коэффициент теплопередачи (Вт/м²·°C):"));
            QLineEdit* heatTransferCoefficient = new QLineEdit();
            m_vLayoutCalculationTabTTOR->addWidget(heatTransferCoefficient);

            m_vLayoutCalculationTabTTOR->addWidget(new QLabel(u8"Площадь теплообменника (м²):"));
            QLineEdit* area = new QLineEdit();
            m_vLayoutCalculationTabTTOR->addWidget(area);

            m_calculationTab->setLayout(m_vLayoutCalculationTabTTOR);
        }
        case 2: //ExplodeWidget::TTRM
        {
            m_vLayoutCalculationTabTTRM = new QVBoxLayout();

            m_calculationTab->setLayout(m_vLayoutCalculationTabTTRM);
            break;
        }
        case 3: //ExplodeWidget::IP
        {
            m_vLayoutCalculationTabIP = new QVBoxLayout();

            m_calculationTab->setLayout(m_vLayoutCalculationTabIP);
            break;
        }
        case 4: //ExplodeWidget::IU
        {

            m_vLayoutCalculationTabIU = new QVBoxLayout();

            m_calculationTab->setLayout(m_vLayoutCalculationTabIU);
            break;
        }
        default:
            break;
    }
} 

void ExplodeManager::onReconfigureButtonClicked() {
    if (m_pExplodeWidget) {
        QVariant propertyValue = m_reconfigureButton->property("CommandsHeatExhanger");
        ExplodeWidget::Exhanchares cmd = static_cast<ExplodeWidget::Exhanchares>(propertyValue.toInt());

        m_pExplodeWidget->viewCommandsHeats(cmd);
    }
}

QAction* ExplodeManager::createActionButton(const QString& fileName, QGroupBox* groupFilter, QHBoxLayout* fGroupLayout)
{
    QToolButton* filterBut = new QToolButton(groupFilter);
    filterBut->setIconSize(QSize(24, 24));
    QAction* actFilter = new QAction(QIcon(fileName), "", groupFilter);
    actFilter->setObjectName(QString("ID_") + fileName);
    actFilter->setCheckable(true);
    filterBut->setDefaultAction(actFilter);
    fGroupLayout->addWidget(filterBut);
    return actFilter;
}

QGroupBox* ExplodeManager::createExplodingGroupBox()
{
    gr_WExploding = createGroupBox(u8"Разлёт сборки", true, false, true);
    m_vLayoutTab->addWidget(gr_WExploding);

    QVBoxLayout* vLayoutExplodeButtons = createVBoxLayout(gr_WExploding);

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

    return gr_WExploding;
}

QGroupBox* ExplodeManager::createFilterGroupBox()
{
    gr_Wfilters = createGroupBox(u8"Фильтры", true, false, true);
    QHBoxLayout* fGroupLayout = new QHBoxLayout(gr_Wfilters);
    fGroupLayout->setMargin(10); fGroupLayout->setSpacing(0);

    QActionGroup* actionGroupFilter = new QActionGroup(m_pExplodeWidget);
    actionGroupFilter->setExclusive(false);

    QAction* bodyFilterButton = createActionButton(":/res/filterbody24x24.png", gr_Wfilters, fGroupLayout);
    bodyFilterButton->setToolTip(QStringLiteral("Тело"));
    bodyFilterButton->setChecked(true); // Set checked default true because you can select body from start

    actionGroupFilter->addAction(bodyFilterButton);
    actionGroupFilter->addAction(createActionButton(":/res/filterface24x24.png", gr_Wfilters, fGroupLayout))->setToolTip(QStringLiteral("Грань"));
    actionGroupFilter->addAction(createActionButton(":/res/filteredge24x24.png", gr_Wfilters, fGroupLayout))->setToolTip(QStringLiteral("Ребро"));
    actionGroupFilter->addAction(createActionButton(":/res/filtervertex24x24.png", gr_Wfilters, fGroupLayout))->setToolTip(QStringLiteral("Точка"));

    m_pExplodeWidget->setGroupFilter(actionGroupFilter);
    QObject::connect(actionGroupFilter, SIGNAL(triggered(QAction*)), m_pExplodeWidget, SLOT(slotFilterTriggered(QAction*)));

    return gr_Wfilters;
}

QGroupBox* ExplodeManager::createSelectionGroupBox()
{
    gr_WSelections = createGroupBox(u8"Опции селектирования", true, false, true);
    QVBoxLayout* vGroupLayoutColors = new QVBoxLayout(gr_WSelections);
    QCheckBox* highlightBox = new QCheckBox(QStringLiteral("Динамическое выделение"));
    highlightBox->setChecked(true);
    QObject::connect(highlightBox, SIGNAL(stateChanged(int)), m_pExplodeWidget, SLOT(slotDynamicHighlighting(int)));

    ColorButton* highlightColor = new ColorButton(gr_WSelections);
    highlightColor->setText(QStringLiteral(" Выделение     "));
    highlightColor->setColor(m_pExplodeWidget->highlightColor());
    QObject::connect(highlightColor, SIGNAL(colorChanged(const QColor&)), m_pExplodeWidget, SLOT(slotHighlightColor(const QColor&)));
    ColorButton* selectionColor = new ColorButton(gr_WSelections);
    selectionColor->setText(QStringLiteral(" Селектирование"));
    selectionColor->setColor(m_pExplodeWidget->selectionColor());
    QObject::connect(selectionColor, SIGNAL(colorChanged(const QColor&)), m_pExplodeWidget, SLOT(slotSelectionColor(const QColor&)));
    vGroupLayoutColors->addWidget(highlightBox);
    vGroupLayoutColors->addWidget(highlightColor);
    vGroupLayoutColors->addWidget(selectionColor);

    return gr_WSelections;
}

QGroupBox* ExplodeManager::createRenderingGroupBox()
{
    gr_WRendering = createGroupBox(u8"Режим рендера", true, false, true);

    QHBoxLayout* fGroupLayout = new QHBoxLayout(gr_WRendering);
    fGroupLayout->setMargin(10); fGroupLayout->setSpacing(0);

    QActionGroup* actionGroupFilter = new QActionGroup(m_pExplodeWidget);

    QAction* bodyFilterButton = createActionButton(":/res/renderMods/dimmedWireframe.png", gr_WRendering, fGroupLayout);
    bodyFilterButton->setToolTip(QStringLiteral("1"));
    bodyFilterButton->setChecked(true); // Set checked default true because you can select body from start

    actionGroupFilter->addAction(bodyFilterButton);
    actionGroupFilter->addAction(createActionButton(":/res/renderMods/shaded.png", gr_WRendering, fGroupLayout))->setToolTip(QStringLiteral("2"));
    actionGroupFilter->addAction(createActionButton(":/res/renderMods/wirefrm.png", gr_WRendering, fGroupLayout))->setToolTip(QStringLiteral("3"));
    actionGroupFilter->addAction(createActionButton(":/res/renderMods/hiddenremoved.png", gr_WRendering, fGroupLayout))->setToolTip(QStringLiteral("4"));

    m_pExplodeWidget->setGroupFilter(actionGroupFilter);
    QObject::connect(actionGroupFilter, SIGNAL(triggered(QAction*)), m_pExplodeWidget, SLOT(slotRenderTriggered(QAction*)));


    return gr_WRendering;
}

QGroupBox* ExplodeManager::createCuttingGroupBox()
{
    gr_WCutting = createGroupBox(u8"Режим сечения", true, false, true);
    gr_WCutting->setVisible(false);

    QVBoxLayout* mainVlayu = new QVBoxLayout(gr_WCutting);
    QHBoxLayout* fGroupLayout = new QHBoxLayout();
    mainVlayu->addLayout(fGroupLayout);
    QVBoxLayout* vGroupLayoutColors = new QVBoxLayout();
    QVBoxLayout* vGroupLayoutColorsSecond = new QVBoxLayout();
    fGroupLayout->addLayout(vGroupLayoutColors);
    fGroupLayout->addLayout(vGroupLayoutColorsSecond);

    QCheckBox* enable = new QCheckBox(QStringLiteral("Сечение"));
    enable->setChecked(true);
    enable->setMinimumWidth(100);

    QCheckBox* invert = new QCheckBox();
    invert->setText(QStringLiteral("Сменить направление"));
    invert->setChecked(false);

    QCheckBox* closed = new QCheckBox();
    closed->setText(QStringLiteral("Закрытые грани"));
    closed->setChecked(true);

    vGroupLayoutColors->addWidget(enable);
    vGroupLayoutColors->addWidget(invert);
    vGroupLayoutColors->addWidget(closed);

    // check changing section plane with controller
    QCheckBox* controller = new QCheckBox();
    controller->setText(QStringLiteral("Режим значений"));
    controller->setChecked(true);

    // check changing section plane with controller by free section placement
    m_PfreeSection = new QRadioButton();
    m_PfreeSection->setText(QStringLiteral("Свободное перемещение"));
    m_PfreeSection->setChecked(false);
    m_PfreeSection->setEnabled(controller->isChecked());

    // check changing section plane with controller by offset section plane
    m_PoffsetSection = new QRadioButton();
    m_PoffsetSection->setText(QStringLiteral("Фиксированное перемещение"));
    m_PoffsetSection->setChecked(true);
    m_PoffsetSection->setEnabled(controller->isChecked());

    vGroupLayoutColorsSecond->addWidget(controller);
    vGroupLayoutColorsSecond->addWidget(m_PfreeSection);
    vGroupLayoutColorsSecond->addWidget(m_PoffsetSection);


    QObject::connect(enable, SIGNAL(stateChanged(int)), m_pExplodeWidget, SLOT(enableChanged(int)));
    QObject::connect(invert, SIGNAL(stateChanged(int)), m_pExplodeWidget, SLOT(invertChanged(int)));
    QObject::connect(closed, SIGNAL(stateChanged(int)), m_pExplodeWidget, SLOT(closeChanged(int)));

    QObject::connect(m_PfreeSection, SIGNAL(toggled(bool)), m_pExplodeWidget, SLOT(freeSectionChanged(bool)));
    QObject::connect(m_PoffsetSection, SIGNAL(toggled(bool)), m_pExplodeWidget, SLOT(offsetSectionChanged(bool)));

    QHBoxLayout* horizontalLayu = new QHBoxLayout();
    mainVlayu->addLayout(horizontalLayu);
    QVBoxLayout* verticales = new QVBoxLayout();
    horizontalLayu->addLayout(verticales);

    // position offset of section plane
    m_Poffset = new QDoubleSpinBox();
    m_Poffset->setRange(-1000, 1000);
    m_Poffset->setValue(-30);
    m_Poffset->setSingleStep(10.0);
    m_Poffset->setEnabled(false);

    // first angle of section plane direction
    m_Pa1 = new QDoubleSpinBox();
    m_Pa1->setRange(0, 359);
    m_Pa1->setValue(90);
    m_Pa1->setSingleStep(5.0);
    m_Pa1->setEnabled(false);

    // second angle of section plane direction
    m_Pa2 = new QDoubleSpinBox();
    m_Pa2->setRange(0, 359);
    m_Pa2->setValue(90);
    m_Pa2->setSingleStep(5.0);
    m_Pa2->setEnabled(false);
    
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow(QStringLiteral("Угол 1"), m_Pa1);
    formLayout->addRow(QStringLiteral("Угол 2"), m_Pa2);
    formLayout->addRow(QStringLiteral("Смещение"), m_Poffset);

    verticales->addLayout(formLayout);

    QObject::connect(m_Poffset, SIGNAL(valueChanged(double)), m_pExplodeWidget, SLOT(offsetChanged(double)));
    QObject::connect(m_Pa1, SIGNAL(valueChanged(double)), m_pExplodeWidget, SLOT(angleA1Changed(double)));
    QObject::connect(m_Pa2, SIGNAL(valueChanged(double)), m_pExplodeWidget, SLOT(angleA2Changed(double)));
    
    QObject::connect(controller, SIGNAL(stateChanged(int)), m_pExplodeWidget, SLOT(controllerChanged(int)));

    return gr_WCutting;
}

QGroupBox* ExplodeManager::createDimensionsGroupBox()
{
    gr_WDimensionBox = createGroupBox(u8"Проставление размеров", true, false, true);
    gr_WDimensionBox->setVisible(false);

    QHBoxLayout* hLayout = new QHBoxLayout(gr_WDimensionBox);
    hLayout->setMargin(10); hLayout->setSpacing(0);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(vLayout);
    QFormLayout* buttonLayout = new QFormLayout();
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);

    QHBoxLayout* rowLayout1 = new QHBoxLayout();
    QToolButton* selectButton = new QToolButton(gr_WDimensionBox);
    selectButton->setCheckable(true);
    selectButton->setIconSize(sizeIcons);
    selectButton->setIcon(QIcon(":/res/drawing/select_32.png"));
    rowLayout1->addWidget(selectButton);
    QObject::connect(selectButton, SIGNAL(released()), m_pExplodeWidget, SLOT(stopBuildDimensions()));
    QObject::connect(m_pExplodeWidget, SIGNAL(setCheckedSelect(bool)), selectButton, SLOT(setChecked(bool)));

    QToolButton* linearButton = new QToolButton(gr_WDimensionBox);
    linearButton->setCheckable(true);
    linearButton->setIconSize(sizeIcons);
    linearButton->setIcon(QIcon(":/res/drawing/lineardim.png"));
    rowLayout1->addWidget(linearButton);
    QObject::connect(linearButton, SIGNAL(released()), m_pExplodeWidget, SLOT(createLinearDimensions()));
    QObject::connect(m_pExplodeWidget, SIGNAL(setCheckedLinear(bool)), linearButton, SLOT(setChecked(bool)));

    QToolButton* angleButton = new QToolButton(gr_WDimensionBox);
    angleButton->setCheckable(true);
    angleButton->setIconSize(sizeIcons);
    angleButton->setIcon(QIcon(":/res/drawing/angledim.png"));
    rowLayout1->addWidget(angleButton);
    QObject::connect(angleButton, SIGNAL(released()), m_pExplodeWidget, SLOT(createAngleDimensions()));
    QObject::connect(m_pExplodeWidget, SIGNAL(setCheckedAngle(bool)), angleButton, SLOT(setChecked(bool)));

    QToolButton* diameterButton = new QToolButton(gr_WDimensionBox);
    diameterButton->setCheckable(true);
    diameterButton->setIconSize(sizeIcons);
    diameterButton->setIcon(QIcon(":/res/drawing/diameter.png"));
    rowLayout1->addWidget(diameterButton);
    QObject::connect(diameterButton, SIGNAL(released()), m_pExplodeWidget, SLOT(createDiameterDimensions()));
    QObject::connect(m_pExplodeWidget, SIGNAL(setCheckedDiameter(bool)), diameterButton, SLOT(setChecked(bool)));

    QToolButton* radialButton = new QToolButton(gr_WDimensionBox);
    radialButton->setCheckable(true);
    radialButton->setIconSize(sizeIcons);
    radialButton->setIcon(QIcon(":/res/drawing/radius.png"));
    rowLayout1->addWidget(radialButton);
    QObject::connect(radialButton, SIGNAL(released()), m_pExplodeWidget, SLOT(createRadialDimensions()));
    QObject::connect(m_pExplodeWidget, SIGNAL(setCheckedRadial(bool)), radialButton, SLOT(setChecked(bool)));

    rowLayout1->setMargin(5);
    buttonLayout->addRow(rowLayout1);
    vLayout->addLayout(buttonLayout);

    m_pExplodeWidget->setMainLayout(vLayout);
    m_pExplodeWidget->updateButtons();

    return gr_WDimensionBox;
}

QTabWidget* ExplodeManager::createTabWidget(QWidget& widget, const int heightButton, const std::string& mainTabName)
{
    m_pWidget = &widget;
    m_heightButton = heightButton;
    m_mainTabWidget = new QTabWidget();

    m_mainTabWidget->addTab(new QWidget(), m_mainTabName.c_str());
    m_mainTabWidget->addTab(new QWidget(), u8"Параметризация");

    m_vLayoutTab = new QVBoxLayout();
    m_vLayoutTab->setSpacing(2);

    m_vLayoutConfigureTab = new QVBoxLayout();
    m_vLayoutConfigureTab->setSpacing(2);

    m_vLayoutTab->setContentsMargins(4, 2, 4, 2);

    m_mainTabWidget->currentWidget()->setLayout(m_vLayoutTab);
    m_mainTabWidget->widget(1)->setLayout(m_vLayoutConfigureTab);

    m_vLayoutTab->setSizeConstraint(QLayout::SetMinimumSize);
    QObject::connect(m_mainTabWidget, &QTabWidget::currentChanged, this, &ExplodeManager::tabWidgetCurrentChanged);
    QObject::connect(m_mainTabWidget, &QTabWidget::tabCloseRequested, this, &ExplodeManager::tabWidgetCloseRequested);

    m_quantityCombobox = createCombobox(m_vLayoutConfigureTab);
    m_quantityCombobox->addItem(u8"Одиночный", 0);
    m_quantityCombobox->addItem(u8"Блок. Два элемента", 1);
    m_quantityCombobox->addItem(u8"Блок. Три элемента", 2);
    m_quantityCombobox->addItem(u8"Блок. Четыре элемента", 3);

    m_comboConfigure = createCombobox(m_vLayoutConfigureTab);

    QHBoxLayout* hLayoutTypeRadioBox = new QHBoxLayout();
    hLayoutTypeRadioBox->setContentsMargins(0, 10, 0, 10);
    m_vLayoutConfigureTab->addLayout(hLayoutTypeRadioBox);

    QRadioButton* techicalSizesRadioItem = createTypeRadioButton(hLayoutTypeRadioBox, u8"Построение по размерам ТУ", true, 1);
    QRadioButton* manualSizesRadioItem = createTypeRadioButton(hLayoutTypeRadioBox, u8"Построение вручную", false, 2);

    // Группа параметров для расчёта толщины стенок.
    m_warmParams = createGroupBox(u8"Параметры", false, false, true);
    m_warmParams->setDisabled(true);
    m_vLayoutConfigureTab->addWidget(m_warmParams);

    m_vLayoutWarmParams = createVBoxLayout(m_warmParams);

    QToolButton* sizeInfoButton = new QToolButton();
    sizeInfoButton->setIcon(QIcon(":/res/dimensions.png"));
    sizeInfoButton->setToolTip(u8"Посмотреть размеры");
    m_vLayoutConfigureTab->addWidget(sizeInfoButton);

    QDialog* modalDialog = new QDialog();

    connect(sizeInfoButton, &QToolButton::clicked, [modalDialog]() {
        modalDialog->setWindowTitle(u8"Размеры теплообменника ТТРМ");
        modalDialog->setWindowIcon(QIcon(":/res/dimensions.png"));

        QLabel* imageLabel = new QLabel;
        QPixmap image(":/res/draws/ttrm.png");
        imageLabel->setPixmap(image);
        imageLabel->setScaledContents(true); // Масштабировать изображение по размеру QLabel

        QRect screenGeometry = modalDialog->geometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();

        imageLabel->setMaximumSize(screenWidth, screenHeight);

        QVBoxLayout* modalLayout = new QVBoxLayout(modalDialog);
        modalLayout->addWidget(imageLabel);

        modalDialog->exec();
    });

    m_reconfigureButton = new QPushButton;
    m_reconfigureButton->setText(u8"Перестроить теплообменник");
    connect(m_reconfigureButton, &QPushButton::clicked, this, &ExplodeManager::onReconfigureButtonClicked);

    m_vLayoutConfigureTab->addWidget(m_reconfigureButton);

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

    m_vLayoutTab->addWidget(createExplodingGroupBox());
    m_vLayoutTab->addWidget(createFilterGroupBox());
    m_vLayoutTab->addWidget(createSelectionGroupBox());
    m_vLayoutTab->addWidget(createRenderingGroupBox());
    m_vLayoutTab->addWidget(createCuttingGroupBox());
    m_vLayoutTab->addWidget(createDimensionsGroupBox());

    return m_mainTabWidget;
}