#include "Atom.h"
#include <math.h>
#include <QTextStream>

QString Atom::GetAtomName()
{
    return m_AtomName;
}

void Atom::SetAtomName(QString atomName)
{
    m_AtomName = atomName;
}

QString Atom::GetParentResidue()
{
    return m_ParentResidue;
}

void Atom::SetParentResidue(QString parentResidue)
{
    m_ParentResidue = parentResidue;
}

int Atom::GetParentResidueID()
{
    return m_ParentResidueID;
}

void Atom::setParentResidueID(int parentResidueID)
{
    m_ParentResidueID = parentResidueID;
}

QVector<float> &Atom::GetPathCurvatureRef()
{
    return m_PathCurvature;
}

void Atom::setPathCurvature(QVector<float> pathCurvature)
{
    m_PathCurvature = pathCurvature;
}

QVector<float> &Atom::GetPathLengthRef()
{
    return m_PathLength;
}

void Atom::setPathLength(QVector<float> pathLength)
{
    m_PathLength = pathLength;
}

QVector<int> &Atom::GetStepTimeRef()
{
    return m_StepTime;
}

void Atom::setStepTime(QVector<int> stepTime)
{
    m_StepTime = stepTime;
}

QVector<QVector3D> &Atom::GetTrajectoryRef()
{
    return m_Trajectory;
}

void Atom::setTrajectory(QVector<QVector3D> trajectory)
{
    m_Trajectory = trajectory;
}

QVector<QVector3D> &Atom::GetVelocityRef()
{
    return m_Velocity;
}

void Atom::setVelocity(QVector<QVector3D> velocity)
{
    m_Velocity = velocity;
}

Atom::Atom()
{

}

Atom::Atom(QString& atomDetail)
{
    int residueID = atomDetail.left(GRO_FIELD_SIZE).toInt();
    setParentResidueID(residueID);

    QString residueName = atomDetail.mid(RESIDUE_NAME_START,
                                         GRO_FIELD_SIZE).trimmed();
    SetParentResidue(residueName);

    QString atomName = atomDetail.mid(ATOM_NAME_START,
                                      GRO_FIELD_SIZE).trimmed();
    SetAtomName(atomName);

}

void Atom::AddTimeStep(float xPos,
                       float yPos,
                       float zPos,
                       int stepTime)
{
    QVector3D atomStepData(xPos, yPos, zPos);
    GetTrajectoryRef().append(atomStepData);
    GetStepTimeRef().append(stepTime);
}

void Atom::calculateAtomData()
{
    QVector<QVector3D> traj = GetTrajectoryRef();
    QVector<int> stepTime = GetStepTimeRef();
    for (int i = 1; i < traj.length(); ++i)
    {
        QVector3D displacement = traj[i] - traj[i-1];

        int timeStep = stepTime[i] - stepTime[i-1];
        QVector3D velocity = displacement/timeStep;
        GetVelocityRef().append(velocity);

        float pathLength = displacement.length() + GetPathLengthRef()[i];
        GetPathLengthRef().append(pathLength);

        float thisTheta = acos(traj[i].z()/traj[i].length());
        float thisPhi = atan(traj[i].y()/traj[i].x());
        float prevTheta = acos(traj[i-1].z()/traj[i-1].length());
        float prevPhi = atan(traj[i-1].y()/traj[i-1].x());
        float angleChange = fabs(thisTheta - prevTheta) + fabs(thisPhi - prevPhi);

        float curvature = angleChange + GetPathCurvatureRef()[i-1];
        GetPathCurvatureRef().append(curvature);
    }
}

void Atom::CalculatePathLength()
{
    QVector<QVector3D> traj = GetTrajectoryRef();
    GetPathLengthRef().append(0);
    for (int i = 1; i < traj.length(); ++i)
    {
        QVector3D displacement = traj[i] - traj[i-1];
        float pathLength = displacement.length() + GetPathLengthRef()[i-1];
        GetPathLengthRef().append(pathLength);
    }
}

