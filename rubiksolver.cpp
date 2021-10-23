#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
using namespace std;
using namespace chrono;

//  Number of edges for pattern database
#define EDGE_NUM 7
//  Use all edges (position only) pattern
#define EDGE_ALL 0

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

# corner
 0   1   2   3   4   5   6   7
UFL URF UBR ULB DLF DFR DRB DBL

# edge
 0  1  2  3  4  5  6  7  8  9 10 11
UF UR UB UL FL FR BR BL DF DR DB DL
*/
class Cube
{
public:
    Cube();
    bool is_solved() const;
    void move(int m);
    int get_corner_index() const;
    void set_corner_index(int index);
    int get_edge1_index() const;
    void set_edge1_index(int index);
    int get_edge2_index() const;
    void set_edge2_index(int index);
    int get_edge3_index() const;
    void set_edge3_index(int index);
    string str() const;
    string from_str(vector<string> cube);

private:
    struct XY{int x, y;};

    static const char corner_str[8][4];
    static const char edge_str[12][3];
    static const XY corner_pos[8][3];
    static const XY edge_pos[12][2];

    static void rotate1(int v[], int p0, int p1, int p2, int p3);
    static void rotate2(int v[], int p0, int p1, int p2, int p3);
    static void rotate3(int v[], int p0, int p1, int p2, int p3);
    static void flip_c(int c[], int p0, int p1, int p2, int p3);
    static void flip_e(int e[], int p0, int p1, int p2, int p3);
    static void index_to_perm(int index, int perm[], int n, int k);
    static int perm_to_index(const int perm[], int n, int k);

    int corner_cube[8];
    int corner_dir[8];
    int edge_cube[12];
    int edge_dir[12];
};

const char * const move_str[18] = {
    "F", "F2", "F'",
    "B", "B2", "B'",
    "L", "L2", "L'",
    "R", "R2", "R'",
    "U", "U2", "U'",
    "D", "D2", "D'",
};

uint8_t pattern_corner[88'179'840];
#if EDGE_NUM==6
uint8_t pattern_edge1[42'577'920];
uint8_t pattern_edge2[42'577'920];
#endif
#if EDGE_NUM==7
uint8_t pattern_edge1[510'935'040];
uint8_t pattern_edge2[510'935'040];
#endif
#if EDGE_ALL
uint8_t pattern_edge3[479'001'600];
#endif

void make_database();
vector<int> solve(Cube cube);
bool search(Cube &cube, int depth, int prev_move, long long *node_num, vector<int> *answer);
int reverse_move(int m);

int main()
{
    cout.setf(ios::unitbuf);

    cout<<"Make pattern database"<<endl;
    system_clock::time_point start = system_clock::now();
    make_database();
    cout<<duration_cast<chrono::milliseconds>(system_clock::now()-start).count()<<" [ms]"<<endl;

    while (true)
    {
        cout<<"Input cube:"<<endl;
        vector<string> cube_str(9);
        for (string &s: cube_str)
            getline(cin, s);

        Cube cube;
        string result = cube.from_str(cube_str);
        if (result!="")
        {
            cout<<result<<endl;
            continue;
        }

        start = system_clock::now();
        vector<int> answer = solve(cube);
        cout<<duration_cast<chrono::milliseconds>(system_clock::now()-start).count()<<" [ms]"<<endl;

        cout<<answer.size()<<" moves"<<endl;
        cout<<"Solve:";
        for (int m: answer)
            cout<<" "<<move_str[m];
        cout<<endl;
        cout<<"Generate:";
        for (auto m=answer.rbegin(); m!=answer.rend(); m++)
            cout<<" "<<move_str[reverse_move(*m)];
        cout<<endl;
    }
}

