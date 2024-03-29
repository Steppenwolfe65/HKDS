/* The AGPL version 3 License (AGPLv3)
*
* Copyright (c) 2021 Digital Freedom Defence Inc.
* This file is part of the QSC Cryptographic library
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QSC_SHA3_H
#define QSC_SHA3_H

#include "common.h"
#if defined(QSC_SYSTEM_AVX_INTRINSICS)
#	include "intrinsics.h"
#endif

/**
* \file sha3.h
* \author John Underhill
* \date October 27, 2019
* \updated October 19, 2021
*
* \brief SHA3 header definition \n
* Contains the public api and documentation for SHA3 digest, SHAKE, cSHAKE, and KMAC implementations.
*
* Usage Examples \n
*
* SHA3-512 hash computation using long-form api \n
* \code
* #define MSGLEN 200
* uint8_t msg[MSGLEN] = {...};
* uint8_t hash[QSC_SHA3_512_HASH_SIZE] = { 0 };
* qsc_keccak_state ctx;
*
* qsc_sha3_initialize(ctx.state);
*
* qsc_sha3_update(&ctx, qsc_keccak_rate_512, msg, MSGLEN);
*
* qsc_sha3_finalize(&ctx, qsc_keccak_rate_512, hash);
*
* \endcode
*
* KMAC-256 MAC code generation using long-form api \n
* \code
* #define MSGLEN 200
* uint8_t msg[MSGLEN] = {...};
* uint8_t key[QSC_KMAC_256_KEY_SIZE] = {...};
* uint8_t cust[...] = {...};
* qsc_keccak_state ctx;
* uint8_t code[QSC_KMAC_256_MAC_SIZE] = { 0 };
*
* qsc_kmac_initialize(&ctx, qsc_keccak_rate_256, key, sizeof(key), cust, sizeof(cust));
* 	qsc_kmac_update(&ctx, qsc_keccak_rate_256, msg, MSGLEN);
* qsc_kmac_finalize(&ctx, qsc_keccak_rate_256, code, sizeof(code));
*
* \endcode
*
* cSHAKE-512 pseudo-random generation using long-form api \n
* \code
*
* uint8_t output[64] = { 0 };
* uint8_t key[QSC_KMAC_512_KEY_SIZE] = {...};
* uint8_t cust[...] = {...};
* uint8_t name[...] = {...};
* qsc_keccak_state ctx;
*
* qsc_cshake_initialize(&ctx, key, sizeof(key), name, sizeof(name), cust, sizeof(cust));
* qsc_cshake_squeezeblocks(&ctx, output, 1);
* \endcode
*
* \remarks
* \par
* The SHA3, SHAKE, cSHAKE, and KMAC implementations all share two forms of api: short-form and long-form. \n
* The short-form api, which initializes the state, processes a message, and finalizes by producing output, all in a single function call,
* for example; qsc_sha3_compute512(uint8_t* output, const uint8_t* message, size_t msglen),
* the entire message array is processed and the hash code is written to the output array. \n
* The long-form api uses an initialization call to prepare the state, a blockupdate call if the message is longer than a single message block,
* and the finalize call, which finalizes the state and generates a hash, mac-code, or an array of pseudo-random. \n
* Each of the function families (SHA3, SHAKE, KMAC), have a corresponding set of reference constants associated with that member, example;
* SHAKE_256_KEY is the minimum expected SHAKE-256 key size in bytes, QSC_KMAC_512_MAC_SIZE is the minimum size of the KMAC-512 output mac-code output array,
* and QSC_KECCAK_512_RATE is the SHA3-512 message absorption rate.
*
* For additional usage examples, see sha3_test.h. \n
*
* \par
* NIST: SHA3 Fips202 http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf \n
* NIST: SP800-185 http://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-185.pdf \n
* NIST: SHA3 Keccak Submission http://keccak.noekeon.org/Keccak-submission-3.pdf \n
* NIST: SHA3 Keccak Slides http://csrc.nist.gov/groups/ST/hash/sha-3/documents/Keccak-slides-at-NIST.pdf \n
* NIST: SHA3 Third-Round Report http://nvlpubs.nist.gov/nistpubs/ir/2012/NIST.IR.7896.pdf \n
* Team Keccak: Specifications summary https://keccak.team/keccak_specs_summary.html
*/

/*!
* \def QSC_KECCAK_CSHAKE_DOMAIN_ID
* \brief The cSHAKE domain id
*/
#define QSC_KECCAK_CSHAKE_DOMAIN_ID 0x04

/*!
* \def QSC_KECCAK_KMAC_DOMAIN_ID
* \brief The KMAC domain id
*/
#define QSC_KECCAK_KMAC_DOMAIN_ID 0x04

/*!
* \def QSC_KECCAK_KPA_DOMAIN_ID
* \brief The KPA domain id
*/
#define QSC_KECCAK_KPA_DOMAIN_ID 0x41

/*!
* \def QSC_KECCAK_PERMUTATION_ROUNDS
* \brief The standard number of permutation rounds
*/
#define QSC_KECCAK_PERMUTATION_ROUNDS 24

/*!
* \def QSC_KECCAK_PERMUTATION_MAX_ROUNDS
* \brief The maximum number of permutation rounds
*/
#define QSC_KECCAK_PERMUTATION_MAX_ROUNDS 48

/*!
* \def QSC_KECCAK_PERMUTATION_MIN_ROUNDS
* \brief The minimum number of permutation rounds
*/
#define QSC_KECCAK_PERMUTATION_MIN_ROUNDS 12

/*!
* \def QSC_KECCAK_SHA3_DOMAIN_ID
* \brief The SHA3 domain id
*/
#define QSC_KECCAK_SHA3_DOMAIN_ID 0x06

/*!
* \def QSC_KECCAK_SHAKE_DOMAIN_ID
* \brief The SHAKE domain id
*/
#define QSC_KECCAK_SHAKE_DOMAIN_ID 0x1F

