#include "cube.h"

#include <iostream>
#include <string>

void Cube::init()
{
    for (int mi=0; mi<18; mi++)
    {
        Move m = Move(mi);

        //  cp_table
        for (std::uint16_t cp=0; cp<40320; cp++)
        {
            Cube cube;
            cube.cp = cp;
            cube.move_naive(m);
            cp_table[mi][cp] = cube.cp;
        }

        //  cd_table
        for (std::uint16_t cd=0; cd<2187; cd++)
        {
            Cube cube;
            cube.cd = cd;
            cube.move_naive(m);
            cd_table[mi][cd] = cube.cd;
        }

        //  ep1_table
        for (std::uint32_t ep1=0; ep1<665280; ep1++)
        {
            Cube cube;
            cube.ep1 = ep1;
            cube.ep2 = comp_ep(ep1);
            cube.move_naive(m);
            ep1_table[mi][ep1] = cube.ep1;
        }

        //  ep2_table
        for (std::uint32_t ep2=0; ep2<665280; ep2++)
        {
            Cube cube;

            cube.ep1 = comp_ep(ep2);
            cube.ep2 = ep2;
            cube.move_naive(m);
            ep2_table[mi][ep2] = cube.ep2;
        }

        //  ed_table
        for (std::uint16_t ed=0; ed<2048; ed++)
        {
            Cube cube;
            cube.ed = ed;
            cube.move_naive(m);
            ed_table[mi][ed] = cube.ed;
        }
    }
}

Cube::Cube()
{
    //  初期状態のep2は0ではない
    ep2 = ((((11*6+6)*10+6)*9+6)*8+6)*7+6;
}

void Cube::move(Move m)
{
    int mi = int(m);
    cp = cp_table[mi][cp];
    cd = cd_table[mi][cd];
    ep1 = ep1_table[mi][ep1];
    ep2 = ep2_table[mi][ep2];
    ed = ed_table[mi][ed];
}

