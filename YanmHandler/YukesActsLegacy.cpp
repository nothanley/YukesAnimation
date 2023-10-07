#include "YukesActsLegacy.h"
#include "Container/C_YukesAnimFile.h"
#include <sstream>
using namespace BinaryIO;


void YukesActsLegacy::LoadTree() {
    InitializeTree();
    fs->seekg(0x48); /* const definition at 0x48 */
    CollectNodes();
    //DebugLoadAllNodes();
}

void YukesActsLegacy::DebugLoadAllNodes() {
    /* To reduce compute, do not load YANM streams
       unless user calls for 'GetNode' */
    for (auto& node : this->nodes)
        ReadYukesMotion(&node);
    printf("\nAll motions collected.\n");
}

void YukesActsLegacy::InitializeTree() {
    fs->seekg(0x0);
    this->tableSize = ReadUInt32BE(*fs);
    this->streamSize = ReadUInt32BE(*fs);
    this->tableBegin = ReadUInt32BE(*fs);
    this->streamBegin = ReadUInt32BE(*fs);
}

void YukesActsLegacy::CollectNodes() {
    this->numAnims = ReadUInt32BE(*fs);
    this->name = ReadString(*fs, 0x20);
    uint32_t defSig = ReadUInt32BE(*fs);

    for (int i = 0; i < numAnims; i++)
        AddNode();
}

std::istringstream* CreateStreamFromData(std::istream* stream, uint32_t size) {
    std::vector<char> fileData;
    fileData.resize(size);
    stream->read(fileData.data(), size);

    std::istringstream* buffer =
        new std::istringstream(std::string(fileData.begin(), fileData.end()));
    return buffer;
}

void YukesActsLegacy::AddNode() {
    /* Gather stream data */
    int parentIndex = ReadShortBE(*fs); // We load this linear so ignore this
    uint32_t size = ReadUShortBE(*fs) << 0x5;
    this->name = ReadString(*fs, 0x20);
    uint32_t beginAddress = this->streamBegin + ReadUInt32BE(*fs);

    /* Save to tree */
    this->nodes.push_back(YukesAnimNode{ nullptr,name,beginAddress,size });
}

YukesAnimNode* YukesActsLegacy::GetNode(int index) {
    if (index > this->nodes.size()) { return nullptr; }
    YukesAnimNode* node = &this->nodes.at(index);
    /* Read YANM buffer, return pointer */
    ReadYukesMotion(node);
    return node;
}

void YukesActsLegacy::ReadYukesMotion(YukesAnimNode* node) {
    /* Read YANM stream to a new buffer*/
    fs->seekg(node->address);
    std::istringstream* buffer = CreateStreamFromData(fs, node->size);

    /* Pass stream to reader to handle YANM*/
    std::ifstream yanmStream;
    yanmStream.set_rdbuf(buffer->rdbuf());

    YukesAnimFile* yukesMotion = new YukesAnimFile(&yanmStream);
    node->animation = yukesMotion->m_Registry;

    /* Clean memory */
    buffer->clear();
    delete buffer;
}