/*!
* \def QSC_KECCAK_STATE_BYTE_SIZE
* \brief The Keccak state array byte size
*/
#define QSC_KECCAK_STATE_BYTE_SIZE 200

/*!
* \def QSC_KECCAK_128_RATE
* \brief The KMAC-128 byte absorption rate
*/
#define QSC_KECCAK_128_RATE 168

/*!
* \def QSC_KECCAK_256_RATE
* \brief The KMAC-256 byte absorption rate
*/
#define QSC_KECCAK_256_RATE 136

/*!
* \def QSC_KECCAK_512_RATE
* \brief The KMAC-512 byte absorption rate
*/
#define QSC_KECCAK_512_RATE 72

/*!
* \def QSC_KECCAK_STATE_SIZE
* \brief The Keccak SHA3 uint64 state array size
*/
#define QSC_KECCAK_STATE_SIZE 25

/*!
* \def QSC_KECCAK_STATE_BYTE_SIZE
* \brief The Keccak SHA3 state size in bytes
*/
#define QSC_KECCAK_STATE_BYTE_SIZE 200

/*!
* \def QSC_KMAC_256_KEY_SIZE
* \brief The KMAC-256 key size in bytes
*/
#define QSC_KMAC_256_KEY_SIZE 32

/*!
* \def QSC_KMAC_512_KEY_SIZE
* \brief The KMAC-512 key size in bytes
*/
#define QSC_KMAC_512_KEY_SIZE 64

/*!
* \def QSC_SHA3_128_HASH_SIZE
* \brief The QSC_SHA3_128_HASH_SIZE hash size in bytes (16)
*/
#define QSC_SHA3_128_HASH_SIZE 16

/*!
* \def QSC_SHA3_256_HASH_SIZE
* \brief The SHA-256 hash size in bytes (32)
*/
#define QSC_SHA3_256_HASH_SIZE 32

/*!
* \def QSC_SHA3_512_HASH_SIZE
* \brief The SHA-512 hash size in bytes (64)
*/
#define QSC_SHA3_512_HASH_SIZE 64

/*!
* \def QSC_SHAKE_256_KEY_SIZE
* \brief The SHAKE-256 key size in bytes
*/
#define QSC_SHAKE_256_KEY_SIZE 32

/*!
* \def QSC_SHAKE512_KEY_SIZE
* \brief The SHAKE-512 key size in bytes
*/
#define QSC_SHAKE512_KEY_SIZE 64

/* common */

/*!
* \struct qsc_keccak_state
* \brief The Keccak state array; state array must be initialized by the caller
*/
QSC_EXPORT_API typedef struct
{
	uint64_t state[QSC_KECCAK_STATE_SIZE];			/*!< The SHA3 state  */
	uint8_t buffer[QSC_KECCAK_STATE_BYTE_SIZE];		/*!< The message buffer  */
	size_t position;								/*!< The buffer position  */
} qsc_keccak_state;

/*!
* \enum qsc_keccak_rate
* \brief The Keccak rate; determines which security strength is used by the function, 128, 256, or 512-bit
*/
typedef enum
{
	qsc_keccak_rate_128 = QSC_KECCAK_128_RATE,		/*!< The Keccak 128-bit rate  */
	qsc_keccak_rate_256 = QSC_KECCAK_256_RATE,		/*!< The Keccak 256-bit rate  */
	qsc_keccak_rate_512 = QSC_KECCAK_512_RATE,		/*!< The Keccak 512-bit rate  */
} qsc_keccak_rate;

/**
* \brief Absorb an input message into the Keccak state
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param message: [const] The input message byte array
* \param msglen: The number of message bytes to process
* \param domain: The function domain id
* \param rounds: The number of permutation rounds, the default is 24, maximum is 48
*/
QSC_EXPORT_API void qsc_keccak_absorb(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* message, size_t msglen, uint8_t domain, size_t rounds);

/**
* \brief Absorb the custom, and name arrays into the Keccak state
*
* \param ctx: [struct] The Keccak state structure
* \param rate: The rate of absorption in bytes
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
* \param name: [const] The function name string
* \param namelen: The byte length of the function name
* \param rounds: The number of permutation rounds, the default is 24, maximum is 48
*/
QSC_EXPORT_API void qsc_keccak_absorb_custom(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* custom, size_t custlen, const uint8_t* name, size_t namelen, size_t rounds);

/**
* \brief Absorb the custom, name, and key arrays into the Keccak state.
*
* \param ctx: [struct] The Keccak state structure
* \param rate: The rate of absorption in bytes
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
* \param name: [const] The function name string
* \param namelen: The byte length of the function name
* \param rounds: The number of permutation rounds, the default is 24, maximum is 48
*/
QSC_EXPORT_API void qsc_keccak_absorb_key_custom(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* key, size_t keylen, const uint8_t* custom, size_t custlen, const uint8_t* name, size_t namelen, size_t rounds);

/**
* \brief Dispose of the Keccak state.
*
* \warning The dispose function must be called when disposing of the function state.
* This function safely destroys the internal state.
*
* \param ctx: [struct] The Keccak state structure
*/
QSC_EXPORT_API void qsc_keccak_dispose(qsc_keccak_state* ctx);

/**
* \brief Finalize the Keccak state
*
* \param ctx: [struct] The Keccak state structure
* \param rate: The rate of absorption in bytes
* \param output: The output byte array
* \param outlen: The number of output bytes to generate
* \param domain: The function domain id
* \param rounds: The number of permutation rounds, the default is 24, maximum is 48
*/
QSC_EXPORT_API void qsc_keccak_finalize(qsc_keccak_state* ctx, qsc_keccak_rate rate, uint8_t* output, size_t outlen, uint8_t domain, size_t rounds);

/**
* \brief Absorb bytes into state incrementally
*
* \param ctx: The function state
* \param rate: The rate of absorption in bytes
* \param message: [const] The input message array
* \param msglen: The number of message bytes
*/
QSC_EXPORT_API void qsc_keccak_incremental_absorb(qsc_keccak_state* ctx, uint32_t rate, const uint8_t* message, size_t msglen);