void make_database()
{
    Cube cube;
    Cube solved;
    uint8_t oo = 255;

    cout<<"corner:";
    for (uint8_t &x: pattern_corner)
        x = oo;
    pattern_corner[solved.get_corner_index()] = 0;
    for (int d=0; ; d++)
    {
        cout<<" "<<d;
        bool update = false;
        for (int index=0; index<int(sizeof pattern_corner); index++)
            if (pattern_corner[index]==d)
            {
                cube.set_corner_index(index);
                for (int m=0; m<18; m++)
                {
                    cube.move(m);
                    int next = cube.get_corner_index();
                    if (pattern_corner[next]==oo)
                    {
                        pattern_corner[next] = uint8_t(d+1);
                        update = true;
                    }
                    cube.move(reverse_move(m));
                }
            }
        if (!update)
            break;
    }
    cout<<endl;

    cout<<"edge1:";
    for (uint8_t &x: pattern_edge1)
        x = oo;
    pattern_edge1[solved.get_edge1_index()] = 0;
    for (int d=0; ; d++)
    {
        cout<<" "<<d;
        bool update = false;
        for (int index=0; index<int(sizeof pattern_edge1); index++)
            if (pattern_edge1[index]==d)
            {
                cube.set_edge1_index(index);
                for (int m=0; m<18; m++)
                {
                    cube.move(m);
                    int next = cube.get_edge1_index();
                    if (pattern_edge1[next]==oo)
                    {
                        pattern_edge1[next] = uint8_t(d+1);
                        update = true;
                    }
                    cube.move(reverse_move(m));
                }
            }
        if (!update)
            break;
    }
    cout<<endl;

    cout<<"edge2:";
    for (uint8_t &x: pattern_edge2)
        x = oo;
    pattern_edge2[solved.get_edge2_index()] = 0;
    for (int d=0; ; d++)
    {
        cout<<" "<<d;
        bool update = false;
        for (int index=0; index<int(sizeof pattern_edge2); index++)
            if (pattern_edge2[index]==d)
            {
                cube.set_edge2_index(index);
                for (int m=0; m<18; m++)
                {
                    cube.move(m);
                    int next = cube.get_edge2_index();
                    if (pattern_edge2[next]==oo)
                    {
                        pattern_edge2[next] = uint8_t(d+1);
                        update = true;
                    }
                    cube.move(reverse_move(m));
                }
            }
        if (!update)
            break;
    }
    cout<<endl;

#if EDGE_ALL
    cout<<"edge3:";
    for (uint8_t &x: pattern_edge3)
        x = oo;
    pattern_edge3[solved.get_edge3_index()] = 0;
    for (int d=0; ; d++)
    {
        cout<<" "<<d;
        bool update = false;
        for (int index=0; index<int(sizeof pattern_edge3); index++)
            if (pattern_edge3[index]==d)
            {
                cube.set_edge3_index(index);
                for (int m=0; m<18; m++)
                {
                    cube.move(m);
                    int next = cube.get_edge3_index();
                    if (pattern_edge3[next]==oo)
                    {
                        pattern_edge3[next] = uint8_t(d+1);
                        update = true;
                    }
                    cube.move(reverse_move(m));
                }
            }
        if (!update)
            break;
    }
    cout<<endl;
#endif
}

vector<int> solve(Cube cube)
{
    for (int depth=0; ; depth++)
    {
        long long node_num = 0;
        vector<int> answer;
        bool solved = search(cube, depth, -1, &node_num, &answer);
        cout<<"depth: "<<depth<<", node number: "<<node_num<<endl;
        if (solved)
        {
            reverse(answer.begin(), answer.end());
            return answer;
        }
    }
}

