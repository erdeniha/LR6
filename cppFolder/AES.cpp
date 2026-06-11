#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <random>
#include <cstring>
#include <limits>    
#include "AES.h"

void printHex(const std::string& label, const std::vector<uint8_t>& data) {
    std::cout << label << ": ";
    for (uint8_t byte : data) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
}

void printStateMatrix(const std::string& label, uint8_t state[16]) {
    std::cout << label << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(state[i + j * 4]) << " ";
        }
        std::cout << std::dec << std::endl;
    }
}

uint8_t galoisField(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int counter = 0; counter < 8; counter++) {
        if ((b & 1) != 0) p ^= a;
        bool hi_bit_set = (a & 0x80) != 0;
        a <<= 1;
        if (hi_bit_set) a ^= 0x11b; 
        b >>= 1;
    }
    return p;
}

void SubBytes(uint8_t state[16]) {
    for (int i = 0; i < 16; i++) state[i] = sbox[state[i]];
}

void ShiftRows(uint8_t state[16]) {
    uint8_t temp[16];
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            temp[row + col * 4] = state[row + ((col + row) % 4) * 4];
        }
    }
    std::memcpy(state, temp, 16);
}

void MixColumns(uint8_t state[16]) {
    uint8_t temp[16];
    
    const uint8_t mds[4][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };

    for (int c = 0; c < 4; ++c) {
        int col_offset = c * 4;    
        
        for (int r = 0; r < 4; ++r) { 
            uint8_t sum = 0;
            
            for (int k = 0; k < 4; ++k) { 
                sum ^= galoisField(mds[r][k], state[col_offset + k]);
            }
            
            temp[col_offset + r] = sum;
        }
    }
    
    std::memcpy(state, temp, 16);
}

void AddRoundKey(uint8_t state[16], const std::vector<uint8_t>& roundKey, int round) {
    for (int i = 0; i < 16; i++) state[i] ^= roundKey[round * 16 + i];
}

std::vector<uint8_t> KeyExpansion(const std::vector<uint8_t>& key) {
    std::vector<uint8_t> roundKeys(176); 
    for (int i = 0; i < 16; i++) roundKeys[i] = key[i];

    int bytesGenerated = 16;
    int rconIteration = 1;
    uint8_t temp[4];

    while (bytesGenerated < 176) {
        for (int i = 0; i < 4; i++) temp[i] = roundKeys[i + bytesGenerated - 4];

        if (bytesGenerated % 16 == 0) {
            uint8_t t = temp[0]; temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;
            for (int i = 0; i < 4; i++) temp[i] = sbox[temp[i]];
            temp[0] ^= Rcon[rconIteration++];
        }

        for (int i = 0; i < 4; i++) {
            roundKeys[bytesGenerated] = roundKeys[bytesGenerated - 16] ^ temp[i];
            bytesGenerated++;
        }
    }
    return roundKeys;
}

void CipherBlock(uint8_t state[16], const std::vector<uint8_t>& roundKeys, bool printDebug) {
    if (printDebug) std::cout << "\n[Детализация шифрования блока]" << std::endl;
    
    AddRoundKey(state, roundKeys, 0);
    if (printDebug) printStateMatrix("State после AddRoundKey (Round 0)", state);

    for (int round = 1; round < 10; round++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKeys, round);
        if (printDebug) printStateMatrix("State после Раунда " + std::to_string(round), state);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys, 10);
    if (printDebug) printStateMatrix("State Финальный (Round 10)", state);
}

std::vector<uint8_t> AddPadding(const std::string& input) {
    std::vector<uint8_t> padded(input.begin(), input.end());
    uint8_t padValue = 16 - (padded.size() % 16);
    for (int i = 0; i < padValue; i++) padded.push_back(padValue);
    return padded;
}

std::vector<uint8_t> RemovePadding(const std::vector<uint8_t>& input) {
    if (input.empty()) return input;
    uint8_t padValue = input.back();
    if (padValue > 0 && padValue <= 16) {
        return std::vector<uint8_t>(input.begin(), input.end() - padValue);
    }
    return input; 
}

std::vector<uint8_t> CryptOFB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, bool debugBlock ) {
    std::vector<uint8_t> output;
    std::vector<uint8_t> roundKeys = KeyExpansion(key);
    
    if (debugBlock) {
        std::cout << "\n[Сгенерированные раундовые ключи]" << std::endl;
        for (int i = 0; i <= 10; i++) {
            std::vector<uint8_t> rKey(roundKeys.begin() + i * 16, roundKeys.begin() + (i + 1) * 16);
            printHex("Round Key " + std::to_string(i), rKey);
        }
    }

    uint8_t feedback[16];
    std::memcpy(feedback, iv.data(), 16);

    for (std::size_t i = 0; i < input.size(); i += 16) {
        CipherBlock(feedback, roundKeys, (i == 0 && debugBlock)); 
        
        for (std::size_t j = 0; j < 16 && (i + j) < input.size(); j++) {
            output.push_back(input[i + j] ^ feedback[j]);
        }
    }
    return output;
}

std::vector<uint8_t> GenerateRandomBytes(int length) {
    std::vector<uint8_t> bytes(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);
    for (int i = 0; i < length; i++) bytes[i] = static_cast<uint8_t>(dis(gen));
    return bytes;
}


void SaveKeyToFile(const std::string& filename, const std::vector<uint8_t>& key) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(key.data()), key.size());
        file.close();
        std::cout << "Ключ успешно сохранен в файл: " << filename << std::endl;
    } else {
        std::cerr << "Ошибка записи в файл!" << std::endl;
    }
}

int AES() {

    std::vector<uint8_t> key = GenerateRandomBytes(16); 
    std::vector<uint8_t> iv = GenerateRandomBytes(16);  
    
    SaveKeyToFile("aes_key.bin", key);
    
    printHex("Мастер-Ключ (Master Key)", key);
    printHex("Вектор инициализации (IV)", iv);


    std::cin.ignore(256, '\n');

    std::string plaintext;
    std::cout << "Введите текст: ";
    std::getline(std::cin, plaintext); 

    std::vector<uint8_t> paddedInput = AddPadding(plaintext);

    std::vector<uint8_t> ciphertext = CryptOFB(paddedInput, key, iv, true); 
    
    printHex("Зашифрованные байты", ciphertext);

    std::vector<uint8_t> decryptedPadded = CryptOFB(ciphertext, key, iv, false);
    
    std::vector<uint8_t> decryptedData = RemovePadding(decryptedPadded);
    std::string decryptedText(decryptedData.begin(), decryptedData.end());
    std::cout << "Расшифрованный текст: " << decryptedText << std::endl;

    if (plaintext == decryptedText) {
        std::cout << "Результат сравнения: Тексты полностью идентичны! Тест пройден успешно." << std::endl;
    } else {
        std::cout << "Результат сравнения: Ошибка! Тексты различаются." << std::endl;
    }

    return 0;
}