/**
* \brief Finalize state added incrementally
*
* \param ctx: The function state
* \param rate: The rate of absorption in bytes
* \param domain: The function domain id
*/
QSC_EXPORT_API void qsc_keccak_incremental_finalize(qsc_keccak_state* ctx, uint32_t rate, uint8_t domain);

/**
* \brief Extract an array of bytes from the Keccak state
*
* \param ctx: The function state
* \param output: The output byte array
* \param outlen: The number of output bytes to generate
* \param rate: The rate of absorption in bytes
*/
QSC_EXPORT_API void qsc_keccak_incremental_squeeze(qsc_keccak_state* ctx, size_t rate, uint8_t* output, size_t outlen);

/**
* \brief The Keccak permute function.
* Internal function: Permutes the state array, can be used in external constructions.
*
* \param ctx: [struct] The function state; must be initialized
* \param rounds: The number of permutation rounds, the default and maximum is 24
*/
QSC_EXPORT_API void qsc_keccak_permute(qsc_keccak_state* ctx, size_t rounds);

/**
* \brief The compact Keccak permute function.
* Internal function: Permutes the state array, can be used in external constructions.
*
* \param state: The state array; must be initialized
* \param rounds: The number of permutation rounds, the default and maximum is 24
*/
QSC_EXPORT_API void qsc_keccak_permute_p1600c(uint64_t* state, size_t rounds);

/**
* \brief The unrolled Keccak permute function.
* Internal function: Permutes the state array, can be used in external constructions.
*
* \param state: The state array; must be initialized
*/
QSC_EXPORT_API void qsc_keccak_permute_p1600u(uint64_t* state);

/**
* \brief The Keccak squeeze function.
*
* \warning Output array must be initialized to a multiple of the byte rate.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param output: The output byte array
* \param nblocks: The number of blocks to extract
* \param rate: The rate of absorption in bytes
* \param rounds: The number of permutation rounds, the default and maximum is 24
*/
QSC_EXPORT_API void qsc_keccak_squeezeblocks(qsc_keccak_state* ctx, uint8_t* output, size_t nblocks, qsc_keccak_rate rate, size_t rounds);

/**
* \brief Initializes a Keccak state structure, must be called before message processing.
* Long form api: must be used in conjunction with the block-update and finalize functions.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
*/
QSC_EXPORT_API void qsc_keccak_initialize_state(qsc_keccak_state* ctx);

/**
* \brief Update Keccak state with message input.
*
* \warning The state must be initialized before calling
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param message: [const] The input message byte array
* \param msglen: The number of message bytes to process
* \param rounds: The number of permutation rounds, the default and maximum is 24
*/
QSC_EXPORT_API void qsc_keccak_update(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* message, size_t msglen, size_t rounds);

/* SHA3 */

/**
* \brief Process a message with SHA3-128 and return the hash code in the output byte array.
* Short form api: processes the entire message and computes the hash code with a single call.
*
* \warning The output array must be at least 16 bytes in length.
*
* \param output:: The output byte array; receives the hash code
* \param message: [const] The message input byte array
* \param msglen: The number of message bytes to process
*/
QSC_EXPORT_API void qsc_sha3_compute128(uint8_t* output, const uint8_t* message, size_t msglen);

/**
* \brief Process a message with SHA3-256 and return the hash code in the output byte array.
* Short form api: processes the entire message and computes the hash code with a single call.
*
* \warning The output array must be at least 32 bytes in length.
*
* \param output:: The output byte array; receives the hash code
* \param message: [const] The message input byte array
* \param msglen: The number of message bytes to process
*/
QSC_EXPORT_API void qsc_sha3_compute256(uint8_t* output, const uint8_t* message, size_t msglen);

/**
* \brief Process a message with SHA3-512 and return the hash code in the output byte array.
* Short form api: processes the entire message and computes the hash code with a single call.
*
* \warning The output array must be at least 64 bytes in length.
*
* \param output: The output byte array; receives the hash code
* \param message: [const] The message input byte array
* \param msglen: The number of message bytes to process
*/
QSC_EXPORT_API void qsc_sha3_compute512(uint8_t* output, const uint8_t* message, size_t msglen);

/**
* \brief Finalize the message state and returns the hash value in output.
* Long form api: must be used in conjunction with the initialize and block-update functions.
* Absorb the last block of message and create the hash value.
* Produces a 32 byte output code using QSC_KECCAK_256_RATE, 64 bytes with QSC_KECCAK_512_RATE.
*
* \warning The output array must be sized correctly corresponding to the absorption rate ((200 - rate) / 2).
* Finalizes the message state, can not be used in consecutive calls.
* The state must be initialized before calling.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param output: The output byte array; receives the hash code
*/
QSC_EXPORT_API void qsc_sha3_finalize(qsc_keccak_state* ctx, qsc_keccak_rate rate, uint8_t* output);

/**
* \brief Initialize the SHA3 state
* Long form api: Must be called before the update or finalize functions are called.
*
* \param ctx: [struct] A reference to the Keccak state
*/
QSC_EXPORT_API void qsc_sha3_initialize(qsc_keccak_state* ctx);

/**
* \brief Update SHA3 with message input.
* Long form api: must be used in conjunction with the initialize and finalize functions.
* Absorbs the input message into the state.
*
* \warning The state must be initialized before calling
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param message: [const] The input message byte array
* \param msglen: The number of message bytes to process
*/
QSC_EXPORT_API void qsc_sha3_update(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* message, size_t msglen);

/* SHAKE */

/**
* \brief Key a SHAKE-128 instance, and generate an array of pseudo-random bytes.
* Short form api: processes the key and generates the pseudo-random output with a single call.
*
* \warning The output array length must not be zero.
*
* \param output: The output byte array
* \param outlen: The number of output bytes to generate
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
*/
QSC_EXPORT_API void qsc_shake128_compute(uint8_t* output, size_t outlen, const uint8_t* key, size_t keylen);

