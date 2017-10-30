#include <QDebug>
#include "M86_Spartan6.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>


M86_Spartan6::M86_Spartan6(QObject *parent) :
    Node(parent,"M86_Spartan6")
{

    QIcon GenerateIcon;
    QIcon NewFpgaIcon;
    QIcon DeleteIcon;

    GenerateIcon.addFile(":/Images/icons8-robot.png",QSize(25,25));
    pGenerate = new QAction(tr("Generate"), this);
    pGenerate->setIcon(GenerateIcon);
    connect(pGenerate, SIGNAL(triggered()), this, SLOT(generate_package()));

    NewFpgaIcon.addFile(":/Images/integrated-circuit-256.png",QSize(25,25));
    pNewFPGA = new QAction(tr("&New FPGA"), this);
    pNewFPGA->setIcon(NewFpgaIcon);
    connect(pNewFPGA, &QAction::triggered, this, &M86_Spartan6::new_FPGA);

    DeleteIcon.addFile(":/Images/icons8-delete.png", QSize(25,25));
    pDelete = new QAction(tr("&Delete"), this);
    pDelete->setIcon(DeleteIcon);
    connect(pDelete, &QAction::triggered, this, &Node::delete_node);

    connect(parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_package()));
    m_parent = parent;

}
M86_Spartan6::~M86_Spartan6()
{
    disconnect(pGenerate, SIGNAL (triggered()),this, SLOT(generate_package()));
    disconnect(pNewFPGA, &QAction::triggered, this, &M86_Spartan6::new_FPGA);
    disconnect(pDelete, &QAction::triggered, this, &Node::delete_node);
    disconnect(m_parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_package()));
    delete pGenerate;
    delete pNewFPGA;
    delete pDelete;
}

FileString M86_Spartan6::filename()
{
    return m_filename;
}

void M86_Spartan6::setFilename(FileString name)
{
    m_filename = name;
}

QString M86_Spartan6::ver_major()
{
   return m_ver_major;
}

void M86_Spartan6::setVer_major(QString ver_major)
{
    m_ver_major = ver_major;
}

QString M86_Spartan6::ver_minor()
{
    return m_ver_minor;
}

void M86_Spartan6::setVer_minor(QString ver_minor)
{
    m_ver_minor = ver_minor;
}

QString M86_Spartan6::ver_subminor()
{
    return m_ver_subminor;
}

void M86_Spartan6::setVer_subminor(QString ver_subminor)
{
    m_ver_subminor = ver_subminor;
}
/*
VER_STATE M86_Spartan6::ver_state()
{
    return m_ver_state;
}

void M86_Spartan6::setVer_state(VER_STATE ver_state)
{
    m_ver_state = ver_state;
}
*/

void M86_Spartan6::node_menue(QMenu *menu)
{
    menu->addAction(pGenerate);
    menu->addSeparator();
    menu->addAction(pNewFPGA);
    menu->addSeparator();
    menu->addAction(pDelete);
    menu->addSeparator();
}

void M86_Spartan6::new_FPGA()
{
    Fpga *m = new Fpga(this);
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("Spartan 6 FPGA");
    m->setType("FPGA");
    this->setChild(this->rowCount(),m);
}

bool M86_Spartan6::generate_package()
{
    qDebug() << "Generate Spartan M86Package";
    qDebug() << this;
    QString logichdrProg = m_parent->property("logichdr").value<FileString>().filestring;
    qDebug() << logichdrProg;


    QObjectList childrenOfSpartan = this->children();
    QString parameters = "";
    for (auto const& childOfSpartan : childrenOfSpartan)
    {
        if (childOfSpartan->objectName()=="FPGA")
        {
            qDebug() << "Object name" << childOfSpartan->objectName();
            qDebug() << "design number" << childOfSpartan->property("designnumber").value<QString>();
            qDebug() << "flash size" << childOfSpartan->property("flash_size").value<FlashSize>().selectedsize;
            qDebug() << "golden ena" << childOfSpartan->property("dualboot").value<DualBoot>().dualbootena;
            qDebug() << "filename " << childOfSpartan->property("filename").value<FileString>().filestring;
            parameters += "Design Number = ";
            parameters += childOfSpartan->property("designnumber").value<QString>();
            parameters += "\n start addr = ";
            parameters += childOfSpartan->property("start_addr").value<HexString>().hexstring;
        }
    }

    qDebug() << "start logichdr";
    QProcess *process = new QProcess(this);
    process->start(logichdrProg, QIODevice::ReadWrite);

    if (!process->waitForStarted())
        qDebug() << "error by starting logichdr.exe";
    if (!process->waitForFinished())
        qDebug() << "logichdr.exe failed";

    QByteArray stdoutResults = process->readAllStandardOutput();

    parameters += stdoutResults;
//  qDebug() << "logichdr returned value" << stdoutResults;


    QMessageBox msgBox;
    msgBox.setText("Generate m86 with following parameters");
    msgBox.setDetailedText(parameters);
//  msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet("QLabel{min-width: 500px}");
    msgBox.exec();

//  qDebug() << "ver_major : " << this->property("ver_major").toString();
    FileString itsFileName = this->filename();
//  qDebug() << "file_name = " << itsFileName.filestring;

    return true;
}