std::string Cube::str() const
{
    int corner_cube[8];
    int corner_dir[8];
    int edge_cube[12];
    int edge_dir[12];
    index_to_array(cp, cd, ep1, ep2, ed,
        corner_cube, corner_dir, edge_cube, edge_dir);

    char cube[9][13] = {
        "   ...      ",
        "   .U.      ",
        "   ...      ",
        "............",
        ".L..F..R..B.",
        "............",
        "   ...      ",
        "   .D.      ",
        "   ...      ",
    };

    const char *corner[8] = {
        "UFL",
        "URF",
        "UBR",
        "ULB",
        "DLF",
        "DFR",
        "DRB",
        "DBL",
    };
    const char *edge[12] = {
        "UF",
        "UR",
        "UB",
        "UL",
        "FL",
        "FR",
        "BR",
        "BL",
        "DF",
        "DR",
        "DB",
        "DL",
    };

    cube[2][ 3] = corner[corner_cube[0]][(3-corner_dir[0])%3];
    cube[3][ 3] = corner[corner_cube[0]][(4-corner_dir[0])%3];
    cube[3][ 2] = corner[corner_cube[0]][(5-corner_dir[0])%3];

    cube[2][ 5] = corner[corner_cube[1]][(3-corner_dir[1])%3];
    cube[3][ 6] = corner[corner_cube[1]][(4-corner_dir[1])%3];
    cube[3][ 5] = corner[corner_cube[1]][(5-corner_dir[1])%3];

    cube[0][ 5] = corner[corner_cube[2]][(3-corner_dir[2])%3];
    cube[3][ 9] = corner[corner_cube[2]][(4-corner_dir[2])%3];
    cube[3][ 8] = corner[corner_cube[2]][(5-corner_dir[2])%3];

    cube[0][ 3] = corner[corner_cube[3]][(3-corner_dir[3])%3];
    cube[3][ 0] = corner[corner_cube[3]][(4-corner_dir[3])%3];
    cube[3][11] = corner[corner_cube[3]][(5-corner_dir[3])%3];

    cube[6][ 3] = corner[corner_cube[4]][(3-corner_dir[4])%3];
    cube[5][ 2] = corner[corner_cube[4]][(4-corner_dir[4])%3];
    cube[5][ 3] = corner[corner_cube[4]][(5-corner_dir[4])%3];

    cube[6][ 5] = corner[corner_cube[5]][(3-corner_dir[5])%3];
    cube[5][ 5] = corner[corner_cube[5]][(4-corner_dir[5])%3];
    cube[5][ 6] = corner[corner_cube[5]][(5-corner_dir[5])%3];

    cube[8][ 5] = corner[corner_cube[6]][(3-corner_dir[6])%3];
    cube[5][ 8] = corner[corner_cube[6]][(4-corner_dir[6])%3];
    cube[5][ 9] = corner[corner_cube[6]][(5-corner_dir[6])%3];

    cube[8][ 3] = corner[corner_cube[7]][(3-corner_dir[7])%3];
    cube[5][11] = corner[corner_cube[7]][(4-corner_dir[7])%3];
    cube[5][ 0] = corner[corner_cube[7]][(5-corner_dir[7])%3];

    cube[2][ 4] = edge[edge_cube[ 0]][  edge_dir[ 0]];
    cube[3][ 4] = edge[edge_cube[ 0]][1-edge_dir[ 0]];

    cube[1][ 5] = edge[edge_cube[ 1]][  edge_dir[ 1]];
    cube[3][ 7] = edge[edge_cube[ 1]][1-edge_dir[ 1]];

    cube[0][ 4] = edge[edge_cube[ 2]][  edge_dir[ 2]];
    cube[3][10] = edge[edge_cube[ 2]][1-edge_dir[ 2]];

    cube[1][ 3] = edge[edge_cube[ 3]][  edge_dir[ 3]];
    cube[3][ 1] = edge[edge_cube[ 3]][1-edge_dir[ 3]];

    cube[4][ 3] = edge[edge_cube[ 4]][  edge_dir[ 4]];
    cube[4][ 2] = edge[edge_cube[ 4]][1-edge_dir[ 4]];

    cube[4][ 5] = edge[edge_cube[ 5]][  edge_dir[ 5]];
    cube[4][ 6] = edge[edge_cube[ 5]][1-edge_dir[ 5]];

    cube[4][ 9] = edge[edge_cube[ 6]][  edge_dir[ 6]];
    cube[4][ 8] = edge[edge_cube[ 6]][1-edge_dir[ 6]];

    cube[4][11] = edge[edge_cube[ 7]][  edge_dir[ 7]];
    cube[4][ 0] = edge[edge_cube[ 7]][1-edge_dir[ 7]];

    cube[6][ 4] = edge[edge_cube[ 8]][  edge_dir[ 8]];
    cube[5][ 4] = edge[edge_cube[ 8]][1-edge_dir[ 8]];

    cube[7][ 5] = edge[edge_cube[ 9]][  edge_dir[ 9]];
    cube[5][ 7] = edge[edge_cube[ 9]][1-edge_dir[ 9]];

    cube[8][ 4] = edge[edge_cube[10]][  edge_dir[10]];
    cube[5][10] = edge[edge_cube[10]][1-edge_dir[10]];

    cube[7][ 3] = edge[edge_cube[11]][  edge_dir[11]];
    cube[5][ 1] = edge[edge_cube[11]][1-edge_dir[11]];

    std::string ret;
    for (int i=0; i<9; i++)
    {
        ret += cube[i];
        ret += "\n";
    }
    return ret;
}

std::uint16_t Cube::cp_table[18][40320];
std::uint16_t Cube::cd_table[18][2187];
std::uint32_t Cube::ep1_table[18][665280];
std::uint32_t Cube::ep2_table[18][665280];
std::uint16_t Cube::ed_table[18][2048];