bool search(Cube &cube, int depth, int prev_move, long long *node_num, vector<int> *answer)
{
    (*node_num)++;

    if (cube.is_solved())
        return true;
    if (depth==0)
        return false;

    //  heuristic
    if (depth<pattern_corner[cube.get_corner_index()])
        return false;
    if (depth<pattern_edge1[cube.get_edge1_index()])
        return false;
    if (depth<pattern_edge2[cube.get_edge2_index()])
        return false;
#if EDGE_ALL
    if (depth<pattern_edge3[cube.get_edge3_index()])
        return false;
#endif

    for (int m=0; m<18; m++)
    {
        if (prev_move>=0)
            if (m/3==prev_move/3 ||
                m/3==0 && prev_move/3==1 ||
                m/3==2 && prev_move/3==3 ||
                m/3==4 && prev_move/3==5)
                continue;

        cube.move(m);
        bool solved = search(cube, depth-1, m, node_num, answer);
        cube.move(reverse_move(m));

        if (solved)
        {
            answer->push_back(m);
            return true;
        }
    }
    return false;
}

int reverse_move(int m)
{
    return m/3*3+(2-m%3);
}

Cube::Cube()
{
    for (int i=0; i<8; i++)
    {
        corner_cube[i] = i;
        corner_dir[i] = 0;
    }
    for (int i=0; i<12; i++)
    {
        edge_cube[i] = i;
        edge_dir[i] = 0;
    }
}

bool Cube::is_solved() const
{
    for (int i=0; i<8; i++)
    {
        if (corner_cube[i]!=i)
            return false;
        if (corner_dir[i]!=0)
            return false;
    }
    for (int i=0; i<12; i++)
    {
        if (edge_cube[i]!=i)
            return false;
        if (edge_dir[i]!=0)
            return false;
    }
    return true;
}

