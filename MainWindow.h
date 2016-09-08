#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include "Residue.h"
#include "FileReader.h"
#include "Vertex.h"

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

    QVector<QVector3D> COLOUR_VECT = {
        QVector3D(103,0,31)/255,
        QVector3D(178,24,43)/255,
        QVector3D(214,96,77)/255,
        QVector3D(244,165,130)/255,
        QVector3D(253,219,199)/255,
        QVector3D(247,247,247)/255,
        QVector3D(209,229,240)/255,
        QVector3D(146,197,222)/255,
        QVector3D(67,147,195)/255,
        QVector3D(33,102,172)/255,
        QVector3D(5,48,97)/255
    };
};

#endif // MAINWINDOW_H
