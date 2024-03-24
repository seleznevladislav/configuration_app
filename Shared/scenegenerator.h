////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Класс SceneGenerator представляет фабрику представлений геометрических
             объектов, а также генерацию готовой сцены для последующей отрисовки.
         \en SceneGenerator class presents a factory of geometric object representations,
             and also generates the prepared scene for the next rendering. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_SCENEGENERATOR_H
#define __VSN_SCENEGENERATOR_H

#include <model.h>
#include <model_item.h>
#include <vsn_objectmap.h>
#include <vsn_object.h>
#include <vsn_geometry.h>
#include <vsn_geometryrep.h>
#include <vsn_scsegmentdata.h>
#include <vsn_scenesegment.h>

#include <model.h>
#include <model_item.h>

#include <vsn_global.h>

VSN_USE_NAMESPACE

/** \brief \ru ВНИМАНИЕ! Этот файл не является частью API Vision. Этот заголовочный файл может изменяться от версии
               к версии без предупреждения или полностью удаляться.
           \en WARNING! The file is not a part of API Vision. This header file can be changed from a version 
               to a version with no warning or completely deleted.\~
*/

class MbAssembly;
class SceneGenerator;
//------------------------------------------------------------------------------
/** \brief \ru Класс ProgressBuild служит для определения конца и шага генерации полигональной модели.
           \en ProgressBuild class serves to define an end and a step for generating a polygonal model. \~
    \details \ru Класс ProgressBuild имеет ряд функций, а также сигналов, с помощью которых пользователь 
                 может узнать состояние генерации тел и примитивов, а также ее окончание. Важно знать, что
                 ProgressBuild применяется только при генерации математических объектов. \n.
             \en ProgressBuild class has a set of functions, and also signals that the user
                 can use to know a state of the solid and the primitive generation and also when generation ends.
                 It's important to know that ProgressBuild class is used only when generating math objects. \n \~
    \ingroup Vision_Node
*/
// ---
class ProgressBuild : public Object
{
    VSN_OBJECT(ProgressBuild);
protected:
    /// \ru Конструктор по умолчанию. \en Default constructor. \~
    explicit ProgressBuild(SceneGenerator* pGenerator);
    /// \ru Деструктор. \en Destructor. \~ 
    virtual ~ProgressBuild();
public:
    /// . \en . \~
    void AppendGeometry(const Geometry* pGeometry);
    /// \ru Вернуть текущее значение перестраиваемой геометрии. \en Returns the current value of the rebuilding geometry. \~
    int GetValue() const;
    /// \ru Вернуть минимальное значение перестраиваемой геометрии. \en Returns the minimum value of the rebuilding geometry. \~
    int GetMinimum() const;
    /// \ru Вернуть максимальное значение количества перестраиваемой геометрии. \en Returns the maximum quantity value of the rebuilding geometry. \~
    int GetMaximum() const;
public:
    /// \ru Сигнал об окончании перестроения конкретной геометрии. \en The signal about finishing rebuilding of a certain geometry. \~
    VSN_SIGNAL(Public, ValueModified, void ValueModified(int val), val)
public:
    VSN_SLOT(Public, slotFinishBuildRep, void slotFinishBuildRep())
    VSN_SLOT(Public, slotFinishBuildGeometry, void slotFinishBuildGeometry(Geometry* pGeometry))
    /// \ru Сигнал об окончании перестроения всей геометрии. \en The signal about finishing rebuilding of all geometry. \~
    VSN_SIGNAL(Public, BuildAllCompleted, void BuildAllCompleted())
private:
    void RegisterGeometryRep(const GeometryRep* pGeometryRep);
    void RegisterGeometry(const Geometry* pGeometry);
    void UnregisterGeometry(const Geometry* pGeometry);
protected:
    int m_currentGeometry;
    int m_countGeometry;
    SceneGenerator* m_pGenerator;
    ObjectList<const Geometry*> m_geometries;
private:
    VSN_DISABLE_COPY(ProgressBuild)
    friend class SceneGenerator;
};

// Returns the current value of the rebuilding geometry
inline int ProgressBuild::GetValue() const { return m_currentGeometry; }
// Returns the minimum value of the rebuilding geometry.
inline int ProgressBuild::GetMinimum() const { return 0; }
// Returns the maximum quantity value of the rebuilding geometry.
inline int ProgressBuild::GetMaximum() const { return m_countGeometry; }