void Cube::move(int m)
{
    switch (m)
    {
    case 0:  // F
        rotate1(corner_cube, 0, 1, 5, 4);
        rotate1(corner_dir,  0, 1, 5, 4);
        flip_c (corner_dir,  0, 1, 5, 4);
        rotate1(edge_cube, 0, 5, 8, 4);
        rotate1(edge_dir,  0, 5, 8, 4);
        flip_e (edge_dir,  0, 5, 8, 4);
        break;
    case 1:  // F2
        rotate2(corner_cube, 0, 1, 5, 4);
        rotate2(corner_dir,  0, 1, 5, 4);
        rotate2(edge_cube, 0, 5, 8, 4);
        rotate2(edge_dir,  0, 5, 8, 4);
        break;
    case 2:  // F'
        rotate3(corner_cube, 0, 1, 5, 4);
        rotate3(corner_dir,  0, 1, 5, 4);
        flip_c (corner_dir,  0, 1, 5, 4);
        rotate3(edge_cube, 0, 5, 8, 4);
        rotate3(edge_dir,  0, 5, 8, 4);
        flip_e (edge_dir,  0, 5, 8, 4);
        break;
    case 3:  // B
        rotate1(corner_cube, 2, 3, 7, 6);
        rotate1(corner_dir,  2, 3, 7, 6);
        flip_c (corner_dir,  2, 3, 7, 6);
        rotate1(edge_cube, 2, 7, 10, 6);
        rotate1(edge_dir,  2, 7, 10, 6);
        flip_e (edge_dir,  2, 7, 10, 6);
        break;
    case 4:  // B2
        rotate2(corner_cube, 2, 3, 7, 6);
        rotate2(corner_dir,  2, 3, 7, 6);
        rotate2(edge_cube, 2, 7, 10, 6);
        rotate2(edge_dir,  2, 7, 10, 6);
        break;
    case 5:  // B'
        rotate3(corner_cube, 2, 3, 7, 6);
        rotate3(corner_dir,  2, 3, 7, 6);
        flip_c (corner_dir,  2, 3, 7, 6);
        rotate3(edge_cube, 2, 7, 10, 6);
        rotate3(edge_dir,  2, 7, 10, 6);
        flip_e (edge_dir,  2, 7, 10, 6);
        break;
    case 6:  // L
        rotate1(corner_cube, 3, 0, 4, 7);
        rotate1(corner_dir,  3, 0, 4, 7);
        flip_c (corner_dir,  3, 0, 4, 7);
        rotate1(edge_cube, 3, 4, 11, 7);
        rotate1(edge_dir,  3, 4, 11, 7);
        break;
    case 7:  // L2
        rotate2(corner_cube, 3, 0, 4, 7);
        rotate2(corner_dir,  3, 0, 4, 7);
        rotate2(edge_cube, 3, 4, 11, 7);
        rotate2(edge_dir,  3, 4, 11, 7);
        break;
    case 8:  // L'
        rotate3(corner_cube, 3, 0, 4, 7);
        rotate3(corner_dir,  3, 0, 4, 7);
        flip_c (corner_dir,  3, 0, 4, 7);
        rotate3(edge_cube, 3, 4, 11, 7);
        rotate3(edge_dir,  3, 4, 11, 7);
        break;
    case 9:  // R
        rotate1(corner_cube, 1, 2, 6, 5);
        rotate1(corner_dir,  1, 2, 6, 5);
        flip_c (corner_dir,  1, 2, 6, 5);
        rotate1(edge_cube, 1, 6, 9, 5);
        rotate1(edge_dir,  1, 6, 9, 5);
        break;
    case 10:  // R2
        rotate2(corner_cube, 1, 2, 6, 5);
        rotate2(corner_dir,  1, 2, 6, 5);
        rotate2(edge_cube, 1, 6, 9, 5);
        rotate2(edge_dir,  1, 6, 9, 5);
        break;
    case 11:  // R'
        rotate3(corner_cube, 1, 2, 6, 5);
        rotate3(corner_dir,  1, 2, 6, 5);
        flip_c (corner_dir,  1, 2, 6, 5);
        rotate3(edge_cube, 1, 6, 9, 5);
        rotate3(edge_dir,  1, 6, 9, 5);
        break;
    case 12:  // U
        rotate1(corner_cube, 0, 3, 2, 1);
        rotate1(corner_dir,  0, 3, 2, 1);
        rotate1(edge_cube, 0, 3, 2, 1);
        rotate1(edge_dir,  0, 3, 2, 1);
        break;
    case 13:  // U2
        rotate2(corner_cube, 0, 3, 2, 1);
        rotate2(corner_dir,  0, 3, 2, 1);
        rotate2(edge_cube, 0, 3, 2, 1);
        rotate2(edge_dir,  0, 3, 2, 1);
        break;
    case 14:  // U'
        rotate3(corner_cube, 0, 3, 2, 1);
        rotate3(corner_dir,  0, 3, 2, 1);
        rotate3(edge_cube, 0, 3, 2, 1);
        rotate3(edge_dir,  0, 3, 2, 1);
        break;
    case 15:  // D
        rotate1(corner_cube, 4, 5, 6, 7);
        rotate1(corner_dir,  4, 5, 6, 7);
        rotate1(edge_cube, 8, 9, 10, 11);
        rotate1(edge_dir,  8, 9, 10, 11);
        break;
    case 16:  // D2
        rotate2(corner_cube, 4, 5, 6, 7);
        rotate2(corner_dir,  4, 5, 6, 7);
        rotate2(edge_cube, 8, 9, 10, 11);
        rotate2(edge_dir,  8, 9, 10, 11);
        break;
    case 17:  // D'
        rotate3(corner_cube, 4, 5, 6, 7);
        rotate3(corner_dir,  4, 5, 6, 7);
        rotate3(edge_cube, 8, 9, 10, 11);
        rotate3(edge_dir,  8, 9, 10, 11);
        break;
    }
}

int Cube::get_corner_index() const
{
    int index = perm_to_index(corner_cube, 8, 8);

    for (int i=0; i<7; i++)
        index = index*3+corner_dir[i];

    return index;
}

void Cube::set_corner_index(int index)
{
    int d = 0;
    for (int i=6; i>=0; i--)
    {
        corner_dir[i] = index%3;
        d += corner_dir[i];
        index /= 3;
    }
    corner_dir[7] = (15-d)%3;

    index_to_perm(index, corner_cube, 8, 8);
}