void Atom::CalculatePathCurvature()
{
    QVector<QVector3D> traj = GetTrajectoryRef();
    GetPathCurvatureRef().append(0);
    for (int i = 1; i < traj.length(); ++i)
    {
        float thisTheta = acos(traj[i].z()/traj[i].length());
        float thisPhi = atan(traj[i].y()/traj[i].x());
        float prevTheta = acos(traj[i-1].z()/traj[i-1].length());
        float prevPhi = atan(traj[i-1].y()/traj[i-1].x());
        float angleChange = fabs(thisTheta - prevTheta) + fabs(thisPhi - prevPhi);

        float curvature = angleChange + GetPathCurvatureRef()[i-1];
        GetPathCurvatureRef().append(curvature);
    }
}

void Atom::CalculateVelocity()
{
    QVector<QVector3D> traj = GetTrajectoryRef();
    QVector<int> stepTime = GetStepTimeRef();
    GetVelocityRef().append(QVector3D(0,0,0));
    for (int i = 1; i < traj.length(); ++i)
    {
        QVector3D displacement = traj[i] - traj[i-1];
        int timeStep = stepTime[i] - stepTime[i-1];
        QVector3D velocity = displacement/timeStep;
        GetVelocityRef().append(velocity);
    }
}

void Atom::PrintAtom()
{
    QTextStream* out = new QTextStream(stdout, QIODevice::WriteOnly);
    *out << GetParentResidueID() << " " << GetParentResidue();
    *out << " " << GetAtomName() << endl;
    QVector<QVector3D> traj = GetTrajectoryRef();
    QVector<QVector3D> vel = GetVelocityRef();
    QVector<float> pathLength = GetPathLengthRef();
    QVector<float> pathCurvature = GetPathCurvatureRef();
    QVector<int> stepTime = GetStepTimeRef();
    for (int i = 0; i < traj.length(); ++i)
    {
        for (int j = 0; j < Atom::DIMENSIONS; ++j)
        {
            *out << traj[i][j] << '\t';
        }
        if (!vel.isEmpty())
        {
            for (int j = 0; j < Atom::DIMENSIONS; ++j)
            {
                *out << vel[i][j] << '\t';
            }
        }
        if (!pathLength.isEmpty())
        {
            *out << pathLength[i] << '\t';
        }
        if (!pathCurvature.isEmpty())
        {
            *out << pathCurvature[i] << '\t';
        }
        if (!stepTime.isEmpty())
        {
            *out << stepTime[i];
        }
        *out << endl;
    }
    delete out;
}

void Atom::PrintAtomFrame(int frame)
{
    QTextStream* out = new QTextStream(stdout, QIODevice::WriteOnly);
    *out << GetParentResidueID() << " " << GetParentResidue();
    *out << " " << GetAtomName() << " Frame = " << frame << endl;
    QVector<QVector3D> traj = GetTrajectoryRef();
    QVector<QVector3D> vel = GetVelocityRef();
    QVector<float> pathLength = GetPathLengthRef();
    QVector<float> pathCurvature = GetPathCurvatureRef();
    QVector<int> stepTime = GetStepTimeRef();

    for (int j = 0; j < Atom::DIMENSIONS; ++j)
    {
        *out << traj[frame][j] << '\t';
    }
    if (!vel.isEmpty())
    {
        for (int j = 0; j < Atom::DIMENSIONS; ++j)
        {
            *out << vel[frame][j] << '\t';
        }
    }
    if (!pathLength.isEmpty())
    {
        *out << pathLength[frame] << '\t';
    }
    if (!pathCurvature.isEmpty())
    {
        *out << pathCurvature[frame] << '\t';
    }
    if (!stepTime.isEmpty())
    {
        *out << stepTime[frame];
    }
    *out << endl;

    delete out;
}
