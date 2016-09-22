/**
 * @file FileReader.h
 * @author Donal Evans
 * @date 03 Jun 2016
 * @see Atom.h
 * @see Residue.h
 * @brief This class opens and reads .gro and .xtc files and stores them.
 *
 * Detailed description goes here.
 */

#ifndef FILEREADER_H
#define FILEREADER_H

#include "Atom.h"
#include "Residue.h"
#include <QObject>
#include <QVector3D>
#include <limits>

class FileReader : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Getter for the Atom pointer vector.
     * @return A QVector of Atom pointers.
     */
    QVector<Atom*>& GetAtomVectorRef();

    /**
     * @brief Getter for the maximum path curvature value among the atoms
     * in the atom vector.
     * @return The value of the maximum path curvature, as a float.
     */
    float GetMaxPathCurvature();

    /**
     * @brief Getter for the maximum path length value among the atoms
     * in the atom vector.
     * @return The value of the maximum path length, as a float.
     */
    float GetMaxPathLength();

    /**
     * @brief Getter for the maximum velocity value among the atoms
     * in the atom vector.
     * @return The value of the maximum velocity, as a float.
     */
    float GetMaxVelocity();

    /**
     * @brief Getter for the minimum path curvature value among the atoms
     * in the atom vector.
     * @return The value of the minimum path curvature, as a float.
     */
    float GetMinPathCurvature();

    /**
     * @brief Getter for the minimum path length value among the atoms
     * in the atom vector.
     * @return The value of the minimum path length, as a float.
     */
    float GetMinPathLength();

    /**
     * @brief Getter for the minimum velocity value among the atoms
     * in the atom vector.
     * @return The value of the minimum velocity, as a float.
     */
    float GetMinVelocity();

    /**
     * @brief Getter for the vector containing the Residue pointers from
     *        the .gro file.
     * @return QVector of Residue pointers.
     */
    QVector<Residue*>& GetResidueVectorRef();

    /**
     * @brief Setter for the vector containing Residue pointers.
     * @param residueVector A QVector of Residue objects.
     */
    void SetResidueVector(QVector<Residue*> residueVector);

    /**
     * @brief Getter for the 3D vector containing the dimensions of the
     * simulation box.
     * @return QVector3D containing x, y and z dimensions.
     */
    QVector3D& GetSimBoxRef();

    /**
     * @brief Constructor
     */
    FileReader();

    /**
     * @brief Calculates the path curvature for every @Atom in the atom vector.
     */
    void CalculatePathCurvature();

    /**
     * @brief Calculates the path length for every @Atom in the atom vector.
     */
    void CalculatePathLength();

    /**
     * @brief Calculates the velocity for every @Atom in the atom vector.
     */
    void CalculateVelocity();

    /**
     * @brief Reads data from .gro and .xtc files and stores it.
     * @param groFilePath The file path of the .gro file.
     * @param xtcFilePath The file path of the .xtc file.
     * @return true if the data was loaded successfully, false otherwise.
     */
    bool LoadData(const QString& groFilePath,
                  const QString& xtcFilePath);

signals:

    /**
     * @brief Used to output messages from the class.
     * @param output The string to be output.
     * @param duration The duration in ms that the message will be displayed
     * on a QStatusBar.
     */
    void consoleOutput(QString output, int duration);

