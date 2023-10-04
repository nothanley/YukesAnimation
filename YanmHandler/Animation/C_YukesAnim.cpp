#include "C_YukesAnim.h"
#include "../Container/C_YukesAnimFile.h"
#include <iostream>
using namespace std;

void YukesAnim::InitializeStream() {
	this->fs = m_FileObj->fs;
	ReadYANMStream();
}
void YukesAnim::ReadYANMStream() {
	ReadYanmAttributes();
}

void YukesAnim::ReadYanmAttributes() {
	m_FileObj->m_Version = ReadShort(*fs);
	cout << "YANM Version: " << m_FileObj->m_Version;
}