/**
* \brief Key a SHAKE-256 instance, and generate an array of pseudo-random bytes.
* Short form api: processes the key and generates the pseudo-random output with a single call.
*
* \warning The output array length must not be zero.
*
* \param output: The output byte array
* \param outlen: The number of output bytes to generate
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
*/
QSC_EXPORT_API void qsc_shake256_compute(uint8_t* output, size_t outlen, const uint8_t* key, size_t keylen);

/**
* \brief Key a SHAKE-512 instance, and generate an array of pseudo-random bytes.
* Short form api: processes the key and generates the pseudo-random output with a single call.
*
* \warning The output array length must not be zero.
*
* \param output: The output byte array
* \param outlen: The number of output bytes to generate
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
*/
QSC_EXPORT_API void qsc_shake512_compute(uint8_t* output, size_t outlen, const uint8_t* key, size_t keylen);

/**
* \brief The SHAKE initialize function.
* Long form api: must be used in conjunction with the squeezeblocks function.
* Absorb and finalize an input key byte array.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
*/
QSC_EXPORT_API void qsc_shake_initialize(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* key, size_t keylen);

/**
* \brief The SHAKE squeeze function.
* Long form api: must be used in conjunction with the initialize function.
* Permutes and extracts the state to an output byte array.
*
* \warning Output array must be initialized to a multiple of the byte rate.
* The state must be initialized before calling.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param output: The output byte array
* \param nblocks: The number of blocks to extract
*/
QSC_EXPORT_API void qsc_shake_squeezeblocks(qsc_keccak_state* ctx, qsc_keccak_rate rate, uint8_t* output, size_t nblocks);

/* cSHAKE */

/**
* \brief Key a cSHAKE-128 instance and generate pseudo-random output.
* Short form api: processes the key, name, and custom inputs and generates the pseudo-random output with a single call.
* Permutes and extracts the state to an output byte array..
*
* \param output: The output byte array
* \param outlen: The number of output bytes to generate
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param name: [const] The function name string
* \param namelen: The byte length of the function name
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_cshake128_compute(uint8_t* output, size_t outlen, const uint8_t* key, size_t keylen, const uint8_t* name, size_t namelen, const uint8_t* custom, size_t custlen);

/**
* \brief Key a cSHAKE-256 instance and generate pseudo-random output.
* Short form api: processes the key, name, and custom inputs and generates the pseudo-random output with a single call.
* Permutes and extracts the state to an output byte array.
*
* \param output: The output byte array
* \param outlen: The number of output bytes to generate
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param name: [const] The function name string
* \param namelen: The byte length of the function name
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_cshake256_compute(uint8_t* output, size_t outlen, const uint8_t* key, size_t keylen, const uint8_t* name, size_t namelen, const uint8_t* custom, size_t custlen);

/**
* \brief Key a cSHAKE-512 instance and generate pseudo-random output.
* Short form api: processes the key, name, and custom inputs and generates the pseudo-random output with a single call.
* Permutes and extracts the state to an output byte array.
*
* \param output: The output byte array
* \param outlen: The number of output bytes to generate
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param name: [const] The function name string
* \param namelen: The byte length of the function name
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_cshake512_compute(uint8_t* output, size_t outlen, const uint8_t* key, size_t keylen, const uint8_t* name, size_t namelen, const uint8_t* custom, size_t custlen);

/**
* \brief The cSHAKE initialize function.
* Long form api: must be used in conjunction with the squeezeblocks function.
* Initialize the name and customization strings into the state.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param name: [const] The function name string
* \param namelen: The byte length of the function name
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_cshake_initialize(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* key, size_t keylen, const uint8_t* name, size_t namelen, const uint8_t* custom, size_t custlen);

/**
* \brief The cSHAKE squeeze function.
* Long form api: must be used in conjunction with the initialize function.
* Permutes and extracts blocks of state to an output byte array.
*
* \warning Output array must be initialized to a multiple of the byte rate.
* The state must be initialized before calling.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param output: The output byte array
* \param nblocks: The number of blocks to extract
*/
QSC_EXPORT_API void qsc_cshake_squeezeblocks(qsc_keccak_state* ctx, qsc_keccak_rate rate, uint8_t* output, size_t nblocks);

/**
* \brief The cSHAKE update function.
* Long form api: must be used in conjunction with the initialize and squeezeblocks functions.
* Finalize an input key directly into the state.
*
* \warning Finalizes the key state, should not be used in consecutive calls.
* The state must be initialized before calling.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
*/
QSC_EXPORT_API void qsc_cshake_update(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* key, size_t keylen);

/* KMAC */

/**
* \brief Key a KMAC-128 instance and generate a MAC code.
* Short form api: processes the key and custom inputs and generates the MAC code with a single call.
* Key the MAC generator process a message and output the MAC code.
*
* \param output: The MAC code byte array
* \param outlen: The number of MAC code bytes to generate
* \param message: [const] The message input byte array
* \param msglen: The number of message bytes to process
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_kmac128_compute(uint8_t* output, size_t outlen, const uint8_t* message, size_t msglen, const uint8_t* key, size_t keylen, const uint8_t* custom, size_t custlen);

/**
* \brief Key a KMAC-256 instance and generate a MAC code.
* Short form api: processes the key and custom inputs and generates the MAC code with a single call.
* Key the MAC generator process a message and output the MAC code.
*
* \param output: The MAC code byte array
* \param outlen: The number of MAC code bytes to generate
* \param message: [const] The message input byte array
* \param msglen: The number of message bytes to process
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_kmac256_compute(uint8_t* output, size_t outlen, const uint8_t* message, size_t msglen, const uint8_t* key, size_t keylen, const uint8_t* custom, size_t custlen);

/**
* \brief Key a KMAC-512 instance and generate a MAC code.
* Short form api: processes the key and custom inputs and generates the MAC code with a single call.
* Key the MAC generator process a message and output the MAC code.
*
* \param output: The MAC code byte array
* \param outlen: The number of MAC code bytes to generate
* \param message: [const] The message input byte array
* \param msglen: The number of message bytes to process
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_kmac512_compute(uint8_t* output, size_t outlen, const uint8_t* message, size_t msglen, const uint8_t* key, size_t keylen, const uint8_t* custom, size_t custlen);

/**
* \brief The KMAC message update function.
* Long form api: must be used in conjunction with the initialize and finalize functions.
*
* \warning The state must be initialized before calling.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param message: [const] The message input byte array
* \param msglen: The number of message bytes to process
*/
QSC_EXPORT_API void qsc_kmac_update(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* message, size_t msglen);

