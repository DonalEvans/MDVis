#include "Residue.h"
#include <QTextStream>

QVector<Atom*>& Residue::GetAtomVectorRef()
{
    return m_AtomVector;
}

void Residue::setAtomVector(QVector<Atom*> atomVector)
{
    m_AtomVector = atomVector;
}

int Residue::GetResidueID()
{
    return m_ResidueID;
}

void Residue::SetResidueID(int residueID)
{
    m_ResidueID = residueID;
}

QString Residue::GetResidueName()
{
    return m_ResidueName;
}

void Residue::SetResidueName(QString residueName)
{
    m_ResidueName = residueName;
}

Residue::Residue()
{

}

void Residue::AddAtom(Atom* atom)
{
    GetAtomVectorRef().append(atom);
}

void Residue::PrintResidue()
{
    QTextStream out(stdout);
    out << GetResidueID() << " " << GetResidueName() << " " << endl;
    QVector<Atom*> atomVector = GetAtomVectorRef();
    for (int i = 0; i < atomVector.length(); ++i)
    {
        out << atomVector[i]->GetAtomName() << endl;
    }
}
