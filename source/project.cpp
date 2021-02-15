#include "project.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>
#include <fstream>
#include <QDateTime>
#include <file.h>

Project::Project(QObject *parent) :
    Node(parent, "Project")
{
    Q_INIT_RESOURCE(resources);
    QIcon iconAct1;
    iconAct1.addFile(":/Images/icons8-processor.png",QSize(25,25));
    pNewSpartan = new QAction("&New M86", this);
    pNewSpartan->setIcon(iconAct1);
    connect(pNewSpartan, SIGNAL(triggered()), this, SLOT(new_M86_Spartan6()));

    pNewMasterfile = new QAction(tr("New &Masterfile"), this);
    connect(pNewMasterfile, &QAction::triggered, this, &Project::new_Masterfile);

    pNewgoldenRef = new QAction(tr("New &Golden Reference"), this);
    connect(pNewgoldenRef, &QAction::triggered, this, &Project::new_goldenRef);

    connect(parent, SIGNAL (generateFpga()), this, SLOT(readme_all()));
}

Project::~Project()
{
    disconnect(pNewSpartan, SIGNAL(triggered()), this, SLOT(new_M86_Spartan6()));
    disconnect(pNewMasterfile, &QAction::triggered, this, &Project::new_Masterfile);
    disconnect(pNewgoldenRef, &QAction::triggered, this, &Project::new_goldenRef);

    /*
    disconnect(pNewJadeFile, SIGNAL(triggered()), this, SLOT(new_JADE_Package()));
    */

    delete pNewSpartan;
    delete pNewgoldenRef;
    delete pNewMasterfile;
    /*
    delete pNewMasterFile;
    delete pNewJadeFile;
    */
}

//TODO Jade Funktionen wenn möglich ausbauen
bool Project::jade()
{
    return m_jade;
}
void Project::setJade(bool jade)
{
    m_jade = jade;
}

FileString Project::project_directory()
{
    return m_project_directory;
}

void Project::setProject_directory(FileString filename)
{
    m_project_directory = filename;
}

FileString Project::srec_cat()
{
    return m_srecCat;
}

void Project::setSrec_cat(FileString filename)
{
    m_srecCat = filename;
}

FileString Project::logichdr()
{
    return m_logichdr;
}

void Project::setLogichdr(FileString filename)
{
    m_logichdr = filename;
}

FileString Project::mbind()
{
    return m_mbind;
}

void Project::setMbind(FileString filename)
{
    m_mbind = filename;
}
void Project::new_M86_Spartan6()
{
    M86_Spartan6 *m = new M86_Spartan6(this);//(QObject::parent());
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("New M86");
    m->setType("M86 Package");
    m->setObjectName("M86");
//  m->setType("FPGA");
    //m->setVer_state(Alpha);
    this->setChild(this->rowCount(),m);
//  qDebug() << this->children();
}

void Project::new_Masterfile()
{
    Masterfile *m = new Masterfile(this);
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new Masterfile");
    m->setType("Masterfile");
    m->setObjectName("Masterfile");
    this->setChild(this->rowCount(),m);
}

void Project::new_goldenRef()
{
    goldenReference *m = new goldenReference(this);
    Model *m_m = Node::getModel();
    m->setModel(m_m);
    m->setObjectName("golden");
    m->setDescription("new Golden Reference");
    m->setType("Golden Reference");
    this->setChild(this->rowCount(),m);
}

