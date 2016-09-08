#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

QVector<Atom*>& MainWindow::getAtomVectorRef()
{
    return m_AtomVector;
}

void MainWindow::setAtomVector(QVector<Atom*> atomVector)
{
    m_AtomVector = atomVector;
}

QVector<Residue*>& MainWindow::getResidueVectorRef()
{
    return m_ResidueVector;
}

void MainWindow::setResidueVector(QVector<Residue*> residueVector)
{
    m_ResidueVector = residueVector;
}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(m_FileReader, SIGNAL(consoleOutput(QString,int)),
                     this, SLOT(printString(QString,int)));
    QObject::connect(m_Timer, SIGNAL(timeout()),
                     this, SLOT(drawNextFrame()));
    QObject::connect(ui->m_AnimateCheck, SIGNAL(toggled(bool)),
                      this, SLOT(setTimerStatus(bool)));
    QObject::connect(ui->drawPathsCheck, SIGNAL(toggled(bool)),
                     ui->m_OpenGLWidget, SLOT(SetDrawPaths(bool)));
    QObject::connect(ui->drawPointsCheck, SIGNAL(toggled(bool)),
                     ui->m_OpenGLWidget, SLOT(SetDrawPoints(bool)));
    QObject::connect(ui->drawCirclesCheck, SIGNAL(toggled(bool)),
                     ui->m_OpenGLWidget, SLOT(SetDrawCircles(bool)));
    QObject::connect(ui->m_CircleRadiusBox, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetCircleRadius(int)));
    QObject::connect(ui->m_FrameBox, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetFrame(int)));
    QObject::connect(m_FPSTimer, SIGNAL(timeout()),
                     this, SLOT(calculateFPS()));
    QObject::connect(ui->m_ResetCamera, SIGNAL(released()),
                     ui->m_OpenGLWidget, SLOT(ResetView()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateFPS()
{
    ui->statusBar->showMessage("FPS: " + QString::number(m_FPS), 2000);
    m_FPS = 0;
}

void MainWindow::drawNextFrame()
{
    if (m_FileReader->GetAtomVectorRef().length() > 0)
    {
        if (ui->m_FrameBox->value() == ui->m_FrameBox->maximum())
        {
            ui->m_FrameBox->setValue(0);
        }
        else
        {
            ui->m_FrameBox->setValue(ui->m_FrameBox->value() + 1);
        }
        ++m_FPS;
    }
}

void MainWindow::filter()
{
    printString("Filtering atoms...",1000);

    bool condition = true;

    for (int i = 0; i < m_AtomVector.length(); ++i)
    {
        QVector<Vertex> vertices;
        if (condition)
        {
            for (int j = 0; j < m_AtomVector[i]->GetTrajectoryRef().length(); ++j)
            {
                if (m_AtomVector[i]->GetTrajectoryRef()[j].z() < m_MinZ)
                {
                    m_MinZ = m_AtomVector[i]->GetTrajectoryRef()[j].z();
                }
                else if (m_AtomVector[i]->GetTrajectoryRef()[j].z() > m_MaxZ)
                {
                    m_MaxZ = m_AtomVector[i]->GetTrajectoryRef()[j].z();
                }
                Vertex vertex;
                vertex.SetPosition(m_AtomVector[i]->GetTrajectoryRef()[j]);
                vertices.append(vertex);
            }
            ui->m_OpenGLWidget->AddVertices(vertices);
        }
    }
//    printString("Filtering complete!",1000);
}

void MainWindow::mapColour()
{
    printString("Mapping colour to atoms...", 1000);
    float range = m_MaxZ - m_MinZ;
    float zPos;
    int index;
    for (int i = 0; i < ui->m_OpenGLWidget->GetVerticesRef().length(); ++i)
    {
        for (int j = 0; j < ui->m_OpenGLWidget->GetVerticesRef()[i].length(); ++j)
        {
            zPos = ui->m_OpenGLWidget->GetVerticesRef()[i][j].GetPosition().z() - m_MinZ;
            index = COLOUR_VECT.length()*zPos/range;
            if (index >= COLOUR_VECT.length())
            {
                index = COLOUR_VECT.length() - 1;
            }
            ui->m_OpenGLWidget->GetVerticesRef()[i][j].SetColour(COLOUR_VECT[index]);
        }
    }
    printString("Colour mapping complete!",1000);

    ui->m_OpenGLWidget->CreateTrajBuffer();

    ui->m_OpenGLWidget->SetBoundingBox(m_FileReader->GetSimBoxRef());

    ui->m_LegendMax->setText(QString::number(m_MaxZ).left(5));
    ui->m_LegendMin->setText(QString::number(m_MinZ).left(5));
    ui->m_LegendMid->setText(QString::number((m_MaxZ - m_MinZ)/2).left(5));

    QVector3D max = COLOUR_VECT.first()*255;
    QColor maxColour(max.x(),max.y(),max.z());
    ui->m_ColourLegend->SetMax(maxColour);

    QVector3D mid = COLOUR_VECT[COLOUR_VECT.length()/2]*255;
    QColor midColour(mid.x(),mid.y(),mid.z());
    ui->m_ColourLegend->SetMin(midColour);

    QVector3D min = COLOUR_VECT.last()*255;
    QColor minColour(min.x(),min.y(),min.z());
    ui->m_ColourLegend->SetMin(minColour);
}

void MainWindow::on_groSelectButton_clicked()
{
    QString groFilePath = QFileDialog::getOpenFileName(this,
                                                    tr("Select .gro file"),
                                                    QDir::homePath(),
                                                    tr(".gro files (*.gro)"));
    ui->groLineEdit->setText(groFilePath);
}

void MainWindow::on_loadDataButton_clicked()
{
    printString("Starting!", 1000);

    ui->m_FrameBox->setValue(0);
    QString groFilePath = ui->groLineEdit->text();
    QString xtcFilePath = ui->xtcLineEdit->text();
    ui->m_OpenGLWidget->ClearData();
    m_AtomVector.clear();
    m_AtomVector.squeeze();
    if(m_FileReader->LoadData(groFilePath, xtcFilePath))
    {
        setAtomVector(m_FileReader->GetAtomVectorRef());
//        printString("Files Loaded", 1000);
        int totalFrames = m_AtomVector[0]->GetTrajectoryRef().length();
        ui->m_FrameBox->setMaximum(totalFrames - 1);
        filter();
        mapColour();
    }
}

void MainWindow::on_m_RefreshRateSlider_valueChanged(int value)
{
    m_RefreshTime = 1000/value;
    setTimerStatus(false);
    setTimerStatus(ui->m_AnimateCheck->isChecked());
}

void MainWindow::on_xtcSelectButton_clicked()
{
    QString xtcFilePath = QFileDialog::getOpenFileName(this,
                                                       tr("Select .xtc file"),
                                                       QDir::homePath(),
                                                       tr(".xtc files (*.xtc)"));
    ui->xtcLineEdit->setText(xtcFilePath);

}

void MainWindow::printString(QString string, int duration)
{
    QTextStream* out = new QTextStream(stdout, QIODevice::WriteOnly);
    *out << string << endl;
    ui->statusBar->showMessage(string, duration);
    delete out;
}

void MainWindow::setTimerStatus(bool animate)
{
    if (animate)
    {
        m_Timer->start(m_RefreshTime);
        m_FPSTimer->start(1000);
    }
    else
    {
        m_Timer->stop();
        m_FPSTimer->stop();
    }
}