void Cube::move_naive(Move m)
{
    int corner_cube[8];
    int corner_dir[8];
    int edge_cube[12];
    int edge_dir[12];
    index_to_array(cp, cd, ep1, ep2, ed,
        corner_cube, corner_dir, edge_cube, edge_dir);

    Move face = Move(int(m)/3*3);
    int n = int(m)%3+1;

    for (int i=0; i<n; i++)
    {
        int t;
        switch (face) {
        case Move::F1:
            t = corner_cube[0];
            corner_cube[0] = corner_cube[4];
            corner_cube[4] = corner_cube[5];
            corner_cube[5] = corner_cube[1];
            corner_cube[1] = t;

            t = corner_dir[0];
            corner_dir[0] = (corner_dir[4]+2)%3;
            corner_dir[4] = (corner_dir[5]+1)%3;
            corner_dir[5] = (corner_dir[1]+2)%3;
            corner_dir[1] = (t+1)%3;

            t = edge_cube[0];
            edge_cube[0] = edge_cube[4];
            edge_cube[4] = edge_cube[8];
            edge_cube[8] = edge_cube[5];
            edge_cube[5] = t;

            t = edge_dir[0];
            edge_dir[0] = 1-edge_dir[4];
            edge_dir[4] = 1-edge_dir[8];
            edge_dir[8] = 1-edge_dir[5];
            edge_dir[5] = 1-t;
            break;
        case Move::B1:
            t = corner_cube[2];
            corner_cube[2] = corner_cube[6];
            corner_cube[6] = corner_cube[7];
            corner_cube[7] = corner_cube[3];
            corner_cube[3] = t;

            t = corner_dir[2];
            corner_dir[2] = (corner_dir[6]+2)%3;
            corner_dir[6] = (corner_dir[7]+1)%3;
            corner_dir[7] = (corner_dir[3]+2)%3;
            corner_dir[3] = (t+1)%3;

            t = edge_cube[2];
            edge_cube[2] = edge_cube[6];
            edge_cube[6] = edge_cube[10];
            edge_cube[10] = edge_cube[7];
            edge_cube[7] = t;

            t = edge_dir[2];
            edge_dir[2] = 1-edge_dir[6];
            edge_dir[6] = 1-edge_dir[10];
            edge_dir[10] = 1-edge_dir[7];
            edge_dir[7] = 1-t;
            break;
        case Move::L1:
            t = corner_cube[0];
            corner_cube[0] = corner_cube[3];
            corner_cube[3] = corner_cube[7];
            corner_cube[7] = corner_cube[4];
            corner_cube[4] = t;

            t = corner_dir[0];
            corner_dir[0] = (corner_dir[3]+1)%3;
            corner_dir[3] = (corner_dir[7]+2)%3;
            corner_dir[7] = (corner_dir[4]+1)%3;
            corner_dir[4] = (t+2)%3;

            t = edge_cube[3];
            edge_cube[3] = edge_cube[7];
            edge_cube[7] = edge_cube[11];
            edge_cube[11] = edge_cube[4];
            edge_cube[4] = t;

            t = edge_dir[3];
            edge_dir[3] = edge_dir[7];
            edge_dir[7] = edge_dir[11];
            edge_dir[11] = edge_dir[4];
            edge_dir[4] = t;
            break;
        case Move::R1:
            t = corner_cube[1];
            corner_cube[1] = corner_cube[5];
            corner_cube[5] = corner_cube[6];
            corner_cube[6] = corner_cube[2];
            corner_cube[2] = t;

            t = corner_dir[1];
            corner_dir[1] = (corner_dir[5]+2)%3;
            corner_dir[5] = (corner_dir[6]+1)%3;
            corner_dir[6] = (corner_dir[2]+2)%3;
            corner_dir[2] = (t+1)%3;

            t = edge_cube[1];
            edge_cube[1] = edge_cube[5];
            edge_cube[5] = edge_cube[9];
            edge_cube[9] = edge_cube[6];
            edge_cube[6] = t;

            t = edge_dir[1];
            edge_dir[1] = edge_dir[5];
            edge_dir[5] = edge_dir[9];
            edge_dir[9] = edge_dir[6];
            edge_dir[6] = t;
            break;
        case Move::U1:
            t = corner_cube[0];
            corner_cube[0] = corner_cube[1];
            corner_cube[1] = corner_cube[2];
            corner_cube[2] = corner_cube[3];
            corner_cube[3] = t;

            t = corner_dir[0];
            corner_dir[0] = corner_dir[1];
            corner_dir[1] = corner_dir[2];
            corner_dir[2] = corner_dir[3];
            corner_dir[3] = t;

            t = edge_cube[0];
            edge_cube[0] = edge_cube[1];
            edge_cube[1] = edge_cube[2];
            edge_cube[2] = edge_cube[3];
            edge_cube[3] = t;

            t = edge_dir[0];
            edge_dir[0] = edge_dir[1];
            edge_dir[1] = edge_dir[2];
            edge_dir[2] = edge_dir[3];
            edge_dir[3] = t;
            break;
        case Move::D1:
            t = corner_cube[4];
            corner_cube[4] = corner_cube[7];
            corner_cube[7] = corner_cube[6];
            corner_cube[6] = corner_cube[5];
            corner_cube[5] = t;

            t = corner_dir[4];
            corner_dir[4] = corner_dir[7];
            corner_dir[7] = corner_dir[6];
            corner_dir[6] = corner_dir[5];
            corner_dir[5] = t;

            t = edge_cube[8];
            edge_cube[8] = edge_cube[11];
            edge_cube[11] = edge_cube[10];
            edge_cube[10] = edge_cube[9];
            edge_cube[9] = t;

            t = edge_dir[8];
            edge_dir[8] = edge_dir[11];
            edge_dir[11] = edge_dir[10];
            edge_dir[10] = edge_dir[9];
            edge_dir[9] = t;
            break;
        }
    }

    array_to_index(corner_cube, corner_dir, edge_cube, edge_dir,
        &cp, &cd, &ep1, &ep2, &ed);
}

