#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

void CreateSketchCurvedPipe(RPArray<MbContour>& _arrContours, double ttDiam, double ttThickness, double t)
{
    // Построение образующей в виде двух концентрических окружностей
    const double RAD_EXT = ttDiam / 2; // Радиус внешней окружности
    const double RAD_INT = (ttDiam - 2 * ttThickness) / 2; // Радиус внутренней окружности

    //По оси Y сдвинута вверх (параметр t/2)
    MbArc* pCircleExt = new MbArc(MbCartPoint(0, t / 2), RAD_EXT);
    MbArc* pCircleInt = new MbArc(MbCartPoint(0, t / 2), RAD_INT);

    _arrContours.push_back(new MbContour(*pCircleExt, true));
    _arrContours.push_back(new MbContour(*pCircleInt, true));
}

SPtr<MbSolid> ParametricModelCreator::createCurvedPipe_007(double ttDiam, double ttThickness, double t)
{

    // Создание объекта MbPlacement3D для определения положения профиля
    MbPlacement3D* plLine = new MbPlacement3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 0, 1), MbCartPoint3D(0, 1, 0));

    // Рассчитываем половину длины трубы
    const double PL = t / 2;

    // Создание вертикального отрезка пути
    MbLineSegment* pL12D = new MbLineSegment(MbCartPoint(0, PL), MbCartPoint(20, PL));
    MbLineSegment3D* pL1 = new MbLineSegment3D(*pL12D, *plLine);

    // Создание дуги
    MbArc* pA12D = new MbArc(MbCartPoint(20, 0), MbCartPoint(20, PL), MbCartPoint(20, -PL), -1);
    MbArc3D* pA1 = new MbArc3D(*pA12D, *plLine);

    // Создание горизонтального отрезка пути
    MbLineSegment* pL32D = new MbLineSegment(MbCartPoint(20, -PL), MbCartPoint(0, -PL));
    MbLineSegment3D* pL3 = new MbLineSegment3D(*pL32D, *plLine);

    // Создание контура трубы
    MbContour3D* pLine = new MbContour3D();
    pLine->AddSegment(*pL1, false); // Добавляем вертикальный отрезок
    pLine->AddSegment(*pA1, false); // Добавляем дугу
    pLine->AddSegment(*pL3, false); // Добавляем горизонтальный отрезок

    // Создание массива контуров для трубы
    RPArray<MbContour> arrContours;
    CreateSketchCurvedPipe(arrContours, ttDiam, ttThickness, t); // Создаем контур трубы

    // Создание плоскости XY для создания операции вращения
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));

    // Создание данных для операции вращения
    MbSweptData sweptData(*pPlaneXY, arrContours);

    // Настройка параметров эволюции
    EvolutionValues evParms;
    evParms.SetKeepingAngle(); // Установка сохранения угла
    evParms.SetBySurfaceNormal(false); // Установка по нормали к поверхности

    // Создание именователей для операций
    MbSNameMaker operNames(ct_CurveEvolutionSolid, MbSNameMaker::i_SideNone, 0);
    MbSNameMaker cNames(ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> contourNames(1, 0, false);
    contourNames.Add(&cNames);
    MbSNameMaker spineNames(ct_ElementarySolid, MbSNameMaker::i_SideNone, 0);

    MbSolid* pSolid = nullptr;

    // Создание тела вращения путем операции эволюции
    MbResultType res = ::EvolutionSolid(sweptData, *pLine, evParms, operNames, contourNames, spineNames, pSolid);

    // Преобразование указателя на MbSolid в умный указатель SolidSPtr
    SolidSPtr variable = SolidSPtr(pSolid);

    // Возвращаем умный указатель на созданное тело вращения
    return variable;

}