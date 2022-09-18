// motions_decryptor.h - файл с алгоритмом дешифровки анимаций, находящихся в .cpp
#pragma once
#include <string>

#define ENCRYPTION_KEY "Stalker0kVasyan"

class random32
{
    long long m_seed;

public:
    random32() = delete;

    random32(const long long seed)
    {
        m_seed = seed;
    }

    long long random(const long long range)
    {
        m_seed = 0x07194657 * m_seed + 1;
        return (long long)(u64(m_seed) * u64(range) >> 32);
    }
};

std::string std_string_to_hex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (u8 c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

constexpr u32 alphabet_size = u32(1 << (8 * sizeof(u8)));
void decode(const long long decryption_key, const void* source, const u32& source_size, void* destination)
{
    u8 m_alphabet[alphabet_size];
    
    for (u32 i = 0; i < alphabet_size; ++i)
        m_alphabet[i] = (u8)i;

    random32 temp(decryption_key);

    const u8* I = (const u8*)source;
    const u8* E = (const u8*)source + source_size;
    u8* J = (u8*)destination;
    for (; I != E; ++I, ++J)
        *J = m_alphabet[(*I) ^ u8(temp.random(alphabet_size) & 0xff)];
}

void decrypt_coords(void* dest)
{
    long long key = 0;

    for (char c : ENCRYPTION_KEY)
    {
        std::string s = std::to_string(c);
        key += std::stoul(std_string_to_hex(s), nullptr, 16);
    }

    decode(key, dest, sizeof(float), dest);
}