private:

    /**
     * @brief Setter for the Atom pointer vector.
     * @param atomVector A QVector of Atom pointers.
     */
    void setAtomVector(QVector<Atom*> atomVector);

    /**
     * @brief Getter for the list of Strings in the .gro file.
     * @return A list of Strings.
     */
    QStringList getGroList();

    /**
     * @brief Setter for the list of Strings in the .gro file.
     * @param groList A list of Strings.
     */
    void setGroList(QStringList& groList);

    /**
     * @brief Getter for the number of unique Residues in the .gro file.
     * @return An integer.
     */
    int getNumOfResidues();

    /**
     * @brief etter for the number of unique Residues in the .gro file.
     * @param NumOfResidues An integer value.
     */
    void setNumOfResidues(int numOfResidues);

    /**
     * @brief Setter for the 3D vector containing the dimensions of the
     * simulation box.
     * @param x The size of the simulation box in the X dimension.
     * @param y The size of the simulation box in the Y dimension.
     * @param z The size of the simulation box in the Z dimension.
     */
    void setSimBox(float x, float y, float z);

    /**
     * @brief Adds a Residue @e residue to the Residue vector at index @e index.
     * @param residue The Residue to be added to the Residue vector.
     * @param index The index at which the Residue will be added.
     */
    void addResidue(Residue* residue, int index);

    /**
     * @brief Removes all Atoms from the Atom vector.
     */
    void clearAtomVector();

    /**
     * @brief Removes all Residues from the Residue vector.
     */
    void clearResidueVector();

    /**
     * @brief Creates an Atom for each line of the .gro file data and adds
     * it to the Atom vector.
     */
    void createAtomVector();

    /**
     * @brief Stores a list of all the lines of data in the .gro file.
     *
     * Takes a String containing the entire contents of the .gro file, splits
     * it up by line and discards the first two lines and the last line, which
     * do not contain atom information.
     * @param groFileData The contents of the .gro file.
     */
    void createGroList(QString groFileData);

    /**
     * @brief Creates a vector of the residues in the .gro file.
     *
     * For each unique residue in the .gro file, creates a Residue object and
     * populates it with Atom objects for each atom belonging to that residue.
     */
    void createResidueVector();

    /**
     * @brief Reads data from the .gro file and stores it.
     * @param groFilePath The file path of the .gro file.
     * @return true if the data was fetched successfully, false otherwise.
     */
    bool fetchGroData(const QString& groFilePath);

    /**
     * @brief Reads data from the .xtc file and adds it to the data from
     *        the .gro file.
     * @param xtcFilePath The file path of the .xtc file.
     * @return true if the data was fetched successfully, false otherwise.
     */
    bool fetchXtcData(const QString& xtcFilePath);

    /**
     * @brief A QVector of pointers to all the Atoms in the .gro file.
     */
    QVector<Atom*> m_AtomVector;

    /**
     * @brief A list of Strings containing each line of the .gro file.
     */
    QStringList m_GroList;

    /**
     * @brief The value of the maximum path curvature value among the atoms
     * in the atom vector, as a float.
     */
    float m_MaxPathCurvature = -INFINITY;

    /**
     * @brief The value of the maximum path length value among the atoms
     * in the atom vector, as a float.
     */
    float m_MaxPathLength = -INFINITY;

    /**
     * @brief The value of the maximum velocity value among the atoms
     * in the atom vector, as a float.
     */
    float m_MaxVelocity = -INFINITY;

    /**
     * @brief The value of the minimum path curvature value among the atoms
     * in the atom vector, as a float.
     */
    float m_MinPathCurvature = INFINITY;

    /**
     * @brief The value of the minimum path length value among the atoms
     * in the atom vector, as a float.
     */
    float m_MinPathLength = INFINITY;

    /**
     * @brief The value of the minimum velocity value among the atoms
     * in the atom vector, as a float.
     */
    float m_MinVelocity = INFINITY;

    /**
     * @brief The number of unique Residues in the .gro file.
     */
    int m_NumOfResidues;

    /**
     * @brief Flag signifying if the path curvature has already been calculated for
     * the atoms in the atom vector or not.
     */
    bool m_PathCurvature;

    /**
     * @brief Flag signifying if the path length has already been calculated for
     * the atoms in the atom vector or not.
     */
    bool m_PathLength;

    /**
     * @brief A vector of all the Residues in the .gro file.
     */
    QVector<Residue*> m_ResidueVector;

    /**
     * @brief The dimensions of the computational box in which the Atoms exist.
     */
    QVector3D m_SimBox;

    /**
     * @brief Flag signifying if the velocity has already been calculated for
     * the atoms in the atom vector or not.
     */
    bool m_Velocity;

    /**
     * @brief Xtc position data is stored in reduced precision. This scaling
     * factor allows original precision to be restored.
     */
    float XTC_PREC = 1000.0;

    /**
     * @brief The index of the x coordinate in position vectors.
     */
    const int X_POSITION = 0;
    
    /**
     * @brief The index of the y coordinate in position vectors.
     */
    const int Y_POSITION = 1;

    /**
     * @brief The index of the z coordinate in position vectors.
     */
    const int Z_POSITION = 2;

    /**
     * @brief Allows printing of Strings to console.
     * @param output The String to be printed.
     */
    void print(QString output);
};

#endif // FILEREADER_H