/**
* \brief The KMAC finalize function.
* Long form api: must be used in conjunction with the initialize and blockupdate functions.
* Final processing and calculation of the MAC code.
*
* \warning The state must be initialized before calling.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param output: The output byte array
* \param outlen: The number of bytes to extract
*/
QSC_EXPORT_API void qsc_kmac_finalize(qsc_keccak_state* ctx, qsc_keccak_rate rate, uint8_t* output, size_t outlen);

/**
* \brief Initialize a KMAC instance.
* Long form api: must be used in conjunction with the blockupdate and finalize functions.
* Key the MAC generator and initialize the internal state.
*
* \param ctx: [struct] A reference to the keccak state; must be initialized
* \param rate: The rate of absorption in bytes
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_kmac_initialize(qsc_keccak_state* ctx, qsc_keccak_rate rate, const uint8_t* key, size_t keylen, const uint8_t* custom, size_t custlen);

/* KPA - Keccak-based Parallel Authentication */

#if defined(QSC_SYSTEM_HAS_AVX512) || defined(QSC_SYSTEM_HAS_AVX2)
#define QSC_KPA_AVX_PARALLEL
#endif

/*!
* \def QSC_KPA_128_KEY_SIZE
* \brief The KPA-128 key size in bytes
*/
#define QSC_KPA_128_KEY_SIZE 16

/*!
* \def QSC_KPA_256_KEY_SIZE
* \brief The KPA-256 key size in bytes
*/
#define QSC_KPA_256_KEY_SIZE 32

/*!
* \def QSC_KPA_512_KEY_SIZE
* \brief The KPA-512 key size in bytes
*/
#define QSC_KPA_512_KEY_SIZE 64

/*!
* \def QSC_KPA_ROUNDS
* \brief The number of Keccak rounds used by a KPA permutation
*/
#define QSC_KPA_ROUNDS 12

/*!
* \def QSC_KPA_PARALLELISM
* \brief The KPA degree of parallelization
*/
#define QSC_KPA_PARALLELISM 8

/*!
* \struct qsc_kpa_state
* \brief The KPA state array; state array must be initialized by the caller
*/
QSC_EXPORT_API typedef struct
{
#if defined(QSC_SYSTEM_HAS_AVX512)
	__m512i statew[QSC_KECCAK_STATE_SIZE];								/*!< The AVX512 state array  */
#elif defined(QSC_SYSTEM_HAS_AVX2)
	__m256i statew[2][QSC_KECCAK_STATE_SIZE];							/*!< The AVX2 state array  */
#endif

	uint64_t state[QSC_KPA_PARALLELISM][QSC_KECCAK_STATE_SIZE];			/*!< The long state array  */
	uint8_t buffer[QSC_KPA_PARALLELISM * QSC_KECCAK_STATE_BYTE_SIZE];	/*!< The message buffer  */
	size_t position;													/*!< The buffer position  */
	size_t processed;													/*!< The number of message bytes processed  */
	qsc_keccak_rate rate;												/*!< The absorption rate  */
} qsc_kpa_state;

/**
* \brief The KPA finalize function.
* Long form api: must be used in conjunction with the initialize and blockupdate functions.
* Final processing and calculation of the MAC code.
*
* \warning The state must be initialized before calling.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param output: The output byte array
* \param outlen: The number of bytes to extract
*/
QSC_EXPORT_API void qsc_kpa_finalize(qsc_kpa_state* ctx, uint8_t* output, size_t outlen);

/**
* \brief Initialize a KPA instance.
* Long form api: must be used in conjunction with the blockupdate and finalize functions.
* Key the MAC generator and initialize the internal state.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param key: [const] The input key byte array
* \param keylen: The number of key bytes to process
* \param custom: [const] The customization string
* \param custlen: The byte length of the customization string
*/
QSC_EXPORT_API void qsc_kpa_initialize(qsc_kpa_state* ctx, const uint8_t* key, size_t keylen, const uint8_t* custom, size_t custlen);

/**
* \brief The KPA message update function.
* Long form api: must be used in conjunction with the initialize and finalize functions.
*
* \warning The state must be initialized before calling.
*
* \param ctx: [struct] A reference to the Keccak state; must be initialized
* \param message: [const] The message input byte array
* \param msglen: The number of message bytes to process
*/
QSC_EXPORT_API void qsc_kpa_update(qsc_kpa_state* ctx, const uint8_t* message, size_t msglen);

/**
* \brief Dispose of the KPA state.
*
* \warning The dispose function must be called when disposing of the function state.
* This function safely destroys the internal state.
*
* \param ctx: [struct] The Keccak state structure
*/
QSC_EXPORT_API void qsc_kpa_dispose(qsc_kpa_state* ctx);

/* parallel Keccak x4 */

#if defined(QSC_SYSTEM_HAS_AVX2)

/**
* \brief Absorb 4 Keccak instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays muct be of the same length.
* This function requires the AVX2 instruction set.
*
* \param state: The Keccak state array
* \param rate: The shake rate
* \param inp0: [const] The 1st input key array
* \param inp1: [const] The 2nd input key array
* \param inp2: [const] The 3rd input key array
* \param inp3: [const] The 4th input key array
* \param inplen: The length of the input key arrays
* \param domain
*/
void qsc_keccakx4_absorb(__m256i state[QSC_KECCAK_STATE_SIZE], qsc_keccak_rate rate,
	const uint8_t* inp0, const uint8_t* inp1, const uint8_t* inp2, const uint8_t* inp3, size_t inplen, uint8_t domain);

