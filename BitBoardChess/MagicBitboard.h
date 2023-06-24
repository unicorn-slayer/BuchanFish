#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

#define U64 unsigned long long

#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)

// square encoding
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

// rook & bishop flags
enum { rook, bishop };



class MagicBitboards
{
public:

    MagicBitboards();

    // do simple functions as static; do more complex functions, which involve member variables, as non-static
    int static count_bits(U64 bitboard);

    int static get_ls1b_index(U64 bitboard);

    U64 static set_occupancy(int index, int bits_in_mask, U64 attack_mask);

    U64 mask_bishop_attacks(int square);

    U64 mask_rook_attacks(int square);

    U64 bishop_attacks_on_the_fly(int square, U64 block);

    U64 rook_attacks_on_the_fly(int square, U64 block);

    void init_sliders_attacks(int is_bishop);

    U64 get_bishop_attacks(int square, U64 occupancy);

    U64 get_rook_attacks(int square, U64 occupancy);

    void static print_bitboard(U64 bitboard);

    const U64(&getBishopAttacks() const)[64][512];

private:

    U64 m_bishop_masks[64];
    U64 m_rook_masks[64];

    // attacks // consider placing below as global variables in MagicBitboard.cpp so can place on static rather than on heap
    U64 m_bishop_attacks[64][512];
    U64 m_rook_attacks[64][4096];

};