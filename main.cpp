#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

#pragma pack(push,1)

struct TIFFHeader {
    uint16_t byteOrder;
    uint16_t tiffIdentifier;
    uint32_t ifdOffset;
};

struct IFDEntry {
    uint16_t tag;
    uint16_t type;
    uint32_t count;
    uint32_t valueOffset;
};

void read_ascii(ifstream &file,IFDEntry &entry);

void readTIFFMetadata(const string& filename) {
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    //ler dados do cabeçalho
    TIFFHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Verificar a ordem dos bytes (little-endian ou big-endian)
    bool isLittleEndian = (header.byteOrder == 0x4949);
    if (!isLittleEndian && header.byteOrder != 0x4D4D) {
        cerr << "Formato de byte desconhecido" << endl;
        return;
    }

    cout << "TIFF identifier = " << header.tiffIdentifier << "\n";

    // Saltar para o offset da primeira IFD (Image File Directory)
    file.seekg(header.ifdOffset, ios::beg);

    // Ler o número de entradas na IFD
    uint16_t numEntries;
    file.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries));

    cout << "Número de entradas na IFD: " << numEntries << endl;

    // Ler cada entrada IFD e processar as tags conhecidas
    for (int i = 0; i < numEntries; ++i) {
        IFDEntry entrar;
        file.read(reinterpret_cast<char*>(&entrar), sizeof(entrar));

        if (!isLittleEndian) {
            // Converter para o formato do sistema (se necessário)
        }

        // Identificar algumas tags principais (apenas como exemplo)
        switch (entrar.tag) {
            case 0x0100: // ImageWidth
                cout << "Largura da imagem: " << entrar.valueOffset << endl;
                break;
            case 0x0101: // ImageLength
                cout << "Altura da imagem: " << entrar.valueOffset << endl;
                break;
            case 0x0102: // BitsPerSample
                cout << "Bits por amostra: " << entrar.valueOffset << endl;
                break;
            case 0x0106: // PhotometricInterpretation
                cout << "Modelo de cores: " << entrar.valueOffset << endl;
                break;
            case 0x0115: // SamplesPerPixel
                cout << "Amostras por pixel: " << entrar.valueOffset << endl;
                break;
            case 270:
                cout << "Image description: "; 
                read_ascii(file, entrar);
                cout << '\n';
                break;
            case 271:
                cout << "Fabricante do dispositivo: ";
                read_ascii(file, entrar);
                cout << '\n';
                break;
            case 272:
                cout << "Modelo: ";
                read_ascii(file, entrar);
                cout << '\n';
                break;
            case 305:
                cout << "ASCII: ";
                read_ascii(file, entrar);
                cout << '\n';
                break;
            case 306:
                cout << "ASCII: ";
                read_ascii(file, entrar);
                cout << '\n';
                break;
            case 315:
                cout << "ASCII: ";
                read_ascii(file, entrar);
                cout << '\n';
                break;
            case 33432:
                cout << "ASCII: ";
                read_ascii(file, entrar);
                cout << '\n';
                break;
            case 34675:
                cout << "ASCII: ";
                read_ascii(file, entrar);
                cout << '\n';
                break;
            default:
                std::cout << "tag - " << entrar.tag << "\n";
                std::cout << "type - " << entrar.type << "\n";
                std::cout << "count - " << entrar.count << "\n";
                std::cout << "valueOffstet - " << entrar.valueOffset << "\n";
                break;
        }
    }

    file.close();
}

void read_ascii(ifstream &file,IFDEntry &entry){
    if (entry.count <= 4){
        // A string está diretamente em valueOffset
        char asciiValue[5] = {0};
        memcpy(asciiValue, &entry.valueOffset, entry.count);
        cout << asciiValue;

    } else {
        //Guarda posicao atual
        streampos currPos = file.tellg();

        file.seekg(entry.valueOffset, ios::beg);

        vector<char> dados(entry.count + 1, '\0');
        file.read(dados.data(), entry.count);
        cout << dados.data();

        file.seekg(currPos);

    }
}

int main() {
    string filename = "passarinho.tiff";
    readTIFFMetadata(filename);
    return 0;
}