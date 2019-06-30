#include "Cylinder.h"

#include "../../Common/Logger.h"

namespace Coconut
{
	Cylinder::Cylinder(float ld, float ud, float length, int faces)
    	: mID(rand()),
          mFaces(faces),
          mUpperDiameter(ud),
          mLowerDiameter(ld),
          mLength(length)
	{
		debug("SettingsModel: Constructor");
	}

    int Cylinder::GetID() const
    {
        return mID;
    }

    void Cylinder::SetID(int iD)
    {
        mID = iD;
    }

    int Cylinder::GetFaces() const
    {
        return mFaces;
    }

    void Cylinder::SetFaces(int faces)
    {
        mFaces = faces;
    }

    float Cylinder::GetUpperDiameter() const
    {
        return mUpperDiameter;
    }

    void Cylinder::SetUpperDiameter(float upperDiameter)
    {
        mUpperDiameter = upperDiameter;
    }

    float Cylinder::GetLowerDiameter() const
    {
        return mLowerDiameter;
    }

    void Cylinder::SetLowerDiameter(float lowerDiameter)
    {
        mLowerDiameter = lowerDiameter;
    }

    float Cylinder::GetLength() const
    {
        return mLength;
    }

    void Cylinder::SetLength(float length)
    {
        mLength = length;
    }

    json Cylinder::ToJson()
	{
        info("Cylinder: {}",__FUNCTION__);
        json j;
        j[CYLINDER_ID] = GetID();
		j[CYLINDER_FACES] = GetFaces();
        j[CYLINDER_UPPER_DIAMETER] = GetUpperDiameter();
        j[CYLINDER_LOWER_DIAMETER] = GetLowerDiameter();
        j[CYLINDER_LENGTH] = GetLength();
        return j;
	}

	bool Cylinder::FromJson(const json& j)
	{
        info("Cylinder: {}",__FUNCTION__);
        if (j.find(CYLINDER_ID) != j.end() && j[CYLINDER_ID].is_number())
        {
        	SetID(j[CYLINDER_ID]);
        }

        if (j.find(CYLINDER_FACES) != j.end() && j[CYLINDER_FACES].is_number())
        {
        	SetFaces(j[CYLINDER_FACES]);
        }

        if (j.find(CYLINDER_UPPER_DIAMETER) != j.end() && j[CYLINDER_UPPER_DIAMETER].is_number())
        {
        	SetUpperDiameter(j[CYLINDER_UPPER_DIAMETER]);
        }

        if (j.find(CYLINDER_LOWER_DIAMETER) != j.end() && j[CYLINDER_LOWER_DIAMETER].is_number())
        {
        	SetLowerDiameter(j[CYLINDER_LOWER_DIAMETER]);
        }

        if (j.find(CYLINDER_LENGTH) != j.end() && j[CYLINDER_LENGTH].is_number())
        {
        	SetLength(j[CYLINDER_LENGTH]);
        }

        return true;
    }

    bool Cylinder::operator==(const Cylinder& other)
    {
        return mID == other.mID &&
        		mUpperDiameter == other.mUpperDiameter &&
                mLowerDiameter == other.mLowerDiameter &&
                mLength == other.mLength &&
                mFaces == other.mFaces;
    }

    bool Cylinder::operator!=(const Cylinder& other)
    {
		return mID != other.mID ||
			mUpperDiameter != other.mUpperDiameter ||
			mLowerDiameter != other.mLowerDiameter ||
			mLength != other.mLength ||
			mFaces != other.mFaces;
    }


    const string Cylinder::CYLINDER_ID = "id";
    const string Cylinder::CYLINDER_FACES = "faces";
    const string Cylinder::CYLINDER_UPPER_DIAMETER = "upper_diameter";
    const string Cylinder::CYLINDER_LOWER_DIAMETER = "lower_diameter";
    const string Cylinder::CYLINDER_LENGTH = "length";
}
