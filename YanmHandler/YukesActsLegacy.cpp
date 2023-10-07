#include "YukesActsLegacy.h"
#include "Container/C_YukesAnimFile.h"
#include <sstream>
using namespace BinaryIO;

void YukesActsLegacy::LoadTree() {
    InitializeTree();
    fs->seekg(0x48); /* const definition at 0x48 */
    CollectNodes();
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
    streamPos = fs->tellg();

    for (int i = 0; i < numAnims; i++) {
        fs->seekg(this->streamPos);
        GetNodeData();
        ReadYukesMotion();   }
}

std::istringstream* CreateStreamFromData(std::istream* stream, uint32_t size) {
    std::vector<char> fileData;
    fileData.resize(size);
    stream->read(fileData.data(), size);

    std::istringstream* buffer =
        new std::istringstream(std::string(fileData.begin(), fileData.end()));
    return buffer;
}

void YukesActsLegacy::GetNodeData() {
    /* Gather stream data */
    int parentIndex = ReadShortBE(*fs);
    this->animSize = ReadUShortBE(*fs) << 0x5;
    this->name = ReadString(*fs, 0x20);
    uint32_t beginAddress = this->streamBegin + ReadUInt32BE(*fs);

    /* Read YANM buffer */
    streamPos = fs->tellg();
    fs->seekg(beginAddress);
}

void YukesActsLegacy::ReadYukesMotion() {
    /* Read YANM stream to a new buffer*/
    std::istringstream* buffer = CreateStreamFromData(fs, this->animSize);

    /* Pass stream to reader to handle YANM*/
    std::ifstream yanmStream;
    yanmStream.set_rdbuf(buffer->rdbuf());

    YukesAnimFile* yukesMotion = new YukesAnimFile(&yanmStream);
    YukesAnimRegistry* registry = yukesMotion->m_Registry;
    this->nodes.push_back( YukesAnimNode{ registry, name } );

    /* Clean memory */
    buffer->clear();
    delete buffer;
}

