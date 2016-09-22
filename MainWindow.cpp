#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>

QVector<Atom*>& MainWindow::getAtomVectorRef()
{
    return m_AtomVector;
}

void MainWindow::setAtomVector(QVector<Atom*> atomVector)
{
    m_AtomVector = atomVector;
}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(m_FileReader, SIGNAL(consoleOutput(QString,int)),
                     this, SLOT(printString(QString,int)));
    QObject::connect(m_Timer, SIGNAL(timeout()),
                     this, SLOT(incrementFrame()));
    QObject::connect(ui->m_AnimateCheck, SIGNAL(toggled(bool)),
                      this, SLOT(setTimerStatus(bool)));
    QObject::connect(ui->drawPathsCheck, SIGNAL(toggled(bool)),
                     ui->m_OpenGLWidget, SLOT(SetDrawPaths(bool)));
    QObject::connect(ui->drawPointsCheck, SIGNAL(toggled(bool)),
                     ui->m_OpenGLWidget, SLOT(SetDrawPoints(bool)));
    QObject::connect(ui->m_CircleRadiusBox, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetCircleRadius(int)));
    QObject::connect(ui->m_FrameBox, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetFrame(int)));
    QObject::connect(m_FPSTimer, SIGNAL(timeout()),
                     this, SLOT(outputFPS()));
    QObject::connect(ui->m_ResetCamera, SIGNAL(released()),
                     ui->m_OpenGLWidget, SLOT(ResetView()));
    QObject::connect(ui->m_ResetLighting, SIGNAL(released()),
                     ui->m_OpenGLWidget, SLOT(ResetLighting()));
    QObject::connect(ui->m_Ambient, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetAmbientValue(int)));
    QObject::connect(ui->m_MaxPathLength, SIGNAL(valueChanged(int)),
                    ui->m_OpenGLWidget, SLOT(SetMaxPathLength(int)));
    QObject::connect(ui->m_MinPathLength, SIGNAL(valueChanged(int)),
                    ui->m_OpenGLWidget, SLOT(SetMinPathLength(int)));

    ui->m_ColourLegend->SetIsHorizontal(false);
    ui->m_ColourSpinBox->setMaximum(m_ColourMaps.GetNumberOfMaps()-1);

    int map = ui->m_ColourSpinBox->value();
    ui->m_ColourLegend->SetColourMap(m_ColourMaps.GetMap(map));

    ui->m_LegendMax->setValidator(new QDoubleValidator(this));
    ui->m_LegendMin->setValidator(new QDoubleValidator(this));

    ui->m_Mapping->addItem("Path Length",Qt::DisplayRole);
    ui->m_Mapping->addItem("Velocity Magnitude",Qt::DisplayRole);
    ui->m_Mapping->addItem("Path Curvature",Qt::DisplayRole);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateDataRange()
{
    if(ui->m_Mapping->currentText() == "Path Curvature")
    {
        m_FileReader->CalculatePathCurvature();
        if(m_LastMappedTo != ui->m_Mapping->currentText())
        {
            m_RealMapMax = m_FileReader->GetMaxPathCurvature();
            m_RealMapMin = m_FileReader->GetMinPathCurvature();
            resetLegend();
        }
    }
    else if(ui->m_Mapping->currentText() == "Path Length")
    {
        m_FileReader->CalculatePathLength();
        if(m_LastMappedTo != ui->m_Mapping->currentText())
        {
            m_RealMapMax = m_FileReader->GetMaxPathLength();
            m_RealMapMin = m_FileReader->GetMinPathLength();
            resetLegend();
        }
    }
    else if(ui->m_Mapping->currentText() == "Velocity Magnitude")
    {
        m_FileReader->CalculateVelocity();
        if(m_LastMappedTo != ui->m_Mapping->currentText())
        {
            m_RealMapMax = m_FileReader->GetMaxVelocity();
            m_RealMapMin = m_FileReader->GetMinVelocity();
            resetLegend();
        }
    }
}

