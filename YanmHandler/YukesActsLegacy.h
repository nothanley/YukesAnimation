#pragma once
#include "Acts/C_YukesActsFile.h"
#include "Container/C_YAnimRegistry.h"

class YukesActsLegacy : public YukesActs{
public:
	unsigned int numAnims = 0;
	std::string name = "";
	std::vector<YukesAnimNode> nodes;

	~YukesActsLegacy() {
		for (auto& node : nodes)
			delete node.animation;	}

	YukesActsLegacy(std::istream* stream, YukesActsFile* parent) {
		this->fs = stream;
		this->m_FileObj = parent; }
	void LoadTree() override;

protected:
	std::istream* fs;
	YukesActsFile* m_FileObj;
private:
	std::streampos streamPos;
	uint32_t animSize;
	uint32_t tableSize;
	uint32_t streamSize;
	uint32_t tableBegin;
	uint32_t streamBegin;

	void InitializeTree();
	void CollectNodes();
	void GetNodeData();
	void ReadYukesMotion();
};