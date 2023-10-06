#include "./DataIO/BinaryIO.h"
#include "AnimationUtils.h"
using namespace BinaryIO;

void AnimUtils::StreamMatrix3x3(std::istream* fs, Matrix3x3* mat){
	// float URotToDegree = 1.0; pretransform
	mat->row0 = Vec3{ URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };
	mat->row1 = Vec3{ URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };
	mat->row2 = Vec3{ URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs), URotToDegree * ReadShortBE(*fs) };
}

