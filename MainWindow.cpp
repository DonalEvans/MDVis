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
    QObject::connect(ui->m_CircleRadiusBox, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetCircleRadius(int)));
    QObject::connect(ui->m_FrameBox, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetFrame(int)));
    QObject::connect(m_FPSTimer, SIGNAL(timeout()),
                     this, SLOT(calculateFPS()));
    QObject::connect(ui->m_ResetCamera, SIGNAL(released()),
                     ui->m_OpenGLWidget, SLOT(ResetView()));
    QObject::connect(ui->m_ResetLighting, SIGNAL(released()),
                     ui->m_OpenGLWidget, SLOT(ResetLighting()));
    QObject::connect(ui->m_Ambient, SIGNAL(valueChanged(int)),
                     ui->m_OpenGLWidget, SLOT(SetAmbientValue(int)));
    ui->m_ColourLegend->SetIsHorizontal(false);
    ui->m_ColourSpinBox->setMaximum(m_ColourMaps.COLOUR_VECT.length()-1);
    int map = ui->m_ColourSpinBox->value();
    ui->m_ColourLegend->SetColourMap(m_ColourMaps.COLOUR_VECT[map]);
    ui->m_LegendMax->setValidator(new QDoubleValidator(this));
    ui->m_LegendMin->setValidator(new QDoubleValidator(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateFPS()
{
    ui->statusBar->showMessage("FPS: " + QString::number(m_FPS), MS_SECOND);
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
    printString("Filtering atoms...",MS_SECOND);

    bool condition = true;

    QVector<Vertex> vertices;
    Vertex vertex;

    for (int i = 0; i < m_AtomVector.length(); ++i)
    {
        if (condition)
        {
            if (m_AtomVector[i]->GetPathLengthRef().last() < m_RealMapMin)
            {
                m_RealMapMin = m_AtomVector[i]->GetPathLengthRef().last();
            }
            else if (m_AtomVector[i]->GetPathLengthRef().last() > m_RealMapMax)
            {
                m_RealMapMax = m_AtomVector[i]->GetPathLengthRef().last();
            }
            for (int j = 0; j < m_AtomVector[i]->GetTrajectoryRef().length(); ++j)
            {
                vertex.SetPosition(m_AtomVector[i]->GetTrajectoryRef()[j]);
                vertices.append(vertex);
            }
        }
        ui->m_OpenGLWidget->AddVertices(vertices);
        vertices.clear();
    }
    resetLegend();
//    printString("Filtering complete!",MS_SECOND);
}

void MainWindow::mapColour()
{
    if (ui->m_OpenGLWidget->GetVerticesRef().length() == 0)
    {
        return;
    }
    printString("Mapping colour to atoms...", MS_SECOND);
    float range = m_UserMapMax - m_UserMapMin;
    float mapValue;
    int mapIndex;
    int map = ui->m_ColourSpinBox->value();
    for (int i = 0; i < ui->m_OpenGLWidget->GetVerticesRef().length(); ++i)
    {
        for (int j = 0; j < ui->m_OpenGLWidget->GetVerticesRef()[i].length(); ++j)
        {
            mapValue = m_AtomVector[i]->GetPathLengthRef().last() - m_RealMapMin;
            mapIndex = m_ColourMaps.COLOUR_VECT[map].length()*(1-(mapValue/range));
            if (mapIndex >= m_ColourMaps.COLOUR_VECT[map].length())
            {
                mapIndex = m_ColourMaps.COLOUR_VECT[map].length() - 1;
            }
            else if (mapIndex < 0)
            {
                mapIndex = 0;
            }
            ui->m_OpenGLWidget->GetVerticesRef()[i][j].SetColour(m_ColourMaps.COLOUR_VECT[map][mapIndex]);
        }
    }
    ui->m_OpenGLWidget->CreateTrajBuffer();
    ui->m_OpenGLWidget->update();
    printString("Colour mapping complete!",MS_SECOND);
}

void MainWindow::resetLegend()
{
    ui->m_LegendMax->setText(QString::number(m_RealMapMax).left(5));
    ui->m_LegendMin->setText(QString::number(m_RealMapMin).left(5));
    ui->m_LegendMid->setText(QString::number((m_RealMapMax + m_RealMapMin)/2).left(5));
    m_UserMapMax = m_RealMapMax;
    m_UserMapMin = m_RealMapMin;
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
//        printString("Files Loaded", MS_SECOND);
        int totalFrames = m_AtomVector[0]->GetTrajectoryRef().length();
        ui->m_FrameBox->setMaximum(totalFrames - 1);
        filter();
        mapColour();
        ui->m_OpenGLWidget->SetBoundingBox(m_FileReader->GetSimBoxRef());
        ui->m_OpenGLWidget->ResetLighting();
        ui->m_OpenGLWidget->ResetView();
    }
}

void MainWindow::on_m_RefreshRateSlider_valueChanged(int value)
{
    m_RefreshTime = MS_SECOND/value;
    setTimerStatus(false);
    setTimerStatus(ui->m_AnimateCheck->isChecked());
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

void MainWindow::printString(QString string, int duration)
{
    QTextStream* out = new QTextStream(stdout, QIODevice::WriteOnly);
    ui->statusBar->showMessage(string, duration);
    *out << string << endl;
    delete out;
}

void MainWindow::setTimerStatus(bool animate)
{
    if (animate)
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

void MainWindow::on_m_ColourSpinBox_valueChanged(int arg1)
{
    ui->m_ColourLegend->SetColourMap(m_ColourMaps.COLOUR_VECT[arg1]);
}

void MainWindow::on_m_ApplyColour_released()
{
    mapColour();
}

void MainWindow::on_m_LegendMin_textEdited(const QString &arg1)
{
    m_UserMapMin = arg1.toFloat();
    ui->m_LegendMid->setText(QString::number((m_UserMapMax + m_UserMapMin)/2).left(5));

}

void MainWindow::on_m_LegendMax_textEdited(const QString &arg1)
{
    m_UserMapMax = arg1.toFloat();
    ui->m_LegendMid->setText(QString::number((m_UserMapMax + m_UserMapMin)/2).left(5));
}

void MainWindow::on_m_ResetLegendScale_released()
{
    if (m_RealMapMax == -INFINITY || m_RealMapMin == INFINITY)
    {
        return;
    }
    resetLegend();
}