//  ep1と矛盾しないep2を返す
std::uint32_t Cube::comp_ep(std::uint32_t ep1)
{
    int ep1_perm[6];
    index_to_perm(ep1, ep1_perm, 12, 6);
    bool V[12] = {};
    for (int p: ep1_perm)
        V[p] = true;
    int ep2_perm[6];
    int c = 0;
    for (int i=0; i<12; i++)
        if (!V[i])
            ep2_perm[c++] = i;
    return (std::uint32_t)perm_to_index(ep2_perm, 12, 6);
}

void Cube::index_to_array(std::uint16_t cp, std::uint16_t cd, std::uint32_t ep1, std::uint32_t ep2, std::uint16_t ed,
    int corner_cube[8], int corner_dir[8], int edge_cube[12], int edge_dir[12])
{
    int corner_pos[8];
    index_to_perm(cp, corner_pos, 8, 8);
    for (int i=0; i<8; i++)
        corner_cube[corner_pos[i]] = i;

    for (int i=0; i<7; i++)
    {
        corner_dir[6-i] = cd%3;
        cd /= 3;
    }
    int s = 0;
    for (int i=0; i<7; i++)
        s += corner_dir[i];
    corner_dir[7] = (3-s%3)%3;

    int edge_pos[12];
    index_to_perm(ep1, edge_pos, 12, 6);
    index_to_perm(ep2, edge_pos+6, 12, 6);
    for (int i=0; i<12; i++)
        edge_cube[edge_pos[i]] = i;

    for (int i=0; i<11; i++)
    {
        edge_dir[10-i] = ed%2;
        ed /= 2;
    }
    s = 0;
    for (int i=0; i<11; i++)
        s += edge_dir[i];
    edge_dir[11] = s%2;
}

void Cube::array_to_index(int corner_cube[8], int corner_dir[8], int edge_cube[12], int edge_dir[12],
    std::uint16_t *cp, std::uint16_t *cd, std::uint32_t *ep1, std::uint32_t *ep2, std::uint16_t *ed)
{
    int corner_pos[8];
    for (int i=0; i<8; i++)
        corner_pos[corner_cube[i]] = i;
    *cp = (std::uint16_t)perm_to_index(corner_pos, 8, 8);

    *cd = 0;
    for (int i=0; i<7; i++)
        *cd = *cd * 3 + corner_dir[i];

    int edge_pos[12];
    for (int i=0; i<12; i++)
        edge_pos[edge_cube[i]] = i;
    *ep1 = (std::uint32_t)perm_to_index(edge_pos, 12, 6);
    *ep2 = (std::uint32_t)perm_to_index(edge_pos+6, 12, 6);

    *ed = 0;
    for (int i=0; i<11; i++)
        *ed = *ed * 2 + edge_dir[i];
}

void Cube::index_to_perm(int index, int *perm, int n, int k)
{
    static const int N = 12;  // max(n)

    int I[N] = {};
    for (int i=0; i<k; i++)
    {
        I[k-i-1] = index%(n-k+i+1);
        index /= n-k+i+1;
    }

    bool V[N] = {};
    for (int i=0; i<k; i++)
        for (int j=0; j<n; j++)
            if (!V[j])
            {
                if (I[i]==0)
                {
                    V[j] = true;
                    perm[i] = j;
                    break;
                }
                I[i]--;
            }
}

int Cube::perm_to_index(int *perm, int n, int k)
{
    static const int N = 12;  // max(n)

    int I[N] = {};
    bool V[N] = {};
    for (int i=0; i<k; i++)
        for (int j=0; j<n; j++)
            if (!V[j])
            {
                if (j==perm[i])
                {
                    V[j] = true;
                    break;
                }
                I[i]++;
            }

    int index = 0;
    for (int i=0; i<k; i++)
    {
        index *= n-i;
        index += I[i];
    }

    return index;
}

/*
#include <iostream>
using namespace std;

int main()
{
    int V1[4];
    for (int i=0; i<24; i++)
    {
        index_to_perm(i, V1, 4, 4);
        for (int v: V1)
            cout<<" "<<v;
        cout<<": "<<perm_to_index(V1, 4, 4)<<endl;
    }

    int V2[2];
    for (int i=0; i<20; i++)
    {
        index_to_perm(i, V2, 5, 2);
        for (int v: V2)
            cout<<" "<<v;
        cout<<": "<<perm_to_index(V2, 5, 2)<<endl;
    }
}
*/
