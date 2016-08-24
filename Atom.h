/**
 * @file Atom.h
 * @author Donal Evans
 * @date 03 Jun 2016
 * @see Residue.h
 * @brief This class stores information for one atom from the input files.
 *
 * Detailed description goes here
 */

#ifndef ATOM_H
#define ATOM_H

#include <QVector>
#include <QVector3D>

class Atom
{
public:
    /**
     * @brief Getter for the Atom name.
     * @return The Atom name as a QString.
     */
    QString GetAtomName();

    /**
     * @brief Setter for the Atom name.
     * @param atomName The name of the Atom as a QString.
     */
    void SetAtomName(QString atomName);

    /**
     * @brief Getter for the name of the parent Residue of the Atom.
     * @return A String containing the name of the parent Residue.
     */
    QString GetParentResidue();

    /**
     * @brief Setter for the name of the parent Residue of the Atom.
     * @param parentResidue The name of the Residue to which this Atom belongs.
     */
    void SetParentResidue(QString parentResidue);

    /**
     * @brief Getter for the Residue ID for this Atom.
     * @return The ID of the Residue to which this Atom belongs.
     */
    int GetParentResidueID();
    

    /**
     * @brief Getter for the path curvature vector for this Atom.
     * @return A QVector containing the calculated path curvature at each 
     * time step as a float value.
     */
    QVector<float>& GetPathCurvatureRef();

    /**
     * @brief Getter for the path length vector for this Atom.
     * @return A QVector containing the calculated path length at each 
     * time step as a float value.
     */
    QVector<float>& GetPathLengthRef();

    /**
     * @brief Getter for the step time vector for this Atom.
     * @return A QVector containing the time for each step in ms as an int.
     */
    QVector<int>& GetStepTimeRef();

    /**
     * @brief Getter for the trajectory vector for this Atom.
     * @return A QVector containing the 3D coordinates of the Atom at each
     * time step.
     */
    QVector<QVector3D>& GetTrajectoryRef();

    /**
     * @brief Getter for the velocity vector for this Atom.
     * @return A QVector containing the 3D velocity of the Atom at each
     * time step.
     */
    QVector<QVector3D>& GetVelocityRef();

    /**
     * @brief Empty constructor.
     */
    Atom();

    /**
     * @brief Constructor using a String containing Atom name, parent Residue
     * name and parent Residue ID.
     * @param atomDetail The line of the .gro file for this Atom.
     */
    Atom(QString& atomDetail);

    /**
     * @brief addTimeStep Adds a new time step to the atom data.
     * @param xPos The x position of the atom.
     * @param yPos The y position of the atom.
     * @param zPos The z position of the atom.
     * @param stepTime The time at which this step occurs.
     */
    void AddTimeStep(float xPos,
                     float yPos,
                     float zPos,
                     int stepTime);

    /**
     * @brief Calculates the path curvature for this Atom at each time step 
     * using the trajectory data and stores the calcualted values.
     */
    void CalculatePathCurvature();

    /**
     * @brief Calculates the path length for this Atom at each time step using
     * the trajectory data and stores the calcualted values.
     */
    void CalculatePathLength();

    /**
     * @brief Calculates the velocity of this Atom at each time step using
     * the trajectory data and stores the calcualted values.
     */
    void CalculateVelocity();

    /**
     * @brief Prints out the information stored in the Atom object.
     */
    void PrintAtom();

    /**
     * @brief The number of spatial dimensions in the Atom data.
     */
    static const int DIMENSIONS = 3;

    /**
     * @brief The offset within a line of .gro file data of the residue name
     * string, in characters.
     */
    static const int RESIDUE_NAME_START = 5;

    /**
     * @brief The offset within a line of .gro file data of the atom name
     * string, in characters.
     */
    static const int ATOM_NAME_START = 10;

    /**
     * @brief The size in characters of the residue name and atom name
     * fields in the .gro file.
     */
    static const int GRO_FIELD_SIZE = 5;

private:
    /**
     * @brief Setter for the ID of the Residue to which this Atom belongs.
     * @param parentResidueID The ID of the Residue to which this Atom belongs.
     */
    void setParentResidueID(int parentResidueID);

    /**
     * @brief Setter for the path curvature vector for this Atom.
     * @param pathCurvature A QVector of float values representing the
     * curvature of the atom path at each time step.
     */
    void setPathCurvature(QVector<float> pathCurvature);

    /**
     * @brief Setter for the path length vector for this Atom.
     * @param pathLength A QVector of float values representing the
     * length of the atom path at each time step.
     */
    void setPathLength(QVector<float> pathLength);

    /**
     * @brief Setter for the step time vector for this Atom.
     * @param stepTime A QVector of int values representing the
     * time for each step in ms.
     */
    void setStepTime(QVector<int> stepTime);

    /**
     * @brief Setter for the trajectory vector for this Atom.
     * @param trajectory A QVector of 3D coordinates for the atom at
     * each time step.
     */
    void setTrajectory(QVector<QVector3D> trajectory);

    /**
     * @brief Setter for the velocity vector for this Atom.
     * @param velocity A QVector of 3D velocity values for the atom at
     * each time step.
     */
    void setVelocity(QVector<QVector3D> velocity);

    /**
     * @brief Calculates the velocity, cumulative path length and cumulative
     * path curvature for the Atom for all time steps.
     */
    void calculateAtomData();

    /**
     * @brief m_AtomName The name of the Atom.
     */
    QString m_AtomName;

    /**
     * @brief The name of the Residue to which this Atom belongs.
     */
    QString m_ParentResidue;

    /**
     * @brief The ID number of the Residue to which this Atom belongs.
     */
    int m_ParentResidueID;

    /**
     * @brief A QVector containing the path curvature for the Atom at each
     * time step.
     */
    QVector<float> m_PathCurvature;

    /**
     * @brief A QVector containing the path length for the Atom at each
     * time step.
     */
    QVector<float> m_PathLength;

    /**
     * @brief A QVector containing the step time in ms for each step.
     */
    QVector<int> m_StepTime;

    /**
     * @brief A QVector containing the 3D position of the Atom at each
     * time step.
     */
    QVector<QVector3D> m_Trajectory;

    /**
     * @brief A QVector containing the 3D velocity of the Atom at each
     * time step.
     */
    QVector<QVector3D> m_Velocity;
};

#endif // ATOM_H
