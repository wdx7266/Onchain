// g++ main.cpp crypto/Sign.cpp crypto/AES.cpp `pkg-config --cflags --libs
// openssl` -o main &&
// ./main
#include <nlohmann/json.hpp>
#include "crypto/AES.h"
#include "crypto/Sign.h"
#include "io/BinaryReader.h"
#include "io/BinaryWriter.h"
#include <openssl/kdf.h>
#include <vector>

void sign_by_set_pub_pri()
{
  Sign ec_sign;
  ec_sign.EC_init();
  CurveName curve_name = p256;
  std::string private_key =
      "70789d4ac31576c61c5d12e38a66de605b18faf2c8d60a2c1952a6286b67318f";
  std::string public_key =
      "036e01e13fdc24f79387a54c807d0b161603509ddedcc361170f4b12df90d92255";
  ec_sign.EC_set_key(public_key, private_key, curve_name);
  cout << "private_key:\n"
       << private_key << endl
       << "public_key:\n"
       << public_key << endl;
  std::string out_public_key;
  std::string out_private_key;
  ec_sign.EC_get_private_key(out_private_key);
  ec_sign.EC_get_public_key(out_public_key);
  cout << "out_private_key:\n"
       << out_private_key << endl
       << "out_public_key:\n"
       << out_public_key << endl;

  bool ret;
  std::string msg = "Hello world!";
  std::string str_sign_dgst;
  ec_sign.EC_sign(msg, str_sign_dgst);
  cout << "msg:\n"
       << msg << endl
       << "str_sig_dgst:\n"
       << str_sign_dgst << endl;
  ret = ec_sign.EC_veri(msg, str_sign_dgst);
  cout << "ret: " << ret << endl;
}

void sign_by_gen_key()
{
  Sign ec_sign;
  ec_sign.EC_init();
  ec_sign.ECDSA_key_generate();
  std::string out_public_key;
  std::string out_private_key;
  ec_sign.EC_get_private_key(out_private_key);
  ec_sign.EC_get_public_key(out_public_key);
  cout << "out_private_key:\n"
       << out_private_key << endl
       << "out_public_key:\n"
       << out_public_key << endl;

  std::string msg = "Hello world!";
  std::string str_sign_dgst;
  ec_sign.EC_sign(msg, str_sign_dgst);
  cout << "msg:\n"
       << msg << endl
       << "str_sig_dgst:\n"
       << str_sign_dgst << endl;
  bool ret;
  ret = ec_sign.EC_veri(msg, str_sign_dgst);
  cout << "ret: " << ret << endl;
}

void sign_by_pri_key()
{
  Sign ec_sign;
  CurveName curve_name = p256;
  ec_sign.EC_init();
  std::string private_key =
      "70789d4ac31576c61c5d12e38a66de605b18faf2c8d60a2c1952a6286b67318f";
  std::string public_key = "";
  ec_sign.EC_get_pubkey_by_prikey(private_key, public_key, curve_name);
  ec_sign.EC_set_key(public_key, private_key, curve_name);
  cout << "private_key:\n"
       << private_key << endl
       << "public_key:\n"
       << public_key << endl;
  std::string out_public_key;
  std::string out_private_key;
  ec_sign.EC_get_private_key(out_private_key);
  ec_sign.EC_get_public_key(out_public_key);
  cout << "out_private_key:\n"
       << out_private_key << endl
       << "out_public_key:\n"
       << out_public_key << endl;

  bool ret;
  std::string msg = "Hello world!";
  std::string str_sign_dgst;
  ec_sign.EC_sign(msg, str_sign_dgst);
  cout << "msg:\n"
       << msg << endl
       << "str_sig_dgst:\n"
       << str_sign_dgst << endl;
  ret = ec_sign.EC_veri(msg, str_sign_dgst);
  cout << "ret: " << ret << endl;
}

std::string hexStr(unsigned char *data, int len)
{
  char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                   '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  std::string s(len * 2, ' ');
  for (int i = 0; i < len; ++i)
  {
    s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }
  return s;
}

// void aes_enc_dec() {
//   AES aes;
//   aes.params_init();
//   cout << "iv:\n" << aes.get_iv() << endl;
//   cout << "key:\n" << aes.get_key() << endl;
//   std::string msg = "Hello world!";
//   cout << "msg:\n" << msg << endl;
//   std::string enc_msg, dec_msg;
//   aes.auth_encry(msg, enc_msg);
//   cout << "enc_msg:\n" << enc_msg << endl;
//   aes.auth_decry(enc_msg, dec_msg);
//   cout << "dec_msg:\n" << dec_msg << endl;
// }