void MainWindow::createVertices()
{
    QVector<Vertex> vertices;
    Vertex vertex;

    for (int i = 0; i < m_AtomVector.length(); ++i)
    {
        for (int j = 0; j < m_AtomVector[i]->GetTrajectoryRef().length(); ++j)
        {
            vertex.SetPosition(m_AtomVector[i]->GetTrajectoryRef()[j]);
            vertices.append(vertex);
        }
        ui->m_OpenGLWidget->AddVertices(vertices);
        vertices.clear();
    }
}

void MainWindow::incrementFrame()
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

void MainWindow::mapColour()
{
    if (ui->m_OpenGLWidget->GetVerticesRef().length() == 0)
    {
        return;
    }
    printString("Mapping colour to atoms...", MS_SECOND);
    float range = m_UserMapMax - m_UserMapMin;
    if (range == 0)
    {
        range = 1;
    }
    float mapValue;
    int mapIndex;
    int map = ui->m_ColourSpinBox->value();
    bool isCurve = ui->m_Mapping->currentText() == "Path Curvature";
    bool isLength = ui->m_Mapping->currentText() == "Path Length";
    bool isVelocity = ui->m_Mapping->currentText() == "Velocity Magnitude";
    for (int i = 0; i < ui->m_OpenGLWidget->GetVerticesRef().length(); ++i)
    {
        if(isCurve)
        {
            for (int j = 0; j < ui->m_OpenGLWidget->GetVerticesRef()[i].length(); ++j)
            {
                mapValue = m_AtomVector[i]->GetPathCurvatureRef()[j] - m_RealMapMin;
                mapIndex = m_ColourMaps.GetMap(map).length()*(mapValue/range);
                ui->m_OpenGLWidget->GetVerticesRef()[i][j].SetColour(m_ColourMaps.GetColour(map,mapIndex));
            }
        }
        else if(isLength)
        {
            for (int j = 0; j < ui->m_OpenGLWidget->GetVerticesRef()[i].length(); ++j)
            {
                mapValue = m_AtomVector[i]->GetPathLengthRef().last() - m_RealMapMin;
                mapIndex = m_ColourMaps.GetMap(map).length()*(mapValue/range);
                ui->m_OpenGLWidget->GetVerticesRef()[i][j].SetColour(m_ColourMaps.GetColour(map,mapIndex));
            }
        }
        else if(isVelocity)
        {
            for (int j = 0; j < ui->m_OpenGLWidget->GetVerticesRef()[i].length(); ++j)
            {
                mapValue = m_AtomVector[i]->GetVelocityRef()[j] - m_RealMapMin;
                mapIndex = m_ColourMaps.GetMap(map).length()*(mapValue/range);
                ui->m_OpenGLWidget->GetVerticesRef()[i][j].SetColour(m_ColourMaps.GetColour(map,mapIndex));
            }
        }
    }
    m_LastMappedTo = ui->m_Mapping->currentText();
    ui->m_OpenGLWidget->CreateTrajBuffer();
    ui->m_OpenGLWidget->update();
    printString("Colour mapping complete!",MS_SECOND);
}

void MainWindow::on_groSelectButton_clicked()
{
    QString groFilePath = QFileDialog::getOpenFileName(this,
                                                    tr("Select .gro file"),
                                                    QDir::homePath(),
                                                    tr(".gro files (*.gro)"));
    if (!groFilePath.isEmpty())
    {
        ui->groLineEdit->setText(groFilePath);
    }
}

