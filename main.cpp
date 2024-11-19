#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

#pragma pack(push, 1)

struct TIFFHeader
{
    uint16_t byteOrder;
    uint16_t tiffIdentifier;
    uint32_t ifdOffset;
};

struct IFDEntry
{
    uint16_t tag;
    uint16_t type;
    uint32_t count;
    uint32_t valueOffset;
};

void read_ascii(ifstream &file, IFDEntry &entry);

void readTIFFMetadata(const string &filename)
{
    ifstream file(filename, ios::binary);

    if (!file.is_open())
    {
        cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    TIFFHeader header;
    file.read((char *)&header, sizeof(header));

    bool isLittleEndian = (header.byteOrder == 0x4949);
    if (!isLittleEndian && header.byteOrder != 0x4D4D)
    {
        cerr << "Formato de byte desconhecido" << endl;
        return;
    }

    cout << "TIFF identifier = " << header.tiffIdentifier << "\n";

    file.seekg(header.ifdOffset, ios::beg);

    // Ler o número de entradas na IFD
    uint16_t numEntries;
    file.read(reinterpret_cast<char *>(&numEntries), sizeof(numEntries));

    cout << "Número de entradas na IFD: " << numEntries << endl;

    // Ler cada entrada IFD e verificar as tags conhecidas
    for (int i = 0; i < numEntries; ++i)
    {
        IFDEntry entrar;
        file.read((char *)&entrar, sizeof(entrar));

        if (!isLittleEndian)
        {
            // Converter para o formato do sistema (se necessário)
        }

        // Identificar algumas tags principais (apenas como exemplo)
        switch (entrar.tag)
        {
        case 256:
            std::cout << "ImageWidth: " << entrar.valueOffset << "\n";
            break;
        case 257:
            std::cout << "ImageLength: " << entrar.valueOffset << "\n";
            break;
        case 258:
            std::cout << "BitsPerSample: " << entrar.valueOffset << "\n";
            break;
        case 259:
            std::cout << "Compression: " << entrar.valueOffset << "\n";
            break;
        case 270:
            std::cout << "ImageDescription: "; 
            read_ascii(file, entrar);
            cout << "\n";
            break;
        case 271:
            std::cout << "Make (Camera Manufacturer): ";
            read_ascii(file, entrar);
            cout << "\n";
            break;
        case 272:
            std::cout << "Model (Camera Model): ";
            read_ascii(file, entrar);
            cout << "\n";
            break;
        case 282:
            std::cout << "XResolution: " << entrar.valueOffset << "\n";
            break;
        case 283:
            std::cout << "YResolution: " << entrar.valueOffset << "\n";
            break;
        case 305:
            std::cout << "Software: ";
            read_ascii(file, entrar);
            cout << "\n";
            break;
        case 306:
            std::cout << "DateTime: ";
            read_ascii(file, entrar);
            cout << "\n";
            break;
        case 33434:
            std::cout << "ExposureTime: " << entrar.valueOffset << "\n";
            break;
        case 33437:
            std::cout << "FNumber: " << entrar.valueOffset << "\n";
            break;
        case 34855:
            std::cout << "PhotographicSensitivity (ISO): " << entrar.valueOffset << "\n";
            break;
        case 37386:
            std::cout << "FocalLength: " << entrar.valueOffset << "\n";
            break;
        // default:
        //     std::cout << "Unknown Tag (" << entrar.tag << "): " << entrar.valueOffset << "\n";
        //     break;
        }
    }

    file.close();
}

void read_ascii(ifstream &file, IFDEntry &entry)
{
    if (entry.count <= 4)
    {
        // A string está diretamente em valueOffset
        char asciiValue[5] = {0};
        memcpy(asciiValue, &entry.valueOffset, entry.count);
        cout << asciiValue;
    }
    else
    {
        // Guarda posicao atual
        streampos currPos = file.tellg();

        file.seekg(entry.valueOffset, ios::beg);

        vector<char> dados(entry.count + 1, '\0');
        file.read(dados.data(), entry.count);
        cout << dados.data();

        file.seekg(currPos);
    }
}

int main()
{
    string filename = "passarinho.tiff";
    readTIFFMetadata(filename);
    return 0;
}