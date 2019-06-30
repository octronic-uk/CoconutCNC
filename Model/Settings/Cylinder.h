#pragma once

#include "JsonSerialization.h"
#include <string>

using std::string;

namespace Coconut
{
	class Cylinder : public JsonSerialization
	{
	public:
		Cylinder(float ld = 5.f, float ud = 5.f, float length = 20.f, int faces = 6 );

        int GetID() const;
        void SetID(int iD);

        int GetFaces() const;
        void SetFaces(int faces);

        float GetUpperDiameter() const;
        void SetUpperDiameter(float upperDiameter);

        float GetLowerDiameter() const;
        void SetLowerDiameter(float lowerDiameter);

        float GetLength() const;
        void SetLength(float length);

        json ToJson() override;
        bool FromJson(const json& j) override;

        bool operator==(const Cylinder& other);
        bool operator!=(const Cylinder& other);

    protected:
        int mID;
        int mFaces;
        float mUpperDiameter;
        float mLowerDiameter;
        float mLength;

    private:
        const static string CYLINDER_ID;
        const static string CYLINDER_FACES;
        const static string CYLINDER_UPPER_DIAMETER;
        const static string CYLINDER_LOWER_DIAMETER;
        const static string CYLINDER_LENGTH;
    };
}