char *Base64Encode(const char *input, int length, bool with_new_line)
{
  BIO *bmem = NULL;
  BIO *b64 = NULL;
  BUF_MEM *bptr = NULL;

  b64 = BIO_new(BIO_f_base64());
  if (!with_new_line)
  {
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  }
  bmem = BIO_new(BIO_s_mem());
  b64 = BIO_push(b64, bmem);
  BIO_write(b64, input, length);
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bptr);

  char *buff = (char *)malloc(bptr->length + 1);
  memcpy(buff, bptr->data, bptr->length);
  buff[bptr->length] = 0;

  BIO_free_all(b64);

  return buff;
}

char *Base64Decode(char *input, int length, bool with_new_line)
{
  BIO *b64 = NULL;
  BIO *bmem = NULL;
  char *buffer = (char *)malloc(length);
  memset(buffer, 0, length);

  b64 = BIO_new(BIO_f_base64());
  if (!with_new_line)
  {
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  }
  bmem = BIO_new_mem_buf(input, length);
  bmem = BIO_push(b64, bmem);
  BIO_read(bmem, buffer, length);

  BIO_free_all(bmem);

  return buffer;
}

char *base64(const unsigned char *inputBuffer, int inputLen)
{
  EVP_ENCODE_CTX *ctx;
  ctx = EVP_ENCODE_CTX_new();
  int base64Len = (((inputLen + 2) / 3) * 4) + 1; // Base64 text length
  int pemLen = base64Len + base64Len / 64;        // PEM adds a newline every 64 bytes
  char *base64 = new char[pemLen];
  int result;
  EVP_EncodeInit(ctx);
  EVP_EncodeUpdate(ctx, (unsigned char *)base64, &result,
                   (unsigned char *)inputBuffer, inputLen);
  EVP_EncodeFinal(ctx, (unsigned char *)&base64[result], &result);
  return base64;
}

unsigned char *unbase64(char *input, int length, int *outLen)
{
  EVP_ENCODE_CTX *ctx;
  ctx = EVP_ENCODE_CTX_new();
  int orgLen = (((length + 2) / 4) * 3) + 1;
  unsigned char *orgBuf = new unsigned char[orgLen];
  int result, tmpLen;
  EVP_DecodeInit(ctx);
  EVP_DecodeUpdate(ctx, (unsigned char *)orgBuf, &result,
                   (unsigned char *)input, length);
  EVP_DecodeFinal(ctx, (unsigned char *)&orgBuf[result], &tmpLen);
  result += tmpLen;
  *outLen = result;
  return orgBuf;
}

