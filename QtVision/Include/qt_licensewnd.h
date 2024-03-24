///////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru ВНИМАНИЕ! Этот файл не является частью API Vision. Он необходим для иллюстрации использования
               ядра Vision с библиотекой Qt и ее классами. Этот заголовочный файл может изменяться от версии
               к версии без предупреждения или полностью удаляться.
           \en WARNING! The file is not a part of API Vision. It is needed to illustrate how to use the Vision kernel
               with Qt library and its classes. This header file can be changed from a version to a version with
               no warning or completely deleted.\~
*/
///////////////////////////////////////////////////////////////////////////////
#ifndef __QT_LICENSEWND_H
#define __QT_LICENSEWND_H

#include <QDialog>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <qt_visiondef.h>

VSN_BEGIN_NAMESPACE

namespace QtVision {

//------------------------------------------------------------------------------
/** \brief \ru NO TRANSLATION.
           \en NO TRANSLATION. \~
    \details \ru NO TRANSLATION. \n.
             \en NO TRANSLATION. \n \~
*/
// ---
class QT_CLASS LicenseWnd : public QDialog
{
    Q_OBJECT
public:
    /// \ru Конструктор по умолчанию. \en Default constructor.
    LicenseWnd();
    /// \ru Деструктор. \en Destructor.
    virtual ~LicenseWnd();
public:
    /// \ru NO TRANSLATION \en NO TRANSLATION
    bool Activate();
private slots:
    /// \ru NO TRANSLATION \en NO TRANSLATION
    void OnActivateClicked();
    /// \ru NO TRANSLATION \en NO TRANSLATION
    void OnContinueClicked();
private:
    /// \ru NO TRANSLATION \en NO TRANSLATION
    bool ApplyKey(const std::string& key, const std::string& signature);
    /// \ru NO TRANSLATION \en NO TRANSLATION
    bool SaveKey(const std::string& key, const std::string& signature);
    /// \ru NO TRANSLATION \en NO TRANSLATION
    bool LoadKey(std::string& key, std::string& signature);
private:
    const std::string m_keyFile = ".c3dkey";
private:
    QLineEdit* m_editKey = nullptr;
    QPlainTextEdit* m_editSignature = nullptr;
    QCheckBox* m_checkSaveKey = nullptr;
private:
    Q_DISABLE_COPY(LicenseWnd)
};

} // namespace QtVision

VSN_END_NAMESPACE

#endif /* __QT_LICENSEWND_H */
