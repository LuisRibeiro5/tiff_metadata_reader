#include <iostream>
#include  <fstream>

struct IFD_entry
{
    unsigned short tag;
    unsigned short type;
    unsigned int count;
    unsigned valueOffset;
};


int main(){
    std::string path = "foto_trabalho.tiff";
    std::ifstream file(path, std::ios::binary);

    unsigned short byte_order;
    file.read((char*)&byte_order, sizeof(byte_order));
    std::cout << byte_order << "\n";

    if (byte_order == 0x4949) {
        std::cout << "Formato TIFF (Intel - little endian)" << std::endl;
    } else if (byte_order == 0x4D4D) {
        std::cout << "Formato TIFF (Motorola - big endian)" << std::endl;
    } else {
        std::cout << "Formato desconhecido" << std::endl;
    }

    unsigned short id_tiff;
    file.read((char*)&id_tiff, sizeof(id_tiff));
    std::cout << "identificador tiff - " << id_tiff << "\n";

    unsigned int first_idf;
    file.read((char*)&first_idf, sizeof(first_idf));
    std::cout << "first idf offset - " << first_idf << "\n";

    file.seekg(20840452, std::fstream::beg);

    unsigned short num_entries;
    file.read((char*)&num_entries, sizeof(num_entries));



    for (int i = 0; i < num_entries; i++){

        IFD_entry entry;
        file.read((char*)&entry, sizeof(IFD_entry));

        std::cout << "tag - " << entry.tag << "\n";
        std::cout << "type - " << entry.type << "\n";
        std::cout << "count - " << entry.count << "\n";
        std::cout << "valueOffstet - " << entry.valueOffset << "\n";

    }

    return 0;
}