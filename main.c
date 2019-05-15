#include <stdio.h>
#define true 1
#define false 0

char temp[5][30][30]; // 처음 맵파일을 읽어와 데이터를 저장하는 변수
char map[5][30][30]; // temp 변수를 가공하여 저장하는 변수


void load_map(void) // 맵파일에서 데이터를 불러와 temp 에 저장하고 가공하여 map 에 저장하는 함수
{
    FILE *ifp;
    /*
     * imap: 맵 번호
     * iy: 맵의 행(Y축) 번호
     * ix: 맵의 열(X축) 번호
     * will_load: 문자 로드 여부를 설정하는 변수
    */
    int imap = 0, iy = 0, ix = 0, c = 0;
    _Bool will_load = false;
    ifp = fopen("map", "r");

    while ((c = getc(ifp)) != EOF)
    {
        switch (c)
        {
            case '1':
                imap = 0;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '2':
                imap = 1;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '3':
                imap = 2;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '4':
                imap = 3;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '5':
                imap = 4;
                ix = 0;
                ix = 0;
                will_load = false;
                break;

            case '\n':
                //아래 if 절은 맵 파일에서 1, 2, 3, 4, 5, n을 걸러줌
                if (!will_load)
                    will_load = true;
                else
                    iy++;
                ix = 0;
                break;

            case 'e':
                goto load_map_end;

            default:
                temp[imap][iy][ix] = c;
                ix++;
                break;
        }
    }

    load_map_end:

    fclose(ifp);

    for (int i1 = 0; i1 <= 4; i1++)
    {
        for (int i2 = 0; i2 <= 29; i2++)
        {
            for (int i3 = 0; i3 <= 29; i3++)
            {
                if (temp[i1][i2][i3] == NULL)
                    map[i1][i2][i3] = 'C';
                else
                    map[i1][i2][i3] = temp[i1][i2][i3];
            }
        }
    }
}


int checkXsize(int imap) // 배열의 X 사이즈를 알아내는 함수
{
    int size = 0, iy = 0;

    for (int ix = 0; ix <= 29; ix++)
    {
        if (map[imap][iy][ix] == 'C')
        {
            size = ix;
            break;
        }
    }

    return size;
}


int checkYsize(int i1, int Xsize) // 배열의 Y 사이즈를 알아내는 함수
{
    int size = 0;
    int ix = Xsize - 1;

    for (int iy = 0; iy <= 29; iy++)
    {
        if (map[i1][iy][ix] == 'C')
        {
            size = iy;
            break;
        }
    }

    return size;
}


int main(void)
{
    load_map();

    char map1[checkXsize(0)][checkYsize(0, checkXsize(0))];
    char map2[checkXsize(1)][checkYsize(1, checkXsize(1))];
    char map3[checkXsize(2)][checkYsize(2, checkXsize(2))];
    char map4[checkXsize(3)][checkYsize(3, checkXsize(3))];
    char map5[checkXsize(4)][checkYsize(4, checkXsize(4))];

    for (int imap = 0; imap <= 4; imap++)
    {
        for (int ix = 0; ix < checkYsize(imap, checkXsize(imap)); ix++)
        {
            for (int iy = 0; iy < checkXsize(imap); iy++)
            {
                switch (imap)
                {
                    case 0:
                        map1[ix][iy] = map[imap][ix][iy];
                        printf("%c", map1[ix][iy]);
                        break;
                    case 1:
                        map2[ix][iy] = map[imap][ix][iy];
                        printf("%c", map2[ix][iy]);
                        break;
                    case 2:
                        map3[ix][iy] = map[imap][ix][iy];
                        printf("%c", map3[ix][iy]);
                        break;
                    case 3:
                        map4[ix][iy] = map[imap][ix][iy];
                        printf("%c", map4[ix][iy]);
                        break;
                    case 4:
                        map5[ix][iy] = map[imap][ix][iy];
                        printf("%c", map5[ix][iy]);
                        break;
                }
            }
            printf("\n");
        }
        printf("\n\n");
    }



    return 0;
}
