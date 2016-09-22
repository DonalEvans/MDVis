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

QVector<float>& Atom::GetPathCurvatureRef()
{
    return m_PathCurvature;
}

void Atom::setPathCurvature(QVector<float> pathCurvature)
{
    m_PathCurvature = pathCurvature;
}

QVector<float>& Atom::GetPathLengthRef()
{
    return m_PathLength;
}

void Atom::setPathLength(QVector<float> pathLength)
{
    m_PathLength = pathLength;
}

QVector<int>& Atom::GetStepTimeRef()
{
    return m_StepTime;
}

void Atom::setStepTime(QVector<int> stepTime)
{
    m_StepTime = stepTime;
}

QVector<QVector3D>& Atom::GetTrajectoryRef()
{
    return m_Trajectory;
}

void Atom::setTrajectory(QVector<QVector3D> trajectory)
{
    m_Trajectory = trajectory;
}

QVector<float>& Atom::GetVelocityRef()
{
    return m_Velocity;
}

void Atom::setVelocity(QVector<float> velocity)
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

void Atom::CalculatePathLength()
{
    QVector<QVector3D> traj = GetTrajectoryRef();
    GetPathLengthRef().append(0);
    QVector3D displacement;
    for (int i = 1; i < traj.length(); ++i)
    {
        displacement = traj[i] - traj[i-1];
        float pathLength = displacement.length() + GetPathLengthRef()[i-1];
        GetPathLengthRef().append(pathLength);
    }
}

void Atom::CalculatePathCurvature()
{
    QVector<QVector3D> traj = GetTrajectoryRef();
    for (int i = 1; i < traj.length(); ++i)
    {
        m_ThisTheta = acos((traj[i].z()-traj[i-1].z())
                           /(traj[i]-traj[i-1]).length());
        m_ThisPhi = atan((traj[i].y()-traj[i-1].y())
                          /(traj[i].x()-traj[i-1].x()));

        float curvature = fabs(m_ThisTheta - m_PrevTheta)
                        + fabs(m_ThisPhi - m_PrevPhi);
        GetPathCurvatureRef().append(curvature);
        m_PrevTheta = m_ThisTheta;
        m_PrevPhi = m_ThisPhi;

    }
    GetPathCurvatureRef()[0] = 0;
    GetPathCurvatureRef().append(GetPathCurvatureRef().last());
}

void Atom::CalculateVelocity()
{
    GetVelocityRef().append(0);
    QVector3D displacement;
    float velocity;
    for (int i = 1; i < GetTrajectoryRef().length(); ++i)
    {
        displacement = GetTrajectoryRef()[i] - GetTrajectoryRef()[i-1];
        int timeStep = GetStepTimeRef()[i] - GetStepTimeRef()[i-1];
        velocity = MS_SECOND*displacement.length()/timeStep;
        GetVelocityRef().append(velocity);
    }
    GetVelocityRef()[0] = GetVelocityRef()[1];
}

void Atom::PrintAtom()
{
    QTextStream* out = new QTextStream(stdout, QIODevice::WriteOnly);
    *out << GetParentResidueID() << " " << GetParentResidue();
    *out << " " << GetAtomName() << endl;
    QVector<QVector3D> traj = GetTrajectoryRef();
    QVector<float> vel = GetVelocityRef();
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
            *out << vel[i] << '\t';
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
    QVector<float> vel = GetVelocityRef();
    QVector<float> pathLength = GetPathLengthRef();
    QVector<float> pathCurvature = GetPathCurvatureRef();
    QVector<int> stepTime = GetStepTimeRef();

    for (int j = 0; j < Atom::DIMENSIONS; ++j)
    {
        *out << traj[frame][j] << '\t';
    }
    if (!vel.isEmpty())
    {
        *out << vel[frame] << '\t';
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
