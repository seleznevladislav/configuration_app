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
#include <QWindow>


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
        /*
        isCheckedManualType = type == 2 && (m_pExplodeWidget->m_pCurrentExchandger == 1 || m_pExplodeWidget->m_pCurrentExchandger == 2);
        
        m_comboConfigure->setDisabled(isCheckedManualType);
        m_warmParams->setDisabled(!isCheckedManualType);
        if (m_pExplodeWidget->m_pCurrentExchandger == 2) {
            m_reconfigureButton->setDisabled(isCheckedManualType);
        }
        */

        isCheckedManualType = type == 2;

        m_comboConfigure->setDisabled(isCheckedManualType);
        m_warmParams->setDisabled(!isCheckedManualType);
        if (m_pExplodeWidget->m_pCurrentExchandger != 1) {
            m_reconfigureButton->setDisabled(isCheckedManualType);
        }
     
    }
    if (type == 2 && m_pExplodeWidget->m_pCurrentExchandger == 1) 
    {
        const int index = m_comboConfigure->currentIndex();
        BuildParamsForHeatExchangerTTOR config = dataTTOR[index > 0 ? index : 0];

        diametrTubeTTOR->setValue(config.ttDiam);
        diametrKozhuxTTOR->setValue(config.ktDiam);
        thicknessTubeTTOR->setValue(config.ttThickness);
        thicknessKozhuxTTOR->setValue(config.ktThickness);
        distanceOporiTTOR->setValue(config.l0);
        dlinaKameriTTOR->setValue(config.l3);
        visotaOporyH1TTOR->setValue(config.H1);
        visotaOporyH2TTOR->setValue(config.widthFlanec);
        LengthTeplTubeTTOR->setValue(config.Lt);
        LengthKozhuxTubeTTOR->setValue(config.lK);
        distanceConnectorTTOR->setValue(config.l2);
        dUForFlanecTTOR->setValue(config.dU);
    }
    if (type == 1 && m_pExplodeWidget->m_pCurrentExchandger == 1) 
    {
        diametrTubeTTOR->setValue(0);
        diametrKozhuxTTOR->setValue(0);
        thicknessTubeTTOR->setValue(0);
        thicknessKozhuxTTOR->setValue(0);
        distanceOporiTTOR->setValue(0);
        dlinaKameriTTOR->setValue(0);
        visotaOporyH1TTOR->setValue(0);
        visotaOporyH2TTOR->setValue(0);
        LengthTeplTubeTTOR->setValue(0);
        LengthKozhuxTubeTTOR->setValue(0);
        distanceConnectorTTOR->setValue(0);
        dUForFlanecTTOR->setValue(0);
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

ConfigParams ExplodeManager::findClosestMatch(int dimCamera, int param, const std::string& structName) {
    vector<ConfigParams> filteredElements;

    // Фильтрация элементов по диаметру камеры
    for (const ConfigParams& element : dataTTRM) {
        if (element.assortmentCamera == dimCamera) {
            filteredElements.push_back(element);
        }
    }

    filteredElements = dimCamera == 0 ? dataTTRM : filteredElements;

    unordered_map<string, function<int(const ConfigParams&)>> paramAccess = {
        {"LENGTH", [](const ConfigParams& e) { return e.LENGTH; }},
        {"thicknessOuterTubes", [](const ConfigParams& e) { return e.thicknessOuterTubes; }},
        {"assortmentCamera", [](const ConfigParams& e) { return e.assortmentCamera; }},
    };

    // Поиск элемента, четвертый элемент которого ближе всего ко второму параметру
    ConfigParams closestElement = filteredElements[0];
    int minDifference = abs(paramAccess[structName](filteredElements[0]) - param);

    for (const ConfigParams& element : filteredElements) {
        int difference = abs(paramAccess[structName](element) - param);

        if (difference < minDifference) {
            closestElement = element;
            minDifference = difference;
        }
    }

    return closestElement;
}

void ExplodeManager::calculateThickness(QFormLayout* form) {
    // Здесь выполняется ваш расчет
    // Например, предположим, что вы вычислили значения толщины
    QComboBox* steelComboBox = qobject_cast<QComboBox*>(form->itemAt(1)->widget());
    QComboBox* pressureInnerComboBox = qobject_cast<QComboBox*>(form->itemAt(3)->widget());
    QComboBox* pressureOuterComboBox = qobject_cast<QComboBox*>(form->itemAt(5)->widget());
    QComboBox* dimOuterComboBox = qobject_cast<QComboBox*>(form->itemAt(7)->widget());
    QComboBox* dimCameraComboBox = qobject_cast<QComboBox*>(form->itemAt(9)->widget());

    int steelMark = steelComboBox->currentData().toInt();
    float pressureInner = pressureInnerComboBox->currentData().toFloat();
    float pressureOuter = pressureOuterComboBox->currentData().toFloat();
    mp_dimOuterTube = dimOuterComboBox->currentData().toInt();

    QPair<int, int> cameraValues = dimCameraComboBox->currentData().value<QPair<int, int>>();
    mp_dimCamera = cameraValues.first;
    int cameraHeight = cameraValues.second;

    int thicknessOuter = ceil(
        (pressureOuter * mp_dimOuterTube) / 
        (2 * 0.9 * steelMark - pressureOuter) + 2.5);

    mp_thicknessOuterResult = max(thicknessOuter, 4);

    ConfigParams foundElement = findClosestMatch(mp_dimCamera, mp_thicknessOuterResult, "thicknessOuterTubes");

    int thicknessInner = ceil((pressureInner * foundElement.assortmentInnerTubes) / (2 * 0.9 * steelMark - pressureInner) + 2.5);
    mp_thicknessInnerResult = max(thicknessInner, 4);

    const int R = pow(mp_dimCamera, 2) / (4 * cameraHeight);

    int thicknessCamera = ceil(
        (pressureOuter * R) / 
        (2 * steelMark * 0.9 - 0.5 * pressureOuter) + 2.5);

    mp_thicknessCameraResult = max(thicknessCamera, 4);

    QLineEdit* innerTubesLineEdit = qobject_cast<QLineEdit*>(form->itemAt(12)->widget());
    innerTubesLineEdit->setText(QString::number(mp_thicknessInnerResult));

    QLineEdit* outerTubesLineEdit = qobject_cast<QLineEdit*>(form->itemAt(14)->widget());
    outerTubesLineEdit->setText(QString::number(mp_thicknessOuterResult));

    QLineEdit* thicknessGridsLineEdit = qobject_cast<QLineEdit*>(form->itemAt(16)->widget());
    thicknessGridsLineEdit->setText(QString::number(mp_thicknessCameraResult));

    m_lengthSpinBox->setDisabled(false);
    m_closestLengthButton->setDisabled(false);
    m_reconfigureButton->setDisabled(false);
}

void ExplodeManager::calculateThickness_IU_IP(QFormLayout* form) {
    QComboBox* pressureComboBox =
        qobject_cast<QComboBox*>(form->itemAt(1)->widget());
    QDoubleSpinBox* innerThicknessSpinBox =
        qobject_cast<QDoubleSpinBox*>(form->itemAt(3)->widget());
    QDoubleSpinBox* tensionSpinBox =
        qobject_cast<QDoubleSpinBox*>(form->itemAt(13)->widget());
    QComboBox* corrosionComboBox =
        qobject_cast<QComboBox*>(form->itemAt(15)->widget());


    float pressure = pressureComboBox->currentData().toFloat();//давление
    float Dy = innerThicknessSpinBox->value();//диаметр обечайки
    float tension = tensionSpinBox->value();//допускаемое напряжение
    int corrosion = corrosionComboBox->currentData().toInt();//поправка на коррозию

    float h = Dy / 4;//высота выпуклой части днища
    float d0 = 200;//диаметр отверстия на элл днище
    float z = 1 - d0 / Dy;//коэффициент неукрепленного отверстия

    mp_IU_s = ceil(pressure * Dy / (2 * 0.9 * tension - pressure) + corrosion);
    mp_IU_s_ell = ceil(pressure * Dy / (4 * z * tension - pressure) + Dy / (2 * h) + corrosion);

    QLineEdit* thicknessInnerKzh =
        qobject_cast<QLineEdit*>(form->itemAt(19)->widget());
    thicknessInnerKzh->setText(QString::number(mp_IU_s));

    QLineEdit* thicknessElliptic =
        qobject_cast<QLineEdit*>(form->itemAt(21)->widget());
    thicknessElliptic->setText(QString::number(mp_IU_s_ell));
}

QFormLayout* ExplodeManager::createParametrizationForm(const int numberOfHeatExchanger, QVBoxLayout* layout)
{
    QFormLayout* formLayout = new QFormLayout;
    formLayout->setObjectName("warmForm");

    switch (numberOfHeatExchanger)
    {
    case 1:
    {
        m_reconfigureButton->setDisabled(false);

        turnOnStandartDetailsTTOR = new QCheckBox;
        QLabel* turnOnStandartDetailsLabel = new QLabel(u8"Отобразить станд. изделия:");

        simpleModeStandartTTOR = new QCheckBox;
        QLabel* simpleModeStandartLabel = new QLabel(u8"Упростить станд. изделия:");
        
        diametrTubeTTOR = new QDoubleSpinBox;
        diametrTubeTTOR->setMaximum(1000);
        QLabel* diametrTubeLabel = new QLabel(u8"Диаметр трубы, d1 (мм):");

        diametrKozhuxTTOR = new QDoubleSpinBox;
        diametrKozhuxTTOR = new QDoubleSpinBox;
        diametrKozhuxTTOR->setMaximum(1000);
        QLabel* diametrKozhuxLabel = new QLabel(u8"Диаметр кожуха, d2 (мм):");

        thicknessTubeTTOR = new QDoubleSpinBox;
        thicknessTubeTTOR->setMaximum(1000);
        QLabel* thicknessTubeLabel = new QLabel(u8"Толщина стенки трубы, dt1 (мм):");

        thicknessKozhuxTTOR = new QDoubleSpinBox;
        thicknessKozhuxTTOR->setMaximum(1000);
        QLabel* thicknessKozhuxLabel = new QLabel(u8"Толщина стенки кожуха, dt2 (мм):");

        LengthTeplTubeTTOR = new QDoubleSpinBox;
        LengthTeplTubeTTOR->setMaximum(10000);
        QLabel* LengthTeplTubeLabel = new QLabel(u8"Длина теплообменной, Lt (мм):");

        LengthKozhuxTubeTTOR = new QDoubleSpinBox;
        LengthKozhuxTubeTTOR->setMaximum(10000);
        QLabel* LengthKozhuxTubeLabel = new QLabel(u8"Длина кожухуха, Lk (мм):");

        distanceOporiTTOR = new QDoubleSpinBox;
        distanceOporiTTOR->setMaximum(10000);
        QLabel* distanceOporiLabel = new QLabel(u8"Растояние опоры, l0 (мм):");
        
        distanceConnectorTTOR = new QDoubleSpinBox;
        distanceConnectorTTOR->setMaximum(10000);
        QLabel* distanceConnectorLabel = new QLabel(u8"Растояние соединения от опоры, l2 (мм):");
        
        dlinaKameriTTOR = new QDoubleSpinBox;
        dlinaKameriTTOR->setMaximum(10000);
        QLabel* dlinaKameriLabel = new QLabel(u8"Длина камеры, l3 (мм):"); 
        
        dUForFlanecTTOR = new QDoubleSpinBox;
        dUForFlanecTTOR->setMaximum(10000);
        QLabel* dUForFlanecLabel = new QLabel(u8"Диаметр пропуска, dU (мм):");

        visotaOporyH1TTOR = new QDoubleSpinBox;
        visotaOporyH1TTOR->setMaximum(10000);
        QLabel* visotaOporyH1Label = new QLabel(u8"Высота опоры, H1*2 (мм):");
        
        visotaOporyH2TTOR = new QDoubleSpinBox;
        visotaOporyH2TTOR->setMaximum(10000);
        QLabel* visotaOporyH2Label = new QLabel(u8"Ширина опоры, H2*2 (мм):");

        // Creating vertical layouts for each pair of QLabel and QDoubleSpinBox
        QHBoxLayout* turnOnStandartDetailsLayout = new QHBoxLayout;
        turnOnStandartDetailsLayout->addWidget(turnOnStandartDetailsLabel);
        turnOnStandartDetailsLayout->addWidget(turnOnStandartDetailsTTOR);

        QHBoxLayout* simpleModeStandartLayout = new QHBoxLayout;
        simpleModeStandartLayout->addWidget(simpleModeStandartLabel);
        simpleModeStandartLayout->addWidget(simpleModeStandartTTOR);

        QVBoxLayout* diametrTubeLayout = new QVBoxLayout;
        diametrTubeLayout->addWidget(diametrTubeLabel);
        diametrTubeLayout->addWidget(diametrTubeTTOR);

        QVBoxLayout* diametrKozhuxLayout = new QVBoxLayout;
        diametrKozhuxLayout->addWidget(diametrKozhuxLabel);
        diametrKozhuxLayout->addWidget(diametrKozhuxTTOR);

        QVBoxLayout* thicknessTubeLayout = new QVBoxLayout;
        thicknessTubeLayout->addWidget(thicknessTubeLabel);
        thicknessTubeLayout->addWidget(thicknessTubeTTOR);

        QVBoxLayout* thicknessKozhuxLayout = new QVBoxLayout;
        thicknessKozhuxLayout->addWidget(thicknessKozhuxLabel);
        thicknessKozhuxLayout->addWidget(thicknessKozhuxTTOR);

        QVBoxLayout* LengthTeplTubeLayout = new QVBoxLayout;
        LengthTeplTubeLayout->addWidget(LengthTeplTubeLabel);
        LengthTeplTubeLayout->addWidget(LengthTeplTubeTTOR);

        QVBoxLayout* LengthKozhuxTubeLayout = new QVBoxLayout;
        LengthKozhuxTubeLayout->addWidget(LengthKozhuxTubeLabel);
        LengthKozhuxTubeLayout->addWidget(LengthKozhuxTubeTTOR);

        QVBoxLayout* distanceOporiLayout = new QVBoxLayout;
        distanceOporiLayout->addWidget(distanceOporiLabel);
        distanceOporiLayout->addWidget(distanceOporiTTOR);

        QVBoxLayout* distanceConnectorLayout = new QVBoxLayout;
        distanceConnectorLayout->addWidget(distanceConnectorLabel);
        distanceConnectorLayout->addWidget(distanceConnectorTTOR);

        QVBoxLayout* dlinaKameriLayout = new QVBoxLayout;
        dlinaKameriLayout->addWidget(dlinaKameriLabel);
        dlinaKameriLayout->addWidget(dlinaKameriTTOR);

        QVBoxLayout* dUForFlanecLayout = new QVBoxLayout;
        dUForFlanecLayout->addWidget(dUForFlanecLabel);
        dUForFlanecLayout->addWidget(dUForFlanecTTOR);
        
        QVBoxLayout* visotaOporyH1Layout = new QVBoxLayout;
        visotaOporyH1Layout->addWidget(visotaOporyH1Label);
        visotaOporyH1Layout->addWidget(visotaOporyH1TTOR);

        QVBoxLayout* visotaOporyH2Layout = new QVBoxLayout;
        visotaOporyH2Layout->addWidget(visotaOporyH2Label);
        visotaOporyH2Layout->addWidget(visotaOporyH2TTOR);

        // Creating horizontal layouts for grouping
        QHBoxLayout* checkBoxLayout = new QHBoxLayout;
        checkBoxLayout->addLayout(turnOnStandartDetailsLayout);
        checkBoxLayout->addLayout(simpleModeStandartLayout);
        checkBoxLayout->setMargin(10);

        QHBoxLayout* diameterLayout = new QHBoxLayout;
        diameterLayout->addLayout(diametrTubeLayout);
        diameterLayout->addLayout(diametrKozhuxLayout);
        diameterLayout->setContentsMargins(0, 5, 0, 0);
        diameterLayout->setSpacing(5);

        QHBoxLayout* thicknessLayout = new QHBoxLayout;
        thicknessLayout->addLayout(thicknessTubeLayout);
        thicknessLayout->addLayout(thicknessKozhuxLayout);
        thicknessLayout->setContentsMargins(0, 5, 0, 0);
        thicknessLayout->setSpacing(5);

        QHBoxLayout* lengthLayout = new QHBoxLayout;
        lengthLayout->addLayout(LengthTeplTubeLayout);
        lengthLayout->addLayout(LengthKozhuxTubeLayout);
        lengthLayout->setContentsMargins(0, 5, 0, 0);
        lengthLayout->setSpacing(5);

        QHBoxLayout* distanceLayout = new QHBoxLayout;
        distanceLayout->addLayout(distanceOporiLayout);
        distanceLayout->addLayout(distanceConnectorLayout);
        distanceLayout->setContentsMargins(0, 5, 0, 0);
        distanceLayout->setSpacing(5);

        QHBoxLayout* lastElementsLayout = new QHBoxLayout;
        lastElementsLayout->addLayout(dlinaKameriLayout);
        lastElementsLayout->addLayout(dUForFlanecLayout);
        lastElementsLayout->setContentsMargins(0, 5, 0, 0);
        lastElementsLayout->setSpacing(5);
        
        QHBoxLayout* lasLastElementsLayout = new QHBoxLayout;
        lasLastElementsLayout->addLayout(visotaOporyH1Layout);
        lasLastElementsLayout->addLayout(visotaOporyH2Layout);
        lasLastElementsLayout->setContentsMargins(0, 5, 0, 0);
        lasLastElementsLayout->setSpacing(5);

        // Adding horizontal layouts to the form layout
        formLayout->addRow(checkBoxLayout);
        formLayout->addRow(diameterLayout);
        formLayout->addRow(thicknessLayout);
        formLayout->addRow(lengthLayout);
        formLayout->addRow(distanceLayout);
        formLayout->addRow(lastElementsLayout);
        formLayout->addRow(lasLastElementsLayout);

        break;
    }
    case 2:
    {
        QLabel* steelLabel = new QLabel(u8"Марка металла аппарата:");
        QLabel* pressureInnerLabel = new QLabel(u8"Рабочие давление в трубе:");
        QLabel* pressureOuterLabel = new QLabel(u8"Рабочие давление в кожухе:");

        QLabel* dimYLabel = new QLabel(u8"Внешний диаметр кожуха, Dy:");
        QLabel* dimCameraLabel = new QLabel(u8"Диаметр камеры, D:");

        QComboBox* steelComboBox = new QComboBox;
        steelComboBox->addItem(u8"Вст.3", 140);
        steelComboBox->addItem(u8"20, 20К", 147);
        steelComboBox->addItem(u8"О9Г2С, 16ГС, 17ГС, 10Г2С1", 183);
        steelComboBox->addItem(u8"10Г2", 180);
        steelComboBox->addItem(u8"12ХМ", 147);
        steelComboBox->addItem(u8"12МX", 147);
        steelComboBox->addItem(u8"15ХМ", 155);
        steelComboBox->addItem(u8"15Х5М", 146);
        steelComboBox->addItem(u8"15Х5М-У", 240);
        steelComboBox->addItem(u8"08Х22Н6Т, 08Х21Н6М2Т", 240);
        steelComboBox->addItem(u8"03Х21Н21М4ГБ", 80);
        steelComboBox->addItem(u8"03Х18Н11", 160);
        steelComboBox->addItem(u8"03Х16Н15М3", 153);
        steelComboBox->addItem(u8"06ХН28МДТ, 03ХН28МДТ", 147);

        QComboBox* pressureInnerComboBox = new QComboBox;
        pressureInnerComboBox->addItem("6,3", 6.3);
        pressureInnerComboBox->addItem("10", 10);
        pressureInnerComboBox->addItem("16", 16);


        QComboBox* pressureOuterComboBox = new QComboBox;
        pressureOuterComboBox->addItem("1,6", 1.6);
        pressureOuterComboBox->addItem("4,0", 4.0);
        pressureOuterComboBox->addItem("6,3", 6.3);
        pressureOuterComboBox->addItem("10,0", 10.0);

        QComboBox* dimYComboBox = new QComboBox;
        dimYComboBox->addItem("57", 57);
        dimYComboBox->addItem("76", 76);
        dimYComboBox->addItem("89", 89);
        dimYComboBox->addItem("108", 108);

        QComboBox* dimCameraComboBox = new QComboBox;
        dimCameraComboBox->addItem("219", QVariant::fromValue(QPair<int, int>(219, 55)));
        dimCameraComboBox->addItem("273", QVariant::fromValue(QPair<int, int>(273, 68)));
        dimCameraComboBox->addItem("325", QVariant::fromValue(QPair<int, int>(325, 81)));

        formLayout->addRow(steelLabel, steelComboBox);
        formLayout->addRow(pressureInnerLabel, pressureInnerComboBox);
        formLayout->addRow(pressureOuterLabel, pressureOuterComboBox);
        formLayout->addRow(dimYLabel, dimYComboBox);
        formLayout->addRow(dimCameraLabel, dimCameraComboBox);

        QPushButton* calculateThicknessButton = new QPushButton;
        calculateThicknessButton->setText(u8"Расчитать толщину стенок");
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

        m_lengthSpinBox = new QDoubleSpinBox;
        m_lengthSpinBox->setDisabled(true);
        m_lengthSpinBox->setRange(2150, 7040);
        m_lengthSpinBox->setSingleStep(50);
        m_lengthSpinBox->setValue(2150);

        QLabel* lengthSpinBoxLabel = new QLabel(u8"Длина L, мм:");

        m_closestLengthButton = new QToolButton();
        m_closestLengthButton->setDisabled(true);
        m_closestLengthButton->setIcon(QIcon(":/res/arrow.png"));
        m_closestLengthButton->setToolTip(u8"Ближайший стандартный размер");

        QWidget* widgetContainer = new QWidget;
        QHBoxLayout* hLayout = new QHBoxLayout(widgetContainer);
        hLayout->addWidget(m_lengthSpinBox);
        hLayout->addWidget(m_closestLengthButton);
        hLayout->setContentsMargins(0, 0, 0, 0);

        formLayout->addRow(lengthSpinBoxLabel, widgetContainer);


        layout->addLayout(formLayout);

        connect(calculateThicknessButton, &QPushButton::clicked, [=]() {
            calculateThickness(formLayout);
            });

        connect(m_closestLengthButton, &QToolButton::clicked, [=]() {
            ConfigParams foundElement = findClosestMatch(mp_dimCamera, m_lengthSpinBox->value(), "LENGTH");

            m_lengthSpinBox->setValue(foundElement.LENGTH);
            });

        break;
    }
    case 3:
    {
        QComboBox* pressureComboBox = new QComboBox;
        pressureComboBox->addItem(u8"1.6", 1.6);
        pressureComboBox->addItem(u8"2.5", 2.5);
        //pressureComboBox->addItem(u8"1.6", 4);

        QLabel* pressureSpinBoxLabel = new QLabel(u8"Давление P, МПа:");

        QDoubleSpinBox* tensionSpinBox = new QDoubleSpinBox;
        QLabel* tensionSpinBoxLabel = new QLabel(u8"Допускаемое напряжение стали, МПа");
        tensionSpinBox->setRange(50, 1000);//временно

        QComboBox* corrosionComboBox = new QComboBox;
        corrosionComboBox->addItem(u8"1", 1);
        corrosionComboBox->addItem(u8"2", 2);
        corrosionComboBox->addItem(u8"3", 3);
        corrosionComboBox->addItem(u8"4", 4);
        corrosionComboBox->addItem(u8"5", 5);
        QLabel* corrosionComboBoxLabel = new QLabel(u8"Поправка на коррозию, мм:");

        QDoubleSpinBox* innerThicknessSpinBox = new QDoubleSpinBox;
        QLabel* innerThicknessSpinBoxLabel = new QLabel(u8"Диаметр кожуха Д_в, мм:");
        innerThicknessSpinBox->setRange(800, 2000);
        innerThicknessSpinBox->setValue(800);

        QDoubleSpinBox* cameraThicknessSpinBox = new QDoubleSpinBox;
        QLabel* cameraThicknessSpinBoxLabel = new QLabel(u8"Диаметр камеры D_K, мм:");
        cameraThicknessSpinBox->setRange(500, 1100);
        cameraThicknessSpinBox->setValue(500);

        QDoubleSpinBox* iSpinBox = new QDoubleSpinBox;
        QLabel* iSpinBoxLabel = new QLabel(u8"l (max: 600), мм");
        iSpinBox->setRange(350, 600);
        iSpinBox->setValue(350);

        QDoubleSpinBox* iSecondSpinBox = new QDoubleSpinBox;
        QLabel* iSecondSpinBoxLabel = new QLabel(u8"l2 (max: 2000), мм");
        iSecondSpinBox->setRange(1750, 2000);
        iSecondSpinBox->setValue(1750);

        QDoubleSpinBox* iThirdSpinBox = new QDoubleSpinBox;
        QLabel* iThirdSpinBoxLabel = new QLabel(u8"l3 (max: 6500), мм");
        iThirdSpinBox->setRange(6390, 6500);
        iThirdSpinBox->setValue(6390);



        formLayout->addRow(pressureSpinBoxLabel, pressureComboBox);
        formLayout->addRow(innerThicknessSpinBoxLabel, innerThicknessSpinBox);
        formLayout->addRow(cameraThicknessSpinBoxLabel, cameraThicknessSpinBox);
        formLayout->addRow(iSpinBoxLabel, iSpinBox);
        formLayout->addRow(iSecondSpinBoxLabel, iSecondSpinBox);
        formLayout->addRow(iThirdSpinBoxLabel, iThirdSpinBox);
        formLayout->addRow(tensionSpinBoxLabel, tensionSpinBox);
        formLayout->addRow(corrosionComboBoxLabel, corrosionComboBox);

        QFrame* line = new QFrame;
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setContentsMargins(0, 10, 0, 10);
        formLayout->addRow(line);

        layout->addLayout(formLayout);


        QPushButton* calculateThicknessButton = new QPushButton;
        calculateThicknessButton->setText(u8"Расчитать толщину стенок");
        calculateThicknessButton->setIcon(QIcon(":/res/calculate.png"));
        calculateThicknessButton->setContentsMargins(0, 5, 0, 5);

        formLayout->addRow(calculateThicknessButton);


        QLineEdit* thicknessInnerKzh = new QLineEdit;
        thicknessInnerKzh->setReadOnly(true);
        QLabel* thicknessInnerKzhLabel = new QLabel(u8"Толщина стенок кожуха, мм:");

        QLineEdit* thicknessElliptic = new QLineEdit;
        thicknessElliptic->setReadOnly(true);
        QLabel* thicknessEllipticLabel = new QLabel(u8"Толщина эллиптического днища, мм:");

        formLayout->addRow(thicknessInnerKzhLabel, thicknessInnerKzh);
        formLayout->addRow(thicknessEllipticLabel, thicknessElliptic);


        connect(calculateThicknessButton, &QPushButton::clicked, [=]() {
            mp_IU_p = pressureComboBox->currentData().toDouble();
            mp_IU_D_Kzh = innerThicknessSpinBox->value();
            mp_IU_D_Kam = cameraThicknessSpinBox->value();
            mp_IU_D_l = iSpinBox->value();
            mp_IU_D_l2 = iSecondSpinBox->value();
            mp_IU_D_l3 = iThirdSpinBox->value();

            if (checkValidate()) {
                calculateThickness_IU_IP(formLayout);
                m_reconfigureButton->setDisabled(false);
            }

        });
        break;
    }
    case 4:
    {
        QComboBox* pressureComboBox = new QComboBox;
        pressureComboBox->addItem(u8"1.6", 1.6);
        pressureComboBox->addItem(u8"2.5", 2.5);
        //pressureComboBox->addItem(u8"1.6", 4);

        QLabel* pressureSpinBoxLabel = new QLabel(u8"Давление P, МПа:");

        QDoubleSpinBox* tensionSpinBox = new QDoubleSpinBox;
        QLabel* tensionSpinBoxLabel = new QLabel(u8"Допускаемое напряжение стали, МПа");
        tensionSpinBox->setRange(50, 1000);//временно

        QComboBox* corrosionComboBox = new QComboBox;
        corrosionComboBox->addItem(u8"1", 1);
        corrosionComboBox->addItem(u8"2", 2);
        corrosionComboBox->addItem(u8"3", 3);
        corrosionComboBox->addItem(u8"4", 4);
        corrosionComboBox->addItem(u8"5", 5);
        QLabel* corrosionComboBoxLabel = new QLabel(u8"Поправка на коррозию, мм:");

        QDoubleSpinBox* innerThicknessSpinBox = new QDoubleSpinBox;
        QLabel* innerThicknessSpinBoxLabel = new QLabel(u8"Диаметр кожуха Dv, мм:");
        innerThicknessSpinBox->setRange(800, 2000);
        innerThicknessSpinBox->setValue(800);

        QDoubleSpinBox* cameraThicknessSpinBox = new QDoubleSpinBox;
        QLabel* cameraThicknessSpinBoxLabel = new QLabel(u8"Диаметр камеры D, мм:");
        cameraThicknessSpinBox->setRange(500, 1100);
        cameraThicknessSpinBox->setValue(500);

        QDoubleSpinBox* iSpinBox = new QDoubleSpinBox;
        QLabel* iSpinBoxLabel = new QLabel(u8"I (max: 600), мм");
        iSpinBox->setRange(350, 600);
        iSpinBox->setValue(350);

        QDoubleSpinBox* iSecondSpinBox = new QDoubleSpinBox;
        QLabel* iSecondSpinBoxLabel = new QLabel(u8"I2 (max: 2000), мм");
        iSecondSpinBox->setRange(1750, 2000);
        iSecondSpinBox->setValue(1750);

        QDoubleSpinBox* iThirdSpinBox = new QDoubleSpinBox;
        QLabel* iThirdSpinBoxLabel = new QLabel(u8"I3 (max: 6500), мм");
        iThirdSpinBox->setRange(6390, 6500);
        iThirdSpinBox->setValue(6390);

        formLayout->addRow(pressureSpinBoxLabel, pressureComboBox);
        formLayout->addRow(innerThicknessSpinBoxLabel, innerThicknessSpinBox);
        formLayout->addRow(cameraThicknessSpinBoxLabel, cameraThicknessSpinBox);
        formLayout->addRow(iSpinBoxLabel, iSpinBox);
        formLayout->addRow(iSecondSpinBoxLabel, iSecondSpinBox);
        formLayout->addRow(iThirdSpinBoxLabel, iThirdSpinBox);
        formLayout->addRow(tensionSpinBoxLabel, tensionSpinBox);
        formLayout->addRow(corrosionComboBoxLabel, corrosionComboBox);

        QFrame* line = new QFrame;
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setContentsMargins(0, 10, 0, 10);
        formLayout->addRow(line);

        layout->addLayout(formLayout);


        QPushButton* calculateThicknessButton = new QPushButton;
        calculateThicknessButton->setText(u8"Расчитать толщину стенок");
        calculateThicknessButton->setIcon(QIcon(":/res/calculate.png"));
        calculateThicknessButton->setContentsMargins(0, 5, 0, 5);

        formLayout->addRow(calculateThicknessButton);

        QLineEdit* thicknessInnerKzh = new QLineEdit;
        thicknessInnerKzh->setReadOnly(true);
        QLabel* thicknessInnerKzhLabel = new QLabel(u8"Толщина стенок кожуха, мм:");

        QLineEdit* thicknessElliptic = new QLineEdit;
        thicknessElliptic->setReadOnly(true);
        QLabel* thicknessEllipticLabel = new QLabel(u8"Толщина эллиптического днища, мм:");

        formLayout->addRow(thicknessInnerKzhLabel, thicknessInnerKzh);
        formLayout->addRow(thicknessEllipticLabel, thicknessElliptic);

        connect(calculateThicknessButton, &QPushButton::clicked, [=]() {

            mp_IP_p = pressureComboBox->currentData().toDouble();
            mp_IP_D_Kzh = innerThicknessSpinBox->value();
            mp_IP_D_Kam = cameraThicknessSpinBox->value();
            mp_IP_D_l = iSpinBox->value();
            mp_IP_D_l2 = iSecondSpinBox->value();
            mp_IP_D_l3 = iThirdSpinBox->value();

            if (checkValidate()) {
                calculateThickness_IU_IP(formLayout);
                m_reconfigureButton->setDisabled(false);
            }

        });

        break;
    }
    }

    layout->addLayout(formLayout);

    return formLayout;
}

QComboBox* ExplodeManager::createCombobox(QVBoxLayout* vLayout)
{   
    QComboBox* combo = new QComboBox(m_pWidget);

    vLayout->addWidget(combo);
    return combo;
}

QPushButton* ExplodeManager::createButton(const ExplodeDispatcher::ControlParameterType param,
    QHBoxLayout* hLayout, 
    const char* text, 
    const char* tip)
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

QMessageBox* ExplodeManager::createWarning(QString* warningText)
{
    QMessageBox* msgBox = new QMessageBox;
    msgBox->setIcon(QMessageBox::Warning);
    msgBox->setText(*warningText);
    msgBox->setWindowTitle(u8"Ошибка");
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->exec();

    return msgBox;
}

QWidget* ExplodeManager::createPairWidget(QWidget* widget1, QWidget* widget2)
{
    QWidget* widgetContainer = new QWidget;
    QHBoxLayout* hLayout = new QHBoxLayout(widgetContainer);
    hLayout->addWidget(widget1);
    hLayout->addWidget(widget2);
   
    hLayout->setContentsMargins(0, 0, 0, 0);

    return widgetContainer;
}

void ExplodeManager::createCalculationTab(const int numberOfHeatExchanger)
{

    if (m_calculationTab) {

        m_mainTabWidget->removeTab(2);
    }

    m_calculationTab = new QWidget();

    m_mainTabWidget->addTab(m_calculationTab, u8"Расчеты");

    switch (numberOfHeatExchanger)
    {
        case 1: //ExplodeWidget::TTOR
        {
            m_vLayoutCalculationTabTTOR = new QVBoxLayout();
            m_vLayoutCalculationTabTTOR->setSpacing(15);

            // Комбобоксы для выбора теплоносителей
            QHBoxLayout* comboTeplHLayout = new QHBoxLayout();
            QVBoxLayout* comboTeplV1Layout = new QVBoxLayout();
            comboTeplV1Layout->setSpacing(5);
            QVBoxLayout* comboTeplV2Layout = new QVBoxLayout();
            comboTeplV2Layout->setSpacing(5);

            comboTeplV1Layout->addWidget(new QLabel(u8"Горячий теплоноситель:"));
            m_PhotFluidComboBox = new QComboBox();
            for (const auto& fluid : fluidsProperties) {
                m_PhotFluidComboBox->addItem(QString::fromStdString(fluid.name));
            }
            comboTeplV1Layout->addWidget(m_PhotFluidComboBox);

            comboTeplV2Layout->addWidget(new QLabel(u8"Холодный теплоноситель:"));
            m_PcoldFluidComboBox = new QComboBox();
            for (const auto& fluid : fluidsProperties) {
                m_PcoldFluidComboBox->addItem(QString::fromStdString(fluid.name));
            }
            comboTeplV2Layout->addWidget(m_PcoldFluidComboBox);

            comboTeplHLayout->addLayout(comboTeplV1Layout);
            comboTeplHLayout->addLayout(comboTeplV2Layout);
            m_vLayoutCalculationTabTTOR->addLayout(comboTeplHLayout);

            // Поля для ввода температур
            QHBoxLayout* tempsHLayout = new QHBoxLayout();
            QVBoxLayout* tempsV1Layout = new QVBoxLayout();
            tempsV1Layout->setSpacing(5);
            QVBoxLayout* tempsV2Layout = new QVBoxLayout();
            tempsV2Layout->setSpacing(5);

            tempsV1Layout->addWidget(new QLabel(u8"Температура горячей \nсреды на входе - t₁\" (°C):"));
            m_PhotInletTemp = new QDoubleSpinBox();
            m_PhotInletTemp->setMaximum(1000);
            m_PhotInletTemp->setValue(300);
            tempsV1Layout->addWidget(m_PhotInletTemp);

            tempsV2Layout->addWidget(new QLabel(u8"Температура холодной \nсреды на входе - t₂\" (°C):"));
            m_PcoldInletTemp = new QDoubleSpinBox();
            m_PcoldInletTemp->setMaximum(1000);
            m_PcoldInletTemp->setValue(200);
            tempsV2Layout->addWidget(m_PcoldInletTemp);

            tempsHLayout->addLayout(tempsV1Layout);
            tempsHLayout->addLayout(tempsV2Layout);
            m_vLayoutCalculationTabTTOR->addLayout(tempsHLayout);
            // Поля для ввода температур
            QHBoxLayout* tempsVihodHLayout = new QHBoxLayout();
            QVBoxLayout* tempsVihodV1Layout = new QVBoxLayout();
            tempsVihodV1Layout->setSpacing(5);
            QVBoxLayout* tempsVihodV2Layout = new QVBoxLayout();
            tempsVihodV2Layout->setSpacing(5);

            tempsVihodV1Layout->addWidget(new QLabel(u8"Температура горячего \nтеплоносителя на выходе - t₁\" (°C):"));
            m_PrresultTemp1 = new QDoubleSpinBox();
            m_PrresultTemp1->setMaximum(1000);
            m_PrresultTemp1->setToolTip(u8"Необходимо указать одно из значений тепературы на выходе");
            tempsVihodV1Layout->addWidget(m_PrresultTemp1);

            tempsVihodV2Layout->addWidget(new QLabel(u8"Температура холодного \nтеплоносителя на выходе - t₂\" (°C):"));
            m_PrresultTemp2 = new QDoubleSpinBox();
            m_PrresultTemp2->setMaximum(1000);
            m_PrresultTemp2->setToolTip(u8"Необходимо указать одно из значений тепературы на выходе");
            tempsVihodV2Layout->addWidget(m_PrresultTemp2);
            

            auto valueChangedLambda = [this](double value) {
                QDoubleSpinBox* senderSpinBox = qobject_cast<QDoubleSpinBox*>(sender());
                if (!senderSpinBox) return;

                if (senderSpinBox == m_PrresultTemp1) {
                    m_PrresultTemp2->setDisabled(value > 0);
                }
                else if (senderSpinBox == m_PrresultTemp2) {
                    m_PrresultTemp1->setDisabled(value > 0);
                }
                };

            connect(m_PrresultTemp1, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, valueChangedLambda);
            connect(m_PrresultTemp2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, valueChangedLambda);


            tempsVihodHLayout->addLayout(tempsVihodV1Layout);
            tempsVihodHLayout->addLayout(tempsVihodV2Layout);
            m_vLayoutCalculationTabTTOR->addLayout(tempsVihodHLayout);
            
            // Поля для ввода скоростей
            QHBoxLayout* speedHLayout = new QHBoxLayout();
            QVBoxLayout* speedV1Layout = new QVBoxLayout();
            speedV1Layout->setSpacing(5);
            QVBoxLayout* speedV2Layout = new QVBoxLayout();
            speedV2Layout->setSpacing(5);

            speedV1Layout->addWidget(new QLabel(u8"Скорость горячей среды - w₁ (м/с):"));
            m_PhotVelocity = new QDoubleSpinBox();
            m_PhotVelocity->setValue(3.2);
            speedV1Layout->addWidget(m_PhotVelocity);

            speedV2Layout->addWidget(new QLabel(u8"Скорость холодной среды - w₂ (м/с):"));
            m_PcoldVelocity = new QDoubleSpinBox();
            m_PcoldVelocity->setValue(2.4);
            speedV2Layout->addWidget(m_PcoldVelocity);

            speedHLayout->addLayout(speedV1Layout);
            speedHLayout->addLayout(speedV2Layout);
            m_vLayoutCalculationTabTTOR->addLayout(speedHLayout);

            // Комбобоксы для Выбора материала

            QHBoxLayout* materialHLayout = new QHBoxLayout();
            materialHLayout->addWidget(new QLabel(u8"Материал труб теплообменника:"));
            m_PmaterialCombobox = new QComboBox();
            for (const auto& fluid : materialProperties) {
                m_PmaterialCombobox->addItem(QString::fromStdString(fluid.name));
            }
            materialHLayout->addWidget(m_PmaterialCombobox);
            m_vLayoutCalculationTabTTOR->addLayout(materialHLayout);

            //  Загрязненная среда
            QHBoxLayout* q1ResultHLayout = new QHBoxLayout();
            QHBoxLayout* h1ResultHLayout = new QHBoxLayout();
            QHBoxLayout* tempsVihodHZagryaznenie = new QHBoxLayout();
            QVBoxLayout* q1V1Layout = new QVBoxLayout();
            q1V1Layout->setSpacing(5);
            QVBoxLayout* q2V1Layout = new QVBoxLayout();
            q2V1Layout->setSpacing(5);
            QVBoxLayout* h1V1Layout = new QVBoxLayout();
            h1V1Layout->setSpacing(5);
            QVBoxLayout* h2V1Layout = new QVBoxLayout();
            h2V1Layout->setSpacing(5);

            turnOnZagryaznenieTTOR = new QCheckBox;
            QLabel* turnOnZagryaznenieTTORLabel = new QLabel(u8"Учитывать загрязнение теплоносителя:");
            tempsVihodHZagryaznenie->addWidget(turnOnZagryaznenieTTORLabel);
            tempsVihodHZagryaznenie->addWidget(turnOnZagryaznenieTTOR);

            q1V1Layout->addWidget(new QLabel(u8"Толщина отложения \nгорячего телоносителя (мм):"));
            m_PresultFieldZagryaznenieQ1 = new QDoubleSpinBox();
            m_PresultFieldZagryaznenieQ1->setDisabled(true);
            q1V1Layout->addWidget(m_PresultFieldZagryaznenieQ1);

            q2V1Layout->addWidget(new QLabel(u8"Толщина отложения \nхолодного телоносителя (мм):"));
            m_PresultFieldZagryaznenieQ2 = new QDoubleSpinBox();
            m_PresultFieldZagryaznenieQ2->setDisabled(true);
            q2V1Layout->addWidget(m_PresultFieldZagryaznenieQ2);

            h1V1Layout->addWidget(new QLabel(u8"Коэф. теплопроводности отложения \nгорячего телоносителя (Вт/(м*К)):"));
            m_PresultFieldZagryaznenieH1 = new QDoubleSpinBox();
            m_PresultFieldZagryaznenieH1->setDisabled(true);
            m_PresultFieldZagryaznenieH1->setMaximum(1000);
            h1V1Layout->addWidget(m_PresultFieldZagryaznenieH1);

            h2V1Layout->addWidget(new QLabel(u8"Коэф. теплопроводности отложения \nхолодного телоносителя (Вт/(м*К)):"));
            m_PresultFieldZagryaznenieH2 = new QDoubleSpinBox();
            m_PresultFieldZagryaznenieH2->setDisabled(true);
            m_PresultFieldZagryaznenieH2->setMaximum(1000);
            h2V1Layout->addWidget(m_PresultFieldZagryaznenieH2);

            m_vLayoutCalculationTabTTOR->addLayout(tempsVihodHZagryaznenie);
            q1ResultHLayout->addLayout(q1V1Layout);
            q1ResultHLayout->addLayout(q2V1Layout);
            h1ResultHLayout->addLayout(h1V1Layout);
            h1ResultHLayout->addLayout(h2V1Layout);
            m_vLayoutCalculationTabTTOR->addLayout(q1ResultHLayout);
            m_vLayoutCalculationTabTTOR->addLayout(h1ResultHLayout);
            connect(turnOnZagryaznenieTTOR, &QCheckBox::toggled, [=](bool checked) {
                m_PresultFieldZagryaznenieQ1->setEnabled(checked);
                m_PresultFieldZagryaznenieQ2->setEnabled(checked);
                m_PresultFieldZagryaznenieH1->setEnabled(checked);
                m_PresultFieldZagryaznenieH2->setEnabled(checked);
                });

            // Кнопка для расчета
            m_CalculationButton = new QPushButton;
            m_CalculationButton->setText(u8"Выполнить расчет");

            connect(m_CalculationButton, &QPushButton::clicked, this, &ExplodeManager::onCalculationButtonClicked);
            m_vLayoutCalculationTabTTOR->addWidget(m_CalculationButton);

            // РЕЗАЛТЫ
            // Temps
            QHBoxLayout* ForTResultHLayout = new QHBoxLayout();
            QVBoxLayout* ForTq1V1Layout = new QVBoxLayout();
            ForTq1V1Layout->setSpacing(5);
            QVBoxLayout* ForTq2V1Layout = new QVBoxLayout();
            ForTq2V1Layout->setSpacing(5);

            ForTq1V1Layout->addWidget(new QLabel(u8"Температура горячего \nтеплоносителя на выходе - t₁\" (°C):"));
            m_ForTrresultTemp1 = new QLineEdit();
            m_ForTrresultTemp1->setReadOnly(true);
            ForTq1V1Layout->addWidget(m_ForTrresultTemp1);

            ForTq2V1Layout->addWidget(new QLabel(u8"Температура холодного \nтеплоносителя на выходе - t₂\" (°C):"));
            m_ForTrresultTemp2 = new QLineEdit();
            m_ForTrresultTemp2->setReadOnly(true);
            ForTq2V1Layout->addWidget(m_ForTrresultTemp2);

            ForTResultHLayout->addLayout(ForTq1V1Layout);
            ForTResultHLayout->addLayout(ForTq2V1Layout);
            m_vLayoutCalculationTabTTOR->addLayout(ForTResultHLayout);

            // Q
            QHBoxLayout* ForGResultHLayout = new QHBoxLayout();
            QVBoxLayout* ForGq1V1Layout = new QVBoxLayout();
            ForGq1V1Layout->setSpacing(5);
            QVBoxLayout* ForGq2V1Layout = new QVBoxLayout();
            ForGq2V1Layout->setSpacing(5);

            ForGq1V1Layout->addWidget(new QLabel(u8"Расход горячего теплоносителя - G₁ (°C):"));
            m_ForGrresultTemp1 = new QLineEdit();
            m_ForGrresultTemp1->setReadOnly(true);
            ForGq1V1Layout->addWidget(m_ForGrresultTemp1);

            ForGq2V1Layout->addWidget(new QLabel(u8"Расход холодного теплоносителя - G₂ (°C):"));
            m_ForGrresultTemp2 = new QLineEdit();
            m_ForGrresultTemp2->setReadOnly(true);
            ForGq2V1Layout->addWidget(m_ForGrresultTemp2);

            ForGResultHLayout->addLayout(ForGq1V1Layout);
            ForGResultHLayout->addLayout(ForGq2V1Layout);
            m_vLayoutCalculationTabTTOR->addLayout(ForGResultHLayout);

            // F
            QHBoxLayout* ForFResultHLayout = new QHBoxLayout();
            QVBoxLayout* ForFq1V1Layout = new QVBoxLayout();
            ForFq1V1Layout->setSpacing(5);
            QVBoxLayout* ForFq2V1Layout = new QVBoxLayout();
            ForFq2V1Layout->setSpacing(5);

            ForFq1V1Layout->addWidget(new QLabel(u8"Площадь поперечного сечения - F₁ (м²):"));
            m_ForFrresultTemp1 = new QLineEdit();
            m_ForFrresultTemp1->setReadOnly(true);
            ForFq1V1Layout->addWidget(m_ForFrresultTemp1);

            ForFq2V1Layout->addWidget(new QLabel(u8"Площадь поперечного сечения - F₂ (м²):"));
            m_ForFrresultTemp2 = new QLineEdit();
            m_ForFrresultTemp2->setReadOnly(true);
            ForFq2V1Layout->addWidget(m_ForFrresultTemp2);

            ForFResultHLayout->addLayout(ForFq1V1Layout);
            ForFResultHLayout->addLayout(ForFq2V1Layout);
            m_vLayoutCalculationTabTTOR->addLayout(ForFResultHLayout);
            
            // A
            QHBoxLayout* ForAResultHLayout = new QHBoxLayout();
            QVBoxLayout* ForAq1V1Layout = new QVBoxLayout();
            ForAq1V1Layout->setSpacing(5);
            QVBoxLayout* ForAq2V1Layout = new QVBoxLayout();
            ForAq2V1Layout->setSpacing(5);

            ForAq1V1Layout->addWidget(new QLabel(u8"Коэф. теплопередачи - а₁ (Вт/(м²*К)):"));
            m_ForArresultTemp1 = new QLineEdit();
            m_ForArresultTemp1->setReadOnly(true);
            ForAq1V1Layout->addWidget(m_ForArresultTemp1);

            ForAq2V1Layout->addWidget(new QLabel(u8"Коэф. теплопередачи - а₂ (Вт/(м²*К)):"));
            m_ForArresultTemp2 = new QLineEdit();
            m_ForArresultTemp2->setReadOnly(true);
            ForAq2V1Layout->addWidget(m_ForArresultTemp2);

            ForAResultHLayout->addLayout(ForAq1V1Layout);
            ForAResultHLayout->addLayout(ForAq2V1Layout);
            m_vLayoutCalculationTabTTOR->addLayout(ForAResultHLayout);

            //Q
            QHBoxLayout* ForQResultHLayout = new QHBoxLayout();
            ForQResultHLayout->addWidget(new QLabel(u8"Тепловая мощность - Q (Вт):"));
            m_ForQrresultTemp = new QLineEdit();
            m_ForQrresultTemp->setReadOnly(true);
            ForQResultHLayout->addWidget(m_ForQrresultTemp);
            m_vLayoutCalculationTabTTOR->addLayout(ForQResultHLayout);

            //K
            QHBoxLayout* ForKResultHLayout = new QHBoxLayout();
            ForKResultHLayout->addWidget(new QLabel(u8"Коэф. теплопередачи теплообменника - K (Вт/(м²*К)):"));
            m_ForKrresultTemp = new QLineEdit();
            m_ForKrresultTemp->setReadOnly(true);
            ForKResultHLayout->addWidget(m_ForKrresultTemp);
            m_vLayoutCalculationTabTTOR->addLayout(ForKResultHLayout);

            //Rзаг
            QHBoxLayout* ForRZResultHLayout = new QHBoxLayout();
            ForRZResultHLayout->addWidget(new QLabel(u8"Термическое сопротивление загрязнения - Rзаг (м²*К/Вт):"));
            m_ForRZrresultTemp = new QLineEdit();
            m_ForRZrresultTemp->setReadOnly(true);
            ForRZResultHLayout->addWidget(m_ForRZrresultTemp);
            m_vLayoutCalculationTabTTOR->addLayout(ForRZResultHLayout);

            m_calculationTab->setLayout(m_vLayoutCalculationTabTTOR);

        }
        case 2: //ExplodeWidget::TTRM
        {
            m_vLayoutCalculationTabTTRM = new QVBoxLayout();
            //
            QFormLayout* formLayout = new QFormLayout;

            QLabel* headInfoLabel = new QLabel(u8"");
            QLabel* headHLabel = new QLabel(u8"Греющий т.");
            QLabel* headCLabel = new QLabel(u8"Нагреваемый т.");
            QWidget* infoContainer = createPairWidget(headHLabel, headCLabel);

            QLabel* typeLabel = new QLabel(u8"Тип:");
            QComboBox* hotFluidComboBox = new QComboBox();
            for (const auto& fluid : fluidsProperties) {
                hotFluidComboBox->addItem(QString::fromStdString(fluid.name), fluid.c);
            }
            QComboBox* coldFluidComboBox = new QComboBox();
            for (const auto& fluid : fluidsProperties) {
                coldFluidComboBox->addItem(QString::fromStdString(fluid.name), fluid.c);
            }
            QWidget* typeContainer = createPairWidget(hotFluidComboBox, coldFluidComboBox);

            QLabel* consumptionLabel = new QLabel(u8"Расход, кг/c:");
            QDoubleSpinBox* consumptionHSpinBox = new QDoubleSpinBox;
            consumptionHSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* consumptionCSpinBox = new QDoubleSpinBox;
            consumptionCSpinBox->setSingleStep(0.1);
            QWidget* consumptionContainer = createPairWidget(consumptionHSpinBox, consumptionCSpinBox);

            QLabel* tempInLabel = new QLabel(u8"Температура на входе, °C:");
            QDoubleSpinBox* tempInHSpinBox = new QDoubleSpinBox;
            tempInHSpinBox->setRange(-100, 150);
            QDoubleSpinBox* tempInCSpinBox = new QDoubleSpinBox;
            tempInCSpinBox->setRange(-100, 150);
            QWidget* tempInContainer = createPairWidget(tempInHSpinBox, tempInCSpinBox);

            QLabel* tempOutLabel = new QLabel(u8"Температура на выходе, °C:");
            QDoubleSpinBox* tempOutHSpinBox = new QDoubleSpinBox;
            tempOutHSpinBox->setReadOnly(true);
            tempOutHSpinBox->setDisabled(true);
            tempOutHSpinBox->setKeyboardTracking(false);
            QDoubleSpinBox* tempOutCSpinBox = new QDoubleSpinBox;
            tempOutCSpinBox->setRange(-100, 150);
            QWidget* tempOutContainer = createPairWidget(tempOutHSpinBox, tempOutCSpinBox);

            QPushButton* calculationButton = new QPushButton(u8"Выполнить расчёт");
           
            formLayout->addRow(headInfoLabel, infoContainer);
            formLayout->addRow(typeLabel, typeContainer);
            formLayout->addRow(consumptionLabel, consumptionContainer);
            formLayout->addRow(tempInLabel, tempInContainer);
            formLayout->addRow(tempOutLabel, tempOutContainer);
            formLayout->addRow(calculationButton);

            QLabel* avTemLabel = new QLabel(u8"Средняя температура (t\u1D62), °C:");
            QLineEdit* avTemHLineEdit = new QLineEdit;
            avTemHLineEdit->setReadOnly(true);
            QLineEdit* avTemCLineEdit = new QLineEdit;
            avTemCLineEdit->setReadOnly(true);
            QWidget* avTemContainer = createPairWidget(avTemHLineEdit, avTemCLineEdit);

            QLabel* temPowerLabel = new QLabel(u8"Передаваемая тепловая мощность (N), Вт:");
            QLineEdit* temPowerLabelEdit = new QLineEdit;
            temPowerLabelEdit->setReadOnly(true);

            QLabel* densityLabel = new QLabel(u8"Плотность теплоносителя при t\u1D62 (P\u1D62), кг/м\xB3:");
            QLineEdit* densityHEdit = new QLineEdit;
            densityHEdit->setReadOnly(true);
            QLineEdit* densityCEdit = new QLineEdit;
            densityCEdit->setReadOnly(true);
            QWidget* densityContainer = createPairWidget(densityHEdit, densityCEdit);

            QLabel* speedLabel = new QLabel(u8"Скорость двжиения теплоносителя (V\u1D62), м/c:");
            QLineEdit* speedHEdit = new QLineEdit;
            speedHEdit->setReadOnly(true);
            QLineEdit* speedCEdit = new QLineEdit;
            speedCEdit->setReadOnly(true);
            QWidget* speedContainer = createPairWidget(speedHEdit, speedCEdit);

            QLabel* reinoldsLabel = new QLabel(u8"Число Рейнольдса (Re\u1D62):");
            QLineEdit* reinoldsHEdit = new QLineEdit;
            reinoldsHEdit->setReadOnly(true);
            QLineEdit* reinoldsCEdit = new QLineEdit;
            reinoldsCEdit->setReadOnly(true);
            QWidget* reinoldsContainer = createPairWidget(reinoldsHEdit, reinoldsCEdit);

            QLabel* PrLabel = new QLabel(u8"Число Прандтля при t\u1D62 (Pr\u1D62):");
            QLineEdit* PrHEdit = new QLineEdit;
            PrHEdit->setReadOnly(true);
            QLineEdit* PrCEdit = new QLineEdit;
            PrCEdit->setReadOnly(true);
            QWidget* PrContainer = createPairWidget(PrHEdit, PrCEdit);

            QLabel* NuLabel = new QLabel(u8"Критерий Нуссельта (Nu\u1D62):");
            QLineEdit* NuHEdit = new QLineEdit;
            NuHEdit->setReadOnly(true);
            QLineEdit* NuCEdit = new QLineEdit;
            NuCEdit->setReadOnly(true);
            QWidget* NuContainer = createPairWidget(NuHEdit, NuCEdit);

            QLabel* QCapacityLabel = new QLabel(u8"К-т теплоотдачи (стенке и от стенки) (α\u1D62), Вт/м\xB2*K:");
            QLineEdit* QCapacityHEdit = new QLineEdit;
            QCapacityHEdit->setReadOnly(true);
            QLineEdit* QCapacityCEdit = new QLineEdit;
            QCapacityCEdit->setReadOnly(true);
            QWidget* QCapacityContainer = createPairWidget(QCapacityHEdit, QCapacityCEdit);

            QLabel* CapacityLabel = new QLabel(u8"К-т теплопередачи (K), Вт/м\xB2*K:");
            QLineEdit* CapacityEdit = new QLineEdit;
            CapacityEdit->setReadOnly(true);

            QLabel* maxTempLabel = new QLabel(u8"Максимальный температурный напор (Δtmax), °C:");
            QLineEdit* maxTempEdit = new QLineEdit;
            maxTempEdit->setReadOnly(true);

            QLabel* minTempLabel = new QLabel(u8"Минимальный температурный напор (Δtmin), °C:");
            QLineEdit* minTempEdit = new QLineEdit;
            minTempEdit->setReadOnly(true);

            QLabel* avTempLabel = new QLabel(u8"Средний температурный напор (Δtср), °C:");
            QLineEdit* avTempEdit = new QLineEdit;
            avTempEdit->setReadOnly(true);

            QLabel* tempCapacityLabel = new QLabel(u8"Плотность теплового потока (q), Вт/м\xB2:");
            QLineEdit* tempCapacityEdit = new QLineEdit;
            tempCapacityEdit->setReadOnly(true);

            QLabel* squareLabel = new QLabel(u8"Площадь поверхности нагрева (F), м\xB2:");
            QLineEdit* squareEdit = new QLineEdit;
            squareEdit->setReadOnly(true);

            QLabel* quantityLabel = new QLabel(u8"Расчётное число секций (n), м:");
            QLineEdit* quantityEdit = new QLineEdit;
            quantityEdit->setReadOnly(true);

            formLayout->addRow(avTemLabel, avTemContainer);
            formLayout->addRow(temPowerLabel, temPowerLabelEdit);
            formLayout->addRow(densityLabel, densityContainer);
            formLayout->addRow(speedLabel, speedContainer);
            formLayout->addRow(reinoldsLabel, reinoldsContainer);
            formLayout->addRow(PrLabel, PrContainer);
            formLayout->addRow(NuLabel, NuContainer);
            formLayout->addRow(QCapacityLabel, QCapacityContainer);
            formLayout->addRow(CapacityLabel, CapacityEdit);
            formLayout->addRow(maxTempLabel, maxTempEdit);
            formLayout->addRow(minTempLabel, minTempEdit);
            formLayout->addRow(avTempLabel, avTempEdit);
            formLayout->addRow(tempCapacityLabel, tempCapacityEdit);
            formLayout->addRow(squareLabel, squareEdit);
            formLayout->addRow(quantityLabel, quantityEdit);
            

            connect(calculationButton, &QPushButton::clicked, [=]() {
                const int hotFluidIndex = hotFluidComboBox->currentIndex();
                const int coldFluidIndex = coldFluidComboBox->currentIndex();

                const double tempInC = tempInCSpinBox->value();
                const double tempInH = tempInHSpinBox->value();

                const double tempOutC = tempOutCSpinBox->value();

                if (tempInC >= tempInH)
                {
                    QString* text = new QString(u8"Значение температуры нагреваемоего теплоносителя должно быть меньше температуры греющего!");
                    createWarning(text);

                    return;
                }

                if (!consumptionCSpinBox->value())
                {
                    QString* text = new QString(u8"Значение расхода нагреваемоего теплоносителя пустое!");
                    createWarning(text);

                    return;
                }

                if (!consumptionHSpinBox->value())
                {
                    QString* text = new QString(u8"Значение расхода греющего теплоносителя пустое!");
                    createWarning(text);

                    return;
                }

                if (!tempInC && !tempOutC)
                {
                    QString* text = new QString(u8"Значения входной и выходной температуры нагреваемоего теплоносителя пустые!");
                    createWarning(text);

                    return;
                }

                const double avTemC = (tempInC + tempOutC) / 2;

                avTemCLineEdit->setText(QString::number(avTemC));

                const double consumtion = consumptionCSpinBox->value();
                const int fluidC = fluidsProperties[coldFluidIndex].capacity;
                const double temPower = consumtion * fluidC * (tempOutC - tempInC);
                temPowerLabelEdit->setText(QString::number(temPower));

                if (temPower < 0)
                {
                    QString* text = 
                        new QString(u8"Отрицательная тепловая мощность! \n Температура нагреваемого теплоносителя меньше чем на входе.");
                    createWarning(text);

                    return;
                }

                const double tempOutH = tempInH - temPower / (consumptionHSpinBox->value() * fluidsProperties[hotFluidIndex].capacity);
                const double avTemH = (tempInH + tempOutH) / 2;

                tempOutHSpinBox->setValue(tempOutH);

                avTemHLineEdit->setText(QString::number(avTemH));
       
                const double densityHIndex = round(avTemH / 10) + 10;
                const double densityCIndex = round(avTemC / 10) + 10;
                const double densityH = fluidDensity[hotFluidIndex][densityHIndex > 0 ? densityHIndex : 0];
                const double densityC = fluidDensity[coldFluidIndex][densityCIndex > 0 ? densityCIndex : 0];
                densityHEdit->setText(QString::number(densityH));
                densityCEdit->setText(QString::number(densityC));

                const int technicalIndex = m_comboConfigure->currentIndex();

                const ConfigParams params = isCheckedManualType
                    ? manualTTRMParams
                    : dataTTRM[technicalIndex > 0 ? technicalIndex : 0];

                double hViscocity;
                double сViscocity;

                switch (hotFluidIndex)
                {
                    case 2: // Вода
                    {
                        hViscocity = waterViscocity[round(avTemH / 10)];
                        break;
                    }
                    case 3: // Керосин
                    {
                        hViscocity = cerosinViscocity[round(avTemH / 10)];
                        break;
                    }
                    case 4: // Масло нефтяное
                    {
                        hViscocity = oilViscocity[round(avTemH / 10)];
                        break;
                    }
                    default:
                    {
                        hViscocity = fluidsProperties[hotFluidIndex].viscocity;
                        break;
                    }
                }

                switch (coldFluidIndex)
                {
                case 2: // Вода
                {
                    сViscocity = waterViscocity[round(avTemC / 10)];
                    break;
                }
                case 3: // Керосин
                {
                    сViscocity = cerosinViscocity[round(avTemC / 10)];
                    break;
                }
                case 4: // Масло нефтяное
                {
                    сViscocity = oilViscocity[round(avTemC / 10)];
                    break;
                }
                default:
                {
                    сViscocity = fluidsProperties[coldFluidIndex].viscocity;
                    break;
                }
                }

                hViscocity = hViscocity * pow(10, -6);
                сViscocity = сViscocity * pow(10, -6);

                const double D1 = params.assortmentInnerTubes / 10 / 100;
                const double D2 = params.assortmentOuterTubes / 10 / 100;
                const double d1 = (params.assortmentInnerTubes - params.thicknessInnerTubes * 2) / 10 / 100;
                const double d2 = (params.assortmentOuterTubes - params.thicknessOuterTubes * 2) / 10 / 100;

                const double speedH = (4 * consumptionHSpinBox->value()) / (fluidsProperties[hotFluidIndex].p * M_PI * pow(d1, 2));
                const double speedC = (4 * consumptionCSpinBox->value()) / (fluidsProperties[coldFluidIndex].p * M_PI * (pow(d2, 2) - pow(D1, 2)));
                speedHEdit->setText(QString::number(speedH));
                speedCEdit->setText(QString::number(speedC));

                const double reinoldsH = (speedH * params.assortmentInnerTubes / 10 / 100) / hViscocity;
                const double reinoldsC = (speedC * params.assortmentOuterTubes / 10 / 100) / сViscocity;
                reinoldsHEdit->setText(QString::number(reinoldsH));
                reinoldsCEdit->setText(QString::number(reinoldsC));

                const double hotPr = hotFluidIndex == 2 ? 7.5 - 0.0694 * avTemH : fluidsProperties[hotFluidIndex].Pr;
                const double coldPr = coldFluidIndex == 2 ? 7.5 - 0.0694 * avTemC : fluidsProperties[coldFluidIndex].Pr;
                PrHEdit->setText(QString::number(hotPr));
                PrCEdit->setText(QString::number(coldPr));

                double NusseltHotIndex = 0.021 * pow(reinoldsH, 0.8) * pow(hotPr, 0.43) * pow((hotPr / hotPr), 0.25);
                double NusseltColdIndex = 
                    0.017 * 
                    pow(reinoldsC, 0.8) * 
                    pow(coldPr, 0.43) * 
                    pow((coldPr / coldPr), 0.25) * 
                    pow(((d2) / D1), 0.18);

                NuHEdit->setText(QString::number(NusseltHotIndex));
                NuCEdit->setText(QString::number(NusseltColdIndex));

                double Qa1 = NusseltHotIndex * (fluidsProperties[hotFluidIndex].laymbda / d1);
                double Qa2 = NusseltColdIndex * (fluidsProperties[coldFluidIndex].laymbda / (d2 - D1));
                QCapacityHEdit->setText(QString::number(Qa1));
                QCapacityCEdit->setText(QString::number(Qa2));

                const double K = 1 / (1 / Qa1 + ((D1 - d1) / 2) / 50 + 1 / Qa2);
                CapacityEdit->setText(QString::number(K));

                const double maxTemp = abs(tempInH - tempInC);
                maxTempEdit->setText(QString::number(maxTemp));

                const double minTemp = abs(tempOutH - tempOutC);
                minTempEdit->setText(QString::number(minTemp));

                const double averageTemp = (maxTemp - minTemp) / log(maxTemp / minTemp);
                avTempEdit->setText(QString::number(averageTemp));

                const double tempCapacity = K * averageTemp;
                tempCapacityEdit->setText(QString::number(tempCapacity));

                const double squareTemp = temPower / tempCapacity;
                squareEdit->setText(QString::number(squareTemp));

                const double quantity = round(squareTemp / (M_PI * d1 * params.lengthK / 10 / 100));
                quantityEdit->setText(QString::number(quantity));

                tempOutHSpinBox->setDisabled(false);
            });

            m_calculationTab->setLayout(formLayout);
            break;
        }
        case 3: //ExplodeWidget::IU
        {

            m_vLayoutCalculationTabIU = new QVBoxLayout();

            QFormLayout* formLayout = new QFormLayout;

            QLabel* headInfoLabel = new QLabel(u8"");
            QLabel* headHLabel = new QLabel(u8"Трубное пространство");
            QLabel* headCLabel = new QLabel(u8"Межтрубное пространство");
            QWidget* infoContainer = createPairWidget(headHLabel, headCLabel);

            QLabel* typeLabel = new QLabel(u8"Тип:");
            QComboBox* hotFluidComboBox = new QComboBox();
            hotFluidComboBox->addItem(u8"Вода");
            hotFluidComboBox->addItem(u8"Ацетон");
            hotFluidComboBox->addItem(u8"Бензин");
            hotFluidComboBox->addItem(u8"Спирт этиловый");
            hotFluidComboBox->addItem(u8"Нефтяное масло");

            QComboBox* coldFluidComboBox = new QComboBox();
            coldFluidComboBox->addItem(u8"Аммиак");
            coldFluidComboBox->addItem(u8"Пропан");
            coldFluidComboBox->addItem(u8"Пропеллен");
            coldFluidComboBox->addItem(u8"Азот");
            coldFluidComboBox->addItem(u8"Диоксид углерода");

            QWidget* typeContainer = createPairWidget(hotFluidComboBox, coldFluidComboBox);
            // для гидравлического 
            QLabel* densityEntranceLabel = new QLabel(u8"Плотность среды на входе, кг/м\xB3:");
            QDoubleSpinBox* densityEntranceHotSpinBox = new QDoubleSpinBox;
            densityEntranceHotSpinBox->setRange(500, 2000);
            densityEntranceHotSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* densityEntranceColdSpinBox = new QDoubleSpinBox;
            densityEntranceColdSpinBox->setSingleStep(0.1);
            densityEntranceColdSpinBox->setRange(0, 10);
            QWidget* densityEntranceContainer = createPairWidget(densityEntranceHotSpinBox, densityEntranceColdSpinBox);


            QLabel* densityExitLabel = new QLabel(u8"Плотность среды на выходе, кг/м\xB3:");
            QDoubleSpinBox* densityExitHotSpinBox = new QDoubleSpinBox;
            densityExitHotSpinBox->setRange(500, 2000);
            densityExitHotSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* densityExitColdSpinBox = new QDoubleSpinBox;
            densityExitColdSpinBox->setRange(0, 10);
            densityExitColdSpinBox->setSingleStep(0.1);
            QWidget* densityExitContainer = createPairWidget(densityExitHotSpinBox, densityExitColdSpinBox);

            QLabel* speedEntranceLabel = new QLabel(u8"Скорость среды на входе (max: 7), м/с");
            QDoubleSpinBox* speedEntranceHotSpinBox = new QDoubleSpinBox;
            speedEntranceHotSpinBox->setSingleStep(0.1);
            speedEntranceHotSpinBox->setRange(0, 7);
            QDoubleSpinBox* speedEntranceColdSpinBox = new QDoubleSpinBox;
            speedEntranceColdSpinBox->setSingleStep(0.1);
            speedEntranceColdSpinBox->setRange(0, 7);
            QWidget* speedEntranceContainer = createPairWidget(speedEntranceHotSpinBox, speedEntranceColdSpinBox);

            QLabel* speedExitLabel = new QLabel(u8"Скорость среды на выходе (max: 10), м/с");
            QDoubleSpinBox* speedExitHotSpinBox = new QDoubleSpinBox;
            speedExitHotSpinBox->setSingleStep(0.1);
            speedExitHotSpinBox->setRange(0, 10);
            QDoubleSpinBox* speedExitColdSpinBox = new QDoubleSpinBox;
            speedExitColdSpinBox->setSingleStep(0.1);
            speedExitColdSpinBox->setRange(0, 10);
            QWidget* speedExitContainer = createPairWidget(speedExitHotSpinBox, speedExitColdSpinBox);

            // для теплового ! 
            QLabel* temperatureEntranceLabel = new QLabel(u8"Температура среды на входе, ⁰С");
            QDoubleSpinBox* temperatureEntranceHotSpinBox = new QDoubleSpinBox;
            temperatureEntranceHotSpinBox->setRange(0, 150);
            temperatureEntranceHotSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* temperatureEntranceColdSpinBox = new QDoubleSpinBox;
            temperatureEntranceColdSpinBox->setRange(0, 150);
            temperatureEntranceHotSpinBox->setSingleStep(0.1);
            QWidget* temperatureEntranceContainer = createPairWidget(temperatureEntranceHotSpinBox, temperatureEntranceColdSpinBox);

            QLabel* temperatureExitLabel = new QLabel(u8"Температура среды на выходе,⁰С");
            QDoubleSpinBox* temperatureExitHotSpinBox = new QDoubleSpinBox;
            temperatureExitHotSpinBox->setRange(0, 150);
            temperatureExitHotSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* temperatureExitColdSpinBox = new QDoubleSpinBox;
            temperatureExitColdSpinBox->setSingleStep(0.1);
            temperatureExitColdSpinBox->setRange(0, 150);
            QWidget* temperatureExitContainer = createPairWidget(temperatureExitHotSpinBox, temperatureExitColdSpinBox);

            QLabel* consumptionColdLabel = new QLabel(u8"Массовый расход нагреваемого теплоносителя (max: 10), кг/с");
            QDoubleSpinBox* consumptionColdSpinBox = new QDoubleSpinBox;
            consumptionColdSpinBox->setSingleStep(0.1);
            consumptionColdSpinBox->setRange(2, 10);

            densityEntranceHotSpinBox->setValue(870.);
            densityEntranceColdSpinBox->setValue(0.99);
            densityExitHotSpinBox->setValue(899.);
            densityExitColdSpinBox->setValue(0.87);
            speedEntranceHotSpinBox->setValue(6);
            speedEntranceColdSpinBox->setValue(4);
            speedExitHotSpinBox->setValue(7);
            speedExitColdSpinBox->setValue(5);
            temperatureEntranceHotSpinBox->setValue(120);
            temperatureEntranceColdSpinBox->setValue(50);
            temperatureExitHotSpinBox->setValue(60);
            temperatureExitColdSpinBox->setValue(70);
            consumptionColdSpinBox->setValue(3);


            QPushButton* calculationButton = new QPushButton(u8"Выполнить расчёт");

            QLabel* reinoldsLabel = new QLabel(u8"Число Рейнольдса, Re:");
            QLineEdit* reinoldsHEdit = new QLineEdit;
            reinoldsHEdit->setReadOnly(true);
            QLineEdit* reinoldsCEdit = new QLineEdit;
            reinoldsCEdit->setReadOnly(true);
            QWidget* reinoldsContainer = createPairWidget(reinoldsHEdit, reinoldsCEdit);

            QLabel* squareSectionLabel = new QLabel(u8"Площадь поперечного сечения корпуса, м\xB2:");
            QLineEdit* squareSectionEdit = new QLineEdit;
            squareSectionEdit->setReadOnly(true);

            QLabel* squareInterTubeLabel = new QLabel(u8"Площадь межтрубного пространства, м\xB2:");
            QLineEdit* squareInterTubeEdit = new QLineEdit;
            squareInterTubeEdit->setReadOnly(true);

            QLabel* speedInterTubeLabel = new QLabel(u8"Скорость воды в межтрубном пространстве, м/c:");
            QLineEdit* speedInterTubeEdit = new QLineEdit;
            speedInterTubeEdit->setReadOnly(true);

            QLabel* resistanceCoefficientLabel = new QLabel(u8"Коэффициент сопротивления трения, λ:");
            QLineEdit* resistanceCoefficientHotEdit = new QLineEdit;
            resistanceCoefficientHotEdit->setReadOnly(true);
            QLineEdit* resistanceCoefficientColdEdit = new QLineEdit;
            resistanceCoefficientColdEdit->setReadOnly(true);
            QWidget* resistanceCoefficientContainer = createPairWidget(resistanceCoefficientHotEdit, resistanceCoefficientColdEdit);

            QLabel* frictionPressureLabel = new QLabel(u8"Потери давления на трение, ∆Pтр:");
            QLineEdit* frictionPressureHotEdit = new QLineEdit;
            frictionPressureHotEdit->setReadOnly(true);
            QLineEdit* frictionPressureColdEdit = new QLineEdit;
            frictionPressureColdEdit->setReadOnly(true);
            QWidget* frictionPressureContainer = createPairWidget(frictionPressureHotEdit, frictionPressureColdEdit);

            QLabel* localResistancePressureLabel = new QLabel(u8"Потери давления в местных сопротивлениях, ∆Pм:");
            QLineEdit* localResistanceHotEdit = new QLineEdit;
            localResistanceHotEdit->setReadOnly(true);
            QLineEdit* localResistanceColdEdit = new QLineEdit;
            localResistanceColdEdit->setReadOnly(true);
            QWidget* localResistancePressureContainer = createPairWidget(localResistanceHotEdit, localResistanceColdEdit);

            QLabel* accelerationFlowPressureLabel = new QLabel(u8"Потери при ускорении потока, ∆Pу:");
            QLineEdit* accelerationFlowHotEdit = new QLineEdit;
            accelerationFlowHotEdit->setReadOnly(true);
            QLineEdit* accelerationFlowColdEdit = new QLineEdit;
            accelerationFlowColdEdit->setReadOnly(true);
            QWidget* accelerationFlowPressureContainer = createPairWidget(accelerationFlowHotEdit, accelerationFlowColdEdit);

            QLabel* pressureDropLabel = new QLabel(u8"Перепад давления, ∆Pг:");
            QLineEdit* pressureDropHotEdit = new QLineEdit;
            pressureDropHotEdit->setReadOnly(true);
            QLineEdit* pressureDropColdEdit = new QLineEdit;
            pressureDropColdEdit->setReadOnly(true);
            QWidget* pressureDropContainer = createPairWidget(pressureDropHotEdit, pressureDropColdEdit);

            QLabel* fullPressureLabel = new QLabel(u8"Полное давления, ∆P:");
            QLineEdit* fullPressureHotEdit = new QLineEdit;
            fullPressureHotEdit->setReadOnly(true);
            QLineEdit* fullPressureColdEdit = new QLineEdit;
            fullPressureColdEdit->setReadOnly(true);
            QWidget* fullPressureContainer = createPairWidget(fullPressureHotEdit, fullPressureColdEdit);

            QLabel* averageTemperatureLabel = new QLabel(u8"Средняя температура, ⁰С:");
            QLineEdit* averageTemperatureHotEdit = new QLineEdit;
            averageTemperatureHotEdit->setReadOnly(true);
            QLineEdit* averageTemperatureColdEdit = new QLineEdit;
            averageTemperatureColdEdit->setReadOnly(true);
            QWidget* averageTemperatureContainer = createPairWidget(averageTemperatureHotEdit, averageTemperatureColdEdit);

            QLabel* heatLabel = new QLabel(u8"Количество теплоты, Дж:");
            QLineEdit* heatEdit = new QLineEdit;
            heatEdit->setReadOnly(true);

            QLabel* consumptionHotLabel = new QLabel(u8"Массовый расход греющего теплоносителя, кг/с:");
            QLineEdit* consumptionHotEdit = new QLineEdit;
            consumptionHotEdit->setReadOnly(true);


            formLayout->addRow(headInfoLabel, infoContainer);
            formLayout->addRow(typeLabel, typeContainer);
            formLayout->addRow(densityEntranceLabel, densityEntranceContainer);
            formLayout->addRow(densityExitLabel, densityExitContainer);
            formLayout->addRow(speedEntranceLabel, speedEntranceContainer);
            formLayout->addRow(speedExitLabel, speedExitContainer);
            formLayout->addRow(temperatureEntranceLabel, temperatureEntranceContainer);
            formLayout->addRow(temperatureExitLabel, temperatureExitContainer);
            formLayout->addRow(consumptionColdLabel, consumptionColdSpinBox);
            formLayout->addRow(calculationButton);
            formLayout->addRow(reinoldsLabel, reinoldsContainer);
            formLayout->addRow(squareSectionLabel, squareSectionEdit);
            formLayout->addRow(squareInterTubeLabel, squareInterTubeEdit);
            formLayout->addRow(speedInterTubeLabel, speedInterTubeEdit);
            formLayout->addRow(resistanceCoefficientLabel, resistanceCoefficientContainer);
            formLayout->addRow(frictionPressureLabel, frictionPressureContainer);
            formLayout->addRow(localResistancePressureLabel, localResistancePressureContainer);
            formLayout->addRow(accelerationFlowPressureLabel, accelerationFlowPressureContainer);
            formLayout->addRow(pressureDropLabel, pressureDropContainer);
            formLayout->addRow(fullPressureLabel, fullPressureContainer);
            formLayout->addRow(averageTemperatureLabel, averageTemperatureContainer);
            formLayout->addRow(heatLabel, heatEdit);
            formLayout->addRow(consumptionHotLabel, consumptionHotEdit);


            connect(calculationButton, &QPushButton::clicked, [=]() {
                //индексы сред
                const int hotFluidIndex = hotFluidComboBox->currentIndex();
                const int coldFluidIndex = coldFluidComboBox->currentIndex() + 5;     //чтобы в одной структуре хранить

                double temp1_hot = temperatureEntranceHotSpinBox->value();//температура на входе
                double temp2_hot = temperatureExitHotSpinBox->value();//температура на выходе
                double temp1_cold = temperatureEntranceColdSpinBox->value();//температура на входе
                double temp2_cold = temperatureExitColdSpinBox->value();//температура на выходе


                if (temp1_hot < temp1_cold)
                {
                    QString* text = new QString(u8"Температура теплоносителя в межтрубном пространстве на входе должна быть меньше, чем в трубах на входе.");
                    createWarning(text);

                    return;
                }

                if (temp1_hot < temp2_hot)
                {
                    QString* text = new QString(u8"Температура теплоносителя в трубах на входе должна быть больше, чем на выходе.");
                    createWarning(text);

                    return;
                }

                if (temp1_cold > temp2_cold)
                {
                    QString* text = new QString(u8"Температура теплоносителя в межтрубном пространстве на входе должна быть меньше, чем на выходе.");
                    createWarning(text);

                    return;
                }


                if (temp1_hot <= 0)
                {
                    QString* text = new QString(u8"Температура теплоносителя в трубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                //средняя температура
                double temp_hot_av = (temp1_hot + temp2_hot) / 2;
                double temp_cold_av = (temp1_cold + temp2_cold) / 2;
                //индекс температуры
                int temp_hot_index = round(temp_hot_av / 10);
                int temp_cold_index = round(temp_cold_av / 10);

                //определение какую структура использовать: типовую или пользовательскую
                const int technicalIndex = m_comboConfigure->currentIndex();
                const ConfigParams_IU params = isCheckedManualType
                    ? manualIUParams
                    : dataIU[technicalIndex > 0 ? technicalIndex : 0];

                int n;//количество труб
                double delta_h_hot;//разница уровней входа и выхода теплоносителя в систему
                if ((500 <= params.D_Kam) && (params.D_Kam < 600)) {
                    n = 208;
                    delta_h_hot = 884. / 1000;//в метрах
                }
                else if ((600 <= params.D_Kam) && (params.D_Kam < 700)) {
                    n = 310;
                    delta_h_hot = 984. / 1000;//м
                }
                else if ((700 <= params.D_Kam) && (params.D_Kam < 800)) {
                    n = 452;
                    delta_h_hot = 1088. / 1000;
                }
                else if ((800 <= params.D_Kam) && (params.D_Kam < 900)) {
                    n = 590;
                    delta_h_hot = 1188. / 1000;
                }
                else if ((900 <= params.D_Kam) && (params.D_Kam < 1000)) {
                    n = 778;
                    delta_h_hot = 1332. / 1000;
                }
                else  if ((1000 <= params.D_Kam) && (params.D_Kam < 1100)) {
                    n = 980;
                    delta_h_hot = 1392. / 1000;
                }
                else if ((1100 <= params.D_Kam) && (params.D_Kam < 1200)) {
                    n = 1210;
                    delta_h_hot = 1492. / 1000;
                }
                else if (1200 <= params.D_Kam) {
                    n = 1466;
                    delta_h_hot = 1694. / 1000;
                }

                //считаем число рейнольдса
                double d = 20. / 1000.; //диаметр труб в метрах
                double density_hot = fluidDensity_IU_IP[hotFluidIndex][temp_hot_index];//плотность в кг/м3
                double density_cold = fluidDensity_IU_IP[coldFluidIndex][temp_cold_index];//плотность в кг/м3
                double capacity_hot = fluidsProperties_IU_IP[hotFluidIndex].c; //теплоемкость греющей среды
                double capacity_cold = fluidsProperties_IU_IP[coldFluidIndex].c;//теплоемколсть нагреваемой среды

                double Gr_cold = consumptionColdSpinBox->value(); //расход теплоносителя в кг/c

                float N = 0.97; //коэффициент потери теплоты в окр.среду

                double Q = (Gr_cold * capacity_cold * (temp2_cold - temp1_cold)) / 1000;
                double Gr_hot = Q * 1000 / (capacity_hot * (temp1_hot - temp2_hot) * N); //расход теплоносителя, будет считаться
                double viscosity_hot = fluidViscocity_IU_IP[hotFluidIndex][temp_hot_index] * pow(10, -6); // вязкость
                double viscosity_cold = fluidViscocity_IU_IP[coldFluidIndex][temp_cold_index] * pow(10, -6); // вязкость

                //площадь поперечного сечения корпуса
                double Fd = M_PI * pow(params.diam / 1000., 2) / 4;
                //площадь, занятая трубами
                double fm = M_PI * pow(d, 2) * n / 4;
                //площадь межтрубного пространства
                double f1 = Fd - fm;

                //скорость среды в межтрубном пространстве
                double w_cold = Gr_cold / (f1 * density_cold);
                //эквивалентный диаметр межтр пространства
                double d_e_cold = 4. * f1 / (M_PI * (params.diam / 1000. + n * d));
                //число Рейнольдса в межтрубном пространстве
                double Re_cold = w_cold * d_e_cold / viscosity_cold;
                //скорость воды в трубном пространстве
                double w_hot = Gr_hot / (fm * density_hot);
                //число Рейнольдса в трубном пространстве
                double Re_hot = w_hot * d / viscosity_hot;



                double v1_hot = speedEntranceHotSpinBox->value(); // скорость на входе
                double v2_hot = speedExitHotSpinBox->value(); // скорость на выходе
                double v1_cold = speedEntranceColdSpinBox->value(); // скорость на входе
                double v2_cold = speedExitColdSpinBox->value(); // скорость на выходе
                double density1_hot = densityEntranceHotSpinBox->value(); // плотность на входе 
                double density2_hot = densityExitHotSpinBox->value(); //плотность на выходе
                double density1_cold = densityEntranceColdSpinBox->value(); // плотность на входе 
                double density2_cold = densityExitColdSpinBox->value(); //плотность на выходе

                if (v2_hot < v1_hot)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в трубном пространстве на выходе должна быть больше, чем на входе.");
                    createWarning(text);

                    return;
                }

                if (v2_cold < v1_cold)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в межтрубном пространстве на выходе должна быть больше, чем на входе.");
                    createWarning(text);

                    return;
                }

                if (density1_hot > density2_hot)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в трубном пространстве на входе должна быть меньше, чем на выходе.");
                    createWarning(text);

                    return;
                }

                if (density1_cold < density2_cold)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в межтрубном пространстве на входе должна быть больше, чем на выходе.");
                    createWarning(text);

                    return;
                }

                if (v2_hot <= 0)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в трубном пространстве на выходе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (v1_hot <= 0)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в трубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (v2_cold <= 0)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в межтрубном пространстве на выходе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (v1_cold <= 0)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в межтрубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (density1_cold <= 0)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в межтрубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (density2_cold <= 0)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в межтрубном пространстве на выходе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (density1_hot <= 0)
                {
                    QString* text = new QString(u8"Плотность теплоносителя  в трубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (density2_hot <= 0)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в трубном пространстве на выходе должна быть больше 0.");
                    createWarning(text);

                    return;
                }




                double v_sr_hot = (v1_hot + v2_hot) / 2; //средняя скорость 
                double v_sr_cold = (v1_cold + v2_cold) / 2; //средняя скорость 




                //double Re_hot = v_sr_hot * d / viscosity_hot;

                double L = 6000. / 1000. * n; //суммарная длина трубок
                //double d_v = ; //внутренний диаметр трубок
                double density_atm = 1.2754; //плотность атмосферного воздуха
                 //разница уровней входа и выхода теплоносителя в систему
                double delta_h_cold = (380 + params.diam) / 1000; //разница уровней входа и выхода теплоносителя в систему, 380 - сумма высот фланцев
                double lambda_cold = 1 / pow((1.8 * log(Re_cold) - 1.5), 2); //коэффициент сопротивления трения (логарифм надо исправить!)

                double e = 3.5;//коэффициент местного сопротивления

                double Pt_cold = lambda_cold * L * pow(v_sr_cold, 2) * density1_cold / (2 * d_e_cold); //потери на трение d->d_v
                double Pm_cold = e * pow(v_sr_cold, 2) * density_cold / 2; //потери в местных сопротивлениях
                double Py_cold = (density2_cold * v2_cold - density1_cold * v1_cold); //потери при ускорении потока
                double Pg_cold = (density1_cold - density_atm) * -delta_h_cold; //перепад давления
                double P_cold = Pt_cold + Pm_cold + Py_cold + Pg_cold; //полное давление


                //коэффициент сопротивления трения
                double lambda_hot = 1 / pow((1.8 * log(Re_hot) - 1.5), 2);
                //потери на трение
                double Pt_hot = (lambda_hot * L * pow(v_sr_hot, 2) * density1_hot /
                    (2 * (params.diam / 1000))) / pow(10, 6);
                //потери в местных сопротивлениях
                double Pm_hot = (5.5 * pow(v_sr_hot, 2) * density_hot / 2) / pow(10, 6);
                //потери при ускорении потока
                double Py_hot = (density2_hot * v2_hot - density1_hot * v1_hot) / pow(10, 6);
                //перепад давления
                double Pg_hot = ((density1_hot - density_atm) * delta_h_hot) / pow(10, 6);
                //полное давление
                double P_hot = Pt_hot + Pm_hot + Py_hot + Pg_hot;


                reinoldsHEdit->setText(QString::number(Re_hot));
                reinoldsCEdit->setText(QString::number(Re_cold));
                squareSectionEdit->setText(QString::number(Fd) + u8" м\xB2");//Площадь поперечного сечения корпуса
                squareInterTubeEdit->setText(QString::number(f1) + u8" м\xB2");//Площадь межтрубного пространства
                speedInterTubeEdit->setText(QString::number(w_cold) + u8" м/c");//Скорость воды в межтрубном пространстве
                resistanceCoefficientHotEdit->setText(QString::number(lambda_hot) + u8" ");//Коэффициент сопротивления трения
                resistanceCoefficientColdEdit->setText(QString::number(lambda_cold) + u8" ");
                frictionPressureHotEdit->setText(QString::number(Pt_hot) + u8" МПа");//Потери давления на трение
                frictionPressureColdEdit->setText(QString::number(Pt_cold) + u8" Па");//
                localResistanceHotEdit->setText(QString::number(Pm_hot) + u8" МПа");//Потери давления в местных сопротивлениях
                localResistanceColdEdit->setText(QString::number(Pm_cold) + u8" Па");
                accelerationFlowHotEdit->setText(QString::number(Py_hot) + u8" МПа");//Потери при ускорении потока
                accelerationFlowColdEdit->setText(QString::number(Py_cold) + u8" Па");
                pressureDropHotEdit->setText(QString::number(Pg_hot) + u8" МПа");//Перепад давления
                pressureDropColdEdit->setText(QString::number(Pg_cold) + u8" Па");
                fullPressureHotEdit->setText(QString::number(P_hot) + u8" МПа");//Полное давления
                fullPressureColdEdit->setText(QString::number(P_cold) + u8" Па");
                averageTemperatureHotEdit->setText(QString::number(temp_hot_av) + u8" °C");//Средняя температура
                averageTemperatureColdEdit->setText(QString::number(temp_cold_av) + u8" °C");
                heatEdit->setText(QString::number(Q) + u8" кДж");//количество теплоты
                consumptionHotEdit->setText(QString::number(Gr_hot) + u8" кг/с"); //Массовый расход греющего теплоносителя
            }
            );



            m_calculationTab->setLayout(formLayout);

            break;
        }
        case 4: //ExplodeWidget::IP
        {
            m_vLayoutCalculationTabIU = new QVBoxLayout();

            QFormLayout* formLayout = new QFormLayout;

            QLabel* headInfoLabel = new QLabel(u8"");
            QLabel* headHLabel = new QLabel(u8"Трубное пространство");
            QLabel* headCLabel = new QLabel(u8"Межтрубное пространство");
            QWidget* infoContainer = createPairWidget(headHLabel, headCLabel);

            QLabel* typeLabel = new QLabel(u8"Тип:");
            QComboBox* hotFluidComboBox = new QComboBox();
            hotFluidComboBox->addItem(u8"Вода");
            hotFluidComboBox->addItem(u8"Ацетон");
            hotFluidComboBox->addItem(u8"Бензин");
            hotFluidComboBox->addItem(u8"Спирт этиловый");
            hotFluidComboBox->addItem(u8"Нефтяное масло");

            QComboBox* coldFluidComboBox = new QComboBox();
            coldFluidComboBox->addItem(u8"Аммиак");
            coldFluidComboBox->addItem(u8"Пропан");
            coldFluidComboBox->addItem(u8"Пропеллен");
            coldFluidComboBox->addItem(u8"Азот");
            coldFluidComboBox->addItem(u8"Диоксид углерода");

            QWidget* typeContainer = createPairWidget(hotFluidComboBox, coldFluidComboBox);
            // для гидравлического 
            QLabel* densityEntranceLabel = new QLabel(u8"Плотность среды на входе, кг/м3");
            QDoubleSpinBox* densityEntranceHotSpinBox = new QDoubleSpinBox;
            densityEntranceHotSpinBox->setRange(500, 2000);
            densityEntranceHotSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* densityEntranceColdSpinBox = new QDoubleSpinBox;
            densityEntranceColdSpinBox->setSingleStep(0.1);
            densityEntranceColdSpinBox->setRange(0, 10);
            QWidget* densityEntranceContainer = createPairWidget(densityEntranceHotSpinBox, densityEntranceColdSpinBox);


            QLabel* densityExitLabel = new QLabel(u8"Плотность среды на выходе, кг/м3");
            QDoubleSpinBox* densityExitHotSpinBox = new QDoubleSpinBox;
            densityExitHotSpinBox->setRange(500, 2000);
            densityExitHotSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* densityExitColdSpinBox = new QDoubleSpinBox;
            densityExitColdSpinBox->setRange(0, 10);
            densityExitColdSpinBox->setSingleStep(0.1);
            QWidget* densityExitContainer = createPairWidget(densityExitHotSpinBox, densityExitColdSpinBox);

            QLabel* speedEntranceLabel = new QLabel(u8"Скорость среды на входе (max: 7), м/с");
            QDoubleSpinBox* speedEntranceHotSpinBox = new QDoubleSpinBox;
            speedEntranceHotSpinBox->setSingleStep(0.1);
            speedEntranceHotSpinBox->setRange(0, 7);
            QDoubleSpinBox* speedEntranceColdSpinBox = new QDoubleSpinBox;
            speedEntranceColdSpinBox->setSingleStep(0.1);
            speedEntranceColdSpinBox->setRange(0, 7);
            QWidget* speedEntranceContainer = createPairWidget(speedEntranceHotSpinBox, speedEntranceColdSpinBox);

            QLabel* speedExitLabel = new QLabel(u8"Скорость среды на выходе (max: 10), м/с");
            QDoubleSpinBox* speedExitHotSpinBox = new QDoubleSpinBox;
            speedExitHotSpinBox->setSingleStep(0.1);
            speedExitHotSpinBox->setRange(0, 10);
            QDoubleSpinBox* speedExitColdSpinBox = new QDoubleSpinBox;
            speedExitColdSpinBox->setSingleStep(0.1);
            speedExitColdSpinBox->setRange(0, 10);
            QWidget* speedExitContainer = createPairWidget(speedExitHotSpinBox, speedExitColdSpinBox);

            // для теплового ! 
            QLabel* temperatureEntranceLabel = new QLabel(u8"Температура среды на входе, ⁰С");
            QDoubleSpinBox* temperatureEntranceHotSpinBox = new QDoubleSpinBox;
            temperatureEntranceHotSpinBox->setRange(0, 150);
            temperatureEntranceHotSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* temperatureEntranceColdSpinBox = new QDoubleSpinBox;
            temperatureEntranceColdSpinBox->setRange(0, 150);
            temperatureEntranceHotSpinBox->setSingleStep(0.1);
            QWidget* temperatureEntranceContainer = createPairWidget(temperatureEntranceHotSpinBox, temperatureEntranceColdSpinBox);

            QLabel* temperatureExitLabel = new QLabel(u8"Температура среды на выходе,⁰С");
            QDoubleSpinBox* temperatureExitHotSpinBox = new QDoubleSpinBox;
            temperatureExitHotSpinBox->setRange(0, 150);
            temperatureExitHotSpinBox->setSingleStep(0.1);
            QDoubleSpinBox* temperatureExitColdSpinBox = new QDoubleSpinBox;
            temperatureExitColdSpinBox->setSingleStep(0.1);
            temperatureExitColdSpinBox->setRange(0, 150);
            QWidget* temperatureExitContainer = createPairWidget(temperatureExitHotSpinBox, temperatureExitColdSpinBox);

            QLabel* consumptionColdLabel = new QLabel(u8"Массовый расход нагреваемого теплоносителя (max: 10), кг/с");
            QDoubleSpinBox* consumptionColdSpinBox = new QDoubleSpinBox;
            consumptionColdSpinBox->setSingleStep(0.1);
            consumptionColdSpinBox->setRange(2, 10);

            densityEntranceHotSpinBox->setValue(870.);
            densityEntranceColdSpinBox->setValue(0.99);
            densityExitHotSpinBox->setValue(899.);
            densityExitColdSpinBox->setValue(0.87);
            speedEntranceHotSpinBox->setValue(6);
            speedEntranceColdSpinBox->setValue(4);
            speedExitHotSpinBox->setValue(7);
            speedExitColdSpinBox->setValue(5);
            temperatureEntranceHotSpinBox->setValue(120);
            temperatureEntranceColdSpinBox->setValue(50);
            temperatureExitHotSpinBox->setValue(60);
            temperatureExitColdSpinBox->setValue(70);
            consumptionColdSpinBox->setValue(3);


            QPushButton* calculationButton = new QPushButton(u8"Выполнить расчёт");

            QLabel* reinoldsLabel = new QLabel(u8"Число Рейнольдса, Re:");
            QLineEdit* reinoldsHEdit = new QLineEdit;
            reinoldsHEdit->setReadOnly(true);
            QLineEdit* reinoldsCEdit = new QLineEdit;
            reinoldsCEdit->setReadOnly(true);
            QWidget* reinoldsContainer = createPairWidget(reinoldsHEdit, reinoldsCEdit);

            QLabel* squareSectionLabel = new QLabel(u8"Площадь поперечного сечения корпуса,  м\xB2:");
            QLineEdit* squareSectionEdit = new QLineEdit;
            squareSectionEdit->setReadOnly(true);

            QLabel* squareInterTubeLabel = new QLabel(u8"Площадь межтрубного пространства,  м\xB2:");
            QLineEdit* squareInterTubeEdit = new QLineEdit;
            squareInterTubeEdit->setReadOnly(true);




            QLabel* speedInterTubeLabel = new QLabel(u8"Скорость воды в межтрубном пространстве, м/c:");
            QLineEdit* speedInterTubeEdit = new QLineEdit;
            speedInterTubeEdit->setReadOnly(true);


            QLabel* resistanceCoefficientLabel = new QLabel(u8"Коэффициент сопротивления трения, λ:");
            QLineEdit* resistanceCoefficientHotEdit = new QLineEdit;
            resistanceCoefficientHotEdit->setReadOnly(true);
            QLineEdit* resistanceCoefficientColdEdit = new QLineEdit;
            resistanceCoefficientColdEdit->setReadOnly(true);
            QWidget* resistanceCoefficientContainer = createPairWidget(resistanceCoefficientHotEdit, resistanceCoefficientColdEdit);

            QLabel* frictionPressureLabel = new QLabel(u8"Потери давления на трение, ∆Pтр:");
            QLineEdit* frictionPressureHotEdit = new QLineEdit;
            frictionPressureHotEdit->setReadOnly(true);
            QLineEdit* frictionPressureColdEdit = new QLineEdit;
            frictionPressureColdEdit->setReadOnly(true);
            QWidget* frictionPressureContainer = createPairWidget(frictionPressureHotEdit, frictionPressureColdEdit);

            QLabel* localResistancePressureLabel = new QLabel(u8"Потери давления в местных сопротивлениях, ∆Pм:");
            QLineEdit* localResistanceHotEdit = new QLineEdit;
            localResistanceHotEdit->setReadOnly(true);
            QLineEdit* localResistanceColdEdit = new QLineEdit;
            localResistanceColdEdit->setReadOnly(true);
            QWidget* localResistancePressureContainer = createPairWidget(localResistanceHotEdit, localResistanceColdEdit);

            QLabel* accelerationFlowPressureLabel = new QLabel(u8"Потери при ускорении потока, ∆Pу:");
            QLineEdit* accelerationFlowHotEdit = new QLineEdit;
            accelerationFlowHotEdit->setReadOnly(true);
            QLineEdit* accelerationFlowColdEdit = new QLineEdit;
            accelerationFlowColdEdit->setReadOnly(true);
            QWidget* accelerationFlowPressureContainer = createPairWidget(accelerationFlowHotEdit, accelerationFlowColdEdit);

            QLabel* pressureDropLabel = new QLabel(u8"Перепад давления, ∆Pг:");
            QLineEdit* pressureDropHotEdit = new QLineEdit;
            pressureDropHotEdit->setReadOnly(true);
            QLineEdit* pressureDropColdEdit = new QLineEdit;
            pressureDropColdEdit->setReadOnly(true);
            QWidget* pressureDropContainer = createPairWidget(pressureDropHotEdit, pressureDropColdEdit);

            QLabel* fullPressureLabel = new QLabel(u8"Полные потери давления, ∆P:");
            QLineEdit* fullPressureHotEdit = new QLineEdit;
            fullPressureHotEdit->setReadOnly(true);
            QLineEdit* fullPressureColdEdit = new QLineEdit;
            fullPressureColdEdit->setReadOnly(true);
            QWidget* fullPressureContainer = createPairWidget(fullPressureHotEdit, fullPressureColdEdit);

            QLabel* averageTemperatureLabel = new QLabel(u8"Средняя температура, ⁰С:");
            QLineEdit* averageTemperatureHotEdit = new QLineEdit;
            averageTemperatureHotEdit->setReadOnly(true);
            QLineEdit* averageTemperatureColdEdit = new QLineEdit;
            averageTemperatureColdEdit->setReadOnly(true);
            QWidget* averageTemperatureContainer = createPairWidget(averageTemperatureHotEdit, averageTemperatureColdEdit);

            QLabel* heatLabel = new QLabel(u8"Количество теплоты, кДж:");
            QLineEdit* heatEdit = new QLineEdit;
            heatEdit->setReadOnly(true);

            QLabel* consumptionHotLabel = new QLabel(u8"Массовый расход греющего теплоносителя, кг/с:");
            QLineEdit* consumptionHotEdit = new QLineEdit;
            consumptionHotEdit->setReadOnly(true);


            formLayout->addRow(headInfoLabel, infoContainer);
            formLayout->addRow(typeLabel, typeContainer);
            formLayout->addRow(densityEntranceLabel, densityEntranceContainer);
            formLayout->addRow(densityExitLabel, densityExitContainer);
            formLayout->addRow(speedEntranceLabel, speedEntranceContainer);
            formLayout->addRow(speedExitLabel, speedExitContainer);

            formLayout->addRow(temperatureEntranceLabel, temperatureEntranceContainer);
            formLayout->addRow(temperatureExitLabel, temperatureExitContainer);
            formLayout->addRow(consumptionColdLabel, consumptionColdSpinBox);

            formLayout->addRow(calculationButton);

            formLayout->addRow(reinoldsLabel, reinoldsContainer);
            formLayout->addRow(squareSectionLabel, squareSectionEdit);
            formLayout->addRow(squareInterTubeLabel, squareInterTubeEdit);
            formLayout->addRow(speedInterTubeLabel, speedInterTubeEdit);
            formLayout->addRow(resistanceCoefficientLabel, resistanceCoefficientContainer);
            formLayout->addRow(frictionPressureLabel, frictionPressureContainer);
            formLayout->addRow(localResistancePressureLabel, localResistancePressureContainer);
            formLayout->addRow(accelerationFlowPressureLabel, accelerationFlowPressureContainer);
            formLayout->addRow(pressureDropLabel, pressureDropContainer);
            formLayout->addRow(fullPressureLabel, fullPressureContainer);
            formLayout->addRow(averageTemperatureLabel, averageTemperatureContainer);
            formLayout->addRow(heatLabel, heatEdit);
            formLayout->addRow(consumptionHotLabel, consumptionHotEdit);


            connect(calculationButton, &QPushButton::clicked, [=]() {

                //индексы сред
                const int hotFluidIndex = hotFluidComboBox->currentIndex();
                const int coldFluidIndex = coldFluidComboBox->currentIndex() + 5;//чтобы в одной структуре хранить

                double temp1_hot = temperatureEntranceHotSpinBox->value();//температура на входе
                double temp2_hot = temperatureExitHotSpinBox->value();//температура на выходе
                double temp1_cold = temperatureEntranceColdSpinBox->value();//температура на входе
                double temp2_cold = temperatureExitColdSpinBox->value();//температура на выходе

                if (temp1_hot < temp1_cold)
                {
                    QString* text = new QString(u8"Температура теплоносителя в межтрубном пространстве на входе должна быть меньше, чем в трубном на входе.");
                    createWarning(text);

                    return;
                }

                if (temp1_hot < temp2_hot)
                {
                    QString* text = new QString(u8"Температура теплоносителя в трубном пространстве на входе должна быть больше, чем на выходе.");
                    createWarning(text);

                    return;
                }

                if (temp1_cold > temp2_cold)
                {
                    QString* text = new QString(u8"Температура теплоносителя в межтрубном пространстве на входе должна быть меньше, чем на выходе.");
                    createWarning(text);

                    return;
                }
                // if (temp2_hot < temp2_cold)
               //  {
               //      QString* text = new QString(u8"Температура нагреваемого теплоносителя на выходе должна быть меньше, чем греющего на выходе.");
               //      createWarning(text);

               //      return;
               //  }


                if (temp1_hot <= 0)
                {
                    QString* text = new QString(u8"Температура теплоносителя в трубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                double temp_hot_av = (temp1_hot + temp2_hot) / 2; //средняя температура
                double temp_cold_av = (temp1_cold + temp2_cold) / 2;
                int temp_hot_index = round(temp_hot_av / 10);
                int temp_cold_index = round(temp_cold_av / 10);

                //тепловой
                double capacity_hot = fluidsProperties_IU_IP[hotFluidIndex].c; //теплоемкость греющей среды
                double capacity_cold = fluidsProperties_IU_IP[coldFluidIndex].c;//теплоемколсть нагреваемой среды
                float N = 0.97; //коэффициент потери теплоты в окр.среду
                double Gr_cold = consumptionColdSpinBox->value(); //расход теплоносителя в кг/c



                double t_g_sr = (temp1_hot + temp2_hot) / 2;
                double t_n_sr = (temp1_cold + temp2_cold) / 2;
                double Q = (Gr_cold * capacity_cold * (temp2_cold - temp1_cold)) / 1000;
                double Gr_hot = Q * 1000 / (capacity_hot * (temp1_hot - temp2_hot) * N);

                //определение какую структура использовать: типовую или пользовательскую
                const int technicalIndex = m_comboConfigure->currentIndex();
                const ConfigParams_IP params = isCheckedManualType
                    ? manualIPParams
                    : dataIP[technicalIndex > 0 ? technicalIndex : 0];

                double n;//количество труб
                double delta_h_hot;//разница уровней входа и выхода теплоносителя в систему
                if ((500 <= params.DKr) && (params.DKr < 600)) {
                    n = 208.;
                    delta_h_hot = 884. / 1000;//в метрах
                }
                else if ((600 <= params.DKr) && (params.DKr < 700)) {
                    n = 310.;
                    delta_h_hot = 984. / 1000;//м
                }
                else if ((700 <= params.DKr) && (params.DKr < 800)) {
                    n = 452.;
                    delta_h_hot = 1088. / 1000;
                }
                else if ((800 <= params.DKr) && (params.DKr < 900)) {
                    n = 590.;
                    delta_h_hot = 1188. / 1000;
                }
                else if ((900 <= params.DKr) && (params.DKr < 1000)) {
                    n = 778.;
                    delta_h_hot = 1332. / 1000;
                }
                else  if ((1000 <= params.DKr) && (params.DKr < 1100)) {
                    n = 980.;
                    delta_h_hot = 1392. / 1000;
                }
                else if ((1100 <= params.DKr) && (params.DKr < 1200)) {
                    n = 1210.;
                    delta_h_hot = 1492. / 1000;
                }
                else if (1200 <= params.DKr) {
                    n = 1466.;
                    delta_h_hot = 1694. / 1000;
                }


                //считаем число рейнольдса
                double d = 20. / 1000.; //диаметр труб в метрах
                double density_hot = fluidDensity_IU_IP[hotFluidIndex][temp_hot_index];//плотность в кг/м3
                double density_cold = fluidDensity_IU_IP[coldFluidIndex][temp_cold_index];//плотность в кг/м3
                //////////////////////посчитать из теплового

                double viscosity_hot = fluidViscocity_IU_IP[hotFluidIndex][temp_hot_index] * pow(10, -6); // вязкость
                double viscosity_cold = fluidViscocity_IU_IP[coldFluidIndex][temp_cold_index] * pow(10, -6); // вязкость

                double Fd = M_PI * pow(params.Dv_Kzh / 1000., 2) / 4; //площадь поперечного сечения корпуса
                double fm = (M_PI * pow(d, 2) * n) / 4; //площадь, занятая трубами
                double f1 = Fd - fm; //площадь межрубного пространства

                double w_cold = Gr_cold / (f1 * density_cold); //скорость среды в межтрубном пространстве
                double d_e = 4. * f1 / (M_PI * (params.Dv_Kzh / 1000. + n * d));
                double Re_cold = w_cold * d_e / viscosity_cold;//число Рейнольдса в межтрубном пространстве

                double w_hot = Gr_hot / (fm * density_hot); //скорость воды в трубном пространстве
               // double w_hot =

                double v1_hot = speedEntranceHotSpinBox->value(); // скорость на входе
                double v2_hot = speedExitHotSpinBox->value(); // скорость на выходе
                double v1_cold = speedEntranceColdSpinBox->value(); // скорость на входе
                double v2_cold = speedExitColdSpinBox->value(); // скорость на выходе
                double density1_hot = densityEntranceHotSpinBox->value(); // плотность на входе 
                double density2_hot = densityExitHotSpinBox->value(); //плотность на выходе
                double density1_cold = densityEntranceColdSpinBox->value(); // плотность на входе 
                double density2_cold = densityExitColdSpinBox->value(); //плотность на выходе
                double v_sr_hot = (v1_hot + v2_hot) / 2; //средняя скорость 
                double v_sr_cold = (v1_cold + v2_cold) / 2; //средняя скорость 

                if (v2_hot < v1_hot)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в трубном пространстве на выходе должна быть больше, чем на входе.");
                    createWarning(text);

                    return;
                }

                if (v2_cold < v1_cold)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в межтрубном пространстве на выходе должна быть больше, чем на входе.");
                    createWarning(text);

                    return;
                }

                if (density1_hot > density2_hot)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в трубном пространстве на входе должна быть меньше, чем на выходе.");
                    createWarning(text);

                    return;
                }

                if (density1_cold < density2_cold)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в межтрубном пространстве на входе должна быть больше, чем на выходе.");
                    createWarning(text);

                    return;
                }

                if (v2_hot <= 0)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в трубном пространстве на выходе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (v1_hot <= 0)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в трубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (v2_cold <= 0)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в межтрубном пространстве на выходе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (v1_cold <= 0)
                {
                    QString* text = new QString(u8"Скорость теплоносителя в межтрубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (density1_cold <= 0)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в межтрубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (density2_cold <= 0)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в межтрубном пространстве на выходе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (density1_hot <= 0)
                {
                    QString* text = new QString(u8"Плотность теплоносителя  в трубном пространстве на входе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                if (density2_hot <= 0)
                {
                    QString* text = new QString(u8"Плотность теплоносителя в трубном пространстве на выходе должна быть больше 0.");
                    createWarning(text);

                    return;
                }

                double Re_hot = w_hot * d / viscosity_hot;

                double L = 6000. / 1000. * n; //суммарная длина трубок
                //double d_v = ; //внутренний диаметр трубок
                double density_atm = 1.2754; //плотность атмосферного воздуха
                //разница уровней входа и выхода теплоносителя в систему
                double delta_h_cold = (380 + params.Dv_Kzh) / 1000; //разница уровней входа и выхода теплоносителя в систему, 380 - сумма высот фланцев
                double lambda_cold = 1 / pow((1.8 * log(Re_cold) - 1.5), 2); //коэффициент сопротивления трения

                double e = 5.5;//коэффициент местного сопротивления

                double Pt_cold = lambda_cold * L * pow(v_sr_cold, 2) * density1_cold / (2 * d_e); //потери на трение d->d_v
                double Pm_cold = e * pow(v_sr_cold, 2) * density_cold / 2; //потери в местных сопротивлениях

                double Py_cold = (density2_cold * v2_cold - density1_cold * v1_cold); //потери при ускорении потока

                double Pg_cold = (density1_cold - density_atm) * -delta_h_cold; //перепад давления

                double P_cold = Pt_cold + Pm_cold + Py_cold + Pg_cold; //полное давление


                double lambda_hot = 1 / pow((1.8 * log(Re_hot) - 1.5), 2); //коэффициент сопротивления трения (логарифм надо исправить!)
                double Pt_hot = (lambda_hot * L * pow(v_sr_hot, 2) * density1_hot / (2 * (params.Dv_Kzh / 1000))) / pow(10, 6); //потери на трение d->d_v
                double Pm_hot = (5.5 * pow(v_sr_hot, 2) * density_hot / 2) / pow(10, 6); //потери в местных сопротивлениях

                double Py_hot = (density2_hot * v2_hot - density1_hot * v1_hot) / pow(10, 6); //потери при ускорении потока

                double Pg_hot = ((density1_hot - density_atm) * delta_h_hot) / pow(10, 6); //перепад давления

                double P_hot = Pt_hot + Pm_hot + Py_hot + Pg_hot; //полное давление

                reinoldsHEdit->setText(QString::number(Re_hot));
                reinoldsCEdit->setText(QString::number(Re_cold));



                squareSectionEdit->setText(QString::number(Fd) + u8" м\xB2");//Площадь поперечного сечения корпуса
                squareInterTubeEdit->setText(QString::number(f1) + u8" м\xB2");//Площадь межтрубного пространства
                speedInterTubeEdit->setText(QString::number(w_cold) + u8" м/c");//Скорость воды в межтрубном пространстве
                resistanceCoefficientHotEdit->setText(QString::number(lambda_hot) + u8" ");//Коэффициент сопротивления трения
                resistanceCoefficientColdEdit->setText(QString::number(lambda_cold) + u8" ");

                frictionPressureHotEdit->setText(QString::number(Pt_hot) + u8" МПа");//Потери давления на трение
                frictionPressureColdEdit->setText(QString::number(Pt_cold) + u8" Па");//
                localResistanceHotEdit->setText(QString::number(Pm_hot) + u8" МПа");//Потери давления в местных сопротивлениях
                localResistanceColdEdit->setText(QString::number(Pm_cold) + u8" Па");
                accelerationFlowHotEdit->setText(QString::number(Py_hot) + u8" МПа");//Потери при ускорении потока
                accelerationFlowColdEdit->setText(QString::number(Py_cold) + u8" Па");
                pressureDropHotEdit->setText(QString::number(Pg_hot) + u8" МПа");//Перепад давления
                pressureDropColdEdit->setText(QString::number(Pg_cold) + u8" Па");
                fullPressureHotEdit->setText(QString::number(P_hot) + u8" МПа");//Полное давления
                fullPressureColdEdit->setText(QString::number(P_cold) + u8" Па");
                averageTemperatureHotEdit->setText(QString::number(temp_hot_av) + u8" °C");//Средняя температура
                averageTemperatureColdEdit->setText(QString::number(temp_cold_av) + u8" °C");
                heatEdit->setText(QString::number(Q) + u8" кДж");//количество теплоты
                consumptionHotEdit->setText(QString::number(Gr_hot) + u8" кг/с"); //Массовый расход греющего теплоносителя
            }
            );



            m_calculationTab->setLayout(formLayout);
            break;
        }
        default:
            break;
    }
} 


bool ExplodeManager::checkValidate() {
    if ((m_pExplodeWidget->m_pCurrentExchandger == 4 && mp_IP_D_Kzh <= mp_IP_D_Kam) || (m_pExplodeWidget->m_pCurrentExchandger == 3 && mp_IU_D_Kzh <= mp_IU_D_Kam))
    {
        QMessageBox::warning(nullptr, u8"Предупреждение", u8"Диаметр кожуха должен быть больше диаметра камеры");
        return false;
    }

    if ((m_pExplodeWidget->m_pCurrentExchandger == 4 && mp_IP_D_l2 > mp_IP_D_l3) || (m_pExplodeWidget->m_pCurrentExchandger == 3 && mp_IU_D_l2 > mp_IU_D_l3))
    {
        QMessageBox::warning(nullptr, u8"Предупреждение", u8"l2 должна быть меньше, чем l3");
        return false;
    }

    return true;
}

void ExplodeManager::onDrawingShowButtonClicked() {
    m_PmodalDialog = new QDialog();

    const int indexOfCurrentExchanger = m_pExplodeWidget->m_pCurrentExchandger;
    const char* windowTittle = nullptr;
    const char* iconPath = nullptr;

    switch (indexOfCurrentExchanger) {
    case 1:
    {
        windowTittle = u8"Чертёж теплообменного аппарата ТТОР";
        iconPath = ":/res/draws/ttor.png";
        break;
    }case 2:
    {
        windowTittle = u8"Размеры теплообменника ТТРМ";
        iconPath = ":/res/draws/ttrm.png";
        break;
    }case 3:
    {
        windowTittle = u8"Размеры теплообменника IU";
        iconPath = ":/res/draws/iu.jpg";
        break;
    }case 4:
    {
        windowTittle = u8"Размеры теплообменника IP";
        iconPath = ":/res/draws/ip.jpg";
        break;
    }
    }
    m_PmodalDialog->setWindowTitle(windowTittle);
    m_PmodalDialog->setWindowIcon(QIcon(":/res/dimensions.png"));

    QLabel* imageLabel = new QLabel;
    QPixmap image(iconPath);
    imageLabel->setPixmap(image);
    imageLabel->setScaledContents(true); // Масштабировать изображение по размеру QLabel\

    QRect screenGeometry = m_PmodalDialog->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    if (indexOfCurrentExchanger == 3 || indexOfCurrentExchanger == 4) screenWidth += 300;

    imageLabel->setMaximumSize(screenWidth, screenHeight);

    QVBoxLayout* modalLayout = new QVBoxLayout(m_PmodalDialog);
    modalLayout->addWidget(imageLabel);
    m_PmodalDialog->exec();
}

void ExplodeManager::onReconfigureButtonClicked() {
    const int indexOfCurrentExchanger = m_pExplodeWidget->m_pCurrentExchandger;
    if (isCheckedManualType && indexOfCurrentExchanger ==2) {
        ConfigParams lengthParams = findClosestMatch(0, m_lengthSpinBox->value(), "LENGTH");
        manualTTRMParams = findClosestMatch(0, mp_dimCamera, "assortmentCamera");

        manualTTRMParams.thicknessInnerTubes = mp_thicknessInnerResult;
        manualTTRMParams.thicknessOuterTubes = mp_thicknessOuterResult;      
        manualTTRMParams.LENGTH = m_lengthSpinBox->value();
        manualTTRMParams.length0 = lengthParams.length0;
        manualTTRMParams.length1 = lengthParams.length1;
        manualTTRMParams.length2 = lengthParams.length2;
        manualTTRMParams.length3 = lengthParams.length3;
        manualTTRMParams.lengthK = lengthParams.lengthK;
    }

    if (isCheckedManualType && indexOfCurrentExchanger == 1) {
        const int index = m_comboConfigure->currentIndex();
        BuildParamsForHeatExchangerTTOR config = dataTTOR[index > 0 ? index : 0];
        manualTTORParams = config;

        manualTTORParams.ttDiam = diametrTubeTTOR->value();
        manualTTORParams.ktDiam = diametrKozhuxTTOR->value();
        manualTTORParams.ttThickness = thicknessTubeTTOR->value();
        manualTTORParams.ktThickness = thicknessKozhuxTTOR->value();
        manualTTORParams.l0 = distanceOporiTTOR->value();
        manualTTORParams.l3 = dlinaKameriTTOR->value();
        manualTTORParams.H1 = visotaOporyH1TTOR->value();
        manualTTORParams.widthFlanec = visotaOporyH2TTOR->value();
        manualTTORParams.Lt = LengthTeplTubeTTOR->value();
        manualTTORParams.lK = LengthKozhuxTubeTTOR->value();
        manualTTORParams.l2 = distanceConnectorTTOR->value();
        manualTTORParams.dU = dUForFlanecTTOR->value();
        manualTTORParams.turnOnStandart = turnOnStandartDetailsTTOR->checkState() == 2 ? true : false;
        manualTTORParams.simpleMode = simpleModeStandartTTOR->checkState() == 2 ? true : false;
    }


    if (isCheckedManualType && indexOfCurrentExchanger == 3) {

        manualIUParams.p = mp_IU_p;
        manualIUParams.diam = mp_IU_D_Kzh;
        manualIUParams.D_Kam = mp_IU_D_Kam;
        manualIUParams.l = mp_IU_D_l;
        manualIUParams.l2 = mp_IU_D_l2;
        manualIUParams.l3 = mp_IU_D_l3;
    }

    if (isCheckedManualType && indexOfCurrentExchanger == 4) {
        manualIPParams.p = mp_IP_p;
        manualIPParams.Dv_Kzh = mp_IP_D_Kzh;
        manualIPParams.DKr = mp_IP_D_Kam;
        manualIPParams.l0 = mp_IP_D_l;
        manualIPParams.l2 = mp_IP_D_l2;
        manualIPParams.l3 = mp_IP_D_l3;
    }

    if (m_pExplodeWidget) {
        QVariant propertyValue = m_reconfigureButton->property("CommandsHeatExhanger");
        ExplodeWidget::Exhanchares cmd = static_cast<ExplodeWidget::Exhanchares>(propertyValue.toInt());

        m_pExplodeWidget->viewCommandsHeats(cmd);
    }


}

double ExplodeManager::calculateDirtyEnviroment(double dVN, double dNar) {
    double q1 = m_PresultFieldZagryaznenieQ1->value();
    double q2 = m_PresultFieldZagryaznenieQ2->value();
    double h1 = m_PresultFieldZagryaznenieH1->value();
    double h2 = m_PresultFieldZagryaznenieH2->value();

    return (q1 / h1) * (dNar / dVN) + (q2 / h2);
}

double ExplodeManager::calculateKParamTTOR(double dvn, double dcp, double dnar, double Rzag, double a1, double a2) {
    int index = m_PmaterialCombobox->currentIndex();
    data_materialProperties material = materialProperties[index];

    double calc1 = 1/(a1 * dvn);
    double calc2 = 1/(2 * material.laymbdaMateral);
    double calc3 = log(dnar/ dvn);
    double calc4 = 1/(a2* dnar);

    double calc5 = calc1 + calc2 * calc3 + calc4;

    double finalCalc = dcp * calc5 + Rzag;

    return 1/finalCalc;
}

double ExplodeManager::calculateAParamTTOR(double d, bool isCold, double G) {
    int index;
    if (isCold) {
        index = m_PcoldFluidComboBox->currentIndex();;
    }
    else {
        index = m_PhotFluidComboBox->currentIndex();;
    }

    data_fluidProperties veshestvo = fluidsProperties[index];
    double u = veshestvo.u_viscocity || 0.3;
    double uw = veshestvo.viscocity || 0.4;

    double firstCalc = veshestvo.laymbda / d;
    double secondCalc = pow( G * d / u, 0.8);
    double thirdCalc = pow(u / uw, 1.4 );

    double finalCalc = 0.023 * firstCalc * secondCalc * thirdCalc;
        
    return finalCalc;
}

double ExplodeManager::calculateFParamTTOR(double d, double dnar, double index) {
    int n = 4;
    if (index > 0) {
        if (index == 1) {
            n = 8;
        }if (index == 2) {
            n = 12;
        }if (index == 3) {
            n = 16;
        }
    }

    double result;

    if (dnar) {
        result = (M_PI * pow(dnar, 2) * n / 4) - M_PI * pow(d, 2) * n / 4;
    }
    else {
        result = M_PI * pow(d, 2) * n / 4;
    }

    return result;
}

double ExplodeManager::calculateGParamTTOR(double F, bool isCold) {
    int index;
    double w;
    if (isCold) {
        index = m_PcoldFluidComboBox->currentIndex();
        w = m_PhotVelocity->value();
    }
    else {
        index = m_PhotFluidComboBox->currentIndex();
        w = m_PcoldVelocity->value();
    }

    data_fluidProperties veshestvo = fluidsProperties[index];
    double p = veshestvo.p;

    return F * p * w;
}

bool ExplodeManager::onCalculationButtonClicked() {
    double hotInletTemp = m_PhotInletTemp->text().toDouble();
    double coldInletTemp = m_PcoldInletTemp->text().toDouble();

    int index = m_comboConfigure->currentIndex();

    BuildParamsForHeatExchangerTTOR config = dataTTOR[index > 0 ? index : 0];

   // check form надо TOZO

    double Rzag = 0;
    double Q = 0;

    if (turnOnZagryaznenieTTOR->isChecked()) {
        Rzag = calculateDirtyEnviroment(config.ttDiam, config.ttDiam + 2 * config.ttThickness);
    }

    double Ftt = calculateFParamTTOR(config.ttDiam, 0, index);
    double Fkt = calculateFParamTTOR(config.ttDiam + 2 * config.ttThickness, config.ktDiam, index);

    double G1 = calculateGParamTTOR(Ftt, false);
    double G2 = calculateGParamTTOR(Fkt, true);

    double a1 = calculateAParamTTOR(config.ttDiam, false, G1);
    double a2 = calculateAParamTTOR(config.ktDiam, true, G2);
    double K = calculateKParamTTOR(config.ttDiam, config.ttDiam + 2 * config.ttThickness, config.ktDiam, Rzag, a1, a2);

    double valueHotField = m_PrresultTemp1->value();
    double valueColdField = m_PrresultTemp2->value();
    double t1;
    double t2;
    bool isCold = false;

    int indexW;

    if (hotInletTemp == 0 && coldInletTemp == 0) {
        QMessageBox::warning(nullptr, u8"Пустые значения температуры", u8"Заполните значения температуры на входе");

        return false;
    }
    if (valueHotField == 0 && valueColdField == 0) {
        QMessageBox::warning(nullptr, u8"Пустые значения температуры", u8"Заполните значения температуры на выходе");

        return false;
    }
    if (m_PcoldVelocity->value() == 0 && m_PhotVelocity->value() == 0) {
        QMessageBox::warning(nullptr, u8"Пустые значения скорости", u8"Заполните значения скорости теплоносителей");

        return false;
    }

    if (valueHotField) {
        indexW = m_PhotFluidComboBox->currentIndex();
        data_fluidProperties veshestvo = fluidsProperties[index];
        double c = veshestvo.c;

        Q = c * G1 * ( m_PhotInletTemp->value() - valueHotField);

        t1 = valueHotField;       

        t2 = Q / G2 / c + m_PcoldInletTemp->value();

    }
    if (valueColdField) {
        indexW = m_PcoldFluidComboBox->currentIndex();
        data_fluidProperties veshestvo = fluidsProperties[index];
        double c = veshestvo.c;

        Q = c * G2 * ( m_PcoldInletTemp->value() - valueColdField);

        t2 = valueColdField;

        t1 = Q / G1 / c + m_PhotInletTemp->value();
    }

    data_fluidProperties veshestvo = fluidsProperties[index];
    double p = veshestvo.p;

    m_ForTrresultTemp1->setText(QString::number(t1));
    m_ForTrresultTemp2->setText(QString::number(t2));
    m_ForGrresultTemp1->setText(QString::number(G1, 'f', 1));
    m_ForGrresultTemp2->setText(QString::number(G2, 'f', 1));
    m_ForFrresultTemp1->setText(QString::number(Ftt));
    m_ForFrresultTemp2->setText(QString::number(Fkt));
    m_ForArresultTemp1->setText(QString::number(a1));
    m_ForArresultTemp2->setText(QString::number(a2));

    m_ForQrresultTemp->setText(QString::number(Q, 'f', 1));
    m_ForKrresultTemp->setText(QString::number(K));
    m_ForRZrresultTemp->setText(QString::number(Rzag));

    return true;
    // QMessageBox::warning(nullptr, u8"Неверные значения температуры", u8"Расчет не может быть выполнен. Пожалуйста укажите правильные значения температуры теплоносителей");
    // QMessageBox::information(nullptr, u8"Успешно", u8"Расчеты выполнены правильно.");
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

    QAction* bodyFilterButton = createActionButton(":/res/renderMods/dimmedWireframe.png", gr_WRendering, 
        fGroupLayout);
    bodyFilterButton->setToolTip(QStringLiteral("Обычный"));
    bodyFilterButton->setChecked(true);

    actionGroupFilter->addAction(bodyFilterButton);
    actionGroupFilter->addAction(createActionButton(":/res/renderMods/shaded.png", gr_WRendering, 
        fGroupLayout))->setToolTip(QStringLiteral("Обычный ьез отображения граней"));
    actionGroupFilter->addAction(createActionButton(":/res/renderMods/wirefrm.png", gr_WRendering, 
        fGroupLayout))->setToolTip(QStringLiteral("Каркасный"));
    actionGroupFilter->addAction(createActionButton(":/res/renderMods/hiddenremoved.png", gr_WRendering, 
        fGroupLayout))->setToolTip(QStringLiteral("Чертежный"));

    m_pExplodeWidget->setGroupFilter(actionGroupFilter);
    QObject::connect(actionGroupFilter, SIGNAL(triggered(QAction*)), m_pExplodeWidget, 
        SLOT(slotRenderTriggered(QAction*)));


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
    sizeInfoButton->setToolTip(u8"Посмотреть размеры аппарата");
    m_vLayoutConfigureTab->addWidget(sizeInfoButton);

    connect(sizeInfoButton, &QToolButton::clicked, this, &ExplodeManager::onDrawingShowButtonClicked);

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