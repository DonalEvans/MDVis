/**
 * @file MainWindow.h
 * @author Donal Evans
 * @date 03 Jun 2016
 * @brief This class provides the main window for the application.
 *
 * UI elements are created and initialized, connection between signals and
 * slots are established, and user inputs are managed.
 */

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
    /**
     * @brief Constructor
     * @param parent The parent QWidget for this window.
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Destructor
     */
    ~MainWindow();

private slots:
    /**
     * @brief Increments the frame counter, and resets it if the final frame
     * has been reached.
     */
    void incrementFrame();

    /**
     * @brief Function describing actions to be taken upon clicking the .gro
     * file select button.
     */
    void on_groSelectButton_clicked();

    /**
     * @brief Function describing actions to be taken upon clicking the load
     * data button.
     */
    void on_loadDataButton_clicked();

    /**
     * @brief Function describing actions to be taken upon releasing the
     * Apply Colour Mapping button.
     */
    void on_m_ApplyColour_released();

    /**
     * @brief Function describing actions to be taken upon changing the
     * value of the colour map selection spin box.
     * @param arg1 The new value of the colour map selection spin box.
     */
    void on_m_ColourSpinBox_valueChanged(int arg1);

    /**
     * @brief Function describing actions to be taken upon editing the contents
     * of the maximum legend value line edit.
     * @param arg1 The new contents of the maximum legend value line edit.
     */
    void on_m_LegendMax_textEdited(const QString &arg1);

    /**
     * @brief Function describing actions to be taken upon editing the contents
     * of the minimum legend value line edit.
     * @param arg1 The new contents of the minimum legend value line edit.
     */
    void on_m_LegendMin_textEdited(const QString &arg1);

    /**
     * @brief Function describing actions to be taken upon changing the
     * value of the refresh rate slider.
     * @param value The new value of the refresh rate slider.
     */
    void on_m_RefreshRateSlider_valueChanged(int value);

    /**
     * @brief Function describing actions to be taken upon releasing the
     * reset legend scale button.
     */
    void on_m_ResetLegendScale_released();

    /**
     * @brief Function describing actions to be taken upon clicking the .xtc
     * file select button.
     */
    void on_xtcSelectButton_clicked();

    /**
     * @brief Outputs the current value of @m_FPS to the status bar.
     */
    void outputFPS();

    /**
     * @brief Prints a string to the console and to the status bar for duration
     * miliseconds.
     * @param string The QString to be printed.
     * @param duration The duration of the message in the status bar, in ms.
     */
    void printString(QString string, int duration);

    /**
     * @brief Determines if the timers associated with stepping through the
     * frames of the data are running or not.
     * @param running If true, the timers begin running and the visualization
     * will animate successive frames. If false, the frame being displayed will
     * not change.
     */
    void setTimerStatus(bool running);

private:
    /**
     * @brief Getter for the @Atom pointer vector.
     * @return A reference to a QVector of @Atom pointers.
     */
    QVector<Atom*>& getAtomVectorRef();

    /**
     * @brief Setter for the @Atom pointer vector.
     * @param atomVector The new value of the @Atom pointer vector.
     */
    void setAtomVector(QVector<Atom*> atomVector);

    /**
     * @brief Calculates the range of values for the currently colour-mapped
     * variable and updates the maximum and minimum mapping values accordingly.
     */
    void calculateDataRange();

    /**
     * @brief Generates a list of @Vertex objects from the @Atoms in
     * m_AtomVector and adds them to the OpenGL drawing surface.
     */
    void createVertices();

    /**
     * @brief Applies the currently selected colour mapping to the data.
     */
    void mapColour();

    /**
     * @brief Resets the maximum and minimum values of the legend to the values
     * determined by @calculateDataRange();
     */
    void resetLegend();

    /**
     * @brief Sorts m_AtomVector by path length.
     */
    void sort();

    /**
     * @brief The UI for this window.
     */
    Ui::MainWindow *ui;

    /**
     * @brief A QVector of @Atom pointers.
     */
    QVector<Atom*> m_AtomVector;

    /**
     * @brief The @ColourMaps object to be used.
     */
    ColourMaps m_ColourMaps;

    /**
     * @brief The @FileReader object to be used.
     */
    FileReader* m_FileReader = new FileReader;

    /**
     * @brief A counter for the number of frames drawn that gets reset every
     * second.
     */
    int m_FPS = 0;

    /**
     * @brief The timer that determines when the FPS value is output.
     */
    QTimer* m_FPSTimer = new QTimer(this);

    /**
     * @brief QString containing the name of the value to which colour was last
     * mapped.
     * Used to prevent unwanted actions if the mapping dropdown has not changed
     * since the last time colour was mapped.
     */
    QString m_LastMappedTo;

    /**
     * @brief The actual maximum value of the variable to which colour is
     * currently mapped.
     */
    float m_RealMapMax = -INFINITY;

    /**
     * @brief The actual minimum value of the variable to which colour is
     * currently mapped.
     */
    float m_RealMapMin = INFINITY;

    /**
     * @brief The timeout value in ms of @m_Timer.
     */
    int m_RefreshTime = 1000;

    /**
     * @brief The timer that determines the rate at which frames are iterated
     * through.
     */
    QTimer* m_Timer = new QTimer(this);

    /**
     * @brief The user-defined maximum value for use in colour mapping, used to
     * rescale the colour legend.
     */
    float m_UserMapMax;

    /**
     * @brief The user-defined minimum value for use in colour mapping, used to
     * rescale the colour legend.
     */
    float m_UserMapMin;

    /**
     * @brief The number of miliseconds in a second.
     */
    const int MS_SECOND = 1000;
};

#endif // MAINWINDOW_H