void Project::readme_all()
{
     QString filename = m_project_directory.filestring+"/"+"README.txt";
     QFile file(filename);

     if(file.open(QFile::WriteOnly|QFile::Text))
     {
        QTextStream stream(&file);

        QString date = QDateTime().currentDateTime().date().toString();
        QString time = QDateTime().currentDateTime().time().toString();

        stream << "Package Generator v1.00.05 Beta\n\n";
        stream << "Build start time: " + date + " " + time +"\n\n";

        //Project
        stream << "Project: " + description() + "\n";
        stream << " JadeProject         = " + QVariant(jade()).toString() + "\n";
        stream << " project_directoy    = " + project_directory().filestring + "\n";
        stream << " srec_cat            = " + srec_cat().filestring + "\n";
        stream << " logichdr            = " + logichdr().filestring + "\n";
        stream << " mbind               = " + mbind().filestring + "\n\n";


        //childes
        QObjectList objectsProject = this->children();
        for(int i = 0; i < this->children().length(); i++)
        {
            if(QLatin1String(objectsProject[i]->metaObject()->className()) == "M86_Spartan6")
            {
                M86_Spartan6 *item = static_cast<M86_Spartan6*>(objectsProject[i]);
                stream << "\n\n M86: " + item->description() + "\n";
                stream << "     module_type             = " + ModuleType().modultype[static_cast<int>(item->module_type().selectedmodultype)] + "\n";
                stream << "     outputFile_location     = " + item->location() + "\n";
                stream << "     outputFile_name         = " + item->pkgName() + "\n";
                stream << "     typecode                = " + item->typecode() + "\n";
                stream << "     variant                 = " + item->variant() + "\n";
                stream << "     ver_major               = " + item->ver_major() + "\n";
                stream << "     ver_minor               = "  + item->ver_minor() + "\n";
                stream << "     ver_subminor            = " + item->ver_subminor() + "\n";
                stream << "     verstate                = " + VerState().verstate[static_cast<int>(item->verstate().selectedVersion)] + "\n";
                stream << "     required_driver_version = " + item->required_driver_version() + "\n\n";

                QObjectList itemchilds = item->children();
                for(int i = 0; i < item->children().length(); i++)
                {
                    if(QLatin1String(itemchilds[i]->metaObject()->className()) == "Fpga")
                    {
                        Fpga *item_child = static_cast<Fpga*>(itemchilds[i]);
                        stream << "     FPGA: " + item_child->description() + "\n";
                        stream << "         outputFile_name     = " + item_child->name() + "\n";
                        stream << "         inputFile_directory = " + item_child->filename() + "\n";
                        stream << "         file_type           = " + FileType().filetype[static_cast<int>(item_child->file_type().selectedType)] + "\n";
                        stream << "         fpgatype            = " + FpgaType().fpgatype[static_cast<int>(item_child->fpgatype().selectedfpga)] + "\n";
                        stream << "         designnumber        = " + item_child->designnumber() + "\n";
                        stream << "         revision            = " + item_child->revision() + "\n";
                        stream << "         testversion         = "  + item_child->testversion() + "\n";
                        stream << "         dualboot            = " + DualBoot().dualboot[static_cast<int>(item_child->dualboot().dualbootena)] + "\n";
                        stream << "         flash_size          = " + FlashSize().memorysize[static_cast<int>(item_child->flash_size().selectedsize)] + "\n";
                        stream << "         start_addr          = " + item_child->start_addr() + "\n\n";
                    }
                    else if(QLatin1String(itemchilds[i]->metaObject()->className()) == "firmware")
                    {
                        firmware *item_child = static_cast<firmware*>(itemchilds[i]);
                        stream << "     Firmware: " + item_child->description() + "\n";
                        stream << "         outputFile_name     = " + item_child->name() + "\n";
                        stream << "         outputFile_directory= " + item_child->filename() + "\n";
                        stream << "         ver_major           = " + item_child->ver_major() + "\n";
                        stream << "         ver_minor           = " + item_child->ver_minor() + "\n";
                        stream << "         ver_subminor        = " + item_child->ver_subminor() + "\n";
                        stream << "         verstate            = " + VerState().verstate[static_cast<int>(item_child->verstate().selectedVersion)] + "\n";
                        stream << "         start_addr          = " + item_child->start_addr() + "\n\n";
                    }
                }
            }

            else if(QLatin1String(objectsProject[i]->metaObject()->className()) == "goldenReference")
            {
                goldenReference *item = static_cast<goldenReference*>(objectsProject[i]);
                stream << "\n\n Golden Reference: " + item->description() + "\n";
                stream << "     outputFile_location = " + item->location() + "\n";
                stream << "     outputFile_name     = " + item->filename() + "\n";
                stream << "     inputFile_directoy  = " + item->goldenRef_file() + "\n";
                stream << "     inputFile_type      = " + FileType().filetype[static_cast<int>(item->file_type().selectedType)] + "\n";
                stream << "     flash_size          = " + FlashSize().memorysize[static_cast<int>(item->flash_size().selectedsize)] + "\n";
                stream << "     fpgatype            = " + FpgaType().fpgatype[static_cast<int>(item->fpgatype().selectedfpga)] + "\n";
                stream << "     start_addr          = " + item->start_addr() + "\n\n";
            }

            else if(QLatin1String(objectsProject[i]->metaObject()->className()) == "Masterfile")
            {
                Masterfile *item = static_cast<Masterfile*>(objectsProject[i]);
                stream << "\n\n Masterfile: " + item->description() + "\n";
                stream << "     outputFile_location = " + item->location() + "\n";
                stream << "     outputFile_name     = " + item->filename() + "\n";
                stream << "     fpgatype            = " + FpgaType().fpgatype[static_cast<int>(item->fpgatype().selectedfpga)] + "\n";
                stream << "     flash_size          = " + FlashSize().memorysize[static_cast<int>(item->flash_size().selectedsize)] + "\n";
                stream << "     bit_reverse         = " + QVariant(item->bit_reverse()).toString() + "\n\n";

                QObjectList itemchilds = item->children();
                for(int i = 0; i < item->children().length(); i++)
                {
                    if(QLatin1String(itemchilds[i]->metaObject()->className()) == "file")
                    {
                        class file *item_child = static_cast< class file*>(itemchilds[i]);
                        stream << "     File/FPGA/Firmware: " + item_child->description() + "\n";
                        stream << "         inputFile_direcotry = " + item_child->filename() + "\n";
                        stream << "         file_type           = " + FileType().filetype[static_cast<int>(item_child->file_type().selectedType)] + "\n";
                        stream << "         start_addr          = " + item_child->start_addr() + "\n\n";
                    }
                }
            }
        }

        file.flush();
        file.close();
     }
}


/*
void Project::new_JADE_Package()
{
    JADE_Package *m = new JADE_Package();
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("JADE Package");
    this->setChild(this->rowCount(),m);
}

void Project::new_Masterfile()
{
    Masterfile *m = new Masterfile();
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("Masterfile");
    this->setChild(this->rowCount(),m);
}
*/
void Project::node_menue(QMenu *menu)
{
    if(!jade())
        menu->addAction(pNewSpartan);
    menu->addAction(pNewMasterfile);
    menu->addAction(pNewgoldenRef);
    /*
    menu->addAction(pNewMasterFile);
    menu->addAction(pNewJadeFile);
    */
}
