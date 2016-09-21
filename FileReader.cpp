#include "FileReader.h"
#include "xdrfile.h"
#include "xdrfile_xtc.h"
#include <QFile>
#include <QTextStream>

QVector<Atom*>& FileReader::GetAtomVectorRef()
{
    return m_AtomVector;
}

void FileReader::setAtomVector(QVector<Atom*> atomVector)
{
    m_AtomVector = atomVector;
}

QStringList FileReader::getGroList()
{
    return m_GroList;
}

void FileReader::setGroList(QStringList& groList)
{
    m_GroList = groList;
}

float FileReader::GetMaxPathCurvature()
{
    return m_MaxPathCurvature;
}

float FileReader::GetMaxPathLength()
{
    return m_MaxPathLength;
}

float FileReader::GetMaxVelocity()
{
    return m_MaxVelocity;
}

float FileReader::GetMinPathCurvature()
{
    return m_MinPathCurvature;
}

float FileReader::GetMinPathLength()
{
    return m_MinPathLength;
}

float FileReader::GetMinVelocity()
{
    return m_MinVelocity;
}

int FileReader::getNumOfResidues()
{
    return m_NumOfResidues;
}

void FileReader::setNumOfResidues(int numOfResidues)
{
    m_NumOfResidues = numOfResidues;
}

QVector<Residue *> &FileReader::GetResidueVectorRef()
{
    return m_ResidueVector;
}

void FileReader::SetResidueVector(QVector<Residue *> residueVector)
{
    m_ResidueVector = residueVector;
}

QVector3D &FileReader::GetSimBoxRef()
{
    return m_SimBox;
}

void FileReader::setSimBox(float x, float y, float z)
{
    m_SimBox.setX(x);
    m_SimBox.setY(y);
    m_SimBox.setZ(z);
}

FileReader::FileReader()
{

}

void FileReader::CalculatePathCurvature()
{
    if(!m_PathCurvature)
    {
        emit consoleOutput("Calculating Path Curvature",0);
        QVector<float> pathCurve;
        for (int i = 0; i < GetAtomVectorRef().length(); ++i)
        {
            GetAtomVectorRef()[i]->CalculatePathCurvature();
            pathCurve = GetAtomVectorRef()[i]->GetPathCurvatureRef();
            for (int j = 0; j < pathCurve.length(); ++j)
            {
                if (pathCurve[j] > m_MaxPathCurvature)
                {
                    m_MaxPathCurvature = pathCurve[j];
                }
                else if (pathCurve[j] < m_MinPathCurvature)
                {
                    m_MinPathCurvature = pathCurve[j];
                }
            }
        }
        m_PathCurvature = true;
    }
}

void FileReader::CalculatePathLength()
{
    if(!m_PathLength)
    {
        emit consoleOutput("Calculating Path Length",0);
        QVector<float> pathLength;
        for (int i = 0; i < GetAtomVectorRef().length(); ++i)
        {
            GetAtomVectorRef()[i]->CalculatePathLength();
            pathLength = GetAtomVectorRef()[i]->GetPathLengthRef();
            if (pathLength.last() > m_MaxPathLength)
            {
                m_MaxPathLength = pathLength.last();
            }
            else if (pathLength.last() < m_MinPathLength)
            {
                m_MinPathLength = pathLength.last();
            }
        }
        m_PathLength = true;
    }
}

void FileReader::CalculateVelocity()
{
    if(!m_Velocity)
    {
        emit consoleOutput("Calculating velocity magnitude",0);
        QVector<QVector3D> velocity;
        for (int i = 0; i < GetAtomVectorRef().length(); ++i)
        {
            GetAtomVectorRef()[i]->CalculateVelocity();
            velocity = GetAtomVectorRef()[i]->GetVelocityRef();
            for (int j = 0; j < velocity.length(); ++j)
            {
                if (velocity[j].length() > m_MaxVelocity)
                {
                    m_MaxVelocity = velocity[j].length();
                }
                else if (velocity[j].length() < m_MinVelocity)
                {
                    m_MinVelocity = velocity[j].length();
                }
            }
        }
        m_Velocity = true;
    }
}

void FileReader::addResidue(Residue* residue, int index)
{
    GetResidueVectorRef()[index] = residue;
}

void FileReader::clearAtomVector()
{
    emit consoleOutput("Clearing atom vector",0);
    QVectorIterator<Atom*> atomIterator(GetAtomVectorRef());
    while (atomIterator.hasNext())
    {
        delete atomIterator.next();
    }
    GetAtomVectorRef().clear();
    GetAtomVectorRef().squeeze();
    m_PathCurvature = false;
    m_PathLength = false;
    m_Velocity = false;
}

void FileReader::clearResidueVector()
{
    emit consoleOutput("Clearing residue vector",0);
    for (int i = 0; i < GetResidueVectorRef().length(); ++i)
    {
        delete GetResidueVectorRef()[i];
    }
    GetResidueVectorRef().clear();
    GetResidueVectorRef().squeeze();
}

void FileReader::createAtomVector()
{
    emit consoleOutput("Creating atom vector...",0);
    QStringListIterator groIterator(getGroList());
    setNumOfResidues(0);
    while (groIterator.hasNext())
    {
        QString atomDetail = groIterator.next();
        Atom* newAtomPtr = new Atom(atomDetail);
        int residueNumber = newAtomPtr->GetParentResidueID();
        if (residueNumber > getNumOfResidues())
        {
            setNumOfResidues(residueNumber);
        }
        GetAtomVectorRef().append(newAtomPtr);
    }
}