class GeometryHash;
//------------------------------------------------------------------------------
/** \brief \ru Класс SceneGenerator представляет фабрику представлений геометрических
               объектов, а также генерацию готовой сцены для последующей отрисовки.
           \en SceneGenerator class presents a factory of geometric object representations and
               also generates the prepared scene for the next rendering. \~
  \details \ru SceneGenerator имеет две группы функций. Первая группа работает непосредственно с математическими
               объектами и тем самым обеспечивает глубокую интеграцию с математическим ядром. Вторая группа
               генерирует представления простых объектов, таких как куб, цилиндр, сфера и др., т.е. где 
               полигональная модель рассчитывается не с помощью математического ядра, а самими визуальными
               объектами. \n.
           \en SceneGenerator class has two function groups. The first one directly operates with math objects
               providing a deep integration with the math kernel. The second one generates representations
               of such simple objects as cube, cylinder, sphere, etc., i.e. a polygonal model is computed
               with not a math kernel, but with visual objects themselves. \n \~
    \ingroup Vision_Node
*/
// ---
class SceneGenerator : public Object
{
    VSN_OBJECT(SceneGenerator);
public:
    /// \ru Конструктор по умолчанию. \en Default constructor. \~
    explicit SceneGenerator(Object* parent = nullptr);
    /// \ru Деструктор. \en Destructor. \~ 
    virtual ~SceneGenerator();
public:
    /// \ru Загрузить модель из памяти. \en Loads the model from memory. \~
    SPtr<MbModel> LoadModel(const char* pMemory, size_t length);
public: /// \ru Группа функций, работающая с математическим ядром. \en Group functions operating with the math kernel. \~
    /// \ru Создать прогресс построения геометрии. \en Creates the progress of the rebuilding geometry. \~
    ProgressBuild* CreateProgressBuild();
    /// \ru Создать граф сцены из MbModel. \en Creates a scene graph from MbModel. \~
    /// \ru bCheckIsSameBody - Проверка того же тела. \en bCheckIsSameBody - Сheck is same Body. \~
    SceneSegment* CreateSceneSegment(SPtr<MbModel> pModel, SceneSegment* pParent = nullptr, bool bCheckIsSameBody = false,
        bool bCheckIsSameMeshBody = false, const std::vector<MbeSpaceType>& spaceTypes = { st_Assembly, st_Instance, st_Solid, st_Mesh, st_WireFrame });
    /// \ru Построение полигонального представления для отрисовки сцены. \en Building a polygon representation for scene rendering. \~
    void StartBuildGeometry(CommandType buildType = CommandType::Threaded);

    void SetVisualSag(double visualSag);
public:
    /// \ru Сигнал об окончании перестроения конкретной геометрии. \en The signal about finishing rebuilding of a certain geometry. \~
    VSN_SIGNAL(Public, CreateSegment, void CreateSegment(SceneSegment* newSeg, SceneSegment* parentSeg), newSeg, parentSeg)
    /// \ru Сигнал об окончании перестроения конкретной геометрии. \en The signal about finishing rebuilding of a certain geometry. \~
    VSN_SIGNAL(Public, CreateSegmentItem, void CreateSegmentItem(SceneSegment* newSeg, SceneSegment* parentSeg, 
        const MbModel* model, const MbItem* item, const MbItem* parentItem), newSeg, parentSeg, model, item, parentItem)
protected:
    void BuildSceneFromModel(const MbItem* pItem, const MbItem* pParentItem, SceneSegment* pParent,
    const std::vector<MbeSpaceType>& spaceTypes,const MbMatrix3D& mx, uint& countGeom);
    void LoadSceneFromModel( SPtr<MbModel> pModel, SceneSegment* pParent, const std::vector<MbeSpaceType>& spaceTypes);
    void ClearProgressBuild();
    SceneSegmentData* CreateSegmentData(const MbItem* pItem);
private:
    ProgressBuild* m_pProgressBuild;
    std::shared_ptr<GeometryHash> m_pGeometryHash;
    std::shared_ptr<MaterialCache> m_ptrMaterialCache;
    std::vector<SPtr<MbItem>> m_items;
    double m_visualSag;
private:
    VSN_DISABLE_COPY(SceneGenerator);
    friend class ProgressBuild;
};

//-----------------------------------------------------------------------------
// Функция возвращает основную сборку модели, если она существует.
// The function returns the main assembly of the model, if it exists.
// ---
const MbAssembly* GetMainAssembly(const MbModel* pModel);

//-----------------------------------------------------------------------------
// Функция возвращает существующую основную сборку или создает ее, объединяя все элементы модели.
// The function returns an existing main assembly or creates one by uniting all elements of the model.
// ---
MbAssembly* UniteToMainAssembly(MbModel* pModel);

#endif /* __VSN_SCENEGENERATOR_H */