int Cube::get_edge1_index() const
{
    int c[12];
    int d[12];
    for (int i=0; i<12; i++)
    {
        c[edge_cube[i]] = i;
        d[edge_cube[i]] = edge_dir[i];
    }

    int index = perm_to_index(c, 12, EDGE_NUM);

    for (int i=0; i<EDGE_NUM; i++)
        index = index*2+d[i];

    return index;
}

void Cube::set_edge1_index(int index)
{
    int d[12] = {};
    for (int i=EDGE_NUM-1; i>=0; i--)
    {
        d[i] = index%2;
        index /= 2;
    }

    int c[12];
    index_to_perm(index, c, 12, EDGE_NUM);
    bool u[12] = {};
    for (int i=0; i<EDGE_NUM; i++)
        u[c[i]] = true;
    int p = EDGE_NUM;
    for (int i=0; i<12; i++)
        if (!u[i])
            c[p++] = i;

    for (int i=0; i<12; i++)
    {
        edge_cube[c[i]] = i;
        edge_dir[c[i]] = d[i];
    }
}

int Cube::get_edge2_index() const
{
    int c[12];
    int d[12];
    for (int i=0; i<12; i++)
    {
        c[edge_cube[i]] = i;
        d[edge_cube[i]] = edge_dir[i];
    }

    int index = perm_to_index(c+(12-EDGE_NUM), 12, EDGE_NUM);

    for (int i=(12-EDGE_NUM); i<12; i++)
        index = index*2+d[i];

    return index;
}

void Cube::set_edge2_index(int index)
{
    int d[12] = {};
    for (int i=11; i>=(12-EDGE_NUM); i--)
    {
        d[i] = index%2;
        index /= 2;
    }

    int c[12];
    index_to_perm(index, c+(12-EDGE_NUM), 12, EDGE_NUM);
    bool u[12] = {};
    for (int i=(12-EDGE_NUM); i<12; i++)
        u[c[i]] = true;
    int p = 0;
    for (int i=0; i<12; i++)
        if (!u[i])
            c[p++] = i;

    for (int i=0; i<12; i++)
    {
        edge_cube[c[i]] = i;
        edge_dir[c[i]] = d[i];
    }
}

int Cube::get_edge3_index() const
{
    return perm_to_index(edge_cube, 12, 12);
}

void Cube::set_edge3_index(int index)
{
    index_to_perm(index, edge_cube, 12, 12);
}

string Cube::str() const
{
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

    for (int c=0; c<8; c++)
        for (int f=0; f<3; f++)
            cube[corner_pos[c][f].y][corner_pos[c][f].x]
                = corner_str[corner_cube[c]][(f+3-corner_dir[c])%3];
    for (int e=0; e<12; e++)
        for (int f=0; f<2; f++)
            cube[edge_pos[e][f].y][edge_pos[e][f].x]
                = edge_str[edge_cube[e]][(f+2-edge_dir[e])%2];

    string ret;
    for (char *l: cube)
    {
        ret += l;
        ret += "\n";
    }
    return ret;
}