std::string exportCtrEncryptedPrikey(std::string passphrase, int n)
{
  int N = n;
  int r = 8;
  int p = 8;
  int dkLen = 64;

  int salt_len = 4;
  unsigned char salt[] = {0xfa, 0xa4, 0x88, 0x3d};

  EVP_PKEY_CTX *pctx;
  unsigned char derivedkey[dkLen];

  size_t outlen = sizeof(derivedkey);
  pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_SCRYPT, NULL);

  if (EVP_PKEY_derive_init(pctx) <= 0)
  {
    return NULL;
  }
  if (EVP_PKEY_CTX_set1_pbe_pass(pctx, passphrase.c_str(),
                                 passphrase.length()) <= 0)
  {
    return NULL;
  }
  if (EVP_PKEY_CTX_set1_scrypt_salt(pctx, salt, salt_len) <= 0)
  {
    return NULL;
  }
  if (EVP_PKEY_CTX_set_scrypt_N(pctx, n) <= 0)
  {
    return NULL;
  }
  if (EVP_PKEY_CTX_set_scrypt_r(pctx, r) <= 0)
  {
    return NULL;
  }
  if (EVP_PKEY_CTX_set_scrypt_p(pctx, p) <= 0)
  {
    return NULL;
  }
  if (EVP_PKEY_derive(pctx, derivedkey, &outlen) <= 0)
  {
    return NULL;
  }

  std::string hex_derivedkey;
  hex_derivedkey = hexStr(derivedkey, outlen);
  cout << hex_derivedkey << endl
       << hex_derivedkey.length() << endl;
  if (hex_derivedkey.empty())
  {
    return NULL;
  }

  // std::string str_derivedkey =
  //     std::string(reinterpret_cast<char *>(derivedkey));
  // cout << "str_derivedkey:\n"
  //      << str_derivedkey << endl
  //      << "outlen:" << outlen << endl
  //      << str_derivedkey.length() << endl;

  unsigned char uc_iv[AES_BLOCK_SIZE];
  unsigned char uc_key[AES_256_KEY_SIZE];
  memcpy(uc_iv, &derivedkey[0], AES_BLOCK_SIZE * sizeof(unsigned char));
  memcpy(uc_key, &derivedkey[32], AES_256_KEY_SIZE * sizeof(unsigned char));

  cout << "uc_iv:\n"
       << hexStr(uc_iv, 16) << endl
       << sizeof(uc_iv) << endl;
  cout << "uc_key:\n"
       << hexStr(uc_key, 32) << endl
       << sizeof(uc_key) << endl;

  AES aes;

  aes.set_params(uc_key, uc_iv, AES_CTR);

  unsigned char uc_private_key[] = {
      0xc1, 0x9f, 0x16, 0x78, 0x5b, 0x8f, 0x35, 0x43, 0xbb, 0xaf, 0x5e,
      0x1d, 0xbb, 0x5d, 0x39, 0x8d, 0xfa, 0x6c, 0x85, 0xaa, 0xad, 0x54,
      0xfc, 0x9d, 0x71, 0x20, 0x3c, 0xe8, 0x3e, 0x50, 0x5c, 0x07};

  cout << "uc_private_key:\n"
       << hexStr(uc_private_key, 32) << endl;
  // cout << "uc_private_key:\n"
  //      << uc_private_key << endl
  //      << sizeof(uc_private_key) << endl;

  // std::string private_key =
  // std::string(reinterpret_cast<char *>(uc_private_key));
  // std::string enc_private_key;

  cout << "aes.get_iv():\n"
       << hexStr(aes.get_iv(), 16) << endl;
  cout << "aes.get_key():\n"
       << hexStr(aes.get_key(), 32) << endl;

  int enc_private_key_sz = 32;
  unsigned char *enc_private_key = new unsigned char(enc_private_key_sz);
  aes.auth_encry(uc_private_key, enc_private_key);
  cout << "enc_private_key:\n"
       << enc_private_key << endl;
  string str_enc_private_key;
  // str_enc_private_key = hexStr(enc_private_key, 48);
  // cout << "str_enc_private_key:\n" << str_enc_private_key << endl;
  // cout << Base64Encode(str_enc_private_key.c_str(),
  //                      str_enc_private_key.length(), false)
  //      << endl;
  cout << "base64(enc_private_key, 48):\n"
       << base64(enc_private_key, enc_private_key_sz) << endl;

  char test_key[] = "3JZLD/X45qSFjmRRvRVhcEjKgCJQDPWOsjx2dcTEj58=";
  int o_l;
  unsigned char *uc_tset_key = unbase64(test_key, 45, &o_l);

  cout << "test_key:\n"
       << unbase64(test_key, 45, &o_l) << endl;

  cout << hexStr(enc_private_key, 48) << endl;
  cout << hexStr(uc_tset_key, 48) << endl;

  EVP_PKEY_CTX_free(pctx);

  return "test";
}

void bn_write_read()
{
  // std::ofstream outfile;
  // outfile.open("outfile.txt");
  // if (!outfile.is_open()) {
  //   cout << "outfile.txt error!" << endl;
  // }
  BinaryWriter BW;
  // BW.set_ofstream(outfile);
  BW.writeVarInt(12345678);

  std::ifstream infile;
  // infile.open("infile.txt");
  // if (!infile.is_open()) {
  //   cout << "infile.txt error!" << endl;
  // }
  BinaryReader BR;
  // BR.set_ifstream(infile);

  long long value;
  value = BR.readVarInt(1234567890);
  cout << value << endl;
}

int main()
{
  // sign_by_pri_key();
  // sign_by_set_pub_pri();
  // sign_by_gen_key();
  // aes_enc_dec();

  // std::string passphrase = "passwordtest";
  // exportCtrEncryptedPrikey(passphrase, 16384);

  // bn_write_read();
  nlohmann::json array = {"hello", 1, 2.5, false, true, {1, 2}};

  return 0;
}