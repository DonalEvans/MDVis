#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include "Residue.h"
#include "FileReader.h"
#include "Vertex.h"
#include "ColourMaps.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void calculateFPS();

    void drawNextFrame();

    void on_groSelectButton_clicked();

    void on_loadDataButton_clicked();

    void on_m_ApplyColour_released();

    void on_m_ColourSpinBox_valueChanged(int arg1);

    void on_m_RefreshRateSlider_valueChanged(int value);

    void printString(QString, int);

    void setTimerStatus(bool);

    void on_xtcSelectButton_clicked();

    void on_m_LegendMin_textEdited(const QString &arg1);

    void on_m_LegendMax_textEdited(const QString &arg1);

    void on_m_ResetLegendScale_released();

private:

     QVector<Atom*>& getAtomVectorRef();

     void setAtomVector(QVector<Atom*> atomVector);

    void filter();

    void mapColour();

    void resetLegend();

    Ui::MainWindow *ui;
    QVector<Atom*> m_AtomVector;
    ColourMaps m_ColourMaps;
    FileReader* m_FileReader = new FileReader;
    int m_FPS = 0;
    QTimer* m_FPSTimer = new QTimer(this);
    float m_RealMapMax = -INFINITY;
    float m_RealMapMin = INFINITY;
    float m_UserMapMax;
    float m_UserMapMin;
    int m_RefreshTime = 1000;
    QTimer* m_Timer = new QTimer(this);

    int MS_SECOND = 1000;
};

#endif // MAINWINDOW_H