//  Return error description
string Cube::from_str(vector<string> cube)
{
    if (cube.size()<9 ||
        cube[0].size()<6 ||
        cube[1].size()<6 ||
        cube[2].size()<6 ||
        cube[3].size()<12 ||
        cube[4].size()<12 ||
        cube[5].size()<12 ||
        cube[6].size()<6 ||
        cube[7].size()<6 ||
        cube[8].size()<6)
        return "invalid string size";

    //  Support arbitary face colors
    map<char, char> C;
    C[cube[4][4]] = 'F';
    C[cube[4][10]] = 'B';
    C[cube[4][1]] = 'L';
    C[cube[4][7]] = 'R';
    C[cube[1][4]] = 'U';
    C[cube[7][4]] = 'D';
    if (C.size()!=6)
        return "inconsistent centor colors";

    for (int y=0; y<9; y++)
    {
        int xs = y<3 ? 3 : y<6 ? 0 : 3;
        int xe = y<3 ? 6 : y<6 ? 12 : 6;
        for (int x=xs; x<xe; x++)
        {
            if (C.count(cube[y][x])==0)
                return "invalid face color";
            cube[y][x] = C[cube[y][x]];
        }
    }

    int corner_cube[8];
    int corner_dir[8];
    for (int i=0; i<8; i++)
    {
        bool found = false;
        for (int c=0; c<8 && !found; c++)
            for (int d=0; d<3 && !found; d++)
            {
                bool ok = true;
                for (int f=0; f<3; f++)
                    if (cube[corner_pos[i][f].y][corner_pos[i][f].x] != corner_str[c][(f+3-d)%3])
                        ok = false;
                if (ok)
                {
                    corner_cube[i] = c;
                    corner_dir[i] = d;
                    found = true;
                }
            }
        if (!found)
            return "inconsistent corner colors";
    }

    int edge_cube[12];
    int edge_dir[12];
    for (int i=0; i<12; i++)
    {
        bool found = false;
        for (int c=0; c<12 && !found; c++)
            for (int d=0; d<2 && !found; d++)
            {
                bool ok = true;
                for (int f=0; f<2; f++)
                    if (cube[edge_pos[i][f].y][edge_pos[i][f].x] != edge_str[c][(f+2-d)%2])
                        ok = false;
                if (ok)
                {
                    edge_cube[i] = c;
                    edge_dir[i] = d;
                    found = true;
                }
            }
        if (!found)
            return "inconsistent edge colors";
    }

    bool cu[8] = {};
    for (int i=0; i<8; i++)
        cu[corner_cube[i]] = true;
    for (int i=0; i<8; i++)
        if (!cu[i])
            return "inconsistent cornder cubes";

    bool eu[12] = {};
    for (int i=0; i<12; i++)
        eu[edge_cube[i]] = true;
    for (int i=0; i<12; i++)
        if (!eu[i])
            return "inconsistent edge cubes";

    int cs = 0;
    for (int i=0; i<8; i++)
        cs += corner_dir[i];
    if (cs%3!=0)
        return "parity error (corner directions)";

    int es = 0;
    for (int i=0; i<12; i++)
        es += edge_dir[i];
    if (es%2!=0)
        return "parity error (edge directions)";

    int cinv = 0;
    for (int i=0; i<8; i++)
        for (int j=0; j<i; j++)
            if (corner_cube[j]>corner_cube[i])
                cinv++;
    int einv = 0;
    for (int i=0; i<12; i++)
        for (int j=0; j<i; j++)
            if (edge_cube[j]>edge_cube[i])
                einv++;
    if (cinv%2!=einv%2)
        return "parity error (corner and edge inversions)";

    for (int i=0; i<8; i++)
    {
        this->corner_cube[i] = corner_cube[i];
        this->corner_dir[i] = corner_dir[i];
    }
    for (int i=0; i<12; i++)
    {
        this->edge_cube[i] = edge_cube[i];
        this->edge_dir[i] = edge_dir[i];
    }

    return "";
}