void FileReader::createGroList(QString groFileData)
{
    QStringList groList = groFileData.split(QString("\n"));
    groList.removeFirst();
    groList.removeFirst();
    while (groList.last().isEmpty())
    {
        groList.removeLast();
    }
    groList.removeLast();
    setGroList(groList);
}

void FileReader::createResidueVector()
{
    GetResidueVectorRef().resize(getNumOfResidues());
    GetResidueVectorRef().fill(0);
    QVectorIterator<Atom*> atomIterator(GetAtomVectorRef());
    while (atomIterator.hasNext())
    {
        Atom* atomPtr = atomIterator.next();
        int residueID = atomPtr->GetParentResidueID();

        if (GetResidueVectorRef()[residueID - 1] == 0)
        {
            Residue* residuePtr = new Residue();
            residuePtr->SetResidueID(residueID);
            QString residueName = atomPtr->GetParentResidue();
            residuePtr->SetResidueName(residueName);
            addResidue(residuePtr, residueID - 1);
        }

        GetResidueVectorRef()[residueID - 1]->AddAtom(atomPtr);

    }
}

bool FileReader::fetchGroData(const QString& groFilePath)
{
    QFile groFile(groFilePath);
    QString groFileData;

    if (groFile.open(QIODevice::ReadOnly))
    {
        groFileData = groFile.readAll();
        groFile.close();

        createGroList(groFileData);
        createAtomVector();
        return true;
    }
    else
    {
        emit consoleOutput("Could not open .gro file.",0);
        return false;
    }
}

bool FileReader::fetchXtcData(const QString& xtcFilePath)
{
    emit consoleOutput("Fetching .xtc data...",0);
    char* xtcCharPath = xtcFilePath.toLatin1().data();
    int xtcResult;
    int xtcNumOfAtoms;

    xtcResult = read_xtc_natoms(xtcCharPath,& xtcNumOfAtoms);

    if (xtcResult != exdrOK)
    {
        emit consoleOutput("Failed to open .xtc file.",0);
        return false;
    }

    if (xtcNumOfAtoms != getGroList().length())
    {
        emit consoleOutput(".gro file and .xtc file "
                           "have different number of atoms!",0);
        return false;
    }

    XDRFILE* xtcFile;
    xtcFile = xdrfile_open(xtcCharPath, "r");

    if (xtcFile == NULL)
    {
        xdrfile_close(xtcFile);
        return false;
    }

    int xtcStep;
    float xtcTime;
    matrix boxMatrix;
    int actualStep = 0;
    int startTime;
    int stepTime = 0;

    while(1)
    {
        rvec* xtcPosition;
        xtcPosition = (rvec* )calloc(xtcNumOfAtoms, sizeof(xtcPosition[0]));

        xtcResult = read_xtc(xtcFile,xtcNumOfAtoms,&xtcStep,&xtcTime,
                             boxMatrix,xtcPosition,&XTC_PREC);

        if (xtcResult == exdrOK)
        {
            if (actualStep == 0)
            {
                startTime = xtcTime;
            }
            stepTime = xtcTime - startTime;

            int atomIndex = 0;

            for (int i = 0; i < GetAtomVectorRef().length(); ++i)
            {
                float xPos = xtcPosition[atomIndex][X_POSITION];
                float yPos = xtcPosition[atomIndex][Y_POSITION];
                float zPos = xtcPosition[atomIndex][Z_POSITION];

                if ((actualStep > 0)&&
                   ((boxMatrix[X_POSITION][X_POSITION]/xPos > 0.9)||
                   (boxMatrix[X_POSITION][X_POSITION]/xPos < 0.1)))
                {
                    float prevX = GetAtomVectorRef()[i]->GetTrajectoryRef()[actualStep -1].x();
                    if (abs(prevX - xPos) > boxMatrix[X_POSITION][X_POSITION] / 2)
                    {
                        xPos += (boxMatrix[X_POSITION][X_POSITION]*
                                 (((prevX - xPos) > 0) - ((prevX - xPos) < 0)));
                    }
                }

                if ((actualStep > 0)&&
                   ((boxMatrix[Y_POSITION][Y_POSITION]/yPos > 0.9)||
                   (boxMatrix[Y_POSITION][Y_POSITION]/yPos < 0.1)))
                {
                    float prevY = GetAtomVectorRef()[i]->GetTrajectoryRef()[actualStep -1].y();
                    if (abs(prevY - yPos) > boxMatrix[Y_POSITION][Y_POSITION] / 2)
                    {
                        yPos += (boxMatrix[Y_POSITION][Y_POSITION]*
                                 (((prevY - yPos) > 0) - ((prevY - yPos) < 0)));
                    }
                }

                GetAtomVectorRef()[i]->AddTimeStep(xPos, yPos, zPos, stepTime);
                ++atomIndex;
            }
            ++actualStep;
        }
        else
        {
            setSimBox(boxMatrix[X_POSITION][X_POSITION],
                      boxMatrix[Y_POSITION][Y_POSITION],
                      boxMatrix[Z_POSITION][Z_POSITION]);
            break;
        }
    }

    xdrfile_close(xtcFile);
    emit consoleOutput(".xtc data fetching complete!",0);
    return true;
}

bool FileReader::LoadData(const QString& groFilePath,
                          const QString& xtcFilePath)
{
    if (GetAtomVectorRef().length() > 0)
    {
        clearAtomVector();
        clearResidueVector();
    }
    if (!fetchGroData(groFilePath))
    {
        return false;
    }

    if (!fetchXtcData(xtcFilePath))
    {
        return false;
    }

//    createResidueVector();

    return true;
}

void FileReader::print(QString output)
{
    QTextStream out(stdout);
    out << output << endl;
}
