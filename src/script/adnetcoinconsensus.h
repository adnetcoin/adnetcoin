// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_BITCOINCONSENSUS_H
#define BITCOIN_BITCOINCONSENSUS_H

#include <stdint.h>

#if defined(BUILD_BITCOIN_INTERNAL) && defined(HAVE_CONFIG_H)
#include "config/adnetcoin-config.h"
  #if defined(_WIN32)
    #if defined(DLL_EXPORT)
      #if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
        #define EXPORT_SYMBOL __declspec(dllexport)
      #else
        #define EXPORT_SYMBOL
      #endif
    #endif
  #elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBBITCOINCONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define BITCOINCONSENSUS_API_VER 1

typedef enum adnetcoinconsensus_error_t
{
    adnetcoinconsensus_ERR_OK = 0,
    adnetcoinconsensus_ERR_TX_INDEX,
    adnetcoinconsensus_ERR_TX_SIZE_MISMATCH,
    adnetcoinconsensus_ERR_TX_DESERIALIZE,
    adnetcoinconsensus_ERR_AMOUNT_REQUIRED,
    adnetcoinconsensus_ERR_INVALID_FLAGS,
} adnetcoinconsensus_error;

/** Script verification flags */
enum
{
    adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_WITNESS             = (1U << 11), // enable WITNESS (BIP141)
    adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH | adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                               adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY | adnetcoinconsensus_SCRIPT_FLAGS_VERIFY_WITNESS
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int adnetcoinconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                                 const unsigned char *txTo        , unsigned int txToLen,
                                                 unsigned int nIn, unsigned int flags, adnetcoinconsensus_error* err);

EXPORT_SYMBOL int adnetcoinconsensus_verify_script_with_amount(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, adnetcoinconsensus_error* err);

EXPORT_SYMBOL unsigned int adnetcoinconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // BITCOIN_BITCOINCONSENSUS_H
