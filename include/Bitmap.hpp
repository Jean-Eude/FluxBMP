#pragma once
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>


// Structures bitmap
#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };          // Type de fichier toujours BM, soit 0x4D42 (stocké en hexadécimal uint16_t en little endian)
    uint32_t file_size{ 0 };               // Taille du fichier (en octets)
    uint16_t reserved1{ 0 };               // Réservé, toujours 0
    uint16_t reserved2{ 0 };               // Réservé, toujours 0
    uint32_t offset_data{ 0 };             // Position de départ des données de pixels (en octets depuis le début du fichier)
};

struct BMPInfoHeader {
    uint32_t size{0};                      // Taille de cet en-tête (en octets)
    int32_t width{0};                      // Largeur du bitmap en pixels
    int32_t height{0};                     // Hauteur du bitmap en pixels
                                             //       (si positif, de bas en haut, avec origine en bas à gauche)
                                             //       (si négatif, de haut en bas, avec origine en haut à gauche)
    uint16_t planes{1};                    // Nombre de plans pour le dispositif cible, toujours 1
    uint16_t bit_count{0};                 // Nombre de bits par pixel
    uint32_t compression{0};               // 0 ou 3 - non compressé. CE PROGRAMME CONSIDÈRE UNIQUEMENT LES IMAGES BMP NON COMPRESSÉES
    uint32_t size_image{0};                // 0 - pour les images non compressées
    int32_t x_pixels_per_meter{0};         // Résolution horizontale (pixels par mètre)
    int32_t y_pixels_per_meter{0};         // Résolution verticale (pixels par mètre)
    uint32_t colors_used{0};               // Nombre d'index de couleur dans la table des couleurs. Utiliser 0 pour le nombre maximum de couleurs autorisé par bit_count
    uint32_t colors_important{0};          // Nombre de couleurs utilisées pour afficher le bitmap. Si 0, toutes les couleurs sont nécessaires
};

struct BMPColorHeader {
    uint32_t red_mask{0x00ff0000};         // Masque de bits pour le canal rouge
    uint32_t green_mask{0x0000ff00};       // Masque de bits pour le canal vert
    uint32_t blue_mask{0x000000ff};        // Masque de bits pour le canal bleu
    uint32_t alpha_mask{0xff000000};       // Masque de bits pour le canal alpha
    uint32_t color_space_type{0x73524742}; // "sRGB" par défaut (0x73524742)
    uint32_t unused[16]{0};                // Données inutilisées pour l'espace colorimétrique sRGB
};
#pragma pack(pop)


class Bitmap {
    public:

        Bitmap(int32_t width, int32_t height, bool has_alpha = true) {
            if (width <= 0 || height <= 0) {
                throw std::runtime_error("La hauteur et la longueur de l'image doivent être positifs.");
            }

            this->bmp_info_header.width = width;
            this->bmp_info_header.height = height;
            if (has_alpha) {
                this->bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
                this->file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);

                this->bmp_info_header.bit_count = 32;
                this->bmp_info_header.compression = 3;
                this->row_stride = width * 4;
                this->data.resize(row_stride * height);
                this->file_header.file_size = this->file_header.offset_data +this->data.size();
            }
            else {
                this->bmp_info_header.size = sizeof(BMPInfoHeader);
                this->file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

                this->bmp_info_header.bit_count = 24;
                this->bmp_info_header.compression = 0;
                this->row_stride = width * 3;
                this->data.resize(row_stride * height);

                uint32_t new_stride = make_stride_aligned(4);
                this->file_header.file_size = this->file_header.offset_data + static_cast<uint32_t>(data.size()) +this->bmp_info_header.height * (new_stride -this->row_stride);
            }
        }

        Bitmap(const char *fname) {
            this->fname = fname;
            read(fname);
        }

        // Méthode d'écriture
        void write(const char *fname) {
            this->fname = fname;

            std::ofstream of{ fname, std::ios_base::binary };
            if (of) {
                if (bmp_info_header.bit_count == 32) {
                    write_headers_and_data(of);
                }
                else if (bmp_info_header.bit_count == 24) {
                    if (bmp_info_header.width % 4 == 0) {
                        write_headers_and_data(of);
                    }
                    else {
                        uint32_t new_stride = make_stride_aligned(4);
                        std::vector<uint8_t> padding_row(new_stride -this->row_stride);

                        write_headers(of);

                        for (int y = 0; y <this->bmp_info_header.height; ++y) {
                            of.write((const char*)(data.data() +this->row_stride * y),this->row_stride);
                            of.write((const char*)padding_row.data(), padding_row.size());
                        }
                    }
                }
                else {
                    throw std::runtime_error("Le programme ne peut que traiter des fichiers BMP de 24 ou 32 bits seulement");
                }
            }
            else {
                throw std::runtime_error("Impossible d'ouvrir le fichier image de sortie.");
            }
        }

        // Méthode pour dessiner un pixel
        void set_pixel(double r, double g, double b, double a, int x, int y) {
            if (x >= bmp_info_header.width || y >= bmp_info_header.height || x < 0 || y < 0) {
                throw std::runtime_error("The point is outside the image boundaries!");
            }

            uint32_t channels = bmp_info_header.bit_count / 8;

            uint8_t ir = static_cast<uint8_t>(r * 255);
            uint8_t ig = static_cast<uint8_t>(g * 255);
            uint8_t ib = static_cast<uint8_t>(b * 255);
            uint8_t ia = static_cast<uint8_t>(a * 255);

            data[channels * (y * bmp_info_header.width + x) + 0] = ib;
            data[channels * (y * bmp_info_header.width + x) + 1] = ig;
            data[channels * (y * bmp_info_header.width + x) + 2] = ir;
            if (channels == 4) {
                data[channels * (y * bmp_info_header.width + x) + 3] = ia;
            }
        }

        // Méthode pour récupérer la valeur d'un pixel
        double getPixel(int x, int y, char channel, bool isDouble) {
            uint32_t channels =this->bmp_info_header.bit_count / 8;

            if (channel == 'R') {
                uint8_t red =this->data[channels * (y *this->bmp_info_header.width + x) + 2];
                if(isDouble == true) {
                    double pixel = static_cast<double>(red) / 256.;
                    return pixel;
                } else {
                    return red;
                }
            }
            if (channel == 'G') {
                uint8_t green =this->data[channels * (y *this->bmp_info_header.width + x) + 1];
                if(isDouble == true) {
                    double pixel = static_cast<double>(green) / 256.;
                    return pixel;
                } else {
                    return green;
                }
            }
            if (channel == 'B') {
                uint8_t blue =this->data[channels * (y *this->bmp_info_header.width + x) + 0];
                if(isDouble == true) {
                    double pixel = static_cast<double>(blue) / 256.;
                    return pixel;
                } else {
                    return blue;
                }
            }
            if (channel == 'A') {
                uint8_t alpha =this->data[channels * (y *this->bmp_info_header.width + x) + 3];
                if(isDouble == true) {
                    double pixel = static_cast<double>(alpha) / 256.;
                    return pixel;
                } else {
                    return alpha;
                }
            }

            return 1.0;
        }

        void normalize(double *integerValue) {
            *integerValue /= 256.;
        }

        uint32_t getWidth() {
            return this->bmp_info_header.width;
        }

        uint32_t getHeight() {
            return this->bmp_info_header.height;
        }

    private:
        BMPFileHeader file_header;
        BMPInfoHeader bmp_info_header;
        BMPColorHeader bmp_color_header;
        std::vector<uint8_t> data;  
        uint32_t row_stride{0};
        const char *fname;

        // Fonctions datas
        void write_headers(std::ofstream &of) {
            of.write((const char*)&this->file_header, sizeof(this->file_header));
            of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
            if(bmp_info_header.bit_count == 32) {
                of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
            }
        }

        void write_headers_and_data(std::ofstream &of) {
            write_headers(of);
            of.write((const char*)data.data(),this->data.size());
        }

        uint32_t make_stride_aligned(uint32_t align_stride) {
            uint32_t new_stride =this->row_stride;
            while (new_stride % align_stride != 0) {
                new_stride++;
            }
            return new_stride;
        }

        void check_color_header(BMPColorHeader &bmp_color_header) {
            BMPColorHeader expected_color_header;
            if(expected_color_header.red_mask !=this->bmp_color_header.red_mask ||
                expected_color_header.blue_mask !=this->bmp_color_header.blue_mask ||
                expected_color_header.green_mask !=this->bmp_color_header.green_mask ||
                expected_color_header.alpha_mask !=this->bmp_color_header.alpha_mask) {
                throw std::runtime_error("Format du masque de couleur incorrect! Le programme attends que les données du pixel doivent être au format BGRA");
            }
            if(expected_color_header.color_space_type !=this->bmp_color_header.color_space_type) {
                throw std::runtime_error("Espace couleur non reconnu! Le programme attends des velaurs sRGB");
            }
        }

        void read(const char *fname) {
            std::ifstream inp{ fname, std::ios_base::binary };
            if (inp) {
                inp.read((char*)&this->file_header, sizeof(this->file_header));
                if(this->file_header.file_type != 0x4D42) {
                    throw std::runtime_error("ERREUR : Format du fichier non reconnu ! ");
                }
                inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

                if(bmp_info_header.bit_count == 32) {
                    if(bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                        inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
                        check_color_header(bmp_color_header);
                    } else {
                        std::cerr << "ERREUR! Le fichier \"" << fname << "\" ne semble pas contenir de bit masqué\n";
                        throw std::runtime_error("ERREUR! Format du fichier non reconnu !");
                    }
                }

                inp.seekg(this->file_header.offset_data, inp.beg);

                if(bmp_info_header.bit_count == 32) {
                this->bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
                    this->file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
                } else {
                this->bmp_info_header.size = sizeof(BMPInfoHeader);
                    this->file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
                }

                this->file_header.file_size = this->file_header.offset_data;

                if (bmp_info_header.height < 0) {
                    throw std::runtime_error("Le programme ne peut que seulement traiter des images BMP avec pour origine le coin en bas à gauche !");
                }

                this->data.resize(bmp_info_header.width *this->bmp_info_header.height *this->bmp_info_header.bit_count / 8);

                if (bmp_info_header.width % 4 == 0) {
                    inp.read((char*)data.data(),this->data.size());
                    this->file_header.file_size += static_cast<uint32_t>(data.size());
                } else {
                    this->row_stride =this->bmp_info_header.width *this->bmp_info_header.bit_count / 8;
                    uint32_t new_stride = make_stride_aligned(4);
                    std::vector<uint8_t> padding_row(new_stride -this->row_stride);

                    for (int y = 0; y <this->bmp_info_header.height; ++y) {
                        inp.read((char*)(data.data() +this->row_stride * y),this->row_stride);
                        inp.read((char*)padding_row.data(), padding_row.size());
                    }
                    this->file_header.file_size += static_cast<uint32_t>(data.size()) +this->bmp_info_header.height * static_cast<uint32_t>(padding_row.size());
                }
            }
            else {
                throw std::runtime_error("Impossible d'ouvrir le fichier d'entrée.");
            }
        }     
};