/**
* \brief Squeeze 4 Keccak instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param state: The Keccak state array
* \param rate: The Keccak rate
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param nblocks: The number of output blocks
*/
void qsc_keccakx4_squeezeblocks(__m256i state[QSC_KECCAK_STATE_SIZE], qsc_keccak_rate rate,
	uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3, size_t nblocks);

#endif

/* parallel Keccak x8 */

#if defined(QSC_SYSTEM_HAS_AVX512)

/**
* \brief Absorb 4 Keccak instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param state: The Keccak state array
* \param rate: The shake rate
* \param inp0: [const] The 1st input key array
* \param inp1: [const] The 2nd input key array
* \param inp2: [const] The 3rd input key array
* \param inp3: [const] The 4th input key array
* \param inp4: [const] The 5th input key array
* \param inp5: [const] The 6th input key array
* \param inp6: [const] The 7th input key array
* \param inp7: [const] The 8th input key array
* \param inplen: The length of the input key arrays
* \param domain
*/
void qsc_keccakx8_absorb(__m512i state[QSC_KECCAK_STATE_SIZE], qsc_keccak_rate rate,
	const uint8_t* inp0, const uint8_t* inp1, const uint8_t* inp2, const uint8_t* inp3,
	const uint8_t* inp4, const uint8_t* inp5, const uint8_t* inp6, const uint8_t* inp7, size_t inplen, uint8_t domain);

/**
* \brief Squeeze 4 Keccak instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param state: The Keccak state array
* \param rate: The Keccak rate
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param nblocks: The number of output blocks
*/
void qsc_keccakx8_squeezeblocks(__m512i state[QSC_KECCAK_STATE_SIZE], qsc_keccak_rate rate,
	uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3, uint8_t* out4,
	uint8_t* out5, uint8_t* out6, uint8_t* out7, size_t nblocks);

#endif

/* parallel SHAKE x4 */

/**
* \brief Process 4 SHAKE-128 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param outlen: The length of the output arrays
* \param inp0: [const] The 1st input key array
* \param inp1: [const] The 2nd input key array
* \param inp2: [const] The 3rd input key array
* \param inp3: [const] The 4th input key array
* \param inplen: The length of the input key arrays
*/
QSC_EXPORT_API void shake128x4(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3, size_t outlen,
	const uint8_t* inp0, const uint8_t* inp1, const uint8_t* inp2, const uint8_t* inp3, size_t inplen);

/**
* \brief Process 4 SHAKE-256 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param outlen: The length of the output arrays
* \param inp0: [const] The 1st input key array
* \param inp1: [const] The 2nd input key array
* \param inp2: [const] The 3rd input key array
* \param inp3: [const] The 4th input key array
* \param inplen: The length of the input key arrays
*/
QSC_EXPORT_API void shake256x4(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3, size_t outlen,
	const uint8_t* inp0, const uint8_t* inp1, const uint8_t* inp2, const uint8_t* inp3, size_t inplen);

/**
* \brief Process 4 SHAKE-512 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param outlen: The length of the output arrays
* \param inp0: [const] The 1st input key array
* \param inp1: [const] The 2nd input key array
* \param inp2: [const] The 3rd input key array
* \param inp3: [const] The 4th input key array
* \param inplen: The length of the input key arrays
*/
QSC_EXPORT_API void shake512x4(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3, size_t outlen,
	const uint8_t* inp0, const uint8_t* inp1, const uint8_t* inp2, const uint8_t* inp3, size_t inplen);

/* parallel shake x8 */

/**
* \brief Process 8 SHAKE-128 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX512 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param out4: The 5th output array
* \param out5: The 6th output array
* \param out6: The 7th output array
* \param out7: The 8th output array
* \param outlen: The length of the output arrays
* \param inp0: [const] The 1st input key array
* \param inp1: [const] The 2nd input key array
* \param inp2: [const] The 3rd input key array
* \param inp3: [const] The 4th input key array
* \param inp4: [const] The 5th input key array
* \param inp5: [const] The 6th input key array
* \param inp6: [const] The 7th input key array
* \param inp7: [const] The 8th input key array
* \param inplen: The length of the input key arrays
*/
QSC_EXPORT_API void shake128x8(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3,
	uint8_t* out4, uint8_t* out5, uint8_t* out6, uint8_t* out7, size_t outlen,
	const uint8_t* inp0, const uint8_t* inp1, const uint8_t* inp2, const uint8_t* inp3,
	const uint8_t* inp4, const uint8_t* inp5, const uint8_t* inp6, const uint8_t* inp7, size_t inplen);

/**
* \brief Process 8 SHAKE-256 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX512 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param out4: The 5th output array
* \param out5: The 6th output array
* \param out6: The 7th output array
* \param out7: The 8th output array
* \param outlen: The length of the output arrays
* \param [const] inp0: The 1st input key array
* \param [const] inp1: The 2nd input key array
* \param [const] inp2: The 3rd input key array
* \param [const] inp3: The 4th input key array
* \param [const] inp4: The 5th input key array
* \param [const] inp5: The 6th input key array
* \param [const] inp6: The 7th input key array
* \param [const] inp7: The 8th input key array
* \param inplen: The length of the input key arrays
*/
QSC_EXPORT_API void shake256x8(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3,
	uint8_t* out4, uint8_t* out5, uint8_t* out6, uint8_t* out7, size_t outlen,
	const uint8_t* inp0, const uint8_t* inp1, const uint8_t* inp2, const uint8_t* inp3,
	const uint8_t* inp4, const uint8_t* inp5, const uint8_t* inp6, const uint8_t* inp7, size_t inplen);