void MainWindow::on_loadDataButton_clicked()
{
    printString("Starting!", MS_SECOND);

    ui->m_FrameBox->setValue(0);
    QString groFilePath = ui->groLineEdit->text();
    QString xtcFilePath = ui->xtcLineEdit->text();
    if(m_FileReader->LoadData(groFilePath, xtcFilePath))
    {
        ui->m_OpenGLWidget->ClearData();
        m_AtomVector.clear();
        m_AtomVector.squeeze();
        setAtomVector(m_FileReader->GetAtomVectorRef());
        printString("Files Loaded", MS_SECOND);
        int totalFrames = m_AtomVector[0]->GetTrajectoryRef().length();
        ui->m_FrameBox->setMaximum(totalFrames - 1);
        sort();
        createVertices();
        calculateDataRange();
        resetLegend();
        mapColour();
        ui->m_OpenGLWidget->SetBoundingBox(m_FileReader->GetSimBoxRef());
        ui->m_OpenGLWidget->ResetLighting();
        ui->m_OpenGLWidget->ResetView();
    }
}

void MainWindow::on_m_ApplyColour_released()
{
    calculateDataRange();
    mapColour();
}

void MainWindow::on_m_ColourSpinBox_valueChanged(int arg1)
{
    ui->m_ColourLegend->SetColourMap(m_ColourMaps.GetMap(arg1));
}

void MainWindow::on_m_LegendMax_textEdited(const QString &arg1)
{
    m_UserMapMax = arg1.toFloat();
    ui->m_LegendMid->setText(QString::number((m_UserMapMax + m_UserMapMin)/2).left(5));
}

void MainWindow::on_m_LegendMin_textEdited(const QString &arg1)
{
    m_UserMapMin = arg1.toFloat();
    ui->m_LegendMid->setText(QString::number((m_UserMapMax + m_UserMapMin)/2).left(5));
}

void MainWindow::on_m_RefreshRateSlider_valueChanged(int value)
{
    m_RefreshTime = MS_SECOND/value;
    setTimerStatus(false);
    setTimerStatus(ui->m_AnimateCheck->isChecked());
}

void MainWindow::on_m_ResetLegendScale_released()
{
    if (m_RealMapMax == -std::numeric_limits<float>::min() || m_RealMapMin == std::numeric_limits<float>::min())
    {
        return;
    }
    resetLegend();
}

void MainWindow::on_xtcSelectButton_clicked()
{
    QString xtcFilePath = QFileDialog::getOpenFileName(this,
                                                       tr("Select .xtc file"),
                                                       QDir::homePath(),
                                                       tr(".xtc files (*.xtc)"));
    if (!xtcFilePath.isEmpty())
    {
        ui->xtcLineEdit->setText(xtcFilePath);
    }
}

void MainWindow::outputFPS()
{
    ui->statusBar->showMessage("FPS: " + QString::number(m_FPS), MS_SECOND);
    m_FPS = 0;
}

void MainWindow::resetLegend()
{
    ui->m_LegendMax->setText(QString::number(m_RealMapMax,'f',3));
    ui->m_LegendMin->setText(QString::number(m_RealMapMin,'f',3));
    ui->m_LegendMid->setText(QString::number((m_RealMapMax + m_RealMapMin)/2,'f',3));
    m_UserMapMax = m_RealMapMax;
    m_UserMapMin = m_RealMapMin;
}

void MainWindow::printString(QString string, int duration)
{
    QTextStream* out = new QTextStream(stdout, QIODevice::WriteOnly);
    ui->statusBar->showMessage(string, duration);
    *out << string << endl;
    delete out;
}

void MainWindow::setTimerStatus(bool running)
{
    if (running)
    {
        m_Timer->start(m_RefreshTime);
        m_FPSTimer->start(MS_SECOND);
    }
    else
    {
        m_Timer->stop();
        m_FPSTimer->stop();
    }
}

void MainWindow::sort()
{
    struct {
        bool operator()(Atom* atom1, Atom* atom2)
        {
            return atom1->GetPathLengthRef().last()
                 < atom2->GetPathLengthRef().last();
        }
    } compare;
    m_FileReader->CalculatePathLength();
    std::sort(m_AtomVector.begin(), m_AtomVector.end(), compare);
}
