#include <qt_licensewnd.h>

#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDir>

#include <tool_enabler.h>

#include <last.h>


VSN_USE_NAMESPACE
using namespace QtVision;

///////////////////////////////////////////////////////////////////////////////
//
// класс LicenseWnd
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// конструктор по умолчанию
// ---
LicenseWnd::LicenseWnd()
{
}

//-----------------------------------------------------------------------------
// деструктор
// ---
LicenseWnd::~LicenseWnd()
{
}

//-----------------------------------------------------------------------------
// 
// ---
bool LicenseWnd::Activate()
{
    std::string key;
    std::string signature;
    if (LoadKey(key, signature) && ApplyKey(key, signature))
        return true;

    auto labelLicense = new QLabel;
    labelLicense->setText(QObject::tr("The current license key is missing or wrong. Please enter your license key to activate."));
    labelLicense->setAlignment(Qt::AlignCenter);

    auto formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignRight);

    m_editKey = new QLineEdit();
    m_editSignature = new QPlainTextEdit();

    formLayout->addRow(QObject::tr("License key"), m_editKey);
    formLayout->addRow(QObject::tr("Signature"), m_editSignature);

    m_checkSaveKey = new QCheckBox;
    m_checkSaveKey->setText(QObject::tr("Save this license key"));

    auto buttonActivate = new QPushButton;
    auto buttonContinue = new QPushButton;

    buttonActivate->setFixedWidth(170);
    buttonContinue->setFixedWidth(170);

    buttonActivate->setText(QObject::tr("Activate"));
    buttonContinue->setText(QObject::tr("Continue without key"));

    buttonContinue->setDefault(true);

    auto buttons = new QHBoxLayout();
    buttons->addWidget(buttonActivate);
    buttons->addWidget(buttonContinue);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowIcon(QIcon(":/res/crystal.ico"));

    auto vLayout = new QVBoxLayout(this);
    vLayout->setSizeConstraint(QLayout::SetFixedSize);

    vLayout->addWidget(labelLicense);
    vLayout->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    vLayout->addLayout(formLayout);
    vLayout->addWidget(m_checkSaveKey);
    vLayout->addLayout(buttons);

    QObject::connect(buttonActivate, SIGNAL(clicked()), this, SLOT(OnActivateClicked()));
    QObject::connect(buttonContinue, SIGNAL(clicked()), this, SLOT(OnContinueClicked()));
    return (exec() == Accepted);
}

//-----------------------------------------------------------------------------
// 
// ---
void LicenseWnd::OnActivateClicked()
{
    VSN_ASSERT(m_editKey);
    VSN_ASSERT(m_editSignature);
    VSN_ASSERT(m_checkSaveKey);

    const std::string key = m_editKey->text().trimmed().toStdString();
    const std::string signature = m_editSignature->toPlainText().trimmed().toStdString();

    if (ApplyKey(key, signature))
    {
        const bool saveKeyFlag = (m_checkSaveKey->checkState() == Qt::CheckState::Checked);
        if (saveKeyFlag)
            SaveKey(key, signature);
        accept();
    }
    else
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Invalid license key. For a new license, please contact C3D Labs."));
}

//-----------------------------------------------------------------------------
// 
// ---
void LicenseWnd::OnContinueClicked()
{
    accept();
}

//-----------------------------------------------------------------------------
// 
// ---
bool LicenseWnd::ApplyKey(const std::string& key, const std::string& signature)
{
    ::EnableMathModules(key.c_str(), static_cast<int>(key.length()), signature.c_str(), static_cast<int>(signature.length()));
    return IsMathVisionEnable();
}

//-----------------------------------------------------------------------------
// 
// ---
bool LicenseWnd::SaveKey(const std::string& key, const std::string& signature)
{
    const auto keyPath = QDir::homePath().toStdString() + "/" + m_keyFile;
    QFile file(keyPath.c_str());
    if (file.open(QFile::WriteOnly))
    {
        file.write(signature.data(), signature.size());
        file.write("\n");
        file.write(key.data(), key.size());
        file.close();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// 
// ---
bool LicenseWnd::LoadKey(std::string& key, std::string& signature)
{
    const auto keyPath = QDir::homePath().toStdString() + "/" + m_keyFile;
    QFile file(keyPath.c_str());
    if (file.open(QFile::ReadOnly))
    {
        signature = file.readLine().trimmed().toStdString();
        key = file.readLine().trimmed().toStdString();
        file.close();
        return true;
    }
    return false;
}