/**
* \brief Process 8 SHAKE-512 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX512 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param out4: The 5th output array
* \param out5: The 6th output array
* \param out6: The 7th output array
* \param out7: The 8th output array
* \param outlen: The length of the output arrays
* \param inp0: [const] The 1st input key array
* \param inp1: [const] The 2nd input key array
* \param inp2: [const] The 3rd input key array
* \param inp3: [const] The 4th input key array
* \param inp4: [const] The 5th input key array
* \param inp5: [const] The 6th input key array
* \param inp6: [const] The 7th input key array
* \param inp7: [const] The 8th input key array
* \param inplen: The length of the input key arrays
*/
QSC_EXPORT_API void shake512x8(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3,
	uint8_t* out4, uint8_t* out5, uint8_t* out6, uint8_t* out7, size_t outlen,
	const uint8_t* inp0, const uint8_t* inp1, const uint8_t* inp2, const uint8_t* inp3,
	const uint8_t* inp4, const uint8_t* inp5, const uint8_t* inp6, const uint8_t* inp7, size_t inplen);

/* parallel kmac x4 */

/**
* \brief Process 4 KMAC-128 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param outlen: The length of the output arrays
* \param key0: [const] The 1st key array
* \param key1: [const] The 2nd key array
* \param key2: [const] The 3rd key array
* \param key3: [const] The 4th key array
* \param keylen: The length of the input key arrays
* \param cst0: [const] The 1st custom array
* \param cst1: [const] The 2nd custom array
* \param cst2: [const] The 3rd custom array
* \param cst3: [const] The 4th custom array
* \param cstlen: The length of the custom arrays
* \param msg0: [const] The 1st message array
* \param msg1: [const] The 2nd message array
* \param msg2: [const] The 3rd message array
* \param msg3: [const] The 4th message array
* \param msglen: The length of the message arrays
*/
QSC_EXPORT_API void kmac128x4(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3, size_t outlen,
	const uint8_t* key0, const uint8_t* key1, const uint8_t* key2, const uint8_t* key3, size_t keylen,
	const uint8_t* cst0, const uint8_t* cst1, const uint8_t* cst2, const uint8_t* cst3, size_t cstlen,
	const uint8_t* msg0, const uint8_t* msg1, const uint8_t* msg2, const uint8_t* msg3, size_t msglen);

/**
* \brief Process 4 KMAC-256 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param outlen: The length of the output arrays
* \param key0: [const] The 1st key array
* \param key1: [const] The 2nd key array
* \param key2: [const] The 3rd key array
* \param key3: [const] The 4th key array
* \param keylen: The length of the input key arrays
* \param cst0: [const] The 1st custom array
* \param cst1: [const] The 2nd custom array
* \param cst2: [const] The 3rd custom array
* \param cst3: [const] The 4th custom array
* \param cstlen: The length of the custom arrays
* \param msg0: [const] The 1st message array
* \param msg1: [const] The 2nd message array
* \param msg2: [const] The 3rd message array
* \param msg3: [const] The 4th message array
* \param msglen: The length of the message arrays
*/
QSC_EXPORT_API void kmac256x4(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3, size_t outlen,
	const uint8_t* key0, const uint8_t* key1, const uint8_t* key2, const uint8_t* key3, size_t keylen,
	const uint8_t* cst0, const uint8_t* cst1, const uint8_t* cst2, const uint8_t* cst3, size_t cstlen,
	const uint8_t* msg0, const uint8_t* msg1, const uint8_t* msg2, const uint8_t* msg3, size_t msglen);

/**
* \brief Process 4 KMAC-512 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX2 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param outlen: The length of the output arrays
* \param key0: [const] The 1st key array
* \param key1: [const] The 2nd key array
* \param key2: [const] The 3rd key array
* \param key3: [const] The 4th key array
* \param keylen: The length of the input key arrays
* \param cst0: [const] The 1st custom array
* \param cst1: [const] The 2nd custom array
* \param cst2: [const] The 3rd custom array
* \param cst3: [const] The 4th custom array
* \param cstlen: The length of the custom arrays
* \param msg0: [const] The 1st message array
* \param msg1: [const] The 2nd message array
* \param msg2: [const] The 3rd message array
* \param msg3: [const] The 4th message array
* \param msglen: The length of the message arrays
*/
QSC_EXPORT_API void kmac512x4(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3, size_t outlen,
	const uint8_t* key0, const uint8_t* key1, const uint8_t* key2, const uint8_t* key3, size_t keylen,
	const uint8_t* cst0, const uint8_t* cst1, const uint8_t* cst2, const uint8_t* cst3, size_t cstlen,
	const uint8_t* msg0, const uint8_t* msg1, const uint8_t* msg2, const uint8_t* msg3, size_t msglen);

/* parallel KMAC x8 */

/**
* \brief Process 8 KMAC-128 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX512 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param out4: The 5th output array
* \param out5: The 6th output array
* \param out6: The 7th output array
* \param out7: The 8th output array
* \param outlen: The length of the output arrays
* \param key0: [const] The 1st key array
* \param key1: [const] The 2nd key array
* \param key2: [const] The 3rd key array
* \param key3: [const] The 4th key array
* \param key4: [const] The 5th key array
* \param key5: [const] The 6th key array
* \param key6: [const] The 7th key array
* \param key7: [const] The 8th key array
* \param keylen: The length of the key arrays
* \param cst0: [const] The 1st custom array
* \param cst1: [const] The 2nd custom array
* \param cst2: [const] The 3rd custom array
* \param cst3: [const] The 4th custom array
* \param cst4: [const] The 5th custom array
* \param cst5: [const] The 6th custom array
* \param cst6: [const] The 7th custom array
* \param cst7: [const] The 8th custom array
* \param cstlen: The length of the custom arrays
* \param msg0: [const] The 1st message array
* \param msg1: [const] The 2nd message array
* \param msg2: [const] The 3rd message array
* \param msg3: [const] The 4th message array
* \param msg4: [const] The 5th message array
* \param msg5: [const] The 6th message array
* \param msg6: [const] The 7th message array
* \param msg7: [const] The 8th message array
* \param msglen: The length of the message arrays
*/
QSC_EXPORT_API void kmac128x8(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3,
	uint8_t* out4, uint8_t* out5, uint8_t* out6, uint8_t* out7, size_t outlen,
	const uint8_t* key0, const uint8_t* key1, const uint8_t* key2, const uint8_t* key3,
	const uint8_t* key4, const uint8_t* key5, const uint8_t* key6, const uint8_t* key7, size_t keylen,
	const uint8_t* cst0, const uint8_t* cst1, const uint8_t* cst2, const uint8_t* cst3,
	const uint8_t* cst4, const uint8_t* cst5, const uint8_t* cst6, const uint8_t* cst7, size_t cstlen,
	const uint8_t* msg0, const uint8_t* msg1, const uint8_t* msg2, const uint8_t* msg3,
	const uint8_t* msg4, const uint8_t* msg5, const uint8_t* msg6, const uint8_t* msg7, size_t msglen);

