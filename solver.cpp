#include <iostream>
#include <cstdlib>
#include <functional>
#include <vector>
#include <algorithm>
#include "cube.h"
using namespace std;

string move_to_str(Move m)
{
    switch (m)
    {
    case Move::F1: return "F";
    case Move::F2: return "F2";
    case Move::F3: return "F'";
    case Move::B1: return "B";
    case Move::B2: return "B2";
    case Move::B3: return "B'";
    case Move::L1: return "L";
    case Move::L2: return "L2";
    case Move::L3: return "L'";
    case Move::R1: return "R";
    case Move::R2: return "R2";
    case Move::R3: return "R'";
    case Move::U1: return "U";
    case Move::U2: return "U2";
    case Move::U3: return "U'";
    case Move::D1: return "D";
    case Move::D2: return "D2";
    case Move::D3: return "D'";
    }
    return "";
}

Move rev(Move m)
{
    int mi = (int)m;
    return Move(mi/3*3+(2-mi%3));
}

uint8_t pattern_c[88179840];
uint8_t pattern_e1[194641920];
uint8_t pattern_e2[194641920];

void make_pattern_database();

long long node_num;
vector<Move> ans;

bool search(Cube &cube, int depth, int prev_move)
{
    node_num++;

    if (cube.is_solved())
        return true;

    int h = 0;
    h = max(h, (int)pattern_c[cube.cd*40320+cube.cp]);
    if (depth<h)
        return false;
    h = max(h, (int)pattern_e1[cube.ep1/7*2048+cube.ed]);
    if (depth<h)
        return false;
    h = max(h, (int)pattern_e2[cube.ep2/7*2048+cube.ed]);
    if (depth<h)
        return false;
    for (int m=0; m<18; m++)
    {
        if (prev_move>=0)
            if (m/3==prev_move/3 ||
                m/3==0 && prev_move/3==1 ||
                m/3==2 && prev_move/3==3 ||
                m/3==4 && prev_move/3==5)
                continue;

        cube.move(Move(m));
        bool ok = search(cube, depth-1, m);
        cube.move(rev(Move(m)));
        if (ok)
        {
            ans.push_back(Move(m));
            return true;
        }
    }
    return false;
};

int main()
{
    Cube::init();
    cout<<"cube init end"<<endl;

    make_pattern_database();
    cout<<"made pattern database"<<endl;

    Cube cube;
    for (int i=0; i<100; i++)
    {
        Move m = Move(rand()%18);
        cube.move(m);
        cout<<" "<<move_to_str(m);
    }
    cout<<endl;
    cout<<cube.str()<<endl;

    ans.clear();
    for (int depth=0; depth<=30; depth++)
    {
        node_num = 0;
        bool r = search(cube, depth, -1);
        cout<<"depth: "<<depth<<", node number: "<<node_num<<endl;
        if (r)
            break;
    }

    reverse(ans.begin(), ans.end());
    for (Move m: ans)
        cout<<" "<<move_to_str(m);
    cout<<endl;

    cout<<cube.str()<<endl;

    return 0;
}

void make_pattern_database()
{
    int cn = 88179840;
    for (int i=0; i<cn; i++)
        pattern_c[i] = 255;
    pattern_c[0] = 0;
    cout<<"c:";
    for (int d=0; ; d++)
    {
        cout<<" "<<d;
        bool up = false;
        for (int c=0; c<cn; c++)
            if (pattern_c[c]==d)
            {
                Cube cube;
                cube.cp = c%40320;
                cube.cd = c/40320;

                for (int m=0; m<18; m++)
                {
                    cube.move(Move(m));
                    int c2 = cube.cd*40320+cube.cp;
                    cube.move(rev(Move(m)));

                    if (pattern_c[c2]==255)
                    {
                        up = true;
                        pattern_c[c2] = d+1;
                    }
                }
            }
        if (!up)
            break;
    }
    cout<<endl;

    int e1n = 194641920;
    for (int i=0; i<e1n; i++)
        pattern_e1[i] = 255;
    pattern_e1[0] = 0;
    cout<<"e1:";
    for (int d=0; ; d++)
    {
        cout<<" "<<d;
        bool up = false;
        for (int e1=0; e1<e1n; e1++)
            if (pattern_e1[e1]==d)
            {
                Cube cube;
                cube.ep1 = e1/2048*7;
                cube.ep2 = Cube::comp_ep(cube.ep1);
                cube.ed = e1%2048;

                for (int m=0; m<18; m++)
                {
                    cube.move(Move(m));
                    int e12 = cube.ep1/7*2048+cube.ed;
                    cube.move(rev(Move(m)));

                    if (pattern_e1[e12]==255)
                    {
                        up = true;
                        pattern_e1[e12] = d+1;
                    }
                }
            }
        if (!up)
            break;
    }
    cout<<endl;

    int e2n = 194641920;
    for (int i=0; i<e2n; i++)
        pattern_e2[i] = 255;
    pattern_e2[((((11*6+6)*10+6)*9+6)*8+6)*2048] = 0;
    cout<<"e2:";
    for (int d=0; ; d++)
    {
        cout<<" "<<d;
        bool up = false;
        for (int e2=0; e2<e2n; e2++)
            if (pattern_e2[e2]==d)
            {
                Cube cube;
                cube.ep2 = e2/2048*7;
                cube.ep1 = Cube::comp_ep(cube.ep2);
                cube.ed = e2%2048;

                for (int m=0; m<18; m++)
                {
                    cube.move(Move(m));
                    int e22 = cube.ep2/7*2048+cube.ed;
                    cube.move(rev(Move(m)));

                    if (pattern_e2[e22]==255)
                    {
                        up = true;
                        pattern_e2[e22] = d+1;
                    }
                }
            }
        if (!up)
            break;
    }
    cout<<endl;
}
