#ifndef KEYTYPE_HPP
#define KEYTYPE_HPP

#if __cplusplus < 201103L
    #error "use --std=c++11 option for compile."
#endif

#include "../common/ErrorCode.hpp"

#include <iostream>
#include <exception>
#include <vector>

enum class KeyType : int
{
    ECDSA = 0x12,
    SM2 = 0x13,
    EDDSA = 0x14
};

class KeyTypeMethod
{
  public:
    // get the crypto.KeyType according to the input label
    static KeyType fromLabel(int label)
    {
        KeyType type;
        switch (label)
        {
        case 0x12:
            type = KeyType::ECDSA;
            break;
        case 0x13:
            type = KeyType::SM2;
            break;
        case 0x14:
            type = KeyType::EDDSA;
            break;
        default:
            throw new std::invalid_argument(ErrorCode::StrUnknownAsymmetricKeyType);
        }
        return type;
    }

    // get the crypto.KeyType according to the input label
    static KeyType fromLabel(unsigned char label)
    {
        return fromLabel((int)label);
    }

    static KeyType fromPubkey(const std::vector<unsigned char> &pubkey)
    {
        KeyType type;
        if (pubkey.size() == 66)
        {
            type = KeyType::ECDSA;
        }
        else
        {
            try
            {
                type = fromLabel(pubkey[0]);
            }
            catch (std::invalid_argument &ia)
            {
                std::cerr << ia.what() << std::endl;
            }
        }
        return type;
    }

    static KeyType fromPubkey(const std::string &pubkey)
    {
        std::vector<unsigned char> vec_pubkey(pubkey.begin(), pubkey.end());
        return fromPubkey(vec_pubkey);
    }

    static int getLabel(KeyType type)
    {
        int label;
        switch (type)
        {
        case KeyType::ECDSA:
            label = 0x12;
            break;
        case KeyType::SM2:
            label = 0x13;
            break;
        case KeyType::EDDSA:
            label = 0x14;
            break;
        default:
            throw std::runtime_error("getLabel Unsupport type");
            break;
        }
        return label;
    }
};

#endif