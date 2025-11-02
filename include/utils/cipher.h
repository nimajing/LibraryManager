#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>
#include <string>
#include <vector>

const int AES_KEY_SIZE = 256; // AES-256

namespace Cipher {
namespace {
std::string getKey() {
  const char *key = std::getenv("KEY");
  if (key == nullptr) {
    throw std::runtime_error("Encryption key not found!");
  }
  return std::string(key);
}
} // namespace
// Encrypt data using AES-256-CBC
inline std::string encryptData(const std::string &data) {
  unsigned char iv[16];
  if (RAND_bytes(iv, sizeof(iv)) != 1) {
    throw std::runtime_error("Failed to generate IV");
  }

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    throw std::runtime_error("Failed to create cipher context");
  }

  // Set up AES-256-CBC encryption
  if (EVP_EncryptInit_ex(
          ctx, EVP_aes_256_cbc(), nullptr,
          reinterpret_cast<const unsigned char *>(getKey().c_str()),
          reinterpret_cast<const unsigned char *>(iv)) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Failed to initialize encryption");
  }

  std::vector<unsigned char> ciphertext(
      data.size() + AES_BLOCK_SIZE); // Output buffer for ciphertext
  int len = 0;

  // Perform encryption
  if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                        reinterpret_cast<const unsigned char *>(data.c_str()),
                        data.size()) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Encryption failed");
  }

  int final_len = 0;
  if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &final_len) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Final encryption step failed");
  }

  EVP_CIPHER_CTX_free(ctx);

  std::string encryptedData(reinterpret_cast<char *>(ciphertext.data()),
                            len + final_len);
  // Convert IV to hexadecimal string

  std::ostringstream ivStream;
  for (int i = 0; i < 16; i++) {
    ivStream << std::hex << std::setw(2) << std::setfill('0') << (int)iv[i];
  }
  std::string ivString = ivStream.str();

  // Convert encrypted data to a string (for demonstration purposes, using hex
  // as well)
  std::ostringstream encryptedStream;
  for (unsigned char byte : encryptedData) {
    encryptedStream << std::hex << std::setw(2) << std::setfill('0')
                    << (int)byte;
  }
  std::string encryptedString = encryptedStream.str();

  // Concatenate the IV and encrypted data
  return ivString + encryptedString;
}

/**
 * @brief Decrypts data using AES-256-CBC.
 * @param encryptedData The input string containing the IV (first 32 hex
 * characters) followed by the encrypted data in hexadecimal format.
 * @return The decrypted plaintext string.
 * @throws std::runtime_error if the IV or encrypted data cannot be parsed, or
 * if decryption fails.
 */
// Decrypt data using AES-256-CBC
inline std::string decryptData(const std::string &encryptedData) {
  // Extract the IV (first 16 bytes)
  std::string ivString =
      encryptedData.substr(0, 32); // 32 hex characters (16 bytes)
  std::vector<unsigned char> iv(16);
  try {
    if (ivString.length() != 32) {
      throw std::runtime_error("Invalid IV length");
    }
    for (int i = 0; i < 16; ++i) {
      iv[i] = static_cast<unsigned char>(
          std::stoi(ivString.substr(i * 2, 2), nullptr, 16));
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("Failed to parse IV: ") + e.what());
  }

  // Extract the encrypted data (after the first 16 bytes)
  std::string encryptedDataString =
      encryptedData.substr(32); // Remove the IV part
  std::vector<unsigned char> encryptedText(encryptedDataString.length() / 2);
  try {
    if (encryptedDataString.length() % 2 != 0) {
      throw std::runtime_error("Invalid encrypted data length");
    }
    for (size_t i = 0; i < encryptedText.size(); ++i) {
      encryptedText[i] = static_cast<unsigned char>(
          std::stoi(encryptedDataString.substr(i * 2, 2), nullptr, 16));
    }
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("Failed to parse encrypted data: ") +
                             e.what());
  }

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    throw std::runtime_error("Failed to create cipher context");
  }

  // Set up AES-256-CBC decryption
  if (EVP_DecryptInit_ex(
          ctx, EVP_aes_256_cbc(), nullptr,
          reinterpret_cast<const unsigned char *>(getKey().c_str()),
          iv.data()) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Failed to initialize decryption");
  }

  std::vector<unsigned char> decryptedData(encryptedText.size() +
                                           AES_BLOCK_SIZE);
  int len = 0;

  // Perform decryption
  if (EVP_DecryptUpdate(ctx, decryptedData.data(), &len, encryptedText.data(),
                        encryptedText.size()) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Decryption failed");
  }

  int final_len = 0;
  if (EVP_DecryptFinal_ex(ctx, decryptedData.data() + len, &final_len) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Final decryption step failed");
  }

  EVP_CIPHER_CTX_free(ctx);

  // Return the decrypted data as a string
  return std::string(decryptedData.begin(),
                     decryptedData.begin() + len + final_len);
}
}; // namespace Cipher