/**
* \brief Process 8 KMAC-256 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX512 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param out4: The 5th output array
* \param out5: The 6th output array
* \param out6: The 7th output array
* \param out7: The 8th output array
* \param outlen: The length of the output arrays
* \param key0: [const] The 1st key array
* \param key1: [const] The 2nd key array
* \param key2: [const] The 3rd key array
* \param key3: [const] The 4th key array
* \param key4: [const] The 5th key array
* \param key5: [const] The 6th key array
* \param key6: [const] The 7th key array
* \param key7: [const] The 8th key array
* \param keylen: The length of the key arrays
* \param cst0: [const] The 1st custom array
* \param cst1: [const] The 2nd custom array
* \param cst2: [const] The 3rd custom array
* \param cst3: [const] The 4th custom array
* \param cst4: [const] The 5th custom array
* \param cst5: [const] The 6th custom array
* \param cst6: [const] The 7th custom array
* \param cst7: [const] The 8th custom array
* \param cstlen: The length of the custom arrays
* \param msg0: [const] The 1st message array
* \param msg1: [const] The 2nd message array
* \param msg2: [const] The 3rd message array
* \param msg3: [const] The 4th message array
* \param msg4: [const] The 5th message array
* \param msg5: [const] The 6th message array
* \param msg6: [const] The 7th message array
* \param msg7: [const] The 8th message array
* \param msglen: The length of the message arrays
*/
QSC_EXPORT_API void kmac256x8(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3,
	uint8_t* out4, uint8_t* out5, uint8_t* out6, uint8_t* out7, size_t outlen,
	const uint8_t* key0, const uint8_t* key1, const uint8_t* key2, const uint8_t* key3,
	const uint8_t* key4, const uint8_t* key5, const uint8_t* key6, const uint8_t* key7, size_t keylen,
	const uint8_t* cst0, const uint8_t* cst1, const uint8_t* cst2, const uint8_t* cst3,
	const uint8_t* cst4, const uint8_t* cst5, const uint8_t* cst6, const uint8_t* cst7, size_t cstlen,
	const uint8_t* msg0, const uint8_t* msg1, const uint8_t* msg2, const uint8_t* msg3,
	const uint8_t* msg4, const uint8_t* msg5, const uint8_t* msg6, const uint8_t* msg7, size_t msglen);

/**
* \brief Process 8 KMAC-512 instances simultaneously using SIMD instructions.
*
* \warning The input and output arrays must be of the same length.
* This function requires the AVX512 instruction set.
*
* \param out0: The 1st output array
* \param out1: The 2nd output array
* \param out2: The 3rd output array
* \param out3: The 4th output array
* \param out4: The 5th output array
* \param out5: The 6th output array
* \param out6: The 7th output array
* \param out7: The 8th output array
* \param outlen: The length of the output arrays
* \param key0: [const] The 1st key array
* \param key1: [const] The 2nd key array
* \param key2: [const] The 3rd key array
* \param key3: [const] The 4th key array
* \param key4: [const] The 5th key array
* \param key5: [const] The 6th key array
* \param key6: [const] The 7th key array
* \param key7: [const] The 8th key array
* \param keylen: The length of the key arrays
* \param cst0: [const] The 1st custom array
* \param cst1: [const] The 2nd custom array
* \param cst2: [const] The 3rd custom array
* \param cst3: [const] The 4th custom array
* \param cst4: [const] The 5th custom array
* \param cst5: [const] The 6th custom array
* \param cst6: [const] The 7th custom array
* \param cst7: [const] The 8th custom array
* \param cstlen: The length of the custom arrays
* \param msg0: [const] The 1st message array
* \param msg1: [const] The 2nd message array
* \param msg2: [const] The 3rd message array
* \param msg3: [const] The 4th message array
* \param msg4: [const] The 5th message array
* \param msg5: [const] The 6th message array
* \param msg6: [const] The 7th message array
* \param msg7: [const] The 8th message array
* \param msglen: The length of the message arrays
*/
QSC_EXPORT_API void kmac512x8(uint8_t* out0, uint8_t* out1, uint8_t* out2, uint8_t* out3,
	uint8_t* out4, uint8_t* out5, uint8_t* out6, uint8_t* out7, size_t outlen,
	const uint8_t* key0, const uint8_t* key1, const uint8_t* key2, const uint8_t* key3,
	const uint8_t* key4, const uint8_t* key5, const uint8_t* key6, const uint8_t* key7, size_t keylen,
	const uint8_t* cst0, const uint8_t* cst1, const uint8_t* cst2, const uint8_t* cst3,
	const uint8_t* cst4, const uint8_t* cst5, const uint8_t* cst6, const uint8_t* cst7, size_t cstlen,
	const uint8_t* msg0, const uint8_t* msg1, const uint8_t* msg2, const uint8_t* msg3,
	const uint8_t* msg4, const uint8_t* msg5, const uint8_t* msg6, const uint8_t* msg7, size_t msglen);

#endif
