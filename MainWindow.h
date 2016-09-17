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

    void on_m_RefreshRateSlider_valueChanged(int value);

    void on_xtcSelectButton_clicked();

    void on_loadDataButton_clicked();

    void printString(QString, int);

    void setFrame(int frame){m_Frame = frame;}

    void setTimerStatus(bool);

    void on_m_ColourSpinBox_valueChanged(int arg1);

    void on_m_ApplyColour_released();

private:

     QVector<Atom*>& getAtomVectorRef();

     void setAtomVector(QVector<Atom*> atomVector);

    /**
     * @brief Getter for the Residue vector.
     * @return A reference to the Residue vector.
     */
     QVector<Residue*>& getResidueVectorRef();

    /**
     * @brief Setter for the Residue vector.
     * @param residueVector The new value of the Residue vector.
     */
     void setResidueVector(QVector<Residue*> residueVector);

    void filter();

    void mapColour();

    Ui::MainWindow *ui;
    QVector<Atom*> m_AtomVector;
    FileReader* m_FileReader = new FileReader;
    int m_FPS = 0;
    QTimer* m_FPSTimer = new QTimer(this);
    int m_Frame = 0;
    float m_MaxZ = -INFINITY;
    float m_MinZ = INFINITY;
    int m_RefreshTime = 1000;
    QVector<Residue*> m_ResidueVector;
    QTimer* m_Timer = new QTimer(this);
    ColourMaps m_ColourMaps;
};

#endif // MAINWINDOW_H
