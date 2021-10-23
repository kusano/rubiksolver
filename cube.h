#pragma once

/*
   UUU
   UUU
   UUU
LLLFFFRRRBBB
LLLFFFRRRBBB
LLLFFFRRRBBB
   DDD
   DDD
   DDD

■コーナーの位置
   3.2
   .U.
   0.1
3.00.11.22.3
.L..F..R..B.
7.44.55.66.7
   4.5
   .D.
   7.6

■コーナーの向き
U/DがU/Dを向いているときに0
時計回りに1、2 

■エッジの位置
   .2.
   3U1
   .0.
.3..0..1..2.
7L44F55R66B7
.b..8..9..a.
   .8.
   bD9
   .a.

■エッジの向き
U/Dを含むエッジはU/DがU/D/F/Bを向いているときに0
U/Dを含まないエッジはF/BがU/D/F/Bを向いているときに0

corner_pos[x]=y: y番目のコーナーが位置xにある
corner_dir[x]=y: 位置xのコーナーの向きはy
edge_pos[x]=y: y番目のエッジが位置xにある
edge_dir[x]=y: 位置xのエッジの向きはy

後述のcp, cdなどが他の値に依存せずに移動後の値を決められるよう、posは添え字のキューブががどこにあるか、
dirは添え字の位置のキューブの向きを持つ。

cp: corner_posが8P8の何番目か
cd: corner_dir[:7]
ep1: edge_pos[:6]が12P6の何番目か
ep2: edge_pos[6:]が12P6の何番目か
ed: edge_dir[:12]
  ep1/ep2と組み合わせてpattern databaseに利用するため、冗長な最後の1要素分も持つ
*/

#include <cstdint>
#include <string>

enum class Move
{
    F1 = 0,
    F2 = 1,
    F3 = 2,
    B1 = 3,
    B2 = 4,
    B3 = 5,
    L1 = 6,
    L2 = 7,
    L3 = 8,
    R1 = 9,
    R2 = 10,
    R3 = 11,
    U1 = 12,
    U2 = 13,
    U3 = 14,
    D1 = 15,
    D2 = 16,
    D3 = 17,
};

class Cube
{
public:
    static void init();

    std::uint16_t cp = 0;  // <40320
    std::uint16_t cd = 0;  // <2187
    std::uint32_t ep1 = 0;  // <665280
    std::uint32_t ep2 = 0;  // <665280
    std::uint16_t ed = 0;  // <2048

    Cube();
    void move(Move m);
    void move_naive(Move m);
    bool is_solved() {return cp==0 && cd==0 && ep1==0 && ep2==((((11*6+6)*10+6)*9+6)*8+6)*7+6 && ed==0;}
    static std::uint32_t comp_ep(std::uint32_t ep1);
    std::string str() const;

private:
    static std::uint16_t cp_table[18][40320];
    static std::uint16_t cd_table[18][2187];
    static std::uint32_t ep1_table[18][665280];
    static std::uint32_t ep2_table[18][665280];
    static std::uint16_t ed_table[18][2048];

    //void move_naive(Move m);

    static void index_to_array(std::uint16_t cp, std::uint16_t cd, std::uint32_t ep1, std::uint32_t ep2, std::uint16_t ed,
        int corner_cube[8], int corner_dir[8], int edge_cube[12], int edge_dir[12]);
    static void array_to_index(int corner_cube[8], int corner_dir[8], int edge_cube[12], int edge_dir[12],
        std::uint16_t *cp, std::uint16_t *cd, std::uint32_t *ep1, std::uint32_t *ep2, std::uint16_t *ed);
    static void index_to_perm(int index, int *perm, int n, int k);
    static int perm_to_index(int *perm, int n, int k);
};