const char Cube::corner_str[8][4] = {
    "UFL", "URF", "UBR", "ULB", "DLF", "DFR", "DRB", "DBL",
};
const char Cube::edge_str[12][3] = {
    "UF", "UR", "UB", "UL", "FL", "FR", "BR", "BL", "DF", "DR", "DB", "DL"
};
const Cube::XY Cube::corner_pos[8][3] = {
    {{ 3,  2}, { 3,  3}, { 2,  3}},  // UFL
    {{ 5,  2}, { 6,  3}, { 5,  3}},  // URF
    {{ 5,  0}, { 9,  3}, { 8,  3}},  // UBR
    {{ 3,  0}, { 0,  3}, {11,  3}},  // ULB
    {{ 3,  6}, { 2,  5}, { 3,  5}},  // DLF
    {{ 5,  6}, { 5,  5}, { 6,  5}},  // DFR
    {{ 5,  8}, { 8,  5}, { 9,  5}},  // DRB
    {{ 3,  8}, {11,  5}, { 0,  5}},  // DBL
};
const Cube::XY Cube::edge_pos[12][2] = {
    {{ 4,  2}, { 4,  3}},  // UF
    {{ 5,  1}, { 7,  3}},  // UR
    {{ 4,  0}, {10,  3}},  // UB
    {{ 3,  1}, { 1,  3}},  // UL
    {{ 3,  4}, { 2,  4}},  // FL
    {{ 5,  4}, { 6,  4}},  // FR
    {{ 9,  4}, { 8,  4}},  // BR
    {{11,  4}, { 0,  4}},  // BL
    {{ 4,  6}, { 4,  5}},  // DF
    {{ 5,  7}, { 7,  5}},  // DR
    {{ 4,  8}, {10,  5}},  // DB
    {{ 3,  7}, { 1,  5}},  // DL
};

void Cube::rotate1(int v[], int p0, int p1, int p2, int p3)
{
    int t = v[p0];
    v[p0] = v[p3];
    v[p3] = v[p2];
    v[p2] = v[p1];
    v[p1] = t;
}

void Cube::rotate2(int v[], int p0, int p1, int p2, int p3)
{
    int t = v[p0];
    v[p0] = v[p2];
    v[p2] = t;
    t = v[p1];
    v[p1] = v[p3];
    v[p3] = t;
}

void Cube::rotate3(int v[], int p0, int p1, int p2, int p3)
{
    int t = v[p0];
    v[p0] = v[p1];
    v[p1] = v[p2];
    v[p2] = v[p3];
    v[p3] = t;
}

void Cube::flip_c(int c[], int p0, int p1, int p2, int p3)
{
    c[p0] = (c[p0]+2)%3;
    c[p1] = (c[p1]+1)%3;
    c[p2] = (c[p2]+2)%3;
    c[p3] = (c[p3]+1)%3;
}

void Cube::flip_e(int e[], int p0, int p1, int p2, int p3)
{
    e[p0] = 1-e[p0];
    e[p1] = 1-e[p1];
    e[p2] = 1-e[p2];
    e[p3] = 1-e[p3];
}

/*
e.g. n = 3, k = 2
0 -> 0 1
1 -> 0 2
2 -> 1 0
3 -> 1 2
4 -> 2 0
5 -> 2 1
*/
void Cube::index_to_perm(int index, int perm[], int n, int k)
{
    static const int N = 12;

    int I[N] = {};
    for (int i=0; i<k; i++)
    {
        int t = n-k+i+1;
        I[k-i-1] = index%t;
        index /= t;
    }

    bool U[N] = {};
    for (int i=0; i<k; i++)
        for (int j=0; ; j++)
            if (!U[j])
            {
                if (I[i]==0)
                {
                    U[j] = true;
                    perm[i] = j;
                    break;
                }
                I[i]--;
            }
}

int Cube::perm_to_index(const int perm[], int n, int k)
{
    static const int N = 12;
    static bool init = false;
    static uint8_t popcount[1<<12];
    if (!init)
    {
        init = true;
        for (int i=0; i<1<<12; i++)
        {
            int c = 0;
            for (int j=0; j<12; j++)
                c += i>>j&1;
            popcount[i] = uint8_t(c);
        }
    }

    int I[N] = {};
    int U = 0;
    for (int i=0; i<k; i++)
    {
        I[i] = perm[i]-popcount[U&((1<<perm[i])-1)];
        U |= 1<<perm[i];
    }

    int index = 0;
    for (int i=0; i<k; i++)
        index = index*(n-i)+I[i];

    return index;
}
