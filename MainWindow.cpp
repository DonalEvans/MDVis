#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Vertex.h"
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

void MainWindow::setResidueVector(QVector<Residue *> residueVector)
{
    m_ResidueVector = residueVector;
}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->zoomSlider, SIGNAL(valueChanged(int)),
                     ui->zoomLevel, SLOT(setValue(int)));
    QObject::connect(ui->zoomLevel, SIGNAL(valueChanged(int)),
                     ui->zoomSlider, SLOT(setValue(int)));
    QObject::connect(m_FileReader, SIGNAL(consoleOutput(QString,int)),
                     ui->statusBar, SLOT(showMessage(QString,int)));
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
    QObject::connect(ui->zoomLevel, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetZoom(int)));
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
    printString("Filtering atoms...",0);
    float scalingY = m_FileReader->GetSimBoxRef().y();
    float scalingZ = m_FileReader->GetSimBoxRef().z();

    QVector3D offset(scalingY/2,scalingY/2,0);
    QVector3D scale((scalingY + 5)/2,(scalingY + 5)/2,scalingZ);
    QVector3D pos;
    QVector<QVector3D> traj;

    bool condition = true;

    m_FilteredPos.clear();

    for (int i = 0; i < m_AtomVector.length(); ++i)
    {
        Atom* currAtom = m_AtomVector[i];
        if (condition)
        {
            QVector<QVector3D> posData = currAtom->GetTrajectoryRef();
            for (int j = 0; j < posData.length(); ++j)
            {
                pos = (posData[j]- offset)/scale;
//                pos = posData[j];
                if (pos.z() < m_MinZ)
                {
                    m_MinZ = pos.z();
                }
                else if (pos.z() > m_MaxZ)
                {
                    m_MaxZ = pos.z();
                }
                traj.append(pos);
            }
            m_FilteredPos.append(traj);
            traj.clear();
        }
    }
    printString("Filtering complete!",0);
//    ui->m_OpenGLWidget->SetTrajectory(m_FilteredPos);
}

void MainWindow::mapColour()
{
    printString("Mapping colour to atoms...", 0);
    float range = m_MaxZ - m_MinZ;
    float zPos;
    int index;
    QVector<QVector<Vertex>> vertexVect;
    QVector<Vertex> trajVertex;
    for (int i = 0; i < m_FilteredPos.length(); ++i)
    {
//        if (i > 222800)
//        {
//            printString(QString::number(i),0);
//        }
        for (int j = 0; j < m_FilteredPos[i].length(); ++j)
        {
            zPos = m_FilteredPos[i][j].z() - m_MinZ;
            index = COLOUR_VECT.length()*zPos/range;
            if (index >= 7)
            {
                index = 6;
            }
            trajVertex.append(Vertex(m_FilteredPos[i][j],COLOUR_VECT[index]));
        }
        vertexVect.append(trajVertex);
        trajVertex.clear();
    }
    printString("Colour mapping complete!",0);

    ui->m_OpenGLWidget->SetVertices(vertexVect);

    ui->m_LegendMax->setText(QString::number(m_MaxZ).left(5));
    ui->m_LegendMin->setText(QString::number(m_MinZ).left(5));
    ui->m_LegendMid->setText(QString::number((m_MaxZ - m_MinZ)/2).left(5));

    QVector3D max = COLOUR_VECT[6]*255;
    QColor maxColour(max.x(),max.y(),max.z());
    ui->m_ColourLegend->SetMax(maxColour);
    QVector3D min = COLOUR_VECT[0]*255;
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
    ui->m_FrameBox->setValue(0);
    QString groFilePath = ui->groLineEdit->text();
    QString xtcFilePath = ui->xtcLineEdit->text();
    ui->statusBar->showMessage("Starting!", 2000);
    if(m_FileReader->LoadData(groFilePath, xtcFilePath))
    {
        setAtomVector(m_FileReader->GetAtomVectorRef());
        ui->statusBar->showMessage("Files Loaded", 2000);
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
