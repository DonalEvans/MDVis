/**
 * @file Residue.h
 * @author Donal Evans
 * @date 03 Jun 2016
 * @see Atom.h
 * @brief This class stores information about one residue from the input files.
 *
 * Detailed description goes here
 */

#ifndef RESIDUE_H
#define RESIDUE_H

#include "Atom.h"
#include <QVector>

class Residue
{
public:

    /**
     * @brief Getter for the Vector of Atoms that belong to the Residue.
     * @return A QVector of Atom pointers.
     */
     QVector<Atom*>& GetAtomVectorRef();

    /**
     * @brief Getter for the Residue ID.
     * @return The unique ID number for the Residue.
     */
     int GetResidueID();

    /**
     * @brief Setter for the Residue ID.
     * @param residueID The unique ID number for the Residue.
     */
     void SetResidueID(int residueID);

    /**
     * @brief Getter for the Residue name.
     * @return A String containing the name of the Residue.
     */
     QString GetResidueName();

    /**
     * @brief Setter for the Residue name.
     * @param residueName A String containing the name of the Residue.
     */
     void SetResidueName(QString residueName);

    /**
     * @brief Constructor
     */
    Residue();

    /**
     * @brief Adds an Atom to the atom Vector.
     * @param newAtom The Atom to be added to the Vector.
     */
    void AddAtom(Atom* atom);

    /**
     * @brief Prints out the Residue info.
     */
    void PrintResidue();

private:

    /**
     * @brief Setter for the Vector of Atoms that belong to the Residue.
     * @param atomVector A QVector of Atom pointers.
     */
     void setAtomVector(QVector<Atom*> atomVector);

    /**
     * @brief A Vector that contains pointers to all of the Atoms that belong
     *        to the Residue.
     */
    QVector<Atom*> m_AtomVector;

    /**
     * @brief The unique ID number for the Residue.
     */
    int m_ResidueID;

    /**
     * @brief The name of the Residue.
     */
    QString m_ResidueName;

};

#endif // RESIDUE_H
